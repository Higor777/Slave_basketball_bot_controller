#ifndef __uart1_IMU_h
#define __uart1_IMU_h
#include "myextern.h"


void USART1_IMU_Init(uint32_t baud);
void USART1_IRQHandler(void) ;  
void USART1_SendChar(unsigned char b);
void get_imudata(void);
void clean_imudate(void);

#endif
