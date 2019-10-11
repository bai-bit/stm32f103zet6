#include<hw_systick.h>
#include<led.h>
#include<uart.h>
#include<getclock.h>


extern u8 uart_rx_buf[UART_SIZE];
extern uint16_t uart_rx_status;

int main(int argc,const char *argv[])
{
	//初始化延时函数
	//初始化led
	//优先级分组
	//初始化串口
	//while(1);
	uint32_t clock;
	char *string;
	clock = getclock_frequency(pclk1);
	Delayinit(SYSTICK_SOURCE);
	systick_setexception(true);
	led_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	uart_init(115200);
	
	while(1)
	{
		delayms(100);
		if(uart_rx_status & 0x8000)
		{
			string = itoa(clock);
			log_uart(HW_USART1,string);
			//printf("%d\n",clock);
			uart_rx_status = 0;
		}
//		GPIO_PinWrite(HW_GPIOD,13,0);
//		delayms(500);
//		GPIO_PinWrite(HW_GPIOD,13,1);
//		delayms(500);
	}
	
		
}	

