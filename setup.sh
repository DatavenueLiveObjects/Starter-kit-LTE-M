#!/bin/bash

echo "==============================================================================="
echo " Orange Starter Kit"
echo "==============================================================================="
echo ""
echo "This application requires a mangOH Red board, with the Sierra Wireless module WP7702 and an Orange Live Objects Apikey."
echo ""
echo "Now plug your mangOH Red board"
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"

device="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info device')"

if [ $device == "WP7702" ]
    then
        echo "Module is a WP7702"
    else
        echo "Device is not a WP7702"
	echo "Installation stop"
        exit 1
fi

echo "Configuring shell for Legato development"
echo "----------------------------------------"
pushd /home/mangoh/legato_framework/legato > /dev/null
source bin/configlegatoenv
popd > /dev/null

echo ""
echo "==============================================================================="
echo "Check board firmware"
version1="$(ssh root@192.168.2.2 '/legato/systems/current/bin/legato version'| \
cut -c1-2)"
version2="$(ssh root@192.168.2.2 '/legato/systems/current/bin/legato version'| \
cut -c4-5)"
version="$version1$version2"
echo "Legato version found : $version"

if [ $version -lt "1809"  ]
    then
        echo "Update required"
        update ~/mangOH/build/update_files/red/mangOH.wp77xx.update 192.168.2.2
    	sleep 3
	echo "Waiting for the mangOH Red"
	while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
	do
	    printf "%c" "."
	done
    else
        echo "No update required"

fi
echo ""

echo "==============================================================================="
echo "Network configuration"
echo "==============================================================================="

MNO="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm sim imsi'| cut -c32-36)"

if [ -z "$MNO" ]
	then
	echo "Unable to get SIM informations"
	echo "check your SIM card and retry"
	echo "exit setup"
	exit 1
fi

if [ $MNO = "20801" ]
	then
                echo "Configuration for Orange France"
                APN="orange.ltem.spec"
                auth="pap"
                username="orange"
                password="orange"
        else
                echo "Unkown Network Operator"
                echo "enter your connection settings"
                echo "APN : "
                read APN
                echo "Username :"
                read username
                echo "Password :"
                read password
                echo "Authentification mode : <none/pap/chap>"
                read auth
fi

ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect -1'

ssh root@192.168.2.2 '/legato/systems/current/bin/cm data profile 1'
command="/legato/systems/current/bin/cm data apn"
ssh root@192.168.2.2 $command $APN
command="/legato/systems/current/bin/cm data auth"
ssh root@192.168.2.2 $command $auth $username $password

ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio off'
sleep 2
ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on'
ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect'



echo "done"
echo ""



echo "==============================================================================="
echo "Live Objects configuration"
echo "==============================================================================="
echo "If you don't have your api key yet :"
echo "  - connect to Live Objects https://liveobjects.orange-business.com/#/login"
echo "  - select the configuration menu and then Api Keys"
echo "  - add a new key, select the  profile : Mqtt Device"

currentAPIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"
if [ -z "${currentAPIKEY}" ]
    then
        echo "enter your Api key "
	read APIKEY
	command="/legato/systems/current/bin/config set /LiveObjects/apiKey"
	ssh root@192.168.2.2 $command $APIKEY

    else
        echo "Previous APIKEY found : ${currentAPIKEY}"
        echo "Replace ? y / n"
        read updateAPIKEYControl
        if [ "$updateAPIKEYControl" = "y"  ]
            then
            	echo "enter your Api key "
        	read APIKEY
        	command="/legato/systems/current/bin/config set /LiveObjects/apiKey"
        	ssh root@192.168.2.2 $command $APIKEY
        fi
fi

echo "==============================================================================="
echo "Install mqttClient"
echo "==============================================================================="
cd ~/mqttClient-for-Legato/mqttClientApi/
make clean

echo "build mqttClient"
make wp77xx

build_result=$?

if [ $build_result -eq 0 ]; then
        echo "upload to the mangOH red"
        update mqttClient.wp77xx.update 192.168.2.2
        sleep 3
else
        echo "==============================================================================="
        echo "mqttClient build failed"
        exit 1
fi

echo "==============================================================================="
echo "Install OrangeStarterKit"
echo "==============================================================================="

cd ~/OrangeStarterKit/
echo "build Orange StarterKit"
make wp77xx

build_result=$?
if [ $build_result -eq 0 ] 
   then
	echo "upload to the mangOH red"
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
    	echo "OrangeStarter kit build failed"
    	exit 1
fi
