/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：timer.c
 * 摘    要 ：定时器初始化
 * 函数接口：1.TIM2_Init(uint16_t TIM_scale,uint16_t TIM_Period)		设定定时器2，1ms中断用于跑高级任务
 *			  2.TIM4_Init(void)											设定定时器4，用于编码器
 *
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "timer.h"

//1ms中断，用于跑高级任务
void TIM2_Init(uint16_t TIM_scale,uint16_t TIM_Period)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//开启TIM2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	//定时器2的基本配置
	//时钟源：72MHz  输出频率：72Mhz/(Period+1)/(Prescaler+1)
	TIM_TimeBaseStructure.TIM_Period = (TIM_Period - 1);
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_scale - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}

//TIM4用做QEI 
//提供两种方案：
//PB6-PB9 CH1-CH4
//重映射 PD12-PD15 CH1-CH4
void TIM4_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;	
	
	//使能GPIOA,GPIOD的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//此种方案是利用重映射，将CH1-->PD12 CH2-->PD13
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);

//	//配置IO口
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOD,&GPIO_InitStructure);	
	
	//配置IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	
	//配置TIM4
	//时钟源：72MHz 不分频
	//说明：常规的是将TIM_Period设置为编码器的线数的四倍，原因见《STM32定时器---正交编码器模式详解》
	//学长程序里，都将TIM_Period定为最大值，在读出TIM4->CNT的时候没有进行处理
	//疑问：TIM_Period的确定，TIM_EncoderInterfaceConfig中参数的确定
	//猜想：TIM_Period足够大即可，只要读取的时候不超过其值就行，进行PID运算时，相当于将编码器的线放大了
    TIM_DeInit(TIM4);
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 0xffff;       
    TIM_TimeBaseStructure.TIM_Prescaler = 0;	  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    //编码配置  编码模式
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,
							   TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);  //上升沿捕获
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 6;           						   //比较滤波器
    TIM_ICInit(TIM4,&TIM_ICInitStructure);

    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM4->CNT =0;

    TIM_Cmd(TIM4, ENABLE);   //使能定时器4
}

//void SetPWMFrequency(uint32_t hz)
//{
//	int32_t prescaler;
//	uint32_t period;
//	uint8_t ps = 0;
//	uint8_t i;
//	uint32_t min_val;
//	
//	prescaler = (72000000/hz)/65535;
//	min_val = 0xFFFF;
//    if((72000000/hz)/prescaler > 65535)
//    {
//        prescaler++;
//    }
//    for(i = 0; i < 7; i++)
//    {
//        if((ABS(prescaler - (1<<i))) < min_val)
//        {
//            ps = i;
//            min_val = ABS(prescaler - (1<<i));
//        }
//    }
//    if(prescaler > (1<<ps)) ps++;
//    if(ps > 7) ps = 7;
//    period = ((72000000/(1<<ps))/hz) - 1;
//	ps -= 1;
//	TIM1->ARR = period;
//	TIM1->PSC = ps;
//	
//	PWMManage.Period = period;
//	SetPWMDuty(1,PWMManage.Channel1_Duty);
//	SetPWMDuty(2,PWMManage.Channel2_Duty);	
//}
