#include "tick.h"

__IO u32 TimingDelay=0;

void init_tick()
{		
	  //* SystemCoreClock / 100000    10us中断一次
	  //* SystemCoreClock / 10000		  100us中断一次
		//* SystemCoreClock / 1000      1ms中断一次
    if (SysTick_Config(SystemCoreClock / 100000))
    {
        while (1);
    }
		//关闭滴答定时器
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void d(__IO u32 x)
{
    TimingDelay = x;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (TimingDelay != 0);
}

void TimingDelayDecrement(void)
{
    if (TimingDelay)
        --TimingDelay;
}

void delay(__IO u32 x)
{
    
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (x-- != 0);
}
