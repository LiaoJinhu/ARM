#ifndef __LED_H
#define __LED_H

#include "common.h"
#include "key.h"

//不需要修改.c文件里的结构，只需要修改此处即可
#define LedRedPeri  RCC_APB2Periph_GPIOA
#define LedBluePeri RCC_APB2Periph_GPIOD
#define LedRedGPIO  GPIOA
#define LedBlueGPIO GPIOD
#define LedRedPin   GPIO_Pin_8
#define LedBluePin  GPIO_Pin_2

#define LedRedOff   LedRedGPIO->BSRR  = LedRedPin
#define LedRedOn    LedRedGPIO->BRR   = LedRedPin
#define LedRedRev   LedRedGPIO->ODR  ^= LedRedPin
#define LedBlueOff  LedBlueGPIO->BSRR = LedBluePin
#define LedBlueOn   LedBlueGPIO->BRR  = LedBluePin
#define LedBlueRev  LedBlueGPIO->ODR ^= LedBluePin

void LEDGPIO_Init(void);				//LED初始化

#endif
