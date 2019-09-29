static uint32_t fac_us1;
volatile static uint32_t fac_ms1;


void Delayinit(uint32_t systicknum)
{
	//传入内核时钟源的频率
	//内核时钟????
	//初始化systick定时器
	//选择时钟源
	//用传入的内部时钟源,计算出us的周期数
	//初始化fac_us.
	SysTick->CTRL |= SysTick_CLKSource_HCLK;
	fac_us1 = systicknum / 1000000;
	fac_ms1 = fac_us1 * 1000;
}

void delayms(uint32_t num)
{
	//由于重装载寄存器只有24位来保存数据,所以传入的数不能太大,
	//如果传入的数据过大,需要分多次写入重装载寄存器.
	//为了能够实现延时超长时间,我们使用while循环来进行延时
	uint32_t temp = 0;
	uint32_t i;
	
//	SysTick->LOAD = num * fac_msl;
//	SysTick->VAL = 0;
//	SysTick->CTRL |= 1;
//	do{
//		 temp = SysTick->CTRL;
//  }while((temp & 0x01) &&! (temp & (1<<16)));
	SysTick->LOAD = fac_us1 *1000;             //这里存在一个问题,如果使用fac_ms1这样的一个值就出现bug,但是采用fac_us1 * 1000,就能符合要求.
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while(num--)
	{
		do{
			temp = SysTick->CTRL;
		}while((temp & 1) && (SysTick->VAL != 0));
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
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		 temp = SysTick->CTRL;
  }while((temp & 0x01) &&! (temp & (1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//该函数用来实现延迟秒数
//实际就是调用毫秒延迟函数
void dalaysec(uint32_t num)
{
	num *= 1000;
	delayms(num);
}
