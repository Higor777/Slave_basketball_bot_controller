#include "BSP.h"
#include "usart1_IMU.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	 



/*-----USART1_TX-----PA9-----*/
/*-----USART1_RX-----PA10-----*/


u8 TF_clean_yaw=0;
float imu_Angle=0;
extern Robot_data  Ke ;
extern float imu_Angle_error;
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
		

}
enum imuRecstate								   		//状态机 
	{
		RECFF,RECLEN,SENDID,RECSEL,RECCHECK
	} imu_Recstate = RECFF;
static uint16_t imu_checksum;
uint8_t  imu_buffer[30] = {0};
int imu_cur=0;
uint8_t datalen1;
short data_len;

void USART1_IRQHandler(void)     
{
	u8 data;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
		{
			data =USART_ReceiveData(USART1);
			switch (imu_Recstate)		
				{
				case RECFF :
					if (data == 0x68)	  
						{	
							imu_Recstate =RECLEN;	
							imu_checksum=0;		
							imu_cur=0;
						}
			  break;
				case RECLEN :	
					imu_checksum += data;	
				  data_len  = data;
				  imu_Recstate=SENDID;
			  break;
				case SENDID :	
					if (data == 0x00)	  
						{	
							imu_Recstate =RECSEL;	
							imu_checksum+=data;		
						}	
					else
						{
							imu_Recstate=RECFF;
							imu_checksum=0;
						}
					break;
					case RECSEL :
					  imu_checksum += data;
					  imu_buffer[imu_cur++] = data;
						if(imu_cur >= data_len-3)
						{
							imu_Recstate = RECCHECK;
						}
					break;
					case RECCHECK :	
						imu_checksum=imu_checksum%256;
						if(data == imu_checksum)
							{				
								get_imudata();	
								imu_checksum=0;	
								imu_Recstate = RECFF;	 
							}
						else
						{
							imu_Recstate = RECFF;
							imu_checksum=0;
						}
						   
						break;
					 default:
							imu_Recstate = RECFF;						
			}
				USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		}
}

void get_imudata()
{
	
	if(imu_buffer[0]==0x84)
		imu_Angle=imu_buffer[7]%16*100+imu_buffer[8]/16*10+imu_buffer[8]%16+imu_buffer[9]/16*0.1f+imu_buffer[9]%16*0.01f;
	if(imu_buffer[0]==0x28) 
		TF_clean_yaw=1;           //清除飘移指令返回值	
}
void clean_imudate()         //清除飘移指令，不是清零指令
{
		u8 i=0;
	while(!TF_clean_yaw&&i<0x10)
	{
		i++;
	  USART1_SendChar(0x68);
		USART1_SendChar(0x04);
		USART1_SendChar(0x00);
		USART1_SendChar(0x28);
		USART1_SendChar(0x2c);
		delay_us(10);
	}
	TF_clean_yaw=0;
}


void USART1_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
		USART_SendData(USART1,b);
}
