#!/bin/bash
#
# Converter for WAV to C Header
# Used for MicroDexed sampler
#
# (C)2021 Holger Wirtz
#

SOX=`which sox`
XXD=`which xxd`
TMP="/tmp/wav2c"
AUDIO_BLOCK_SIZE=128
DRUMSET_H="drumset.h"
DRUMS_H="${TMP}/drums.h"

function cleanexit()
{
	#rm -rf "${TMP}"
	echo -n ""
}

trap cleanexit EXIT

if [ -z "${SOX}" ]
then
	echo "Cannot find 'sox' in your PATH." >&2
	exit 100
fi

if [ -z "${XXD}" ]
then
	echo "Cannot find 'xxd' in your PATH." >&2
	exit 101
fi

POSITIONAL=()
while [[ $# -gt 0 ]]; do
  key="$1"

  case $key in
    -c|--config)
      CONFIG="$2"
      shift # past argument
      shift # past value
      ;;
    -w|--wavs)
      WAV_DIR="$2"
      shift # past argument
      shift # past value
      ;;
#    -t|--test)
#      TEST="true"
#      shift # past argument
#      ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
      ;;
  esac
done

if [ ! -f "${CONFIG}" ]
then
	echo "Cannot find configuration file." >&2
	exit 200
fi

if [ ! -d "${WAV_DIR}" ]
then
	echo "Cannot find folder for WAV files." >&2
	exit 201
fi

if (( `ls "${WAV_DIR}" | wc -l` == 0 ))
then
	echo "Cannot find any file in ${WAV_DIR}." >&2
	exit 202
fi

mkdir -p "${TMP}"
cat >> "${DRUMSET_H}" << EOF
#ifndef _DRUMSET_H
#define _DRUMSET_H

#include "drums.h"

EOF
rm -f "${DRUMS_H}"
cat >> "${DRUMS_H}" << EOF
drum_config_t drum_config[NUM_DRUMSET_CONFIG] =
{
EOF

declare -A converted_files
NUM_DRUMSET_CONFIG=1

while IFS= read -r l
do
	declare -A sample
	
	if [[ "${l:0:1}" == "#" ]]
	then
		continue
	fi
	IFS=',' read -ra samplecfg <<< "${l}"

	sample['class']=`echo "${samplecfg[0]}" | xargs`
	sample['midinote']=`echo "${samplecfg[1]}" | xargs`
	sample['name']=`echo "${samplecfg[2]}" | xargs`
	sample['c_name']=`echo "${samplecfg[2]}" | xargs | sed 's/[^a-zA-Z0-9]/_/g'`
	sample['c_name']=`echo "DRUM_${sample[c_name]}"`
	sample['shortname']=`echo "${samplecfg[3]}" | xargs`
	sample['pitch']=`echo "${samplecfg[4]}" | xargs`
	sample['p_offset']=`echo "${samplecfg[5]}" | xargs`
	sample['pan']=`echo "${samplecfg[6]}" | xargs`
	sample['vol_max']=`echo "${samplecfg[7]}" | xargs`
	sample['vol_min']=`echo "${samplecfg[8]}" | xargs`
	sample['reverb_send']=`echo "${samplecfg[9]}" | xargs`
	sample['filename']=`echo "${samplecfg[10]}" | xargs`

	if [ -f "${WAV_DIR}/${sample['filename']}" ]
	then
		file "${WAV_DIR}/${sample['filename']}" | grep -i -q "WAVE audio"
		if [ "${?}" == 0 ]
		then
			# Generate drumset.h

			echo "'${WAV_DIR}/${sample['filename']}' -> '${sample['c_name']}'"
			NUM_DRUMSET_CONFIG=`expr "${NUM_DRUMSET_CONFIG}" + 1` 

			if [ -z "${converted_files[${sample['filename']}]}" ]
			then
				basename=`echo "${sample['filename']}" | cut -d'.' -f1`
        			sox "${WAV_DIR}/${sample['filename']}" -c 1 -b 16 -L "${TMP}/${basename}.raw"
				xxd -i "${TMP}/${basename}.raw" > "${TMP}/${basename}.h"
				sample['len']=`grep "^unsigned int" "${TMP}/${basename}.h" | cut -d"=" -f2 | sed 's/\s*\([0-9]\+\);/\1/'`
				fill_mod=`expr "${sample['len']}" % "${AUDIO_BLOCK_SIZE}"`
				fill=`expr "${AUDIO_BLOCK_SIZE}" - "${fill_mod}"`
				sample['len']=`expr "${sample['len']}" + "${fill}"`
				sample['len']=`expr "${sample['len']}" / 2`

				echo "// Converted from ${sample['filename']}, length = ${sample['len']} bytes" >> "${DRUMSET_H}"
				echo "PROGMEM const uint8_t ${sample['c_name']}[] = {" >> "${DRUMSET_H}"
				grep "^ " "${TMP}/${basename}.h" >> "${DRUMSET_H}"

				if (( "${fill}" > 0 ))
				then
					echo -n "," >> "${DRUMSET_H}"
					fill_counter=0
					for i in $(seq 1 "${fill}")
					do
						echo -n "0x00" >> "${DRUMSET_H}"
						let fill_counter+=1
						if (( "${fill_counter}" >= 8 ))
						then
							echo "," >> "${DRUMSET_H}"
							fill_counter=0
						else
							echo -n ", " >> "${DRUMSET_H}"
						fi
					done
				fi
				echo "};" >> "${DRUMSET_H}"

				converted_files["${sample['filename']}"]="${sample['c_name']},${sample['len']}"
			else
				sample['c_name']=`echo ${converted_files[${sample['filename']}]} | cut -d"," -f1`
				sample['len']=`echo ${converted_files[${sample['filename']}]} | cut -d"," -f2`
			fi	

			# Generate drums.h
			cat >> "${DRUMS_H}" << EOF
  {
    ${sample[class]},
    ${sample[midinote]},
    "${sample[name]}",
    ${sample[c_name]},
    "${sample[shortname]}",
    ${sample[len]},
    ${sample[pitch]},
    ${sample[p_offset]},
    ${sample[pan]},
    ${sample[vol_max]},
    ${sample[vol_min]},
    ${sample[reverb_send]}
  },
EOF
		fi
	else
		echo "File \'${WAV_DIR}/${sample['filename']}\' does not exits." >&2
	fi
done < "${CONFIG}"

cat >> "${DRUMS_H}" << EOF
  {
    DRUM_NONE,
    0,
    "EMPTY",
    NULL,
    "-",
    0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0
  }
};

#endif
EOF

echo "" >> "${DRUMSET_H}"
cat "${DRUMS_H}" >> "${DRUMSET_H}"

echo ""
echo "Created drumset.h. Copy the file to the root dicrectory of MicroDexed and don't"
echo "forget to edit config.h and change the following line to the right number:"
echo ""
echo "		#define NUM_DRUMSET_CONFIG ${NUM_DRUMSET_CONFIG}"
echo ""
