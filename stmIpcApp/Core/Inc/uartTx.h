//******************************* uartTx **************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
//
// Summary : Include files declared & forward declarations are included
// Note    : None
//
//*****************************************************************************
#ifndef _UART_TX_H_
#define _UART_TX_H_

//***************************** Include Files *********************************
#include "common.h"
#include "main.h"
#include <app_freertos.h>

//***************************** Global Types **********************************

//**************************** Global Constants *******************************

//**************************** Global Variables *******************************

//************************** Forward Declarations *****************************
bool uartTxBegin(UART_HandleTypeDef *huart);

//*********************** Inline Method Implementations ***********************

#endif // _UART_TX_H_
// EOF
