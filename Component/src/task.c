/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：task.c
 * 摘    要 ：定时器初始化
 * 函数接口：1.SysNormalWorkLedTask(void)			//系统正常运行指示
 *
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "task.h"

//系统正常运行指示
void SysNormalWorkLedTask(void)
{
	LedRedRev;
}

//按键控制OLED事件
void KeyControlOLED(void)
{
	static uint32_t indexDataNum = 0;
	float Data[7] = {0.0};
	
	KEY_MSG Key;
	
	Get_KeyMsg(&Key);
	
	switch(OledState)				
	{
		case OLED_INDEXDATA:								//索引事件
		{
			if(Key.status == KEY_DOWN)
			{
				if(Key.key == KEY_ONE)
				{
					indexData(ADD,indexDataNum);			//加页
				}
				else if(Key.key == KEY_TWO)
				{
					indexData(SUB,indexDataNum);			//减页
				}
			}
			else if(Key.status == KEY_HOLD)					//长按任意键退回储存界面
			{
				OLED_CLS();
				OledState = OLED_SAVEDATA;
			}
			break;
		}
		case OLED_SAVEDATA:								   //存数据事件
		{
			if(Key.status == KEY_DOWN || Key.status == KEY_HOLD)  //存储数据时,按任意按键进入数据索引
			{
				if(Key.key == KEY_ONE || Key.key == KEY_TWO)
				{
					OLED_CLS();
					OledState = OLED_INDEXDATA;
					OLED_Show_Sentence(0,0,"Index.......");
					indexCmd(&indexDataNum);
					OLED_Show_StrNum(0,2,"IndexTotal:",indexDataNum);
				}
			}
			break;
		}
		case OLED_SARTPAGE:			//起始页按任意键进入存数据
		{
			if(Key.status == KEY_DOWN || Key.status == KEY_HOLD)
			{
				if(Key.key == KEY_ONE || Key.key == KEY_TWO)
				{
					OLED_CLS();
					enableOneRootine(SaveDataTask);
					OledState = OLED_SAVEDATA;
					break;
				}
			}
		}
		case OLED_EXISTPREDATA:	   					//存在以前的数据
		{
			if(Key.status == KEY_DOWN || Key.status == KEY_HOLD)
			{
				if(Key.key == KEY_ONE)				//key1-->继续存
				{
					OLED_CLS();
					enableOneRootine(SaveDataTask);
					OledState = OLED_SAVEDATA;
					setSDDataToSys();			   //将数据加载到写缓冲区，如果不加载，会覆盖以前存的区域
					break;
				}
				else if(Key.key == KEY_TWO)			//key2-->从头存
				{
					OLED_CLS();
					enableOneRootine(SaveDataTask);
					OledState = OLED_SAVEDATA;
					page = 0;
					wi   = 0;
					paraSector = 1;					//注意：数据扇区是从1开始的
					dataCount  = 0;
					break;	
				}
			}
		}
	}
}

//存数据的任务
void SaveDataTask(void)
{
	static float i = 0;
	i++;
	saveSDPara(i);
}

