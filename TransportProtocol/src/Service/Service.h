/**
 * @file   TPService.h
 * @author Douglas Reis
 * @date   02/10/2014
 * @brief  Brief description of file.
 * @private
 * @internal
 *
 * Detailed description of file.
 */

#ifndef TPSERVICE_H_
#define TPSERVICE_H_

#include "../Core/Core.h"
#include "../Core/Helper/Helper.h"
#include "../Core/Type/Context.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void *handle;
}TP_Obj;

/*!
 * @private
 * @internal 
 * @brief Tp init
 *
 * @param obj
 * @param driver
 * @param callback
 * @param param
 * @param port
 * @param timeout
 * @param buffer
 * @param size
 * @return
 */
bool TP_Init(TP_Obj *obj, TP_Driver *driver, ITPCallback callback, void *param,const void * port, uint32_t timeout, uint8_t *buffer, uint32_t size);

/*!
 * @internal
 * @private  
 *
 * @param obj
 * @param address
 * @param payload
 * @param size
 * @return
 */
bool TP_Send(TP_Obj *obj, uint8_t address, const uint8_t *payload, uint32_t size);

/*!
 * @internal
 * @private  
 *
 * @param obj
 */
void TP_Process(TP_Obj *obj);

#ifdef __cplusplus
}
#endif

#endif /* TPSERVICE_H_ */
