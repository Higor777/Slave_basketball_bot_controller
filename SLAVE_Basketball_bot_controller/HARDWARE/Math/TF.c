#include <math.h>
#include "TF.h"
#include "myextern.h"





/*************************************************
函数  void Robot_To_Motor_tf(float Motor[3] , float *Global ,float R_theta)   
**************************************************/

void Robot_To_Motor_tf(float Robot[3] , float *Motor )   
{
	*Motor = 0 * Robot[0] + 1 * Robot[1] + radius * Robot[2] ;
	*(Motor + 1) =  -0.866025f* Robot[0] - 0.5f * Robot[1] + radius * Robot[2] ;
	*(Motor + 2) =  0.866025f * Robot[0] - 0.5f * Robot[1] + radius * Robot[2] ;

}



/*************************************************
函数  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Motor_To_Robot_tf(float Motor[3] , float *Global )   
{
	*Global = 0 * Motor[0] + -0.57735f * Motor[1] + 0.57735f * Motor[2] ;
	*(Global + 1) =  0.666667f* Motor[0]  -0.333333f * Motor[1] + -0.333333f * Motor[2] ;
	*(Global + 2) =  ( 0.333333f/ (float)radius ) * ( Motor[0] + Motor[1] +  Motor[2] ) ;

}

/*************************************************
函数  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Global_To_Robot_tf(float Robot[3] , float *Global ,float R_theta)   
{
	
	*Robot =  cos(R_theta) * Global[0] + sin(R_theta) * Global[1] + 0 * Global[2] ;
	*(Robot + 1) =  -sin(R_theta) * Global[0] + cos(R_theta) * Global[1] + 0 * Global[2] ;
	*(Robot + 2) =   0 * Global[0] + 0 * Global[1] + 1 * Global[2] ;
	
}


/*************************************************
函数  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Robot_To_Global_tf(float Robot[3] , float *Global ,float R_theta)   
{
	*Global = cos(R_theta)*Robot[0] - sin(R_theta) * Robot[1] +  0 * Robot[2] ;
	*(Global + 1) =  sin(R_theta) * Robot[0] + cos(R_theta) * Robot[1] +  + 0 * Robot[2] ;
	*(Global + 2) =  0 * Robot[0] + 0 * Robot[1] + 1 * Robot[2] ;

}

/*************************************************
函数  void Global_To_Motor_tf(float Global[3] , float *Motor ,float R_theta)   
三个万向轮式全局坐标到Motor坐标的转化

输入 机器人坐标的三个变量    全局坐标的三个变量的保存地址   机器人的和全局坐标的夹角

v1 v2 v3  Gvx Gvy Gvz  速度变换  或者坐标变换的微分
Global[3]  为全局坐标的速度x ，y ，z
Motor 为三个电机的速度 v1 ， v2 ，v3
**************************************************/


void Global_To_Motor_tf(float Global[3] , float *Motor ,float R_theta)   
{
	
	*Motor =  -sin(R_theta) * Global[0] + cos(R_theta) * Global[1] + radius * Global[2] ;
	*(Motor + 1) =  -sin(pi/3 - R_theta) * Global[0] - cos(pi/3 - R_theta) * Global[1] + radius * Global[2] ;
	*(Motor + 2) =  sin(pi/3 + R_theta) * Global[0] - cos(pi/3 + R_theta) * Global[1] + radius * Global[2] ; 

}

/*************************************************
函数  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
三个万向轮式的机器人坐标到全局坐标的转化

输入 机器人坐标的三个变量    全局坐标的三个变量的保存地址   机器人的和全局坐标的夹角
Gvx Gvy Gvz  v1 v2 v3 
 Global 为全局坐标的速度x ，y ，z
 Motor[3]  为三个电机的速度 v1 ， v2 ，v3

或者

 Global 为全局坐标的微分  dx ，dy ，dz
 Motor[3] 为三个电机的里程微分 d s1 ， d s2 ，d s3
**************************************************/

void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)   
{
	*Global = -0.666667f * sin(R_theta)*Motor[0] + (-0.57735f*cos(R_theta) + 0.333333f*sin(R_theta))*Motor[1] + (0.577350f*cos(R_theta) + 0.333333*sin(R_theta))*Motor[2] ;
	*(Global + 1) =  0.666667f * cos(R_theta)*Motor[0] + (-0.577350f*sin(R_theta) - 0.333333f*cos(R_theta))*Motor[1] + (0.57735f*sin(R_theta) - 0.333333f*cos(R_theta))*Motor[2] ;
	*(Global + 2) =  ( 1 / (float)(3*radius) ) * ( Motor[0] + Motor[1] +Motor[2] ) ;

}

