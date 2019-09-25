static u8  fac_us=0;									   
static u16 fac_ms=0;


void delay_init()
{
#if SYSTEM_SUPPORT_OS  							//判断是否支持OS系统
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;		//设置为系统时钟的1/8  
#if SYSTEM_SUPPORT_OS  				//判断是否支持OS系统.
	reload=SystemCoreClock/8000000;		// 每us计数次数
	reload*=1000000/delay_ostickspersec;		//
						
	fac_ms=1000/delay_ostickspersec;		//	   

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启systick中断
	SysTick->LOAD=reload; 			//设置中断的时间间隔
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启systick定时器

#else
	fac_ms=(u16)fac_us*1000;			//非OS模式下，每ms，需要的systick时钟数  
#endif
}



void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//设置LOAD的计数值	  		 
	SysTick->VAL=0x00;        					//设置VAL的初值
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;			//打开systick定时器	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (1 << 16)));		//判断条件：定时器开启，COUNTFLAG位为0，表示systick定时器还没有计数到0   
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;	//关闭定时器
	SysTick->VAL =0X00;      					 //将计数初值设置为0	 
}

void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//设置LOAD的计数值，这个值为要延迟的毫秒数
	SysTick->VAL =0x00;				//设置计数初值
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;		//打开systick定时器 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//判断条件：定时器开启，COUNTFLAG位为0，表示systick定时器还没有计数到0.   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;		//关闭systick定时器
	SysTick->VAL =0X00;       				//将计数初值设置为0.	  	    
} 