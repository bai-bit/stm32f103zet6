static uint32_t fac_us1;
volatile static uint32_t fac_ms1;


void Delayinit(uint32_t systicknum)
{
	//�����ں�ʱ��Դ��Ƶ��
	//�ں�ʱ��????
	//��ʼ��systick��ʱ��
	//ѡ��ʱ��Դ
	//�ô�����ڲ�ʱ��Դ,�����us��������
	//��ʼ��fac_us.
	SysTick->CTRL |= SysTick_CLKSource_HCLK;
	fac_us1 = systicknum / 1000000;
	fac_ms1 = fac_us1 * 1000;
}

void delayms(uint32_t num)
{
	//������װ�ؼĴ���ֻ��24λ����������,���Դ����������̫��,
	//�����������ݹ���,��Ҫ�ֶ��д����װ�ؼĴ���.
	//Ϊ���ܹ�ʵ����ʱ����ʱ��,����ʹ��whileѭ����������ʱ
	uint32_t temp = 0;
	uint32_t i;
	
//	SysTick->LOAD = num * fac_msl;
//	SysTick->VAL = 0;
//	SysTick->CTRL |= 1;
//	do{
//		 temp = SysTick->CTRL;
//  }while((temp & 0x01) &&! (temp & (1<<16)));
	SysTick->LOAD = fac_us1 *1000;             //�������һ������,���ʹ��fac_ms1������һ��ֵ�ͳ���bug,���ǲ���fac_us1 * 1000,���ܷ���Ҫ��.
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
	//�Ѵ������ʱʱ�����fac_usд����װ�ؼĴ���
	//������ʱ��
	//��do...while()��������ѭ�����жϿ��ƼĴ����ĵ�ʮ��λ
	//����ʮ��λΪ1ʱ,�رն�ʱ��.
	//��������������.
	uint32_t temp = 0;
	
	SysTick->LOAD = num * fac_us1;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		 temp = SysTick->CTRL;
  }while((temp & 0x01) &&! (temp & (1<<16)));
	
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//�ú�������ʵ���ӳ�����
//ʵ�ʾ��ǵ��ú����ӳٺ���
void dalaysec(uint32_t num)
{
	num *= 1000;
	delayms(num);
}
