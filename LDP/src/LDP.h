/*!
 * @file    LDP.h
 * @author  Douglas Reis
 * @date    30/03/2019
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 */

#ifndef LDP_ProtocolH_
#define LDP_ProtocolH_

#include <stdint.h>
#include <stdbool.h>

#include "PacketID.h"
#include "Frame.h"
#include "StatusCode.h"
/*!
 * @defgroup API
 * 
 * @par Server example
 * @code{.cpp}
 * void Command(void *param, uint8_t address, LDP_Frame *data)
 * {	
 * 	switch(data->id)
 * 	{
 * 	case LDP_Cmd1:
 * 	{
 * 		st_cmd1 cmd1;
 * 		cmd1.field1 = data->payload.cmd1.field1 + 1;
 * 		cmd1.field2 = data->payload.cmd1.field2 + 2;
 * 		LDP_Response1(param, LDP_OK, &cmd1);
 * 	}break;
 * 	case LDP_Cmd2:
 * 	{
 * 		LDP_Response2(param, LDP_NotSupportedError, NULL);
 * 	}break;
 * 	default:
 * 		break;
 * 	}
 * }
 * 
 * void Event(void *param, uint8_t address, LDP_Frame *data)
 * {
 * 	//Do something
 * }
 * 
 * int main (int argc, char** argv)
 * {
 * 	uint8_t buffer[512];
 * 	int timeout;
 * 	LDP_Obj obj;
 * 	
 * 	bool ret = LDP_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 
 * 	ret = LDP_RegisterCommandCallback(&obj, Command, &obj);
 * 	ret = LDP_RegisterEventCallback(&obj, Event, &obj);
 * 
 * 	timeout = SYS_Tick() + TIMEOUT;
 * 	while(timeout > SYS_Tick())
 * 	{
 * 		ret = LDP_Run(&obj);
 * 	}
 * 
 * 	exit:
 * 	return 0;
 * }
 * @endcode
 * 
 * @par Client example
 * @code{.cpp}
 *
 * int main (int argc, char** argv)
 * {
 * 	uint8_t buffer[512];
 * 	int timeout;
 * 	LDP_Obj obj;
 * 	st_cmd1 in, out
 * 	
 * 	bool ret = LDP_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 	
 * 	in.field1 = 0;
 * 	in.field2 = 20;
 * 	LDP_Command1(&test.obj, &in, &out)
 * 
 * 	printf("%d %d", out.field1, out.field2)
 * 
 * 	exit:
 * 	return 0;
 * }
 * @endcode
 */


/*! @defgroup InitAPI Initialization API
 *  @brief This API is used to initialize the library in order to use the specifics APIs.
 *  @ingroup API
 *  @{
 */

typedef void (*LDP_Callback)(void *param, uint8_t address, LDP_Frame *data);

/*!
 * @brief Callback struct to provide access to platform resources.
 */ 
typedef struct LDP_Driver
{
	/*!
	 * @brief Open the port and return a handler to used by the functions LDP_Driver::Write LDP_Driver::Read LDP_Driver::Close
	 *
	 * @param[in] portName Name or identification to port to be opened.
	 *
	 * @return Handler to used to used by the functions LDP_Driver::Write LDP_Driver::Read LDP_Driver::Close
	 */
	void * ( *Open )(const void *portName);

	/*!
	 * @brief Write raw data to port opened by LDP_Driver::Open
	 *
	 * @param[in] handle Returned by LDP_Driver::Open.
	 * @param[in] buffer Buffer with the data to be send.
	 * @param[in] size   Amount of data so be send.
	 *
	 * @return Amount of data written.
	 */
	uint16_t ( *Write)(void *handle, const void *buffer, uint16_t size);

	/*!
	 * @brief Read raw data from port opened by LDP_Driver::Open
	 *
	 * @param[in] handle Returned by LDP_Driver::Open.
	 * @param[in] buffer Memory buffer to receive the data read.
	 * @param[in] size   Total space available in the buffer.
	 *
	 * @return Amount of data read.
	 */
	uint16_t ( *Read )(void *handle, void *buffer, uint16_t size);

	/*!
	 * @brief Close the port opened by LDP_Driver::Open
	 *
	 * @param[in] handle Returned by LDP_Driver::Open.
	 *
	 * @return 0 if ok.
	 */
	uint16_t ( *Close)(void *handle);

	/*!
	 * @brief Flush the port opened by LDP_Driver::Open
	 *
	 * @param[in] handle Returned by LDP_Driver::Open.
	 *
	 */
	void     ( *Flush)(void *handle);

	/*!
	 * @brief Read the system tick.
	 *
	 * @return System tick.
	 */
	uint32_t ( *Tick )();

	/*!
	 * @brief Sleep during the time specified.
	 *
	 * @param[in] time to sleep
	 */
	void (*Sleep) (uint32_t time);
}LDP_Driver;

/*!
 * @brief Class to hold the execution context.
 *
 * @details Must be allocated by the application.
 * @code{.cpp}
 * void main(void)
 * {
 *    uint8_t buffer[1024];
 *    LDP_Obj obj;
 *    LDP_Init(&obj, "COM1", &driver, buffer, sizeof(buffer));
 * ...
 * }
 * @endcode
 *
 */
typedef struct
{
	void *handle;
}LDP_Obj;

/*!
 * @brief Initialize the library
 *
 * @param[out] obj Pointer to the object which will be initialized.
 * 
 * @return ::LDP_StatusCode
 */
bool LDP_Init(LDP_Obj *obj, const void *port, LDP_Driver *driver, uint8_t * buffer, uint32_t size);

/*!
 * @brief Register a callback to receive notifications when a event arrives.
 *
 * @details The only way to receive event is by registering this callback, other wise the even
 * will be just discarded.
 * The function ::LDP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::LDP_Init function.
 * @param callback  Pointer to function to receive the events
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool LDP_RegisterEventCallback(LDP_Obj *obj, LDP_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a command arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::LDP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::LDP_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool LDP_RegisterCommandCallback(LDP_Obj *obj, LDP_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a response arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::LDP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::LDP_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool LDP_RegisterResponseCallback(LDP_Obj *obj, LDP_Callback callback, void *arg);

/*!
 * @brief Check for arrived events or async commands
 * 
 * @param[in] obj   Pointer to the object initialized in ::LDP_Init function.
 * 
 * @return ::LDP_StatusCode
 */
bool LDP_Run (LDP_Obj *obj);

/*!@}*/

/*! @defgroup CmdAPI Command API
 *  @brief This API is used to implement a Client.
 *  @ingroup API
 *  @{
 */
/*!
 * @brief Used to send the cmd1.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[in]   data  Pointer to the struct to send the data.
 *    
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Command1(LDP_Obj *obj, st_cmd1* data);

/*!
 * @copydoc LDP_Command1
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered
 * with the function ::LDP_RegisterResponseCallback
 */
LDP_StatusCode LDP_Command1Async(LDP_Obj *obj, st_cmd1* data);

/*!
 * @brief Used to send the cmd2.
 * 
 * @param[in]    obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[out]   data  Pointer to the struct with the data.
 *
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Command2(LDP_Obj *obj, st_cmd2* data);

/*!
 * @copydoc LDP_Command1
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered
 * with the function ::LDP_RegisterResponseCallback
 */
LDP_StatusCode LDP_Command2Async(LDP_Obj *obj);

/*!@}*/

/*! @defgroup RespAPI Response API
 * 	@brief This API is used to implement a Server
 *  @ingroup API
 *  @{
 */

/*!
 * @brief Used to send a response to the cmd1.
 *
 * @param[in]  obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[in]  data  Pointer to the struct to receive the data.
 *
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Response1(LDP_Obj *obj, LDP_StatusCode statusCode);

/*!
 * @brief Used to send a response to the cmd2.
 *
 * @param[in]  obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[in]  data  Pointer to the struct to receive the data.
 *
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Response2(LDP_Obj *obj, LDP_StatusCode statusCode, st_cmd2* data);


/*!@}*/

/*! @defgroup EvtAPI Event API
 *  @ingroup API
 *  @{
 */

/*!
 * @brief Used to warn.
 *
 * @param[in]  obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[in]  data  Pointer to the struct with the data.
 *
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Event1(LDP_Obj *obj, st_cmd1* data);

/*!
 * @brief Used to warn.
 *
 * @param[in]  obj   Pointer to the object initialized in ::LDP_Init function.
 * @param[in]  data  Pointer to the struct with the data.
 *
 * @return ::LDP_StatusCode
 */
LDP_StatusCode LDP_Event2(LDP_Obj *obj, st_cmd2* data);

/*! @}*/

#endif
