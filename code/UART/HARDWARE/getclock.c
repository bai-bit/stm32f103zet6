//输入clock，返回对应的clock frequency
//思路：先获取systemcoreclock frequency
//然后读取相应的寄存器配置值
//然后用switch。。。case，根据clock和RCC->CFG的value
//将systemcoreclock 分频，return value
#include<getclock.h>

uint32_t getclock_frequency(CLOCKFRE_t clock)
{
	//get system core clock frequency
	uint32_t value = 0,ahb_div = 0,apb1_div = 0,apb2_div = 0;
	uint32_t pll_frafremask = 0,pll_frafreval = 0,pll_value = 0,pll_value1 = 0,pll_value2 = 0;
	
	//getsystemcoreclock(&sysclock);
	//获取pll的分频值，然后根据分频值返回HSE的频率
	//HSI的频率一般为8M的频率
	//PLL一般等于systemcoreclock
	//AHB,APB1,APB2,先获取RCC->CFG value
	//然后用systemcoreclock计算，return value
	pll_frafremask = (PLL_FRAMASK & RCC->CFG); 
	//分频计算
	//将pll_frafremask的值与上0x6000 0000；把这个值右移25位
	//将pll_frafremask的值与上0x003c 0000；把这个值右移18位
	//然后将上的两个值相或，根据结果加一来确定分频系数。
	pll_value1 = pll_frafremask & 0x60000000;
	pll_value2 = pll_frafremask & 0x003c0000;
	
	if((pll_value = (pll_value1 | pll_value2)) == 0)
		pll_frafreval = 2;
	else if(pll_value < 50 && pll_value > 1)
		pll_frafreval = pll_value + 1;
	else
		pll_frafreval = pll_value - 1;
		
	switch(AHB_FRAMASK & RCC->CFG)
	{
		case SYSTEM_1:
			ahb_div = 1;
			break;
		case SYSTEM_2:
			ahb_div = 2;
			break;
		case SYSTEM_4:
			ahb_div = 4;
			break;			
		case SYSTEM_8:
			ahb_div = 8;
			break;
		case SYSTEM_64:
			ahb_div = 64;
			break;
		case SYSTEM_128:
			ahb_div = 128;
			break;
		case SYSTEM_256:
			ahb_div = 256;
			break;
		case SYSTEM_512:
			ahb_div = 512;
	}
	
		switch((APB1_FRAMASK & RCC->CFG) >> 8)
		{
			case HCLK_1:
				apb1_div = 1;
				break;
			case HCLK_2:
				apb1_div = 2;
				break;
			case HCLK_4:
				apb1_div = 4;
				break;
			case HCLK_8:
				apb1_div = 8;
				break;
			case HCLK_16:
				apb1_div = 16;
		}
		switch((APB2_FRAMASK & RCC->CFG) >> 11)
		{
			case HCLK_1:
				apb2_div = 1;
				break;
			case HCLK_2:
				apb2_div = 2;
				break;
			case HCLK_4:
				apb2_div = 4;
				break;
			case HCLK_8:
				apb2_div  = 8;
				break;
			case HCLK_16:
				apb2_div = 16;
		}
		
	switch(clock)
	{
		case pll:
		case systemclock:
			value = SystemCoreClock;
			break;
		case hsi:
			value = 8*1000*1000;
			break;
		case hse:
			value = SystemCoreClock / pll_frafreval;
		case lsi:
			value = 40*1000;
			break;
		case lse:
			value = 32.768*1000;
			break;
		case hclk:
			value = SystemCoreClock / ahb_div;
			break;
		case pclk1:
			value = SystemCoreClock / ahb_div / apb1_div;
			break;
		case pclk2:
			value = SystemCoreClock / ahb_div / apb2_div;
	}
			
	return value;
}
