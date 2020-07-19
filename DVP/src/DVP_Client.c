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

DVP_StatusCode DVP_ReadVehicleStatus(DVP_Obj *obj, DVP_VehicleStatus* data)
{
	uint32_t size = (data==NULL)?0:sizeof(DVP_VehicleStatus);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eReadVehicleStatus, DVP_OK, NULL, 0, data, &size);
}

DVP_StatusCode DVP_ReadVehicleStatusAsync(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eReadVehicleStatus, DVP_OK, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_WriteVehicleStatus(DVP_Obj *obj, DVP_VehicleStatus* data)
{
	uint32_t size = (data==NULL)?0:sizeof(DVP_VehicleStatus);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eWriteVehicleStatus, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_WriteVehicleStatusAsync(DVP_Obj *obj, DVP_VehicleStatus* data)
{
	uint32_t size = (data==NULL)?0:sizeof(DVP_VehicleStatus);
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eWriteVehicleStatus, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReadVehicleConfig(DVP_Obj *obj, DVP_VehicleConfig *data)
{
	uint32_t size = sizeof(DVP_VehicleConfig);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eReadVehicleConfig, DVP_OK, NULL, 0, data, &size);
}

DVP_StatusCode DVP_ReadVehicleConfigAsync(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eReadVehicleConfig, DVP_OK, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_WriteVehicleConfig(DVP_Obj *obj, DVP_VehicleConfig* data)
{
	uint32_t size = sizeof(DVP_VehicleConfig);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eWriteVehicleConfig, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_WriteVehicleConfigAsync(DVP_Obj *obj, DVP_VehicleConfig* data)
{
	uint32_t size = sizeof(DVP_VehicleConfig);
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eWriteVehicleConfig, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_ReadVehicleInfo(DVP_Obj *obj, DVP_Info* data)
{
	uint32_t size = sizeof(DVP_Info);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eReadVehicleInfo, DVP_OK, NULL, 0, data, &size);
}

DVP_StatusCode DVP_ReadVehicleInfoAsync(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eReadVehicleInfo, DVP_OK, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReadBatteryStatus(DVP_Obj *obj, DVP_BatteryStatus* data)
{
	uint32_t size = sizeof(DVP_BatteryStatus);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eReadBatteryStatus, DVP_OK, NULL, 0, data, &size);
}

DVP_StatusCode DVP_ReadBatteryStatusAsync(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eReadVehicleInfo, DVP_OK, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_ReadBatteryInfo(DVP_Obj *obj, DVP_Info* data)
{
	uint32_t size = sizeof(DVP_Info);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eReadBatteryInfo, DVP_OK, NULL, 0, data, &size);
}

DVP_StatusCode DVP_ReadBatteryInfoAsync(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eReadVehicleInfo, DVP_OK, NULL, 0, NULL, 0);
}

DVP_StatusCode DVP_FirmwareUpdateStart(DVP_Obj *obj, DVP_FirmwareUpdateStartPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateStartPacket);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eFirmwareUpdateStart, DVP_OK, data, size, NULL, 0);
}


DVP_StatusCode DVP_FirmwareUpdateStartAsync(DVP_Obj *obj, DVP_FirmwareUpdateStartPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateStartPacket);
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eFirmwareUpdateStart, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_FirmwareUpdateLoad(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateLoadPacket);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eFirmwareUpdateLoad, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_FirmwareUpdateLoadAsync(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateLoadPacket);
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eFirmwareUpdateLoad, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_FirmwareUpdateFinish(DVP_Obj *obj, DVP_FirmwareUpdateFinishPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateFinishPacket);
	return DVP_SendGeneric(obj, true, DVP_FrameCommand, DVP_eFirmwareUpdateFinish, DVP_OK, data, size, NULL, 0);
}

DVP_StatusCode DVP_FirmwareUpdateFinishAsync(DVP_Obj *obj, DVP_FirmwareUpdateLoadPacket* data)
{
	uint32_t size = sizeof(DVP_FirmwareUpdateFinishPacket);
	return DVP_SendGeneric(obj, false, DVP_FrameCommand, DVP_eFirmwareUpdateFinish, DVP_OK, data, size, NULL, 0);
}
