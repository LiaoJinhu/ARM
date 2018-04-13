#ifndef __USART_H
#define __USART_H

#include "common.h"

/**************上位机协议****************/
#define UartRxBufferLen 100
#define UartDataNum     20
#define LineDataNum     2  //应是偶数
#define UartRxDataLen   41
#define UartRxCmdLen    7
#define UartCmdNum  SerialPortRx.Buffer[SerialPortRx.Stack-3]//命令号
#define UartCmdData SerialPortRx.Buffer[SerialPortRx.Stack-2]//命令数据

typedef struct 
{
  int Stack;
  uint8_t Data;
  uint8_t PreData;
  uint8_t Buffer[UartRxBufferLen];
  uint8_t Enable;
  uint8_t Check;
}SerialPortType;

/*****************************************/

extern SerialPortType SerialPortRx;			//接收中断里调用
extern double UartData[12];

void Freecars_Init(void);					//上位机所用串口初始化
void DMASendOnePage(void);					//DMA无阻塞发送一页数据
void SendOnePage(void);						//有阻塞的发送一页数据
void push(uint8_t chanel,uint16_t data);    //将数据存在发送缓冲区
void UartDebug(void);						//接收中断调用,用于调试参数
void UartCmd(uint8_t Num,uint8_t Data);     //接收中断调用,用于调试参数

void SendOneChar(uint8_t sendData);			//有阻塞发送一个字节数据 

#endif
