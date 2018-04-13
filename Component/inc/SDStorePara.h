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
	uint8_t  SDCState;			  //SD卡状态
	uint32_t Sectors; 			  //扇区
	uint32_t Capacity;			  //容量
	int32_t  TransferTime;		  //传输时间
}SDCardInfoClass;				  //SD卡信息结构体

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
/****************内部函数***************/
static void pushSDDouble(uint8_t x,uint8_t y,double data);
static void pushSDInt32(uint8_t x,uint8_t y,int32_t data);
static double readSDDouble(uint8_t x,uint8_t y);
static int32_t readSDInt32(uint8_t x,uint8_t y);

static uint8_t readSDCmd(void);
static uint8_t readSDData(uint8_t sector);
static uint8_t writeSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector,float data);
static uint8_t readSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector);
static void pushDataToSDBuffer(float data);

/***************高层函数***************/
void SDCard_Init(void);        	  //SD卡初始化
uint8_t saveSDPara(float data);   //储存参数到SD卡
void setSDCmdToSys(void);		  //读命令到系统
void setSDDataToSys(void);		  //读数据到系统
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
