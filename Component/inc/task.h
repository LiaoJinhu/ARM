#ifndef __TASK_H
#define __TASK_H

#include "common.h"
#include "led.h"
#include "key.h"
#include "MySysCtrl.h"
#include "oled.h"
#include "SDStorePara.h"

void SysNormalWorkLedTask(void);		//系统正常运行指示
void KeyControlOLED(void);			    //按键控制LED
void SaveDataTask(void);				//存储数据任务

#endif
