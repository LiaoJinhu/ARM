#include "oled.h"

/*===========================================================
�ļ����ƣ�OLED�����ļ�
�������ã�OLED_CS        P2.0
          OLED_RES       P2.1
          OLED_DC        P2.2
          OLED_SCLK      P2.3
          OLED_SDIN      P2.4
ʹ��˵����ʹ��ʱ��OLED.c��OLED.h��ͬ��ӽ�����
          �����и�����������
��Ҫ������void OLED_Init(void);
          ֱ�ӵ����Գ�ʼ��Һ��

          void OLED_Fill(uint8_t dat);
          �����ַ����������������Ļ������������0x00������

          void OLED_Show_Sentence(uint8_t x, uint8_t y, uint8_t *dataptr);
          ������ʾ��������x��y���Լ��ַ���ָ������Һ������λ����ʾ�ַ���
===========================================================*/

/*============================================================
                    LED���������ú���   ��ʼ
=============================================================*/
void SetStartColumn(uint8_t dat)
{
    OLED_Write_Byte((0x00+dat%16), COM);        // Set Lower Column Start Address for Page Addressing Mode
                                                // Default => 0x00
    OLED_Write_Byte((0x10+dat/16), COM);        // Set Higher Column Start Address for Page Addressing Mode
                                                // Default => 0x10
}

void SetAddressingMode(uint8_t dat)
{
    OLED_Write_Byte(0x20, COM);         // Set Memory Addressing Mode
    OLED_Write_Byte(dat, COM);          // Default => 0x02
                                        // 0x00 => Horizontal Addressing Mode
                                        // 0x01 => Vertical Addressing Mode
                                        // 0x02 => Page Addressing Mode
}

void SetColumnAddress(uint8_t a, uint8_t b)
{
    OLED_Write_Byte(0x21, COM);        // Set Column Address
    OLED_Write_Byte(a, COM);           // Default => 0x00 (Column Start Address)
    OLED_Write_Byte(b, COM);           // Default => 0x7F (Column End Address)
}

void SetPageAddress(uint8_t a, uint8_t b)
{
    OLED_Write_Byte(0x22, COM);        // Set Page Address
    OLED_Write_Byte(a, COM);           // Default => 0x00 (Page Start Address)
    OLED_Write_Byte(b, COM);           // Default => 0x07 (Page End Address)
}

void SetStartLine(uint8_t dat)
{
    OLED_Write_Byte((0x40|dat), COM);       // Set Display Start Line
                                            // Default => 0x40 (0x00)
}

void SetContrastControl(uint8_t dat)
{
    OLED_Write_Byte(0x81, COM);          // Set Contrast Control
    OLED_Write_Byte(dat, COM);           // Default => 0x7F
}

void SetChargePump(uint8_t dat)
{
    OLED_Write_Byte(0x8D, COM);             // Set Charge Pump
    OLED_Write_Byte((0x10|dat), COM);       // Default => 0x10
                                            // 0x10 (0x00) => Disable Charge Pump
                                            // 0x14 (0x04) => Enable Charge Pump
}

void SetSegmentRemap(uint8_t dat)
{
    OLED_Write_Byte((0xA0|dat), COM);       // Set Segment Re-Map
                                            // Default => 0xA0
                                            // 0xA0 (0x00) => Column Address 0 Mapped to SEG0
                                            // 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void SetEntireDisplay(uint8_t dat)
{
    OLED_Write_Byte((0xA4|dat), COM);       // Set Entire Display On / Off
                                            // Default => 0xA4
                                            // 0xA4 (0x00) => Normal Display
                                            // 0xA5 (0x01) => Entire Display On
}

void SetInverseDisplay(uint8_t dat)
{
    OLED_Write_Byte((0xA6|dat), COM);       // Set Inverse Display On/Off
                                            // Default => 0xA6
                                            // 0xA6 (0x00) => Normal Display
                                            // 0xA7 (0x01) => Inverse Display On
}

void SetMultiplexRatio(uint8_t dat)
{
    OLED_Write_Byte(0xA8, COM);              // Set Multiplex Ratio
    OLED_Write_Byte(dat, COM);               // Default => 0x3F (1/64 Duty)
}

void SetDisplayOnOff(uint8_t dat)
{
    OLED_Write_Byte((0xAE|dat), COM);       // Set Display On/Off
                                            // Default => 0xAE
                                            // 0xAE (0x00) => Display Off
                                            // 0xAF (0x01) => Display On
}

void SetStartPage(uint8_t dat)
{
    OLED_Write_Byte((0xB0|dat), COM);       // Set Page Start Address for Page Addressing Mode
                                            // Default => 0xB0 (0x00)
}

void SetCommonRemap(uint8_t dat)
{
    OLED_Write_Byte((0xC0|dat), COM);       // Set COM Output Scan Direction
                                            // Default => 0xC0
                                            // 0xC0 (0x00) => Scan from COM0 to 63
                                            // 0xC8 (0x08) => Scan from COM63 to 0
}

void SetDisplayOffset(uint8_t dat)
{
    OLED_Write_Byte(0xD3, COM);         // Set Display Offset
    OLED_Write_Byte(dat, COM);          // Default => 0x00
}

void SetDisplayClock(uint8_t dat)
{
    OLED_Write_Byte(0xD5, COM);         // Set Display Clock Divide Ratio / Oscillator Frequency
    OLED_Write_Byte(dat, COM);          // Default => 0x80
                                        // D[3:0] => Display Clock Divider
                                        // D[7:4] => Oscillator Frequency
}

void SetPrechargePeriod(uint8_t dat)
{
    OLED_Write_Byte(0xD9, COM);         // Set Pre-Charge Period
    OLED_Write_Byte(dat, COM);          // Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
                                        // D[3:0] => Phase 1 Period in 1~15 Display Clocks
                                        // D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void SetCommonConfig(uint8_t dat)
{
    OLED_Write_Byte(0xDA, COM);             // Set COM Pins Hardware Configuration
    OLED_Write_Byte((0x02|dat), COM);       // Default => 0x12 (0x10)
                                            // Alternative COM Pin Configuration
                                            // Disable COM Left/Right Re-Map
}

void SetVCOMH(uint8_t dat)
{
    OLED_Write_Byte(0xDB, COM);         // Set VCOMH Deselect Level
    OLED_Write_Byte(dat, COM);          // Default => 0x20 (0.77*VCC)
}

void SetNop(void)
{
    OLED_Write_Byte(0xE3, COM);        // Command for No Operation
}

void OLED_Set_XY(uint8_t x, uint8_t y)
{ 
    OLED_Write_Byte((0xb0 + y), COM);
    OLED_Write_Byte(((((x*6) & 0xf0) >> 4) | 0x10), COM);
    OLED_Write_Byte((((x*6) & 0x0f) | 0x00), COM); 
} 
 
/*============================================================
                    LED���������ú���   ����
=============================================================*/

/***************************************
���ƣ������ʾ����
���ܣ���OLED���ĳһ�ַ�
������uint8_t dat��Ҫ��������
���أ���
****************************************/
void OLED_Fill(uint8_t dat)
{
    uint8_t Page, Column;
    
    for(Page = 0; Page < 8; Page++)
    {
        OLED_Write_Byte((0xb0 + Page), COM);  //0xb0+0~7��ʾҳ0~7
        OLED_Write_Byte(0x00, COM);           //0x00+0~16��ʾ��128�зֳ�16�����ַ��ĳ���еĵڼ���
        OLED_Write_Byte(0x10, COM);           //0x10+0~16��ʾ��128�зֳ�16�����ַ���ڵڼ���
        
        for(Column = 0; Column < 128; Column++)
        {
            OLED_Write_Byte(dat, DAT);
        }
    }
} 

void OLED_Init(void)
{
    uint8_t i;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin   = OLED_CS_PIN|OLED_DC_PIN|OLED_RES_PIN|OLED_SDIN_PIN|OLED_SCLK_PIN;				
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
    OLED_SCLK_H;
    OLED_RES_L;
    for(i = 0; i < 100; i++)
    {
        __NOP();              //�ȴ�RC��λ���      
    } 
    OLED_RES_H;
    
    SetDisplayOnOff(0x00);     // Display Off (0x00/0x01)
    SetDisplayClock(0x80);     // Set Clock as 100 Frames/Sec
    SetMultiplexRatio(0x3F);   // 1/64 Duty (0x0F~0x3F)
    SetDisplayOffset(0x00);    // Shift Mapping RAM Counter (0x00~0x3F)
    SetStartLine(0x00);        // Set Mapping RAM Display Start Line (0x00~0x3F)
    SetChargePump(0x04);       // Enable Embedded DC/DC Converter (0x00/0x04)
    SetAddressingMode(0x02);   // Set Page Addressing Mode (0x00/0x01/0x02)
    SetSegmentRemap(0x01);     // Set SEG/Column Mapping     0x00���ҷ��� 0x01����
    SetCommonRemap(0x08);      // Set COM/Row Scan Direction 0x00���·��� 0x08����
    SetCommonConfig(0x10);     // Set Sequential Configuration (0x00/0x10)
    SetContrastControl(0xCF);  // Set SEG Output Current
    SetPrechargePeriod(0xF1);  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SetVCOMH(0x40);            // Set VCOM Deselect Level
    SetEntireDisplay(0x00);    // Disable Entire Display On (0x00/0x01)
    SetInverseDisplay(0x00);   // Disable Inverse Display On (0x00/0x01)  
    SetDisplayOnOff(0x01);     // Display On (0x00/0x01)
    OLED_Fill(0x00);            // ��ʼ����
    OLED_Set_XY(0, 0);
}

/*********************************************************************
���ƣ�OLEDд�ֽں���
���ܣ���OLEDдһ���ֽڵ����ݻ�����
������uint8_t uint8_t��Ҫд������ݻ�����
      uint8_t control������д�����ݻ����DATΪ���ݣ�COMΪ����
���أ���
*********************************************************************/
static void OLED_Write_Byte(uint8_t data, uint8_t control)
{
    uint8_t i = 0;
    
    OLED_CS_L;
    
    if(control == DAT)
    {
        OLED_DC_H;
    }
    if(control == COM)
    {
        OLED_DC_L;
    }
    
    for(i = 0; i < 8; i++)
    {
        if(data&0x80)
        {
            OLED_SDIN_H;
        }
        else
        {
            OLED_SDIN_L;
        }
        
        OLED_SCLK_L;
        data <<= 1;
        OLED_SCLK_H;
    }
    
    OLED_CS_H;
}

/***************************************
���ƣ������ʾ����
���ܣ���(x,y)��ʾ���
������X--�M����  Y--������
���أ���
****************************************/
void OLED_Cursor(uint8_t x,uint8_t y)
{
	uint8_t i=0;
    OLED_Set_XY(x,y);        
    for(i = 0; i < 3; i++)
    {
        OLED_Write_Byte(0XFF, DAT);
    }
}

/***************************************
���ƣ�����������
���ܣ���(x,y)������
������X--�M����  Y--������
���أ���
****************************************/
void OLED_ClearCursor(uint8_t x,uint8_t y)
{
	uint8_t i=0;
    OLED_Set_XY(x,y);
    for(i = 0;i < 3; i++)
    {
        OLED_Write_Byte(0x00, DAT);
    }
}

/***************************************
���ƣ���������
���ܣ�����
��������
���أ���
****************************************/
void OLED_CLS(void)
{
	uint8_t y,x;	
	for(y=0;y<8;y++)
	{
		OLED_Write_Byte((0xb0+y),COM);
		OLED_Write_Byte(0x00,COM);
		OLED_Write_Byte(0x10,COM); 
		for(x=0;x<128;x++)
			OLED_Write_Byte(0,DAT);
	}
}

/***************************************
���ƣ�������������
���ܣ���(x,y)��ʼ���N���ַ�
��������
���أ���
****************************************/
void OLED_PARTCLS(uint8_t x,uint8_t y,uint8_t len)
{
    OLED_Set_XY(x,y);
    len = len*6;
    while(len --)
    {
        OLED_Write_Byte(0,DAT);
    }
}

/***********************************************************
���ƣ�OLEDд�ַ�����
���ܣ���OLEDд��һ���ַ�
������uint8_t data��Ҫ��ʾ���ַ����ӿո�32����ʼ
���أ���
***********************************************************/
static void OLED_Print_ASCII(uint8_t data)
{
    uint8_t i;
    
    if((data-32) > 90)  //��������Χ��ֱ�ӷ���
    {
        return;
    }
    
    for(i = 0; i < 6; i++)
    {
        OLED_Write_Byte(ASCII_Table[data-32][i], DAT);
    }
}

/***************************************************************
���ƣ�OLED�ַ�����ʾ����
���ܣ���OLED����ʾһ���ַ���
������uint8_t x����ʾ����ʼ�����꣬��Χ0~21
      uint8_t y����ʾ����ʼ�����꣬��Χ0~7
      uint8_t *dataptr����ʾ���ַ���ָ��
������(0,0)��ʾWHUT
OLED_Show_Sentence(0,0,"WHUT");
ע�⣺�����ݵĵ������������ַ����飬Ҫȷ�����һ��Ԫ���ǡ�\0��
****************************************************************/
void OLED_Show_Sentence(uint8_t x, uint8_t y, uint8_t *dataptr)
{
    uint8_t i = 0;
    
    OLED_Set_XY(x, y);
    
    while(*dataptr)
    {        
        OLED_Print_ASCII(*dataptr++);
        
        i++;
    }
}

/***************************************************************
���ƣ�OLED������ʾ����
���ܣ���OLED����ʾ����
������uint8_t x����ʾ����ʼ�����꣬��Χ0~21
      uint8_t y����ʾ����ʼ�����꣬��Χ0~7
      int32_t data:��Ҫ��ʾ�����֣���Χ-99999~99999
������(0,0)��ʾ9531
OLED_Show_Number(0,0,9531)
****************************************************************/
void OLED_Show_Number(uint8_t x, uint8_t y, int32_t data)
{
    char s[7]="      ";
    if(data>=0)s[0]=' ';
    else
    {
        s[0]='-';
        data=-data;
    }
    if(data>9999) s[1] = data/10000 + '0';
    if(data>999)  s[2] = data%10000/1000 + '0';
    if(data>99)   s[3] = data%1000/100 + '0';
    if(data>9)    s[4] = data%100/10 + '0';
    s[5] = data%10 + '0';
    s[6] ='\0';
    
    OLED_Show_Sentence(x,y,s);
}

/***************************************************************
���ƣ�OLED�ַ���+������ʾ����
���ܣ���OLED����ʾ�ַ���+����
������uint8_t x����ʾ����ʼ�����꣬��Χ0~21
     uint8_t y����ʾ����ʼ�����꣬��Χ0~7
     uint8_t *dataptr����ʾ���ַ���ָ�� 
     int32_t data:��Ҫ��ʾ�����֣���Χ-99999~99999
������0,0��ʾValue:9982
OLED_Show_StrNum(0,0,"Value:",9982);
****************************************************************/
void OLED_Show_StrNum(uint8_t x, uint8_t y,uint8_t *dataptr,int32_t data)
{
    char s[7]="      ";
    char length = 0;
    if(data>=0)s[0]=' ';
    else
    {
        s[0]='-';
        data=-data;
    }
    if(data>9999) s[1] = data/10000 + '0';
    if(data>999)  s[2] = data%10000/1000 + '0';
    if(data>99)   s[3] = data%1000/100 + '0';
    if(data>9)    s[4] = data%100/10 + '0';
    s[5] = data%10 + '0';
    s[6] ='\0';
    length = strlen(dataptr);
    if(length > 15)
    {
	  OLED_Show_Sentence(x,y,"String is too long");  
    }
    else
    {
    	OLED_Show_Sentence(x,y,dataptr);
    	OLED_Show_Sentence(x+length,y,s);
    }
}

/***************************************************************
���ƣ�OLEDͼƬ��ʾ����
���ܣ���ʾBMPͼƬ����СΪ(x1-x0+1)X((y1-y0+1)*8)
	  BMPͼƬ��ȡҳ��yΪ0��8λ(��λ����)Ϊ��λȡ��(x1-x0+1)��
	  ��ȡҳ��yΪ1ͼƬ��ʾ��8λ(��λ����)Ϊ��λȡ��(x1-x0+1)����������
������x�ķ�ΧΪ0-127��yΪҳ�ķ�Χ0~7
ע�⣺δ����
****************************************************************/
void OLED_PrintBMP(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t bmp[])
{ 	
	uint16_t ii=0;
	uint8_t x,y;
	for(y=y0;y<=y1;y++)
	{
		OLED_Set_XY(x0,y);				
		for(x=x0;x<x1;x++)
		{      
			OLED_Write_Byte(bmp[ii++],DAT);	    	
		}
	}
}


/***************************************************************
���ƣ�OLED������ʾ����
���ܣ���ʾ����
������LrΪ��������ָ�룬RrΪ��������ָ�룬MrΪ��������ָ��
ע�⣺δ����
****************************************************************/
//#define ZOOMHIGH   64			//OLED����ʾ����������ĸ�
//#define ZOOMWIDTH  80			//OLED����ʾ����������Ŀ�
//void Road_Show(uint8_t* Lr,uint8_t* Rr)//,uint8_t* Mr)
//{
//    uint8_t Data[ZOOMHIGH * ZOOMWIDTH / 8] = {0};		//��������
//    uint8_t line;
//    uint8_t Num2 ,k;
//    uint16_t  Num1;
//	uint8_t Line = 0;
//	uint8_t Row = 0;
//	uint8_t Col = 0;
//    for(Line = 0;Line < ZOOMHIGH;Line++)	//64��
//    {
//        line = Line << 1;	   //ȡͼ�������е�ż����,��0 2 4 6...128
//        Num2 = 1 << (Line%8);  //

//        if(Lr[line] != 255)	   //��ȡͼ��ı�����Ч
//        {
//            k =  (Lr[line]) / (Image_Width / ZOOMWIDTH) ;
//            Num1 = (Line >> 3) * ZOOMWIDTH + k;
//            Data[Num1] |= Num2;
//        }
//        if(Rr[line] != 255)
//        {
//            k =  (Rr[line])/ (Image_Width / ZOOMWIDTH);
//            Num1 = (Line >> 3) * ZOOMWIDTH + k;
//            Data[Num1] |= Num2;
//        }
////        if(Mr[line] != 255)
////        { 
////            k =  (Mr[line])/3;
////            Num1 = (Line / 8) * 80 + k;
////            Data[Num1] |= Num2;
////        }
//    }
//    for(Row = 0; Row < (ZOOMHIGH / 8) ; Row++) //80��
//    {
//        OLED_Set_XY(0,Row);
//        for(Col = 0; Col < ZOOMWIDTH; Col++) 
//        {
//			OLED_Write_Byte(Data[Row * 80 + Col],DAT);
//        }
//    }
//}

/***************************************************************
����:OLED�ַ���+������ʾ����
����:��OLED����ʾ�ַ���+��С��������
����:uint8_t x����ʾ����ʼ�����꣬��Χ0~21
	 uint8_t y����ʾ����ʼ�����꣬��Χ0~7
     uint8_t *detaptr����ʾ���ַ���ָ��
     float data����Ҫ��ʾ������
������0,0��ʾDouble:982.98
OLED_Show_StrDouble(0,0,"Double:",982.98);
****************************************************************/
void OLED_Show_StrDouble(uint8_t x, uint8_t y,uint8_t *dataptr,double data)
{
	uint8_t length = 0;
    int32_t d;
    char s[10];
    if(data>=0)
    {
        s[0]='+';
    }
    else
    {
        s[0]='-';
        data=-data;
    }
    d=(int32_t)data;
    data=data-d;
    s[1]=d/100+'0';
    s[2]=d%100/10+'0';
    s[3]=d%10+'0';
    s[4]='.';
	
    data*=100000;
    d=(long)data;
    d/=10;
    s[5] = d/1000 + '0';
    s[6] = d%1000/100 + '0';
    s[7] = d%100/10 + '0';
    s[8] = d%10+'0';
    s[9] = '\0';
	
	length = strlen(dataptr);
	if(length > 12)
    {
	  	OLED_Show_Sentence(x,y,"String is too long");  
    }
	else
    {
    	OLED_Show_Sentence(x,y,dataptr);
    	OLED_Show_Sentence(x+length,y,s);
    }
}
