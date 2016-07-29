#!/bin/bash

#CMD="cat /tmp/usage.txt"
CMD="btrfs fi usa -b $1"

TMP_FILE="btrfs_us_$$"
$CMD > $TMP_FILE

DEVICE_SIZE=`cat $TMP_FILE | awk '/Device size\:/ { print $3 }'`
FREE_ESTIMAT=`cat $TMP_FILE | awk '/Free\ \(estimated\)\:/ { print $3 }'`

echo "Device size    : "$DEVICE_SIZE
echo "Free Estimated : "$FREE_ESTIMAT

PERCENT_FREE=$(awk "BEGIN { printf \"%.2f\",(100-(${FREE_ESTIMAT}/${DEVICE_SIZE})*100) }")
echo "Data : "$PERCENT_FREE

rm -f $TMP_FILE
