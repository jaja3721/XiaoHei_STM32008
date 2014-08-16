
// readme
// 连接：PA04-->INT	PB13-->CLK	PB14-->MOSI	PB15-->MISO
#include "IMU_ADIS16xxx.h"
#include "MahonyAHRS.h"
#include "arm_math.h"
#include "main.h"
#define imu_div 10

uint16_t data[15];
uint16_t count;
volatile int16_t ax,ay,az,gx,gy,gz,mx,my,mz;
volatile float roll,pitch,yaw;

volatile int32_t ggz;


 void ADI_BurstReadMode(void);

//中断服务函数
void EXTI4_IRQHandler() //外部中断
 {
	 float t11,t12,t13,t21,t22,t23,t31,t32,t33;
	 float aax,aay,aaz,ggx,ggy,ggz,mmx,mmy,mmz;	 
		if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
		{	  	
			count++;	
			 if(count==imu_div)
				 {
					 
					count=0;
					ADI_BurstReadMode();	
					 ggx=gz/1145.916f;//*0.05f*3.1415/180
					 ggy=gx/1145.916f;
					 ggz=gy/1145.916f;
					 
					 aax=az;//300
					 aay=ax;
					 aaz=ay;
					 
					 mmx=mz;
					 mmy=mx;
					 mmz=my;
					// MadgwickAHRSupdate(ggx, ggy, ggz, aax, aay, aaz, mmx, mmy, mmz);//well done #define betaDef		0.21f	
					 //MadgwickAHRSupdateIMU(ggx, ggy, ggz, aax, aay, aaz);//broke easy
					 
					// MahonyAHRSupdate(ggx, ggy, ggz, aax, aay, aaz, mmx, mmy, mmz);
					// MahonyAHRSupdate(ggx, ggy, ggz, aax, aay, aaz, 0.0f, 0.0f, 0.0f);//well done
					MahonyAHRSupdateIMU( ggx,  ggy,  ggz,  aax, aay, aaz);// 	
						//MahonyAHRSupdateIMU( ggx,  ggy,  ggz,  0, 0, 0);
					 //IMU_update(ggx, ggy, ggz);
					//更新方向余弦矩阵
					t11=q0*q0+q1*q1-q2*q2-q3*q3;
					t12=2.0f*(q1*q2+q0*q3);
					t13=2.0f*(q1*q3-q0*q2);
					t23=2.0f*(q2*q3+q0*q1);
					t33=q0*q0-q1*q1-q2*q2+q3*q3;
					//求出欧拉角
					roll = atan2(t23,t33);				
					pitch = -asin(t13);
					yaw = atan2(t12,t11);
				 } 
			EXTI_ClearITPendingBit(EXTI_Line4);	
				 
		}		
 }

void spi2_init()
{
	
	SPI_InitTypeDef    SPI_InitStruct;
	GPIO_InitTypeDef   GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType    = GPIO_OType_PP;
	GPIO_Init(GPIOI,&GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOI, GPIO_PinSource1 ,GPIO_AF_SPI2 );
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_SPI2 );
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource3, GPIO_AF_SPI2 );

	SPI_StructInit(&SPI_InitStruct);
	SPI_I2S_DeInit(SPI2);
	SPI_InitStruct.SPI_Direction=	SPI_Direction_2Lines_FullDuplex;//双向
	SPI_InitStruct.SPI_Mode		=	SPI_Mode_Master;//MSTA=1选择主设备，SSI=1
	SPI_InitStruct.SPI_DataSize	=	SPI_DataSize_16b;
	/*极性配置*/
	SPI_InitStruct.SPI_CPOL		=	SPI_CPOL_High;//SPI Clock Polarity
	/*相位配置*/
	SPI_InitStruct.SPI_CPHA		=	SPI_CPHA_2Edge;//SPI Clock Active Edge
	SPI_InitStruct.SPI_NSS		=	SPI_NSS_Soft; //SSM=1
	/*波特率为CPU频率的64分频*/
	SPI_InitStruct.SPI_BaudRatePrescaler	=	SPI_BaudRatePrescaler_128;//波特率128分频
	SPI_InitStruct.SPI_FirstBit	=	SPI_FirstBit_MSB ;//每帧先发送MSB
//	SPI_InitStruct.SPI_CRCPolynomial	= 7	 ;//CRC 校验

 	SPI_Init(SPI2,&SPI_InitStruct); /*初始化SPI1*/
  //  SPI_ITConfig(SPI2,SPI_I2S_IT_RXNE,ENABLE);
	SPI_Cmd(SPI2, ENABLE);	/*使能SPI1*/
}

void RST_init()
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);
	
}
void RST_IMU()
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	Delay(40);
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}

void EXTI4_init()
{
	GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//I/O配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
	//IMU
	/* Enable and set EXTI4 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; /* 抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; /* 响应优先级 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}


  /*
 输入值： uint16_t
 输出值： int16_t
 功能：14bits无符号 转换为 16bits 有符号格式
 */
 int16_t map_14bits_to_16bits(uint16_t data_14bits)
 {
 	unsigned short tp;
   	if(data_14bits & 0x2000)
   	tp=(unsigned)data_14bits | 0xC000;
   	else
   	tp=(unsigned)data_14bits & 0x3FFF;
   	return tp;

 }
 /*
 输入值：
 输出值：
 功能：突发式读取传感器值
 */
 //void ADI_BurstReadMode(int16_t *ax,int16_t *ay,int16_t *az,int16_t *gx,int16_t *gy,int16_t *gz,int16_t *mx,int16_t *my,int16_t *mz)
 void ADI_BurstReadMode()
 {
 	uint8_t i;

		while(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_TXE)==RESET);
			SPI_I2S_SendData(SPI2,0x3E00);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_RXNE)==RESET);
			SPI_I2S_ReceiveData(SPI2);
	for(i=0;i<12;i++)
 	{
	 	while(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_TXE)==RESET);
			SPI_I2S_SendData(SPI2,0x0000);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_FLAG_RXNE)==RESET);
			data[i]=SPI_I2S_ReceiveData(SPI2);
	}

 
	gx=map_14bits_to_16bits(data[1]);
	gy=map_14bits_to_16bits(data[2]);
	gz=map_14bits_to_16bits(data[3]);
	ax=map_14bits_to_16bits(data[4]);
	ay=map_14bits_to_16bits(data[5]);
	az=map_14bits_to_16bits(data[6]); 
	mx=map_14bits_to_16bits(data[7]);
	my=map_14bits_to_16bits(data[8]);
	mz=map_14bits_to_16bits(data[9]);
 
	/*
	依次输出：SUPPLY_OUT  XGYRO_OUT YGYRO_OUT ZGYRO_OUT XACCL_OUT YACCL_OUT ZACCL_OUT  XTEMP_OUT YTEMP_OUT ZTEMP_OUT  AUX_ADC
	*/
 }



void IMU_init()
{
	spi2_init();	
	RST_init();
	RST_IMU();
	
	Delay(50);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,0xB904);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,0xB802);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2);
	Delay(40);
	EXTI4_init();

}

//初始化设置 精准自动校准：BE10 恢复出厂设置：BE02
void IMU_BE10()
{
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,0xBE10);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2);
}

