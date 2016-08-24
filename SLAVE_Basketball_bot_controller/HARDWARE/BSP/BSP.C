#include "BSP.h"
#include "can.h"
#include "myextern.h"
#include "four_motor.h"
#include "usart1_IMU.h"
#include "can2_IMU.h"
extern System_flag System;   

	GPIO_InitTypeDef  gpio;
	EXTI_InitTypeDef  exti;
	NVIC_InitTypeDef  nvic;
	TIM_TimeBaseInitTypeDef   tim;
	TIM_OCInitTypeDef oc;
	
void RCC_Init(void)
{  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

}

void TIM6_Configuration(void)
{
	
	tim.TIM_Period = 9999;	  //10MS
	tim.TIM_Prescaler = 84-1;   // 84M/
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&tim);
	TIM_Cmd(TIM6, ENABLE);	 
	TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);	

}

/************主中断************/
void TIM6_DAC_IRQHandler(void)   //10ms
{
	static u8 cnt=0;
	//static u8 klm_cnt=1;
  //u8 i=0;
  if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{    
			static int16_t led_hz = 1;
      
			System.S_cnt1++;		
			if(	System.S_cnt1 >= (int16_t)(1000/(led_hz*10)))
			{
				System.S_cnt1=0;
				LED=!LED;
			}
	   
//			if(klm_cnt)
//			{
//				for(i=0;i<20;i++)loop();
//				klm_cnt=0;
//			}
//			
//			 loop();
//			
			
			if(cnt==0)
			{
				Chassis_control(); 
				cnt=1;
			}   
			else
		  {
				Encoder_analysis();
				cnt=0;
			}  
		  
			TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	}
}




void NVIC_Configuration(void) 
{
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	
  	
	  nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
	 
		nvic.NVIC_IRQChannel = USART3_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
	
		
		nvic.NVIC_IRQChannel = USART1_IRQn;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelSubPriority =0;	  
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);  
		
		nvic.NVIC_IRQChannel = UART4_IRQn;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelSubPriority =0;	  
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic); 
	
}


void BSP_Init(void)
{
		RCC_Init();		      
	

	  Init_System_Timer();      

    TIM6_Configuration();	   //10ms  主中断
	  LED_Init();					

	  USART3_Configuration();   //上位机通信
    CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_7tq,6,CAN_Mode_Normal);//can1初始化500k波特率
		CAN2_Mode_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,3,CAN_Mode_Normal);//can2初始化500k波特率
  	USART1_IMU_Init(115200);//陀螺仪232通信
	  //USART4_IMU_Init(115200);//磁力计，卡尔曼滤波
}


