#ifndef _AHRS_H
#define _AHRS_H

#include<stdint.h>

#define T 0.005f

void Kalman_filter(float* W,float* Z,uint8_t flag);
void observe(float acc_x,float acc_y,float acc_z);

#endif
