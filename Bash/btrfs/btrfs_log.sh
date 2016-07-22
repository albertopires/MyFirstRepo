#!/bin/bash

BASE_DIR=.
PAST_HOUR=$(date +'%b %e %H:[0-9][0-9]:[0-9][0-9] ' -d "1 hour ago")
PAST_HOUR_NAME=$BASE_DIR/out_$(date +'%Y%m%d_%H' -d "1 hour ago").log

echo $PAST_HOUR

if [ "$#" -ne 1 ]; then
	echo "Illegal number of parameters"
	exit
fi

cat $1 | grep -i btrfs | egrep "$PAST_HOUR"  |\
	egrep -i "corrupt|csum" > $PAST_HOUR_NAME
#xz $PAST_HOUR_NAME
