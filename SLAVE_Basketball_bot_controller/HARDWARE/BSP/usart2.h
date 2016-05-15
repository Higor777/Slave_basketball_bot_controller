#ifndef __USART2_H__
#define __USART2_H__


u8 keyb_filtering(u8 buf[],u8 x);
uint16_t Inwidth(uint16_t data,u8 m);   //去掉无效宽度
u8 Getbit(u8 data, u8 x);
void keyboard_analysis(void);
int16_t juduizhi(int16_t x);


void USART2_Configuration(void);
void DBUS_analysis(void);

#endif
