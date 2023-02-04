from shutil import copyfile
import os

Import("env", "projenv")

## Copy HEX in release folder
def save_hex(*args, **kwargs):
    print("Copying hex output to project directory...")
    target = str(kwargs['target'][0])
    copyfile(target, 'release/' + os.path.basename(target))
    print("Done.")

env.AddPostAction("$BUILD_DIR/${PROGNAME}.hex", save_hex)   #post action for .hex
