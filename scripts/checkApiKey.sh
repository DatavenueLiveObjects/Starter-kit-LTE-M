#!/bin/bash

echo "========================================================================="
echo "Check the Live Objects APIKEY"
echo "========================================================================="
echo ""
echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"

while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

echo " => MangOH Red is online"
echo ""

echo "==============================================================================="
echo "Live Objects configuration"
echo "==============================================================================="
echo "If you don't have your api key yet :"
echo "  - connect to Live Objects https://liveobjects.orange-business.com/#/login"
echo "  - select the configuration menu and then Api Keys"
echo "  - add a new key"
echo "  - select the  profile : customized"
echo -e "	- check \033[4;37mReading /writing options\033[0m for both \033[4;37m'Device Access'\033[0m and \033[4;37m'Device options'\033[0m"

while :
do

	echo ""
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
	    		else
				APIKEY=$currentAPIKEY
        	fi
	fi

	URL="https://liveobjects.orange-business.com/api/v1/deviceMgt/devices"
	#echo "Live Objects URL : "
	#echo $URL
	#echo ""
	#echo "Request Headers:"
	#echo "Accept:application/json"
	#echo "X-API-Key:$APIKEY"
	#echo ""

	#echo "Server response:"

	response=$(curl -I -s -H "Content-type:application/json" -H "X-API-Key:$APIKEY" -X GET "$URL")
	#echo $response

	if [[ $response == *"Access-Control-Allow-Headers"* ]]
		then
			echo -e "\033[1;32mAPIKEY checked\033[0m"
			exit 0
		else
			if [[ $response == *"must-revalidate"* ]]
        			then
                			echo -e "\033[1;31mERROR: Wrong ApiKey.\033[0m"
        			else
					echo -e "\033[1;31mERROR: Failed to contact Live Objects.\033[0m"
					echo "Check your internet access from the Virtual Machine"
					exit 1
			fi
	fi
done
