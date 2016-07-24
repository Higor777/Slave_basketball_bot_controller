#ifndef __bsp_H
#define	__bsp_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"	 
#include <stdarg.h>
#include "math.h"
#include "sys.h"
#include "led.h"
#include "can.h"
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


void BSP_Init(void);

//uasrt4.c
void USART4_IMU_Init(uint32_t baud);
void USART4_Put_String(char *Str);
void USART4_SendChar(unsigned char b);
void loop(void);
#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)		) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1  ) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2  ) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3  ) )
void ANO_DT_Send_Status(float Klm_angle, float angleA, float imu_Angle);
struct SAcc
{
	double a[3];
	double T;
};
struct SGyro
{
	double w[3];
	double T;
};
struct SAngle
{
	double Angle[3];
	double T;
};
struct SMag
{
	short h[3];
	short T;
};


#endif
