#ifndef __MYNVIC_H
#define __MYNVIC_H

#include "common.h"

void MyNVIC_Init(uint32_t group,uint32_t PreempPriority,uint32_t SubPriority,uint8_t IRQCha,FunctionalState ENorDis);

#endif
