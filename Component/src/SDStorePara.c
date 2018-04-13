#include "SDStorePara.h"

//要求SDPage * PageWi <= 127
#define SDPage 18
#define PageWi 7
#define MsgNum 6
#define USE_SDCARD

uint8_t existPerData = false;
uint8_t page = 0,wi = 0;
uint32_t paraSector = 1;				  //数据扇区
uint32_t dataCount = 0;					  //储存数据计数

int32_t wSDPara[SDPage][PageWi];	      //写入的缓冲区
int32_t rSDPara[SDPage][PageWi];	      //读出的缓冲区

uint8_t SDDataBuffer[512]={0};			  //数据缓冲区
uint8_t SDCmdBuffer[512] = {0};		      //命令缓冲区

uint8_t readSDParaOk = false;			  //读SD卡参数
SDCardInfoClass SDCardInfo;
OLED_EVENT OledState = OLED_SARTPAGE;

//初始化SD卡,不论是什么卡,返回0才是初始化正确
void SDCard_Init(void)
{
#ifdef USE_SDCARD							//使用SD卡
	uint8_t SDStatus = 1;
	SDStatus = SD_Initialize();		//初始化成功的话,SDStatus应该为0
	SDCardInfo.SDCState = SDStatus;	    
	SDCardInfo.Sectors = SD_GetSectorCount();  //获取SD卡容量,单位0.5Kb  (因为一个扇区为512字节)
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

//存数据   命令和数据一起存
uint8_t saveSDPara(float data)
{
	uint8_t state = false;
	dataCount++;									//数据计数自加
	if(OledState == OLED_SAVEDATA)					//存数据的时候才显示
	{
		OLED_Show_StrNum(0,0,"DataNum:",dataCount);
		OLED_Show_StrDouble(0,1,"Data:",data);
	}
	state = writeSDPara(DATA,SDDataBuffer,paraSector,data);			//先写数据
	state &= writeSDPara(CMD,SDCmdBuffer,0,0);						//再写命令
	return state;
}

//将命令读到系统
void setSDCmdToSys(void)
{
	if(readSDCmd() == true)								//只在开始调用  读命令判断是否存在旧数据
	{
		existPerData = SDCmdBuffer[3];
		if(existPerData == true)						//内存卡里存有以前的数据
		{
			OledState = OLED_EXISTPREDATA;				//OLED事件--->存在数据
			OLED_CLS();									//清屏
			page = SDCmdBuffer[4];						//先把数据读回来,根据按键选择之后再进行更新
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
//将数据从SD卡读到系统
void setSDDataToSys(void)
{
	uint8_t i,j;
	readSDParaOk = readSDData(paraSector);				//读数据
	if(readSDParaOk)									//读取成功
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
	
		if((page == (SDPage - 1)) && (wi == PageWi))		//缓冲区已满
		{
			paraSector++;									//SD卡扇区增加   一个块存126个数据
			page = 0;										//缓冲区清零
			wi   = 0;
		}
		else if(wi == PageWi)								//一页的数据满了
		{
			page++;											//翻页
			wi = 0;
		}
	}
	
}

//获取已经存的总的数据数量
void indexCmd(uint32_t *indexTotal)
{
	uint8_t buffer[512] = {0};
	
	if(readSDPara(CMD,buffer,0))
	{
		*indexTotal = buffer[8]*255+buffer[7]; 
	}
}

//数据索引
void indexData(INDEXTYPE addOrSub, uint32_t maxIndexNum)
{
	static uint8_t indexCount = 0;
	static int8_t  indexPage = -1;
	static uint8_t indexSector = 1;
	uint8_t showNum = 7,i = 0;
		
	OLED_CLS();
	if(addOrSub == ADD)					//页数增加
	{
		indexPage  += 1;
		if(indexPage > 17)				//索引页最大为17,再增大就需要改变扇区了
		{
			indexPage = 0;
			indexSector += 1;
		}
		
		if(indexCount >= maxIndexNum)	//索引数达到最大之后,在加之前,重置参数
		{
			indexCount = 0;
			indexPage = 0;
			indexSector = 1;
		}
		indexCount += 7;				//每增加一页  数据加7
		
		if(indexCount > maxIndexNum)    //大于最大索引数,说明所存的数据不是7的倍数,得到应该显示的数据
		{
			showNum = maxIndexNum - indexCount + 7;
		}
	}
	else if(addOrSub == SUB)			//页数减小
	{	
		indexPage  -= 1;
		if(indexPage < 0)				//索引页最小为0,再减小就需要改变扇区了
		{
			if(indexSector == 1)		//如果是最小的扇区，再减小就应该到达最大扇区
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
		if(showNum == 7)			   //只在全页显示的时候自减
		{
			indexCount -= 7;
		}
	}
	readSDData(indexSector);
	OLED_Show_Sentence(0,0,"Index:");
	OLED_Show_Number(6,0,indexCount - 7 + showNum);
	
	if(showNum == 0)				  //在为7的整数倍时，需要进行特殊处理
	{
		showNum = 7;
	}
	for(i = 0;i < showNum;i++)
	{
		OLED_Show_StrDouble(0,i+1,"Data:",readSDDouble(indexPage,i));
	}
}

//将数据存到缓冲区
static void pushDataToSDBuffer(float data)
{	
	existPerData = 1;
	SDCmdBuffer[3] = existPerData;						//该缓冲区为1说明之前存过数据，避免掉电之后再开机时覆盖以前的数据
	SDCmdBuffer[4] = page;								//第几页
	SDCmdBuffer[5] = wi;								//一页中的那个数据
	SDCmdBuffer[6] = paraSector;
	SDCmdBuffer[7] = dataCount%255;					    //存一共存储了多少数据
	SDCmdBuffer[8] = dataCount/255;
	
	pushSDDouble(page,wi,data);
	wi++;
	
	if((page == (SDPage - 1)) && (wi == PageWi))		//缓冲区已满
	{
		paraSector++;									//SD卡扇区增加   一个块存126个数据
		page = 0;										//缓冲区清零
		wi   = 0;
	}
	else if(wi == PageWi)								//一页的数据满了
	{
		page++;											//翻页
		wi = 0;
	}
}

//读命令
static uint8_t readSDCmd(void)
{
	return (readSDPara(CMD,SDCmdBuffer,0));
}

//读数据
static uint8_t readSDData(uint8_t sector)
{
	return (readSDPara(DATA,SDDataBuffer,sector));
}

//静态函数，内部调用
static uint8_t writeSDPara(DATATYPE type,uint8_t *Buffer,uint8_t sector,float data)
{
	uint8_t check = 0;
	uint8_t SDBuff[512]={0};
	uint16_t i,j,p;

	if(SDCardInfo.SDCState == false)return false;	//初始化失败的话直接返回
	
	Buffer[0] = 254;								//起始校验位
	Buffer[1] = 119;
	Buffer[2] = 3;
	
	check = (uint8_t)(254 + 119 + 3);
		
	p=3;
	if(type == CMD)									//命令
	{
		for(i=0;i<MsgNum;i++)
		{
			check += Buffer[p];  
			p++; 
		}
	}
	else if(type == DATA)						   //数据
	{
		pushDataToSDBuffer(data);				  //将数据放到SDPara数组
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
	Buffer[p] = check;						   //所有数据之和组成最后的校验位
	
	i=0;
	while(i<10)
	{
		if(!SD_WriteDisk(Buffer,sector,1))//写到SD卡里面去
		{
			Delay_ms(2);
			j=0;
			while(j<4)
			{
				if(!SD_ReadDisk(SDBuff,sector,1))
					break; //读取成功,出去处理
				j++;
			}
			if(j == 4)continue; //如果读取有问题，说明没写成功，再次写

			j=1;
			for(p=0;p<512;p++)
			{
				if(Buffer[p] != SDBuff[p])//如果读取成功，校验位也应该正确
				{
					j = 0;
					break;
				}
			}
			if(j == 0)continue;//如果校验有问题，再次写   
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
	if(SDCardInfo.SDCState ==0) return false;		  //SD卡初始化有错直接返回

	i=0;
	while(i<10)
	{
		if(!SD_ReadDisk(Buffer,sector,1))  //读数据到Buffer
		break;
		i++;
	}
	if(i==10)return false;							  //读了10次还是没读成功

	p = 3;
	check = (uint8_t)(254 + 119 + 3);
	if(Buffer[0]==254 && Buffer[1]==119 && Buffer[2]==3)   //检查起始校验位
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
			//读数据到数据缓冲区
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
		
		//检查校验位
		if(Buffer[p]==check)	    
		{  
			return true;		 //校验位检查成功
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

//将小数数据放在缓冲区中
static void pushSDDouble(uint8_t x,uint8_t y,double data)
{
	int32_t integer,decimal;				//分别存整数和小数
	integer = (int32_t)data;	            //获取整数部分
	data -= integer;				        //去掉整数部分
	data*=100;				                //得到小数部分
	decimal = (int32_t)data;                //存起来
	wSDPara[x][y] = (integer<<8) + decimal;	 
}

//将整数数据放在缓存区中
static void pushSDInt32(uint8_t x,uint8_t y,int32_t data)
{
	wSDPara[x][y] = data; 
}

//读小数数据
static double readSDDouble(uint8_t x,uint8_t y)
{
	return (rSDPara[x][y]>>8) + ((double)(rSDPara[x][y]&0xFF))/100;
}

//读整数数据
static int32_t readSDInt32(uint8_t x,uint8_t y)
{
	return rSDPara[x][y];
}
