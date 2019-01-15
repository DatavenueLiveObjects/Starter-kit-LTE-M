#!/bin/bash

echo "==============================================================================="
echo " Orange Starter Kit"
echo "==============================================================================="
echo ""
echo "System check"
echo ""
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"
echo ""


operatorStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio | grep "Current Network"')"
networkStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio | grep "Current RAT"')"

dataConnectionStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data | grep "Connected:"')"

if [ $(echo "$dataConnectionStatus" | cut -c32-32)  = "y" ]
        then
                data="OK"
		operator=$(echo "$operatorStatus" | cut -c32-)
		network=$(echo "$networkStatus=" | cut -c32-42)
		echo "Data connection : $data ($operator - $network)"

        else
                data="KO"
		radio="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio | grep "Power:"')"
		echo "Data connection : $data"
		echo "  " $radio
		echo "  " $operatorStatus
		echo "  " $networkStatus
		echo ""
		echo "Test data connection"
		dataConnectionTest="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect')"

fi

APIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
if [ -z "${APIKEY}" ]
    then
        echo "No Api key found, please execute bash setup.sh to configure your board"
        exit 1
    else
        echo "APIKEY          : OK (${APIKEY})"
fi


mqttClientStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app status mqttClient')"
if [ $(echo "$mqttClientStatus=" | cut -c2-2) = "r" ]
        then
                mqttClient="OK"
        else
                mqttClient="KO"
fi
echo "Client MQTT     : " $mqttClient

starterKitStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app status OrangeStarterKit')"
if [ $(echo "$starterKitStatus" | cut -c2-2) = "r" ]
        then
                starterKit="OK"
        else
                starterKit="KO"
fi
echo "Starter Kit     : " $starterKit


