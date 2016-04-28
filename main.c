#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "led.h"
#include "tick.h"

void led_gpio_config(void);
void button_gpio_config(void);

extern unsigned short int status;
extern int delayflag;

int main(void) {
	// Add your initializing code here!
	led_gpio_config();
    button_gpio_config();

    init_tick();

	while(1) {
		// Add your code here!
		if (delayflag == 0 ) {
			if(status == 0) {
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			}
			else if(status == 1) {
				GPIO_SetBits(GPIOG, GPIO_Pin_14);
				d(5000);
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				d(5000);
			}
			else if(status == 2) {
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				d(5000);
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				d(5000);
			}
			else {
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				GPIO_SetBits(GPIOG, GPIO_Pin_14);
				d(5000);
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				d(5000);
			}
			//GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		}
		else {
			if(status == 0) {
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
			}
			else if(status == 1) {
				GPIO_SetBits(GPIOG, GPIO_Pin_14);
				d(50000);
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				d(50000);
			}
			else if(status == 2) {
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				d(50000);
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				d(50000);
			}
			else {
				GPIO_ResetBits(GPIOD, GPIO_Pin_13);
				GPIO_SetBits(GPIOG, GPIO_Pin_14);
				d(50000);
				GPIO_SetBits(GPIOD, GPIO_Pin_13);
				GPIO_ResetBits(GPIOG, GPIO_Pin_14);
				d(50000);
			}
		}
	}
}

void assert_failed(uint8_t* file, uint32_t line) {
    while (1) {

    }
}
