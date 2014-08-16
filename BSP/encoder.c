#include "encoder.h"

volatile float velocity;//实际速度

void Encoder_init()
{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
		TIM_ICInitTypeDef TIM_ICInitStructure;

		/* TIM8 clock enable */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

		/* GPIOC clock enable */
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		//PC6 A相 PC7 B相
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	  /* Connect TIM8 pins to AF2 */  
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8); 

		
		/* Timer configuration in Encoder mode */

		TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // No prescaling 
		TIM_TimeBaseStructure.TIM_Period = 10000; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
		TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

		TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12, 
														 TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
		TIM_ICStructInit(&TIM_ICInitStructure);
		TIM_ICInitStructure.TIM_ICFilter = 6;//ICx_FILTER;
		TIM_ICInit(TIM8, &TIM_ICInitStructure);

		// Clear all pending interrupts
		TIM_ClearFlag(TIM8, TIM_FLAG_Update);
		TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
		//Reset counter
		TIM8->CNT = 5000;

		TIM_Cmd(TIM8, ENABLE);
}
int16_t get_speed()
{
		int16_t speed_temp;
		speed_temp=TIM_GetCounter(TIM8)-5000;
		TIM8->CNT = 5000;//Reset counter
		return speed_temp;
}

