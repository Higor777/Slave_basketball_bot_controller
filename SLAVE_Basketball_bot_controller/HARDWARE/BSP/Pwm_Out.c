#include "BSP.h"



u16 PWMEN1 = 0;             /* PWM信号电平跳变值 */
u16 PWMEN2 = 0;
u16 CCR_Val1=0;
u16 CCR_Val2=0;

void PWM_Mode_Config(void)
{
	

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure; 
	
	 /******************************TIM1复用推挽输出*********************************************/
	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;		    
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 5000;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse =PWMEN1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);																 
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	
	TIM_OCInitStructure.TIM_Pulse = PWMEN2;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);	
	TIM_Cmd(TIM1, ENABLE);	
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
   
}


