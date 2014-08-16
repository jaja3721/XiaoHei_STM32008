#include "record.h"

FATFS fs[2];            // Work area (file system object) for logical drive
FIL fsrc, fdst, file;         // file objects
BYTE buffer[512];       // file copy buffer
int a = 0;
FRESULT res;            // FatFs function common result code
UINT br, bw;            // File R/W count
char path[512]="0:"; 
char record_file_name[10];
char record_num[10];
uint8_t textFileBuffer[] = "感谢您选用 野火STM32开发板 ！^_^ \r\n";
char read_Buff[100];
char write_Buff[100];

extern volatile int32_t ggz;


void record_init()
{
	
	uint16_t num=0;
	int8_t i;
	NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure the NVIC Preemption Priority Bits */
 // NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  NVIC_InitStructure.NVIC_IRQChannel = SD_SDIO_DMA_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);  
	
	
	disk_initialize(0);
	res=f_mount(0, &fs[0]);

	res=f_open(&fsrc, "0:/num.txt",FA_OPEN_EXISTING | FA_READ | FA_WRITE);  //  以读方式打开，如果文件不存在则打开失败
	
	if ( res == FR_OK )
  { 

		res=f_read(&fsrc, read_Buff, sizeof(read_Buff), &br);
		for (i=0;i<br;i++)
		{
			num=num*10+(read_Buff[i]-0x30);
		}
		if (num>=99)
			num=0;


		sprintf(record_num,"%d",num+1);
		f_lseek(&fsrc,0);
		f_puts (record_num,&fsrc);
		f_truncate(&fsrc);		
    f_close(&fsrc);  
		
		sprintf(record_file_name,"0:/N%d.txt",num+1);
		f_open(&file, record_file_name,FA_CREATE_NEW| FA_WRITE);  //  以读方式打开，如果文件不存在则打开失败
		f_close(&file);  
  }
	else
		f_close(&fsrc);	                                      /* 关闭打开的文件 */	
	
}

void record()
{
		f_open(&file, record_file_name, FA_OPEN_EXISTING | FA_READ | FA_WRITE);  //  以读方式打开，如果文件不存在则打开失败
		f_lseek(&file,file.fsize);
		sprintf(write_Buff,"%f %d\r\n",velocity,ggz);
// 		f_puts (write_Buff,&file);
		f_printf(&file,write_Buff);
		f_close(&file);  
	
}

/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler()
{
  /* Process All SDIO Interrupt Sources  */
  SD_ProcessIRQSrc();
}

/**
  * @brief  This function handles DMA2 Stream3 or DMA2 Stream6 global interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void SD_SDIO_DMA_IRQHANDLER()
{
  /* Process DMA2 Stream3 or DMA2 Stream6 Interrupt Sources */
  SD_ProcessDMAIRQ();
}


