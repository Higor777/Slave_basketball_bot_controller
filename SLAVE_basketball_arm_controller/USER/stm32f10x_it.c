#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
#include "math.h"
#include "string.h"  
#include "stdio.h"
#include "queue.h"
#include "usart2.h"
#include "motorcontrol.h"
#include "delay.h"


extern Queue rx_queue;	 //485接收缓存
extern int step,flag,hold;		        //步进电机步数
u16 test=1;
extern int hehe;	
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{  static int count=0;
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			count++;
			test++;
			if (count>step)
			{
			count=0;
			step=0;
			flag=1;
			if(hold==0)
			{
			TIM_Cmd(TIM3, DISABLE);
			TIM_Cmd(TIM8, DISABLE);
}		
}
		}
}


//void USART2_IRQHandler(void) 
//{
//    u8 c;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
//	{ 
//    test++;
//		if(queue_full(rx_queue))
//		{		   		   
//			USART_485_OUT(0xff);
//			USART_485_OUT(0xff);
//	       //USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);	//关闭串口2	 	
//		}
//		else
//		{
//		    c = USART_ReceiveData(USART2);
//			queue_put(&rx_queue, c);
//	    }
//				  
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//	} 			 
// GPIO_ResetBits(GPIOC,GPIO_Pin_10);
// GPIO_ResetBits(GPIOC,GPIO_Pin_11);
// GPIO_ResetBits(GPIOC,GPIO_Pin_12);   
//}
                                                                                              


 
void TIM7_IRQHandler(void) 							   
{ static int flag_hold=1;

	if(TIM_GetITStatus(TIM7, TIM_IT_Update)== SET)	   //检测是否发生溢出更新事件
	{
		//test=-test;
      flag_hold=-flag_hold;
      if(hold==1)
		  direction(flag_hold);
	    hehe=1;
		TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);

	}
}


