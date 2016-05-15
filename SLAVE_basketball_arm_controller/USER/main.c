#include "stm32f10x.h"
#include "string.h"
#include "delay.h"	
#include "queue.h"
#include "usart2.h"			 //usart and 485
#include "motorcontrol.h"
#include "bsp.h"
#include "bsp_usart1.h"

#define MAX_DATA_LEN 100

volatile Queue rx_queue;
static int noselect;			//ÃüÁîÄ£Ê½Ñ¡Ôñ±ê¼Ç
float x,y,w,distance1,distance2,distance3,distance4;								//×ø±êºÍ´«¸ÐÆ÷¾àÀë
extern float destpid_angle4;            //²½½øµç»úËÙ¶
volatile int hehe;

uint8_t radBufferRS485[99];  		 	//¼ÇÂ¼×ª½Ç
void handle_data(uint8_t buf[], int D,int len)    
{
	 
		switch(D)
		{ 
      case 2:      if(buf[0]==1) 
		            	  {
                      if(buf[1]==1) {noselect = 0x04 ;break;}       //get_ball
			           else if(buf[1]==2) {noselect = 0x05 ;break;}       //get_hold_ball
							   else if(buf[1]==3) {noselect = 0x06 ;break;}       //getfromhold
						     else if(buf[1]==4) {noselect = 0x07 ;break;}       //high_down
							   else if(buf[1]==5) {noselect = 0x08 ;break;}       //high_lift
//							else if(buf[0]==6) {noselect = 0x09 ;break;}       //¼ñÇò»ú¹¹·ÅÏÂ  mid
			              }
			case 3:      if(buf[0]==1) {noselect = 0x0a ;break;}	      //Í¶Çò   ´Ë´¦CASEÒªºÍÐ­ÒéÅäºÏ
			
			case 4:      if(buf[0]==1) {noselect = 0x0b ;break;}        //·µ»ØËÄ¸ö´«¸ÐÆ÷¾àÀë
			
			default:break;
		}
	    
}  


void shot()            //ÉäÇò
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);	
	delayms(1500);
	GPIO_SetBits(GPIOB,GPIO_Pin_15);	
	delayms(1500);
}

void send(void)
{
#define d1 distance1 
#define d2 distance2 
#define d3 distance3 
#define d4 distance4
	int i;
	char byte[4];		   					
	uint16_t checksumsend1=0x16;				 //Ð£ÑéÎ»³õÊ¼Öµ£¨·µ»Ø´«¸ÐÆ÷¾àÀë1 | ·µ»ØÎ»ÖÃ2£©
	RS485_TX_MODE   //TXÄ£Ê½
	if(USART_GetFlagStatus(USART2, USART_FLAG_TC)!=RESET)
	{	
 if(noselect==0x04) 							//µÃÇò		 
		{
		  get_ball();
      distance1++;			
						
			noselect=0;
	 	}
		
 if(noselect==0x05) 							//µÃÇò²¢±£³Ö³ÖÇò×´Ì¬
		{
		  get_hold_ball();				
			USART1_SendChar(0xff);			
			USART1_SendChar(0xff);			
			USART1_SendChar(0x02);										
			USART1_SendChar(0x00);	
			USART1_SendChar(0x02);
			USART1_SendChar(0x01);
			USART1_SendChar(0x00);	
			USART1_SendChar(0x05);						
			noselect=0;
	 	}
	if(noselect==0x06) 							//´Ó³ÖÇòÎ»ÖÃµÃÇò			 
		{
		  getfromhold();				
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0x02);										
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x02);
//			USART1_SendChar(0x01);
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x05);							
			noselect=0;
	 	}
	if(noselect==0x07) 							//high_down		 
	{				
			USART1_SendChar(0xff);			
			USART1_SendChar(0xff);			
			USART1_SendChar(0x02);										
			USART1_SendChar(0x00);	
			USART1_SendChar(0x02);
			USART1_SendChar(0x01);
			USART1_SendChar(0x00);	
			USART1_SendChar(0x05);
				
		  high_down();
			noselect=0;
	 	}
		if(noselect==0x08) 							//mid_lift	 
		{				
			USART1_SendChar(0xff);			
			USART1_SendChar(0xff);			
			USART1_SendChar(0x02);										
			USART1_SendChar(0x00);	
			USART1_SendChar(0x02);
			USART1_SendChar(0x01);
			USART1_SendChar(0x00);	
			USART1_SendChar(0x05);     
			high_lift();			
			noselect=0;
	 	}
//		if(noselect==0x09) 							//mid_down		 
//		{
//		 // mid_down();				
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0x02);										
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x02);
//			USART1_SendChar(0x04);
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x05);
//			
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0xff);			
//			USART1_SendChar(0x02);										
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x02);
//			USART1_SendChar(0x04);
//			USART1_SendChar(0x00);	
//			USART1_SendChar(0x05);		
//			noselect=0;
//	 	}
 if(noselect==0x0a) 							//Í¶Çò			 
		{
		  shot();				
			USART1_SendChar(0xff);			
			USART1_SendChar(0xff);			
			USART1_SendChar(0x03);										
			USART1_SendChar(0x00);	
			USART1_SendChar(0x02);
			USART1_SendChar(0x01);
			USART1_SendChar(0x00);	
			USART1_SendChar(0x06);
			noselect=0;
	 	}
		
	if(noselect==0x0b) 							//ºìÍâ²â¾à			 
		{
		  //LED2(ON);				
			USART1_SendChar(0xff);			
			USART1_SendChar(0xff);			
			USART1_SendChar(0x04);										
			USART1_SendChar(0x00);	
			USART1_SendChar(0x11);
			USART1_SendChar(0x01);	
		  memcpy( byte,&d1,sizeof(float));
		   
			for(i=0;i<4;i++)
			 {
				USART1_SendChar(byte[i]);
				checksumsend1=checksumsend1+byte[i];
			 }		 
		  	memcpy( byte,&d2,sizeof(float));
			for(i=0;i<4;i++)
			 {
			  USART1_SendChar(byte[i]);			
			  checksumsend1=checksumsend1+byte[i];
			 }	
		  	memcpy( byte,&d3,sizeof(float));
			
			for(i=0;i<4;i++)
			 {
				USART1_SendChar(byte[i]);		
				checksumsend1=checksumsend1+byte[i];
			 }
		  	memcpy( byte,&d4,sizeof(float));
	    for(i=0;i<4;i++)
			 {
			  USART1_SendChar(byte[i]);			
			  checksumsend1=checksumsend1+byte[i];
			 }
			checksumsend1=checksumsend1%(0xff);
		
			USART1_SendChar(checksumsend1);						
			noselect=0;
	 	}
		 
     }
    	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET);				
	//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		RS485_RX_MODE	 //RXÄ£Ê½			
}


/*******************************Í¨ÐÅ²¿·ÖÒªÇó¸ßËÙ£¬²¨ÌØÂÊ1000000
ÉÏÎ»»ú´«À´µÄÊý¾ÝÒÑÑ­»·¶ÓÁÐµÄ·½Ê½ÔÚÖ÷º¯ÊýÖÐ¹ý×´Ì¬»ú£¬ÒÔ±£Ö¤ÎÞÊý¾Ý¶ªÊ§
**********************************************************/
int main()
{	

	static uint16_t checksum;
	uint8_t datalen1;
  int device;
	short data_len;
	int cur=0;
	uint8_t data;
	uint8_t  buffer[MAX_DATA_LEN] = {0};

  uint16_t adcx;
  uint8_t k=4,distance=0;
  uint16_t b=166;
  uint16_t m=34404;
  
    enum Recstate								   		//×´Ì¬»ú 
	{
		RECFF1,RECFF2,SENDID,RECID,RECLEN1,RECLEN2,RECSEL,RECCHECK
	} rs485Recstate = RECFF1;
   delayinit(72);	   
	//LED1(0);
	//Init_System_Timer();   //ÏµÍ³Ê±ÖÓ³õÊ¼»¯  ÕâÀïÒÔtim5×÷ÎªÏµÍ³Ê±ÖÓ
  //USART2_485init();	  //484(usart2)³õÊ¼»¯
	usart1_config(115200);  //usart1³õÊ¼»¯
	BSPINIT();
	RS485_RX_MODE			
	queue_init(&rx_queue);

	//USART1_SendChar(0xff);
	
	while(1)	
	{
      if(hehe==1)
			{
				
				hehe=0;
				adcx=Get_Adc_Average(ADC_Channel_1,10);
        distance=(m/(adcx-b))-k;
				if(distance<=10)                           //±¨¾¯£¬¾àÀëÇòÐ¡ÓÚ10cm
				{
				  // USART1_SendChar(rx);
				  // USART1_SendChar(rx);
					// USART1_SendChar(rx);
				  // USART1_SendChar(rx);
					// USART1_SendChar(rx);
				  // USART1_SendChar(rx);
					// USART1_SendChar(rx);
				  // USART1_SendChar(rx);
				}
       
       }				
		if(queue_empty(rx_queue))
			            continue;

		data = (uint8_t)queue_get(&rx_queue);
  
		switch (rs485Recstate) 
		{
			case RECFF1:
				if (data == 0xff)	  
				{	
					rs485Recstate = RECFF2;	
					checksum=0;		
					cur = 0;								//Ð£ÑéÎ»ÇåÁã
 
}
	
				break;
	
			case RECFF2:
				if (data == 0xff)
					rs485Recstate = RECID;
								else
					rs485Recstate = RECFF1;
		
				break;
	
	
			case RECID:				 					
			     if(data==0x01)	                       //Éè±¸±àºÅ0x01µ×ÅÌÓëº½¼£
			        {
							device=1;
					    checksum += data;
					    rs485Recstate = RECLEN1;
								
				      }
			else if(data==0x02)                       //Éè±¸±àºÅ0x02¼ñÇò 
							{
							device=2;
					    checksum += data;
					    rs485Recstate = RECLEN1;
				      }
			else if(data==0x03)                        //Éè±¸±àºÅ0x03Í¶Çò 
				     {
							device=3;
					    checksum += data;
					    rs485Recstate = RECLEN1;
				      }
			else if(data==0x04) 			              //Éè±¸±àºÅ0x04 ²â¾à
				      {
				      device=4;
					    checksum += data;
					    rs485Recstate = RECLEN1;
				}
				else
					rs485Recstate = RECFF1;	  
				break;
	
			case RECLEN1:				 				
				checksum += data;
				datalen1 = data;
				rs485Recstate = RECLEN2;	  
				break;
		
			case RECLEN2:	
				checksum += data;			 				
				data_len = (short)datalen1 << 8	 | data;
				rs485Recstate = RECSEL;	  
				break;
	
			case RECSEL:
				checksum += data;
				buffer[cur++] = data;
				if(cur >= data_len)
					rs485Recstate = RECCHECK;
				break;
	
			case RECCHECK:
				checksum=checksum%255;
				if(data == checksum)
				{				
					handle_data(buffer,device, data_len);
  				send();				
					checksum=0;	
					rs485Recstate = RECFF1;	 
				}
				else
					rs485Recstate = RECFF1;
				break;
			 default:
					rs485Recstate = RECFF1;
			}	   
	   		 
	}		
		     
}



/***********************
³õÊ¼»¯³É¹¦ led1ÁÁ    
Í¨ÐÅ³É¹¦   led2ÁÁ

 ff ff 69 04  len1 len2 Ö¸Áî *********** chenk

case 1: noselect = 0x01 ;break;		   //¿ØÖÆÈý¸öµç»úµÄ½ÇËÙ¶È
case 2: noselect = 0x02 ;break;		   //µÃµ½Èý¸öµç»úµÄ½ÇËÙ¶È
case 3: noselect = 0x03 ;break;	       //ÖØÖÃ±àÂëÆ÷Êý¾Ý		 
case 4: noselect = 0x04 ;break;	       //¶ÁÈ¡x,y,w×ø±ê	



·¢ËÍ ff ff 69 04 00 0d   01   £¨Ò»ºÅ£©**** £¨¶þºÅ£©**** £¨ÈýºÅ£©****	  chek  ¿ØÖÆÈý¸öµç»úµ½Ö¸¶¨½ÇËÙ¶È
·µ»Ø ff ff 04 69 00 02   01     0c	   7c (chk) 

·¢ËÍ ff ff 69 04 00 01   02    74(chenk)                                         ¶ÁÈ¡Èý¸ö½ÇËÙ¶È
·µ»Ø ff ff 04 69 00 0d   02   £¨Ò»ºÅ£©**** £¨¶þºÅ£©**** £¨ÈýºÅ£©****	   chek 

·¢ËÍ ff ff 69 04 00 01	 03	   75(chk)						   ÖØÖÃ±àÂëÆ÷Êý¾Ý	
·µ»Ø ff ff 04 69 00	02	 03     0c    7e£¨chk£©

·¢ËÍ ff ff 69 04 00 01	 04	   76(chk)					 ¶ÁÈ¡x,y,w×ø±ê
·µ»Ø ff ff 04 69 00 0d   04   £¨Ò»ºÅ£©**** £¨¶þºÅ£©**** £¨ÈýºÅ£©****	   chek 

//	   	RS485_TX_MODE   //TXÄ£Ê½
//	    printf("\r\n destpid_angle1=%f destpid_angle2=%f destpid_angle3=%f\r\n ",destpid_angle1, destpid_angle2,destpid_angle3); 
//	    printf("\r\n AV1=%lf AV2=%lf  AV3=%lf \r\n ", AV1 , AV2, AV3);
//      printf("\r\n destpid_length1=%f destpid_length2=%f  destpid_length3=%f \r\n ",destpid_length1,destpid_length2,destpid_length3); 
//      printf("\r\n past_length1=%f past_length2=%f  past_length3=%f \r\n ",past_length1,past_length2,past_length3); 
//	    printf("\r\n x=%f y=%f  w=%f \r\n ",x,y,w); 
//       printf("\r\n rlength1=%f rlength2=%f  rlength3=%f \r\n ",rlength1,rlength2,rlength3);		
// 	    RS485_RX_MODE   //RXÄ£Ê½
//	    NOW_TIME=GET_Nowtime();

*****************************/
