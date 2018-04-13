#include "common.h"
#include "MySysCtrl.h"
#include "MyNVIC.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "usart.h"
#include "timer.h"
#include "oled.h"
#include "SDStorePara.h"
#include "sd.h"
#include "task.h"

void SysInitialization(void);
void addRootine(void);

int main(void)
{
	DisableInterrupts;
	SysInitialization();
	EnableInterrupts;	
	while(1)
	{	  
		runLowRootine();
	}
}

void addRootine(void)
{
	//低优先级任务
//	addOneRootine(100,DMASendOnePage,1,true);  			//发送数据到上位机
	addOneRootine(50,KeyControlOLED,1,true);    		//按键控制LED-->测试按键用
	addOneRootine(1000,SaveDataTask,1,false);			//默认不开启存数据,等待按键选择
	
	//高级任务
	addOneRootine(500,SysNormalWorkLedTask,0,true);		//系统正常运行指示---->0.5s改变一次LED的状态
	addOneRootine(5,ScanIndependentKey,0,true);			//按键扫描------------>改为高级任务,避免运行周期的不准确
}
void SysInitialization(void)
{
	
	Delay_Init();			//开启SysTick滴答定时器---------->用于操作系统和延时
	TIM2_Init(7200,10);		//1ms    		   7200*10/72000000=1/1000s=1ms
	LEDGPIO_Init();			//LED初始化---------------------->默认都是关闭
	KeyGPIO_Init();			//按键初始化--------------------->两个独立按键，非阻塞式
//	Freecars_Init();		//上位机初始化------------------->调试备用
	OLED_Init();			//OLED初始化
	SDCard_Init();			//SD卡初始化--------------------->如果初始化失败,会在OLED上显示SD卡初始化失败

	MyNVIC_Init(NVIC_PriorityGroup_2,1,0,USART1_IRQn,ENABLE);
	MyNVIC_Init(NVIC_PriorityGroup_2,2,0,TIM2_IRQn,ENABLE);
	
	addRootine();
	setSDCmdToSys();
}
