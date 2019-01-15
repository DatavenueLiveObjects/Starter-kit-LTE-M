#!/bin/bash

bash ./scripts/checkApiKey.sh
if [[ $? -eq 1 ]]
        then
                echo -e "\033[1;31mERROR: Failed to configure the apikey, abort setup.\033[0m"
                exit 1
fi
echo ""

echo "==============================================================================="
echo "Configure the starter kit on Live Objects"
echo "==============================================================================="
#echo "In the Devices menu, a new device is created with the name 'Starter Kit' and the ID 'urn:lo:nsid:starterkit:<board IMEI>'"

APIKEY="$(ssh root@192.168.2.2 '/legato/systems/current/bin/config get /LiveObjects/apiKey')"

URL="https://liveobjects.orange-business.com/api/v1/deviceMgt/devices"
IMEI="$(ssh root@192.168.2.2 '/legato/systems/current/bin/cm info imei')"
deviceName="Starter Kit"
namespace="starterkit"
deviceID=$IMEI
description="Device created for the Orange Starter Kit with the mangOH Red board. You can send a 'hello' command or a 'led' command to interact with the board"
result=$(curl -H "Content-type:application/json" -H "X-API-Key:$APIKEY" -X POST "$URL" -d "{\"id\":\"urn:lo:nsid:$namespace:$deviceID\",\"name\":\"$deviceName\",\"description\":\"$description\"}" 2>/dev/null)
if [[ $result == *"DM_DEVICE_DUPLICATE"* ]]; then
  	echo "Device already exists with the ID 'urn:lo:nsid:starterkit:$IMEI'"
  else
	echo "In the Devices menu, a new device is created with the name 'Starter Kit' and the ID 'urn:lo:nsid:starterkit:$IMEI'"
fi

exit 0


