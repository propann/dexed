#!/bin/bash

WAV2SKETCH="${HOME}/Arduino-Teensy/MicroDexed/third-party/wav2sketch"

if [ ! -e "${WAV2SKETCH}" ]
then
	gcc "${HOME}/Arduino-Teensy/MicroDexed/third-party/wav2sketch.c" -o "${WAV2SKETCH}"
	chmod 700 "${WAV2SKETCH}"
fi

"${WAV2SKETCH}" -16

rm *.h
rm -f drumset.h
touch drumset.h

for i in `ls -1 *.cpp`
do
	echo "# ${i}" >> drumset.h
	cat "${i}" | grep '^[A-Za-z0-9]' >> drumset.h
	echo "" >> drumset.h
	rm "${i}"
done
