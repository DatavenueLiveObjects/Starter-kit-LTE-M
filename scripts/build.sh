#!/bin/bash

#echo 'clean'
#make clean

#echo "Configuring shell for Legato development"
#echo "----------------------------------------"
pushd /home/mangoh/legato_framework/legato > /dev/null
source bin/configlegatoenv
popd > /dev/null

echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"




echo ""
echo "==============================================================================="
echo "Install OrangeStarterKit"
echo "==============================================================================="

cd ~/OrangeStarterKit/
echo " > Build Orange StarterKit. Please wait up to 5 minutes..."
make wp77xx &>/dev/null

build_result=$?
if [ $build_result -eq 1 ]     
    then
    	echo "==============================================================================="
    	echo -e "\033[1;31mOrangeStarter kit build failed\033[0m"
    	exit 1
fi

# check firmware
echo " > Check board firmware"
boardFirmwareVersion=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info firmware' | cut -c10-20)
vmFirmwareVersion=$(cat /home/mangoh/.OrangeStarterKit/firmware_version)

if [ "$boardFirmwareVersion" != "$vmFirmwareVersion" ]
    then
        echo "Firmware Update required"
        fwupdate download /home/mangOH/.OrangeStarterKit/WP77_Firmware_Orange.spk 192.168.2.2
        echo "Waiting for the mangOH Red to reboot (you may have to power-cycle it manually)"
        sleep 20
        while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
        do
            printf "%c" "."
        done
    else
        echo "No update required"
fi
echo ""

#upload
echo " > Upload to the mangOH red"
update Orange.wp77xx.update 192.168.2.2

updload_result=$?
if [ $updload_result -eq 0 ]; then
	echo ' > done'
	exit 0
else
	echo '\033[1;31m > build failed! \033[0m'
    exit 1
fi



#echo 'build'
#make wp77xx

#build_result=$?
#
#if [ $build_result -eq 0 ]; then
#	echo 'run'
#	update OrangeStarterKit.wp77xx.update 192.168.2.2
#    exit 0
#else
#	echo 'build failed'
#    exit 1
#fi

