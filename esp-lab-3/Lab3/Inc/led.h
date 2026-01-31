#ifndef LED_H
#define LED_H
#include "stm32l4xx_hal.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} LED_t;


// led 1 + 2 definitions changes
#define LED2_PIN            GPIO_PIN_14
#define LED2_GPIO_PORT      GPIOB
#define LED2_GPIO_CLK_ENABLE() __HAL_RCC_GPIOB_CLK_ENABLE()
#define LED1_PIN            GPIO_PIN_5 // change
#define LED1_GPIO_PORT      GPIOA
#define LED1_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()


// functions prototypes
void LED_On(LED_t *led);
void LED_Off(LED_t *led);
void LED_Toggle(LED_t *led);

void LED2_Init(void);
void LED2_On(void);
void LED2_Off(void);
void LED2_Toggle(void);
// add functions for led1
void LED1_Init(void);
void LED1_On(void);
void LED1_Off(void);


#endif
