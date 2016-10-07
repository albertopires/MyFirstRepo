#!/bin/bash

if [ "$#" -ne 2 ]; then
	echo "Illegal number of parameters"
	echo "$0 <filesystem> <filesystem_label>"
	echo 
	echo "Filesystem Label MUST NOT contain \"/\" or spaces"
	exit
fi

##############################################################################
# Define verbosity, possible values are: 
# LOG_SILENT=0
# LOG_VERBOSE=1
# LOG_DEBUG=2
VERBOSE=2

# Usage Threshold
US_THRESHOLD=80

# Directory where processed logs will be stored.
LOG_DIR="/tmp/btrfs_logs"
##############################################################################

BASEDIR=$(dirname $0)
. $BASEDIR/lib.sh

LOG_FILE=$LOG_DIR/fs_usage_$2_$(date +'%Y%m%d_%H%M').log

CMD="btrfs fi usa -b $1"
CMD_H="btrfs fi usa $1"

TMP_FILE="btrfs_us_$$"
$CMD > $TMP_FILE

DEVICE_SIZE=`cat $TMP_FILE | awk '/Device size\:/ { print $3 }'`
FREE_ESTIMAT=`cat $TMP_FILE | awk '/Free\ \(estimated\)\:/ { print $3 }'`

print_log $LOG_VERBOSE "Device size    : "$DEVICE_SIZE
print_log $LOG_VERBOSE "Free Estimated : "$FREE_ESTIMAT

PERCENT_USED=$(awk "BEGIN { printf \"%.2f\",(100-(${FREE_ESTIMAT}/${DEVICE_SIZE})*100) }")
PERCENT_USED_INT=$(awk "BEGIN { printf \"%.0f\",(100-(${FREE_ESTIMAT}/${DEVICE_SIZE})*100) }")
print_log $LOG_VERBOSE "Percent USED : "$PERCENT_USED

rm -f $TMP_FILE

if [ "$PERCENT_USED_INT" -gt $US_THRESHOLD ]; then
	print_log $LOG_DEBUG "Disk space alarm : "$PERCENT_USED"%"
	$CMD_H > $LOG_FILE
fi
