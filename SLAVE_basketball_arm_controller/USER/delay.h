#ifndef __DELAY_H
#define __DELAY_H 			   
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//LX-1A开发板		   
//青岛智芯科技
//电话：13012418100
//修改日期:2011/10/28 
//版权所有，盗版必究。
//********************************************************************************
//////////////////////////////////////////////////////////////////////////////////
extern  int16_t time; 
void delayinit(u8 SYSCLK);
void delayms(u16 nms);
void delayus(u32 nus);
void Init_System_Timer(void);
float GET_Nowtime(void)   ;    //返回当前systick计数器值,32位
float GET_microstime(uint32_t * lasttime) ;    //返回两次时间读取的差值


#endif

