/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：spi_hard.c
 * 摘    要 ：定时器初始化
 * 函数接口：1.Spi_Hard_Init(void)	        硬件SPI初始化   不带片选 CS
 *			  2.SetSPISpeed(uint8_t speed)  设置SPI的速度
 *
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "spi_hard.h"

SPI_InitTypeDef SPI_InitStructure;

//硬件SPI的IO口初始化
void Spi_Hard_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//初始化GPIOA,SPI1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	//初始化IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
 	GPIO_SetBits(GPIOA,GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);

	
	//SPI配置
	//两根线 全双工
	//主机模式
	//数据大小：8位
	//时钟极性：高电平        即SPI通信空闲时SCK为高电平
	//时钟相位：偶数边采集
	//片选：软件模式
	//波特率分频值：4分频
	//高位数据在前，即MSB
	//CRC校验----对于该款Flash无意义
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	//使能SPI1
	SPI_Cmd(SPI1,ENABLE);
	
	//启动传输
	SPI_ReadWriteByte(0xff);
}

//SPI速度设置函数
//Speed:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
void SetSPISpeed(uint8_t speed)
{
	SPI_InitStructure.SPI_BaudRatePrescaler = speed;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

//SPI读写一个字节
//TxData:要写入的字节
//返回值：读取到的字节
uint8_t SPI_ReadWriteByte(uint8_t TxData)
{		
	uint8_t retry=0;
	 
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)   //检查指定的SPI标志位设置与否：发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI_I2S_SendData(SPI1, TxData); 							    //通过外设SPI发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否：接收缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI_I2S_ReceiveData(SPI1); 								//返回通过SPI最近接收的数据
}
