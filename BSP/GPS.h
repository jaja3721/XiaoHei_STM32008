
#ifndef __GPS_H
#define __GPS_H
#include "stm32f4xx.h"
#define Length_GPS 256

typedef struct
{
	uint8_t  Len_Header;
	uint16_t ID;//42:BESTPOS	99:BESTVEL
	uint16_t Len_Msg;//The length in bytes of the body of the message, not including the header nor the CRC
	double lat;//维度
	double lon;//经度
	double hor_spd;//大小 单位：m/s
	double trk_gnd;//方向 单位：°
	//float y;
}GPSMsgTypeDef;

extern uint8_t volatile GPS_pos_flag;
extern uint8_t volatile GPS_vel_flag;
extern GPSMsgTypeDef GPSMsg;




void GPS_int(void);
void USART1_IRQHandler(void);
uint8_t get_PV(void);

#endif
