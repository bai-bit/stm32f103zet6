#include<hw_systick.h>

static uint32_t fac_us1 = 0;

void Delayinit(uint32_t systicknum)
{
	//�����ڲ�ʱ��Դ��Ƶ��
	//��ʼ��systick��ʱ��
	//ѡ��ʱ��Դ
	//�ô�����ڲ�ʱ��Դ,�����ms��us��������
	//��ʼ��fac_ms,fac_us.
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk ;
	fac_us1 = systicknum / 1000000;
}

static volatile int test;
void delayms(uint32_t num)
{
	//������װ�ؼĴ���ֻ��24λ����������,���Դ����������̫��,
	//�����������ݹ���,��Ҫ�ֶ��д����װ�ؼĴ���.
	//�������ֵ
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
	//�Ѵ������ʱʱ�����fac_usд����װ�ؼĴ���
	//������ʱ��
	//��do...while()��������ѭ�����жϿ��ƼĴ����ĵ�ʮ��λ
	//����ʮ��λΪ1ʱ,�رն�ʱ��.
	//��������������.
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
	//��ӡ����ֵ
	char *str = 0;
	static uint32_t count = 0;
	count++;
	str = itoa(count);
	log_uart(HW_USART1,str);
}


