#!/bin/bash

echo ""
echo "========================================================================="
echo "Orange Strater Kit"
echo "========================================================================="
echo ""
echo "Command demo : send a 'led' command to the board to turn on or off a led"
echo ""
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"
echo ""

APIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
if [ -z "${APIKEY}" ]
    then
	echo -e "\033[31mERROR: No Api key found.\033[0m"
        echo "Please execute ~/OrangeStarterKit/setup.sh to configure your board"
        exit 1
    else
        echo "Current APIKEY : ${APIKEY}"
        echo ""
fi


deviceID="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info imei')"

namespace="starterkit"

URL="https://liveobjects.orange-business.com/api/v1/deviceMgt/devices/urn:lo:nsid:$namespace:$deviceID/commands"
echo "Live Objects URL : "
echo $URL
echo ""
echo "Request Headers:"
echo "Accept:application/json"
echo "X-API-Key:$APIKEY"
echo ""

command='{"request": {"connector": "mqtt","value": {"req": "led","arg":{}}},"policy": {"expirationInSeconds": 120,"ackMode": "DEVICE"}}'
echo "JSON command as POST:"
echo $command
echo ""

result=$(curl -H "Content-type:application/json" -H "X-API-Key:$APIKEY" -X POST "$URL" -d "$command" 2>/dev/null)

if [[ $result == *"FORBIDDEN"* ]]
	then
		echo -e "\033[31mERROR: Check you APIKEY.\033[0m"
		echo " Device Management 'Writing role' must be check to remotly send commands to the mangOH red"
		echo ""
	else
		echo "Server response:"
		echo $result
		echo ""
		echo ""
		echo "check the middle led bellow the USB port, near the SIM"
		echo ""
fi
