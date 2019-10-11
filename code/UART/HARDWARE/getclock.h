#ifndef __GETCLOCK_H
#define __GETCLOCK_H

#include<at32f4xx.h>

typedef enum {systemclock,hclk,pclk1,pclk2,hse,hsi,lse,lsi,pll} CLOCKFRE_t;
#define SYSTEMSOURCE     0x000c
#define PLL_FRAMASK      0x603C0000
#define AHB_FRAMASK      0x00F0
#define APB1_FRAMASK     0x0700
#define APB2_FRAMASK     0x3800

#define SYSTEM_1      0x0000
#define SYSTEM_2      0x0080
#define SYSTEM_4      0x0090
#define SYSTEM_8      0x00A0
#define SYSTEM_16     0x00B0
#define SYSTEM_64     0x00C0
#define SYSTEM_128    0x00D0
#define SYSTEM_256    0x00E0
#define SYSTEM_512    0x00F0
#define	HCLK_1        0x0000
#define	HCLK_2        0x0004
#define	HCLK_4        0x0005
#define	HCLK_8        0x0006
#define	HCLK_16       0x0007

uint32_t getclock_frequency(CLOCKFRE_t clock);
#endif
