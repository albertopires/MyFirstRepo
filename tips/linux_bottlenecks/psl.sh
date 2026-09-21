#!/bin/bash

sudo ps -L -o f,s,uid,pid,ppid,c,pri,ni,rss,wchan:20,sgi_p,stime,tty,time,cmd -p `pidof $1`

