/*===========================================================
�ļ����ƣ�OLED����ͷ�ļ�
�������ã�CS        P2.0
          RES       P2.1
          DC        P2.2
          SCLK      P2.3
          SDIN      P2.4
ʹ��˵����ʹ��ʱ��OLED.c��OLED.h��ͬ���ӽ�����
          �����и�����������
��Ҫ������void OLED_Init(void);
          ֱ�ӵ����Գ�ʼ��Һ��

          void OLED_Fill(uint8_t dat);
          �����ַ����������������Ļ������������0x00������

          void OLED_Show_Sentence(uint8_t x, uint8_t y, uint8_t *dataptr);
          ������ʾ��������x��y���Լ��ַ���ָ������Һ������λ����ʾ�ַ���
===========================================================*/

#ifndef __OLED_H__
#define __OLED_H__

#include "common.h"
#include "MyMath.h"
#include "string.h"

#define DAT 1
#define COM 0

//#define OLED_CS_PIN   14
//#define OLED_DC_PIN   15
//#define OLED_RES_PIN  5
//#define OLED_SDIN_PIN 6      //D1
//#define OLED_SCLK_PIN 7      //D0

#define OLED_CS_PIN   GPIO_Pin_9
#define OLED_DC_PIN   GPIO_Pin_7
#define OLED_RES_PIN  GPIO_Pin_8
#define OLED_SDIN_PIN GPIO_Pin_5       //D1
#define OLED_SCLK_PIN GPIO_Pin_6       //D0


#define OLED_CS_PORT   GPIOB
#define OLED_DC_PORT   GPIOB
#define OLED_RES_PORT  GPIOB
#define OLED_SDIN_PORT GPIOB
#define OLED_SCLK_PORT GPIOB

#define OLED_CS_H    OLED_CS_PORT->BSRR = OLED_CS_PIN
#define OLED_CS_L    OLED_CS_PORT->BRR =  OLED_CS_PIN
#define OLED_RES_H   OLED_DC_PORT->BSRR = OLED_RES_PIN
#define OLED_RES_L   OLED_DC_PORT->BRR =  OLED_RES_PIN
#define OLED_DC_H    OLED_RES_PORT->BSRR = OLED_DC_PIN
#define OLED_DC_L    OLED_RES_PORT->BRR =  OLED_DC_PIN
#define OLED_SCLK_H  OLED_SDIN_PORT->BSRR = OLED_SCLK_PIN
#define OLED_SCLK_L  OLED_SDIN_PORT->BRR =  OLED_SCLK_PIN
#define OLED_SDIN_H  OLED_SCLK_PORT->BSRR = OLED_SDIN_PIN
#define OLED_SDIN_L  OLED_SCLK_PORT->BRR =  OLED_SDIN_PIN

typedef enum 
{    
	OLED_SARTPAGE,
	OLED_EXISTPREDATA,
	OLED_SAVEDATA,
	OLED_INDEXDATA,
}OLED_EVENT;

void SetStartColumn(uint8_t dat);
void SetAddressingMode(uint8_t dat);
void SetColumnAddress(uint8_t a, uint8_t b);
void SetPageAddress(uint8_t a, uint8_t b);
void SetStartLine(uint8_t dat);
void SetContrastControl(uint8_t dat);
void SetChargePump(uint8_t dat);
void SetSegmentRemap(uint8_t dat);
void SetEntireDisplay(uint8_t dat);
void SetInverseDisplay(uint8_t dat);
void SetMultiplexRatio(uint8_t dat);
void SetDisplayOnOff(uint8_t dat);
void SetStartPage(uint8_t dat);
void SetCommonRemap(uint8_t dat);
void SetDisplayOffset(uint8_t dat);
void SetDisplayClock(uint8_t dat);
void SetPrechargePeriod(uint8_t dat);
void SetCommonConfig(uint8_t dat);
void SetVCOMH(uint8_t dat);
void SetNop(void);
void OLED_Set_XY(uint8_t x, uint8_t y);  //�������꺯��

void OLED_Init(void);
void OLED_Fill(uint8_t dat);
void OLED_Cursor(uint8_t x,uint8_t y);
void OLED_ClearCursor(uint8_t x,uint8_t y);
void OLED_CLS(void);
void OLED_PARTCLS(uint8_t x,uint8_t y,uint8_t len);
void OLED_Show_Sentence(uint8_t x, uint8_t y, uint8_t *dataptr);
void OLED_Show_Number(unsigned char x, unsigned char y, int32_t data);
void OLED_Show_StrNum(unsigned char x, unsigned char y,unsigned char *dataptr,int32_t data);
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[]);
//void Road_Show(uint8_t* Lr,uint8_t* Rr);//,uint8_t* Mr);
void OLED_Show_StrDouble(unsigned char x, unsigned char y,unsigned char *dataptr,double data);
static void OLED_Print_ASCII(uint8_t data);
static void OLED_Write_Byte(uint8_t byte, uint8_t control);

static const uint8_t ASCII_Table[][6] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   //sp 0
    { 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // ! 1
    { 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // " 2
    { 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // # 3
    { 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $ 4
    { 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // % 5
    { 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // & 6
    { 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // ' 7
    { 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // ( 8
    { 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // ) 9
    { 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // * 10
    { 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // + 11
    { 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // , 12
    { 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // - 13
    { 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // . 14
    { 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // / 15
    { 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0 16
    { 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1 17
    { 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2 18
    { 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3 19
    { 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4 20
    { 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5 21
    { 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6 22
    { 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7 23
    { 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8 24
    { 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9 25
    { 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // : 26
    { 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ; 27
    { 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // < 28
    { 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // = 29
    { 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // > 30
    { 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ? 31
    { 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @ 32
    { 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A 33
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B 34
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C 35
    { 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D 36
    { 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E 37
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F 38
    { 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G 39
    { 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H 40
    { 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I 41
    { 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J 42
    { 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K 43
    { 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L 44
    { 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M 45
    { 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N 46
    { 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O 47
    { 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P 48
    { 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q 49
    { 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R 50
    { 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S 51
    { 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T 52
    { 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U 53
    { 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V 54
    { 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W 55
    { 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X 56
    { 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y 57
    { 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z 58
    { 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 59
    { 0x00, 0x02, 0x04, 0x08, 0x10, 0x20 },   // \ 60
    { 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ] 61
    { 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^ 62
    { 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _ 63
    { 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // ' 64
    { 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a 65
    { 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b 66
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c 67
    { 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d 68
    { 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e 69
    { 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f 70
    { 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g 71
    { 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h 72
    { 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i 73
    { 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j 74
    { 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k 75
    { 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l 76
    { 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m 77
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n 78
    { 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o 79
    { 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p 80
    { 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q 81
    { 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r 82
    { 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s 83
    { 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t 84
    { 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u 85
    { 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v 86
    { 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w 87
    { 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x 88
    { 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y 89
    { 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z9 0
    { 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    //horiz lines 91
};

#endif