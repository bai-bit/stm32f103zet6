//外部中断
#include<exti.h>

extern u8 uart_read_buf[UART_SIZE];
extern uint16_t uart_read_status;

void exti_init(uint32_t exti_line,EXTI_MODE exti_mode,TRIGGER_MODE trigger_mode,FunctionalState NewStatus)
{
	//使能某个中断线
	//配置触发方式
	if(NewStatus)
	{
		exti_mode ? (EXTI->EVTEN |= SET << exti_line) : (EXTI->INTEN |= SET << exti_line);
		if(trigger_mode == failling)
			EXTI->FTRSEL |= SET << exti_line;
		else if(trigger_mode == rising)
			EXTI->RTRSEL |= SET << exti_line;
		else
		{
			EXTI->FTRSEL |= SET << exti_line;
			EXTI->RTRSEL |= SET << exti_line;
		}
	}
	else
		exti_mode ? (EXTI->INTEN &= ~(SET << exti_line)) : (EXTI->EVTEN &= ~(SET << exti_line));
	
}



void uart1_io_config(void)
{
	//使能GPIO口
	//使能AFIO时钟
	//配置AFIO,EXTIC的寄存器，外部中断输入源
	//初始化外部中断
	//使能外部中断
	GPIO_Init(HW_GPIOA,gpio_pin_9,gpio_speed_50MHz,mode_out_pp);
	
	GPIO_Init(HW_GPIOA,gpio_pin_10,gpio_speed_inno,mode_in_floating);
	AFIO_Init(HW_GPIOA,gpio_pin_10);
	exti_init(exti_line10,interrupt,failling,ENABLE);
	NVIC_Init(EXTI15_10_IRQn,2,3,ENABLE);
}


void EXTI15_10_IRQHandler(void)
{
	//判断中断线
	if(get_extiinter(exti_line10) == SET)
	{
		//读数据
		//判断发送结束标志
		if((uart_read_status & UART_STATUS_NLINE) == RESET )
		{
			if(uart_read_status & UART_STATUS_ENTER)
					if(uart_read_buf[(uart_read_status & UART_STATUS_MASK) - 1] == 0x0a)
						uart_read_status |= UART_STATUS_NLINE;
					else
						uart_read_status = RESET;
			else if(uart_read_buf[(uart_read_status & UART_STATUS_MASK) - 1] == 0x0d)
				uart_read_status |= UART_STATUS_ENTER;
			else
			{
					uart_recvive_data(HW_USART1);
					uart_read_status++;
				if((uart_read_status & UART_STATUS_MASK) > UART_SIZE)
					uart_read_status = RESET;
			}
		}
		clean_extiinter(exti_line10);
	}
}

FlagStatus get_extiinter(uint32_t exti_line)
{
	return (EXTI->PND | (0x02 << exti_line))? SET : RESET;
}

void clean_extiinter(uint32_t exti_line)
{
	EXTI->PND |= 0x02 << exti_line;
}
