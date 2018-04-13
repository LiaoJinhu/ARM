#ifndef _SD_H_
#define _SD_H_		 
#include "common.h"	 
 
//V1.1�޸�˵��
//2010/5/13									   
//������һЩ��ʱ,ʵ�����֧��TF��(1G/2G),��ʿ��2G,4G 16G SD��
//2010/6/24
//������uint8_t SD_GetResponse(uint8_t Response)����
//�޸���uint8_t SD_WaitDataReady(void)����
//������USB������֧�ֵ�uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
//��uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);��������
////////////////////////////////////////////////////////////////////////////////// 	  
 						    	 
// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06	   
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//����д���Ӧ������
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD����Ӧ�����
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF
 							   						 	 
//�ⲿ��Ӧ���ݾ�����������޸�!
//Mini STM32ʹ�õ���PA3��ΪSD����CS��.

#define	SD_CS_H   GPIOA->BSRR = GPIO_Pin_3    //   PAout(3) //SD��Ƭѡ����					    	  
#define	SD_CS_L   GPIOA->BRR =  GPIO_Pin_3 

extern uint8_t  SD_Type;//SD��������
//���������� 
uint8_t SD_WaitReady(void);							//�ȴ�SD��׼��
uint8_t SD_GetResponse(uint8_t Response);					//�����Ӧ
uint8_t SD_Initialize(void);							//��ʼ��
uint8_t SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//����
uint8_t SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt);		//д��
uint32_t SD_GetSectorCount(void);   					//��������
uint8_t SD_GetCID(uint8_t *cid_data);                     //��SD��CID
uint8_t SD_GetCSD(uint8_t *csd_data);                     //��SD��CSD
 
#endif





//���������� 
/*
uint8_t SD_WaitReady(void);                          //�ȴ�SD������
uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);     //SD������һ������
uint8_t SD_SendCommand_NoDeassert(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_Init(void);                               //SD����ʼ��
uint8_t SD_Idle_Sta(void);                           //����SD��������ģʽ

uint8_t SD_ReceiveData(uint8_t *data, u16 len, uint8_t release);//SD��������
uint8_t SD_GetCID(uint8_t *cid_data);                     //��SD��CID
uint8_t SD_GetCSD(uint8_t *csd_data);                     //��SD��CSD
uint32_t SD_GetCapacity(void);                       //ȡSD������
//USB ������ SD����������
uint8_t MSD_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t NumByteToWrite);
uint8_t MSD_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);

uint8_t SD_ReadSingleBlock(uint32_t sector, uint8_t *buffer);  //��һ��sector
uint8_t SD_WriteSingleBlock(uint32_t sector, const uint8_t *buffer); 		//дһ��sector
uint8_t SD_ReadMultiBlock(uint32_t sector, uint8_t *buffer, uint8_t count); 	//�����sector
uint8_t SD_WriteMultiBlock(uint32_t sector, const uint8_t *data, uint8_t count);//д���sector
uint8_t SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);//��ȡһbyte
#endif
*/




