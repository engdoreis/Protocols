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

#ifndef DVP_EventH_
#define DVP_EventH_

#include <stdint.h>
#include <stdbool.h>

#include "PacketID.h"
#include "Frame.h"
#include "StatusCode.h"

/*! @defgroup EvtAPI Event API
 *  @brief This API is used to send asynchronous events
 *  @ingroup API
 *  @{
 */

/*!
 * @brief The IoT sends this event every 60 seconds in order keep the vehicle working.
 *
 * @param[in]  obj   Pointer to the object initialized in ::DVP_Init function. 
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_KeepAlive(DVP_Obj *obj);

/*! @}*/

#endif
