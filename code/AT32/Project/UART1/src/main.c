#include<hw_systick.h>
#include<led.h>
#include<uart.h>
#include<getclock.h>


extern u8 uart_rx_buf[UART_SIZE];
extern uint16_t uart_rx_status;

extern u8 uart_read_buf[UART_SIZE];
extern uint16_t uart_read_status;
int main(int argc,const char *argv[])
{
	//��ʼ����ʱ����
	//��ʼ��led
	//���ȼ�����
	//��ʼ������
	//while(1);
	uint32_t clock;
	char *string;
	clock = getclock_frequency(pll);
	Delayinit(SYSTICK_SOURCE);
	//systick_setexception(true);
	led_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 //	uart_init(115200);
	uart_virtual(115200);

	while(1)
	{
	//	delayms(100);
//		if(uart_rx_status & 0x8000)
//		{
//			string = itoa(clock);
//			log_uart(HW_USART1,string);
//			//printf("%d\n",clock);
//			uart_rx_status = 0;
//			uart_read_status = 0;
//		}
//		GPIO_PinWrite(HW_GPIOD,13,0);
//		delayms(500);
//		GPIO_PinWrite(HW_GPIOD,13,1);
//		delayms(500);
	
		if(uart_read_status & UART_STATUS_NLINE)
		{
			 	uartsend_data(HW_USART1);
				uart_read_status = RESET;
//			GPIO_PinWrite(HW_GPIOA,9,0);
//		  delayms(500);
//		  GPIO_PinWrite(HW_GPIOA,9,1);
//			delayms(500);
		}
	}
	
		
}	
