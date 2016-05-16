#include "stm32f10x.h"
#include "stm32f10x.h"
#include "ili9320.h"
#include "TOUCH_SPI2.h"
#include "core_cm3.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "tick.h"

#define DAC_DHR12R2_Address      0x40007414  //??????????????????��????????
#define ADC1_DR_Address 			   0x4001244C

/* Init Structure definition */
 DAC_InitTypeDef            DAC_InitStructure;
 ADC_InitTypeDef 			      ADC_InitStructure;
 DMA_InitTypeDef            DMA_InitStructure;
 DMA_InitTypeDef            DMA_InitStructure2;
 TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

const uint16_t Sine12bit[32] = {
                2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056, 4095, 4056,
                3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
                599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647};
const uint16_t Table2bit = 4095;

const uint16_t Sin72new[72] = {
            120, 111, 103, 94, 86, 78 , 70, 63, 56,
            49, 43, 38, 33, 29, 26, 23, 22, 20,
            20, 20, 22, 23, 26, 29, 33, 38, 43,
            49, 56, 63, 70, 78, 86, 94, 103, 111,
            120, 129, 137, 146, 154, 162, 170, 177, 184,
            191, 197, 202, 207, 211, 214, 217, 218, 220,
            220, 220, 218, 217, 214, 211, 207, 202, 197,
            191, 184, 177, 170, 162, 154, 146, 137, 129
};

 /* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void TIM_Configuration(void);
void DAC_Configuration(void);
void DMA_Configuration(void);
void GPIO_Configuration(void);
void ADC1_Init(void);

void Four_point(u16 cx, u16 cy);

// ADC1 转换的电压值通过 MDA 方式传到 SRAM
__IO uint16_t ADC_ConvertedValue;

// 局部变量,用于保存转换计算后的电压值
float ADC_ConvertedValueLocal;

int main(void) {
    /*!< At this stage the microcontroller clock setting is already configured,
     this is done through SystemInit() function which is called from startup
     file (startup_stm32f10x_xx.s) before to branch to application main.
     To reconfigure the default setting of SystemInit() function, refer to
     system_stm32f10x.c file
     */

    SystemInit();				//?????????????system_stm32f10x.c??
    SPI_Config();				//???????SPI??????????
    ili9320_Initializtion();	//TFT????SSD1289??RU61505?????????????????????????
    ili9320_Clear(0xffff);
    init_tick();

    RCC_Configuration();

    /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
     connected to the DAC converter. In order to avoid parasitic consumption,
     the GPIO pin should be configured in analog */
    GPIO_Configuration();
    TIM_Configuration();
    DMA_Configuration();
    DAC_Configuration();

    ADC1_Init();
    // uint16_t table[320];
    int flag = 0;
    int value = 1;
    uint16_t i = 0;


    while(1) {
        switch(flag % 3) {
            case 0:
                value = 1;
                break;
            case 1:
                value = 2;
                break;
            case 2:
                value = 4;
                break;
        }
        flag++;
        GUI_Line(20, 120, 320, 120, Black);
        GUI_Line(20, 20, 20, 220, Black);

        for(i = 0; i < 72; i++) {
            Four_point(20 + i * value, Sin72new[i]);
            //GUI_Circle(20 + i * 3, Sin72new[i], 1, Blue, 1);
            // ili9320_SetPoint(20 + i, Sin72new(i), Black);
            // GUI_Text(10, 220,"The Current Value:",18, Blue, White);
        }

        for(i = 0; i < 71; i++) {
            GUI_Line(20 + i * value, Sin72new[i], 20 + (i + 1) * value, Sin72new[i + 1], Blue);
        }
        GUI_Line(20 + 70 * value, Sin72new[70], 20 + 71 * value, Sin72new[71], Blue);
        d(100000);
        ili9320_Clear(White);
    }

}
/**
   * @brief  Configures the different system clocks.
   * @param  None
   * @retval None
*/
void RCC_Configuration(void) {
    //时钟配置，不使用库默认时钟配置
    ErrorStatus HSEStartUpStatus;  //定义外部高速晶体启动状态枚举变量
    RCC_DeInit();     //复位RCC外部设备寄存器到默认值
    RCC_HSEConfig(RCC_HSE_ON);    //打开外部高速晶振
    HSEStartUpStatus = RCC_WaitForHSEStartUp();   //等待外部高速时钟准备好
    if(HSEStartUpStatus == SUCCESS) {
        //外部高速时钟已经准别好
        RCC_HCLKConfig(RCC_SYSCLK_Div1);   //配置AHB(HCLK)时钟=SYSCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);    //配置APB2(PCLK2)钟=AHB时钟
        RCC_PCLK1Config(RCC_HCLK_Div2);    //配置APB1(PCLK1)钟=AHB 1/2时钟
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);  //配置ADC时钟=PCLK2 1/4

        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //配置PLL时钟 == 外部高速晶体时钟*9
        //RCC_ADCCLKConfig(RCC_PCLK2_Div4);  //配置ADC时钟= PCLK2/4
        RCC_PLLCmd(ENABLE);  //使能PLL时钟
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
            //等待PLL时钟就绪
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//配置系统时钟 = PLL时钟

        while(RCC_GetSYSCLKSource() != 0x08) {
          //检查PLL时钟是否作为系统时钟
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

    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC,ENABLE);
}


void TIM_Configuration(void)
{
    // Time base configuration
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 550;
    TIM_TimeBaseStructure.TIM_Prescaler = 1000;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
    // TIM6 TRGO selection */
    TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
    /* TIM6 enable counter */
    TIM_Cmd(TIM6, ENABLE);
}


void DAC_Configuration(void)
{
    // DAC channel1 Configuration
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO; //DAC触发  TIM7触发
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //不使用内部波形发生器
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;   //关闭外部输出缓存
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);  //用DAC输出波形，传输的数据比较多，所以采用DMA传输可以节省CPU的开支
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);
    // Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is automatically connected to the DAC converter
    // Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is automatically connected to the DAC converter
    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
    /* Enable DMA for DAC Channel1 */
    DAC_DMACmd(DAC_Channel_1, ENABLE);
	DAC_DMACmd(DAC_Channel_2, ENABLE);
}

 void DMA_Configuration(void)
 {
   DMA_DeInit(DMA2_Channel4);   //将dma的通道寄存器设为默认值
   DMA_DeInit(DMA1_Channel1);

   DMA_InitStructure2.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;  //定义dma外设基地址
   DMA_InitStructure2.DMA_MemoryBaseAddr = (uint32_t)&Sine12bit;//Sine12bit;
   DMA_InitStructure2.DMA_DIR = DMA_DIR_PeripheralDST;  //外设作为数据传输的目的地
   DMA_InitStructure2.DMA_BufferSize = 32;  //dma缓存大小
   DMA_InitStructure2.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变
   DMA_InitStructure2.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址寄存器 递增
   DMA_InitStructure2.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度
   DMA_InitStructure2.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure2.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式，数据传输数目为0时，自动恢复配置初值
   DMA_InitStructure2.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure2.DMA_M2M = DMA_M2M_Disable; //通道未被设置成内存到内存模式，与循环模式相对


   DMA_Init(DMA2_Channel4, &DMA_InitStructure2);  //将DMA_InitStructure中指定的参数初始化dma的通道寄存器
   // Enable DMA2 Channel4
   DMA_Cmd(DMA2_Channel4, ENABLE);  //使能通道

   DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; /* 配置外部设备基地址：ADC 地址*/
   DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue; /*配置内存基地址*/
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //外设为数据源 （设置传输方向）
   DMA_InitStructure.DMA_BufferSize = 1;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*外设地址固定*/
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable; /*内存地址固定*/
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //半字
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环传输
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);
   /* Enable DMA channel1 */
   DMA_Cmd(DMA1_Channel1, ENABLE);
 }

 /**
   * @brief  Configures the different GPIO ports.
   * @param  None
   * @retval None
   */
void GPIO_Configuration(void)
{
     GPIO_InitTypeDef GPIO_InitStructure;

   /* Once the DAC channel is enabled, the corresponding GPIO pin is automatically
      connected to the DAC converter. In order to avoid parasitic consumption,
      the GPIO pin should be configured in analog */
     GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5;  //一旦DAC通道使能，相应的GPIO PA4,PA5自动与DAC的模拟输出相连，
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //为了避免避免寄生的干扰和额外的功耗，PA4 置成模拟输入?
     GPIO_Init(GPIOA, &GPIO_InitStructure);

    // adc PC1 ????
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
     GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
     GPIO_Init(GPIOC, &GPIO_InitStructure); // PC1,输入时不用设置速率
}

void ADC1_Init(void){
  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;/*独立 ADC 模式*/
  ADC_InitStructure.ADC_ScanConvMode = DISABLE ; /*禁止扫描模式,扫描 模式用于多通道采集*/
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; /*开启连续转换模 式,即不停地进行 ADC 转换*/
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None ; /*不使用外部触发转换*/
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; /*采集数据 右对齐*/
  ADC_InitStructure.ADC_NbrOfChannel = 1; /*要转换的通道数目1*/
  ADC_Init(ADC1, &ADC_InitStructure);

  /*配置 ADC 时钟,为 PCLK2 的 8 分频,即 9MHz*/
  //RCC_ADCCLKConfig(RCC_PCLK2_Div8);
  /*配置 ADC1 的通道 11 为 55.5 个采样周期 */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5);
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  /*复位校准寄存器 */
  ADC_ResetCalibration(ADC1);

  /*等待校准寄存器复位完成 */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /*ADC校准*/
  ADC_StartCalibration(ADC1);
  /* 等待校准完成*/
  while(ADC_GetCalibrationStatus(ADC1));

  /* 由于没有采用外部触发,所以使用软件触发 ADC 转换 */
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void assert_failed(uint8_t* file, uint32_t line)
{
    while (1) {};
}

void Four_point(u16 cx, u16 cy) {
    ili9320_SetPoint(cx, cy, Black);
    ili9320_SetPoint(cx + 1, cy, Black);
    ili9320_SetPoint(cx - 1, cy, Black);
    ili9320_SetPoint(cx, cy + 1, Black);
    ili9320_SetPoint(cx, cy - 1, Black);
}
