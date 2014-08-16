#include "GPS.h"
#include "usart.h"
#include "main.h"
#include "crc_gps.h"

uint8_t GPS_Buff[Length_GPS];
uint8_t GPS_Buff_ptr;
uint8_t volatile GPS_pos_flag;
uint8_t volatile GPS_vel_flag;

GPSMsgTypeDef GPSMsg;

void GPS_int()
{

	NVIC_InitTypeDef 		NVIC_InitStructure; 
	char GPS_init1_Char[]="unlogall true\r\n";//42
	char GPS_init2_Char[]="log bestposb ontime 0.2\r\n";
	char GPS_init3_Char[]="log bestvelb ontime 0.2\r\n";
	uint8_t i;
	
  usart1_init();
	Delay(300);
	for (i=0;i<15;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, (uint16_t) GPS_init1_Char[i]);	
	}
	Delay(50);
	for (i=0;i<25;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, (uint16_t) GPS_init2_Char[i]);	
	}
	Delay(50);
	for (i=0;i<25;i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_SendData(USART1, (uint16_t) GPS_init3_Char[i]);	
	}
			
	/* Enable the USART1 ͨ�Ŷ˿� Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* ��ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	
	
	GPS_Buff[0]=0xaa;
	GPS_Buff[1]=0x44;
	GPS_Buff[2]=0x12;	
}

//COM �жϽ���
void USART1_IRQHandler()
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET && get_PV())//
  {
		GPS_Buff[GPS_Buff_ptr++] = USART_ReceiveData(USART1);		
		
		if(GPS_Buff[GPS_Buff_ptr-1]==0x12 )
		{
			if(GPS_Buff[GPS_Buff_ptr-2]==0x44 && GPS_Buff[GPS_Buff_ptr-3]==0xaa)
			{
				GPS_Buff_ptr=3;
				GPS_Buff[0]=0xaa;
				GPS_Buff[1]=0x44;
				GPS_Buff[2]=0x12;
			}
		}
		
		if (GPS_Buff_ptr==4)
			GPSMsg.Len_Header=GPS_Buff[3];
		else if (GPS_Buff_ptr==6)
			GPSMsg.ID=*(uint16_t*)(&GPS_Buff[4]);
		else if (GPS_Buff_ptr==10)
			GPSMsg.Len_Msg=*(uint16_t*)(&GPS_Buff[8]);
				
		if (GPS_Buff_ptr==(GPSMsg.Len_Header + GPSMsg.Len_Msg +4))
		{
			if(CalculateBlockCRC32(GPSMsg.Len_Header + GPSMsg.Len_Msg +4, GPS_Buff)==0);//60us
			{
				if (GPSMsg.ID==42)
				{
					GPSMsg.lat=*((double*)(&GPS_Buff[GPSMsg.Len_Header+8]));
					GPSMsg.lon=*((double*)(&GPS_Buff[GPSMsg.Len_Header+16]));
					GPS_pos_flag=1;
				}
				else if (GPSMsg.ID==99)
				{
					GPSMsg.hor_spd=*((double*)(&GPS_Buff[GPSMsg.Len_Header+16]));
					GPSMsg.trk_gnd=*((double*)(&GPS_Buff[GPSMsg.Len_Header+24]));
					GPS_vel_flag=1;
					printf("%f %f\r\n",GPSMsg.hor_spd, GPSMsg.trk_gnd);
				}			
			}			
		}	
	 }
	else
		USART_ReceiveData(USART1);	
}

	


