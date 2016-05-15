#include "BSP.h"



//TK:添加	:::
void usart_init(USART_TypeDef* USARTx,u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	if(USARTx==USART2){	 
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
		
		RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		 //USART2_TX   PA.2
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//USART2_RX	  PA.3
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	} 
	else if(USARTx==USART1){	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
		
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		  //USART1_TX   PA.9
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	    
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  //USART1_RX	  PA.10
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	    GPIO_Init(GPIOA, &GPIO_InitStructure);
	}
	else if(USARTx==USART3){	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		  //USART3_TX   PB.10
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	    GPIO_Init(GPIOB, &GPIO_InitStructure);
	    
	    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;		  //USART3_RX	  PB.11
	    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	    GPIO_Init(GPIOB, &GPIO_InitStructure);
	}
	 
	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USARTx, &USART_InitStructure);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);//开启中断
	USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	USART_Cmd(USARTx, ENABLE);                    //使能串口 

}


int fputc(int ch, FILE *f)
{
	/* 将Printf内容发往串口 */
	USART_SendData(USART2, (unsigned char) ch);
	while (!(USART2->SR & USART_FLAG_TXE));
	
	return (ch);
}



void Packet_Reply(USART_TypeDef* USARTx,unsigned char * data, unsigned short int length)
{
  //发送有效数据
	unsigned char j=0,check_sum=0;
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, 0xff);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, 0xff);
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, 0x04);
	check_sum=check_sum+0x04;
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, 0x69);
	check_sum=check_sum+0x69;
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, (unsigned char)(length>>8));
	check_sum=check_sum+(unsigned char)(length>>8);	
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, (unsigned char)(length&0xff));
	check_sum=check_sum+(unsigned char)(length&0xff);		
	 for (j = 0; j < length; j++) 
	 {
  	 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
		 USART_SendData(USARTx, *(data + j));
		 check_sum=check_sum+*(data + j);	
	 } 
	 while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
	USART_SendData(USARTx, check_sum);
}




