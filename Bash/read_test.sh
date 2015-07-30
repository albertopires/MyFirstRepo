#!/bin/bash

read -s -p "Enter password: " password

echo
echo "Password : "$password
echo
echo "---------------"

sshpass -p $password ssh -C -p6798 simade@192.168.40.10
