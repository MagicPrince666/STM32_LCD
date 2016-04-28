#ifndef __TICK_H__
#define __TICK_H__

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void init_tick(void);
void d(__IO u32 x);
void TimingDelayDecrement(void);
void delay(__IO u32 x);

#endif
