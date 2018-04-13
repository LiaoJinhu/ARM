#ifndef __SPI_HARD_H
#define __SPI_HARD_H

#include "common.h"

void Spi_Hard_Init(void);						//SPI硬件初始化  不带片选
void SetSPISpeed(uint8_t speed);				//设置SPI的速度
uint8_t SPI_ReadWriteByte(uint8_t TxData);		//SPI读写一个字节的数据

#endif
