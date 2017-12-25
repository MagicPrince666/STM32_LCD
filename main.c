#include "stm32f10x.h"
#include "core_cm3.h"
#include "led.h"
#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "adc.h"
#include "LCD.h"
#include "exti.h"
#include "spi_tft.h"


int main(void) {

    Stm32_Clock_Init(9);	//系统时钟设置72M
    delay_init(72);			//延时初始化
 	uart_init(72,115200); 	//串口1初始化
    LCD_Init();				//LCD初始化 
    LED_Init();
    EXTI_Init();
    //Lcd_Initialize();

    Adc_Init();
    int flag = 0;

    while(1) {
        switch(flag % 3) {
            case 0:
                LCD_Clear(RED);
                POINT_COLOR = WHITE;
                BACK_COLOR = BLACK;
                LCD_ShowString(0,0,240,320,16,"red");
                LED0 = 1;
                break;
            case 1:
                LCD_Clear(GREEN);
                POINT_COLOR = WHITE;
                BACK_COLOR = BLACK;
                LCD_ShowString(0,0,240,320,16,"green");
                LED1 = 1;
                break;
            case 2:
                LCD_Clear(BLUE);
                POINT_COLOR = WHITE;
                BACK_COLOR = BLACK;
                LCD_ShowString(0,0,240,320,16,"blue");
                LED0 = 0;
                LED1 = 0;
                break;
        }
        flag++;
        delay_ms(500);
    }

}
