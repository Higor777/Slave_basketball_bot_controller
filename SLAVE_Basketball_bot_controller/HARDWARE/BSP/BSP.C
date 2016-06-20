#include "BSP.h"
#include "can.h"
#include "myextern.h"
#include "four_motor.h"
#include "usart1_IMU.h"
uint32_t    While1_Lasttime=0;    //´æ´¢ÉÏÒ»´Îtim5µÄ¼ÆÊý
float	NOW_TIME ,IMU_CYCTIME1,IMU_CYCTIME2;	
extern System_flag System;   // ÏµÍ³¸÷±êÖ¾

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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

}

void TIM6_Configuration(void)
{
	
	tim.TIM_Period = 9999;	  //10MS
	tim.TIM_Prescaler = 84-1;   // Ê±ÖÓÔ¤·ÖÆµÊý 84M
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM6,&tim);
	TIM_Cmd(TIM6, ENABLE);	 
	TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);	

}

/************Ö÷ÖÐ¶Ï************/
void TIM6_DAC_IRQHandler(void)   //10ms
{
	static u8 cnt=0;
  if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{    
			static int16_t led_hz = 1;
      
			System.S_cnt1++;		
			if(	System.S_cnt1 >= (int16_t)(1000/(led_hz*10)))
			{
				System.S_cnt1=0;
				LED=!LED;
			}
	    
			IMU_CYCTIME1 = GET_microstime(&While1_Lasttime);		//¸üÐÂÁ½´Î¶ÁÈ¡Ê±¼ä¼ä¸ô ´ó¸Å20ms  2¸öÖÐ¶ÏÊ±¼ä
			if(cnt==0)
			{
			Chassis_control(); 
			cnt=1;}   //µ×ÅÌ¿ØÖÆ
			else
		  {
			Encoder_analysis();
			cnt=0;}  //±àÂëÆ÷Êý¾Ý·ÖÎö  ½âËã×ø±ê
		  
			
			IMU_CYCTIME2 = GET_microstime(&While1_Lasttime);		//¸üÐÂÁ½´Î¶ÁÈ¡Ê±¼ä¼ä¸ô		
		
			TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	}
}




void NVIC_Configuration(void) 
{
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	
  	/*¿ªÆôTIM6Ö÷ÖÐ¶Ï*/  
	  nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
	  /*¿ªÆôUSART3½ÓÊÕÖÐ¶Ï*/  
		nvic.NVIC_IRQChannel = USART3_IRQn;
		nvic.NVIC_IRQChannelPreemptionPriority = 0;
		nvic.NVIC_IRQChannelSubPriority = 1;
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);
	
		/*¿ªÆôUSART1½ÓÊÕÖÐ¶Ï*/  
		nvic.NVIC_IRQChannel = USART1_IRQn;
		nvic.NVIC_IRQChannelSubPriority = 0;
		nvic.NVIC_IRQChannelSubPriority =3;	   //ÖÐ¶ÏÏìÓ¦ÓÅÏÈ¼¶
		nvic.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&nvic);  
	
}


void BSP_Init(void)
{
/*************Ê±ÖÓÊ¹ÄÜ********************/
		RCC_Init();		      
	
/************ÏµÍ³Ê±ÖÓ³õÊ¼»¯  ÕâÀïÒÔtim4×÷ÎªÏµÍ³Ê±ÖÓ*******************/
	  Init_System_Timer();      
/*************Ó²¼þÉèÖÃ³õÊ¼»¯********************/
    TIM6_Configuration();	   //10ms  TIM6Ö÷ÖÐ¶ÏÉèÖÃ		
	  LED_Init();					//³õÊ¼»¯LED 
/*************Í¨ÐÅ³õÊ¼»¯*******************/
		USART3_Configuration();   //ÉÏÎ»»úÍ¨ÐÅ
    CAN1_Mode_Init(CAN_SJW_1tq,CAN_BS1_6tq,CAN_BS2_7tq,6,CAN_Mode_Normal);
  	USART1_IMU_Init(115200);
}


