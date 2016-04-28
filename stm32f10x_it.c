/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
#include "tick.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    TimingDelayDecrement();
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
//static int state0 = 0, state1 = 0;
unsigned short int status = 0x00;
int delayflag = 0;
//int count0 = 0;
void EXTI0_IRQHandler(void)  
{  
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)  
    {   
			  //d(30000);
				delayflag = ~delayflag;
				EXTI_ClearITPendingBit(EXTI_Line0);  
    }  
				//if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0)
				//{		
					  //d(1000);
						//while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0);
						//state0 = ~state0;
						//delayflag = 1;
				//}
				/*
        while (!state0)
        {
            //GPIO_ResetBits(GPIOG, GPIO_Pin_14);
            //GPIO_SetBits(GPIOD, GPIO_Pin_13);
					  
            if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0)
						{
								//while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == 0);
								state0 = ~state0;
								delayflag = ~delayflag;
						}
						
						//{
								//if(1 == state0)
								//	state0 = 0;
								//else
								//	state0 = 1;
								//state0 = ~state0;
						//}
							
        }
				state0 = 0;
				*/
        /* Clear the EXTI line 9 pending bit */  
     
}

void EXTI15_10_IRQHandler(void)  
{  
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)  
    {  	
				  delay(100000);
				  //if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)== 0)  //#define KEY1 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5)   #define KEY_DOWN 0
             //   {
                      switch(status){
									case 0:status = 1;break;
									case 1:status = 2;break;
									case 2:status = 3;break;
									case 3:status = 0;break;  
			
                }
						//	}
				//if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0)
				//{         
				//	count0++;                        
        //  if(count0>=10000)                  
        //  {                                                                                           
							
					
       //   }      			
				//}
				//count0 =0;
        //while (!state1)
        //{
            //GPIO_SetBits(GPIOG, GPIO_Pin_14);
           // GPIO_ResetBits(GPIOD, GPIO_Pin_13);
            //if ((GPIO_ReadInputData(GPIOC) >> 13) & 1)
            //   break;
					  //if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0)
						//{
								//while(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) == 0);
						//		state1 = ~state1;
						//}
        //}
				//state1 = 0;
        /* Clear the EXTI line 9 pending bit */  
        EXTI_ClearITPendingBit(EXTI_Line13);  
				//d(1000);
    }  
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
