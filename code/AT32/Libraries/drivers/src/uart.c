#include<uart.h>

USART_Type* uart_list[]={USART1,USART2,USART3,UART4,UART5};
char uart_rx_buf[UART_SIZE]= "";
uint16_t uart_rx_status;

void uart_init(uint32_t baud)
{
	GPIO_Init(HW_GPIOA,gpio_pin_9,gpio_speed_50MHz,mode_af_pp);
	GPIO_Init(HW_GPIOA,gpio_pin_10,gpio_speed_inno,mode_in_floating);
	USART_Init(HW_USART1,baud,wordlength_8,stopbits_1,parity_none,mode_tx | mode_rx,hardcontrol_no);
	USART_ITConfig(HW_USART1,uart_int_rdne,ENABLE);
	USART_Cmd(HW_USART1,ENABLE);
	
	NVIC_Init(USART1_IRQn,2,2,ENABLE);
}



void USART_Init(uint32_t uartnum,uint32_t baud,uint32_t wordlength,uint32_t stopbits,uint32_t parity,uint32_t mode,uint32_t hardcontrol)
{
	//��������ʱ��
	//���ô��ڼĴ���
	//CR1:ֹͣλ
	//CR2:�ֳ���ģʽ����żУ��
	//CR3:Ӳ��������
	switch(uartnum)
	{
		case HW_USART1:
			RCC->APB2EN |= RCC_APB2EN_USART1EN;
			break;
		case HW_USART2:
			RCC->APB1EN |= RCC_APB1EN_USART2EN;
			break;
		case HW_USART3:
			RCC->APB1EN |= RCC_APB1EN_USART3EN;
			break;
		case HW_UART4:
			RCC->APB1EN |= RCC_APB1EN_UART4EN;
			break;
		case HW_UART5:
			RCC->APB1EN |= RCC_APB1EN_UART5EN;
			break;
		default:
			break;
	}
	
	uint32_t tmpreg;

	tmpreg = uart_list[uartnum]->CTRL1;
  tmpreg &= CTRL1_MASK;
  tmpreg |= wordlength | parity | mode;
  
  uart_list[uartnum]->CTRL1 = (uint16_t)tmpreg;
	
  tmpreg = uart_list[uartnum]->CTRL2;
	tmpreg &= CTRL2_STOP_Mask;
	tmpreg |= stopbits;

  uart_list[uartnum]->CTRL2 = (uint16_t)tmpreg;

  tmpreg = uart_list[uartnum]->CTRL3;
  tmpreg &= CTRL3_Mask;
  tmpreg |= hardcontrol;
  uart_list[uartnum]->CTRL3 = (uint16_t)tmpreg;

 // set_brr(uartnum,baud);

	lib_set_brr(uartnum,baud);
	
}
void set_brr(uint32_t uartnum,uint32_t baud)
{
	uint32_t apbclock;
	uint32_t uartdiv;
	
	if(uartnum == HW_USART1)
		apbclock = getclock_frequency(pclk1);
	else
		apbclock = getclock_frequency(pclk2);

 
	uartdiv = ( apbclock / ((baud) << 4)) << 4;
	
	uart_list[uartnum]->BAUDR = uartdiv;
}

void lib_set_brr(uint32_t uartnum,uint32_t baud)
{
	uint32_t apbclock;
	uint32_t tmpreg,integerdivider,fractionaldivider;
	
	
	if(uartnum == HW_USART1)
		apbclock = getclock_frequency(pclk1);
	else
		apbclock = getclock_frequency(pclk2);
	

  /* Determine the integer part */
  
	integerdivider = ((25 * apbclock) / (4 * baud));
  
  tmpreg = (integerdivider / 100) << 4;

  /* Determine the fractional part */
  fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  /* Implement the fractional part in the register */
	tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
   /* Write to USART BRR */
	uart_list[uartnum]->BAUDR = (uint16_t)tmpreg;
}

void USART_ITConfig(uint32_t uartnum,uint16_t uart_interrupt,FunctionalState NewStatus)
{
	//������Ӧ���ж�
	//���Ȼ�ȡ���ڵĻ���ַ
	//Ȼ����ж�ֵ���θ߰�λ�����Ͱ�λ������λ��ȷ�����õļĴ���,
	//��������ȡ����λ��ֵ��ȷ�����õ�λ
	//Ȼ��ͨ������ַ��ƫ�Ƶ�ַ��ȷ�����Ĵ����ĵ�ַ������NewState������ĳһ��λ
	uint32_t uartxbase = 0;
	uint16_t tempreg = 0,setmov = 0,configbit = 0;
	
	uartxbase = (uint32_t)uart_list[uartnum];
	
	tempreg = (uart_interrupt & 0xff) >> 5;
	
	setmov = uart_interrupt & REGMASK;
	
	configbit = 1 << setmov;
	
	if(0x01 == tempreg)
		uartxbase += 0x0c;
	else if(tempreg == 0x02)
		uartxbase += 0x10;
	else
		uartxbase += 0x14;
	
	if(NewStatus == ENABLE)
		*(uint32_t *)uartxbase |= configbit;
	else
		*(uint32_t *)uartxbase &= ~configbit;
//uart_list[uartnum]->CTRL1 |= (1 << 5);

}

void USART_Cmd(uint32_t uartnum,FunctionalState NewStatus)
{
	//ʹ�ܴ���,����CR1�Ĵ����ĵ�ʮ��λ
	
	if(NewStatus == ENABLE)
		uart_list[uartnum]->CTRL1 |= UART_ENABLE;
	else
		uart_list[uartnum]->CTRL1 &= ~UART_ENABLE;
}


	
void USART1_IRQHandler(void)
{
	//��������
	//�жϽ��ܱ�־λ
	//�жϽ��ܵ����ݣ�0x0a 0x0d	
	u8 rev = 0;
	if(uart_getinter(HW_USART1,uart_int_rdne) == SET)
	{
		rev = uartrecvive_data(HW_USART1);
		if((uart_rx_status & UART_STATUS_NLINE) == RESET)
		{
			if(uart_rx_status & UART_STATUS_ENTER)
			{
				if(rev == 0x0a)
				{
					uart_rx_status |= UART_STATUS_NLINE;
				//	GPIO_PinToggle(HW_GPIOD,13);
				}
				else
					uart_rx_status = RESET;
			}
			else
			{
				if(rev == 0x0d)
					uart_rx_status |= UART_STATUS_ENTER;
				else
				{
					uart_rx_buf[uart_rx_status & UART_STATUS_MASK] = rev;
					uart_rx_status ++;
					GPIO_PinWrite(HW_GPIOD,gpio_pin_13,SET);
					if(uart_rx_status > UART_SIZE)
						uart_rx_status = RESET;
				}
			}
		}
	}
}

void log_uart(uint8_t uartnum,char *buf)
{
	u8 send_buf[UART_SIZE];
	uint32_t i;
	for(i=0;i<UART_SIZE;i++)
	{
		send_buf[i] = *buf++;
		if(send_buf[i] == '\0')
				i=UART_SIZE;
	}
	for(i=0;i<UART_SIZE;i++)
	{
		call_back_send(uartnum,send_buf[i]);
		if(send_buf[i] == '\0')
			i = UART_SIZE;
	}
	
}

void call_back_send(uint8_t uartnum,char ch)
{
	while((uart_list[uartnum]->STS &0x80) == 0)
		continue;
	uart_list[uartnum]->DT =(uint8_t) ch;
}

ITStatus uart_getinter(uint32_t uartnum,uint32_t uart_interrupt)
{
	//����ȷ���Ĵ���
	//Ȼ��ȷ�����õ��ж�
	//������ȷ����Ӧ�жϵı�־λ
	//ͨ��if������else��䷵��״̬
	uint16_t uartreg = 0,interbit = 0,uartinter = 0,uartflag = 0,flagmask = 0;
	
	uartreg = (uint8_t)uart_interrupt >> 0x05;
	interbit = uart_interrupt & REGMASK;
	interbit = 0x01 << interbit;
	
	if(0x01 == uartreg)
		uartinter = uart_list[uartnum]->CTRL1 & interbit;
	else if(0x02 == uartreg)
		uartinter = uart_list[uartnum]->CTRL2 & interbit;
	else
		uartinter = uart_list[uartnum]->CTRL3 & interbit;
	
	uartflag = uart_interrupt >> 0x08;
	flagmask = 0x01 << uartflag;
	flagmask &= uart_list[uartnum]->STS;
	
	if(uartinter != 0 && flagmask != 0)
		return SET;
	else
		return RESET;
}
	
uint16_t uartrecvive_data(uint32_t uartnum)
{
	return uart_list[uartnum]->DT & UART_DATA_MASK;
}



/***************************************************************/
/***************************************************************/
/***************************************************************/

char uart_read_buf[UART_SIZE] = "";
uint16_t uart_read_status;
static uint16_t uart_delayus;
static uint16_t uart_delayus_again;



void uart_virtual(uint32_t baud)
{
	uart_delayus = VIRTUAL_BAUD / baud;
	uart_delayus_again = uart_delayus / 2;
	uart1_io_config();
}

void uart_recvive_data(uint32_t uartnum)
{
	//return uart_list[uartnum]->DT & UART_DATA_MASK;
	//��ȡģ�⴮��ʱ��ķ�ʽ����ȡ����
	//PA9  ��������   writedata()
	//PA10 ��������   readdata()
	//�ú���λ���ܺ���������readdata()����������delayus()����
	//forѭ��
	int i;
	delayus(uart_delayus_again);
	
	for(i = 0;i < 8;i++)
	{
		//ÿ��ȡ��һλ��ֵ�������ŵ������С�
		//����Ĭ��LSB���ͣ�С�˴洢��
		//���͹�����������ASCII���ʽ
		delayus(uart_delayus);
		uart_read_buf[uart_read_status & UART_STATUS_MASK] |= (readdata(uartnum) << i);
	}
}

void uartsend_data(uint32_t uartnum)
{
	//����д�뺯��
	uint32_t i = 0;
	
	while(uart_read_buf[i] != '\0')
			virtual_serial(uartnum,uart_read_buf[i++]);
	
	
		
}
uint16_t  readdata(uint32_t uartnum)
{
	//��ȡPA10����
	switch(uartnum)
	{
		case HW_USART1:
			return read_gpioport(HW_GPIOA,gpio_pin_10);
		case HW_USART2:
		case HW_USART3:
		case HW_UART4:
		case HW_UART5:
			
			break;
	}
	return 0;
}

void virtual_serial(uint32_t uartnum,uint16_t data)
{
	//��װ��������
	//ģ�⴮�ڵ�ʱ��
	uint16_t send_data = 0,bit_data = 0,i;
	send_data = data;
	send_data &= 0xff;
	send_data |= 1 << 8;
	send_data <<= 1;
	for(i = 0;i < 10;i++)
	{
		bit_data = (send_data >>i) & 0x01;
		writedata(uartnum,bit_data);
		delayus(uart_delayus);
	}
}

void writedata(uint32_t uartnum,uint16_t data)
{
	//д��PA9����
	switch(uartnum)
	{
		case HW_USART1:
			GPIO_PinWrite(HW_GPIOA,gpio_pin_9,data);
			break;
		case HW_USART2:
		case HW_USART3:
		case HW_UART4:
		case HW_UART5:
			
			break;
	}
}





