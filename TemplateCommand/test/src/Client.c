/*!
 * @file main.c
 *
 *  @date Jul 11, 2020
 *  @author Douglas Reis
 */

#include <Template.h>
#include "Porting.h"
#include <stdio.h>
#include <string.h>

#define TIMEOUT 5000



typedef struct
{
	char * name;
	Template_StatusCode (*function)(T_Obj *obj, ...);
	bool isAsync;
	int dataSize;
	int respSize;
}TestCase;

TestCase TestMap[] =
{
		{"T_Command1Async" , (Template_StatusCode (*)(T_Obj *, ...))T_Command1Async, true,  sizeof(st_cmd1), 0},
		{"T_Event1"        , (Template_StatusCode (*)(T_Obj *, ...))T_Event1       , true,  sizeof(st_cmd1), 0},
		{"T_Command1"      , (Template_StatusCode (*)(T_Obj *, ...))T_Command1     , false, sizeof(st_cmd1), sizeof(st_cmd1)},
		{"T_Command2"      , (Template_StatusCode (*)(T_Obj *, ...))T_Command2     , false, sizeof(st_cmd2), sizeof(st_cmd2)},
};

TestCase * GetTestCase(char * name)
{
	for(int i = 0; i < sizeof(TestMap)/sizeof(TestMap[0]); i++)
	{
		if(strcmp(TestMap[i].name, name) == 0)
		{
			return &TestMap[i];
		}
	}
	return NULL;
}

typedef struct
{
	T_Obj obj;
	bool running;
	uint8_t payload[1024];
	Template_StatusCode status;
	uint32_t size;
	uint32_t timeout;
}ClassTest;

T_Driver driver =
{
		.Open = UART_Open,
		.Write = UART_Write,
		.Read = UART_Read,
		.Close = UART_Close,
		.Flush = UART_Flush,
		.Tick = SYS_Tick,
		.Sleep = SYS_Sleep
};


void Response(void *param, uint8_t address, T_Frame *data)
{
	ClassTest *test = (ClassTest*) param;

	test->timeout = SYS_Tick() + TIMEOUT;
	switch(data->id)
	{
	case T_Cmd1:
	{
		test->size = sizeof(data->payload.cmd1);
		memcpy(test->payload, &data->payload.cmd1, test->size);
	}break;
	default:
		break;
	}

	test->status = data->statusCode;
	test->running = false;
}

void Event(void *param, uint8_t address, T_Frame *data)
{
	ClassTest *test = (ClassTest*) param;

	test->timeout = SYS_Tick() + TIMEOUT;
	switch(data->id)
	{
	case T_Evt2:
	{
		test->size = sizeof(data->payload.cmd2);
		memcpy(test->payload, &data->payload.cmd2, test->size);
	}break;
	default:
		break;
	}

	test->status = data->statusCode;
	test->running = false;
}

int main (int argc, char** argv)
{

	uint8_t buffer[512];
	size_t size;
	ClassTest test =
	{
			.running = true,
			.obj = {0},
			.size = 0
	};

	if(argc < 4)
		return 1;

	//Disable stdout buffer to flush immediately
	setvbuf(stdout, NULL, _IONBF, 0);

	bool ret = T_Init(&test.obj, "client:1003", &driver, buffer, sizeof(buffer));
	test.timeout = SYS_Tick() + TIMEOUT;

	if (ret)
	{
		ret = T_RegisterResponseCallback(&test.obj, Response, &test);
		ret = T_RegisterEventCallback(&test.obj, Event, &test);

		printf("\nOpening: %s\n", argv[2]);
		FILE *fp = fopen(argv[2], "rb");
		if(fp)
		{
			TestCase *tc;

			printf("Test: %s\n", argv[1]);
			tc = GetTestCase(argv[1]);
			if(tc == NULL)
			{
				printf("Test %s not found\n", argv[1]);
				return 2;
			}

			size = fread(test.payload, 1, tc->dataSize, fp);
			fclose(fp);

			if(size < tc->dataSize)
			{
				printf("Wrong data size\n");
				return 1;
			}


			printf("Sending\n");
			if(tc->isAsync)
			{
				test.status = tc->function(&test.obj, test.payload);
				test.running = true;
				while(test.running)
				{
					T_Run(&test.obj);
					if(	test.timeout < SYS_Tick())
					{
						printf("Timeout\n");
						return 2;
					}
				}
			}
			else
			{
				test.status = tc->function(&test.obj, test.payload, test.payload);
				test.size = tc->respSize;
			}

			printf("\nOpening: %s\n", argv[3]);
			FILE *fp = fopen(argv[3], "w");
			if(fp)
			{
				size = fwrite(test.payload, 1, test.size, fp);
				sprintf((char *)test.payload, "->Ret[%02X]", test.status);
				size = fwrite((char *)test.payload, 1, strlen((char *)test.payload), fp);
				fclose(fp);
			}
		}
	}

	return 0;
}
