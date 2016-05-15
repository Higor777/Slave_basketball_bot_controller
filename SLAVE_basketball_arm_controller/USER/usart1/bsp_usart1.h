#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

void usart1_config(u32 baudrate);
void NVIC_Configuration(void);
void USART1_SendChar(u8 b); 
void Change_Baudrate(u32 baudrate); //修改串口波特率 
int fputc(int ch, FILE *f);
int fgetc(FILE *f);

#endif /* __USART1_H */
