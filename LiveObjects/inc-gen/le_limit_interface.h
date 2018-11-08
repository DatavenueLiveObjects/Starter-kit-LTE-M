

/*
 * ====================== WARNING ======================
 *
 * THE CONTENTS OF THIS FILE HAVE BEEN AUTO-GENERATED.
 * DO NOT MODIFY IN ANY WAY.
 *
 * ====================== WARNING ======================
 */

/**
 * @page c_limit Limit Definitions
 *
 * @ref le_limit_interface.h "API Reference"
 *
 * This file defines limit definitions used by other APIs.
 *
 * <HR>
 *
 * Copyright (C) Sierra Wireless Inc.
 */
/**
 * @file le_limit_interface.h
 *
 * Legato @ref c_limit include file.
 *
 * Copyright (C) Sierra Wireless Inc.
 */

#ifndef LE_LIMIT_INTERFACE_H_INCLUDE_GUARD
#define LE_LIMIT_INTERFACE_H_INCLUDE_GUARD


#include "legato.h"


//--------------------------------------------------------------------------------------------------
/**
 * Type for handler called when a server disconnects.
 */
//--------------------------------------------------------------------------------------------------
typedef void (*le_limit_DisconnectHandler_t)(void *);

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
void le_limit_ConnectService
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
le_result_t le_limit_TryConnectService
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
void le_limit_SetServerDisconnectHandler
(
    le_limit_DisconnectHandler_t disconnectHandler,
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
void le_limit_DisconnectService
(
    void
);


//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of application names.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_APP_NAME_LEN 47

//--------------------------------------------------------------------------------------------------
/**
 * Maximum length of process names.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_PROC_NAME_LEN 47

//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length of paths.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_MAX_PATH_LEN 511

//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length of priority names.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_MAX_PRIORITY_NAME_LEN 6

//--------------------------------------------------------------------------------------------------
/**
 * Maximum string length of argument lists.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_MAX_ARGS_STR_LEN 511

//--------------------------------------------------------------------------------------------------
/**
 * Length of a MD5 string.
 */
//--------------------------------------------------------------------------------------------------
#define LE_LIMIT_MD5_STR_LEN 32

#endif // LE_LIMIT_INTERFACE_H_INCLUDE_GUARD