/*!
 * @file main.c
 *
 *  @date Jul 11, 2020
 *  @author Douglas Reis
 */

#include <DVP.h>
#include "Porting.h"
#include <stdio.h>

#define TIMEOUT 1000

typedef struct
{
	DVP_Obj obj;
	bool running;
	uint8_t payload[1024];
	uint32_t size;
	uint32_t timeout;
}ClassServerTest;

DVP_VehicleStatus status = {
		.wheelLock = false, .poweredOn = true, .cruiseOn = false, .buzzerOn = false, .tailLightOn = true, .headLightOn = true, .speed = 3315
};

DVP_VehicleConfig config = {
		.brakeLevel = 5, .throttleLevel = 5, .cruiseEnabled = true, .startSpeed=300, .speedLimit=2700
};

DVP_Info vehicleInfo = {
		.firmwareVersion = {0x01, 0x02, 0x53, 0x09},
		.serialnumber = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14}
};

DVP_BatteryStatus batStatus = {
		.isCharging = false, .charge = 79, .temperature = 62, .voltage = 3988
};

DVP_Info batInfo = {
		.firmwareVersion = {0x01, 0x00, 0x03, 0x29},
		.serialnumber = {0x00, 0x00, 0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14}
};

DVP_AuthenticationData auth =
{
		.size = 32,
		.AuthData = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10}
};

DVP_Driver driver =
{
		.Open = UART_Open,
		.Write = UART_Write,
		.Read = UART_Read,
		.Close = UART_Close,
		.Flush = UART_Flush,
		.Tick = SYS_Tick,
		.Sleep = SYS_Sleep
};

void Command(void *param, uint8_t address, DVP_Frame *data)
{
	ClassServerTest *test = (ClassServerTest *) param;
	switch(data->id)
	{
	case DVP_eReadVehicleStatus:
	{
		DVP_ReplyReadVehicleStatus(&test->obj, DVP_OK, &status);
	}break;
	case DVP_eWriteVehicleStatus:
	{
		memcpy(&status, &data->payload.vehicleStatus, sizeof(status));
		DVP_ReplyWriteVehicleStatus(&test->obj, DVP_OK);
	}break;
	case DVP_eReadVehicleConfig:
	{
		DVP_ReplyReadVehicleConfig(&test->obj, DVP_OK, &config);
	}break;
	case DVP_eWriteVehicleConfig:
	{
		memcpy(&config, &data->payload.vehicleConfig, sizeof(config));
		DVP_ReplyWriteVehicleConfig(&test->obj, DVP_OK);
	}break;
	case DVP_eReadVehicleInfo:
	{
		DVP_ReplyReadVehicleInfo(&test->obj, DVP_OK, &vehicleInfo);
	}break;
	case DVP_eReadBatteryStatus:
	{
		DVP_ReplyReadBatteryStatus(&test->obj, DVP_OK, &batStatus);
	}break;

	case DVP_eReadBatteryInfo:
	{
		DVP_ReplyReadBatteryInfo(&test->obj, DVP_OK, &batInfo);
	}break;

	case DVP_eFirmwareUpdateStart:
	{
		DVP_ReplyFirmwareUpdateStart(&test->obj, DVP_OK);
	}break;

	case DVP_eFirmwareUpdateLoad:
	{
		DVP_ReplyFirmwareUpdateLoad(&test->obj, DVP_OK);
	}break;
	case DVP_eFirmwareUpdateFinish:
	{
		DVP_ReplyFirmwareUpdateFinish(&test->obj, DVP_OK);
	}break;
	case DVP_eStartAuthentication:
	{
		DVP_StatusCode status = DVP_OK;
		DVP_ReplyStartAuthentication(&test->obj, status, &auth);
	}break;
	case DVP_eAuthenticate:
	{
		DVP_StatusCode status = DVP_OK;
		status = (data->payload.authenticate.size == 32) ? status : DVP_ProtocolError;
		DVP_ReplyAuthenticate(&test->obj, status);
	}break;
	case DVP_eUpdatePublicKey:
	{
		DVP_StatusCode status = DVP_OK;
		status = (data->payload.updatePublicKey.size == 64) ? status : DVP_ProtocolError;
		DVP_ReplyUpdatePublickey(&test->obj, status);
	}break;
	default:
		break;
	}
	test->timeout = SYS_Tick() + TIMEOUT;
}

void Event(void *param, uint8_t address, DVP_Frame *data)
{
	ClassServerTest *test = (ClassServerTest*) param;

	test->timeout = SYS_Tick() + TIMEOUT;
	switch(data->id)
	{
	case DVP_eKeepAlive:
	{
	}
	break;
	default:
		break;
	}
}

int main (int argc, char** argv)
{

	uint8_t buffer[1024];
	ClassServerTest test =
	{
			.running = true,
			.obj = {0}
	};

	//Disable stdout buffer to flush immediately
	setvbuf(stdout, NULL, _IONBF, 0);

	bool ret = DVP_Init(&test.obj, "server:1001", &driver, buffer, sizeof(buffer));
	if (!ret)
		goto exit;

	ret = DVP_RegisterCommandCallback(&test.obj, Command, &test);
	ret = DVP_RegisterEventCallback(&test.obj, Event, &test);


	test.timeout = SYS_Tick() + TIMEOUT;
	while(test.timeout > SYS_Tick())
	{
		ret = DVP_Run(&test.obj);
	}

	exit:
	return 0;
}
