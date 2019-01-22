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
        echo "Module is a WP7702."
    else
        echo -e "\033[1;31mDevice is not a WP7702.\033[0m"
	echo "Installation stop."
        exit 1
fi

echo ""
echo "==============================================================================="
echo "Configuring shell for Legato development..."
echo "==============================================================================="
pushd /home/mangoh/legato_framework/legato &>/dev/null
source bin/configlegatoenv &>/dev/null
popd &>/dev/null

echo ""
echo "==============================================================================="
echo "Check board firmware"
echo "==============================================================================="
legatoBoardVersion=$(ssh root@192.168.2.2 '/legato/systems/current/bin/legato version' | cut -c1-52)
legatoVMVersion=$(cat /home/mangoh/mangOH/build/red_wp77xx/staging/version)

if [ "$legatoBoardVersion" != "$legatoVMVersion" ]
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

bash scripts/network.sh
if [[ $? -eq 1 ]]
        then
                echo -e "\033[1;31mConnection to network failed, abort setup.\033[0m"
                exit 1
fi
echo ""

bash scripts/liveObjects.sh
if [[ $? -eq 1 ]]
        then
                echo -e "\033[1;31mFailed to configure Live Objects, abort setup.\033[0m"
                exit 1
fi
echo ""
echo "==============================================================================="
echo "Install mqttClient"
echo "==============================================================================="
cd ~/mqttClient-for-Legato/mqttClientApi/
make clean &>/dev/null

echo "Build mqttClient"
mqtt=$(make wp77xx &>/dev/null)

build_result=$?

if [ $build_result -eq 0 ]; then
        echo "Upload to the mangOH red"
        update mqttClient.wp77xx.update 192.168.2.2
        sleep 3
else
        echo "==============================================================================="
        echo -e "\033[1;31mmqttClient build failed\033[0m"
        exit 1
fi

echo ""
echo "==============================================================================="
echo "Install OrangeStarterKit"
echo "==============================================================================="

cd ~/OrangeStarterKit/
echo "Build Orange StarterKit"
make clean &>/dev/null
make wp77xx &>/dev/null

build_result=$?
if [ $build_result -eq 0 ] 
   then
	echo "Upload to the mangOH red"
	update OrangeStarterKit.wp77xx.update 192.168.2.2
    else
    	echo "==============================================================================="
    	echo -e "\033[1;31mOrangeStarter kit build failed\033[0m"
    	exit 1
fi


sleep 3
#ssh root@192.168.2.2 '/legato/systems/current/bin/app restart OrangeStarterKit'

chmod +x scripts/led.sh
chmod +x scripts/liveObjects.sh
chmod +x scripts/checkApiKey.sh
chmod +x scripts/logs.sh
chmod +x scripts/systemCheck.sh
chmod +x scripts/appStatus.sh


#check app is running
bash scripts/appStatus.sh
if [[ $? -eq 1 ]]
        then
                echo -e "\033[1;31mApplication failed to join Live Objects, abort setup.\033[0m"
                exit 1
fi


IMEI="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info imei')"

echo ""
echo "==============================================================================="
echo -e "\033[1;32mSetup successful\033[0m"
echo "==============================================================================="
echo ""
echo -e "Connect to LiveObjects to see your datas:"
echo -e "  In the Devices menu, a new device is created with the name \033[32m'Starter Kit'\033[0m and the ID \033[32m'urn:lo:nsid:starterkit:$IMEI'\033[0m"
echo ""
echo -e "\033[4;37mCheck board's logs with the following command:\033[0m"
echo "	~/OrangeStarterKit/scripts/logs.sh"
echo ""
echo -e "\033[4;37mYou can now try to change the led status on the board with the following command:\033[0m"
echo "	~/OrangeStarterKit/scripts/led.sh"
echo ""

exit 0

