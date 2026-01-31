#include "led.h"

void LED2_Init(void)
{
    GPIO_InitTypeDef gpio;

    LED2_GPIO_CLK_ENABLE();

    gpio.Pin = LED2_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio);
}


void LED2_On(void)
{
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
}

void LED2_Off(void)
{
    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_RESET);
}


void LED2_Toggle(void)
{
    HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_PIN);
}

// copy led2 code for led1 changing names

void LED1_Init(void)
{
    GPIO_InitTypeDef gpio;

    LED1_GPIO_CLK_ENABLE();

    gpio.Pin = LED1_PIN;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio);
}

void LED1_On(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_SET);
}

void LED1_Off(void)
{
    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, GPIO_PIN_RESET);
}

// needed help to figure this out
void LED_On(LED_t *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

void LED_Off(LED_t *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void LED_Toggle(LED_t *led)
{
    HAL_GPIO_TogglePin(led->port, led->pin);
}
