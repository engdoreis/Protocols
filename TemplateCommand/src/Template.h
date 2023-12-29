/*!
 * @file    Template.h
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

#ifndef T_ProtocolH_
#define T_ProtocolH_

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
 * void Command(void *param, uint8_t address, T_Frame *data)
 * {	
 * 	switch(data->id)
 * 	{
 * 	case T_Cmd1:
 * 	{
 * 		st_cmd1 cmd1;
 * 		cmd1.field1 = data->payload.cmd1.field1 + 1;
 * 		cmd1.field2 = data->payload.cmd1.field2 + 2;
 * 		T_Response1(param, T_OK, &cmd1);
 * 	}break;
 * 	case T_Cmd2:
 * 	{
 * 		T_Response2(param, T_NotSupportedError, NULL);
 * 	}break;
 * 	default:
 * 		break;
 * 	}
 * }
 * 
 * void Event(void *param, uint8_t address, T_Frame *data)
 * {
 * 	//Do something
 * }
 * 
 * int main (int argc, char** argv)
 * {
 * 	uint8_t buffer[512];
 * 	int timeout;
 * 	T_Obj obj;
 * 	
 * 	bool ret = T_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 
 * 	ret = T_RegisterCommandCallback(&obj, Command, &obj);
 * 	ret = T_RegisterEventCallback(&obj, Event, &obj);
 * 
 * 	timeout = SYS_Tick() + TIMEOUT;
 * 	while(timeout > SYS_Tick())
 * 	{
 * 		ret = T_Run(&obj);
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
 * 	T_Obj obj;
 * 	st_cmd1 in, out
 * 	
 * 	bool ret = T_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 	
 * 	in.field1 = 0;
 * 	in.field2 = 20;
 * 	T_Command1(&test.obj, &in, &out)
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

typedef void (*T_Callback)(void *param, uint8_t address, T_Frame *data);

/*!
 * @brief Callback struct to provide access to platform resources.
 */ 
typedef struct T_Driver
{
	/*!
	 * @brief Open the port and return a handler to used by the functions T_Driver::Write T_Driver::Read T_Driver::Close
	 *
	 * @param[in] portName Name or identification to port to be opened.
	 *
	 * @return Handler to used to used by the functions T_Driver::Write T_Driver::Read T_Driver::Close
	 */
	void * ( *Open )(const void *portName);

	/*!
	 * @brief Write raw data to port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 * @param[in] buffer Buffer with the data to be send.
	 * @param[in] size   Amount of data so be send.
	 *
	 * @return Amount of data written.
	 */
	uint16_t ( *Write)(void *handle, const void *buffer, uint16_t size);

	/*!
	 * @brief Read raw data from port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 * @param[in] buffer Memory buffer to receive the data read.
	 * @param[in] size   Total space available in the buffer.
	 *
	 * @return Amount of data read.
	 */
	uint16_t ( *Read )(void *handle, void *buffer, uint16_t size);

	/*!
	 * @brief Close the port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
	 *
	 * @return 0 if ok.
	 */
	uint16_t ( *Close)(void *handle);

	/*!
	 * @brief Flush the port opened by T_Driver::Open
	 *
	 * @param[in] handle Returned by T_Driver::Open.
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
}T_Driver;

/*!
 * @brief Class to hold the execution context.
 *
 * @details Must be allocated by the application.
 * @code{.cpp}
 * void main(void)
 * {
 *    uint8_t buffer[1024];
 *    T_Obj obj;
 *    T_Init(&obj, "COM1", &driver, buffer, sizeof(buffer));
 * ...
 * }
 * @endcode
 *
 */
typedef struct
{
	void *handle;
}T_Obj;

/*!
 * @brief Initialize the library
 *
 * @param[out] obj Pointer to the object which will be initialized.
 * 
 * @return ::Template_StatusCode
 */
bool T_Init(T_Obj *obj, const void *port, T_Driver *driver, uint8_t * buffer, uint32_t size);

/*!
 * @brief Register a callback to receive notifications when a event arrives.
 *
 * @details The only way to receive event is by registering this callback, other wise the even
 * will be just discarded.
 * The function ::T_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::T_Init function.
 * @param callback  Pointer to function to receive the events
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool T_RegisterEventCallback(T_Obj *obj, T_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a command arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::T_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::T_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool T_RegisterCommandCallback(T_Obj *obj, T_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a response arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::T_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::T_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool T_RegisterResponseCallback(T_Obj *obj, T_Callback callback, void *arg);

/*!
 * @brief Check for arrived events or async commands
 * 
 * @param[in] obj   Pointer to the object initialized in ::T_Init function.
 * 
 * @return ::Template_StatusCode
 */
bool T_Run (T_Obj *obj);

/*!@}*/

/*! @defgroup CmdAPI Command API
 *  @brief This API is used to implement a Client.
 *  @ingroup API
 *  @{
 */
/*!
 * @brief Used to send the cmd1.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::T_Init function.
 * @param[in]   data  Pointer to the struct to send the data.
 *    
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Command1(T_Obj *obj, st_cmd1* data);

/*!
 * @copydoc T_Command1
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered
 * with the function ::T_RegisterResponseCallback
 */
Template_StatusCode T_Command1Async(T_Obj *obj, st_cmd1* data);

/*!
 * @brief Used to send the cmd2.
 * 
 * @param[in]    obj   Pointer to the object initialized in ::T_Init function.
 * @param[out]   data  Pointer to the struct with the data.
 *
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Command2(T_Obj *obj, st_cmd2* data);

/*!
 * @copydoc T_Command1
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered
 * with the function ::T_RegisterResponseCallback
 */
Template_StatusCode T_Command2Async(T_Obj *obj);

/*!@}*/

/*! @defgroup RespAPI Response API
 * 	@brief This API is used to implement a Server
 *  @ingroup API
 *  @{
 */

/*!
 * @brief Used to send a response to the cmd1.
 *
 * @param[in]  obj   Pointer to the object initialized in ::T_Init function.
 * @param[in]  data  Pointer to the struct to receive the data.
 *
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Response1(T_Obj *obj, Template_StatusCode statusCode);

/*!
 * @brief Used to send a response to the cmd2.
 *
 * @param[in]  obj   Pointer to the object initialized in ::T_Init function.
 * @param[in]  data  Pointer to the struct to receive the data.
 *
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Response2(T_Obj *obj, Template_StatusCode statusCode, st_cmd2* data);


/*!@}*/

/*! @defgroup EvtAPI Event API
 *  @ingroup API
 *  @{
 */

/*!
 * @brief Used to warn.
 *
 * @param[in]  obj   Pointer to the object initialized in ::T_Init function.
 * @param[in]  data  Pointer to the struct with the data.
 *
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Event1(T_Obj *obj, st_cmd1* data);

/*!
 * @brief Used to warn.
 *
 * @param[in]  obj   Pointer to the object initialized in ::T_Init function.
 * @param[in]  data  Pointer to the struct with the data.
 *
 * @return ::Template_StatusCode
 */
Template_StatusCode T_Event2(T_Obj *obj, st_cmd2* data);

/*! @}*/

#endif
