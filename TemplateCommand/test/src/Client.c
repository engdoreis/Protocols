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
#include <stdarg.h>

#define TIMEOUT 5000

struct ArrayWrapper
{
	uint32_t param[64];
};

typedef Template_StatusCode (*Function)(T_Obj *, void *);
typedef struct
{
	char * name;
	Function function;
	bool isAsync;
	bool isReadable;
	int dataSize;
	char * outputFormat;
	char * inputFormat;
}TestCase;

TestCase TestMap[] =
{
	{"T_Command1"           ,(Function)T_Command1             ,false ,true ,sizeof(st_cmd1)             ,.outputFormat="Ret=%u", .inputFormat="field1=%hhu\r\n field2=%hhu\r\n"},
	{"T_Command1Async"      ,(Function)T_Command1Async        ,true  ,true ,sizeof(st_cmd1)             ,.outputFormat="Ret=%u", .inputFormat="field1=%hhu\r\n field2=%hhu\r\n"},
	{"T_Command2"           ,(Function)T_Command2             ,false ,true ,sizeof(st_cmd2)             ,.outputFormat="Ret=%u\r\n field1=%hhu\r\n field2=%hhu\r\n field3=%hu\r\n" , .inputFormat=" "},
	{"T_Event1"             ,(Function)T_Event1               ,true ,true  ,sizeof(st_cmd2)             ,.outputFormat="Ret=%u\r\n field1=%hhu\r\n field2=%hhu\r\n field3=%hu\r\n", .inputFormat="field1=%hhu\r\n field2=%hhu\r\n"},

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
	T_Frame * frame;
	Template_StatusCode status;
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
uint32_t VA_ArgsGetSize(char * format)
{
	uint32_t size = 0;
	if(memcmp(format, "ll", 2) == 0)
	{
		size = sizeof(uint64_t);
	}else if(memcmp(format, "l", 1) == 0)
	{
		size = sizeof(uint32_t);
	}else if(memcmp(format, "hh", 2) == 0)
	{
		size = sizeof(uint8_t);
	}else if(memcmp(format, "h", 1) == 0)
	{
		size = sizeof(uint16_t);
	}else if( !memcmp(format, "u", 1) || !memcmp(format, "d", 1) || !memcmp(format, "x", 1) || !memcmp(format, "X", 1) || !memcmp(format, "i", 1) )
	{
		size = sizeof(uint32_t);
	}
	return size;
}

void VA_ArgsPrintfConverter(char * format, int ret, void * stack, uint32_t * output, uint32_t size)
{

	char template[512];
	uint32_t index = 0;
	strcpy(template, format);

	char *tok = strtok(template, "%");
	tok = strtok (NULL, "%");
	output[index++] = ret;
	tok = strtok (NULL, "%");

	while(tok && index < size)
	{
		if(memcmp(tok, "ll", 2) == 0)
		{
			output[index++] = *((uint64_t *)stack);
			stack += sizeof(uint64_t);
		}else if(memcmp(tok, "l", 1) == 0)
		{
			output[index++] = *((uint32_t *)stack);
			stack += sizeof(uint32_t);
		}else if(memcmp(tok, "hh", 2) == 0)
		{
			output[index++] = *((uint8_t *)stack);
			stack += sizeof(uint8_t);
		}else if(memcmp(tok, "h", 1) == 0)
		{
			output[index++] = *((uint16_t *)stack);
			stack += sizeof(uint16_t);
		}else if( !memcmp(tok, "u", 1) || !memcmp(tok, "d", 1) || !memcmp(tok, "x", 1) || !memcmp(tok, "X", 1) || !memcmp(tok, "i", 1) )
		{
			output[index++] = *((uint32_t *)stack);
			stack += sizeof(uint32_t);
		}
		tok = strtok (NULL, "%");
	}
}

void VA_ArgsScanfConverter(char * format, void * stack, uint32_t * output, uint32_t size)
{

	char template[512];
	uint32_t index = 0;
	strcpy(template, format);

	char *tok = strtok(template, "%");
	tok = strtok (NULL, "%");

	while(tok && index < size)
	{
		output[index++] = (uintptr_t )stack;

		stack += VA_ArgsGetSize(tok);

		tok = strtok (NULL, "%");
	}
}

void Response(void *param, uint8_t address, T_Frame *data)
{
	ClassTest *test = (ClassTest*) param;

	test->timeout = SYS_Tick() + TIMEOUT;

	test->frame = data;

	test->status = data->statusCode;
	test->running = false;
}

void Event(void *param, uint8_t address, T_Frame *data)
{
	ClassTest *test = (ClassTest*) param;

	test->timeout = SYS_Tick() + TIMEOUT;

	test->frame = data;

	test->status = data->statusCode;
	test->running = false;
}

int main (int argc, char** argv)
{

	uint8_t buffer[1024], testBuffer[512];
	size_t size = 0;
	ClassTest test =
	{
			.running = true,
			.obj = {0},
	};

	if(argc < 4)
		return 1;

	//Disable stdout buffer to flush immediately
	setvbuf(stdout, NULL, _IONBF, 0);

	bool ret = T_Init(&test.obj, "client:1001", &driver, buffer, sizeof(buffer));
	test.timeout = SYS_Tick() + TIMEOUT;

	if (ret)
	{
		struct ArrayWrapper array;
		TestCase *tc;
		ret = T_RegisterResponseCallback(&test.obj, Response, &test);
		ret = T_RegisterEventCallback(&test.obj, Event, &test);


		printf("Test: %s\n ", argv[1]);
		tc = GetTestCase(argv[1]);
		if(tc == NULL)
		{
			printf("Test %s not found\n ", argv[1]);
			return 2;
		}

		printf("\n Opening: %s\n ", argv[2]);
		FILE *fp = fopen(argv[2], "rb");
		if(fp)
		{
			VA_ArgsScanfConverter(tc->inputFormat, testBuffer, array.param, sizeof(array.param) / sizeof(array.param[0]));
			int n = fscanf(fp, tc->inputFormat, array);

			fclose(fp);

			if(n < 1 && size < tc->dataSize)
			{
				printf("Wrong data size\n ");
			}
		}

		printf("Sending\n ");
		if(tc->isAsync)
		{
			test.status = tc->function(&test.obj, testBuffer);
			test.running = true;
			test.status = T_TimeoutError;
			while(test.running)
			{
				T_Run(&test.obj);
				if(	test.timeout < SYS_Tick())
				{
					printf("Timeout\n ");
					return 2;
				}
			}
			memcpy(testBuffer, test.frame->payload.raw, tc->dataSize);
		}
		else
		{
			test.status = tc->function(&test.obj, testBuffer);
		}

		printf("\n Opening: %s\n ", argv[3]);
		fp = fopen(argv[3], "w");
		if(fp)
		{
			VA_ArgsPrintfConverter(tc->outputFormat, test.status, testBuffer, array.param, sizeof(array.param) / sizeof(array.param[0]));
			fprintf(fp, tc->outputFormat, array);
			fclose(fp);
		}
	}

	return 0;
}
