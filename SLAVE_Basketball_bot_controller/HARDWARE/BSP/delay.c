#include "BSP.h"

int16_t time;	 	  //用来做系统时钟

void delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=42000;
		while(a--);
	}
}

void delay_us(unsigned int t)
{
	
	int i;
	for( i=0;i<t;i++)
	{
		int a=40;
		while(a--);
	}
	
}


void Init_System_Timer(void)			   //系统时钟初始化  这里以tim4作为系统时钟
{	
	  
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //************		
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  //*********
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	TIM_DeInit(TIM4); 

	TIM_TimeBaseStructure.TIM_Period=65535;		 								/* 自动重装载寄存器周期的值(计数值) */
	TIM_TimeBaseStructure.TIM_Prescaler= (84 - 1);				    /* 时钟预分频数 72M/72 */
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);																		/* 开启时钟 */    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);		/*先关闭等待使用*/ 
	 
}

float GET_Nowtime(void)//返回当前systick计数器值,32位
{
	
	float temp; 
	temp=(float)(TIM4->CNT+65536*time)/1000000;	       //得到当前系统时钟的计数   时间=TIM5->CNT+65536*time/1000000			 1m的时钟
	return temp;
	
}


float GET_microstime(uint32_t * lasttime)//返回两次时间读取的差值
{
	int32_t temp1,temp2;
	float temp3;		
	temp1 =TIM4->CNT+65536*time;
	temp2 =temp1 - *lasttime;
	*lasttime = temp1;
	if(temp2<0)	return 0;
	temp3=temp2;
	return temp3;
}



void TIM4_IRQHandler(void)
{
	if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET ) 
	{	
	   
  	time++;
		if(time>=1000)
		{
		  time=0;
		}
	  TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 
	}		 	
}
