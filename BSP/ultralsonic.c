#include "ultralsonic.h"
#include "main.h"
void ultralsonic_init()
{
	USART_InitTypeDef USART_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure; 
  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
	//PB10 TX
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10 , GPIO_AF_USART3 );
  
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
	
  USART_Cmd(USART3, ENABLE);
	USART_HalfDuplexCmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
 // USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
	Delay(10);
}

void Halfduplex_sendbreak()
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    Delay(10);
    GPIO_SetBits(GPIOB,GPIO_Pin_10); 
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendBreak(USART3);  
}

void Halfduplex_command(u8 add, u8 com)
{
//     USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
		Halfduplex_sendbreak();
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3,add);
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    USART_SendData(USART3,com);
// 		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
}

void Set_Halfduplex_Adress(u8 odd_add,u8 new_add)
{
    Halfduplex_command(odd_add, 0xa0);
    Halfduplex_command(odd_add, 0xaa); 
    Halfduplex_command(odd_add, 0xa5);
    Halfduplex_command(odd_add, new_add);

}

