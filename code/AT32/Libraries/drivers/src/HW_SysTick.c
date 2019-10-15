#include<hw_systick.h>

static uint32_t fac_us1 = 0;

void Delayinit(uint32_t systicknum)
{
	//传入内部时钟源的频率
	//初始化systick定时器
	//选择时钟源
	//用传入的内部时钟源,计算出ms和us的周期数
	//初始化fac_ms,fac_us.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk ;
	fac_us1 = systicknum / 1000000;
}

static volatile int test;
void delayms(uint32_t num)
{
	//由于重装载寄存器只有24位来保存数据,所以传入的数不能太大,
	//如果传入的数据过大,需要分多次写入重装载寄存器.
	//计算最大值
	uint32_t temp = 0;
	SysTick->LOAD = fac_us1 * 1000;
	SysTick->VAL = SysTick->LOAD;
	SysTick->CTRL |= 1;
	while(num--)
	{
        do{
            temp = SysTick->CTRL;
        }while((temp & SysTick_CTRL_ENABLE_Msk ) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
    }
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

void delayus(uint32_t num)
{
	//把传入的延时时间乘以fac_us写入重装载寄存器
	//开启定时器
	//用do...while()出口条件循环来判断控制寄存器的第十六位
	//当第十六位为1时,关闭定时器.
	//最后结束函数调用.
	uint32_t temp = 0;
	
	SysTick->LOAD = num * fac_us1;
	SysTick->VAL = 0;
	SysTick->CTRL |= 1;
	do{
		 temp = SysTick->CTRL;
    }while((temp & SysTick_CTRL_ENABLE_Msk ) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/*
** @brief   enable or disable systick
** @note    none
** @param   val:
**          @arg  true    enable systick exception request
**          @arg  false   disable systick exception request
** @retval  none
*/
void systick_setexception(bool val)
{
	val == true ? (SysTick->CTRL |= 0x02) : (SysTick->CTRL &= ~(0x02)); 
}
void SysTick_Handler(void)
{
	//打印计数值
	char *str = 0;
	static uint32_t count = 0;
	count++;
	str = itoa(count);
	log_uart(HW_USART1,str);
}


