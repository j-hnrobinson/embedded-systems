/**
  ******************************************************************************
  * @file    BSP/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  ******************************************************************************

  * @brief  Main program
  * @param  None
  * @retval None
  */
#include <stdio.h>

int i = 0;
int off = 5;

void inc(void){
  i += off;
}
int main(void)
{
    printf("Welcome to the ESP Lab 1 test program \n");
    printf("this program does next to nothing!\n");
    
  /* Infinite loop */
   while (1) {
    inc();
  }
}


