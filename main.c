


#include "stm32f10x.h"
#include "led.h"


int main(void)
{
	// Add your initializing code here!
	led_gpio_config();
	beep_gpio_config();

	while(1) {
		// Add your code here!
		GPIO_ResetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);
		GPIO_SetBits(GPIOF, GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);

		GPIO_ResetBits(GPIOF, GPIO_Pin_6 |  GPIO_Pin_8 | GPIO_Pin_10);	// fancy stuff
		GPIO_SetBits(GPIOF, GPIO_Pin_6 |  GPIO_Pin_8 |  GPIO_Pin_10);

		GPIO_ResetBits(GPIOF,GPIO_Pin_7 | GPIO_Pin_9 );
		GPIO_SetBits( GPIOF, GPIO_Pin_7 | GPIO_Pin_9 );

		GPIO_ResetBits(GPIOF,GPIO_Pin_8 );
		//GPIO_SetBits( GPIOF, GPIO_Pin_8 );

		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	}


}
