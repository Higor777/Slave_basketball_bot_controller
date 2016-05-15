#include "BSP.h"
#include "math.h"

#include "myextern.h"

//解码遥控器信号
/*-----USART2_RX-----PA3----*/ 
//for D-BUS

    


uint8_t   S_sbus_buffer[25];
uint16_t  G_sbus_channel[16] = {0};     //temp sbus decode channel data
uint8_t  G_sbus_flag ;

void USART2_Configuration(void)
{
		USART_InitTypeDef usart2;
		GPIO_InitTypeDef  gpio;
		//NVIC_InitTypeDef  nvic;
		DMA_InitTypeDef   dma;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_DMA1,ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);

		GPIO_PinAFConfig(GPIOA,GPIO_PinSource3 ,GPIO_AF_USART2);

		gpio.GPIO_Pin = GPIO_Pin_3 ;
		gpio.GPIO_Mode = GPIO_Mode_AF;
		gpio.GPIO_OType = GPIO_OType_PP;
		gpio.GPIO_Speed = GPIO_Speed_100MHz;
		gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA,&gpio);

		USART_DeInit(USART2);
		usart2.USART_BaudRate = 100000;   //SBUS 100K baudrate
		usart2.USART_WordLength = USART_WordLength_8b;
		usart2.USART_StopBits = USART_StopBits_1;
		usart2.USART_Parity = USART_Parity_Even;
		usart2.USART_Mode = USART_Mode_Rx;
		usart2.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init(USART2,&usart2);

		USART_Cmd(USART2,ENABLE);
		USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);

//		nvic.NVIC_IRQChannel = DMA1_Stream5_IRQn;
//		nvic.NVIC_IRQChannelPreemptionPriority = 0;
//		nvic.NVIC_IRQChannelSubPriority =0;
//		nvic.NVIC_IRQChannelCmd = ENABLE;
//		NVIC_Init(&nvic);

		DMA_DeInit(DMA1_Stream5);
		dma.DMA_Channel= DMA_Channel_4;
		dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART2->DR);
		dma.DMA_Memory0BaseAddr = (uint32_t)S_sbus_buffer;
		dma.DMA_DIR = DMA_DIR_PeripheralToMemory;
		dma.DMA_BufferSize = 25;
		dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
		dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		dma.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		dma.DMA_Mode = DMA_Mode_Circular;
		dma.DMA_Priority = DMA_Priority_VeryHigh;
		dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
		dma.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
		dma.DMA_MemoryBurst = DMA_Mode_Normal;
		dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(DMA1_Stream5,&dma);

		DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Stream5,ENABLE);
}



void DMA1_Stream5_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5))
    {
			  DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
        DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);

        //遥控器的六个通道解码，每个通道占11bit,从低位到高位截取出来
		if (S_sbus_buffer[ 0] == 0x0f)	
		{
			G_sbus_channel[0] = (S_sbus_buffer[ 2] & 0x7) << 8 | S_sbus_buffer[ 1];   //右手  左打到底366    右打到底1680  
			G_sbus_channel[8] = (S_sbus_buffer[ 13] & 0x7) << 8 | S_sbus_buffer[ 12];
			G_sbus_channel[1] = (S_sbus_buffer[ 3] & 0x3F) << 5 | S_sbus_buffer[ 2] >> 3;   //右手  前打到底366    后打到底1680 
			G_sbus_channel[9] = (S_sbus_buffer[ 14] & 0x3F) << 5 | S_sbus_buffer[ 13] >> 3;
			G_sbus_channel[2] = (S_sbus_buffer[ 5] & 0x1) << 10 | S_sbus_buffer[ 4] << 2 | S_sbus_buffer[ 3] >> 6;  //左手  前打到底1680    后打到底368 
			G_sbus_channel[10] = (S_sbus_buffer[ 16] & 0x1) << 10 | S_sbus_buffer[ 15] << 2 | S_sbus_buffer[ 14] >> 6;
			G_sbus_channel[3] = (S_sbus_buffer[ 6] & 0xF) << 7 | S_sbus_buffer[ 5] >> 1; //左手  右打到底1680    左打到底366 
			G_sbus_channel[11] = (S_sbus_buffer[ 17] & 0xF) << 7 | S_sbus_buffer[ 16] >> 1;
			G_sbus_channel[4] = (S_sbus_buffer[ 7] & 0x7F) << 4 | S_sbus_buffer[ 6] >> 4;  // F  前打144    后打1902
			G_sbus_channel[12] = (S_sbus_buffer[ 18] & 0x7F) << 4 | S_sbus_buffer[ 17] >> 4;
			G_sbus_channel[5] = (S_sbus_buffer[ 9] & 0x3) << 9 | S_sbus_buffer[ 8] << 1 | S_sbus_buffer[ 7] >> 7;
			G_sbus_channel[13] = (S_sbus_buffer[ 20] & 0x3) << 9 | S_sbus_buffer[ 19] << 1 | S_sbus_buffer[ 18] >> 7;
			G_sbus_channel[6] = (S_sbus_buffer[ 10] & 0x1F) << 6 | S_sbus_buffer[ 9] >> 2; //C  前打到底144    中打到底1024  后打1904 
			G_sbus_channel[14] = (S_sbus_buffer[ 21] & 0x1F) << 6 | S_sbus_buffer[ 20] >> 2;
			G_sbus_channel[7] = S_sbus_buffer[ 11] << 3 | S_sbus_buffer[ 10] >> 5;
			G_sbus_channel[15] = S_sbus_buffer[ 22] << 3 | S_sbus_buffer[ 21] >> 5;
    }			

    G_sbus_flag = S_sbus_buffer[ 23];
				
				  	
		DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF5);
    }
}

extern Robot_data  Ke ;
extern System_flag System;   // 系统各标志
#define Invalidwidth 80  	//遥控器无效宽度
uint16_t x=1024,y=1024,z=1024;  // 速度向量


		

uint16_t Inwidth(uint16_t data,u8 m)   //去掉无效宽度
{
	
	 uint16_t temp;
   if(data>(1024+m))
	 {
     temp=data-m;
   }
	 else if(data<(1024-m))
	 {
     temp=data+m;
   }
	 else
	 {
     temp=1024;
   }
	 
	 return temp;
	 
}

