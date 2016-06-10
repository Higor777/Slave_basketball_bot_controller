#include "bsp.h"
#include "four_motor.h"
#include "myextern.h"
#include "TF.h"
#include "Tracks_GUI.h"
#include "stm32f4xx_can.h"
#include "can.h"
#include "usart1_IMU.h"
extern Robot_data  Ke ;
extern System_flag System;   // 系统各标志
extern struct SAngle 	stcAngle;
extern float FITST_angle_erroer;
extern float Yaw_Angle;
//进制转换函数，将10进制转换为16进制，形参N为待转换值，转换完后的值放入指针h对应的数组中
void itoh(int N, uint8_t *h)   
{
    uint8_t a[10]={0};
    int count=0;
    while(N != 0)
    {
       a[count++]=N&0xf;
       N>>=4;
			if(count==8) break;
    }
		
    *h=(a[1]<<4)|a[0];
    *(h+1)=(a[3]<<4)|a[2];
    *(h+2)=(a[5]<<4)|a[4];
    *(h+3)=(a[7]<<4)|a[6];
}
/****************底盘控制函数****************/
void Chassis_control(void)            
{ 	 
	  static u8 Motor_cnt = 0;
	  static char motor_cnt=1;
	  motor_cnt=!motor_cnt;
	  Motor_cnt++;
		if(System.Control_Moder == 0)   //停止
		{
				Ke.E_Speed.M1=0 ;
				Ke.E_Speed.M2=0 ;
				Ke.E_Speed.M3=0 ;

    }
		else if(System.Control_Moder == 1)  //遥控器控制模式
		{

				
			Robot_Speed_Set(Ke.Remote_Speed.x , Ke.Remote_Speed.y , Ke.Remote_Speed.z);
		

    }
		else if(System.Control_Moder == 2)  //上位机控制，一般来说都是进的这个控制模式
		{
			
      Global_Speed_Set(Ke.E_Global_Speed.x ,Ke.E_Global_Speed.y ,Ke.E_Global_Speed.z);  //设置全局速度

    }
			
		else if(System.Control_Moder == 3)//机器人坐标系控制
		{
			Robot_Speed_Set(Ke.Remote_Speed.x , Ke.Remote_Speed.y , Ke.Remote_Speed.z);
		}
	  if(System.Control_Moder ==1 || System.Control_Moder == 2 || System.Control_Moder ==3 )               //使能控制
		{		
			if(motor_cnt)
				{
					Can_send_motorx( Ke.E_Speed.M3 ,0x30);//驱动盒3的ID：0x30
					delay_us(400);
			    Can_send_motorx( Ke.E_Speed.M2 ,0x32);//驱动盒2的ID：0x32
				}
				else
				{
					Can_send_motorx( Ke.E_Speed.M1 ,0x36);//驱动盒1的ID：0x36
				}
	   

    }
	  else              //失能控制
		{
			Can_send_motorx( 0,0x36);
			delay_us(200);
			Can_send_motorx( 0,0x30);
			delay_us(200);
			Can_send_motorx( 0,0x32);
			delay_us(200);	
    }
}


/*************************************************
函数  void Encoder_analysis(void) 
编码器的数据分析
解算出x，y，w,给上位机
**************************************************/
float Len_M[3]={0,0,0};	   	           //每个万向轮走过里程差值
float Global_C[3]={0,0,0};	   	       //全局参数保存
/**************机器人坐标系解算**************/
void Encoder_analysis(void)
{
	static u8 Encoder_cnt = 0;
	static u8 cnt=0;
	if(cnt==0)     //主控上电清除驱动板里程数据
	{
		cnt=1;
		Can_get_encoder(0x01,0x01);
		delay_us(300);
		Can_get_encoder(0x03,0x01);
		delay_us(300);
		Can_get_encoder(0x02,0x01);
		delay_us(300);
  }
	 else
	 {
		 
		  //由于三个请求一起发会出现丢帧现象，所以轮流每次发两个。
//	  	Can_get_encoder(++Encoder_cnt%3+1,0x00);
//      delay_us(300) ;
//	  	Can_get_encoder(++Encoder_cnt%3+1,0x00);
//      delay_us(300) ;

		  //发送一个请求，返回三个轮子数据
			Can_get_encoder(0x01,0x00);
      delay_us(300) ;
		 
			Len_M[0] = ( Ke.Total_angle.M1/360.0f ) * 2.0f* pi * wheel_R - Ke.Total_distance .M1;
			Ke.Total_distance .M1 += Len_M[0] ;
	

		
			Len_M[2] = ( Ke.Total_angle.M3/360.0f ) * 2.0f * pi * wheel_R - Ke.Total_distance .M3;
			Ke.Total_distance .M3 += Len_M[2] ;
			
			Len_M[1] = ( Ke.Total_angle.M2/360.0f ) * 2.0f * pi * wheel_R - Ke.Total_distance .M2;
			Ke.Total_distance .M2 += Len_M[1] ;	
			delay_us(200) ;
		 
		 
			Motor_To_Global_tf( Len_M , Global_C , (float)Ke.Robot.z ) ; 
			
			
			if(Global_C[0]<0.000001f&&Global_C[0]>-0.000001f)
			{
				Global_C[0]=0;
			}
			if(Global_C[1]<0.000001f&&Global_C[1]>-0.000001f)
			{
				Global_C[1]=0;
			}		
			if(Global_C[2]<0.000001f&&Global_C[2]>-0.000001f)
			{
				Global_C[2]=0;
			}	
			
			Ke.Robot.x -= Global_C[0];
			Ke.Robot.y -= Global_C[1];
		  Ke.Robot.z  = Yaw_Angle/180.0f*pi;
			
			
			//moto_to_robot
			Motor_To_Robot_tf(Len_M , Global_C);
			if(Global_C[0]<0.000001f&&Global_C[0]>-0.000001f)
			{
				Global_C[0]=0;
			}
			if(Global_C[1]<0.000001f&&Global_C[1]>-0.000001f)
			{
				Global_C[1]=0;
			}		
			if(Global_C[2]<0.000001f&&Global_C[2]>-0.000001f)
			{
				Global_C[2]=0;
			}	
			Ke.local_speed.x = Global_C[0];
			Ke.local_speed.y = Global_C[1];
			Ke.local_speed.z = Global_C[2];

   }

}



u8 Can_send_motorx(float v,u8 UID)   //  发送角速度到驱动核
{
	u8 mbox;
	u16 i=0;
	uint8_t b[4]={0};
	CanTxMsg TxMessageBuffer;
	
	TxMessageBuffer.StdId=UID;	 
	TxMessageBuffer.ExtId=UID;
	TxMessageBuffer.RTR=0;	              // 传输消息的帧类型为数据帧（还有远程帧）
	TxMessageBuffer.IDE=0;	              // 消息标志符实验标准标识符
	TxMessageBuffer.DLC=8;					       // 发送两帧，一帧8位
	TxMessageBuffer.Data[0]=0x08;			     // 1or2or3请求和回复和控制
	TxMessageBuffer.Data[1]=UID;			     // 驱动器ID
	TxMessageBuffer.Data[2]=0x90;          //速度指令
  TxMessageBuffer.Data[3]=0x00;
	itoh((int)v, b);                      /*********10进制转16进制***********/
	TxMessageBuffer.Data[4]=*b;
	TxMessageBuffer.Data[5]=*(b+1);
	TxMessageBuffer.Data[6]=*(b+2);
	TxMessageBuffer.Data[7]=*(b+3);
	
	i=0;	
  do{	
	mbox = CAN_Transmit(CAN1, &TxMessageBuffer);   //把期望角速度发给驱动板
	i++;
  }while((mbox==CAN_TxStatus_NoMailBox)&&i<0xfff);	//邮箱满重发
	i=0;	
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束	
  if(i>=0XFFF)return 1;
  return 0;
}


void Can_get_encoder(u8 UID , u8 noselect)          //  发送请求指令
{
	u16 i=0;
	u8 mbox;
	CanTxMsg TxMessageBuffe;
	
  TxMessageBuffe.StdId=UID;	 
	TxMessageBuffe.ExtId=UID; //发送者ID |  接受者ID     
	TxMessageBuffe.RTR=0;	     // 传输消息的帧类型为数据帧（还有远程帧）
	TxMessageBuffe.IDE=0;		     // 消息标志符实验标准标识符
	TxMessageBuffe.DLC=3;					       // 发送两帧，一帧8位
	TxMessageBuffe.Data[0]=0x03;			     // 数据长度
	TxMessageBuffe.Data[1]=UID;           // 接受者ID
	TxMessageBuffe.Data[2]=noselect;			 // 指令类型	 
	

	i=0;	
  do{	
	mbox = CAN_Transmit(CAN1, &TxMessageBuffe);   //把请求发给F1
	i++;
  }while((mbox==CAN_TxStatus_NoMailBox)&&i<0xfff);	//邮箱满重发
	
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//等待发送结束	
	
}



void CAN1_TX_IRQHandler(void)
{
	
	if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
		
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		//LED1=!LED1;
	}
	
}




void CAN1_RX0_IRQHandler(void)
{	
	
	CanRxMsg RxMessage;	
	static unsigned char byte[12];
	static char buff_finish_flag[2]={0,0};
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
		
		
			u8 i;
				
		  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); 		
			
			if((RxMessage.ExtId==((0x01<<8)|MYID)) && (RxMessage.DLC==0x08) && (RxMessage.Data[0]==0x02))	//请求里程
			{
				 
				 for(i=0;i<6;i++)
				 {
					 
						byte[i]=RxMessage.Data[i+2];	
					 
				 }		 
					buff_finish_flag[0]=1;
	    }
		 
		 	if((RxMessage.ExtId==((0x02<<8)|MYID)) && (RxMessage.DLC==0x08) && (RxMessage.Data[0]==0x02))	//请求里程
			{

				 for(i=0;i<6;i++)
				 {
					 
						byte[i+6]=RxMessage.Data[i+2];	
					 
				 }	 
				buff_finish_flag[1]=1;
	   }
		 
		 	if((RxMessage.ExtId==((0x03<<8)|MYID)) && (RxMessage.DLC==0x06) && (RxMessage.Data[0]==0x02))	//请求里程
			{
				 
				 for(i=0;i<4;i++)
				 {
					 
						byte[i]=RxMessage.Data[i+2];	
					 
				 }
		    memcpy( &Ke.Total_angle.M3 , byte ,sizeof(int) );	  //电机3			 
	 
	   }
		 
		 
		 if(buff_finish_flag[0]==1&&buff_finish_flag[1]==1)
		 {
			 buff_finish_flag[0]=0;
			 buff_finish_flag[1]=0;
			 memcpy( &Ke.Total_angle.M1 , byte ,4);	
			 memcpy( &Ke.Total_angle.M2 , byte+4 ,4 );	
			 memcpy( &Ke.Total_angle.M3 , byte+8 ,4);	
		 }
						 
		 CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		 CAN_FIFORelease(CAN1,CAN_FIFO0); //清中断标志
			
	}
	
}



/**********************************************************************
TxMessage数据包格式
uint32_t StdId;   //发送者ID
uint32_t ExtId;   //接受者ID
uint8_t IDE;      //CAN标识符  系统占用
uint8_t RTR;       // 消息类型  系统占用
uint8_t DLC;       // 数据帧的个数
uint8_t Data[8];   // 最大支持8帧数据

底盘电机ID    0X01 0X02 0X03 0X04
底盘主控ID    0X10 

主控和电机之间的通讯

1.主控发送一个包的请求数据指令DLC=2，DATA[0]=1or2or3请求和回复和控制   DATA[1]=指令类型
  驱动返回2个包的信息
  包1   DLC= 6   DATA[0]=1or2or3请求和回复和控制     DATA[1]=0X00指令类型   DATA[2]DATA[3]DATA[4]DATA[5]    为轮子当前里程 
  包2   DLC= 6   DATA[0]=1or2or3请求和回复和控制     DATA[1]=0X01指令类型   DATA[2]DATA[3]DATA[4]DATA[5]    clear
  包3   DLC= 6   DATA[0]=1or2or3请求和回复和控制     DATA[1]=0X03指令类型   DATA[2]DATA[3]DATA[4]DATA[5]    为主控期望速度 （云台电机为期望角度）
 
2.主控发送一个包的控制指令DLC=6，DATA[0]=1or2or3请求和回复和控制   DATA[1]=指令类型  DATA[2]DATA[3]DATA[4]DATA[5] 为期望速度（云台电机为期望角度）
  驱动返回1个包确认收到信息 
  包   DLC= 3   DATA[0]=1or2or3请求和回复和控制   DATA[1]=指令类型    DATA[3]=1 确认收到


**********************************************************************/
