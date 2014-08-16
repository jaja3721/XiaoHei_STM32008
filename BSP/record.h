#ifndef RECORD_H
#define RECORD_H

#include "main.h"
#include "sd.h"
#include "ff.h"
#include "diskio.h"
void record_init(void);
void SD_SDIO_DMA_IRQHANDLER(void);
void SDIO_IRQHandler(void);
void record(void);
#endif
