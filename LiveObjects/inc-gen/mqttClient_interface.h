

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */


#ifndef MQTTCLIENT_INTERFACE_H_INCLUDE_GUARD
#define MQTTCLIENT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"

// Interface specific includes
#include "le_limit_interface.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*mqttClient_DisconnectHandler_t)(void *);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Connect the current client thread to the service providing this API. Block until the service is
 * available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void mqttClient_ConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Try to connect the current client thread to the service providing this API. Return with an error
 * if the service is not available.
 *
 * For each thread that wants to use this API, either ConnectService or TryConnectService must be
 * called before any other functions in this API.  Normally, ConnectService is automatically called
 * for the main thread, but not for any other thread. For details, see @ref apiFilesC_client.
 *
 * This function is created automatically.
 *
 * @return
 *  - LE_OK if the client connected successfully to the service.
 *  - LE_UNAVAILABLE if the server is not currently offering the service to which the client is
 *    bound.
 *  - LE_NOT_PERMITTED if the client interface is not bound to any service (doesn't have a binding).
 *  - LE_COMM_ERROR if the Service Directory cannot be reached.
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_TryConnectService
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Set handler called when server disconnection is detected.
 *
 * When a server connection is lost, call this handler then exit with LE_FATAL.  If a program wants
 * to continue without exiting, it should call longjmp() from inside the handler.
 */
//--------------------------------------------------------------------------------------------------
void mqttClient_SetServerDisconnectHandler
(
    mqttClient_DisconnectHandler_t disconnectHandler,
    void *contextPtr
);

//--------------------------------------------------------------------------------------------------
/**
 *
 * Disconnect the current client thread from the service providing this API.
 *
 * Normally, this function doesn't need to be called. After this function is called, there's no
 * longer a connection to the service, and the functions in this API can't be used. For details, see
 * @ref apiFilesC_client.
 *
 * This function is created automatically.
 */
//--------------------------------------------------------------------------------------------------
void mqttClient_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 */
//--------------------------------------------------------------------------------------------------
typedef struct mqttClient_Instance* mqttClient_InstanceRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'mqttClient_IncomingMessage'
 */
//--------------------------------------------------------------------------------------------------
typedef struct mqttClient_IncomingMessageHandler* mqttClient_IncomingMessageHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Reference type used by Add/Remove functions for EVENT 'mqttClient_AvSoftwareInstall'
 */
//--------------------------------------------------------------------------------------------------
typedef struct mqttClient_AvSoftwareInstallHandler* mqttClient_AvSoftwareInstallHandlerRef_t;


//--------------------------------------------------------------------------------------------------
/**
 * Handler for Incoming message
 */
//--------------------------------------------------------------------------------------------------
typedef void (*mqttClient_IncomingMessageHandlerFunc_t)
(
    const char* LE_NONNULL topicName,
        ///< Name of the subscribed topic
    const char* LE_NONNULL key,
        ///< Key Name of the data
    const char* LE_NONNULL value,
        ///< Value of the data
    const char* LE_NONNULL timestamp,
        ///< Timestamp of the data
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Handler for AirVantage Software Install Over the Air Command
 */
//--------------------------------------------------------------------------------------------------
typedef void (*mqttClient_AvSoftwareInstallHandlerFunc_t)
(
    const char* LE_NONNULL uid,
        ///< AV operation uid
    const char* LE_NONNULL type,
        ///< SOTA type
    const char* LE_NONNULL revision,
        ///< SW revision
    const char* LE_NONNULL url,
        ///< url to download the software package
    const char* LE_NONNULL timestamp,
        ///< Timestamp
    void* contextPtr
        ///<
);

//--------------------------------------------------------------------------------------------------
/**
 * Get default MQTT config (if mqttClientRef is invalid), otherwise returns config of the given mqttClientRef
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_GetConfig
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    char* brokerUrl,
        ///< [OUT]
    size_t brokerUrlSize,
        ///< [IN]
    int32_t* portNumberPtr,
        ///< [OUT]
    int32_t* useTLSPtr,
        ///< [OUT]
    char* deviceId,
        ///< [OUT]
    size_t deviceIdSize,
        ///< [IN]
    char* username,
        ///< [OUT]
    size_t usernameSize,
        ///< [IN]
    char* secret,
        ///< [OUT]
    size_t secretSize,
        ///< [IN]
    int32_t* keepAlivePtr,
        ///< [OUT]
    int32_t* qoSPtr
        ///< [OUT]
);

//--------------------------------------------------------------------------------------------------
/**
 * Create a MQTT instance, returns mqttClientRef (to be used in all subsequent calls)
 */
//--------------------------------------------------------------------------------------------------
mqttClient_InstanceRef_t mqttClient_Create
(
    const char* LE_NONNULL brokerUrl,
        ///< [IN]
    int32_t portNumber,
        ///< [IN]
    int32_t useTLS,
        ///< [IN]
    const char* LE_NONNULL deviceId,
        ///< [IN]
    const char* LE_NONNULL username,
        ///< [IN]
    const char* LE_NONNULL secret,
        ///< [IN]
    int32_t keepAlive,
        ///< [IN]
    int32_t qoS
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Set additional TLS parameters, (optional)
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_SetTls
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL rootCA,
        ///< [IN]
    const char* LE_NONNULL certificate,
        ///< [IN]
    const char* LE_NONNULL privateKey
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Delete a MQTT instance
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_Delete
(
    mqttClient_InstanceRef_t mqttClientRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Start a MQTT session
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_StartSession
(
    mqttClient_InstanceRef_t mqttClientRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Stop MQTT session
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_StopSession
(
    mqttClient_InstanceRef_t mqttClientRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Get MQTT session status
 */
//--------------------------------------------------------------------------------------------------
bool mqttClient_IsConnected
(
    mqttClient_InstanceRef_t mqttClientRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Publish {"key" : "value"} to the specified topic
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_PublishKeyValue
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL key,
        ///< [IN]
    const char* LE_NONNULL value,
        ///< [IN]
    const char* LE_NONNULL topicName
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Publish raw data to the specified topic
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_Publish
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const uint8_t* dataPtr,
        ///< [IN]
    size_t dataSize,
        ///< [IN]
    const char* LE_NONNULL topicName
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Publish binary content of a giben filename
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_PublishFileContent
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL filanem,
        ///< [IN]
    const char* LE_NONNULL topicName
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Subscribe to the specified topic
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_Subscribe
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL topicName
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Unsubscribe to the specified topic
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_Unsubscribe
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL topicName
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Process Event - should be calling this function on a regular basis
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_ProcessEvent
(
    mqttClient_InstanceRef_t mqttClientRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'mqttClient_IncomingMessage'
 *
 * This event provides information on the incoming MQTT message
 */
//--------------------------------------------------------------------------------------------------
mqttClient_IncomingMessageHandlerRef_t mqttClient_AddIncomingMessageHandler
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    mqttClient_IncomingMessageHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'mqttClient_IncomingMessage'
 */
//--------------------------------------------------------------------------------------------------
void mqttClient_RemoveIncomingMessageHandler
(
    mqttClient_IncomingMessageHandlerRef_t handlerRef
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Publish {"key" : "value"} to the AirVantage specific topic
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_AvPublish
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL key,
        ///< [IN]
    const char* LE_NONNULL value
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Ack a command to AirVantage
 */
//--------------------------------------------------------------------------------------------------
le_result_t mqttClient_AvAck
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    const char* LE_NONNULL uid,
        ///< [IN]
    int32_t errorCode,
        ///< [IN]
    const char* LE_NONNULL message
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Add handler function for EVENT 'mqttClient_AvSoftwareInstall'
 *
 * This event provides information on the incoming MQTT message
 */
//--------------------------------------------------------------------------------------------------
mqttClient_AvSoftwareInstallHandlerRef_t mqttClient_AddAvSoftwareInstallHandler
(
    mqttClient_InstanceRef_t mqttClientRef,
        ///< [IN]
    mqttClient_AvSoftwareInstallHandlerFunc_t handlerPtr,
        ///< [IN]
    void* contextPtr
        ///< [IN]
);

//--------------------------------------------------------------------------------------------------
/**
 * Remove handler function for EVENT 'mqttClient_AvSoftwareInstall'
 */
//--------------------------------------------------------------------------------------------------
void mqttClient_RemoveAvSoftwareInstallHandler
(
    mqttClient_AvSoftwareInstallHandlerRef_t handlerRef
        ///< [IN]
);

#endif // MQTTCLIENT_INTERFACE_H_INCLUDE_GUARD