#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "config.h"
/* Init Structure definition */
 DAC_InitTypeDef            DAC_InitStructure;
 DMA_InitTypeDef            DMA_InitStructure;
 TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;


 const uint16_t Sine12bit[32] = {  //?????????
                      2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                       3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                       599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
 uint32_t DualSine12bit[32];
 uint32_t Idx = 0;

void TIM_Configuration(void)
 {

    /* TIM2 Periph clock enable */


	 TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
      TIM_TimeBaseStructure.TIM_Period = 25-1;      //????25
      TIM_TimeBaseStructure.TIM_Prescaler = 1-1;    //?????1
      TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
      TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //????
     TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
 // TIM6 TRGO selection */
      TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update); //?????
}

void DAC_Configuration(void)
 {
 // DAC channel1 Configuration

   /* DAC Periph clock enable */

	 DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO; //DAC??  TIM6??
     DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //??????????
     DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;   //????????
     DAC_Init(DAC_Channel_1, &DAC_InitStructure);  //?DAC????,????????,????DMA??????CPU???
DAC_Init(DAC_Channel_2, &DAC_InitStructure);
 // Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is automatically connected to the DAC converter
      DAC_Cmd(DAC_Channel_1, ENABLE);
 DAC_Cmd(DAC_Channel_2, ENABLE);


 }
 void DMA_Configuration(void)
 {
      // Time base configuration



				DMA_DeInit(DMA2_Channel4);   //?dma???????????


      DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;  //??dma?????
       DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;
       DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //????????????
       DMA_InitStructure.DMA_BufferSize = 32;  //dma????
       DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //?????????
      DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //??????? ??
      DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //??????
       DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
       DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //?????????,???????0?,????????
       DMA_InitStructure.DMA_Priority = DMA_Priority_High;
       DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //??????????????,???????


      DMA_Init(DMA2_Channel4, &DMA_InitStructure);  //?DMA_InitStructure?????????dma??????
 // Enable DMA2 Channel4
       DMA_Cmd(DMA2_Channel4, ENABLE);  //????
 }

void GPIO_Configuration(void)
 {

   /* GPIOA Periph clock enable */

	 GPIO_InitTypeDef GPIO_InitStructure;


   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
      connected to the DAC converter. In order to avoid parasitic consumption,
      the GPIO pin should be configured in analog */
   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5;  //??DAC????,???GPIO PA4,PA5???DAC???????,
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //?????????????????,PA4 ??????
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 }

 void RCC_Configuration(void)
 {
 //????,??????????
    ErrorStatus HSEStartUpStatus;  //????????????????
    RCC_DeInit();     //??RCC???????????
    RCC_HSEConfig(RCC_HSE_ON);    //????????
    HSEStartUpStatus = RCC_WaitForHSEStartUp();   //???????????
    if(HSEStartUpStatus == SUCCESS)    //???????????
    {
       RCC_HCLKConfig(RCC_SYSCLK_Div1);   //??AHB(HCLK)??=SYSCLK
       RCC_PCLK2Config(RCC_HCLK_Div1);    //??APB2(PCLK2)?=AHB??
      RCC_PCLK1Config(RCC_HCLK_Div2);    //??APB1(PCLK1)?=AHB 1/2??
      RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //??ADC??=PCLK2 1/4

       RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //??PLL?? == ????????*9
       RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //??ADC??= PCLK2/4
       RCC_PLLCmd(ENABLE);  //??PLL??
      while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) //??PLL????
       {
        }
       RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//?????? = PLL??

       while(RCC_GetSYSCLKSource() != 0x08)    //??PLL??????????
       {
        }
      }


 // DMA2 clock enable
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);


   /* GPIOA Periph clock enable */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
   /* DAC Periph clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
    /* TIM2 Periph clock enable */
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
 }
