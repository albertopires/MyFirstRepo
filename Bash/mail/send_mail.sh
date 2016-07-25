#!/bin/bash

if [ "$#" -ne 5 ]; then
	echo "$(basename $0): Illegal number of parameters ($#)"
	echo "send_mail <from> <from_name> <to> <subject> <file_name>"
	exit
fi

FROM=$1
FROM_NAME=$2
RCPT=$3
SUBJECT=$4
FILE_NAME=$5


(printf "%s\n" \
	"Subject: $SUBJECT" \
	"From: $FROM_NAME <$FROM>" \
	"To: $RCPT" \
	"Content-Type: application/x-xz" \
	"Content-Disposition: attachment; filename=$(basename $FILE_NAME)" \
	"Content-Transfer-Encoding: base64" \
	"";
base64 $FILE_NAME) | sendmail -f "$FROM" "$RCPT"

