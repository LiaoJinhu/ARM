#ifndef __TIMER_H
#define __TIMER_H

#include "common.h"


void TIM2_Init(uint16_t TIM_scale,uint16_t TIM_Period);		//设定定时器2，1ms中断用于跑高级任务
void TIM4_Init(void);										//设定定时器4，用于编码器

#endif
