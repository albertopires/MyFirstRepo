#!/bin/bash

LOG_SILENT=0
LOG_VERBOSE=1
LOG_DEBUG=2

function print_log {
#echo "Verbose: "$VERBOSE
#echo "LEVEL  : "$1
	if (($VERBOSE >= $1)); then
		echo "$2"
	fi
}

