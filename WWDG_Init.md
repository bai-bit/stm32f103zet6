void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_CNT = tr & WWDG_CNT;
	
	WWDG_SetPrescaler(fprer);
	WWDG_SetWindowValue(wr);
	WWDG_ClearFlag();
	WWDG_NVIC_Init();
	WWDG_EnableIT();
	WWDG_Enable(WWDG_CNT);
}


����һ�����ڿ��Ź��ĳ�ʼ������.���ȿ���һ����������:

RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);

��Ϊ���ڿ��Ź�ʹ�õ�ʱ��������ʱ��,��������Ҫʹ����Ӧ��ʱ��,
���������Ź�ֻ�õĵ��ٵ��ڲ�ʱ��,ֻҪоƬһ�ϵ�,�Զ�����ʱ��,����Ҫͨ�������ʹ��.

�ڶ��д���:
WWDG_CNT = tr & WWDG_CNT;

���д����Ǹ�ֵ,����WWDG_CNT��һ��ȫ�ֱ���,��������ֵ���Ա���������ʹ��.

tr�������Լ����õļ���ֵ,ͨ��������,�����ݹ����ļ���ֵ����һ��ȫ�ֱ���.����������ʹ��.

�����д���:

WWDG_SetPrescaler(fprer);

���Ǹ���������,����ȥ׷����ԭ��:

void WWDG_SetPrescaler(uint32_t WWDG_Prescaler)
{
  uint32_t tmpreg = 0;
  /* Check the parameters */
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));
  /* Clear WDGTB[1:0] bits */
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;
  /* Set WDGTB[1:0] bits according to WWDG_Prescaler value */
  tmpreg |= WWDG_Prescaler;
  /* Store the new value */
  WWDG->CFR = tmpreg;
}

����Ǵ���У��:

 assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));

ͨ����,���ǿ���֪��Ԥ��Ƶֵ���з�Χ��:

#define WWDG_Prescaler_1    ((uint32_t)0x00000000)
#define WWDG_Prescaler_2    ((uint32_t)0x00000080)
#define WWDG_Prescaler_4    ((uint32_t)0x00000100)
#define WWDG_Prescaler_8    ((uint32_t)0x00000180)
#define IS_WWDG_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Prescaler_1) || \
                                      ((PRESCALER) == WWDG_Prescaler_2) || \
                                      ((PRESCALER) == WWDG_Prescaler_4) || \
                                      ((PRESCALER) == WWDG_Prescaler_8))

���ݹ����Ĳ������з�Χ��,�������ĸ�ֵ.

��һ�����:

  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;

#define CFR_WDGTB_Mask    ((uint32_t)0xFFFFFE7F)

ͨ������궨��,����֪��,��������������ĳ���Ĵ����ĵ���λ�͵ڰ�λ.

#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)

#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)

#define APB1PERIPH_BASE       PERIPH_BASE

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

ͨ��������⼸�����,����֪����WWDG�������ַ.

typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFR;
  __IO uint32_t SR;
} WWDG_TypeDef;

���Ǹ��ṹ��,�����������Ա����.���������Ĵ���
����ͨ���鿴�ֲ�:
WWDG_CR:���ƼĴ���
	[31:8]����
	[7]:WDGA,����λ,
		0:��ֹ���Ź�
		1:���ÿ��Ź�
	[6:0]:7λ������,
		Ϊ�ݼ�������,����������ֵ��40h���3fhʱ,�������Ź���λ.
WWDG_CFR:���üĴ���
	[31:10]����
	[9] :EWI��ǰ�����ж�.
		����Ϊ��1,�򵱼�����ֵ�ﵽ40hʱ,�������ж�.
	[8:7];WDGTB[1:0]:ʱ��
		Ԥ��Ƶ��,00:CK��ʱ��ʱ��(PCLK1����4096)����1
			01:CK��ʱ��ʱ��(PCLK1����4096)����2
			10:CK��ʱ��ʱ��(PCLK1����4096)����4
			11:CK��ʱ��ʱ��(PCLK1����4096)����8
	[6:0]:W[6:0],7λ����ֵ
		�⼸��λ���һ������ֵ,������ݼ����������бȽ�.
WWDG_SR:״̬�Ĵ���
	[31:1]����
	[0]:EWIF,��ǰ�����жϱ�־
		��������ֵ�ﵽ40hʱ,��λ��Ӳ����1,������ͨ�����д0�����.�Դ�λд1��Ч.���ж�δ��ʹ��,��λҲ�ᱻ��1.

����������ĺ���:��ȡ��ǰ�üĴ�������ֵ,������λ�͵ڰ�λ��0.Ȼ��ֵ��tmpreg.

��һ�����:
tmpreg |= WWDG_Prescaler;

��һ�����:
 WWDG->CFR = tmpreg;

����������ǰ������Զ������ֵд��CFR�Ĵ���.���ô��ڿ��Ź���Ԥ��Ƶֵ.

����������һ����������:

WWDG_SetWindowValue(wr);

���ǰ�����ԭ��׷����:

void WWDG_SetWindowValue(uint8_t WindowValue)
{
  __IO uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));
  /* Clear W[6:0] bits */

  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* Set W[6:0] bits according to WindowValue value */
  tmpreg |= WindowValue & (uint32_t) BIT_Mask;

  /* Store the new value */
  WWDG->CFR = tmpreg;
}

������������������ô���ֵ,�������ֵ�������Ǽ�������ֵ�ʹ���ֵ���бȽ�.����ֵ�������0x40.�����û�д���.
����������ֵ���ڴ���ֵ��ʱ��,����WWDG_CR�Ĵ�����ʱ���,������λ�ź�.
����������ֵС�ڴ���ֵ��ʱ��,����WWDG_CR�Ĵ�����ʱ��,�����Զ������Ϊ��������.

assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));

#define IS_WWDG_WINDOW_VALUE(VALUE) ((VALUE) <= 0x7F)

����ֵ�ķ�Χ0~111 1111

 tmpreg = WWDG->CFR & CFR_W_Mask;

��ȡ��ǰ����ֵ.

tmpreg |= WindowValue & (uint32_t) BIT_Mask;

#define BIT_Mask          ((uint8_t)0x7F)

��ֵ����,�Ѵ��ݹ����Ĳ�����0x7F����,�������ֵС��0x7F,�ͰѴ��ݹ����Ĵ���ֵ����tmpreg.
�������0x7F,��������һ���ͱ���,�����һ��ʧЧ,��ô����һ��,��0x7F����tmpreg.

  WWDG->CFR = tmpreg;

���µĵĴ���ֵд��WWDR_CFR�Ĵ���.

����������һ����������:

WWDG_ClearFlag();

׷�麯��ԭ��:

void WWDG_ClearFlag(void)
{
  WWDG->SR = (uint32_t)RESET;
}

��������ǳ���,��һ�仰.��WWDG_SR�Ĵ���д��RESET.

���ǿ�������궨��:

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

��ʾд��0����д��1.��һ�仰��д��0,����жϱ�־λ.

��һ����������:

WWDG_NVIC_Init();

����ԭ��:
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //?��??2��?������??��??3��?����2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //?��??2��?������??��??3��?����2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC3?��??��
}

���ڿ��Ź��жϳ�ʼ������.�ж�ͨ����WWDG_IRQx.ͨ����Ӧ��ͨ����,���ö�Ӧ���жϴ�����.

��һ����������:

WWDG_EnableIT();

����ԭ��:

void WWDG_EnableIT(void)
{
  *(__IO uint32_t *) CFR_EWI_BB = (uint32_t)ENABLE;
}

�鿴����궨��:

#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

#define PERIPH_BB_BASE        ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region */

#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define APB1PERIPH_BASE       PERIPH_BASE
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

ͨ��������һϵ�еĺ궨��,����֪��,�⼸����λ������,ֱ�Ӳ���ĳһ��λ,
ͨ������,�ó�,��һ��ʵ���Ͼ����ڲ���WWDG_CFR�Ĵ����ĵھ�λ.����һλ��һ,ʹ����ǰ�����ж�.

��һ����������:

WWDG_Enable(WWDG_CNT);

�鿴����ԭ��:

void WWDG_Enable(uint8_t Counter)
{
  /* Check the parameters */
  assert_param(IS_WWDG_COUNTER(Counter));
  WWDG->CR = CR_WDGA_Set | Counter;
}

�鿴����У��:

#define IS_WWDG_COUNTER(COUNTER) (((COUNTER) >= 0x40) && ((COUNTER) <= 0x7F))

У�鴫�ݵĲ�����Χ,����0x40,С��0x7F.���ֵ�Ǽ���ֵ,�������Զ���ļ���ֵ,���ʹ���ֵ��������.

��һ��:

WWDG->CR = CR_WDGA_Set | Counter;

�ȿ����궨��:

#define CR_WDGA_Set       ((uint32_t)0x00000080)

����8λ��1,ͨ���鿴�ֲ�,����֪������Ĵ����ĵڰ�λ��ʹ�ܴ��ڿ��Ź�.