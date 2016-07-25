#!/bin/bash

if [ "$#" -ne 4 ]; then
	echo "Illegal number of parameters"
	echo "btrfs_logs.sh <log_file> <from> <from_name> <to>"
	exit
fi

LOG_FILE=$1
FROM=$2
FROM_NAME=$3
RCPT=$4

BASE_DIR=.
PAST_HOUR=$(date +'%b %e %H:[0-9][0-9]:[0-9][0-9] ' -d "1 hour ago")
PAST_HOUR_NAME=$BASE_DIR/out_$(date +'%Y%m%d_%H' -d "1 hour ago").log

SEND_MAIL_CMD=../mail/send_mail.sh

echo $PAST_HOUR

cat $LOG_FILE | grep -i btrfs | egrep "$PAST_HOUR"  |\
	egrep -i "corrupt|csum" > $PAST_HOUR_NAME

LOG_LINES=`wc -l $PAST_HOUR_NAME | awk '{print $1}'`
echo "Log lines counted: "$LOG_LINES

if [ "$LOG_LINES" -gt 0 ]; then
	echo "Log lines found : "$LOG_LINES
	xz $PAST_HOUR_NAME
	$SEND_MAIL_CMD $FROM "$FROM_NAME" $RCPT $PAST_HOUR_NAME.xz
fi

