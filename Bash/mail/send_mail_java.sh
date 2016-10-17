#!/bin/bash

export JAVA_HOME="/usr/lib/jvm/jdk1.8.0_45"
export JAVA="$JAVA_HOME/bin/java"
export JAVA_UTILS="/tmp/java-utils-0.0.1-jar-with-dependencies.jar"

if [ "$#" -ne 5 ]; then
	echo "$(basename $0): Illegal number of parameters ($#)"
	echo "send_mail <from> <from_name> <to> <subject> <file_name>"
	exit
fi

FROM=$1
FROM_NAME=$2
RCPT_LIST=$3
SUBJECT=$4
FILENAME=$5

echo "From      : "$FROM
echo "From Name : "$FROM_NAME
echo "RCPT LIST : "$RCPT_LIST
echo "Subject   : "$SUBJECT
echo "File Name : "$FILENAME
echo 

for i in $RCPT_LIST;
do
	echo $i
done    

PARAM="-f $FILENAME $FROM \"$FROM_NAME\" \"$SUBJECT\" $RCPT_LIST"
echo "Param: "
echo $PARAM

$JAVA -cp $JAVA_UTILS io.github.albertopires.mail.Example -f $FILENAME $FROM "$FROM_NAME" "$SUBJECT" $RCPT_LIST

#send_mail <from> <from_name> <to> <subject> <file_name>

#Send Mail/File
#
#Invalid parameters.
#-t : send text message.
#-f : send attached file
#-m : send text message with attached file
#
#-t <msg.txt> <from> <from_name> <subject> <rcpt1> <rcpt2> ... <rcptN>
#-f <file.any_type> <from> <from_name> <subject> <rcpt1> <rcpt2> ... <rcptN>
#-m <msg.txt> <file.any_type> <from> <from_name> <subject> <rcpt1> <rcpt2> ... <rcptN>

