#ifndef _TF_H
#define _TF_H
void Robot_To_Motor_tf(float Robot[3] , float *Motor )   ;
void Motor_To_Robot_tf(float Motor[3] , float *Global)  ;
void Global_To_Robot_tf(float Robot[3] , float *Global ,float R_theta) ;
void Robot_To_Global_tf(float Robot[3] , float *Global ,float R_theta) ;
void Global_To_Motor_tf(float Global[3] , float *Motor ,float R_theta) ;
void Motor_To_Global_tf(float Motor[3] , float *Global ,float R_theta) ;
#endif
