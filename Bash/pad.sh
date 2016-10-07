#!/bin/bash

function print_pad {
	STR=$1
	LINE="                                   "
	printf "%s %s [UP]\n" $STR "${LINE:${#STR}}"
}

print_pad "teste"
print_pad "tes"
print_pad "tesrrrr"
print_pad "tesrrrrsdfsdfsdfsdf"
