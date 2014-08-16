
#include "PWM.h"


TIM_OCInitTypeDef  TIM_OCInitStructure_Globle;

void PWM_GPIO_init()
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* GPIOB Configuration  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); 

}

void PWM_init()
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	
	uint16_t PrescalerValue = 0;	

	PWM_GPIO_init();
	 

	
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 1000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 17998;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	
	/* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure_Globle.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure_Globle.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure_Globle.TIM_OCPolarity = TIM_OCPolarity_High;
	
	PWM_servo_set(servo_set_type);
	PWM_motor_set(motor_set_type);
	PWM_pitch_set(pitch_set_type);
	PWM_yaw_set(yaw_set_type);


  TIM_ARRPreloadConfig(TIM3, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
	
		
}
void PWM_motor_set(uint16_t pp)
{
  if (pp > motor_set_max)
		pp=servo_set_max;
	else if (pp < motor_set_min)
		pp=motor_set_min;
	
	TIM_OCInitStructure_Globle.TIM_Pulse = pp;
	
  TIM_OC1Init(TIM3, &TIM_OCInitStructure_Globle);
  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void PWM_servo_set(uint16_t pp)
{
	if (pp > servo_set_max)
		pp=servo_set_max;
	else if (pp < servo_set_min)
		pp=servo_set_min;

	TIM_OCInitStructure_Globle.TIM_Pulse = pp;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure_Globle);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void PWM_pitch_set(uint16_t pp)
{
	if (pp > pitch_set_max)
		pp=pitch_set_max;
	else if (pp < pitch_set_min)
		pp=pitch_set_min;

	TIM_OCInitStructure_Globle.TIM_Pulse = pp;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure_Globle);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

void PWM_yaw_set(uint16_t pp)
{
	if (pp > yaw_set_max)
		pp=yaw_set_max;
	else if (pp < yaw_set_min)
		pp=yaw_set_min;

	TIM_OCInitStructure_Globle.TIM_Pulse = pp;

  TIM_OC4Init(TIM3, &TIM_OCInitStructure_Globle);
  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
}


