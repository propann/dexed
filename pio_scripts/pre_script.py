Import("env")
import sys, re, datetime

PATH_VERSION = './MicroDexed-touch/version.h'
MAJOR, MINOR, PATCH, BUILD = 0, 1, 2, 3

# Read
with open(PATH_VERSION, 'r') as reader:
    # Find "MAJOR.MINOR.PATCH+BUILD" from the first line
    line = re.search(r'"([^"]*)"', reader.readline()).group()[1:-1]
    # Extract old values for MAJOR.MINOR.PATCH+BUILD
    versions = re.split('\.|b', line)
    # version = '%s.%s.%sb%s' % (versions[MAJOR], versions[MINOR], versions[PATCH], versions[BUILD])
    version = '%s_%s_%s' % (versions[MAJOR], versions[MINOR], versions[PATCH])

    env.Replace(PROGNAME="mdt_%s_%s" % (env.GetProjectOption("custom_firmware_name"), version))
