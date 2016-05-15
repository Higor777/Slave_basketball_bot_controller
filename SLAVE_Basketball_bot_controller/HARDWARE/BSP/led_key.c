#include "BSP.h"

void LED_KEY_GPIO_Init(void)
{
		GPIO_InitTypeDef  gpio;
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

		/**********************LED≥ı ºªØ****************************/
	  gpio.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
		gpio.GPIO_Mode = GPIO_Mode_OUT;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_Init(GPIOF,&gpio);
		GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	
} 



