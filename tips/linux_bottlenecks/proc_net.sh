#!/bin/bash

# Show SOCKET buffers

#sudo lsof -i :3333; sudo netstat -tn -p
sudo ss -n -p '( dport = :3333 or sport = :3333 )'; echo; sudo ss -n -p '( dport = :8080 or sport = :8080 )'; echo; sudo ss -n -p '( dport = :7011 or sport = :7011 )'
#sudo netstat -tn -p
