//led function
#include<led.h>


void led_init(void)
{
	GPIO_Init(HW_GPIOD,13,gpio_speed_50MHz,mode_out_pp);
	GPIO_PinWrite(HW_GPIOD,13,1);
}
