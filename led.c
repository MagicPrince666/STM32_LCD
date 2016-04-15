/*********************2016. 04. 12*******************************************************************
file name:			led.c
description:		the very first piece of code to knock embeded system: flash a led, equavelence of "Hello, world"
H/W: 						D1 to LED1(red bull)

author 					:hiway

***********************************************************************************************/



#include "led.h"

void led_gpio_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// enable GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	
	// select pin to operate
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	
	// port mode 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// Pin Clk Freq
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	

	//initialize GPIO port F by loading the structure set above:
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	// turn all LEDs off
	GPIO_SetBits(GPIOF, GPIO_Pin_6| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
	
}

void beep_gpio_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// enable GPIOD clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	// select pin to operate
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	
	// port mode 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	
	// Pin Clk Freq
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	

	//initialize GPIO port F by loading the structure set above:
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// turn all LEDs off
	GPIO_ResetBits(GPIOB, GPIO_Pin_2);

}


