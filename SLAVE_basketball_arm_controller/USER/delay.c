#include "stm32f10x.h"
#include "delay.h"


int16_t time;	 	  //������ϵͳʱ��
static u8  fac_us=0;//us��ʱ������   
static u16 fac_ms=0;//ms��ʱ������


//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
void delayinit(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;//bit2���,ѡ���ⲿʱ��  HCLK/8
	fac_us=SYSCLK/8;		    
	fac_ms=(u16)fac_us*1000;
}								    


//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//��72M������,nms<=1864 
void delayms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
}   

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delayus(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}




// void Init_System_Timer(void)			   //ϵͳʱ�ӳ�ʼ��  ������tim5��Ϊϵͳʱ��
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
// 	TIM_TimeBaseStructure.TIM_Period=65535;		 								/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
// 	TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    /* ʱ��Ԥ��Ƶ�� 72M/72 */
// 	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* ������Ƶ */
// 	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* ���ϼ���ģʽ */
// 	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
// 	
// 	TIM_ClearFlag(TIM5, TIM_FLAG_Update);							    		/* �������жϱ�־ */
// 	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
// 	TIM_Cmd(TIM5, ENABLE);																		/* ����ʱ�� */
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);		/*ʹ��*/    
// }

// float GET_Nowtime(void)//���ص�ǰsystick������ֵ,32λ
// {
// 	float temp; 
// 	temp=(TIM5->CNT+65536*time)/1000000;	       //�õ���ǰϵͳʱ�ӵļ���   ʱ��=TIM5->CNT+65536*time/1000000			 1m��ʱ��
// 	return temp;
// }


// float GET_microstime(uint32_t * lasttime)//��������ʱ���ȡ�Ĳ�ֵ
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
