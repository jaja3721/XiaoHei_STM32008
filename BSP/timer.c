
#include "timer.h"
#include "locate.h"
#include "encoder.h"
#include "control.h"
uint8_t count_lcd;
uint8_t count_record;
uint8_t flag_record;
uint8_t LCD_flag;
uint8_t move_ready=0;
uint8_t move_flag=0;
uint8_t count_encoder;

extern uint8_t volatile connected_flag;
/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM2_Configuration()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef   			 NVIC_InitStructure;
	
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=5000;		 								/* 自动重装载寄存器周期的值(计数值) */
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (uint16_t) ((SystemCoreClock /2) / 100000) - 1; /* 时钟预分频数 100000hz */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    		/* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);																		/* 开启时钟 */
	
		//定时中断   													
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; /* 抢占优先级 */	
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
    	  
}


	
//中断定时 50ms 20Hz
void TIM2_IRQHandler(void)
{
	
	if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		
		if (connected_flag)
		{			
			connected_flag--;
			Move_Control();
		}
		else 
			Move_Stop();
			
		
		velocity=(float)(get_speed()/2740.9f); 
		

		locate_update();
		count_encoder++;
		count_lcd++;
		count_record++;
		if (count_lcd>20)
		{
			count_lcd=0;
			LCD_flag=1;
		}
		if (count_encoder>=5)
		{
			count_encoder=0;
		}

		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  
	}		
  	
}


