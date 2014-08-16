

#include "key.h"
#include "PWM.h"
//k1:pf8	k2:pf9	k3:pf10	k4:pf7	k5:pf6

uint16_t volatile PWM_set = motor_set_type;
// extern uint8_t move_ready;
// extern double pos_des[100];
// extern uint8_t pos_des_num;
// extern double pos_now[3];//纬度、经度、水平精度	

void KEY_init()
{
	
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//I/O配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//I/O配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//I/O配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	//K1...K5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_7| GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//M1、M2   GPS-PV(PA01)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//GPS-PV(PA01)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//test pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_9 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOH, &GPIO_InitStructure);
		
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource8);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource9);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource10);
 	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource6);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource7);

	EXTI_InitStructure.EXTI_Line = GPIO_Pin_8| GPIO_Pin_9| GPIO_Pin_10| GPIO_Pin_7| GPIO_Pin_6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	

	/* Enable and set EXTI4 Interrupt to the lowest priority */
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; /* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set EXTI4 Interrupt to the lowest priority */
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; /* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//中断服务函数
//k1按下则软件复位。
void EXTI9_5_IRQHandler() //外部中断
 {
	 if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
	{	  
		NVIC_SystemReset();
		EXTI_ClearITPendingBit(EXTI_Line8);			
	}	
	
	else if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
	{	  
  		//move_ready=!move_ready;
		//pos_des_num++;
		EXTI_ClearITPendingBit(EXTI_Line9);			
	}	
	
	else if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
	{	  
		PWM_set+=3;
		PWM_motor_set(PWM_set);
		EXTI_ClearITPendingBit(EXTI_Line7);			
	}	
	
	else if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
	{	  
		PWM_set-=3;
		PWM_motor_set(PWM_set);
		EXTI_ClearITPendingBit(EXTI_Line6);			
	}	
 }
 //中断服务函数
//k1按下则软件复位。
void EXTI15_10_IRQHandler() //外部中断
 {
	 if(EXTI_GetITStatus(EXTI_Line10)!=RESET)
	{	  
		//pos_des_num++;
// 		pos_des[pos_des_num*2-2]=pos_now[0];
// 		pos_des[pos_des_num*2-1]=pos_now[1];
		
		EXTI_ClearITPendingBit(EXTI_Line10);			
	}	

 }
 //返回mode的值
uint8_t get_mode()
 {
	return (GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_7)*2 + GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_6));
 }
 
  //返回GPS-PV的值
uint8_t get_PV()
 {
	return GPIO_ReadInputDataBit(GPIOA,  GPIO_Pin_1) ;
 }
 
 
