#!/bin/bash

COLOR="green"
TARGET_EXTENSION="jpg"

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -i|--imgpath) IMG_PATH="${2}"; shift ;;
        -1|--line1) line[0]="${2}"; shift ;;
        -2|--line2) line[1]="${2}"; shift ;;
        -t|--target) TARGET="${2}"; shift ;;
        -b|--baseimg) BASE_IMG="${2}"; shift ;;
        *) echo "Unknown parameter passed: $1"; exit 1 ;;
    esac
    shift
done

if [ ! -e "${IMG_PATH}" ]
then
	echo "Cannot find path for images \"${IMG_PATH}\""
	exit 10
fi

CHAR_PATH="${IMG_PATH}/LCD_characters_${COLOR}"

if [ ! -e "${CHAR_PATH}" ]
then
	echo "Cannot find path for character-images \"${CHAR_PATH}\""
	exit 11
fi

if [ ${BASE_IMG} ]
then
	cp "${IMG_PATH}/manual-images/${BASE_IMG}.png" /tmp/LCD_$$.png
else
	cp "${IMG_PATH}/LCD_green.png" /tmp/LCD_$$.png
fi

special_flag="false"
for (( n=0; n<2; n++ ))
do
	y=$( bc -l <<<"50 + $n * 50" )
	xpos=0
	for (( i=0; i<${#line[$n]}; i++ ))
	do
		c=${line[${n}]:${i}:1}
		if [[ "${c}" == "%" ]]
		then
			if [[ "${special_flag}" == "false" ]]
			then
				special_flag="true"
				special=""
				continue
			else
				special_flag="false"
				if [ ! -e "${CHAR_PATH}/${special}.png" ]
				then
					echo "Cannot find \"${CHAR_PATH}/${special}.png\"."
					c=" "
				else
					c=${special}
				fi
			fi
		fi
		if [[ "${special_flag}" == "true" ]]
		then
			special="${special}${c}"
			continue
		fi
		xpos=$((xpos + 1))
		if [[ "${c}" != " " ]]
		then
			if [[ ${c} == "." ]]
			then
				c="dot"
			fi
			x=$( bc -l <<<"230 + ${xpos} * 30" )
			composite -gravity NorthWest -geometry "+${x}+${y}" "${CHAR_PATH}/${c}.png" /tmp/LCD_$$.png /tmp/LCD_$$.png
		fi
	done
done

convert /tmp/LCD_$$.png /tmp/LCD_$$.${TARGET_EXTENSION}

if [ "${TARGET}" ]
then
	mv /tmp/LCD_$$.${TARGET_EXTENSION} "${TARGET}"
else
	echo "Created /tmp/LCD_$$.${TARGET_EXTENSION}"
fi
