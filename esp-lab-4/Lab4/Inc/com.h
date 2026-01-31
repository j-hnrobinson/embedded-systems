#ifndef __COM_H
#define __COM_H

#include "stm32l4xx_hal.h"

/* Definition for COM port */
#define DISCOVERY_COM1                     USART1
#define DISCOVERY_COM1_CLK_ENABLE()        __HAL_RCC_USART1_CLK_ENABLE()

#define DISCOVERY_COM1_TX_PIN              GPIO_PIN_6
#define DISCOVERY_COM1_TX_GPIO_PORT        GPIOB
#define DISCOVERY_COM1_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISCOVERY_COM1_TX_AF               GPIO_AF7_USART1

#define DISCOVERY_COM1_RX_PIN              GPIO_PIN_7
#define DISCOVERY_COM1_RX_GPIO_PORT        GPIOB
#define DISCOVERY_COM1_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISCOVERY_COM1_RX_AF               GPIO_AF7_USART1

extern UART_HandleTypeDef hDiscoUart;

void BSP_COM_Init(UART_HandleTypeDef *huart);

#endif
