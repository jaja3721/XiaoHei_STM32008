

#include "usart.h"
#include "main.h"
#include "stdio.h"
uint8_t CMD_Buff[20];
uint8_t CMD_Buff_ptr;
uint8_t volatile CMD_flag;
extern int16_t volatile linear, angle;
extern int8_t volatile yun_yaw, yun_pitch;
extern uint8_t volatile connected_flag;
void usart_init()
{
	USART_InitTypeDef 	USART_InitStructure;
  GPIO_InitTypeDef 		GPIO_InitStructure; 
	NVIC_InitTypeDef 		NVIC_InitStructure;
 
 
  //PA2 TX
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //PA3 RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2 , GPIO_AF_USART2 );
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3 , GPIO_AF_USART2 );
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);
  USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	Delay(50);
	
	
		/* Enable the USART2 通信端口 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* 抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	

}

void usart1_init()
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
  

  //PA9 TX
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  

  //PA10 RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9 , GPIO_AF_USART1 );
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10 , GPIO_AF_USART1 );
  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1, &USART_InitStructure);
  USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	Delay(50);
}



//COM 中断接收
void USART2_IRQHandler(void)
{
   //Receive_Command();
	uint8_t temp;
	  if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )//
  {
		temp= USART_ReceiveData(USART2);	
		CMD_Buff[CMD_Buff_ptr++] = temp;		
		if(CMD_Buff_ptr>=19)
			CMD_Buff_ptr=19;
		
		if(temp=='@' )
		{
				CMD_Buff_ptr=1;
				CMD_Buff[0]='@';
		}
		
		if(temp=='e')
		{
			sscanf(CMD_Buff, "@11,%d,%d,%d,%de", &linear, &angle, &yun_yaw, &yun_pitch);
			connected_flag=7;
			//printf("recv:%d, %d**",linear, angle);
		}
	}
}


int fputc(int ch, FILE *f)
{

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
	USART_SendData(USART2, (uint16_t) ch);
  return ch;
}


