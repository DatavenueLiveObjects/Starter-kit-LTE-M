#ifndef LIVEOBJECTS_H_INC
#define LIVEOBJECTS_H_INC

LE_SHARED void liveobjects_connect(char* apikey, char* namespace, char* id, void* connectionHandler);
LE_SHARED void liveobjects_pubData(char* streamid, char* payload, char* model, char* tags, double latitude, double longitude);
LE_SHARED void liveobjects_pubCmdRes(char* jsonStr, int cid);
LE_SHARED void liveobjects_pubConfig(char* key, char* type, char* value);
LE_SHARED void liveobjects_pubConfigUpdateResponse(char* key, char* type, char* value, int cid);
LE_SHARED void liveobjects_pubResource(char* ressourceId, char* version, char* metadata);
LE_SHARED void liveobjects_pubResourceUpdateResponse(char* response, int correlationId);
LE_SHARED void liveobjects_pubResourceUpdateResponseError(char* errorCode, char* errorDetails);
LE_SHARED void liveobjects_AddIncomingMessageHandler(void* msgHandler);

#endif
