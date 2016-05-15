#ifndef _TRACKS_GUI_H
#define _TRACKS_GUI_H
void Local_Speed_Set(float x , float y , float z);
void Global_Speed_Set(float x ,float y ,float z) ;   //设置全局速度
void Robot_Speed_Set(float x ,float y ,float z)  ;  //设置机器人坐标速度
void R_Round_Motion_Set( float Line_Speed , float R) ;    //走圆型曲线  机器人坐标  低端走线
 //走圆型曲线  绝对坐标  高端走线  以x y为圆心 Line_Speed线速度  半径R  自转速度z
void G_Round_Motion_Set(float x ,float y , float Line_Speed , float R ,float z)   ;
void Sin_Motion_Set(float Amplitude  , float T , float Line_Speed , float z)  ;  //sin运动曲线  不能设置相位
#endif
