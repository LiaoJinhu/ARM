#ifndef __DELAY_H
#define __DELAY_H

#include "common.h"

//获取当前时间
#define GetSysMsTime()     (SysUsTimer/1000)
#define GetSys100UsTime()  (SysUsTimer/100)
#define GetSys10UsTime()   (SysUsTimer/10)
#define GetSysUsTime()     (SysUsTimer)


extern uint64_t volatile SysUsTimer;

void Delay_Init(void);					//SysTick初始化
void Delay_ms(uint32_t nms);			//ms级延时
void Delay_us(uint32_t nus);			//us级延时
uint64_t setdelay(uint64_t t);			//设定ms级的延时,与checkdelay配合使用
uint64_t checkdelay(uint64_t t);		//检查设定的时间是否已到,与setdelay配合使用

#endif
