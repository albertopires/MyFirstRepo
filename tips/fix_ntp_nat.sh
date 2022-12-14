#!/bin/bash

# Use UDP source port masquerade to by pass ISP provider block of UDP port 123
iptables -t nat -I POSTROUTING -p udp -m udp --sport 123 -j MASQUERADE --to-ports 60000-61000
