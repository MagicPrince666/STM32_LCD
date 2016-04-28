/*********************2016. 04. 12*******************************************************************
file name:			led.c
description:		the very first piece of code to knock embeded system: flash a led, equavelence of "Hello, world"
H/W: 						D1 to LED1(red bull)

author 					:hiway

***********************************************************************************************/


#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
#include "led.h"

void led_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitTypeDef GPIO_InitStructure2;
	
	// enable GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOG, ENABLE);
	
	// select pin to operate
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13; 
	
	// port mode 
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// Pin Clk Freq
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	

	//initialize GPIO port F by loading the structure set above:
	GPIO_Init(GPIOD, &GPIO_InitStructure1);
	
	// turn all LEDs off
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
	
	// select pin to operate
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_14; 
	
	// port mode 
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// Pin Clk Freq
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	

	//initialize GPIO port F by loading the structure set above:
	GPIO_Init(GPIOG, &GPIO_InitStructure2);
	
	// turn all LEDs off
	GPIO_SetBits(GPIOG, GPIO_Pin_14);
        
}

void button_gpio_config(void)
{
		GPIO_InitTypeDef GPIO_InitStructure1;
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure; 
        
		// enable GPIOD clock
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB1Periph_TIM2 | RCC_APB2Periph_GPIOC, ENABLE);
	
		// select pin to operate
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0; 
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOE, &GPIO_InitStructure1);

    // select pin to operate
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_13; 
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOC, &GPIO_InitStructure1);
        
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
        
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
        
        
        
    NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);  
        
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource13);
        
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
        
        
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
