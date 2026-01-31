#include "com.h"

/* UART handle */
UART_HandleTypeDef hDiscoUart;

void BSP_COM_Init(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_structure;

    DISCOVERY_COM1_TX_GPIO_CLK_ENABLE();
    DISCOVERY_COM1_RX_GPIO_CLK_ENABLE();
    DISCOVERY_COM1_CLK_ENABLE();

    gpio_init_structure.Pin = DISCOVERY_COM1_TX_PIN;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull = GPIO_NOPULL;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_init_structure.Alternate = DISCOVERY_COM1_TX_AF;
    HAL_GPIO_Init(DISCOVERY_COM1_TX_GPIO_PORT, &gpio_init_structure);

    gpio_init_structure.Pin = DISCOVERY_COM1_RX_PIN;
    gpio_init_structure.Alternate = DISCOVERY_COM1_RX_AF;
    HAL_GPIO_Init(DISCOVERY_COM1_RX_GPIO_PORT, &gpio_init_structure);

    huart->Instance = DISCOVERY_COM1;
    HAL_UART_Init(huart);
}

/* Retarget printf */
int putchar(int ch)
{
    while (HAL_OK != HAL_UART_Transmit(&hDiscoUart, (uint8_t *)&ch, 1, 30000))
    {
        ;
    }
    return ch;
}

/* Retarget getchar */
int getchar(void)
{
    uint8_t ch;

    while (HAL_OK != HAL_UART_Receive(&hDiscoUart, &ch, 1, 30000))
    {
        ;
    }
    return ch;
}

int __io_putchar(int ch)
{
    return putchar(ch);
}

int __io_getchar(void)
{
    return getchar();
}
