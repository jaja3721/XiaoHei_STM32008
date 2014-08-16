

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include "PWM.h"
#include "encoder.h"
#include "ultralsonic.h"
#include "usart.h"
#include "stm32f4_lcd.h"
#include "GPS.h"
#include "IMU_ADIS16xxx.h"
#include "key.h"
#include "timer.h"
#include "control.h"
#include "ad.h"
#include "CRC_GPS.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;

extern uint8_t LCD_flag;
void ddelay(uint16_t u);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	RCC_ClocksTypeDef RCC_Clocks;


  /* SysTick end of count event each 10ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);
	
	/********************输出 初始化 ******************/
	PWM_init();
  
	
	STM324xG_LCD_Init();	
  LCD_Clear(BLACK);/* Clear the LCD */
  LCD_SetBackColor(BLACK);/* Set the LCD Back Color */  
  LCD_SetTextColor(WHITE);/* Set the LCD Text Color */	
  LCD_DisplayStringLine(LINE(0), "  Hello jaja. I'm xiaohei01");
	
	
	/********************输入 初始化 ******************/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);/*!< 3 bits for pre-emption priority 1 bits for subpriority */	
	
	Encoder_init();	//encoder tim8	
	usart_init();//通信COM usart2 115200
	
	IMU_init();	
	if (get_mode()==1)
	{
		IMU_BE10();
	}
	
	KEY_init();		
	AD_Init();	
	GPS_int();

	TIM2_Configuration();
	printf("uart init OK 2 \r\n");

	
while(1)
  {


	if(LCD_flag)
	{
		LCD_flag=0;
		lcd_refresh();
	}


  }  
}



/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
 */ 
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
   
/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif 


void recev_ultrasonic()
{
// 	uint8_t tt;
//   if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
//   {
//     /* Read one byte from the receive data register */
//     tt = USART_ReceiveData(USART3);
// 		ultrasonic_data[ultrasonic_pp++]=tt;    
//   }
}


void ddelay(uint16_t u)
{
		uint16_t i,j;
		for(i=0;i<u;i++)
			for (j=0;j<100;j++);
}




