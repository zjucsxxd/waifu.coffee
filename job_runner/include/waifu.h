#pragma once
#include <kcpolydb.h>
#include <msgpack.hpp>
#include <thread>

using namespace kyotocabinet;
using namespace msgpack;
using namespace std;

// This is how we reference the job queue in Kyoto Cabinet:
#define JOBS_QUEUE_NAME "query_jobs"

// The KC DB where jobs are serialized
#define JOBS_DB "jobs.kct"

// How many threads to spin up for processing jobs:
#define DEFAULT_JOB_PROCESSORS 1

#define DEFAULT_URI "tcp://*:1979"

namespace waifu {
    typedef map<string, string> Job;

    int main_loop(int argc, char *argv[]);

    /* The main worker object.
     * This class is responsible for processing queries, adding new images,
     * etc.
     */
    class worker {
        public:
            worker(Job job);
            ~worker();

            /* Returns a serialized string uniquely identifying this worker. */
            string get_id();
        private:
            Job main_job;
            std::thread main_thread;

            void work();
    };

    /* The main scheduler object.
     * This class is responsible for creating new workers, making sure Jobs
     * are serialized and communicating with the main loop about whats going
     * on.
     */
    class scheduler {
        public:
            scheduler();
            ~scheduler();

            /* Responsible for turning strings like add, query, delete, etc.
             * into useful commands.
             */
            sbuffer *process_request(unpacked *request);
        private:
            /* Jobs database */
            PolyDB db;
            /* Tracks all allocated workers */
            vector<worker *> workers;

            /* Schedules a new job to be worked on. */
            bool new_query(Job new_job);
            /* Processes a job. Returns the string representation of the thread working on the job. */
            string process_query(Job job);
            /* Gets all of the current jobs from the db. */
            vector<Job> *get_jobs_from_db();
    };
}
