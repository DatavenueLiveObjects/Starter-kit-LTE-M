

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_adc ADC Channel API
 *
 * @ref le_adc_interface.h "API Reference" <br>
 * @ref platformConstraintsAdc Constraints
 *
 * <HR>
 *
 * This API provides definitions for reading ADC inputs.
 *
 * le_adc_ReadValue() returns the value read from an ADC channel. It takes two arguments:
 *    - The first is the name of the ADC channel to read.
 *    - The second is used to return the value read.
 *
 * @warning Ensure to check the list of supported ADC channels on your specific platform before
 * calling the le_adc_ReadValue() function. Please refer to  @subpage platformConstraintsAdc page.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_adc_interface.h
 *
 * Legato @ref c_adc include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_ADC_INTERFACE_H_INCLUDE_GUARD
#define LE_ADC_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_adc_DisconnectHandler_t)(void *);

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
void le_adc_ConnectService
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
le_result_t le_adc_TryConnectService
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
void le_adc_SetServerDisconnectHandler
(
    le_adc_DisconnectHandler_t disconnectHandler,
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
void le_adc_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum ADC's name string length.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ADC_ADC_NAME_MAX_LEN 30

//--------------------------------------------------------------------------------------------------
/**
 * Maximum ADC's name string length.
 * One extra byte is added for the null character.
 */
//--------------------------------------------------------------------------------------------------
#define LE_ADC_ADC_NAME_MAX_BYTES 31

//--------------------------------------------------------------------------------------------------
/**
 * Get the value of an ADC input
 *
 * @return
 *      - LE_OK            The function succeeded.
 *      - LE_FAULT         The function failed.
 */
//--------------------------------------------------------------------------------------------------
le_result_t le_adc_ReadValue
(
    const char* LE_NONNULL adcName,
        ///< [IN] Name of the ADC to read.
    int32_t* adcValuePtr
        ///< [OUT] The adc value
);

#endif // LE_ADC_INTERFACE_H_INCLUDE_GUARD