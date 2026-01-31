/* changed by craig for lab 2*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#define BootRAM         0xF1E0F85F

// Linker supplied pointers
extern unsigned long _sidata;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

extern UART_HandleTypeDef hDiscoUart;
#ifdef __GNUC__
/* With GCC/RAISONANCE, small msg_info (option LD Linker->Libraries->Small msg_info
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
extern int main(void);
extern void _estack(void);  // to force type checking
void Reset_Handler(void);
void Hal_start(void);
void __attribute__ ((weak)) __libc_init_array (void){}

void default_handler (void) 
{
  while(1);
}

void Hal_start(void) {
    /* STM32L4xx HAL library initialization:
         - Configure the Flash prefetch, Flash preread and Buffer caches
         - Systick timer is configured by default as source of time base, but user
               can eventually implement his proper time base source (a general purpose
               timer for example or other time source), keeping in mind that Time base
               duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
               handled in milliseconds basis.
         - Low Level Initialization
       */
    HAL_Init();

    /* Configure the System clock to have a frequency of 80 MHz */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    hDiscoUart.Instance = DISCOVERY_COM1;
    hDiscoUart.Init.BaudRate = 115200;
    hDiscoUart.Init.WordLength = UART_WORDLENGTH_8B;
    hDiscoUart.Init.StopBits = UART_STOPBITS_1;
    hDiscoUart.Init.Parity = UART_PARITY_NONE;
    hDiscoUart.Init.Mode = UART_MODE_TX_RX;
    hDiscoUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hDiscoUart.Init.OverSampling = UART_OVERSAMPLING_16;
    hDiscoUart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    hDiscoUart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    BSP_COM_Init(COM1, &hDiscoUart);
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

void Reset_Handler(void) {

   unsigned long *src, *dst;

   src = &_sidata;
   dst = &_sdata;

   // Copy data initializers

    while (dst < &_edata)
      *(dst++) = *(src++);

   // Zero bss

   dst = &_sbss;
   while (dst < &_ebss)
       *(dst++) = 0;

  SystemInit();
  Hal_start();
  __libc_init_array();
  main();
  while(1) {}
}

/**
  * @brief  Gets numeric values from the Hyperterminal.
  * @param  Maximun value allowed (value)
  * @retval The character received
  */
uint32_t Serial_Scanf(uint32_t value)
{
  uint16_t tmp = 0;
  
  tmp = getchar();
  
  if (tmp > value)
  {
    printf("\n\r  !!! Please enter valid number between 0 and %lu \n", value);
    return 0xFF;
  }
  return tmp;
}

/**
  * @brief Retargets the C library msg_info function to the USART.
  * @param None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the serial port and Loop until the end of transmission */
  while (HAL_OK != HAL_UART_Transmit(&hDiscoUart, (uint8_t *) &ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @brief Retargets the C library scanf function to the USART.
  * @param None
  * @retval None
  */
GETCHAR_PROTOTYPE
{
  /* Place your implementation of fgetc here */
  /* e.g. readwrite a character to the USART2 and Loop until the end of transmission */
  uint8_t ch = 0;
  while (HAL_OK != HAL_UART_Receive(&hDiscoUart, (uint8_t *)&ch, 1, 30000))
  {
    ;
  }
  return ch;
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  printf("!!! ERROR !!!\n");
  while(1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/* Vector Table */

void NMI_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void HardFault_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void MemMange_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void BusFault_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void UsageFault_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void SVC_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void DebugMon_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void PendSV_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void SysTick_Handler (void) __attribute__ ((weak,  alias ("default_handler")));
void WWDG_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void PVD_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TAMPER_STAMP_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void RTC_WKUP_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void FLASH_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void RCC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI0_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI4_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel4_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel6_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA1_Channel7_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void ADC1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void CAN1_TX_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void CAN1_RX0_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void CAN1_RX1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void CAN1_SCE_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI9_5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM1_BRK_TIM15_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM1_UP_TIM16_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM1_TRG_COM_TIM17_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM1_CC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM4_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C1_EV_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C1_ER_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C2_EV_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C2_ER_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SPI1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SPI2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void USART1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void USART2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void USART3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void EXTI15_10_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void RTCAlarm_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DFSDM1_FLT3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM8_BRK_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM8_UP_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM8_TRG_COM_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM8_CC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void ADC3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void FSMC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SDMMC1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SPI3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void UART4_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void UART5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM6_DAC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TIM7_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel3_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel4_5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel5_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DFSDM1_FLT0_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DFSDM1_FLT1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DFSDM1_FLT2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void COMP_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void LPTIM1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void LPTIM2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void OTG_FS_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel6_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void DMA2_Channel7_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void LPUART1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void QUADSPI_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C3_EV_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void I2C3_ER_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SAI1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SAI2_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void SWPMI1_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void TSC_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void RNG_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));
void FPU_IRQHandler (void) __attribute__ ((weak,  alias ("default_handler")));


__attribute__ ((section(".isr_vector")))

void (* const g_pfnVectors[])(void) = {

	_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemMange_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
    0,
    0,
    0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_IRQHandler,
    CAN1_TX_IRQHandler,
    CAN1_RX0_IRQHandler,
    CAN1_RX1_IRQHandler,
    CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM15_IRQHandler,
	TIM1_UP_TIM16_IRQHandler,
	TIM1_TRG_COM_TIM17_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTCAlarm_IRQHandler,
    DFSDM1_FLT3_IRQHandler,
    TIM8_BRK_IRQHandler,
    TIM8_UP_IRQHandler,
    TIM8_TRG_COM_IRQHandler,
    TIM8_CC_IRQHandler,
	ADC3_IRQHandler,
	FSMC_IRQHandler,
    SDMMC1_IRQHandler,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_5_IRQHandler,
	DMA2_Channel5_IRQHandler,
    DFSDM1_FLT0_IRQHandler,
    DFSDM1_FLT1_IRQHandler,
    DFSDM1_FLT2_IRQHandler,
    COMP_IRQHandler,
    LPTIM1_IRQHandler,
    LPTIM2_IRQHandler,
    OTG_FS_IRQHandler,
    DMA2_Channel6_IRQHandler,
    DMA2_Channel7_IRQHandler,
    LPUART1_IRQHandler,
    QUADSPI_IRQHandler,
    I2C3_EV_IRQHandler,
    I2C3_ER_IRQHandler,
    SAI1_IRQHandler,
    SAI2_IRQHandler,
    SWPMI1_IRQHandler,
    TSC_IRQHandler,
	0,
    0,
    RNG_IRQHandler,
    FPU_IRQHandler

	};
