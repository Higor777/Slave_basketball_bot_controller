#ifndef __USART3_H__
#define __USART3_H__

#include <stdio.h>

void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);
int fputc(int ch, FILE *f);

#endif
