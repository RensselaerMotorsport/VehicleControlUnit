/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f7xx_it.h"
#include "FreeRTOS.h"
#include "task.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../Inc/Sensors/AnalogSensor.h"
#include "../Inc/Sensors/DigitalSensor.h"
#include "../Inc/Outputs/DigitalOutput.h"
#include "../Inc/Utils/Telemetry.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// Receive message buffers
CAN_RxHeaderTypeDef RxHeader1;
CAN_RxHeaderTypeDef RxHeader2;
uint8_t RxData1[8];
uint8_t RxData2[8];

// UART config reception buffer
static uint8_t uart_rx_char;
static char uart_cmd[16];  // Only need "CONFIG_REQUEST"
static uint8_t cmd_index = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern DMA_HandleTypeDef hdma_adc2;
extern DMA_HandleTypeDef hdma_adc3;
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern DMA_HandleTypeDef hdma_dac1;
extern DMA_HandleTypeDef hdma_dac2;
extern DMA_HandleTypeDef hdma_sdmmc1_rx;
extern DMA_HandleTypeDef hdma_sdmmc1_tx;
extern SD_HandleTypeDef hsd1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M7 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif /* INCLUDE_xTaskGetSchedulerState */
  xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState == 1 )
  }
#endif /* INCLUDE_xTaskGetSchedulerState */
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F7xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f7xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac1);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */

  /* USER CODE END DMA1_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_dac2);
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles CAN1 RX0 interrupts.
  */
void CAN1_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN1_RX0_IRQn 0 */

  /* USER CODE END CAN1_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan1);
  /* USER CODE BEGIN CAN1_RX0_IRQn 1 */

  /* USER CODE END CAN1_RX0_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */

  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */

  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles SDMMC1 global interrupt.
  */
void SDMMC1_IRQHandler(void)
{
  /* USER CODE BEGIN SDMMC1_IRQn 0 */

  /* USER CODE END SDMMC1_IRQn 0 */
  HAL_SD_IRQHandler(&hsd1);
  /* USER CODE BEGIN SDMMC1_IRQn 1 */

  /* USER CODE END SDMMC1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream0 global interrupt.
  */
void DMA2_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream0_IRQn 0 */

  /* USER CODE END DMA2_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA2_Stream0_IRQn 1 */

  /* USER CODE END DMA2_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc3);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc2);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream3 global interrupt.
  */
void DMA2_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream3_IRQn 0 */

  /* USER CODE END DMA2_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_sdmmc1_rx);
  /* USER CODE BEGIN DMA2_Stream3_IRQn 1 */

  /* USER CODE END DMA2_Stream3_IRQn 1 */
}

/**
  * @brief This function handles CAN2 RX0 interrupts.
  */
void CAN2_RX0_IRQHandler(void)
{
  /* USER CODE BEGIN CAN2_RX0_IRQn 0 */

  /* USER CODE END CAN2_RX0_IRQn 0 */
  HAL_CAN_IRQHandler(&hcan2);
  /* USER CODE BEGIN CAN2_RX0_IRQn 1 */

  /* USER CODE END CAN2_RX0_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_sdmmc1_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/* USER CODE BEGIN 1 */
volatile uint32_t timer_flag = 0;

void initUartConfigListener(void) {
    // Start UART interrupt reception
    HAL_UART_Receive_IT(&huart3, &uart_rx_char, 1);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        timer_flag++;
    } else if (htim->Instance == TIM2) {
        // OUTPUTS
        // D10
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, digital_out_buffer[0]);
        // D9
        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, digital_out_buffer[1]);
        // D8
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, digital_out_buffer[2]);
        // D7
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, digital_out_buffer[3]);
        // D6
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9 , digital_out_buffer[4]);
        // D5
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, digital_out_buffer[5]);
        // D4 for I2C
        // HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, digital_out_buffer[6]);
        // D3
        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, digital_out_buffer[6]);
        // D2
        // HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, digital_out_buffer[8]);
        // D1
        // HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, digital_out_buffer[9]);
        // D0
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, digital_out_buffer[7]);

        // INPUTS
        // D42
        digital_in_buffer[0] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_8);
        // D41
        digital_in_buffer[1] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7);
        // D40
        digital_in_buffer[2] = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_10);
      }
}

// Polling for sending CAN messages
int send_CAN_message_helper(CANBus bus, CAN_TxHeaderTypeDef *TxHeader, uint8_t *data)
{
	uint32_t mailbox;
    if (bus == CAN_1) {
        if (HAL_CAN_AddTxMessage(&hcan1, TxHeader, data, &mailbox) != HAL_OK) {
        	uint32_t error = HAL_CAN_GetError(&hcan1);
			//printf("CAN1 Transmission Error: %lx\n", error);
            return -1;
        }
    } else if (bus == CAN_2) {
        if (HAL_CAN_AddTxMessage(&hcan2, TxHeader, data, &mailbox) != HAL_OK) {
        	uint32_t error = HAL_CAN_GetError(&hcan2);
			//printf("CAN2 Transmission Error: %lx\n", error);
            return -1;
        }
    } else {
        return -1;
    }
    return 0;
}

// Callback for receiving CAN messages
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    if (hcan == &hcan1) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader1, RxData1);
        receive_CAN_message(&RxHeader1, RxData1, CAN_1);
    } else if (hcan == &hcan2) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader2, RxData2);
        receive_CAN_message(&RxHeader2, RxData2, CAN_2);
    }
}

// Callback for UART reception complete
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3) {
        if (uart_rx_char == '\n' || uart_rx_char == '\r') {
            // Check if it's CONFIG_REQUEST
            uart_cmd[cmd_index] = '\0';
            if (strcmp(uart_cmd, "CONFIG_REQUEST") == 0) {
                handleTelemetryConfigRequest();
            }
            cmd_index = 0;
        }
        else if (uart_rx_char >= 'A' && uart_rx_char <= 'Z' && cmd_index < 15) {
            // Only store uppercase letters for CONFIG_REQUEST
            uart_cmd[cmd_index++] = uart_rx_char;
        }
        else if (uart_rx_char == '_' && cmd_index < 15) {
            // Store underscore for CONFIG_REQUEST
            uart_cmd[cmd_index++] = uart_rx_char;
        }
        else if (uart_rx_char < 'A' || uart_rx_char > 'Z') {
            // Reset on any invalid character
            cmd_index = 0;
        }
        
        // Re-enable interrupt
        HAL_UART_Receive_IT(&huart3, &uart_rx_char, 1);
    }
}

// Callback for ADC conversion complete
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
  if (hadc == &hadc1) {
    ProcessADCData(adc1_buffer, adc2_buffer, adc3_buffer);
  } else if (hadc == &hadc2) {
    ProcessADCData(adc1_buffer, adc2_buffer, adc3_buffer);
  } else if (hadc == &hadc3) {
    ProcessADCData(adc1_buffer, adc2_buffer, adc3_buffer);
  }
}

/* USER CODE END 1 */
