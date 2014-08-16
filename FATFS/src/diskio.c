/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
#include "sd.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2

#define SECTOR_SIZE 512

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
	SD_Error Status;

	switch ( drv )	
	{
		case 0 :
			Status = SD_Init(); /* SD_USER_Init() ����������û���д */
			if ( Status == SD_OK )	 /* SD ����ʼ���ɹ�,��һ���ǳ���Ҫ */
				return 0;
			else
			return STA_NOINIT;

		case 1 :
			return STA_NOINIT;

		case 2 :
			return STA_NOINIT;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{

	switch (drv) /* �û��Լ����Ӧ�ô��� */
	{
	  case 0 :
		
	  /* translate the reslut code here	*/
			if(SD_Detect()==SD_PRESENT)
					return 0;
			else
					return STA_NOINIT;

	  case 1 :
	
	  /* translate the reslut code here	*/

	    return STA_NODISK;

	  case 2 :
	
	  /* translate the reslut code here	*/

	    return STA_NODISK;

	  default:

        break;
	}
	return STA_NOINIT;
	
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{	
	if ( count == 1 )		/* 1��sector�Ķ����� */
  {	
  	SD_ReadBlock( (u8 *)(&buff[0]),sector <<9,  SECTOR_SIZE );
		//SD_ReadBlock( sector << 9, (u32 *)(&buff[0]), SDCardInfo.CardBlockSize );
		
	}
	else        			 /* ���sector�Ķ����� */
	{
		SD_ReadMultiBlocks( (u8 *)(&buff[0]),sector <<9 ,  SECTOR_SIZE, count );
		
	}
	
	SD_WaitReadOperation();
    while(SD_GetStatus() != SD_TRANSFER_OK);
	return RES_OK;		 

}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{	
	if ( count == 1 )		/* 1��sector��д���� */
  {		
		SD_WriteBlock((u8 *)(&buff[0]),sector <<9 ,SECTOR_SIZE);
	}
	else							 /* ���sector��д���� */
  {		
    SD_WriteMultiBlocks((u8 *)(&buff[0]),sector <<9,SECTOR_SIZE,count);
	}  
	SD_WaitWriteOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);	
	return RES_OK;	
}

#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	//return RES_OK;	// 0
	DRESULT res ;
	SD_CardInfo card;
	if(drv)
		return RES_PARERR;
	
	switch(ctrl)
	{
		case CTRL_SYNC:
			SD_WaitWriteOperation();
			while(SD_GetStatus() != SD_TRANSFER_OK);	
			res = RES_OK;	
			break;
		
		case GET_SECTOR_SIZE:
			*(WORD*)buff = 512;
			res = RES_OK;
			break;
		
		case GET_SECTOR_COUNT:
			
			SD_GetCardInfo(&card);
			*(DWORD*)buff=(uint32_t)((card.CardCapacity)>>9);
	//	  *(DWORD*)buff= (uint32_t)(GetCapacity()>>9);
			res = RES_OK;
			break;
		
		default:
			break;
	}

	return res;
/* ����ע�͵��Ĳ���ΪFATFS�Դ��� */
//	DRESULT res;
//	int result;
//
//	switch (drv) {
//	case ATA :
//		// pre-process here
//
//		//result = ATA_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//
//	case MMC :
//		// pre-process here
//
//		result = MMC_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//
//	case USB :
//		// pre-process here
//
//		result = USB_disk_ioctl(ctrl, buff);
//		// post-process here
//
//		return res;
//	}
//	return RES_PARERR;	
}
DWORD get_fattime()
{
	return 0;
}


