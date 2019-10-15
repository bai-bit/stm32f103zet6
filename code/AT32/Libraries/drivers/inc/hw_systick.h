#ifndef __HW_SYSTICK_H
#define __HW_SYSTICK_H
#include<at32f4xx.h>
#include<uart.h>
#include<strfunc.h>
#define SYSTICK_SOURCE  SystemCoreClock
typedef enum {false = 0,true = !false} bool;

void Delayinit(uint32_t systicknum);
void systick_setexception(bool val);
void delayms(uint32_t num);
void delayus(uint32_t num);

#endif
