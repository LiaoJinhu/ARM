#ifndef __MySysCtrl_H
#define __MySysCtrl_H

#include "common.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "delay.h"

#define sysCtrlLen  30			 //30��
#define sysCtrlPriorityDivide 15 //����sysCtrlPriorityDivide����Դ�������ȼ�����

struct sysCtrlClass
{
	bool enable;				 //�Ƿ�����
	uint32_t delay;		  	     //��ʱ
	uint64_t timeRecord;		 //��¼ʱ��
	void (*f)(void);
	struct sysCtrlClass *next;
};

#define sysCtrlClassLen sizeof(struct sysCtrlCalss)  //ϵͳ�ṹ�峤��

bool addOneRootine(uint32_t delay,void (*f)(void),uint8_t priority,bool enable);
bool enableOneRootine(void(*f)(void));
bool disableOneRootine(void(*f)(void));
bool deleteOneRootine(void (*f)(void));
bool changeOneRootine(void (*f)(void),int32_t period);
void runLowRootine(void);
void runHighRootine(void);

#endif
