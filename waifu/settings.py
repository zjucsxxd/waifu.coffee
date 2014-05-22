# File upload stuff
ALLOWED_FILE_EXTENSIONS = set(['png', 'jpg', 'jpeg', 'gif'])
UPLOAD_DESTINATION = "./waifu/static/uploaded_images/"

# Job runner
RUNNER_URI = "tcp://127.0.0.1:1979"

from local_settings import *
