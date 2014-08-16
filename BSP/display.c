
#include "display.h"
#include "locate.h"
#include "encoder.h"
#include "GPS.h"
#include "ad.h"
#include "stm32f4_lcd.h"
#include "stdio.h"
extern volatile int16_t ax,ay,az,gx,gy,gz,mx,my,mz;
extern volatile int32_t ggz;
extern int32_t imu_dir;
extern int16_t mx_max,mx_min,my_max,my_min,mz_max,mz_min;
extern volatile uint8_t GPS_flag;
extern uint8_t GPS_Buff[Length_GPS];
extern uint8_t Length_Header;
extern uint8_t Header_ptr;
extern uint8_t GPS_Buff_ptr;
extern uint16_t volatile PWM_set;
extern double pos_now[3];
extern float dir_deg;
extern float Dire;
extern uint8_t pos_des_num;

//Ë¢ÐÂÆÁÄ»
void lcd_refresh(void)
{
	
	uint8_t message[50];
	sprintf(message,"  Voltage: %.1f V",(float)(Get_Voltage()/223.58));//12Î»AD
	LCD_DisplayStringLine(LINE(1), message);
	
	sprintf(message,"  Speed: %.2f m/s      ",velocity);
  LCD_DisplayStringLine(LINE(2), message);
// 	
	sprintf(message,"  p.x:%.2f**p.y:%.2f**     ",point_now.x, point_now.y);
  LCD_DisplayStringLine(LINE(3), message);

//   LCD_DisplayStringLine(LINE(5), Command0);

// 	LCD_DisplayStringLine(LINE(6), (uint8_t*)RxBuffer);  
	sprintf(message," ax=%d ,ay=%d az=%d******",  ax, ay, az);
	LCD_DisplayStringLine(LINE(6), message);

	sprintf(message," spd: %.8f   ",GPSMsg.hor_spd);
	LCD_DisplayStringLine(LINE(7), message);
	
	sprintf(message," gnd:%.8f   ", GPSMsg.trk_gnd);
	LCD_DisplayStringLine(LINE(8), message);
		
	sprintf(message," lat=%.10f   ", GPSMsg.lat);
	LCD_DisplayStringLine(LINE(9), message);
	
	sprintf(message," lat=%.10f   ", GPSMsg.lon);
	LCD_DisplayStringLine(LINE(10), message);
		
// 	sprintf(message," direction=%.2f", Dire);
// 	LCD_DisplayStringLine(LINE(10), message);

// 	sprintf(message," dir_deg=%.2f gy=%d ", dir_deg, gy );
// 	LCD_DisplayStringLine(LINE(11), message);
		
	sprintf(message," PWM_set=%d    ",PWM_set);
	LCD_DisplayStringLine(LINE(12), message);
 	//	printf("mxmax=%d mxmin=%d mymax=%d mymin=%d mzmax=%d mzmin=%d\r\n",mx_max,mx_min,my_max,my_min,mz_max,mz_min);


}
