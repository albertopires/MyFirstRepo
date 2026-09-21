#!/bin/bash 

# Sort multiple files in parallel and merge them into a single output

date
export LC_ALL=C
time ls *.gz | parallel --progress -j+1 "gunzip -c {} | sort -n -k2,2 -t$'\xa6' -T /tmp -o /temp/{.}.sorted"
cd /temp
time sort -n -k2,2 -t$'\xa6' -m -T /tmp -o final_sorted_file *.sorted
