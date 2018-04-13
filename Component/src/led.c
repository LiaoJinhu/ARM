/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：led.c
 * 摘    要 ：LED驱动文件
 * 函数接口：1.LEDGPIO_Init(void)   LED的IO口初始化函数
 *
 * 说    明 ：若IO口不同，无需修改.c文件的内容，只需要修改.h文件内容
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "led.h"

//LED初始化
void LEDGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOA,GPIOD的时钟
	RCC_APB2PeriphClockCmd(LedRedPeri|LedBluePeri,ENABLE);
	
	//设置红LED
	GPIO_InitStructure.GPIO_Pin   = LedRedPin;				
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LedRedGPIO,&GPIO_InitStructure);								//初始化红LED
	
	//设置绿LED
	GPIO_InitStructure.GPIO_Pin   = LedBluePin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;				

	GPIO_Init(LedBlueGPIO,&GPIO_InitStructure);								//初始化绿LED
	
	LedRedOff;
	LedBlueOff;
}


