#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Illegal number of parameters"
	echo "$0 <filesystem>"
	exit
fi

##############################################################################
# Define verbosity, possible values are: 
# LOG_SILENT=0
# LOG_VERBOSE=1
# LOG_DEBUG=2
VERBOSE=1
##############################################################################

BASEDIR=$(dirname $0)
. $BASEDIR/lib.sh

CMD="cat /tmp/usage.txt"
#CMD="btrfs fi usa -b $1"

TMP_FILE="btrfs_us_$$"
$CMD > $TMP_FILE

DEVICE_SIZE=`cat $TMP_FILE | awk '/Device size\:/ { print $3 }'`
FREE_ESTIMAT=`cat $TMP_FILE | awk '/Free\ \(estimated\)\:/ { print $3 }'`

print_log $LOG_VERBOSE "Device size    : "$DEVICE_SIZE
print_log $LOG_VERBOSE "Free Estimated : "$FREE_ESTIMAT

PERCENT_FREE=$(awk "BEGIN { printf \"%.2f\",(100-(${FREE_ESTIMAT}/${DEVICE_SIZE})*100) }")
PERCENT_FREE_INT=$(awk "BEGIN { printf \"%.0f\",(100-(${FREE_ESTIMAT}/${DEVICE_SIZE})*100) }")
print_log $LOG_VERBOSE "Data : "$PERCENT_FREE

rm -f $TMP_FILE

if [ "$PERCENT_FREE_INT" -gt 70 ]; then
	print_log $LOG_DEBUG "Disk space alarm : "$PERCENT_FREE"%"
fi
