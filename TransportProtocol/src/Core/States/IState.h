/**
 * @file    IState.h
 * @author  Douglas Reis
 * @date    05/06/2014
 * @version 1.0
 *
 * @section LICENSE
 *
 *
 * @section DESCRIPTION
 *
 *
 */

#ifndef ITPSTATE_H_
#define ITPSTATE_H_

typedef struct TP_Context TP_Context; /* Forward declaration */


typedef void (*ITPState)(TP_Context *context); /*!< Interface da classe State */


#endif /* ITPSTATE_H_ */
