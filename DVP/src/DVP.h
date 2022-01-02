/*!
 * @file    DVP.h
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

#ifndef DVP_ProtocolH_
#define DVP_ProtocolH_

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
 * #include <DVP.h>
 * void Command(void *param, uint8_t address, DVP_Frame *data)
 * {	
 * 	switch(data->id)
 * 	{
 * 	case DVP_eReadVehicleStatus:
 * 	{
 * 		DVP_VehicleStatus status;
 * 		status.wheelLock = true;
 * 		status.poweredOn = false;
 * 		DVP_ReplyReadVehicleStatus(param, DVP_OK, &status);
 * 	}break;
 * 	case DVP_eReadVehicleConfig:
 * 	{
 * 		DVP_ReplyReadVehicleConfig(param, DVP_NotSupportedError, NULL);
 * 	}break;
 * 	default:
 * 		break;
 * 	}
 * }
 * 
 * void Event(void *param, uint8_t address, DVP_Frame *data)
 * {
 * 	//Do something
 * }
 * 
 * int main (int argc, char** argv)
 * {
 * 	uint8_t buffer[512];
 * 	int timeout;
 * 	DVP_Obj obj;
 * 	
 * 	bool ret = DVP_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 
 * 	ret = DVP_RegisterCommandCallback(&obj, Command, &obj);
 * 	ret = DVP_RegisterEventCallback(&obj, Event, &obj);
 * 
 * 	timeout = SYS_Tick() + TIMEOUT;
 * 	while(timeout > SYS_Tick())
 * 	{
 * 		ret = DVP_Run(&obj);
 * 	}
 * 
 * 	exit:
 * 	return 0;
 * }
 * @endcode
 * 
 * @par Client example
 * @code{.cpp}
 * #include <DVP.h>
 * int main (int argc, char** argv)
 * {
 * 	uint8_t buffer[512];
 * 	int timeout;
 * 	DVP_Obj obj;
 * 	DVP_VehicleStatus status;
 * 	
 * 	bool ret = DVP_Init(&obj, "COM3", &driver, buffer, sizeof(buffer));
 * 	if (!ret)
 * 		goto exit;
 * 	
 * 	in.field1 = 0;
 * 	in.field2 = 20;
 * 	DVP_ReadVehicleStatus(&test.obj, &status)
 * 
 * 	printf("%d \n", status.wheelLock)
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

typedef void (*DVP_Callback)(void *param, uint8_t address, DVP_Frame *data);

/*!
 * @brief Callback struct to provide access to platform resources.
 */ 
typedef struct DVP_Driver
{
	/*!
	 * @brief Open the port and return a handler to used by the functions DVP_Driver::Write DVP_Driver::Read DVP_Driver::Close
	 *
	 * @param[in] portName Name or identification to port to be opened.
	 *
	 * @return Handler to used to used by the functions DVP_Driver::Write DVP_Driver::Read DVP_Driver::Close
	 */
	void * ( *Open )(const void *portName);

	/*!
	 * @brief Write raw data to port opened by DVP_Driver::Open
	 *
	 * @param[in] handle Returned by DVP_Driver::Open.
	 * @param[in] buffer Buffer with the data to be send.
	 * @param[in] size   Amount of data so be send.
	 *
	 * @return Amount of data written.
	 */
	uint16_t ( *Write)(void *handle, const void *buffer, uint16_t size);

	/*!
	 * @brief Read raw data from port opened by DVP_Driver::Open
	 *
	 * @param[in] handle Returned by DVP_Driver::Open.
	 * @param[in] buffer Memory buffer to receive the data read.
	 * @param[in] size   Total space available in the buffer.
	 *
	 * @return Amount of data read.
	 */
	uint16_t ( *Read )(void *handle, void *buffer, uint16_t size);

	/*!
	 * @brief Close the port opened by DVP_Driver::Open
	 *
	 * @param[in] handle Returned by DVP_Driver::Open.
	 *
	 * @return 0 if ok.
	 */
	uint16_t ( *Close)(void *handle);

	/*!
	 * @brief Flush the port opened by DVP_Driver::Open
	 *
	 * @param[in] handle Returned by DVP_Driver::Open.
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
}DVP_Driver;

/*!
 * @brief Class to hold the execution context.
 *
 * @details Must be allocated by the application.
 * @code{.cpp}
 * void main(void)
 * {
 *    uint8_t buffer[1024];
 *    DVP_Obj obj;
 *    DVP_Init(&obj, "COM1", &driver, buffer, sizeof(buffer));
 * 
 * }
 * @endcode
 *
 */
typedef struct
{
	void *handle;
}DVP_Obj;

/*!
 * @brief Initialize the library
 *
 * @param[out] obj Pointer to the object which will be initialized.
 * 
 * @return ::DVP_StatusCode
 */
bool DVP_Init(DVP_Obj *obj, const void *port, DVP_Driver *driver, uint8_t * buffer, uint32_t size);

/*!
 * @brief Register a callback to receive notifications when a event arrives.
 *
 * @details The only way to receive event is by registering this callback, other wise the even
 * will be just discarded.
 * The function ::DVP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::DVP_Init function.
 * @param callback  Pointer to function to receive the events
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool DVP_RegisterEventCallback(DVP_Obj *obj, DVP_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a command arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::DVP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::DVP_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool DVP_RegisterCommandCallback(DVP_Obj *obj, DVP_Callback callback, void *arg);

/*!
 * @brief Register a callback to receive notifications when a response arrives.
 *
 * @details Setting this callback the Async functions are enabled.
 * The function ::DVP_Run must be often called to provide processing time to the library.
 *
 * @param obj       Pointer to the object initialized in ::DVP_Init function.
 * @param callback  Pointer to function to receive the responses.
 * @param arg       Pointer that will used as the first parameter of callback.
 * @return
 */
bool DVP_RegisterResponseCallback(DVP_Obj *obj, DVP_Callback callback, void *arg);

/*!
 * @brief Check for arrived events or async commands
 * 
 * @param[in] obj   Pointer to the object initialized in ::DVP_Init function.
 * 
 * @return ::DVP_StatusCode
 */
bool DVP_Run (DVP_Obj *obj);

/*!
 * @internal
 * @private
 *
 * @param[in]     obj          Pointer to the object initialized in ::DVP_Init function.
 * @param[in]     sync         True to block until the response arrives.
 * @param[in]     type         Packet type.
 * @param[in]     id           Packet ID.
 * @param[in]     statusCode   Processing status code.
 * @param[in]     payload      Content to be send.
 * @param[in]     size         Content size.
 * @param[out]    resp         Buffer to receive the response.
 * @param[in,out] respSize     IN:Size of the response buffer. OUT:Response size.
 * @return
 */
DVP_StatusCode DVP_SendGeneric(DVP_Obj *obj, bool sync, uint32_t type, uint32_t id, uint32_t statusCode, void * payload, uint32_t size, void * resp, uint32_t *respSize );

/*!@}*/

#include "DVP_Client.h"
#include "DVP_Server.h"
#include "DVP_Event.h"
#endif
