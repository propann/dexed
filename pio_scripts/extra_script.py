Import("env")

env.Replace(PROGNAME="firmware_MDT_%s_%s" % (env.GetProjectOption("custom_firmware_name"), env.GetProjectOption("custom_firmware_version")))
