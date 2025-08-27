//******************************** uartTx *************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : uartTx.c
// Summary : Transmission using UART.
// Note    : None.
// Author  : Afraz Ashik
// Date    : 31/JULY/25
//*****************************************************************************

//***************************** Include Files *********************************
#include "uartTx.h"
//***************************** Local Types ***********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//***************************** Local Functions *******************************

//*****************************************************************************

//********************************.uartTxBegin.********************************
// Purpose : To Transmit via UART of stm32 dev kit.
// Inputs  : huart - contains information of selected UART.
// Outputs : None.
// Return  : false if failed to transmit via UART, else true.
// Notes   : None.
//*****************************************************************************
bool uartTxBegin(UART_HandleTypeDef *huart)
{
	bool blResult = false;
	uint8 ucText[] = "POLLER\n";

	do
	{
		if (NULL == huart)
		{
			perror("Invalid argument");

			break;
		}

		if (HAL_OK != HAL_UART_Transmit(huart, ucText, sizeof(ucText)-1, ONE_SECOND))
		{
			perror("Failed to transmit via UART!");

			break;
		}

		blResult = true;

	} while (true != blResult);

	return blResult;
}

// EOF
