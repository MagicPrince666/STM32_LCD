#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_font.h"

u16 DeviceCode=0;

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  vu16 LCD_REG;
  vu16 LCD_RAM;
} LCD_TypeDef;

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as ship select signal */
#define LCD_BASE    ((u32)(0x60000000 | 0x0C000000))
#define LCD         ((LCD_TypeDef *) LCD_BASE)

#define  ILI9320    0  /* 0x9320 */
#define  ILI9325    1  /* 0x9325 */
#define  ILI9328    2  /* 0x9328 */
#define  ILI9331    3  /* 0x9331 */
#define  SSD1298    4  /* 0x8999 */
#define  SSD1289    5  /* 0x8989 */
#define  ST7781     6  /* 0x7783 */
#define  LGDP4531   7  /* 0x4531 */
#define  SPFD5408B  8  /* 0x5408 */
#define  R61505U    9  /* 0x1505 0x0505 */
#define  HX8347D    10 /* 0x0047 */
#define  HX8347A    11 /* 0x0047 */
#define  LGDP4535   12 /* 0x4535 */
#define  SSD2119    13 /* 3.5 LCD 0x9919 */

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: LCD_CtrlLinesConfig(void) FSMC
** 功能描述: 液晶接口配置
** 参数描述：无
** 作  　者: 贝一特科技
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable FSMC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE |
                         RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG |
                         RCC_APB2Periph_AFIO, ENABLE);

  /* Set PD.00(D2), PD.01(D3), PD.04(NOE), PD.05(NWE), PD.08(D13), PD.09(D14),
     PD.10(D15), PD.14(D0), PD.15(D1) as alternate
     function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 |
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
     PE.14(D11), PE.15(D12) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Set PF.00(A0 (RS)) as alternate function push pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  /* Set PG.12(NE4 (LCD/CS)) as alternate function push pull - CE3(LCD /CS) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: LCD_LightPin(void)
** 功能描述: 背光控制脚A1
** 参数描述：无
** 作  　者: 贝一特科技
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void LCD_LightPin(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: LCD_FSMCConfig
** 功能描述: FSMC配置
** 参数描述：无
** 作  　者: 贝一特科技
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;

  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	//使能FSMC时钟
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据建立时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A


	writeTiming.FSMC_AddressSetupTime = 0x02;	 //0x01 地址建立时间（ADDSET）为1个HCLK
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A
    writeTiming.FSMC_DataSetupTime = 0x05;		 ////0x03 数据建立时间为4个HCLK
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A


    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //初始化FSMC配置

   	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // 使能BANK1
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: LCD_X_Init(void)
** 功能描述: 集体初始化
** 参数描述：无
** 作  　者: 贝一特科技
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void LCD_X_Init(void)
{
  /*背光引脚初始化*/
  LCD_LightPin();
 /* Configure the LCD Control pins --------------------------------------------*/
  LCD_CtrlLinesConfig();
/* Configure the FSMC Parallel interface -------------------------------------*/
  LCD_FSMCConfig();
}


/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : Writes to the selected LCD register.
* Input          : - LCD_Reg: address of the selected register.
*                  - LCD_RegValue: value to write to the selected register.
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
*******************************************************************************/
void LCD_WriteReg(u8 LCD_Reg,u16 LCD_RegValue)
{
  /* Write 16-bit Index, then Write Reg */
  LCD->LCD_REG = LCD_Reg;
  /* Write 16-bit Reg */
  LCD->LCD_RAM = LCD_RegValue;
}

/*******************************************************************************
* Function Name  : LCD_ReadReg
* Description    : Reads the selected LCD Register.
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
*******************************************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{
  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = LCD_Reg;
  /* Read 16-bit Reg */
  return (LCD->LCD_RAM);
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : Prepare to write to the LCD RAM.
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
  LCD->LCD_REG = 0x0022;
}

/*******************************************************************************
* Function Name  : LCD_WriteRAM
* Description    : Writes to the LCD RAM.
* Input          : - RGB_Code: the pixel color in RGB mode (5-6-5).
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
*******************************************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{
  /* Write 16-bit GRAM Reg */
  LCD->LCD_RAM = RGB_Code;
}

/*******************************************************************************
* Function Name  : LCD_ReadRAM
* Description    : Reads the LCD RAM.
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
*******************************************************************************/
u16 dummy;
u16 LCD_ReadRAM(void)
{

  /* Write 16-bit Index (then Read Reg) */
  LCD->LCD_REG = R34; /* Select GRAM Reg */
  /* Read 16-bit Reg */
  dummy = LCD->LCD_RAM;
  return dummy;
}

void LCD_SetCursor( u16 Xpos, u16 Ypos )
{
    #if  ( DISP_ORIENTATION == 90 ) || ( DISP_ORIENTATION == 270 )

 	u16 temp = Xpos;

			 Xpos = Ypos;
			 Ypos = ( MAX_X - 1 ) - temp;

	#elif  ( DISP_ORIENTATION == 0 ) || ( DISP_ORIENTATION == 180 )

	#endif

  switch( DeviceCode )
  {
     default:		 /* 0x9320 0x9325 0x9328 0x9331 0x5408 0x1505 0x0505 0x7783 0x4531 0x4535 */
          LCD_WriteReg(0x0020, Xpos );
          LCD_WriteReg(0x0021, Ypos );
	      break;

     case SSD1298: 	 /* 0x8999 */
     case SSD1289:   /* 0x8989 */
	      LCD_WriteReg(0x004e, Xpos );
          LCD_WriteReg(0x004f, Ypos );
	      break;

     case HX8347A: 	 /* 0x0047 */
     case HX8347D: 	 /* 0x0047 */
	      LCD_WriteReg(0x02, Xpos>>8 );
	      LCD_WriteReg(0x03, Xpos );

	      LCD_WriteReg(0x06, Ypos>>8 );
	      LCD_WriteReg(0x07, Ypos );

	      break;
     case SSD2119:	 /* 3.5 LCD 0x9919 */
	      break;
  }
}

void Delay(u32 nCount)
{
 u32 TimingDelay;
 while(nCount--)
   {
    for(TimingDelay=0;TimingDelay<10000;TimingDelay++);
   }
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** 函数名称: void ili9320_Initializtion()
** 功能描述: 初始化LCD控制器
** 参数描述：无
** 作  　者: 贝一特科技
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void ili9320_Initializtion()
{

    u16 i;
    LCD_X_Init();
    Delay(5); /* delay 50 ms */
    Delay(5); /* delay 50 ms */

	DeviceCode = LCD_ReadReg(0x0000);                //读LCD驱动型号：9325、9328、9320、9300、9331、9919、RU1505、8989、SSD1289
	if(DeviceCode==0x1505)	      //RU61505
	{
 		//*********************************************Start intial Sequence
		LCD_WriteReg(0x00e5,0x8000);
		LCD_WriteReg(0x0000,0x0001);
		LCD_WriteReg(0x0001,0x0100);
		LCD_WriteReg(0x0002,0x0700);
		LCD_WriteReg(0x0003,0x1030);
		LCD_WriteReg(0x0004,0x0000);
		LCD_WriteReg(0x0008,0x0202);
		LCD_WriteReg(0x0009,0x0000);
		LCD_WriteReg(0x000a,0x0000);
		LCD_WriteReg(0x000c,0x0000);
		LCD_WriteReg(0x000d,0x0000);
		LCD_WriteReg(0x000f,0x0000);
        //*********************************************Power On
		LCD_WriteReg(0x0010,0x0000);
		LCD_WriteReg(0x0011,0x0000);
		LCD_WriteReg(0x0012,0x0000);
		LCD_WriteReg(0x0013,0x0000);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0010,0x17b0);
		LCD_WriteReg(0x0011,0x0037);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0012,0x0138);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0013,0x1700);
		LCD_WriteReg(0x0029,0x000d);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0020,0x0000);
		LCD_WriteReg(0x0021,0x0000);
        //*********************************************Set gamma
		LCD_WriteReg(0x0030,0x0001);
		LCD_WriteReg(0x0031,0x0606);
		LCD_WriteReg(0x0032,0x0304);
		LCD_WriteReg(0x0033,0x0202);
		LCD_WriteReg(0x0034,0x0202);
		LCD_WriteReg(0x0035,0x0103);
		LCD_WriteReg(0x0036,0x011d);
		LCD_WriteReg(0x0037,0x0404);
		LCD_WriteReg(0x0038,0x0404);
		LCD_WriteReg(0x0039,0x0404);
		LCD_WriteReg(0x003c,0x0700);
		LCD_WriteReg(0x003d,0x0a1f);
        //**********************************************Set Gram aera
		LCD_WriteReg(0x0050,0x0000);
		LCD_WriteReg(0x0051,0x00ef);
		LCD_WriteReg(0x0052,0x0000);
		LCD_WriteReg(0x0053,0x013f);
		LCD_WriteReg(0x0060,0x2700);
		LCD_WriteReg(0x0061,0x0001);
		LCD_WriteReg(0x006a,0x0000);
        //*********************************************Paratial display
		LCD_WriteReg(0x0090,0x0010);
		LCD_WriteReg(0x0092,0x0000);
		LCD_WriteReg(0x0093,0x0003);
		LCD_WriteReg(0x0095,0x0101);
		LCD_WriteReg(0x0097,0x0000);
		LCD_WriteReg(0x0098,0x0000);
        //******************************************** Plan Control
		LCD_WriteReg(0x0007,0x0021);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0007,0x0031);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x0007,0x0173);
		for(i=50000;i>0;i--);
	}
	else if(DeviceCode==0x8989)	    //SSD1289
	{

		LCD_WriteReg(0x0000,0x0001);    ili9320_Delay(50000);   //打开晶振
    	LCD_WriteReg(0x0003,0xA8A4);    ili9320_Delay(50000);   //0xA8A4
    	LCD_WriteReg(0x000C,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x000D,0x080C);    ili9320_Delay(50000);
    	LCD_WriteReg(0x000E,0x2B00);    ili9320_Delay(50000);
    	LCD_WriteReg(0x001E,0x00B0);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0001,0x2B3F);    ili9320_Delay(50000);   //驱动输出控制320*240  0x6B3F
    	LCD_WriteReg(0x0002,0x0600);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0010,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0011,0x6070);    ili9320_Delay(50000);   //0x4030  //定义数据格式  16位色 横屏 0x6058
    	LCD_WriteReg(0x0005,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0006,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0016,0xEF1C);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0017,0x0003);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0007,0x0233);    ili9320_Delay(50000);   //0x0233
    	LCD_WriteReg(0x000B,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x000F,0x0000);    ili9320_Delay(50000);   //扫描开始地址
    	LCD_WriteReg(0x0041,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0042,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0048,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0049,0x013F);    ili9320_Delay(50000);
    	LCD_WriteReg(0x004A,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x004B,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0044,0xEF00);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0045,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0046,0x013F);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0030,0x0707);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0031,0x0204);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0032,0x0204);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0033,0x0502);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0034,0x0507);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0035,0x0204);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0036,0x0204);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0037,0x0502);    ili9320_Delay(50000);
    	LCD_WriteReg(0x003A,0x0302);    ili9320_Delay(50000);
    	LCD_WriteReg(0x003B,0x0302);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0023,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0024,0x0000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x0025,0x8000);    ili9320_Delay(50000);
    	LCD_WriteReg(0x004f,0);        //行首址0
    	LCD_WriteReg(0x004e,0);        //列首址0
	}
	else if(DeviceCode==0x9325||DeviceCode==0x9328)
	{
  		LCD_WriteReg(0x00e7,0x0010);
        LCD_WriteReg(0x0000,0x0001);  			     //start internal osc
        LCD_WriteReg(0x0001,0x0100);
        LCD_WriteReg(0x0002,0x0700); 				 //power on sequence
        LCD_WriteReg(0x0003,(1<<12)|(1<<5)|(1<<4) ); //65K
        LCD_WriteReg(0x0004,0x0000);
        LCD_WriteReg(0x0008,0x0207);
        LCD_WriteReg(0x0009,0x0000);
        LCD_WriteReg(0x000a,0x0000); 				 //display setting
        LCD_WriteReg(0x000c,0x0001);				 //display setting
        LCD_WriteReg(0x000d,0x0000); 				 //0f3c
        LCD_WriteReg(0x000f,0x0000);
        //Power On sequence //
        LCD_WriteReg(0x0010,0x0000);
        LCD_WriteReg(0x0011,0x0007);
        LCD_WriteReg(0x0012,0x0000);
        LCD_WriteReg(0x0013,0x0000);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0010,0x1590);
        LCD_WriteReg(0x0011,0x0227);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0012,0x009c);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0013,0x1900);
        LCD_WriteReg(0x0029,0x0023);
        LCD_WriteReg(0x002b,0x000e);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x0000);
        ///////////////////////////////////////////////////////
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0030,0x0007);
        LCD_WriteReg(0x0031,0x0707);
        LCD_WriteReg(0x0032,0x0006);
        LCD_WriteReg(0x0035,0x0704);
        LCD_WriteReg(0x0036,0x1f04);
        LCD_WriteReg(0x0037,0x0004);
        LCD_WriteReg(0x0038,0x0000);
        LCD_WriteReg(0x0039,0x0706);
        LCD_WriteReg(0x003c,0x0701);
        LCD_WriteReg(0x003d,0x000f);
        for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
        LCD_WriteReg(0x0050,0x0000);
        LCD_WriteReg(0x0051,0x00ef);
        LCD_WriteReg(0x0052,0x0000);
        LCD_WriteReg(0x0053,0x013f);
        LCD_WriteReg(0x0060,0xa700);
        LCD_WriteReg(0x0061,0x0001);
        LCD_WriteReg(0x006a,0x0000);
        LCD_WriteReg(0x0080,0x0000);
        LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0082,0x0000);
        LCD_WriteReg(0x0083,0x0000);
        LCD_WriteReg(0x0084,0x0000);
        LCD_WriteReg(0x0085,0x0000);
        LCD_WriteReg(0x0090,0x0010);
        LCD_WriteReg(0x0092,0x0000);
        LCD_WriteReg(0x0093,0x0003);
        LCD_WriteReg(0x0095,0x0110);
        LCD_WriteReg(0x0097,0x0000);
        LCD_WriteReg(0x0098,0x0000);
        //display on sequence
        LCD_WriteReg(0x0007,0x0133);
        LCD_WriteReg(0x0020,0x0000);
        LCD_WriteReg(0x0021,0x0000);
	}
	else if(DeviceCode==0x9320||DeviceCode==0x9300)
	{
        LCD_WriteReg(0x00,0x0000);
		LCD_WriteReg(0x01,0x0100);	//Driver Output Contral.
		LCD_WriteReg(0x02,0x0700);	//LCD Driver Waveform Contral.
		LCD_WriteReg(0x03,0x1018);	//Entry Mode Set.

		LCD_WriteReg(0x04,0x0000);	//Scalling Contral.
		LCD_WriteReg(0x08,0x0202);	//Display Contral 2.(0x0207)
		LCD_WriteReg(0x09,0x0000);	//Display Contral 3.(0x0000)
		LCD_WriteReg(0x0a,0x0000);	//Frame Cycle Contal.(0x0000)
		LCD_WriteReg(0x0c,(1<<0));	//Extern Display Interface Contral 1.(0x0000)
		LCD_WriteReg(0x0d,0x0000);	//Frame Maker Position.
		LCD_WriteReg(0x0f,0x0000);	//Extern Display Interface Contral 2.

		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);
		LCD_WriteReg(0x07,0x0101);	//Display Contral.
		for(i=50000;i>0;i--);
		for(i=50000;i>0;i--);

		LCD_WriteReg(0x10,(1<<12)|(0<<8)|(1<<7)|(1<<6)|(0<<4));	//Power Control 1.(0x16b0)
		LCD_WriteReg(0x11,0x0007);								//Power Control 2.(0x0001)
		LCD_WriteReg(0x12,(1<<8)|(1<<4)|(0<<0));					//Power Control 3.(0x0138)
		LCD_WriteReg(0x13,0x0b00);								//Power Control 4.
		LCD_WriteReg(0x29,0x0000);								//Power Control 7.

		LCD_WriteReg(0x2b,(1<<14)|(1<<4));

		LCD_WriteReg(0x50,0);		//Set X Start.
		LCD_WriteReg(0x51,239);	//Set X End.
		LCD_WriteReg(0x52,0);		//Set Y Start.
		LCD_WriteReg(0x53,319);	//Set Y End.
		LCD_WriteReg(0x60,0x2700);	//Driver Output Control.
		LCD_WriteReg(0x61,0x0001);	//Driver Output Control.
		LCD_WriteReg(0x6a,0x0000);	//Vertical Srcoll Control.
		LCD_WriteReg(0x80,0x0000);	//Display Position? Partial Display 1.
		LCD_WriteReg(0x81,0x0000);	//RAM Address Start? Partial Display 1.
		LCD_WriteReg(0x82,0x0000);	//RAM Address End-Partial Display 1.
		LCD_WriteReg(0x83,0x0000);	//Displsy Position? Partial Display 2.
		LCD_WriteReg(0x84,0x0000);	//RAM Address Start? Partial Display 2.
		LCD_WriteReg(0x85,0x0000);	//RAM Address End? Partial Display 2.
		LCD_WriteReg(0x90,(0<<7)|(16<<0));	//Frame Cycle Contral.(0x0013)
		LCD_WriteReg(0x92,0x0000);	//Panel Interface Contral 2.(0x0000)
		LCD_WriteReg(0x93,0x0001);	//Panel Interface Contral 3.
		LCD_WriteReg(0x95,0x0110);	//Frame Cycle Contral.(0x0110)
		LCD_WriteReg(0x97,(0<<8));	//
		LCD_WriteReg(0x98,0x0000);	//Frame Cycle Contral.
		LCD_WriteReg(0x07,0x0173);	//(0x0173)
	}
	else if(DeviceCode==0x9331)
	{
		LCD_WriteReg(0x00E7, 0x1014);
		LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit   0x0100
		LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
		LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.     0x1030
		LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
		LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
		LCD_WriteReg(0x000A, 0x0000); // FMARK function
		LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
		LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
		LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
		//*************Power On sequence ****************//
		LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
		LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
		LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
		ili9320_Delay(200); // Dis-charge capacitor power voltage
		LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
		LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
		LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
		LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
		ili9320_Delay(50); // Delay 50ms
		LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
		LCD_WriteReg(0x0021, 0x0000); // GRAM Vertical Address
		// ----------- Adjust the Gamma Curve ----------//
		LCD_WriteReg(0x0030, 0x0000);
		LCD_WriteReg(0x0031, 0x0106);
		LCD_WriteReg(0x0032, 0x0000);
		LCD_WriteReg(0x0035, 0x0204);
		LCD_WriteReg(0x0036, 0x160A);
		LCD_WriteReg(0x0037, 0x0707);
		LCD_WriteReg(0x0038, 0x0106);
		LCD_WriteReg(0x0039, 0x0707);
		LCD_WriteReg(0x003C, 0x0402);
		LCD_WriteReg(0x003D, 0x0C0F);
		//------------------ Set GRAM area ---------------//
		LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
		LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
		LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
		LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
		LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
		LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
		LCD_WriteReg(0x006A, 0x0000); // set scrolling line
		//-------------- Partial Display Control ---------//
		LCD_WriteReg(0x0080, 0x0000);
		LCD_WriteReg(0x0081, 0x0000);
		LCD_WriteReg(0x0082, 0x0000);
		LCD_WriteReg(0x0083, 0x0000);
		LCD_WriteReg(0x0084, 0x0000);
		LCD_WriteReg(0x0085, 0x0000);
		//-------------- Panel Control -------------------//
		LCD_WriteReg(0x0090, 0x0010);
		LCD_WriteReg(0x0092, 0x0600);
		LCD_WriteReg(0x0007,0x0021);
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0061);
		ili9320_Delay(50);
		LCD_WriteReg(0x0007,0x0133);  // 262K color and display ON
		ili9320_Delay(50);
	}
	else if(DeviceCode==0x9919)
	{
		//*********POWER ON &RESET DISPLAY OFF
		 LCD_WriteReg(0x28,0x0006);
		 LCD_WriteReg(0x00,0x0001);
		 LCD_WriteReg(0x10,0x0000);
		 LCD_WriteReg(0x01,0x72ef);
		 LCD_WriteReg(0x02,0x0600);
		 LCD_WriteReg(0x03,0x6a38);
		 LCD_WriteReg(0x11,0x6874);
		 //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0f,0x0000);
		 //  RAM WRITE DATA MASK
		 LCD_WriteReg(0x0b,0x5308);
		 LCD_WriteReg(0x0c,0x0003);
		 LCD_WriteReg(0x0d,0x000a);
		 LCD_WriteReg(0x0e,0x2e00);
		 LCD_WriteReg(0x1e,0x00be);
		 LCD_WriteReg(0x25,0x8000);
		 LCD_WriteReg(0x26,0x7800);
		 LCD_WriteReg(0x27,0x0078);
		 LCD_WriteReg(0x4e,0x0000);
		 LCD_WriteReg(0x4f,0x0000);
		 LCD_WriteReg(0x12,0x08d9);
		 // -----------------Adjust the Gamma Curve----//
		 LCD_WriteReg(0x30,0x0000);
		 LCD_WriteReg(0x31,0x0104);
		 LCD_WriteReg(0x32,0x0100);
		 LCD_WriteReg(0x33,0x0305);
		 LCD_WriteReg(0x34,0x0505);
		 LCD_WriteReg(0x35,0x0305);
		 LCD_WriteReg(0x36,0x0707);
		 LCD_WriteReg(0x37,0x0300);
		 LCD_WriteReg(0x3a,0x1200);
		 LCD_WriteReg(0x3b,0x0800);
		 LCD_WriteReg(0x07,0x0033);
	}
    for(i=50000;i>0;i--);
}

/****************************************************************************
* 名    称：void ili9320_SetCursor(u16 x,u16 y)
* 功    能：设置屏幕座标
* 入口参数：x      行座标
*           y      列座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetCursor(10,10);
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
****************************************************************************/
__inline void ili9320_SetCursor(u16 x,u16 y)
{
	if(DeviceCode==0x8989)
	{
	 	LCD_WriteReg(0x004e,y);        //行
    	LCD_WriteReg(0x004f,0x13f-x);  //列
	}
	else if(DeviceCode==0x9919)
	{
		LCD_WriteReg(0x004e,x);        //行
  		LCD_WriteReg(0x004f,y);        //列
	}
	else
	{
  		LCD_WriteReg(0x0020,y);        //行
  		LCD_WriteReg(0x0021,0x13f-x);  //列
	}
}

/****************************************************************************
* 名    称：void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
* 功    能：设置窗口区域
* 入口参数：StartX     行起始座标
*           StartY     列起始座标
*           EndX       行结束座标
*           EndY       列结束座标
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetWindows(0,0,100,100)；
****************************************************************************/
__inline void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY)
{
  ili9320_SetCursor(StartX,StartY);
  LCD_WriteReg(0x0050, StartX);
  LCD_WriteReg(0x0052, StartY);
  LCD_WriteReg(0x0051, EndX);
  LCD_WriteReg(0x0053, EndY);
}

/****************************************************************************
* 名    称：void ili9320_Clear(u16 dat)
* 功    能：将屏幕填充成指定的颜色，如清屏，则填充 0xffff
* 入口参数：dat      填充值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Clear(0xffff);
****************************************************************************/
void ili9320_Clear(u16 Color)
{
  u32 index=0;
  ili9320_SetCursor(0,0);
  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
  for(index=0;index<76800;index++)
   {
     LCD->LCD_RAM=Color;
   }
}

/****************************************************************************
* 名    称：u16 ili9320_GetPoint(u16 x,u16 y)
* 功    能：获取指定座标的颜色值
* 入口参数：x      行座标
*           y      列座标
* 出口参数：当前座标颜色值
* 说    明：
* 调用方法：i=ili9320_GetPoint(10,10);
****************************************************************************/
u16 ili9320_GetPoint(u16 x,u16 y)
{
  ili9320_SetCursor(x,y);
  return (ili9320_BGR2RGB(LCD_ReadRAM()));
}
/****************************************************************************
* 名    称：void ili9320_SetPoint(u16 x,u16 y,u16 point)
* 功    能：在指定座标画点
* 入口参数：x      行座标
*           y      列座标
*           point  点的颜色
* 出口参数：无
* 说    明：
* 调用方法：ili9320_SetPoint(10,10,0x0fe0);
****************************************************************************/
void ili9320_SetPoint(u16 x,u16 y,u16 point)
{
  if ( (x>320)||(y>240) ) return;
  ili9320_SetCursor(x,y);

  LCD_WriteRAM_Prepare();
  LCD_WriteRAM(point);
}

/****************************************************************************
* 名    称：void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
* 功    能：在指定座标显示一个8x16点阵的ascii字符
* 入口参数：x          行座标
*           y          列座标
*           charColor  字符的颜色
*           bkColor    字符背景颜色
* 出口参数：无
* 说    明：显示范围限定为可显示的ascii码
* 调用方法：ili9320_PutChar(10,10,'a',0x0000,0xffff);
****************************************************************************/
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;

  u8 tmp_char=0;

  for (i=0;i<16;i++)
  {
    tmp_char=ascii_8x16[((c-0x20)*16)+i];
    for (j=0;j<8;j++)
    {
      if ( (tmp_char >> 7-j) & 0x01 == 0x01)
        {
          ili9320_SetPoint(x+j,y+i,charColor); // 字符颜色
        }
        else
        {
          ili9320_SetPoint(x+j,y+i,bkColor); // 背景颜色
        }
    }
  }
}

/****************************************************************************
* 名    称：u16 ili9320_BGR2RGB(u16 c)
* 功    能：RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
* 入口参数：c      BRG 颜色值
* 出口参数：RGB 颜色值
* 说    明：内部函数调用
** 日　  期: 2015年3月20日
** 淘宝店铺：http://shop58856066.taobao.com/
****************************************************************************/
u16 ili9320_BGR2RGB(u16 c)
{
  u16  r, g, b, rgb;

  b = (c>>0)  & 0x1f;
  g = (c>>5)  & 0x3f;
  r = (c>>11) & 0x1f;

  rgb =  (b<<11) + (g<<5) + (r<<0);

  return( rgb );
}

/****************************************************************************
* 名    称：void ili9320_BackLight(u8 status)
* 功    能：开、关液晶背光
* 入口参数：status     1:背光开  0:背光关
* 出口参数：无
* 说    明：
* 调用方法：ili9320_BackLight(1);
****************************************************************************/
void ili9320_BackLight(u8 status)
{
  if ( status >= 1 )
  {
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
  }
  else
  {
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);
  }
}

/****************************************************************************
* 名    称：void ili9320_Delay(vu32 nCount)
* 功    能：延时
* 入口参数：nCount   延时值
* 出口参数：无
* 说    明：
* 调用方法：ili9320_Delay(10000);
****************************************************************************/
void ili9320_Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
