#ifndef ULTRALSONIC_H
#define ULTRALSONIC_H
#include "stm32f4xx.h"
void ultralsonic_init(void);
void Halfduplex_sendbreak(void);
void Halfduplex_command(u8 add, u8 com);
void Set_Halfduplex_Adress(u8 odd_add,u8 new_add);
#endif
