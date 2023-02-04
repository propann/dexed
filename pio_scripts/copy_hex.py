# # import os
# import shutil

# # Import("env")

# # os.rename("path/to/current/file.foo", "path/to/new/destination/for/file.foo")
# # os.replace("path/to/current/file.foo", "path/to/new/destination/for/file.foo")
# shutil.copy2(PROGNAME, "../../../MicroDexed-touch-Firmware/")


Import("env", "projenv")
from shutil import copyfile
import os

def save_hex(*args, **kwargs):
    print("Copying hex output to project directory...")
    target = str(kwargs['target'][0])
    print("hex file: " + os.path.basename(target))

    copyfile(target, 'release/' + os.path.basename(target))
    print("Done.")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", save_hex)   #post action for .hex