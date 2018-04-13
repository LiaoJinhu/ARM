/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：key.c
 * 摘    要 ：按键驱动文件，主要包含按键的IO口初始化及按键无阻塞扫描
 * 函数接口：1.KeyGPIO_Init(void)   		       按键的IO口初始化函数
 *		 	     2.Get_KeyMsg(KEY_MSG *key_msg)    获取按键的值及按键的状态
 *   		     3.ScanIndependentKey(void)        无阻塞的按键扫描，放在定时器中断里进行扫描
 *
 * 说    明 ：若IO口不同，无需修改.c文件的内容，只需要修改.h文件内容
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "key.h"

#define KEY_MAX              2							//有2个按键
#define KEY_MSG_FIFO_MAX   200           	    		//最大存储的键盘信息
KEY_MSG KEY_MSG_FIFO[KEY_MSG_FIFO_MAX];   				//用来储存按键的消息
uint32_t KEY_HOLD_TIME   =   100;						//用于判断按键按下的时间，当大于这个时间后就认为是长按
uint32_t KEY_DOWN_TIME   =	 10;						//用于判断按键按下的时间，当大于这个时间后就任务是短按
uint32_t KEY_Press_Time[KEY_MAX];
uint8_t key_msg_rear = 0,key_msg_front = 0;             //key_msg_rear用来指示当前储存的按键信息
														//key_msg_front用来指示当前所取的按键消息
volatile KEY_MSG_STATUS key_msg_status = KEY_MSG_EMPTY; //key_msg_status用来指示当前消息队列的状态

void KeyGPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//使能GPIOA的时钟
	RCC_APB2PeriphClockCmd(KeyPeri,ENABLE);

	GPIO_InitStructure.GPIO_Pin   = Key0Pin|Key1Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;				//上拉输入
	
	GPIO_Init(KeyGPIO,&GPIO_InitStructure);						//初始化按键
}

//储存按键消息
static uint8_t Send_KeyMsg(KEY_MSG key_msg)
{
	if(key_msg_status == KEY_MSG_FULL)		//消息队列里的消息已经取完
	{
		return 0;
	}
	
	//储存按键消息
	KEY_MSG_FIFO[key_msg_rear].status = key_msg.status;
	KEY_MSG_FIFO[key_msg_rear].key    = key_msg.key;
	key_msg_rear++;
	
	if(key_msg_rear >= KEY_MSG_FIFO_MAX)   //大于最大消息数，清零从头再储存
	{
		key_msg_rear = 0;			
	}
	
	if(key_msg_rear == key_msg_front)	   //一般情况是key_msg_rear>key_msg_front的，如果一直输入消息
		{							       //使key_msg_rear加到和key_msg_front相等时，这个时候是不能再储存消息的
		key_msg_status = KEY_MSG_FULL;	   //储存消息的，因为消息都没被取完
	}
	else
	{
		key_msg_status = KEY_MSG_NORMAL;
	}
	return 1;
}

//获取消息队列里的按键消息
uint8_t Get_KeyMsg(KEY_MSG *key_msg)
{
	if(key_msg_status == KEY_MSG_EMPTY)		//消息队列里得消息
	{
		return 0;
	}
	
	//取消息
	key_msg->status = KEY_MSG_FIFO[key_msg_front].status;
	key_msg->key    = KEY_MSG_FIFO[key_msg_front].key;
	key_msg_front++;
	
	if(key_msg_front >= KEY_MSG_FIFO_MAX)  //大于最大消息数，清零从头再储存
	{
		key_msg_front = 0;			
	}
	
	if(key_msg_front == key_msg_rear)	  //当两者相等时，说明消息队列里已经没有消息了
	{
		key_msg_status = KEY_MSG_EMPTY;
	}
	return 1;
}

//独立按键的扫描
void ScanIndependentKey(void)
{
	uint8_t i = 0;
	KEY_MSG  key_msg;
	uint8_t KEY_IN = 0;
	
	if(GPIO_ReadInputDataBit(KeyGPIO,Key0Pin) == 0 
	   || GPIO_ReadInputDataBit(KeyGPIO,Key1Pin) == 0)		   //有按键按下
	{
		for(i=0;i<KEY_MAX;i++)
		{
			if(i == 0)KEY_IN = GPIO_ReadInputDataBit(KeyGPIO,Key0Pin);
			else if(i == 1)KEY_IN = GPIO_ReadInputDataBit(KeyGPIO,Key1Pin);
			
			if(KEY_IN == 0)
			{
				KEY_Press_Time[i]++;
				if(KEY_Press_Time[i]<=KEY_DOWN_TIME) 		   //一直循环直到确认按下
				{
					continue;
				}
				else if(KEY_Press_Time[i]==KEY_DOWN_TIME+1)
				{
					key_msg.key=(KEY_Type)(i);   
					key_msg.status=KEY_DOWN;
					Send_KeyMsg(key_msg);
				}
				else if(KEY_Press_Time[i]<=KEY_HOLD_TIME)
				{
					continue;    
				}
				else if(KEY_Press_Time[i]==KEY_HOLD_TIME+1)
				{
					key_msg.key=(KEY_Type)(i);   
					key_msg.status=KEY_HOLD;
					Send_KeyMsg(key_msg);
				}
				else
				{
					KEY_Press_Time[i]=KEY_DOWN_TIME+1; 
				}				
			}
			else
			{
				key_msg.key=(KEY_Type)(i); 
				key_msg.status=KEY_IDLE;
				KEY_Press_Time[i]=0;
			}
		}
	}
	else
	{
		for(i=0;i<KEY_MAX;i++)
		{
			key_msg.key=(KEY_Type)(i); 
			key_msg.status=KEY_IDLE;
			KEY_Press_Time[i]=0;
		}
	}
}
