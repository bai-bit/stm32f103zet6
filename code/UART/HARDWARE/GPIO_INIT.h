#ifndef __GPIO_CUSTOM
#define __GPIO_CUSTOM

#include "at32f4xx.h"

#define HW_GPIOA  (0)
#define HW_GPIOB  (1)
#define HW_GPIOC  (2)
#define HW_GPIOD  (3)
#define HW_GPIOE  (4)

typedef enum  {
	mode_in_analog = 0x00,
	mode_in_floating = 0x04,
	mode_in_pd = 0x28,
	mode_in_pu = 0x48,
	mode_out_pp = 0x10,
	mode_out_od = 0x14,
	mode_af_pp = 0x18,
	mode_af_od = 0x1c
}gpiomode;

typedef enum  {
	gpio_speed_inno = 0,
	gpio_speed_10MHz = 1,
	gpio_speed_50MHz = 2,
	gpio_speed_2MHz
}gpiospeed;

void GPIO_Init(uint32_t GPIOx,uint32_t pin,gpiospeed speed,gpiomode mode);
void GPIO_PinWrite(uint32_t GPIOx,uint32_t pin,uint32_t num);
void GPIO_PinToggle(uint32_t GPIOx,uint32_t pin);
#endif
