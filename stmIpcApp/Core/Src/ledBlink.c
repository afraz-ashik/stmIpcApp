//******************************* ledBlink ************************************
// Copyright (c) 2025 Trenser Technology Solutions
// All Rights Reserved
//*****************************************************************************
// File    : ledBlink.c
// Summary : To blink the led within stm32.
// Note    : None.
// Author  : Afraz Ashik
// Date    : 30/JULY/25
//*****************************************************************************

//***************************** Include Files *********************************
#include "ledBlink.h"

//***************************** Local Types ***********************************

//***************************** Local Constants *******************************

//***************************** Local Variables *******************************

//***************************** Local Functions *******************************

//*****************************************************************************

//********************************.ledBlinkToggle.*****************************
// Purpose : To toggle Leds of stm32 dev kit.
// Inputs  : None.
// Outputs : None.
// Return  : True.
// Notes   : None.
//*****************************************************************************
bool ledBlinkToggle()
{
	HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_2);

	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);

	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);

	return true;
}

//********************************.ledBlinkHigh.*******************************
// Purpose : To set Led2 of stm32 dev kit to high.
// Inputs  : None.
// Outputs : None.
// Return  : True.
// Notes   : None.
//*****************************************************************************
bool ledBlinkHigh()
{
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET);

	return true;
}

//********************************.ledBlinkLow.***********************************
// Purpose : To set Led2 of stm32 dev kit to low.
// Inputs  : None.
// Outputs : None.
// Return  : True.
// Notes   : None.
//*****************************************************************************
bool ledBlinkLow()
{
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET);

	return true;
}

// EOF
