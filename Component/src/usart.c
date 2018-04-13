/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：usart.c
 * 摘    要 ：串口的配置，同时移植了FreeCars上位机的代码
 * 函数接口：1.Freecars_Init(void)   		       串口的配置，使用了DMA
 *			  2.SendOneChar(uint8_t sendData)      发送一个字节
 *   		  3.DMASendOnePage(void)       		   上位机用,使用DMA发送一页数据     无阻塞
 *			  4.SendOnePage(void)   			   上位机用,不使用DMA发送一页数据   阻塞
 *			  5.push(uint8_t chanel,uint16_t data)  将需要发送到上位机的数据存到发送缓冲区
 *	          6.UartDebug(void)						上位机调试参数用,在接收中断里自动调用
 *    		  7.UartCmd(uint8_t Num,uint8_t Data)   上位机调试参数用,在接收中断里自动调用				
 *
 * 说    明 ：若IO口不同，无需修改.c文件的内容，只需要修改.h文件内容
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "usart.h"

const uint8_t FreeCarsDataNum=UartDataNum*2;
const uint8_t FreeCarsTotalNum = UartDataNum*2 + 3 + 1;
uint8_t uSendBuf[FreeCarsDataNum]={0};						//发送缓冲区
uint8_t uDMASendBuf[FreeCarsTotalNum]={0};					//DMA发送缓冲区
SerialPortType SerialPortRx;
double UartData[12] = {0};

//串口配置，使用DMA传输
void Freecars_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	DMA_InitTypeDef    DMA_InitStructure;
	
	//使能GPIOA,USART1,DMA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	//USART1 TXD  PA9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    		//模式配置参考STM32中文参考手册P110
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//USART1 RXD  PA10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //模式配置参考STM32中文参考手册P110
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//波特率：115200  无硬件流  全双工模式 无奇偶校验 1个停止位 8位字长
	USART_InitStructure.USART_BaudRate = 115200;   		
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);			     	//清除发送完成标志
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		    //接收寄存器非空中断
	
	//配置DMA
	//方向：内存到外设
	//外设地址不增,内存地址增加
	//外设和内存的数据单位都为Byte
	//DMA模式：一次传输，循环
	//优先级：中
	//禁止内存到内存的传输
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)uDMASendBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = FreeCarsTotalNum;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	//初始化DMA1的通道4
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel4, ENABLE);
	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);   	//串口向DMA发送请求
	
    uDMASendBuf[0] = 251;
    uDMASendBuf[1] = 109;
    uDMASendBuf[2] = 37;
    SerialPortRx.Stack = 0;	
}

//发送一个字节
void SendOneChar(uint8_t sendData)
{
    USART_ClearFlag(USART1, USART_FLAG_TXE);				//清除发送寄存器空标志位
    USART_SendData(USART1 , sendData);					    
    while(USART_GetFlagStatus(USART1 , USART_FLAG_TXE) == RESET);
    USART_ClearFlag(USART1 , USART_FLAG_TXE);	
}

//使用DMA传输一页数据
void DMASendOnePage(void)
{
	int16_t i;
	uint8_t check = 0;
	
	if(DMA_GetFlagStatus(DMA1_FLAG_TC4))//传输完成
	{
		DMA_ClearFlag(DMA1_FLAG_TC4);   //传输前清空DMA1_Channel4传输完成的标志
	}
	else 
	{
		return;
	}
	
	check += 251;					//全部数据加入校验
	check += 109;
	check += 37;
	for(i=0;i<UartDataNum;i++)
	{
		uDMASendBuf[3+i] = uSendBuf[i];
		check += uSendBuf[i];
	}
	uDMASendBuf[3+UartDataNum] = check;
	DMA_Cmd(DMA1_Channel4,ENABLE);
}

//不带DMA的串口数据传输
void SendOnePage(void)
{
	uint8_t i,sum=0;
	
	//使用轮询的方式发送数据，当数据未发送，程序停在此处直到发送完成
	SendOneChar(251);
	SendOneChar(109);
	SendOneChar(37);
    sum+=(251);      //全部数据加入校验
    sum+=(109);
    sum+=(37);
	for(i=0;i<FreeCarsDataNum;i++)
    {
        SendOneChar(uSendBuf[i]);
        sum+=uSendBuf[i];         //全部数据加入校验
    }
    SendOneChar(sum);
}

//数据装载，此时数据并没有发送出去，只是装载到了数组里面
void push(uint8_t chanel,uint16_t data)
{
    uSendBuf[chanel*2]=data/256;
    uSendBuf[chanel*2+1]=data%256;
}

//接收上位机的数据
void UartDebug(void)
{
//	i = UartData[0];
}

//接收电脑键盘给的命令
void UartCmd(uint8_t Num,uint8_t Data) //可以根据键盘的值来改变程序中的参数
{
	if(Num == 1)
	{
		switch(Data)
		{
			case 6:              //F6
				
				break;
			case 7:              //F7
				
				break;
			case 8:              //F8
				
				break;
			case 9:              //F9
				
				break;
			case 10:             //F10
				
				break;
			case 11:             //F11
				
				break;
			case 12:             //F12
				
				break;
			default :
				break;
		}
	}
	else if(Num == 2)
	{
		switch(Data)
		{
			case 100:              //Pause
				
				break;
			case 101:              //Home
				break;
			case 102:              //Paup
				
				break;
			case 103:              //Padn
				
				break;
			case 104:              //end
				break;
			default :
				break;
		}
		
	}
}
