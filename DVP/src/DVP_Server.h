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

#ifndef DVP_ServerH_
#define DVP_ServerH_

#include <stdint.h>
#include <stdbool.h>

#include "PacketID.h"
#include "Frame.h"
#include "StatusCode.h"

/*! @defgroup RespAPI Response API
 * 	@brief This API is used to implement a Server
 *  @ingroup API
 *  @{
 */

/*!
 * @brief Respond the Read Vehicle Status command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 * @param[in]  data       Pointer to the struct containing the data to be replayed.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyReadVehicleStatus(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_VehicleStatus* data);

/*!
 * @brief Respond the Write Vehicle Status command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyWriteVehicleStatus(DVP_Obj *obj, DVP_StatusCode statusCode);

/*!
 * @brief Respond the Read Vehicle Config command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 * @param[in]  data       Pointer to the struct containing the data to be replayed.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyReadVehicleConfig(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_VehicleConfig* data);

/*!
 * @brief Respond the Write Vehicle Config command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyWriteVehicleConfig(DVP_Obj *obj, DVP_StatusCode statusCode);

/*!
 * @brief Respond the Read Vehicle Information command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 * @param[in]  data       Pointer to the struct containing the data to be replayed.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyReadVehicleInfo(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_Info* data);

/*!
 * @brief Respond the Read Battery Status command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 * @param[in]  data       Pointer to the struct containing the data to be replayed.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyReadBatteryStatus(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_BatteryStatus* data);

/*!
 * @brief Respond the Read Battery Information command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 * @param[in]  data       Pointer to the struct containing the data to be replayed.
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyReadBatteryInfo(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_Info* data);

/*!
 * @brief Respond the Firmware Update Start command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyFirmwareUpdateStart(DVP_Obj *obj, DVP_StatusCode statusCode);

/*!
 * @brief Respond the Firmware Update Load command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyFirmwareUpdateLoad(DVP_Obj *obj, DVP_StatusCode statusCode);

/*!
 * @brief Respond the Firmware Update Finish command.
 *
 * @param[in]  obj        Pointer to the object initialized in ::DVP_Init function.
 * @param[in]  statusCode Status for the operation. See:: ::DVP_StatusCode for the available values
 *
 * @return ::DVP_StatusCode
 */
DVP_StatusCode DVP_ReplyFirmwareUpdateFinish(DVP_Obj *obj, DVP_StatusCode statusCode);

/*!@}*/

#endif
