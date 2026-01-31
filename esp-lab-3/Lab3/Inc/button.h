#ifndef BUTTON_H
#define BUTTON_H

#include "stm32l4xx_hal.h"

#define BUTTON_PIN        GPIO_PIN_13
#define BUTTON_GPIO_PORT  GPIOC
#define BUTTON_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

void Button_Init(void);
GPIO_PinState Button_Read(void);

#endif
