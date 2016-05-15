#ifndef __bsp_H
#define	__bsp_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"	 
#include <stdarg.h>
#include "math.h"
#include "sys.h"
#include "led.h"
#define LED0 PFout(9)
#define LED1 PFout(10)

#define MYID 0X10   //主控ID

//bsp.c
void NVIC_Configuration(void) ;

//delay.c
void delay_ms(unsigned int t);
void delay_us(unsigned int t);
void Init_System_Timer(void);
float GET_Nowtime(void)   ;    //返回当前systick计数器值,32位
float GET_microstime(uint32_t * lasttime) ;    //返回两次时间读取的差值

void LED_KEY_GPIO_Init(void);

//can1.c

void CAN1_Configuration(void);

//can2.c
void CAN2_Configuration(void);


//usart2.c
u8 keyb_filtering(u8 buf[],u8 x);
uint16_t Inwidth(uint16_t data,u8 m);   //去掉无效宽度
u8 Getbit(u8 data, u8 x);
void keyboard_analysis(void);
int16_t juduizhi(int16_t x);
void USART2_Configuration(void);



//usart3.c
void USART3_Configuration(void);
void USART3_SendChar(unsigned char b);
int fputc(int ch, FILE *f);


void BSP_Init(void);

#endif
