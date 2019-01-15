#!/bin/bash

echo ""
echo "========================================================================="
echo "Orange Strater Kit"
echo "========================================================================="
echo ""
echo "This will remove The Orange starter kit application"
echo "Confirm ? y/n"
read confirm


if [ $confirm = "y" ]
	then 
	APIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
	echo "Previous APIKEY : " $APIKEY
	ssh root@192.168.2.2 '/legato/systems/current/bin/config delete /LiveObjects/apiKey'
	ssh root@192.168.2.2 '/legato/systems/current/bin/app remove OrangeStarterKit'
	ssh root@192.168.2.2 '/legato/systems/current/bin/app remove mqttClient'
	echo "done"
	exit 0
	else
		echo "Reset cancelled"
		exit 1
fi 
