/*
 * Porting.c
 *
 *  Created on: May 20, 2019
 *      Author: Douglas Reis
 */

//https://stackoverflow.com/questions/42597685/storage-size-of-timespec-isnt-known
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <strings.h>

#include "Porting.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include "circular_buffer.h"

typedef struct
{
	int socketfd;
	int connfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int len;
	circularBuffer_t ringBuffer;
}OS_Porting;

char ring_buf[1024];

OS_Porting op = { 0 };

void dumpBuffer(char *buffer, int size)
{
	for (int i=0; i<size; i++) {
	    printf("%02x ", buffer[i]);
	}
}

void * UART_Open(const void *port)
{
	char url[32] = {0};
	int portnum = 0;

	CircBufInit(&op.ringBuffer, (unsigned char*)ring_buf, sizeof(ring_buf), NULL);

	sscanf((char*)port, "%[^:]:%u", url, &portnum);

	op.socketfd = socket(AF_INET, SOCK_DGRAM, 0);
	memset(&op.serv_addr, '0', sizeof(op.serv_addr));

	op.serv_addr.sin_family = AF_INET;
	op.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	op.serv_addr.sin_port = htons(portnum);

	if(strcmp(url, "server") == 0)
	{
		if(bind(op.socketfd, (struct sockaddr*)&op.serv_addr, sizeof(op.serv_addr)) < 0)
		{
			return 0;
		}

//		listen(op.socketfd, 5);
//
//		op.connfd = accept(op.socketfd, (struct sockaddr*)NULL, NULL);
		op.connfd = op.socketfd;
	}
	else
	{
		struct hostent *hp;

		hp = gethostbyname("localhost");
		bzero((char *)&op.cli_addr, sizeof(op.cli_addr));
		op.cli_addr.sin_family = AF_INET;
		memcpy(&(op.cli_addr.sin_addr),  hp->h_addr, hp->h_length);
		op.cli_addr.sin_port = htons(portnum);

//		if( connect(op.socketfd, (struct sockaddr *)&op.cli_addr, sizeof(op.cli_addr)) < 0)
//		{
//			printf("\n Error : Connect Failed \n");
//			return 0;
//		}
		op.connfd = op.socketfd;
	}

	return &op;
}

uint16_t UART_Write(void *handle, const void *buffer, uint16_t size)
{
//	dumpBuffer((char*)buffer, size);
	size = sendto(op.connfd, (const char *)buffer, size, MSG_WAITALL , (const struct sockaddr *)&op.cli_addr, sizeof(op.cli_addr));

	return size;
}

uint16_t UART_Read(void *handle, void *buffer, uint16_t size)
{

	char _buffer[1024];
	op.len = sizeof(op.cli_addr);

	int count;
	ioctl(op.connfd, FIONREAD, &count);
	if(count > 0)
	{
		count = recvfrom(op.connfd, _buffer, count, 0, (struct sockaddr *)&op.cli_addr, (socklen_t *)&op.len);
		if (count < 0 )
		{
			char errnoMessage[50] = {0};
			snprintf(errnoMessage, sizeof(errnoMessage), "%s", strerror(errno));
			printf("%s", errnoMessage);
		}
		else
		{
			CircBufPut(&op.ringBuffer, (unsigned char* )_buffer, count);
		}
	}

	return CircBufGet(&op.ringBuffer,buffer, size);
}

uint16_t UART_Close(void *handle)
{

	close(op.connfd);//Closing the Serial Port
	return 0;
}

void     UART_Flush(void *handle)
{
	//uint8_t byte;
	//while( UART_Read(handle, &byte, 1) > 0);
}

uint32_t SYS_Tick()
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000 + ts.tv_nsec / (1000 * 1000);
}

void SYS_Sleep(uint32_t time)
{
	sleep(time);
}

