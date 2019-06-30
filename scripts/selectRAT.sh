#!/bin/bash

if [[ -z "$1" ]]
	then
                while :
                do
                        echo "Select your radio access technology:"
                        echo " 1 - LTE-M"
                        echo " 2 - NB-IoT"
			echo " 3 - LTE-M / NB-IoT"

                        read RAT

                        if [[ "$RAT" == "1" || "$RAT" == "2" || "$RAT" == "3" ]]
                                then
                                        break
                        fi
                done

	else
                if [[ "$1" == "1" || "$1" == "2" || "$1" == "3" ]]
                        then
                                RAT=$1
                        else
                                echo "Unknown parameter $1"
                                exit 1
                fi

fi

echo "Waiting for the mangOH Red - board boot can last up to 2 min - ctrl-z to stop"
while ! ping -c 1 -n -w 1 192.168.2.2 &> /dev/null
do
    printf "%c" "."
done

ssh root@192.168.2.2 'rm /var/lock/LCK..ttyAT' &> /dev/null
radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio off')"


case "$RAT" in
	1)
		MODE="LTE-M"
                echo "Set LTE-M"
                ssh root@192.168.2.2 '(echo -ne"AT\r"; echo -ne "AT!SELCIOT=2\r") | /usr/bin/microcom /dev/ttyAT -t 1000 &> /dev/null'
		;;
	2)
                MODE="NB-IoT"
                echo "Set NB IoT"
                ssh root@192.168.2.2 '(echo -ne"AT\r"; echo -ne "AT!SELCIOT=4\r") | /usr/bin/microcom /dev/ttyAT -t 1000 &> /dev/null'
		;;

	*)
                MODE="LTE-M / NB-IoT"
                echo "Set LTE-M / NB IoT"
                ssh root@192.168.2.2 '(echo -ne"AT\r"; echo -ne "AT!SELCIOT=6\r") | /usr/bin/microcom /dev/ttyAT -t 1000 &> /dev/null'
		;;
esac

echo "done"

radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on')"

timer=1
while :
do
        radioStatus="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio'| grep Status | cut -c32-34)"
        if [ $radioStatus = "Reg" ]
                then
                        echo -e "\033[1;32m$MODE: connected.\033[0m"
                        exit 0
                else
                        if [[ $timer -gt "20" ]]
                                then
                                        echo -e "\033[1;31m$MODE: unable to connect.\033[0m"
                                        exit 1
                        fi
                        printf "%c" "."
			radioPowerOn="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm radio on')"

                        sleep 1
        fi
        timer=$(($timer+1))
done



