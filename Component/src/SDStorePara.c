#include "SDStorePara.h"

//Ҫ��SDPage * PageWi <= 127
#define SDPage 18
#define PageWi 7
#define MsgNum 6
#define USE_SDCARD

uint8_t existPerData = false;
uint8_t page = 0,wi = 0;
uint32_t paraSector = 1;				  //��������
uint32_t dataCount = 0;					  //�������ݼ���

int32_t wSDPara[SDPage][PageWi];	      //д��Ļ�����
int32_t rSDPara[SDPage][PageWi];	      //�����Ļ�����

uint8_t SDDataBuffer[512]={0};			  //���ݻ�����
uint8_t SDCmdBuffer[512] = {0};		      //�������

uint8_t readSDParaOk = false;			  //��SD������
SDCardInfoClass SDCardInfo;
OLED_EVENT OledState = OLED_SARTPAGE;

//��ʼ��SD��,������ʲô��,����0���ǳ�ʼ����ȷ
void SDCard_Init(void)
{
#ifdef USE_SDCARD							//ʹ��SD��
	uint8_t SDStatus = 1;
	SDStatus = SD_Initialize();		//��ʼ���ɹ��Ļ�,SDStatusӦ��Ϊ0
	SDCardInfo.SDCState = SDStatus;	    
	SDCardInfo.Sectors = SD_GetSectorCount();  //��ȡSD������,��λ0.5Kb  (��Ϊһ������Ϊ512�ֽ�)
	if(SDStatus == 0)
	{
		SDCardInfo.SDCState = true;
		OLED_Show_Sentence(0,0,"SD initialize succeed");
		OLED_Show_StrNum(0,1,"SD Card Size:",SDCardInfo.Sectors>>11);
		OLED_Show_Sentence(19,1,"Mb");
		OLED_Show_Sentence(0,3,"Please press key to");
		OLED_Show_Sentence(0,4,"save data!");
	}
	else
	{
		SDCardInfo.SDCState = false;
		OLED_Show_Sentence(0,0,"SD initialize false!");
	}
	SDCardInfo.TransferTime  = 0;
#endif
}

//������   ���������һ���
uint8_t saveSDPara(float data)
{
	uint8_t state = false;
	dataCount++;									//���ݼ����Լ�
	if(OledState == OLED_SAVEDATA)					//�����ݵ�ʱ�����ʾ
	{
		OLED_Show_StrNum(0,0,"DataNum:",dataCount);
		OLED_Show_StrDouble(0,1,"Data:",data);
	}
	state = writeSDPara(DATA,SDDataBuffer,paraSector,data);			//��д����
	state &= writeSDPara(CMD,SDCmdBuffer,0,0);						//��д����
	return state;
}

//���������ϵͳ
void setSDCmdToSys(void)
{
	if(readSDCmd() == true)								//ֻ�ڿ�ʼ����  �������ж��Ƿ���ھ�����
	{
		existPerData = SDCmdBuffer[3];
		if(existPerData == true)						//�ڴ濨�������ǰ������
		{
			OledState = OLED_EXISTPREDATA;				//OLED�¼�--->��������
			OLED_CLS();									//����
			page = SDCmdBuffer[4];						//�Ȱ����ݶ�����,���ݰ���ѡ��֮���ٽ��и���
			wi   = SDCmdBuffer[5];
			paraSector = SDCmdBuffer[6];
			dataCount = SDCmdBuffer[8]*255+SDCmdBuffer[7];
			OLED_Show_Sentence(0,0,"Exist previous data!!");
			OLED_Show_StrNum(0,1,"DataNum:",dataCount);
			OLED_Show_Sentence(0,3,"Press key1 to continu");
			OLED_Show_Sentence(0,4,"e to save data!");
			
			OLED_Show_Sentence(0,6,"Press key2 to refill");
			OLED_Show_Sentence(0,7,"data!");
		}
	}	
}
//�����ݴ�SD������ϵͳ
void setSDDataToSys(void)
{
	uint8_t i,j;
	readSDParaOk = readSDData(paraSector);				//������
	if(readSDParaOk)									//��ȡ�ɹ�
	{
		for(i=0;i<=page;i++)
		{
			if(i == page)
			{
				for(j=0;j<=wi;j++)
				{
					wSDPara[i][j] = rSDPara[i][j];
				}
			}
			else
			{
				for(j=0;j<7;j++)
				{
					wSDPara[i][j] = rSDPara[i][j];
				}
			}
		}
		
		wi++;
	
		if((page == (SDPage - 1)) && (wi == PageWi))		//����������
		{
			paraSector++;									//SD����������   һ�����126������
			page = 0;										//����������
			wi   = 0;
		}
		else if(wi == PageWi)								//һҳ����������
		{
			page++;											//��ҳ
			wi = 0;
		}
	}
	
}

//��ȡ�Ѿ�����ܵ���������
void indexCmd(uint32_t *indexTotal)
{
	uint8_t buffer[512] = {0};
	
	if(readSDPara(CMD,buffer,0))
	{
		*indexTotal = buffer[8]*255+buffer[7]; 
	}
}

//��������
void indexData(INDEXTYPE addOrSub, uint32_t maxIndexNum)
{
	static uint8_t indexCount = 0;
	static int8_t  indexPage = -1;
	static uint8_t indexSector = 1;
	uint8_t showNum = 7,i = 0;
		
	OLED_CLS();
	if(addOrSub == ADD)					//ҳ������
	{
		indexPage  += 1;
		if(indexPage > 17)				//����ҳ���Ϊ17,���������Ҫ�ı�������
		{
			indexPage = 0;
			indexSector += 1;
		}
		
		if(indexCount >= maxIndexNum)	//�������ﵽ���֮��,�ڼ�֮ǰ,���ò���
		{
			indexCount = 0;
			indexPage = 0;
			indexSector = 1;
		}
		indexCount += 7;				//ÿ����һҳ  ���ݼ�7
		
		if(indexCount > maxIndexNum)    //�������������,˵����������ݲ���7�ı���,�õ�Ӧ����ʾ������
		{
			showNum = maxIndexNum - indexCount + 7;
		}
	}
	else if(addOrSub == SUB)			//ҳ����С
	{	
		indexPage  -= 1;
		if(indexPage < 0)				//����ҳ��СΪ0,�ټ�С����Ҫ�ı�������
		{
			if(indexSector == 1)		//�������С���������ټ�С��Ӧ�õ����������
			{
				indexSector = (maxIndexNum - 1)/126 + 1;
				indexPage = (maxIndexNum-1)/7 - (indexSector - 1)*18;
				showNum = maxIndexNum % 7;
				indexCount = maxIndexNum + 7 - showNum;
			}
			else
			{
				indexPage = 17;
				indexSector -= 1;
			}
		}
		if(showNum == 7)			   //ֻ��ȫҳ��ʾ��ʱ���Լ�
		{
			indexCount -= 7;
		}
	}
	readSDData(indexSector);
	OLED_Show_Sentence(0,0,"Index:");
	OLED_Show_Number(6,0,indexCount - 7 + showNum);
	
	if(showNum == 0)				  //��Ϊ7��������ʱ����Ҫ�������⴦��
	{
		showNum = 7;
	}
	for(i = 0;i < showNum;i++)
	{
		OLED_Show_StrDouble(0,i+1,"Data:",readSDDouble(indexPage,i));
	}
}

//�����ݴ浽������
static void pushDataToSDBuffer(float data)
{	
	existPerData = 1;
	SDCmdBuffer[3] = existPerData;						//�û�����Ϊ1˵��֮ǰ������ݣ��������֮���ٿ���ʱ������ǰ������
	SDCmdBuffer[4] = page;								//�ڼ�ҳ
	SDCmdBuffer[5] = wi;								//һҳ�е��Ǹ�����
	SDCmdBuffer[6] = paraSector;
	SDCmdBuffer[7] = dataCount%255;					    //��һ���洢�˶�������
	SDCmdBuffer[8] = dataCount/255;
	
	pushSDDouble(page,wi,data);
	wi++;
	
	if((page == (SDPage - 1)) && (wi == PageWi))		//����������
	{
		paraSector++;									//SD����������   һ�����126������
		page = 0;										//����������
		wi   = 0;
	}
	else if(wi == PageWi)								//һҳ����������
	{
		page++;											//��ҳ
		wi = 0;
	}
}

//������
static uint8_t readSDCmd(void)
{
	return (readSDPara(CMD,SDCmdBuffer,0));
}

//������
static uint8_t readSDData(uint8_t sector)
{
	return (readSDPara(DATA,SDDataBuffer,sector));
}

//��̬�������ڲ�����
static uint8_t writeSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector,float data)
{
	uint8_t check = 0;
	uint8_t SDBuff[512]={0};
	uint16_t i,j,p;

	if(SDCardInfo.SDCState == false)return false;	//��ʼ��ʧ�ܵĻ�ֱ�ӷ���
	
	Buffer[0] = 254;								//��ʼУ��λ
	Buffer[1] = 119;
	Buffer[2] = 3;
	
	check = (uint8_t)(254 + 119 + 3);
		
	p=3;
	if(type == CMD)									//����
	{
		for(i=0;i<MsgNum;i++)
		{
			check += Buffer[p];  
			p++; 
		}
	}
	else if(type == DATA)						   //����
	{
		pushDataToSDBuffer(data);				  //�����ݷŵ�SDPara����
		for(i=0;i<SDPage;i++)
		{
			for(j=0;j<PageWi;j++)
			{
			  Buffer[p] = (wSDPara[i][j]>>24)&0xFF;  check += Buffer[p];  p++; 
			  Buffer[p] = (wSDPara[i][j]>>16)&0xFF;  check += Buffer[p];  p++;
			  Buffer[p] = (wSDPara[i][j]>>8 )&0xFF;  check += Buffer[p];  p++;
			  Buffer[p] = (wSDPara[i][j]>>0 )&0xFF;  check += Buffer[p];  p++;
			}
		}
	}
	Buffer[p] = check;						   //��������֮���������У��λ
	
	i=0;
	while(i<10)
	{
		if(!SD_WriteDisk(Buffer,sector,1))//д��SD������ȥ
		{
			Delay_ms(2);
			j=0;
			while(j<4)
			{
				if(!SD_ReadDisk(SDBuff,sector,1))
					break; //��ȡ�ɹ�,��ȥ����
				j++;
			}
			if(j == 4)continue; //�����ȡ�����⣬˵��ûд�ɹ����ٴ�д

			j=1;
			for(p=0;p<512;p++)
			{
				if(Buffer[p] != SDBuff[p])//�����ȡ�ɹ���У��λҲӦ����ȷ
				{
					j = 0;
					break;
				}
			}
			if(j == 0)continue;//���У�������⣬�ٴ�д   
			return true;
		}
		i++;
	}
	return false;
}

static uint8_t readSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector)
{
	uint8_t check = 0;
	uint16_t i,j,p;
	if(SDCardInfo.SDCState ==0) return false;		  //SD����ʼ���д�ֱ�ӷ���

	i=0;
	while(i<10)
	{
		if(!SD_ReadDisk(Buffer,sector,1))  //�����ݵ�Buffer
		break;
		i++;
	}
	if(i==10)return false;							  //����10�λ���û���ɹ�

	p = 3;
	check = (uint8_t)(254 + 119 + 3);
	if(Buffer[0]==254 && Buffer[1]==119 && Buffer[2]==3)   //�����ʼУ��λ
	{
		if(type == CMD)
		{
			for(i=0;i<MsgNum;i++)
			{
				check += Buffer[p];
				p++;
			}
		}
		else if(type == DATA)
		{
			//�����ݵ����ݻ�����
			for(i=0;i<SDPage;i++)
			{
				for(j=0;j<PageWi;j++)
				{
					rSDPara[i][j] = ((int32_t)Buffer[p]<<24)+((int32_t)Buffer[p+1]<<16)+((int32_t)Buffer[p+2]<<8)+Buffer[p+3];
					check += Buffer[p];
					check += Buffer[p+1];
					check += Buffer[p+2];
					check += Buffer[p+3];
					p+=4;
				}
			}
		}
		
		//���У��λ
		if(Buffer[p]==check)	    
		{  
			return true;		 //У��λ���ɹ�
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

//��С�����ݷ��ڻ�������
static void pushSDDouble(uint8_t x,uint8_t y,double data)
{
	int32_t integer,decimal;				//�ֱ��������С��
	integer = (int32_t)data;	            //��ȡ��������
	data -= integer;				        //ȥ����������
	data*=100;				                //�õ�С������
	decimal = (int32_t)data;                //������
	wSDPara[x][y] = (integer<<8) + decimal;	 
}

//���������ݷ��ڻ�������
static void pushSDInt32(uint8_t x,uint8_t y,int32_t data)
{
	wSDPara[x][y] = data; 
}

//��С������
static double readSDDouble(uint8_t x,uint8_t y)
{
	return (rSDPara[x][y]>>8) + ((double)(rSDPara[x][y]&0xFF))/100;
}

//����������
static int32_t readSDInt32(uint8_t x,uint8_t y)
{
	return rSDPara[x][y];
}
