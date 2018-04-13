/**
 * CopyRight (c) 2016,武汉理工大学ATGroup
 * All rights reserved
 *
 * 文件名称：MyNVIC.c
 * 摘    要 ：中断配置
 * 函数接口：1.MyNVIC_Init(uint32_t group,uint32_t PreempPriority,uint32_t SubPriority,uint8_t IRQCha,FunctionalState ENorDis)
 *			  
 * 说     明：设置中断，一般在外设全部初始化完之后进行所以中断的配置
 * 当前版本：1.0
 * 作    者 ：廖金虎
 * 完成日期：2016年10月11日
 */
#include "MyNVIC.h"

/*
        @arg NVIC_PriorityGroup_0: 0 bits for pre-emption priority
  *                                4 bits for subpriority
  *     @arg NVIC_PriorityGroup_1: 1 bits for pre-emption priority	  2
  *                                3 bits for subpriority
  *     @arg NVIC_PriorityGroup_2: 2 bits for pre-emption priority	  4
  *                                2 bits for subpriority
  *     @arg NVIC_PriorityGroup_3: 3 bits for pre-emption priority	  8
  *                                1 bits for subpriority
  *     @arg NVIC_PriorityGroup_4: 4 bits for pre-emption priority	  16
  *                                0 bits for subpriority
  ENorDis: ENABLE
           DISABLE
  
*/
void MyNVIC_Init(uint32_t group,uint32_t PreempPriority,uint32_t SubPriority,uint8_t IRQCha,FunctionalState ENorDis)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	
    NVIC_PriorityGroupConfig(group);
    NVIC_InitStructure.NVIC_IRQChannel = IRQCha;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreempPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENorDis;
    NVIC_Init(&NVIC_InitStructure);
}
