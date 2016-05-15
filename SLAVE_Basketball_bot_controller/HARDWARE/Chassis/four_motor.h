#ifndef __four_motor_H__
#define __four_motor_H__
#include "stm32f4xx.h"
u8 Can_send_motorx(float v,u8 UID);
void Chassis_control(void);
void Can_get_encoder(u8 UID , u8 noselect);         //  ∑¢ÀÕ«Î«Û÷∏¡Ó
void Encoder_analysis(void);

#endif


