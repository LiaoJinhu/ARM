#ifndef __KEY_H
#define __KEY_H

#include "common.h"

#define KeyPeri  RCC_APB2Periph_GPIOA
#define KeyGPIO  GPIOA
#define Key0Pin  GPIO_Pin_13
#define Key1Pin  GPIO_Pin_15

//按键的种类
typedef enum 
{    
	KEY_ONE,
	KEY_TWO,
}KEY_Type;

//按键的状态
typedef enum
{
    KEY_IDLE,          			//空闲态
    KEY_DOWN,       			//按下按键
	KEY_HOLD,					//保持
}KEY_Status;

//按键类
typedef struct
{
	KEY_Type 	key;			//哪个按键
	KEY_Status  status;         //按键的状态
}KEY_MSG;

//消息的状态
typedef enum
{
    KEY_MSG_NORMAL,    //一般消息
    KEY_MSG_FULL,      //消息已满
    KEY_MSG_EMPTY,     //消息空
}KEY_MSG_STATUS;

void KeyGPIO_Init(void);					//按键初始化
uint8_t Get_KeyMsg(KEY_MSG *key_msg);		//获取按键值和状态
void ScanIndependentKey(void);				//独立按键扫描

#endif
