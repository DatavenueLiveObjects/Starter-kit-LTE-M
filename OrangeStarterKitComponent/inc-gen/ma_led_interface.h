

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_led mangOH LED  API
 *
 * @ref ma_led_interface.h "API Reference" <br>
 * @subpage howToLed "How To Setup Led" <br>
 *
 * <HR>
 *
 * @section led_admin overview 
 *
 * This API is used to update up the LED and reads its current status.
 *
 * @section led_usage Usage
 *
 * The sample code in this section shows how to turn on/off the LED in a user space app
 * written in C.
 *
 * ma_led_TurnOn() turns on the LED.
 * ma_led_TurnOff() turns on the LED.
 * @code
 * ma_led_TurnOn();
 * ma_led_TurnOff();
 * @endcode
 * 
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef MA_LED_INTERFACE_H_INCLUDE_GUARD
#define MA_LED_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*ma_led_DisconnectHandler_t)(void *);

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
void ma_led_ConnectService
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
le_result_t ma_led_TryConnectService
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
void ma_led_SetServerDisconnectHandler
(
    ma_led_DisconnectHandler_t disconnectHandler,
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
void ma_led_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Charging status of battery
 */
//--------------------------------------------------------------------------------------------------
typedef enum
{
    MA_LED_OFF = 0,
        ///< The LED is off
    MA_LED_ON = 1
        ///< The LED is on
}
ma_led_LedStatus_t;


//--------------------------------------------------------------------------------------------------
/**
 * Turns on the LED.
 */
//--------------------------------------------------------------------------------------------------
void ma_led_TurnOn
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Turns off the LED.
 */
//--------------------------------------------------------------------------------------------------
void ma_led_TurnOff
(
    void
);

//--------------------------------------------------------------------------------------------------
/**
 * Provides battery health status
 *
 * @return
 *     LE_OK on success or LE_FAULT on failure.
 *     LE_BAD_PARAMETER for invalid parameter
 */
//--------------------------------------------------------------------------------------------------
ma_led_LedStatus_t ma_led_GetLedStatus
(
    void
);

#endif // MA_LED_INTERFACE_H_INCLUDE_GUARD