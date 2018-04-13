#ifndef __COMMON_H
#define __COMMON_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stdio.h"

#define EnableInterrupts   __enable_irq()     //开总中断
#define DisableInterrupts  __disable_irq()    //关总中断

#define ABS(x) ((x)>0?(x):-(x))
#endif
