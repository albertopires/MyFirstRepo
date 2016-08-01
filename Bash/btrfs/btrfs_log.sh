#!/bin/bash

if [ "$#" -ne 4 ]; then
	echo "Illegal number of parameters"
	echo "btrfs_logs.sh <log_file> <from> <from_name> <to>"
	exit
fi

##############################################################################
# Define verbosity, possible values are: 
# LOG_SILENT=0
# LOG_VERBOSE=1
# LOG_DEBUG=2
VERBOSE=0

# Directory where processed logs will be stored.
LOG_DIR="/tmp/btrfs_logs"

# Send logs via email.
# 0 - Disable
# 1 - Enable
SEND_MAIL=1

# Script used to send emails
SEND_MAIL_CMD="../mail/send_mail.sh"
##############################################################################

LOG_FILE=$1
FROM=$2
FROM_NAME=$3
RCPT=$4
BASEDIR=$(dirname $0)

. $BASEDIR/lib.sh

PAST_HOUR=$(date +'%b %e %H:[0-9][0-9]:[0-9][0-9] ' -d "1 hour ago")
PAST_HOUR_NAME=$LOG_DIR/out_$(date +'%Y%m%d_%H' -d "1 hour ago").log

print_log $LOG_DEBUG "Hour regex: $PAST_HOUR"
print_log $LOG_DEBUG "LOG_DIR   : $LOG_DIR"

SUBJECT="BTRFS errors found : $(basename $PAST_HOUR_NAME).xz"

cat $LOG_FILE | grep -i btrfs | egrep "$PAST_HOUR"  |\
	egrep -i "corrupt|csum" > $PAST_HOUR_NAME

LOG_LINES=`wc -l $PAST_HOUR_NAME | awk '{print $1}'`
print_log $LOG_DEBUG "Log lines counted: "$LOG_LINES

if [ "$LOG_LINES" -gt 0 ]; then
	print_log $LOG_VERBOSE "Log lines found : "$LOG_LINES
	xz $PAST_HOUR_NAME
	if [ "$SEND_MAIL" -ne 0 ]; then
		print_log $LOG_DEBUG "Sending email..."
		$SEND_MAIL_CMD $FROM "$FROM_NAME" $RCPT "$SUBJECT" $PAST_HOUR_NAME.xz
	fi
fi

