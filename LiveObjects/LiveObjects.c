#include "legato.h"
#include "interfaces.h"
#include "LiveObjects.h"
#include "swir_json.h"

#define YIELD_INTERVAL_SECOND 15

//------
/**
 * Live Objects settings
 */
char                                        _broker[] = "liveobjects.orange-business.com";
int32_t                                     _portNumber = 8883;
int32_t                                     _useTLS = 1;
char                                        _username[] = "json+device";

int32_t                                     _keepAlive = 30;
int32_t                                     _qoS = 0;


//----
/**
 *  Live Objects MQTT emit topics
 */
char _topicConfig[] 			= "dev/cfg";
char _topicData[]				= "dev/data";
char _topicResource[] 			= "dev/rsc";
char _topicCommandRsp[] 		= "dev/cmd/res";
char _topicResourceUpdResp[] 	= "dev/rsc/upd/res";
char _topicResourceUpdErr[] 	= "dev/rsc/upd/err";

//----
/**
 *  Live Objects MQTT subscribe topics
 */
char _topicConfigUpdate[] 		= "dev/cfg/upd";
char _topicCommand[] 			= "dev/cmd";
char _topicResourceUpd[]		= "dev/rsc/upd";

//---
/**
 * user config
 */

char* _apikey;
char* _deviceid;


void (*_connectionHandler)();
mqttClient_InstanceRef_t                    _cliMqttRef = NULL;
le_data_RequestObjRef_t                     _DataRequestRef = NULL;
le_data_ConnectionStateHandlerRef_t         _DataConnectionStateHandlerRef = NULL;

le_timer_Ref_t                              _timerRef = NULL;

//--------------------------------------------------------------------------------------------------
/**
 *  The opposite of Connect, this function will tear down the data connection.
 */
//--------------------------------------------------------------------------------------------------
static void Disconnect(bool quitApp)
{
    if (_cliMqttRef)
    {
        LE_INFO("Delete MQTT instance");
        mqttClient_Delete(_cliMqttRef);
        _cliMqttRef = NULL;
    }


    if (_DataConnectionStateHandlerRef)
    {
        le_data_RemoveConnectionStateHandler(_DataConnectionStateHandlerRef);
        _DataConnectionStateHandlerRef = NULL;
    }

    if (_DataRequestRef)
    {
        LE_INFO("Releasing the data connection.");
        le_data_Release(_DataRequestRef);

        _DataRequestRef = NULL;
    }

    if (quitApp)
    {
        exit(EXIT_SUCCESS);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 *  Event callback for data connection state changes.
 */
//--------------------------------------------------------------------------------------------------
static void DcsStateHandler
(
    const char* intfName,
    bool        isConnected,
    void*       contextPtr
)
{

	LE_INFO("DcsStateHandler %d", isConnected);
    if (isConnected)
    {
        //PrintMessage("DSC connected... starting MQTT connection");

        LE_INFO("%s connected! Starting MQTT session", intfName);

        if (LE_OK == mqttClient_StartSession(_cliMqttRef))
        {

            if (_connectionHandler)
            {
            	_connectionHandler();
            }

            //subscribe to Live Objects Topics

            LE_INFO("Subscribe to %s, %s, %s", _topicConfigUpdate,_topicResourceUpd, _topicCommand);
            mqttClient_Subscribe(_cliMqttRef, _topicCommand);
            mqttClient_Subscribe(_cliMqttRef, _topicConfigUpdate);
            mqttClient_Subscribe(_cliMqttRef, _topicResourceUpd);

        	le_timer_Start(_timerRef);

        }
        else
        {
            //PrintMessage("Failed to start MQTT session with LO");

            Disconnect(true);
        }
    }
}

//--------------------------------------------------------------------------------------------------
/**
 *  This function will request the data connection
 */
//--------------------------------------------------------------------------------------------------
void Connect
(
    void
)
{
    if (_cliMqttRef == NULL)
    {
        //data connection might be already exists, so it is important to create the MQTT instance first

        LE_INFO("Create MQTT instance");
        _cliMqttRef = mqttClient_Create(_broker, _portNumber, _useTLS, _deviceid, _username, _apikey, _keepAlive, _qoS);
    }

    // register handler for data connection state change
    if (!_DataConnectionStateHandlerRef)
    {
        _DataConnectionStateHandlerRef = le_data_AddConnectionStateHandler(DcsStateHandler, NULL);
        LE_INFO("Registered for data connection state : %p.", _DataConnectionStateHandlerRef);
    }

    if (!_DataRequestRef)
    {
        _DataRequestRef = le_data_Request();
        LE_INFO("Requesting the data connection: %p.", _DataRequestRef);
    }
}



//--------------------------------------------------------------------------------------------------
/**
 *  publish Binary Data
 *  topic : one the Live Objects topic available
 *  filename : file to send
 */
//--------------------------------------------------------------------------------------------------

/*
void liveobjects_pubFile
(
        const char* filename
)
{
    if (mqttClient_IsConnected(_cliMqttRef))
    {
        LE_INFO("[LO][PublishFileContent] %s : %s ", _topicBinary, filename);
        mqttClient_PublishFileContent(_cliMqttRef,  filename, _topicBinary);
    }
}
*/

//--------------------------------------------------------------------------------------------------
/**
 *  publish Data
 *  topic : one of the Live Objects topic available
 *  payload : json payload
 */
//--------------------------------------------------------------------------------------------------
static void liveobjects_publish
(
    char* topic,
	char* payload
)
{
    if (mqttClient_IsConnected(_cliMqttRef))
    {
       	LE_INFO("[LO][Publish] %s : %s ", topic, payload);
    	mqttClient_Publish(_cliMqttRef,   (uint8_t *)  payload, strlen(payload), topic);
    }
}

//--------------------------------------------------------------------------------------------------
/**
 *  publish Data
 *
 *  "s":  "<<streamId>>",
 *  "ts": "<<timestamp>>",
 *  "m":  "<<model>>",
 *  "v": <<value>> JSON object max size 896 octets. to send more data use the liveobjects_pubFile function
 *  "t" : [<<tag1>>,<<tag2>>,...]
 *  "loc": [<<latitude>>, <<longitude>>]
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubData
(
		char* streamid,
		char* payload,
		char* model,
		char* tags,
		double latitude,
		double longitude
)
{
	char* s = swirjson_szSerialize("s", streamid, 0);
	char v[896] = "";
	sprintf(v, "\"v\": %s", payload);
	char* m = swirjson_szSerialize("m", model, 0);
	char loc[30] = "";
	sprintf(loc, "\"loc\":[%lf,%lf]", latitude, longitude);
	char t[100] = "";
	sprintf(t, "\"t\":%s", tags);
	char message[1024] = "";
	sprintf(message, "{%s, %s, %s, %s, %s}", s, m, v, loc, t);

	LE_INFO("Publish data , %s", message);

	liveobjects_publish(_topicData, message);
    
    if (m) free(m);  
    if (s) free(s);  
}

//--------------------------------------------------------------------------------------------------
/**
 *  publish Command response
 *  {
 *  "res": {
 *     "<<res1>>": "<<res1Value>>",
 *     "<<res2>>": "<<res2Value>>" // multiple responses not yet implemented
 *     ...
 *  },
 *  "cid":  <<correlationId>>
}
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubCmdRes
(
    char* response,
	int cid
)
{
	LE_INFO("[LO][Publish][Command response] done: %s ,cid: %d", response, cid);
	//char result[256] = "";
	char result[1024] = "";
	sprintf(result, "{\"res\": {\"done\": %s}, \"cid\": %d}", response, cid);

	liveobjects_publish(_topicCommandRsp, result);
}

//--------------------------------------------------------------------------------------------------
/**
 *  publish config
 *  used to notify Live Objects of device configuration
 *
 *     "cfg": {
 *     	  "<<param1Key>>": {
 *       	 "t": "<<param1Type>>",
 *        	 "v": <<param1Value>>
 *     	  },
 *     	  ...
 *  	}
 *
 *  Available types : "i32", "u32", "f64", "str" UTF-8, "bin" base64 encoded
 *
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubConfig
(
    char* key,
	char* type,
	char* value
)
{
	char payload[1024] = "";
	//char payload[256] = "";
	sprintf(payload, "{\"cfg\": {\"%s\" : {\"t\": \"%s\",\"v\": \"%s\"}}}", key, type, value);
	liveobjects_publish(_topicConfig, payload);
}


//--------------------------------------------------------------------------------------------------
/**
 *  publish update response
 *  response to a configuration update request
 *     "cfg": {
 *     "<<param1Key>>": {
 *        "t": "<<param1Type>>",
 *        "v": <<param1Value>>,
 *     },
 *     ...
 *  },
 *  "cid": <<correlationId>>
 *
 *  Available types : "i32", "u32", "f64", "str" UTF-8, "bin" base64 encoded
 *
 *  Multiple configurations submission not yet implemented
 *
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubConfigUpdateResponse
(
    char* key,
	char* type,
	char* value,
	int cid
)
{
	//char payload[256] = "";
	char payload[1024] = "";
	sprintf(payload, "{\"cfg\": {\"%s\": {\"t\": \"%s\",\"v\": \"%s\"}},\"cid\": %d}", key, type, value, cid);
	liveobjects_publish(_topicConfig, payload);
}

//--------------------------------------------------------------------------------------------------
/**
 *  publish resources
 *	your device can announce the currently deployed versions of its resources
 *
 *  format :
 * 		"rsc": {
 *     		"<<resource1Id>>": {
 *        	 	"v": "<<resource1Version>>",
 *        	 	"m": <<resource1Metadata>>
 *     	  	  },
 *     	  	 "<<resource2Id>>": {
 *        	 	 "v": "<<resource2Version>>",
 *        	 	 "m": <<resource2Metadata>>
 *     	  	  },
 *     	  ...
 *  	  }
 *	Multiple resources submission not yet implemented
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubResource
(
    char* ressourceId,
	char* version,
	char* jsonMetadata
)
{
	char payload[1024] = "";
	//char payload[256] = "";
	sprintf(payload, "{\"rsc\": {\"%s\" : {\"v\": \"%s\",\"m\": %s}}}", ressourceId, version, jsonMetadata);
	liveobjects_publish(_topicResource, payload);
}

//--------------------------------------------------------------------------------------------------
/**
 *  publish resources update response
 *
 *  format :
   	   "res": "<<responseStatus>>",
   "	cid": "<<correlationId>>"
 *
 *  responseStatus values:
 *
 *   "OK" : the update is accepted and will start,
 *   "UNKNOWN_RESOURCE" : the update is refused, because the resource (identifier) is unsupported by the device,
 *   "WRONG_SOURCE_VERSION" : the device is no longer in the "current" (old) resource version specified in the resource update request,
 *   "WRONG_TARGET_VERSION" : the device doesn’t support the "target" (new) resource version specified in the resource update request,
 *   "INVALID_RESOURCE" : the requested new resource version has incorrect version format or metadata,
 *   "NOT_AUTHORIZED" : the device refuses to update the targeted resource (ex: bad timing, "read-only" resource, etc.),
 *   "INTERNAL_ERROR" : an error occurred on the device, preventing for the requested resource update,
 *
 *
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubResourceUpdateResponse
(
    char* response,
	int correlationId
)
{
	char result[256] = "";
	sprintf(result, "{\"done\": %s}", response);
	liveobjects_publish(_topicResourceUpd, result);
}


//--------------------------------------------------------------------------------------------------
/**
 *  publish resources update response error
 *
 *  format :
 *  	   "errorCode":"ERROR_CODE",
 *  	   "errorDetails":"error details"
 *
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_pubResourceUpdateResponseError
(
    char* errorCode,
	char* errorDetails
)
{
	char payload[1024] = "";
	//char payload[256] = "";
	sprintf(payload, "{\"errorCode\": \"%s\", \"errorDetails\": \"%s\"}", errorCode, errorDetails);
	liveobjects_publish(_topicResourceUpdErr, payload);
}


//--------------------------------------------------------------------------------------------------
/**
 *   Live Objects event
 *   register an incoming message handler
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_AddIncomingMessageHandler
(
	void* msgHandler
)
{
	mqttClient_AddIncomingMessageHandler(_cliMqttRef, msgHandler, NULL);
}


//--------------------------------------------------------------------------------------------------
/**
 *  Timer Handler
 *  check connection status
 */
//--------------------------------------------------------------------------------------------------
static void timerHandler
(
    le_timer_Ref_t  timerRef
)
{
    if (mqttClient_IsConnected(_cliMqttRef))
    {
        LE_INFO("MQTT yield");
        if (LE_FAULT == mqttClient_ProcessEvent(_cliMqttRef))
        {
            //MQTT connection might be lost
            //Start over again
            LE_INFO("MQTT connection Lost - Reconnecting");
            Disconnect(false);
            Connect();
        }
        else
        {
            le_timer_Start(_timerRef);
        }
    }
    else
    {
        LE_INFO("No active MQTT session, now stop Timer");
        le_timer_Stop(_timerRef);
    }

}

//--------------------------------------------------------------------------------------------------
/**
 *  Connect to Live Objects with user settings
 *  apikey : Live Objects APIKEY
 *  namespace : device identifier namespace (device model, identifier class...)
 *  id: device identifier (IMEI, Serial Number, MAC adress...)
 *  connectionHandler : handler called when registration to Live Objects is done
 *
 *  function store the _deviceid, format : urn:lo:nsid:{namespace}:{id}
 */
//--------------------------------------------------------------------------------------------------
void liveobjects_connect
(
    char* apikey,
	char* namespace,
	char* id,
	void* connectionHandler
)
{

	_apikey = malloc(strlen(apikey));
	strcpy(_apikey, apikey);
	_deviceid = malloc(strlen(namespace)+ strlen(id) + 13);
	sprintf(_deviceid, "urn:lo:nsid:%s:%s", namespace,id);
	LE_INFO("register %s, deviceid: %s", _apikey, _deviceid);
	_connectionHandler = connectionHandler;


    //Create timer to call mqttClient_ProcessEvent
    _timerRef = le_timer_Create("Live Objects Timer");
    LE_FATAL_IF(_timerRef == NULL, "timerApp timer ref is NULL");

    le_clk_Time_t   interval = { YIELD_INTERVAL_SECOND, 0 };
    le_result_t     res;

    res = le_timer_SetInterval(_timerRef, interval);
    LE_FATAL_IF(res != LE_OK, "set interval to %lu seconds: %d", interval.sec, res);

    res = le_timer_SetRepeat(_timerRef, 1);
    LE_FATAL_IF(res != LE_OK, "set repeat to once: %d", res);

    le_timer_SetHandler(_timerRef, timerHandler);

	Connect();

}

COMPONENT_INIT
{
    LE_INFO("Live Objects module loaded.");


}
