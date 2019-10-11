#include "GPIO_Init.h"

GPIO_Type* gpio_list[]={GPIOA,GPIOB,GPIOC,GPIOD,GPIOE};

void GPIO_Init(uint32_t GPIOx,uint32_t pin,gpiospeed speed,gpiomode mode)
{
    int pinmode = 0;
    int temp = 0,pinmask = 0;
    
    switch(GPIOx)
    {
        case HW_GPIOA:
            RCC->APB2EN |= RCC_APB2EN_GPIOAEN;
            break;
        case HW_GPIOB:
            RCC->APB2EN |= RCC_APB2EN_GPIOBEN;
            break;
        case HW_GPIOC:
            RCC->APB2EN |= RCC_APB2EN_GPIOCEN;
            break;
        case HW_GPIOD:
            RCC->APB2EN |= RCC_APB2EN_GPIODEN;
            break;
        case HW_GPIOE:
            RCC->APB2EN |= RCC_APB2EN_GPIOEEN;
    }

    pinmode = mode & 0xf;
    if((mode & 0x10) == 0x10)
    	pinmode |= speed;
  
    if(pin<8) 
    {
     	temp = gpio_list[GPIOx]->CTRLL;
        pin <<= 2;
        
        pinmask = 0xf << pin;
        temp &= ~pinmask;
        temp |= pinmode << pin;
        
        if(mode == mode_in_pu)
            gpio_list[GPIOx]->BSRE |= 1 << pin;
        else if(mode == mode_in_pd)
            gpio_list[GPIOx]->BRE |= 1 << pin;
      
        gpio_list[GPIOx]->CTRLL = temp;
    }
    else if(pin >= 8 && pin < 16)                                         
    {
        temp = gpio_list[GPIOx]->CTRLH;
        pin = (pin -8)  << 2;
       
        pinmask = 0xf <<pin;
        temp &= ~pinmask;
        temp |= pinmode <<pin;
        if(mode == mode_in_pu)
            gpio_list[GPIOx]->BSRE |= 1 << pin;
        else if(mode == mode_in_pu)
            gpio_list[GPIOx]->BRE |= 1 << pin;
       
        gpio_list[GPIOx]->CTRLH = temp;
    }
}

void GPIO_PinWrite(uint32_t GPIOx,uint32_t pin,uint32_t num)
{
    if(num)
        gpio_list[GPIOx]->BSRE = 1 << pin;
    else
        gpio_list[GPIOx]->BRE = 1 << pin;
}

void GPIO_PinToggle(uint32_t GPIOx,uint32_t pin)
{
    if(gpio_list[GPIOx]->OPTDT & (0x1 << pin))
        GPIO_PinWrite(GPIOx,pin,0);
    else
        GPIO_PinWrite(GPIOx,pin,1);
}


