#!/bin/bash

echo "==============================================================================="
echo 'Setup Orange Starter Kit'
echo "==============================================================================="

if [ -z "$1" ]
	then
		echo "ERROR : No APIKEY found"
		echo "use the following command to setup the starter kit"
		echo "bash setup.sh <your apikey>"
		exit 1
fi

echo "Waiting for the mangOH Red"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"

device="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info device')"

if [ $device == "WP7702" ]
    then
        echo "Device is WP7702"
    else
        echo "Device is not a WP7702"
        exit 1
fi

currentAPIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
if [ -z "${currentAPIKEY}" ]
    then
        APIKEY="$1"
    else
        echo "Previous APIKEY found : ${currentAPIKEY}"
        echo "Replace with $1 ?"
        echo " y / n"
        read updateAPIKEYControl
        if [ "$updateAPIKEYControl" = "y"  ]
            then 
                APIKEY="$1"
            else
                APIKEY="${currentAPIKEY}"
        fi
fi

echo "APIKEY : ${APIKEY}"

echo "Configuring shell for Legato development"
echo "----------------------------------------"
pushd /home/mangoh/legato_framework/legato > /dev/null
source bin/configlegatoenv
popd > /dev/null

echo "==============================================================================="
echo "Check board firmware"
echo "==============================================================================="
checkLedService="$(ssh root@192.168.2.2 '/legato/systems/current/bin/app status ledService' | cut -c2-14)" 

if [ "$checkLedService" = "not installed" ]
    then
        echo "Update required"
        update ~/mangOH/build/update_files/red/mangOH.wp77xx.update 192.168.2.2
    else
        echo "No update required"    
fi

sleep 3
echo "Waiting for the mangOH Red"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo "==============================================================================="
echo "Set APIKEY"
echo "==============================================================================="

command='/legato/systems/current/bin/config set /LiveObjects/apiKey'
ssh root@192.168.2.2 $command $APIKEY

echo "==============================================================================="
echo "Install mqttClient"
echo "==============================================================================="
cd ~/mqttClient-for-Legato/mqttClientApi/
make clean

echo 'build mqttClient'
make wp77xx

build_result=$?

if [ $build_result -eq 0 ]; then
        echo 'upload to the mangOH red'
        update mqttClient.wp77xx.update 192.168.2.2
        sleep 3
else
        echo "==============================================================================="
        echo 'mqttClient build failed'
        exit 1
fi

echo "==============================================================================="
echo "Install OrangeStarterKit"
echo "==============================================================================="

cd ~/OrangeStarterKit/
echo 'build Orange StarterKit'
make wp77xx

build_result=$?

if [ $build_result -eq 0 ]; then
	echo 'upload to the mangOH red'
	update OrangeStarterKit.wp77xx.update 192.168.2.2
    sleep 3
    ssh root@192.168.2.2 '/legato/systems/current/bin/app start OrangeStarterKit'
    echo "==============================================================================="
    echo "Setup successful"
    echo "Connect to LiveObjects to see your datas"
    echo "check logs with the following command : ssh root@192.168.2.2 '/sbin/logread -f'"
    exit 0
else
    echo "==============================================================================="
	echo 'OrangeStarter kit build failed'
    exit 1
fi
