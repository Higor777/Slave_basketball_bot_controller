#include <math.h>
#include "TF.h"
#include "myextern.h"





/*************************************************
����  void Robot_To_Motor_tf(float Motor[3] , float *Global ,float R_theta)   
**************************************************/

void Robot_To_Motor_tf(float Robot[3] , float *Motor )   
{
	*Motor = 0 * Robot[0] + 1 * Robot[1] + radius * Robot[2] ;
	*(Motor + 1) =  -0.866025 * Robot[0] - 0.5 * Robot[1] + radius * Robot[2] ;
	*(Motor + 2) =  0.866025 * Robot[0] - 0.5 * Robot[1] + radius * Robot[2] ;

}



/*************************************************
����  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Motor_To_Robot_tf(float Motor[3] , float *Global )   
{
	*Global = 0 * Motor[0] + -0.57735 * Motor[1] + 0.57735 * Motor[2] ;
	*(Global + 1) =  0.666667* Motor[0]  -0.333333 * Motor[1] + -0.333333 * Motor[2] ;
	*(Global + 2) =  ( 0.333333/ (float)radius ) * ( Motor[0] + Motor[1] +  Motor[2] ) ;

}

/*************************************************
����  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Global_To_Robot_tf(float Robot[3] , float *Global ,float R_theta)   
{
	
	*Robot =  cos(R_theta) * Global[0] + sin(R_theta) * Global[1] + 0 * Global[2] ;
	*(Robot + 1) =  -sin(R_theta) * Global[0] + cos(R_theta) * Global[1] + 0 * Global[2] ;
	*(Robot + 2) =   0 * Global[0] + 0 * Global[1] + 1 * Global[2] ;
	
}


/*************************************************
����  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
**************************************************/

void Robot_To_Global_tf(float Robot[3] , float *Global ,float R_theta)   
{
	*Global = cos(R_theta)*Robot[0] - sin(R_theta) * Robot[1] +  0 * Robot[2] ;
	*(Global + 1) =  sin(R_theta) * Robot[0] + cos(R_theta) * Robot[1] +  + 0 * Robot[2] ;
	*(Global + 2) =  0 * Robot[0] + 0 * Robot[1] + 1 * Robot[2] ;

}

/*************************************************
����  void Global_To_Motor_tf(float Global[3] , float *Motor ,float R_theta)   
����������ʽȫ�����굽Motor�����ת��

���� �������������������    ȫ����������������ı����ַ   �����˵ĺ�ȫ������ļн�

v1 v2 v3  Gvx Gvy Gvz  �ٶȱ任  ��������任��΢��
Global[3]  Ϊȫ��������ٶ�x ��y ��z
Motor Ϊ����������ٶ� v1 �� v2 ��v3
**************************************************/


void Global_To_Motor_tf(float Global[3] , float *Motor ,float R_theta)   
{
	
	*Motor =  -sin(R_theta) * Global[0] + cos(R_theta) * Global[1] + radius * Global[2] ;
	*(Motor + 1) =  -sin(pi/3 - R_theta) * Global[0] - cos(pi/3 - R_theta) * Global[1] + radius * Global[2] ;
	*(Motor + 2) =  sin(pi/3 + R_theta) * Global[0] - cos(pi/3 + R_theta) * Global[1] + radius * Global[2] ; 

}

/*************************************************
����  void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)  
����������ʽ�Ļ��������굽ȫ�������ת��

���� �������������������    ȫ����������������ı����ַ   �����˵ĺ�ȫ������ļн�
Gvx Gvy Gvz  v1 v2 v3 
 Global Ϊȫ��������ٶ�x ��y ��z
 Motor[3]  Ϊ����������ٶ� v1 �� v2 ��v3

����

 Global Ϊȫ�������΢��  dx ��dy ��dz
 Motor[3] Ϊ������������΢�� d s1 �� d s2 ��d s3
**************************************************/

void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta)   
{
	*Global = -0.666667 * sin(R_theta)*Motor[0] + (-0.577350*cos(R_theta) + 0.333333*sin(R_theta))*Motor[1] + (0.577350*cos(R_theta) + 0.333333*sin(R_theta))*Motor[2] ;
	*(Global + 1) =  0.666667 * cos(R_theta)*Motor[0] + (-0.577350*sin(R_theta) - 0.333333*cos(R_theta))*Motor[1] + (0.577350*sin(R_theta) - 0.333333*cos(R_theta))*Motor[2] ;
	*(Global + 2) =  ( 1 / (float)(3*radius) ) * ( Motor[0] + Motor[1] +Motor[2] ) ;

}

