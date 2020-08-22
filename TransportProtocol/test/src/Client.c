/*!
 * @file main.c
 *
 *  @date Jul 11, 2020
 *  @author Douglas Reis
 */

#include <TransportProtocol.h>
#include "Porting.h"

#define TIMEOUT 2000

typedef struct
{
	TP_Obj obj;
	bool running;
	uint8_t payload[1024];
	uint32_t size;
	uint32_t timeout;
}ClassTest;

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
	ClassTest *test = (ClassTest*) param;
	test->running = false;
	memcpy(test->payload, payload, size);
	test->size = size;
}



int main (int argc, char** argv)
{

	uint8_t buffer[1024];
	uint8_t testData[512];
	uint32_t size = 0;
	ClassTest test =
	{
			.running = true,
			.obj = {0}
	};

	//Disable stdout buffer to flush imediatelly
	setvbuf(stdout, NULL, _IONBF, 0);

	bool ret = TP_Init(&test.obj, &driver, Callback, &test, "client:1003", 1000, buffer, sizeof(buffer));
	test.timeout = SYS_Tick() + TIMEOUT;

	if (ret)
	{
		printf("\nOpening: %s\n", argv[2]);
		FILE *fp = fopen(argv[2], "r");
		if(fp)
		{
			size = fread(testData, 1, sizeof(buffer), fp);
			fclose(fp);

			printf("\nSending\n");
			TP_Send(&test.obj, 0, testData, size);

			test.running = true;
			while(test.running)
			{
				TP_Process(&test.obj);
				if(	test.timeout < SYS_Tick())
				{
					return 1;
				}
			}

			printf("\nOpening: %s\n", argv[3]);
			FILE *fp = fopen(argv[3], "w");
			if(fp)
			{
				size = fwrite(test.payload, 1, test.size, fp);
				fclose(fp);
			}
		}
	}

	return 0;
}
