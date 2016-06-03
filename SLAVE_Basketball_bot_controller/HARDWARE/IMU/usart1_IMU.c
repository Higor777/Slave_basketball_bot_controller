#include "BSP.h"
#include "usart1_IMU.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 



/*-----USART1_TX-----PA9-----*/
/*-----USART1_RX-----PA10-----*/
float Yaw_Angle=0;
void USART1_IMU_Init(uint32_t baud)
{
		USART_InitTypeDef usart1;
		GPIO_InitTypeDef  gpio;


		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 

		gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);

		usart1.USART_BaudRate =baud;
		usart1.USART_WordLength = USART_WordLength_8b;
		usart1.USART_StopBits = USART_StopBits_1;
		usart1.USART_Parity = USART_Parity_No;
		usart1.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART1,&usart1);

		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		USART_Cmd(USART1,ENABLE);
		
		//使Z轴角度归零
		delay_ms(10);
		USART1_SendChar(0xFF);
		USART1_SendChar(0xAA);
		USART1_SendChar(0x52);
//		delay_ms(10);
//		USART1_SendChar(0xFF);
//		USART1_SendChar(0xAA);
//		USART1_SendChar(0x52);

}





struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;


unsigned char ucRxBuffer[250];
//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerialData(unsigned char ucData)
{

	static unsigned char ucRxCnt = 0;	
	short temp;
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x51:	
				memcpy(&temp,&ucRxBuffer[2],2);
				stcAcc.a[0] = (double)temp/32768.0*16.0;
				memcpy(&temp,&ucRxBuffer[4],2);
				stcAcc.a[1] = (double)temp/32768.0*16.0;
				memcpy(&temp,&ucRxBuffer[6],2);
				stcAcc.a[2] = (double)temp/32768.0*16.0;
				memcpy(&temp,&ucRxBuffer[8],2);
				stcAcc.T = (double)temp/340.0+36.25;
				break;
			case 0x52:	
				memcpy(&temp,&ucRxBuffer[2],2);
				stcGyro.w[0] = (double)temp/32768.0*2000.0;
				memcpy(&temp,&ucRxBuffer[4],2);
				stcGyro.w[1] = (double)temp/32768.0*2000.0;
				memcpy(&temp,&ucRxBuffer[6],2);
				stcGyro.w[2] = (double)temp/32768.0*2000.0;
				memcpy(&temp,&ucRxBuffer[8],2);
				stcGyro.T = (double)temp/340.0+36.25;
				break;
			case 0x53:	
				memcpy(&temp,&ucRxBuffer[2],2);
				stcAngle.Angle[0] = (double)temp/32768.0*pi;//180.0;
				memcpy(&temp,&ucRxBuffer[4],2);
				stcAngle.Angle[1] = (double)temp/32768.0*pi;//180.0;
				memcpy(&temp,&ucRxBuffer[6],2);
				stcAngle.Angle[2] = (double)temp/32768.0*pi;//180.0;
				memcpy(&temp,&ucRxBuffer[8],2);
				stcAngle.T = (double)temp/340.0+36.25;
				break;
			default : break;
		}
		ucRxCnt=0;
	}
}
void Dajiang_CopeSerialData(unsigned char ucData)
{ 
  static char first_angle=1;
	static unsigned char ucRxCnt = 0;	
	short temp;
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0xAA) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<6) {return;}//数据不满6个，则返回
	else
		{
			if (ucRxBuffer[1]==0xAA)
			{
				memcpy(&Yaw_Angle,&ucRxBuffer[2],4);
			}
			
			ucRxCnt=0;
		}
	}
u8 TxBuffer[256];
u8 TxCounter=0;
u8 count=0; 

void USART1_IRQHandler(void)                	//UART1中断服务程序
{
//	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  
//	{
//						USART_SendData(USART1, TxBuffer[TxCounter++]); 
//						if(TxCounter == count) 
//						{
//							USART_ITConfig(USART1, USART_IT_TXE, DISABLE);// 全部发送完成
//						}
//						USART_ClearITPendingBit(USART1, USART_IT_TXE); 
//					}
//					else
					if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
					{
						Dajiang_CopeSerialData((unsigned char)USART1->DR);//处理数据
						USART_ClearITPendingBit(USART1, USART_IT_RXNE);
					}
					USART_ClearITPendingBit(USART1,USART_IT_ORE);
  

} 





void USART1_Put_String(char *Str)
{
	//判断Str指向的数据是否有效.
	while(*Str){
	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
	if(*Str=='\r'){
		USART_SendData(USART1, 0X0d);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
		}
		else if(*Str=='\n'){
			USART_SendData(USART1, 0X0a);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
			}
			else {USART_SendData(USART1,*Str);
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				}
	Str++;
	}
}


void USART1_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		USART_SendData(USART1,b);
}

