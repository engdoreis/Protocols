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

DVP_StatusCode DVP_KeepAlive(DVP_Obj *obj)
{
	return DVP_SendGeneric(obj, false, DVP_FrameEvent, DVP_eKeepAlive, DVP_OK, NULL, 0, NULL, 0);
}



