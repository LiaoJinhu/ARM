#ifndef __MySysCtrl_H
#define __MySysCtrl_H

#include "common.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "delay.h"

#define sysCtrlLen  30			 //30个
#define sysCtrlPriorityDivide 15 //分配sysCtrlPriorityDivide个资源给高优先级任务

struct sysCtrlClass
{
	bool enable;				 //是否允许
	uint32_t delay;		  	     //延时
	uint64_t timeRecord;		 //记录时间
	void (*f)(void);
	struct sysCtrlClass *next;
};

#define sysCtrlClassLen sizeof(struct sysCtrlCalss)  //系统结构体长度

bool addOneRootine(uint32_t delay,void (*f)(void),uint8_t priority,bool enable);
bool enableOneRootine(void(*f)(void));
bool disableOneRootine(void(*f)(void));
bool deleteOneRootine(void (*f)(void));
bool changeOneRootine(void (*f)(void),int32_t period);
void runLowRootine(void);
void runHighRootine(void);

#endif
