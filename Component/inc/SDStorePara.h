#ifndef __SDSTOREPARA_H_
#define __SDSTOREPARA_H_

#include "sd.h"
#include "stdbool.h"
#include "common.h"
#include "delay.h"
#include "oled.h"
#include "key.h"

typedef struct
{
	uint8_t  SDCState;			  //SD��״̬
	uint32_t Sectors; 			  //����
	uint32_t Capacity;			  //����
	int32_t  TransferTime;		  //����ʱ��
}SDCardInfoClass;				  //SD����Ϣ�ṹ��

typedef enum
{
	CMD,
	DATA,
}DATATYPE;

typedef enum
{
	ADD,
	SUB,
}INDEXTYPE;
/****************�ڲ�����***************/
static void pushSDDouble(uint8_t x,uint8_t y,double data);
static void pushSDInt32(uint8_t x,uint8_t y,int32_t data);
static double readSDDouble(uint8_t x,uint8_t y);
static int32_t readSDInt32(uint8_t x,uint8_t y);

static uint8_t readSDCmd(void);
static uint8_t readSDData(uint8_t sector);
static uint8_t writeSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector,float data);
static uint8_t readSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector);
static void pushDataToSDBuffer(float data);

/***************�߲㺯��***************/
void SDCard_Init(void);        	  //SD����ʼ��
uint8_t saveSDPara(float data);   //���������SD��
void setSDCmdToSys(void);		  //�����ϵͳ
void setSDDataToSys(void);		  //�����ݵ�ϵͳ
void indexCmd(uint32_t *indexTotal);
void indexData(INDEXTYPE addOrSub, uint32_t indexNum);

extern SDCardInfoClass SDCardInfo;
extern OLED_EVENT OledState;
extern uint8_t existPerData;
extern uint8_t page;
extern uint8_t wi;
extern uint32_t paraSector;
extern uint32_t dataCount;	
extern uint8_t SDCmdBuffer[512];
#endif
