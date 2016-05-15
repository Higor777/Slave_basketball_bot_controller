#ifndef __myextern_H__
#define __myextern_H__
#include "stm32f4xx.h"


#define IMAX   400            //MA
#define PID_hz 200            // 1圈编码器的脉冲数
#define  wheel_R 0.051       //轮子半径
#define  pi 3.1415
#define  radius 0.255					//到中心的半径距离 	50/sqrt(3)	


typedef struct System{   // 系统各种标志
	      int8_t Identity ;        //ID 号
	      int8_t Signal_Update;    //通信更新标志
	      int8_t Signal_error;     //通信错误标志
	      int8_t S_Initialize;     //系统状态  0  系统没初始化   |1   系统初始化成功
	      float  Sys_Time;         //系统时钟 
	      float  CPU_T;            //CPU温度
	      float  Eledat;           //电流大小	
	      int8_t Elemoder;         //过流标志   1为过流
        int8_t Control_Moder;    // 控制模式
	      int16_t S_cnt1 ;
	      int16_t S_cnt2 ;
	      int16_t S_cnt3 ;
        }System_flag;      // 系统时间



#define Motor_Num 3 

typedef struct float_motor{
		float M1;
		float M2;
		float M3;
	 }Motor; 

//机器人绝对坐标
typedef struct float_Coordinate{        
		float x;
		float y;
		float z;
	 }Coord;

 //头部姿态
typedef struct float_head_Attitude{    
		float Yaw;
		float Pitch;
	 }Head;


 //头部姿态
typedef struct float_IMU{    
	  float Yaw_Angle;
		float Pitch_Angle;
	  float Roll_Angle;
	  float Acc_x;
		float Acc_y;
		float Acc_z;
	 }float_IMU;

typedef struct Robot_Local_Speed{
	float x;
	float y;
	float z;
}Robot_Local_Speed;

//底盘数据
typedef struct Robot_data{
	Robot_Local_Speed local_speed;
	Coord E_Global_Speed ;    //机器人的目标速度向量
	Coord Remote_Speed ;       //遥控器给的速度
	Motor L_Speed ;            //电机的当前速度
	Motor E_Speed ;            //电机的目的速度
	Coord Robot ;              //机器人坐标
	Motor Total_angle ;        //电机走的总角度 
	Motor Total_distance ;     //电机走的总里程
	float_IMU Imu ;
}Robot_data;


#endif
