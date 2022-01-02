/*!
 * @file main.c
 *
 *  @date Jul 11, 2020
 *  @author Douglas Reis
 */

#include <TransportProtocol.h>
#include "Porting.h"

#define TIMEOUT 5000

typedef struct
{
	TP_Obj obj;
	bool running;
	uint8_t payload[1024];
	uint32_t size;
	uint32_t timeout;
}ClassServerTest;

TP_Driver driver =
{
		.Open = UART_Open,
		.Write = UART_Write,
		.Read = UART_Read,
		.Close = UART_Close,
		.Flush = UART_Flush,
		.Tick = SYS_Tick,
		.Sleep = SYS_Sleep
};

void Callback(void *param, uint8_t address, uint16_t size, uint8_t *payload)
{
	ClassServerTest *test = (ClassServerTest*) param;
	test->running = false;
	memcpy(test->payload, payload, size);
	test->size = size;
}

int main (int argc, char** argv)
{

	uint8_t buffer[1024];
	ClassServerTest test =
	{
			.running = true,
			.obj = {0}
	};

	//Disable stdout buffer to flush imediatelly
	setvbuf(stdout, NULL, _IONBF, 0);

	bool ret = TP_Init(&test.obj, &driver, Callback, &test, "server:8888", 1000, buffer, sizeof(buffer));
	test.timeout = SYS_Tick() + TIMEOUT;

	while(ret)
	{
		test.running = true;
		while(test.running)
		{
			TP_Process(&test.obj);
			if(	test.timeout < SYS_Tick())
			{
				return 1;
			}
		}

		printf("\nReplying %s\n", test.payload);
		TP_Send(&test.obj, 0, test.payload, test.size);
		test.timeout = SYS_Tick() + TIMEOUT;
	}

	return 0;
}
