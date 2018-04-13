/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：delay.c
 * 摘    要 ：延时函数及任务调度函数
 * 函数接口：1.Delay_Init(void) 				  SysTick初始化函数
 *			  2.Delay_us(uint32_t nus)			  us级延时函数
 * 			  3.Delay_ms(uint32_t nms)			  ms级延时函数
 *			  4.setdelay(uint32_t t)			  设定ms级的延时时间，与checkdelay配合使用
 *			  5.checkdelay (uint32_t t)           检查设定的时间是否已到,与setdelay配合使用
 *
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "delay.h"

uint64_t volatile SysUsTimer = 0;			//应为1us刷新一次，使用volatile定义

//初始化SysTick 1us中断一次，最高优先级
void Delay_Init(void)
{
	//使用外部时钟 HCLK 72MHz
	//定时时间：72*10^6/10^6*1/72Mhz = 1us
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	while(SysTick_Config(SystemCoreClock / 1000000));     //若SysTick配置不成功，程序将卡在这里
	SCB->SHP[11] = 0;									  //优先级设置为最高
}

//us级延时
void Delay_us(uint32_t nus)
{
	uint64_t temp = 0;
	
	temp = SysUsTimer + nus;
	while(SysUsTimer <= temp);
}

//ms级延时
void Delay_ms(uint32_t nms)
{
	uint64_t temp = 0;
	
	temp = SysUsTimer + nms*1000;
	while(SysUsTimer <= temp);
}

//ms级的设定延时时间，与checkdelay配合使用
uint64_t setdelay(uint64_t t)
{
  return (GetSysMsTime()+ t - 1);                             
}

//ms检查设定的延时是否已到，与setdelay配合使用
uint64_t checkdelay(uint64_t t)
{
  return (((uint64_t)(t - GetSysMsTime()) & 0x8000000000000000) >> 48);
}
