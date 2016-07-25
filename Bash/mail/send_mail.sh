#!/bin/bash

if [ "$#" -ne 4 ]; then
	echo "Illegal number of parameters"
	echo "send_mail <from> <from_name> <to> <file_name>"
	exit
fi

FROM=$1
FROM_NAME=$2
RCPT=$3
FILE_NAME=$4


(printf "%s\n" \
	"Subject: test-001" \
	"From: $FROM_NAME <$FROM>" \
	"To: $RCPT" \
	"Content-Type: application/x-xz" \
	"Content-Disposition: attachment; filename=$(basename $FILE_NAME)" \
	"Content-Transfer-Encoding: base64" \
	"";
base64 $FILE_NAME) | sendmail -f "$FROM" "$RCPT"

