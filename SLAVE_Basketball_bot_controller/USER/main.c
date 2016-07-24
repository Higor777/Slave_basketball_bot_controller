#include "sys.h"
#include "bsp.h"
#include "queue.h"
#include "myextern.h"
#include "usart3.h"
#include "four_motor.h"
#include "usart1_IMU.h"
/********************************************************************
篮球底盘万向轮以顺时针旋转为正方向
轮子序号为逆时针
********************************************************************/

#define MAX_DATA_LEN 100   //数据接收缓冲区大小
volatile Queue rx_queue;
uint8_t radBufferRS485[99]={0};  		 //记录转角
int noselect;		  	//命令模式选择标记
Robot_data  Ke ;
System_flag System;
extern float imu_Angle_error;
extern float imu_Angle;
/***************数据限制**************/
float xianfu(float v, float x);
/***************清零指令**************/
void clear(void);
/***************速度赋值**************/
void start_dc(void);
/*******赋值机器人坐标系速度**********/
void start_dc_local(void);
/***************动作分类**************/
void handle_data(uint8_t buf[], int len);
/***************数据返回**************/
void send(void);

int main(void)
{ 
	static uint16_t checksum;
	uint8_t datalen1;
	short data_len;
	int cur=0;
	uint8_t data;
	uint8_t  buffer[MAX_DATA_LEN] = {0};
	enum Recstate								   		//状态机 
	{
		RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
	} rs485Recstate = RECFF1;
	//初始化
	System.S_Initialize=0;     //系统状态  0  系统没初始化   |1   系统初始化成功
	BSP_Init();   
 	NVIC_Configuration();  
	queue_init(&rx_queue);
  System.Control_Moder=2;    // 控制模式  2 3 使能电机控制
	System.S_Initialize= 1;

	while(1)
	{
		if(queue_empty(rx_queue))
							continue;

		data = (uint8_t)queue_get(&rx_queue);
  
		switch (rs485Recstate) 
		{			case RECFF1:
				if (data == 0xff)	  
				{	
					rs485Recstate = RECFF2;	
					checksum=0;		
					cur = 0;								//校验位清零
				}
	
				break;
	
			case RECFF2:
				if (data == 0xff)
					rs485Recstate = RECID;
				else
					rs485Recstate = RECFF1;
				break;
	
	
			case RECID:				 					
			  if(data==0x01)	       //设备编号0x01底盘
			    {
					checksum += data;
					rs485Recstate = RECLEN1;
				  }
				else
					rs485Recstate = RECFF1;	  
				break;
	
			case RECLEN1:				 				
				checksum += data;
				datalen1 = data;
				rs485Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				checksum += data;			 				
				data_len = (short)datalen1 << 8	 | data;
				rs485Recstate = RECSEL;	  
				break;
	
			case RECSEL:
				checksum += data;
				buffer[cur++] = data;
				if(cur >= data_len)
					rs485Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				checksum=checksum%255;
				if(data == checksum)
				{				
					handle_data(buffer, data_len);
  				send();				
					checksum=0;	
					rs485Recstate = RECFF1;	 
				}
				else
					rs485Recstate = RECFF1;
				break;
			 default:
					rs485Recstate = RECFF1;
		}	
	}
}


float xianfu(float v, float x)
{
   if(v>x)
	 {
     return x;
   }	 
   else if(v<-x)
	 {
     return -x;
   }
	 else return v;

}

void clear(void)			  				
{
	
	Ke.Robot.x = 0 ; 
	Ke.Robot.y = 0 ; 
	Ke.Robot.z = 0 ; 
	Ke.Total_angle.M1 = 0 ; 
	Ke.Total_angle.M2 = 0 ; 
	Ke.Total_angle.M3 = 0 ;
	Ke.Total_distance.M1 =0 ; 	
	Ke.Total_distance.M2 =0 ;
	Ke.Total_distance.M3 =0 ;
	Can_get_encoder(0x01,0x01);
	delay_us(300);
	Can_get_encoder(0x03,0x01);
	delay_us(300);
	Can_get_encoder(0x02,0x01);
	delay_us(300);
	clean_imudate();  //清除飘移指令
	imu_Angle_error=imu_Angle;//记下当前陀罗角度值
	

} 
void start_dc(void)	
{	

		/*********************赋值机器人速度*******************/
		if(noselect==0x01)
		{

		System.Control_Moder = 2;    //添加contro_mode,避免和local控制干扰

    memcpy(&Ke.E_Global_Speed  , radBufferRS485 , 12);	  //

	  Ke.E_Global_Speed.x=xianfu(Ke.E_Global_Speed.x, 0.8);  //防止速度过大，通信很难说的。
	  Ke.E_Global_Speed.y=xianfu(Ke.E_Global_Speed.y, 0.8);
		Ke.E_Global_Speed.z=xianfu(Ke.E_Global_Speed.z, 0.8);
		}
}
void start_dc_local(void)
{
	
		if(noselect==0x05)   
		{
			System.Control_Moder = 3;    //添加contro_mode,避免和local控制干扰
    memcpy(&Ke.Remote_Speed  , radBufferRS485 , 12);	  //将三个电机速度存储到radBufferRS485

	  Ke.Remote_Speed.x=xianfu(Ke.Remote_Speed.x, 0.8);  //防止速度过大，通信很难说的。
	  Ke.Remote_Speed.y=xianfu(Ke.Remote_Speed.y, 0.8);
		Ke.Remote_Speed.z=xianfu(Ke.Remote_Speed.z, 0.8);
		}
}
void handle_data(uint8_t buf[], int len)
{
	  int i=0;	
	
		switch(buf[0])
		{	
			case 1: noselect = 0x01 ;break;		   //机器人世界坐标系速度	
			case 2: noselect = 0x02 ;break;		   //读取三个电机的角速度
			case 3: noselect = 0x03 ;break;	     //重置编码器数据		 
      case 4: noselect = 0x04 ;break;	     //读取x,y,w坐标
      case 5: noselect = 0x05 ;break;      //控制机器人坐标系速度			
			default:break;
		}
		if(noselect == 0x01 )			             //机器人世界坐标系速度信息转移到radBufferRS485[i]
		{						
			for(i=0;i< 4 * Motor_Num ; i++)												
			{
				radBufferRS485[i] = buf[i+1] ;							
			}			
		}
		if(noselect == 0x05 )			             //电机速度信息转移到radBufferRS485[i]
		{						
			for(i=0;i< 4 * Motor_Num ; i++)												
			{
				radBufferRS485[i] = buf[i+1] ;							
			}			
		}
}
void send(void)
{

	int i;
	char byte_s[4*Motor_Num],byte_Coord[4*3];	
  uint16_t checksumsend;				 //之前是	 uint16_t   这个有问题之前怎么会用这个
	
	if(USART_GetFlagStatus(USART3, USART_FLAG_TC)!=RESET)
	{	

	 if(noselect==0x01) 				 //机器人世界坐标系速度			 
		{
			
			System.Signal_Update = 1 ;    //控制信号更新
			start_dc();										
			noselect=0;
	 	}

	 	if(noselect == 0x02)			 // 读取角速度 
		{	
			USART3_SendChar(0xff);
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);									 										   
			USART3_SendChar(0x00);											   		
			USART3_SendChar(0x0d);											   
			USART3_SendChar(0x02);
			
			checksumsend = 0x10;      //以上数据校验位
			
		  memcpy( byte_s , &Ke.local_speed , 4*Motor_Num );		   
			for(i=0 ;i < 4 * Motor_Num ; i++)
			{
				USART3_SendChar( byte_s[i] );
			  checksumsend = checksumsend + byte_s[i];
			}
				
			checksumsend = checksumsend%255 ;
			USART3_SendChar(checksumsend) ;
			
			noselect=0;			  					
	 	}

	   if(noselect==0x03)			   //重置编码器的数据
		{	
		 
			clear();		
			USART3_SendChar(0xff);		
			USART3_SendChar(0xff);			
			USART3_SendChar(0x01);     //底盘控制
			USART3_SendChar(0x00);		 //两位 表示数据长度为2
			USART3_SendChar(0x02);	   
			USART3_SendChar(0x03);     //表示3号动作
			USART3_SendChar(0x00);		 //表示动作执行成功	
			USART3_SendChar(0x06);
			noselect=0;
		}

		if(noselect == 0x04)		   //  返回FLOAT 的X Y W坐标
		{
		
			USART3_SendChar(0xff);					
			USART3_SendChar(0xff);

			USART3_SendChar(0x01);		 //底盘控制							
			USART3_SendChar(0x00);				
			USART3_SendChar(0x0d);		 //两位 表示数据长度为13
			USART3_SendChar(0x04);     //表示4号动作

			checksumsend = 0x12;

			memcpy( byte_Coord , &Ke.Robot , 4*3 );

			for(i=0 ; i < 4 * 3 ; i++)
			{
				USART3_SendChar( byte_Coord[i] );
				checksumsend=checksumsend + byte_Coord[i] ;
			}		 
	
			checksumsend =checksumsend % (0xff);
		
			USART3_SendChar(checksumsend) ;						
			noselect=0;		  					
	 	}
	 	if(noselect == 0x05){
	 		System.Signal_Update = 1 ;    //控制信号更新
			start_dc_local();										
			noselect=0;
	 	}
  }
   while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);				
	
}
