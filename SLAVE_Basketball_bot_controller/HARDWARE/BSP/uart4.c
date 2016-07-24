#include "BSP.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	
#include "myextern.h"

/*-----USART4_TX-----PC10-----*/
/*-----USART4_RX-----PC11-----*/
extern float imu_Angle_correct;
float angleA_offset=0; 
void USART4_IMU_Init(uint32_t baud)
{
		USART_InitTypeDef uart4;
		GPIO_InitTypeDef  gpio;


		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); 

		gpio.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC,&gpio);

		uart4.USART_BaudRate =baud;
		uart4.USART_WordLength = USART_WordLength_8b;
		uart4.USART_StopBits = USART_StopBits_1;
		uart4.USART_Parity = USART_Parity_No;
		uart4.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
		uart4.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(UART4,&uart4);

		USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
		USART_Cmd(UART4,ENABLE);
		
		//ʹZ��Ƕȹ���
		delay_ms(10);
		USART4_SendChar(0xFF);
		USART4_SendChar(0xAA);
		USART4_SendChar(0x52);


}





struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag     stcMag;

unsigned char ucRxBuffer[25];
//CopeSerialDataΪ�����жϵ��ú���������ÿ�յ�һ�����ݣ�����һ�����������
void CopeSerialData(unsigned char ucData)
{
  static u8 Angle_cnt=1;
	static unsigned char ucRxCnt = 0;	
	short temp;
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
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
			 case 0x54:
				 memcpy(&stcMag,&ucRxBuffer[2],8);
			   if(Angle_cnt)
					 {
					 angleA_offset=atan2(stcMag.h[0]-Xoffset,stcMag.h[1]-Yoffset);
					 Angle_cnt=0;
				 }// ���³�ʼֵ
			 break;
			default : break;
		}
		ucRxCnt=0;
	}
}

void UART4_IRQHandler(void)                	//UART4�жϷ������
{

					if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
					{
					  CopeSerialData(UART4->DR);
						USART_ClearITPendingBit(UART4, USART_IT_RXNE);
					}
					USART_ClearITPendingBit(UART4,USART_IT_ORE);

} 


void USART4_Put_String(char *Str)
{
	//�ж�Strָ��������Ƿ���Ч.
	while(*Str){
	//�Ƿ��ǻس��ַ� �����,������Ӧ�Ļس� 0x0d 0x0a
	if(*Str=='\r'){
		USART_SendData(UART4, 0X0d);
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
		}
		else if(*Str=='\n'){
			USART_SendData(UART4, 0X0a);
			while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
			}
			else {USART_SendData(UART4,*Str);
				while(USART_GetFlagStatus(UART4,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
				}
	Str++;
	}
}


void USART4_SendChar(unsigned char b)
{
		while (USART_GetFlagStatus(UART4,USART_FLAG_TC) == RESET);
		USART_SendData(UART4,b);
}


float dt=0.01;
float bias = 0;
float P_00=0,P_01=0,P_10=0,P_11=0;
float K_0=0,K_1=0;
float angleA=0;             // ���ݴ����ƻ�ýǶ�

float  Klm_angle=0;
float omega=0;               //���������ǵý��ٶ� 

float angleA_correct=0;
/*
������У׼


1��ˮƽ������ת���ռ�XY������
2��ת������90�ȣ�Z�ᣩ����ת�����ռ�Z������
Xoffset = ��Xmax + Xmin��/2
Yoffset = ��Ymax + Ymin��/2
Zoffset = ��Zmax + Zmin��/2
n 
�������ƶ�������ֵ��ȥoffset���õ������Ƕȼ����Headingֵ
XH = X - Xoffset
YH = Y - Yoffset
ZH = Z - Zoffset
*/

void loop(void) 
{  
	 static float last_imu_Angle_correct=0;
	  angleA_correct=atan2(stcMag.h[0]-Xoffset,stcMag.h[1]-Yoffset);
	 angleA=(atan2(stcMag.h[0]-Xoffset,stcMag.h[1]-Yoffset)-angleA_offset) * 180.0 / pi;          // ���ݴ����ƻ�ýǶ�
	 
	 omega=(imu_Angle_correct-last_imu_Angle_correct)/dt;       //���������ǵý��ٶ�
	 last_imu_Angle_correct=imu_Angle_correct;
	 Klm_angle += (omega - bias) * dt;          // �������
	
	 P_00 += -(P_10 + P_01) * dt + Q_angle *dt;
	 P_01 += -P_11 * dt;
	 P_10 += -P_11 * dt;
	 P_11 += Q_omega * dt;                     // ����������Э����
	 
	 K_0 = P_00 / (P_00 + R_angle);
   K_1 = P_10 / (P_00 + R_angle);
	 
	bias += K_1 * (angleA - Klm_angle);
	Klm_angle += K_0 * (angleA - Klm_angle);   // ����   ����
	P_00 -= K_0 * P_00;
	P_01 -= K_0 * P_01;
	P_10 -= K_1 * P_00;
	P_11 -= K_1 * P_01;                        // ����������Э����

}
void ANO_DT_Send_Data(u8 fun,u8 *data_to_send,u8 len)
{
	
	u8 send_buf[50];
	u8 i;
    if(len>45)return;	//���28�ֽ����� 
    send_buf[len+4]=0;	//У��������
    send_buf[0]=0xAA;	//֡ͷ
	send_buf[1]=0xAA;
	send_buf[2]=fun;	//������
	send_buf[3]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[4+i]=data_to_send[i];			//��������
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//����У���	
	for(i=0;i<len+5;i++)USART4_SendChar(send_buf[i]);	//�������ݵ�����1 
	
}
/*������ʾ��ANO_TC�����ƴ�����վv4-20151027.exe */
void ANO_DT_Send_Status(float Klm_angle, float angleA, float imu_Angle)
{
	u8 _cnt=0;
	vs16 _temp;
	u8 data_to_send[50] ;
	_temp = (int)(Klm_angle*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = (int)(angleA*100);
	data_to_send[_cnt++]=BYTE1(_temp); 
	data_to_send[_cnt++]=BYTE0(_temp);
	
	_temp = (int)(imu_Angle*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	ANO_DT_Send_Data(0xF1,data_to_send,_cnt);

}
