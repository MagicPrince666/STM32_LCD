#include "tick.h"

__IO u32 TimingDelay=0;

void init_tick() {
	  //* SystemCoreClock / 100000    10us�ж�һ��
	  //* SystemCoreClock / 10000		  100us�ж�һ��
		//* SystemCoreClock / 1000      1ms�ж�һ��
    if (SysTick_Config(SystemCoreClock / 100000))
    {
        while (1);
    }
		//�رյδ���ʱ��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void d(__IO u32 x) {
    TimingDelay = x;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (TimingDelay != 0);
}

void TimingDelayDecrement(void)
{
    if (TimingDelay) {
        --TimingDelay;
    }
}

void delay(__IO u32 x) {
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
    while (x-- != 0);
}
