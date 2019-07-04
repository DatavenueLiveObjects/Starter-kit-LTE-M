#!/bin/bash

echo "==============================================================================="
echo "Network configuration"
echo "==============================================================================="

MNO="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm sim imsi'| cut -c32-36)"

if [ -z "$MNO" ]
	then
	echo -e "\033[1;31mERROR: Unable to get SIM informations\033[0m"
	echo "check your SIM card and retry"
	echo "exit setup"
	exit 1
fi

SIM="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm sim' | cut -c36-47)"
if [ "$SIM" != "LE_SIM_READY" ]
        then
        echo -e "\033[1;31mERROR: SIM card is not ready.\033[0m"
	SIMINFO="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm sim')"
        echo $SIMINFO
        echo "exit setup"
        exit 1
fi

#default RAT is LTE-M
radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio off')"
ssh root@192.168.2.2 '(echo -ne"AT\r"; echo -ne "AT!SELCIOT=6\r") | /usr/bin/microcom /dev/ttyAT -t 1000 &> /dev/null'
#radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on')"

case "$MNO" in
                "20801")
                        echo "Data configuration for Orange France"
                        APN="orange.ltem.spec"
                        auth="pap"
                        username="orange"
                        password="orange"
                        ;;
                "20610")
                        echo "Data configuration for Orange Belgium"
                        APN="orange.ltem.spec"
                        auth="none"
                        ;;
                "21403")
                        echo "Data configuration for Orange Spain"
                        APN="iot.nat.es"
                        auth="pap"
                        username="any"
                        password="any"
                        ;;
                "23101")
                        echo "Data configuration for Orange Slovakia"
                        APN="INTERNET"
                        auth="none"
                        ;;
                "20895")
                        echo "Data configuration for Orange IoT Lab"
                        APN="iot"
                        auth="none"
                        ;;
                *)
                        echo "Unkown Network Operator"
                        echo "enter your data connection settings"
                        echo "APN : "
                       read APN
                        echo "Username :"
                        read username
                        echo "Password :"
                        read password
                        echo "Authentification mode : <none/pap/chap>"
                        read auth
                        ;;
        esac

        #disconnect
        dataConnectionStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data | grep "Connected:"')"

        if [ $(echo "$dataConnectionStatus" | cut -c32-32)  = "y" ]
                then
                        result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect -1' 2>/dev/null)
        fi

        #configure data profile 1
        result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data profile 1')
        command="/legato/systems/current/bin/cm data apn"
        result=$(ssh root@192.168.2.2 $command $APN)
        command="/legato/systems/current/bin/cm data auth"
        result=$(ssh root@192.168.2.2 $command $auth $username $password)
        result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio off')
        sleep 2
        result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on')
        result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect' 2>/dev/null)

        echo "Data configuration done"





#Wait device is attached
while :
do
	radioPower="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio'| grep Power | cut -c32-33)"
	if [ $radioPower = "ON" ]
		then
			echo "radio is ON"
			break
		else
			radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on')"
			printf "%c" "."
			sleep 1
	fi
done

echo "Check signal strength"

timer=1
while :
do

	radioSignal="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio'| grep Signal | cut -c32-33)"

	if [ $radioSignal != "No" ]
		then
			radioSignal="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio'| grep Signal)"
			echo $radioSignal
			break 
		else
                       if [[ $timer -gt "20" ]]
                               then
                                       echo -e "\033[1;31mERROR: Unable to join network.\033[0m"
                                       echo "Check the antenna and your subscription"
                                       exit 1
                       fi
			printf "%c" "."
			sleep 2
	fi
done


echo ""
echo "Set data profile."
while :
do
	timer=1

	while :
	do
		dataConnectionStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data | grep "Connected:"')"

		if [ $(echo "$dataConnectionStatus" | cut -c32-32)  = "y" ]
        		then
              			echo -e "\033[1;32mConnected.\033[0m"
	      			exit 0
			else
	      			if [[ $timer -gt "10" ]]
					then
	      					echo -e "\033[1;31mERROR: Connection failed.\033[0m"
						echo "Check your network settings."
						exit 1
					else
	      					printf "%c" "."
						result=$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm data connect' 2>/dev/null)
	      					sleep 3
				fi
		fi
		timer=$(($timer+1))

	done
done
