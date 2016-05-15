#include "stm32f10x.h"
#include "delay.h"


int16_t time;	 	  //用来做系统时钟
static u8  fac_us=0;//us延时倍乘数   
static u16 fac_ms=0;//ms延时倍乘数


//SYSTICK的时钟固定为HCLK时钟的1/8
void delayinit(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2清空,选择外部时钟  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    


//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//对72M条件下,nms<=1864 
void delayms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
}   

//延时nus
//nus为要延时的us数.		    								   
void delayus(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}




// void Init_System_Timer(void)			   //系统时钟初始化  这里以tim5作为系统时钟
// {	
// 	  
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
// 	    
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //************		
// 	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	 
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	  //*********
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
// 	NVIC_Init(&NVIC_InitStructure);
// 	
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
// 	TIM_DeInit(TIM5);                                                                               
// 	TIM_TimeBaseStructure.TIM_Period=65535;		 								/* 自动重装载寄存器周期的值(计数值) */
// 	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* 时钟预分频数 72M/72 */
// 	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
// 	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
// 	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
// 	
// 	TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
// 	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
// 	TIM_Cmd(TIM5, ENABLE);																		/* 开启时钟 */
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);		/*使用*/    
// }

// float GET_Nowtime(void)//返回当前systick计数器值,32位
// {
// 	float temp; 
// 	temp=(TIM5->CNT+65536*time)/1000000;	       //得到当前系统时钟的计数   时间=TIM5->CNT+65536*time/1000000			 1m的时钟
// 	return temp;
// }


// float GET_microstime(uint32_t * lasttime)//返回两次时间读取的差值
// {
// 	int32_t temp1,temp2;
// 	float temp3;		
// 	temp1 =TIM5->CNT+65536*time;
// 	temp2 =temp1 - *lasttime;
// 	*lasttime = temp1;

// 	if(temp2<0)
// 	{
//       return  ((65556*1000 -*lasttime)+temp2);
//   }	
// 	temp3=temp2;
// 	return temp3;
// }



// void TIM5_IRQHandler(void)
// {
// 	if ( TIM_GetITStatus(TIM5 , TIM_IT_Update) != RESET ) 
// 	{	
// 	   
//   		time++;
// 		if(time>=1000)
// 		{
// 		  time=0;
// 		}
// 	  TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update); 
// 	}		 	
// }
