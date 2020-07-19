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

#ifndef DVP_ClientH_
#define DVP_ClientH_

#include <stdint.h>
#include <stdbool.h>

#include "PacketID.h"
#include "Frame.h"
#include "StatusCode.h"

/*! 
 * @defgroup CmdAPI Command API
 * @brief This API is used to implement a Client.
 * @ingroup API
 * @{
 */
/*!
 * @brief Read the vehicle dynamic information.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[out]  data  Pointer to the struct to receive the data.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReadVehicleStatus(DVP_Obj *obj, DVP_VehicleStatus* data);

/*!
 * @copydoc DVP_ReadVehicleStatus
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_ReadVehicleStatusAsync(DVP_Obj *obj);

/*!
 * @brief Write the vehicle dynamic information.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[in]   data  Pointer to the struct containing the data to be send.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_WriteVehicleStatus(DVP_Obj *obj, DVP_VehicleStatus* data);

/*!
 * @copydoc DVP_WriteVehicleStatus
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_WriteVehicleStatusAsync(DVP_Obj *obj, DVP_VehicleStatus* data);

/*!
 * @brief Read the vehicle configuration.
 * 
 * @param[in]    obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[out]   data  Pointer to the struct to recieve the data.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReadVehicleConfig(DVP_Obj *obj, DVP_VehicleConfig* data);

/*!
 * @copydoc DVP_ReadVehicleConfig
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_ReadVehicleConfigAsync(DVP_Obj *obj);

/*!
 * @brief Write the vehicle configuration
 * 
 * @param[in]    obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[in]   data  Pointer to the struct containing the data to be send.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_WriteVehicleConfig(DVP_Obj *obj, DVP_VehicleConfig* data);

/*!
 * @copydoc DVP_WriteVehicleConfig
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_WriteVehicleConfigAsync(DVP_Obj *obj, DVP_VehicleConfig* data);

/*!
 * @brief Read the vehicle static information.
 * 
 * @param[in]    obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[out]   data  Pointer to the struct to recieve the data.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReadVehicleInfo(DVP_Obj *obj, DVP_Info* data);

/*!
 * @copydoc DVP_ReadVehicleInfo
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_ReadVehicleInfoAsync(DVP_Obj *obj);

/*!
 * @brief Read the vehicle dynamic information.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[out]  data  Pointer to the struct to receive the data.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReadBatteryStatus(DVP_Obj *obj, DVP_BatteryStatus* data);

/*!
 * @copydoc DVP_ReadBatteryStatus
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_ReadBatteryStatusAsync(DVP_Obj *obj);

/*!
 * @brief Read the battery static information.
 * 
 * @param[in]    obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[out]   data  Pointer to the struct to recieve the data.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReadBatteryInfo(DVP_Obj *obj, DVP_Info* data);

/*!
 * @copydoc DVP_ReadBatteryInfo
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_ReadBatteryInfoAsync(DVP_Obj *obj);

/*!
 * @brief Starts a firmware update process
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[in]   data  Pointer to the struct containing the data to be send.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_FirmwareUpdateStart(DVP_Obj *obj, DVP_FirmwareUpdateStartPacket* data);

/*!
 * @copydoc DVP_FirmwareUpdateStart
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_FirmwareUpdateStartAsync(DVP_Obj *obj, DVP_FirmwareUpdateStartPacket* data);

/*!
 * @brief The firmware binary will be divided in several packets, this command will transfer one packet at a time until the last.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[in]   data  Pointer to the struct containing the data to be send.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_FirmwareUpdateLoad(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data);

/**
 * @copydoc DVP_FirmwareUpdateLoad
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_FirmwareUpdateLoadAsync(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data);

/*!
 * @brief Indicates the end of firmware update process.
 * 
 * @param[in]   obj   Pointer to the object initialized in ::DVP_Init function.
 * @param[in]   data  Pointer to the struct containing the data to be send.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_FirmwareUpdateFinish(DVP_Obj *obj, DVP_FirmwareUpdateFinishPacket* data);

/**
 * @copydoc DVP_FirmwareUpdateFinish
 * @attention This functions doesn't block until the response arrives. The response will arrive via the callback registered 
 * with the function ::DVP_RegisterResponseCallback
 */
DVP_StatusCode DVP_FirmwareUpdateFinishAsync(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data);

/*!@}*/

#endif
