/**
 * @file    Result.h
 * @author  Douglas Reis
 * @date    30/05/2019
 * @version 1.0
 *
 * @author Eng. Douglas Reis
 * @section LICENSE
 * @section DESCRIPTION
 */

#ifndef SRC_DVP_StatusCode_H_
#define SRC_DVP_StatusCode_H_

/**
 * @defgroup StatusCode Status Code
 * @ingroup CommandProtocol
 * @{
 * @par Status Code
 *  |Name                     |Value |
 *  |:--                      |:--:  |
 *  |OK                       | 0x00 |
 *  |Not Available Error      | 0x01 |
 *  |Timeout Error            | 0x02 |
 *  |Protocol Error           | 0x03 |
 *  |Not Open Error           | 0x04 |
 *  |Parameter Error          | 0x05 |
 *  |Not Supported Error      | 0xFE |
 *  |General Error            | 0xFF |
 * 
 */

 /**
 * @brief result
 */ 
typedef enum __SHORT_ENUM__
{
	DVP_OK                       = 0x00,
	DVP_NotAvailableError        = 0x01,
	DVP_TimeoutError             = 0x02,
	DVP_ProtocolError            = 0x03,
	DVP_NotOpenError             = 0x04,
	DVP_ParameterError           = 0x05,
	DVP_NotSupportedError        = 0xFE,
	DVP_GeneralError             = 0xFF
}DVP_StatusCode;

///@}
#endif /* SRC_DVP_StatusCode_H_ */
