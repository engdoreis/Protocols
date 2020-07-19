/*
 ============================================================================
 Name        : DVP_ProtocolServer.c
 Author      : Douglas Reis
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include <TransportProtocol.h>

#include "PacketID.h"

#include "types.h"
#include "DVP.h"

DVP_StatusCode DVP_ReplyReadVehicleStatus(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_VehicleStatus* data)
{
	uint32_t size = (data == NULL) ? 0 : sizeof(DVP_VehicleStatus);
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eReadVehicleStatus, statusCode, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReplyWriteVehicleStatus(DVP_Obj *obj, DVP_StatusCode statusCode)
{
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eWriteVehicleStatus, statusCode, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReplyReadVehicleConfig(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_VehicleConfig* data)
{
	uint32_t size = (data == NULL) ? 0 : sizeof(DVP_VehicleConfig);
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eReadVehicleConfig, statusCode, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReplyWriteVehicleConfig(DVP_Obj *obj, DVP_StatusCode statusCode)
{
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eWriteVehicleConfig, statusCode, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReplyReadVehicleInfo(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_Info* data)
{
	uint32_t size = (data == NULL) ? 0 : sizeof(DVP_Info);
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eReadVehicleInfo, statusCode, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReplyReadBatteryStatus(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_BatteryStatus* data)
{
	uint32_t size = (data == NULL) ? 0 : sizeof(DVP_BatteryStatus);
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eReadBatteryStatus, statusCode, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReplyReadBatteryInfo(DVP_Obj *obj, DVP_StatusCode statusCode, DVP_Info* data)
{
	uint32_t size = (data == NULL) ? 0 : sizeof(DVP_Info);
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eReadBatteryInfo, statusCode, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReplyFirmwareUpdateStart(DVP_Obj *obj, DVP_StatusCode statusCode)
{
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eFirmwareUpdateStart, statusCode, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReplyFirmwareUpdateLoad(DVP_Obj *obj, DVP_StatusCode statusCode)
{
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eFirmwareUpdateLoad, statusCode, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReplyFirmwareUpdateFinish(DVP_Obj *obj, DVP_StatusCode statusCode)
{
	return DVP_SendGeneric(obj, false, DVP_FrameResponse, DVP_eFirmwareUpdateFinish, statusCode, NULL, 0, NULL, 0);
}

