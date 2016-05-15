#ifndef __usart2_h
#define __usart2_h
#include"stm32f10x.h"
#include <stdio.h>


void USART2_485init(void);	  //484(usart2)初始化
void USART_485_OUT(u8 c);
void Uart2_PutChar(u8 ch);
int fputc2(int ch, FILE *f);
void USART2_printf(USART_TypeDef* USARTx, uint8_t *Data,...);

//#define RS485_RX_MODE GPIO_ResetBits(GPIOA,GPIO_Pin_4);		  //a4 f11
//#define RS485_TX_MODE GPIO_SetBits(GPIOA,GPIO_Pin_4);
#define RS485_RX_MODE GPIO_ResetBits(GPIOG,GPIO_Pin_9);		  //战舰测试参数
#define RS485_TX_MODE GPIO_SetBits(GPIOG,GPIO_Pin_9);
#endif
