#!/bin/bash

echo "==============================================================================="
echo " Orange Starter Kit"
echo "==============================================================================="
echo ""

echo "Now plug your mangOH Red board"
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo ""
echo "Check application status"
appStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app status OrangeStarterKit')"
if [[ $appStatus == *"stopped"* ]]
    then
        echo "Application is not running. Starting..."
        appStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app start OrangeStarterKit')"
        sleep 2
   else 
        forceRestart=appStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app restart OrangeStarterKit')"
	sleep 2
fi


timer=1

while :
do
appStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app status OrangeStarterKit')"
if [[ $appStatus == *"running"* ]]
    then
        echo -e "\033[1;32mApplication is running.\033[0m"
	break
    else
	printf "%c" "."
        if [[ $timer -gt "10" ]] 
                then
        		echo -e "\033[31mERROR: Application failed to start.\033[0m"
                        echo "Check board logs."
                        exit 1
        fi
        sleep 2
        timer=$timer+1

	sleep 1
fi

done


echo "Check Live Objects status"

APIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
IMEI="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info imei')"
namespace="starterkit"
URL="https://liveobjects.orange-business.com/api/v1/deviceMgt/devices/urn%3Alo%3Ansid%3A$namespace%3A$IMEI"

timer=1
while :
do
result=$(curl -X GET --header 'Accept: application/json' --header "X-API-KEY:$APIKEY" "$URL" 2>/dev/null)
#echo $result

if [[ $result != *"ONLINE"* ]]
    then
	printf "%c" ".$timer"
        if [[ $timer -gt "10" ]] 
                then
                        echo -e "\033[31mERROR: Application connection to Live Object failed.\033[0m"
			echo "Check you APIKEY. Device MQTT read/Write mode must be check to enable the device connection to Live Objects."
                        exit 1
        fi
	sleep 2
	timer=$timer+1

    else
	echo -e "\033[1;32mApplication is online.\033[0m"
	echo ""
	exit 0
fi
timer=$(($timer+1))

done
