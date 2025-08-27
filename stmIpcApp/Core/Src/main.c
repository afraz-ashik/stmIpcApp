/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os2.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
COM_InitTypeDef BspCOMInit;
UART_HandleTypeDef huart1;

/* Definitions for Poller */
osThreadId_t PvPollerHandle;
const osThreadAttr_t stPollerAttributes = {
  .name = "Poller",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Definitions for Receiver */
osThreadId_t pvReceiverHandle;
const osThreadAttr_t stReceiverAttributes = {
  .name = "Receiver",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Definitions for Poller Queue */
osMessageQueueId_t pvPollerQueueHandle;
const osMessageQueueAttr_t stPollerQueueAttributes = {
  .name = "pollerQueue"
};

/* Definitions for Receiver Queue */
osMessageQueueId_t pvReceiverQueueHandle;
const osMessageQueueAttr_t stReceiverQueueAttributes = {
  .name = "receiverQueue"
};

osMutexId_t pvPollerMutex;
osMutexId_t pvReceiverMutex;

osSemaphoreId_t pvPollerSem;
osSemaphoreId_t pvReceiverSem;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void SystemPower_Config(void);
void MX_FREERTOS_Init(void);
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);
static bool tasksUid(uint32 *pulId);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the System Power */
  SystemPower_Config();

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in app_freertos.c) */
  MX_FREERTOS_Init();

  /* Initialize leds */
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_BLUE);
  BSP_LED_Init(LED_RED);

  /* Initialize USER push-button, will be used to trigger an interrupt each time it's pressed.*/
  BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity */
  BspCOMInit.BaudRate   = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits   = COM_STOPBITS_1;
  BspCOMInit.Parity     = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl  = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE)
  {
    Error_Handler();
  }

  /*Initialize UART1*/
  MX_USART1_Init(&huart1, &BspCOMInit);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_4;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Power Configuration
  * @retval None
  */
static void SystemPower_Config(void)
{
  HAL_PWREx_EnableVddIO2();

  /*
   * Switch to SMPS regulator instead of LDO
   */
  if (HAL_PWREx_ConfigSupply(PWR_SMPS_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }
/* USER CODE BEGIN PWR */
/* USER CODE END PWR */
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache in 1-way (direct mapped cache)
  */
  if (HAL_ICACHE_ConfigAssociativityMode(ICACHE_1WAY) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PG2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI13_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI13_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
	pvPollerMutex = osMutexNew(NULL);
	pvReceiverMutex = osMutexNew(NULL);

  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	pvPollerSem = osSemaphoreNew(1, 1, NULL);
	pvReceiverSem = osSemaphoreNew(1, 0, NULL);

  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	pvPollerQueueHandle = osMessageQueueNew(MAX_MESG_COUNT, TWICE*(sizeof(ACK)),
                                         &stPollerQueueAttributes);
	pvReceiverQueueHandle = osMessageQueueNew(MAX_MESG_COUNT, TWICE*
						(sizeof(REQUEST)), &stReceiverQueueAttributes);

  /* USER CODE END RTOS_QUEUES */
  /* creation of Poller */
	PvPollerHandle = osThreadNew(tasksPoller, NULL, &stPollerAttributes);

  /* creation of Receiver */
  pvReceiverHandle = osThreadNew(tasksReceiver, NULL, &stReceiverAttributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}
/* USER CODE BEGIN Header_tasksPoller */
//******************************.tasksPoller.**********************************
// Purpose : To Reads a GPIO switch and sends a Request message to Transport.
// Inputs  : None.
// Outputs : None.
// Return  : None.
// Notes   : None.
//*****************************************************************************
/* USER CODE END Header_tasksPoller */
void tasksPoller(void *argument)
{
  /* USER CODE BEGIN Poller */
  uint32 ulUID = 0;
  REQUEST stReq = ST_INITIALIZER;
  ACK stAck = ST_INITIALIZER;

  /* Infinite loop */
  for(;;)
  {
	memset(&stReq, 0, sizeof(stReq));

	osSemaphoreAcquire(pvPollerSem, osWaitForever);

	while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13))
	{
		// Wait for Button press
	}

	printf("[POLLER]   BUTTON PRESS DETECTED\r\n");

	// Increment UID
	if (true != tasksUid
  (&ulUID))
	{
		printf("Failed to increment UID!\n");
	}

	stReq.ulUID = ulUID;
	stReq.ucCMD = SET_CMD;
	stReq.ulData = DEC_ONE;

	osMutexAcquire(pvPollerMutex, osWaitForever);

	if (osOK != osMessageQueuePut(pvReceiverQueueHandle, &stReq, 0, MIN_TIME))
	{
		printf("Failed to send message to receiver\r\n");
	}

	osMutexRelease(pvPollerMutex);

	osSemaphoreRelease(pvReceiverSem);

	osDelay(ONE_SECOND);

	osSemaphoreAcquire(pvPollerSem, osWaitForever);

	osMutexAcquire(pvReceiverMutex, osWaitForever);

	if (osOK == osMessageQueueGet(pvPollerQueueHandle, &stAck, 0, MAX_TIME))
	{
		printf("[POLLER]   ACKNOWLEDGEMENT RECIEVED FROM RECEIVER\r\n");
		printf("[POLLER]   | UID:%04lu | ", stAck.ulUID);
		printf("CMD:0x%02x |", stAck.ucCMD);
		printf(" STATE:0x%02x | ", stAck.ucState);
		printf("DATA:0x%02lu |\r\n", stAck.ulData);
	}
	else
	{
		printf("Failed to receive message from transport\r\n");
	}

	if (OK_STATE == stAck.ucState)
	{
		printf("[POLLER]   LED STATUS->ON\r\n\n");
	}
	else
	{
		printf("[POLLER]   LED STATUS->OFF\r\n\n");
	}

	osMutexRelease(pvReceiverMutex);

	osSemaphoreRelease(pvPollerSem);
	}
  /* USER CODE END Poller */
}

/* USER CODE BEGIN Header_tasksReceiver */
//******************************.tasksReceiver.********************************
// Purpose : To receive message and change LED states and acknowledge the IPC.
// Inputs  : None.
// Outputs : None.
// Return  : None.
// Notes   : None.
//*****************************************************************************
/* USER CODE END Header_taksReceiver */
void tasksReceiver(void *argument)
{
  /* USER CODE BEGIN Receiver */
  REQUEST stReq = ST_INITIALIZER;
  ACK stAck = ST_INITIALIZER;

  /* Infinite loop */
  for(;;)
  {
	memset(&stReq, 0, sizeof(stReq));

	osSemaphoreAcquire(pvReceiverSem, osWaitForever);

	osMutexAcquire(pvPollerMutex, osWaitForever);

	osDelay(ONE_SECOND);

	if (osOK == osMessageQueueGet(pvReceiverQueueHandle, &stReq, 0, MAX_TIME))
	{
		printf("[RECEIVER] REQUEST RECIEVED FROM POLLER\r\n");
		printf("[RECEIVER] | UID:%04lu | CMD:0x%02x | DATA:0x%02lu |\r\n",
				stReq.ulUID, stReq.ucCMD, stReq.ulData);
	}
	else
	{
		printf("Failed to receive message from poller\r\n");
	}

	osMutexRelease(pvPollerMutex);
	if
	osMutexAcquire(pvReceiverMutex, osWaitForever);

	stAck.ucCMD = ACK_CMD;
	stAck.ulUID = stReq.ulUID;

	if (SET_CMD == stReq.ucCMD)
	{
      if (true == ledBlinkToggle())
      {
    	  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7))
    	  {
			  stAck.ucState = ERR_STATE;
			  stAck.ulData = 0;
    	  }
    	  else
    	  {
    		  stAck.ucState = OK_STATE;
    		  stAck.ulData = stReq.ulData;
    	  }
      }

      if (osOK != osMessageQueuePut(pvPollerQueueHandle, &stAck, 0, MIN_TIME))
      {
    	  printf("Failed to send message to receiver\r\n");
      }
	}
	osMutexRelease(pvReceiverMutex);

	osSemaphoreRelease(pvPollerSem);

	osDelay(ONE_SECOND);
  }

  /* USER CODE END Receiver */
 }

/* Private application code -------------------------------------------------*/
/* USER CODE BEGIN Application */

//******************************.tasksUid.*************************************
// Purpose : To increment UIDs for each transaction.
// Inputs  : pulId - pointer to the UID.
// Outputs : pulId - pointer to the UID.
// Return  : false if failed invalid arguments passed, else true.
// Notes   : None.
//*****************************************************************************
static bool tasksUid(uint32 *pulId)
{
    static uint32 sulId = 0;
    bool blResult = false;

    do
    {
        if (NULL == pulId)
        {
            perror("Invalid arguments!");

            break;
        }

        sulId++;
        *pulId = sulId;

        blResult = true;

    } while (true != blResult);

    return blResult;
}

/* USER CODE END Application */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
