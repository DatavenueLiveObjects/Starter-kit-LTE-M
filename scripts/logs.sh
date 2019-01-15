#!/bin/bash

echo ""
echo "========================================================================="
echo "Orange Strater Kit"
echo "========================================================================="
echo ""
echo "MangOH Red board real time logs"
echo ""
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"
echo ""
echo "Waiting for logs..."
ssh root@192.168.2.2 '/sbin/logread -f'

