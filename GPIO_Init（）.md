void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  	uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  	uint32_t tmpreg = 0x00, pinmask = 0x00;
  	/* Check the parameters */
  	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  	assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  	assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
	/*---------------------------- GPIO Mode Configuration -----------------------*/
  	currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
 	 if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
 	 { 
    		/* Check the parameters */
    		assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    		/* Output mode */
    		currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
 	 }
	/*---------------------------- GPIO CRL Configuration ------------------------*/
 	 /* Configure the eight low port pins */
  	if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  	{
   		 tmpreg = GPIOx->CRL;
    		for (pinpos = 0x00; pinpos < 0x08; pinpos++)
   		 {
     			 pos = ((uint32_t)0x01) << pinpos;
      			/* Get the port pins position */
      			currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      			if (currentpin == pos)
     			 {
        				pos = pinpos << 2;
       				/* Clear the corresponding low control register bits */
      				pinmask = ((uint32_t)0x0F) << pos;
       				tmpreg &= ~pinmask;
        				/* Write the mode configuration in the corresponding bits */
       				tmpreg |= (currentmode << pos);
        				/* Reset the corresponding ODR bit */
       				if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        				{
          					GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        				}
        				else
        				{
         					 /* Set the corresponding ODR bit */
          					if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
         					 {
            					GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          					}
        				}
      			}
   		 }
    		GPIOx->CRL = tmpreg;
  	}
	/*---------------------------- GPIO CRH Configuration ------------------------*/
  	/* Configure the eight high port pins */
 	if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
 	{
    		tmpreg = GPIOx->CRH;
    		for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    		{
      			pos = (((uint32_t)0x01) << (pinpos + 0x08));
      			/* Get the port pins position */
     			currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
     			if (currentpin == pos)
      			{
       				pos = pinpos << 2;
        				/* Clear the corresponding high control register bits */
        				pinmask = ((uint32_t)0x0F) << pos;
        				tmpreg &= ~pinmask;
        				/* Write the mode configuration in the corresponding bits */
       				tmpreg |= (currentmode << pos);
        				/* Reset the corresponding ODR bit */
       				if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
       				{
         					GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        				}
       				 /* Set the corresponding ODR bit */
        				if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        				{
          					GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
       				}
      			}
    		}
    		GPIOx->CRH = tmpreg;
  	}
}



��һ��һ�е�������

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)

���Ǻ���ͷ�������ͣ���һ����

uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
uint32_t tmpreg = 0x00, pinmask = 0x00��

�Զ���ı�������ʱ�Ȳ������ǵ����ã��������¿���

assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));

�������Ǵ�����֤��У�鴫�ݵĲ����Ƿ���Ч��

�ȿ�GPIOx����Ч��Χ��

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \

                                    ((PERIPH) == GPIOB) || \

                                    ((PERIPH) == GPIOC) || \

                                    ((PERIPH) == GPIOD) || \

                                    ((PERIPH) == GPIOE) || \

                                    ((PERIPH) == GPIOF) || \

                                    ((PERIPH) == GPIOG))

Ȼ��GPIO_Mode����Чѡ��

typedef enum

{ GPIO_Mode_AIN = 0x0,

  GPIO_Mode_IN_FLOATING = 0x04,

  GPIO_Mode_IPD = 0x28,

  GPIO_Mode_IPU = 0x48,

  GPIO_Mode_Out_OD = 0x14,

  GPIO_Mode_Out_PP = 0x10,

  GPIO_Mode_AF_OD = 0x1C,

  GPIO_Mode_AF_PP = 0x18

}GPIOMode_TypeDef;



#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_Mode_AIN) || ((MODE) == GPIO_Mode_IN_FLOATING) || \

                            ((MODE) == GPIO_Mode_IPD) || ((MODE) == GPIO_Mode_IPU) || \

                            ((MODE) == GPIO_Mode_Out_OD) || ((MODE) == GPIO_Mode_Out_PP) || \

                            ((MODE) == GPIO_Mode_AF_OD) || ((MODE) == GPIO_Mode_AF_PP))

�ٿ�GPIO_Pin��ѡ��Χ��

#define GPIO_Pin_0                 ((uint16_t)0x0001)  /*!< Pin 0 selected */

#define GPIO_Pin_1                 ((uint16_t)0x0002)  /*!< Pin 1 selected */

#define GPIO_Pin_2                 ((uint16_t)0x0004)  /*!< Pin 2 selected */

#define GPIO_Pin_3                 ((uint16_t)0x0008)  /*!< Pin 3 selected */

#define GPIO_Pin_4                 ((uint16_t)0x0010)  /*!< Pin 4 selected */

#define GPIO_Pin_5                 ((uint16_t)0x0020)  /*!< Pin 5 selected */

#define GPIO_Pin_6                 ((uint16_t)0x0040)  /*!< Pin 6 selected */

#define GPIO_Pin_7                 ((uint16_t)0x0080)  /*!< Pin 7 selected */

#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */

#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */

#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */

#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */

#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */

#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */

#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */

#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */

#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */



#define IS_GPIO_PIN(PIN) ((((PIN) & (uint16_t)0x00) == 0x00) && ((PIN) != (uint16_t)0x00))

��������һ�Ѵ��룬����֪����������ӣ��������е�IO�ڷֳ���7�飬ÿһ����15�����ţ�ÿһ�������а��ֹ���ģʽ������ÿһ�����Ŷ���Ψһ�ĵ�ַ��
�����ǲ����Ĵ�����ĳ��λʱ���Ĵ����������λ��Ӧ�ĵ�ַ������ĳ��GPIO���š�



#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */



#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)



#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)

#define GPIOB_BASE            (APB2PERIPH_BASE + 0x0C00)

#define GPIOC_BASE            (APB2PERIPH_BASE + 0x1000)

#define GPIOD_BASE            (APB2PERIPH_BASE + 0x1400)

#define GPIOE_BASE            (APB2PERIPH_BASE + 0x1800)

#define GPIOF_BASE            (APB2PERIPH_BASE + 0x1C00)

#define GPIOG_BASE            (APB2PERIPH_BASE + 0x2000)

#define GPIOA_BASE            (APB2PERIPH_BASE + 0x0800)



#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)   //0x40010800

#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)    //0x40010c00

#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)   //0x40011000
   
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)

#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)

#define GPIOF               ((GPIO_TypeDef *) GPIOF_BASE)

#define GPIOG               ((GPIO_TypeDef *) GPIOG_BASE)

ͨ��������Щ�궨�壬���ǿ���ȷ��ÿһ��GPIO���ŵĵ�ַ��

typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;   //�ṩһ��GPIO_TypeDef�Ľṹ�壬

���ſ���

currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

��һ�У���Ϊ����currentmode����һ��ֵ��

����һ�����ֵ�ķ�Χ��

�ȿ���GPIO_Mode��ȡֵ��Χ��

typedef enum

{ 	
	GPIO_Mode_AIN = 0x0,

  	GPIO_Mode_IN_FLOATING = 0x04,

  	GPIO_Mode_IPD = 0x28,

  	GPIO_Mode_IPU = 0x48,

  	GPIO_Mode_Out_OD = 0x14,

  	GPIO_Mode_Out_PP = 0x10,

  	GPIO_Mode_AF_OD = 0x1C,

  	GPIO_Mode_AF_PP = 0x18

}GPIOMode_TypeDef;

�������ö���壬����ȷ����GPIO_Mode��ȡֵ��Χ���ֱ��ǣ�

0x0,   0x04,   0x28,   0x48,   0x14,    0x10,     0x1c,    0x18,

��ô������ֵ���ֵ��

�⼸����ֵҪ��0x0f���룬����һ��������һ�£������봦һ��ʲôֵ��

0x0,    0x4,    0x8,    0x8,      0x4,      0x0,       0xc,       0x8

���Ƿ��֣�����֮��Ľ���Ǳ�������λ������λ�����㡣��ʲô�ã���ʱ��������������Ǽ������¿�


if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
{ 
	/* Check the parameters */
	assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
	/* Output mode */
	currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
}
����һ��if�����ж���䣬�������Ϊ��
((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00
��������λ��������λȫ�����㡣
ͨ���۲�GPIOMode_TypeDefö���壬���ֵ���λΪ1��ģʽ�������ģʽ��
���¿���������飺
assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
�ȿ���GPIO_Speed��ȡֵ��Χ��
typedef enum
{ 
	GPIO_Speed_10MHz = 1,
	GPIO_Speed_2MHz, 
	GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
					((SPEED) == GPIO_Speed_50MHz))


ͨ���鿴GPIOSpeed_TypeDefö���壬GPIO_Speed������ѡ��
�������������һ�䣺
currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
currentmode��ֵ����GPIO_Speed��ֵ��Ȼ�󸳸�currentmode���ó�һ������,ֻ�������ģʽ��,�Ż�����GPIO_Speed�����Ա.
GPIO_Speed:���������·����Ӧ�ٶȣ������Ϊ: ���������·�Ĵ�����һ��������·���Բ�ʧ���ͨ���źŵ����Ƶ��
��ϸ�Ľ���������https://blog.csdn.net/KantsWang/article/details/83022803
����������:

 if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    	tmpreg = GPIOx->CRL;
    	for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    	{
      		pos = ((uint32_t)0x01) << pinpos;
      		/* Get the port pins position */
      		currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      		if (currentpin == pos)
      		{
        			pos = pinpos << 2;
       			/* Clear the corresponding low control register bits */
        			pinmask = ((uint32_t)0x0F) << pos;
        			tmpreg &= ~pinmask;
        			/* Write the mode configuration in the corresponding bits */
        			tmpreg |= (currentmode << pos);
        			/* Reset the corresponding ODR bit */
        			if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
       			{
          				GPIOx->BRR = (((uint32_t)0x01) << pinpos);
       			}
        			else
       			{
          				/* Set the corresponding ODR bit */
          				if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          				{
            					GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          				}
        			}
      		}
    	}
    	GPIOx->CRL = tmpreg;
  }

��һ�κó�,�������ż�,����һ��һ��Ŀ�,�ȵ�һ��if����:

if(((uint32_t)GPIO_InitStruct->GPIO_Pin & (uint32_t)0x00FF)) != 0x00)

GPIO_Pin  ��  0x00FF����,�����Ͱ�λ,�߰�λ����,�ж�ͨ������:�������Ϊ0,�ͽ���if�������

tmpreg = GPIOx->CRL;

��ȡĳһGPIO���CRL�Ĵ�����Ϣ;   CRL��CRH���Ƕ˿����üĴ���,�������ŵĹ���ģʽ��Ƶ��.�������ݲ鿴STM32���Ĳο��ֲ�ĵڰ���,GPIO�Ĵ���˵��.

for (pinpos = 0x00; pinpos < 0x08; pinpos++)

forѭ�����,��������,���forѭ��,���Ҫִ�а˴�.

 pos = ((uint32_t)0x01) << pinpos;

��0x01  ���� pinposλ,Ȼ��ֵ��pos.������ʲô��,�����,�������¿�.

currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

��GPIO_Pin��pos����,Ȼ�󽫵õ��Ľ����ֵ��currentpin,���������ǻ����ܲµ�pos����;,����������ȡGPIO_Pin��
ֵ,�������ֵ���з�Χ��,����������pos�ķ�Χ֮��,�������0x00,

if (currentpin == pos)

�ж�current�ĸ�ֵ���,�����pos���,Ҳ����˵GPIO_Pin��ֵ�͵�ֵ���,��ʾ��ȡ��GPIO��������Ϣ

pos = pinpos << 2;

��pinpos��ֵ����2λ,��pinpos��ֵ�Ŵ�4��,Ȼ��ֵ��pos,��;��֪��,�������¿�:

pinmask = ((uint32_t)0x0F) << pos;

��һ��,�ǽ�0xF���ֵ����pos��λ,Ϊʲô������,���������˽�һ��GPIO_CRL��GPIO_CRH�������Ĵ����Ľṹ:

GPIO_CRL:����Ĵ��������˿����õͼĴ���GPIOx_CRL(x=A,B,C,D,E,F,G)
ÿ4��λ����һ��GPIO����.���ĸ�λ�ֳ���������:ǰ��λ������������������ģʽ,��Ϊ���а��ֹ���ģʽ,����λ������
ȷ���������������,��������,˳��ѹ���Ƶ��Speed��ȷ����.
������һ����������8��GPIO����.Ҳ���ǵͰ˸�GPIO����:0~7

GPIO_CRH:����Ĵ��������˿����ø߼Ĵ���GPIOx_CRH(x=A,B,C,D,E,F,G)
�����Ϻ�CRLһ��,ֻ��������Ĵ������õ��Ǹ߰˸�GPIO����:8~15

��Ϊһ��GPIO������16��:0~15;

���������������˼�ǰ���Ӧ��4��λȷ����,���潫��ȡ��.

tmpreg &= ~pinmask;

.��������pinmaskȡ����ֵ��tmpreg,����ֻ�ǽ������ĳ4λ���������ֵ��ֵ��tmpreg,��û���õ��Ĵ�����

�������¿�:

tmpreg |= (currentmode << pos);

���������ٻ�ͷ����currentmode�������:

currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

 currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;

����ô������ֵ���:�������currentmode�ϵ�ֵ�ǵ���λ��Ч,�ղ�����˵���õ�һЩ��ֵ���ֵ,�������ǰ���Ų��������һ��:

����ȷ����GPIO_Mode��ȡֵ��Χ���ֱ��ǣ�

0x0,   0x04,   0x28,   0x48,   0x14,    0x10,     0x1c,    0x18,

�⼸����ֵҪ��0x0f���룬����һ��������һ�£������봦һ��ʲôֵ��

0x0,    0x4,    0x8,    0x8,      0x4,      0x0,       0xc,       0x8

ͨ���۲�GPIO_CRL�Ĵ�����˵��,����֪��:��Щ��ֵ��ȷ�����ŵ�����,���Ƕ�������ģʽ,���������ǿ���֪��,����ģʽ�������ù���Ƶ��,

���������Ҫ����Ƶ�ʵ�,����:

 currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;

��仰������if���������ִ��,����GPIO_Mode�ĵ���λ�Ƿ�Ϊ1��ȷ���Ƿ�Ϊ���ģʽ.Ҳ����������仰:

if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)

������,�ص����Ǹղŷ��������:

tmpreg |= (currentmode << pos);

��currentmode����pos��λ,���pos����֪��,����ȷ��������Ҫ���õ�ĳһ��GPIO������,

������仰�Ĺ�����:��ĳһ��GPIO���ŵ�������Ϣ��ֵ��tmpreg;ǰ����,��Ӧ��λ����������,���Բ������⼸�仰:

	pos = pinpos << 2;
        	pinmask = ((uint32_t)0x0F) << pos;
        	tmpreg &= ~pinmask;

���ſ�:

if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
{
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
}
else
{
          /* Set the corresponding ODR bit */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
}

���һ��if.....else.....���:

�����˼��������������ģʽ��,���ǻ���Ҫϸ��,���������������������Ĵ���:GPIOx_BRR��GPIOx_BSRR�Ĵ���,�����Ȱ��������Ĵ���
����һ��,��˵����,
������GPIOx_BRR:�˿�λ����Ĵ���(GPIOx_BRR,x=A,B,C,D,E,F,G)
            ֻ�õ�����0~15��λ,��16λΪ����,��Щλֻ��д�벢ֻ����16λ��ֵ���в���
         ������:Ϊ0ʱ,�Զ�Ӧ��ODRyλ������Ӱ��.(�ð�,�ֳ�����ODR�Ĵ���,���Ͽ���)
                   Ϊ1ʱ,�����Ӧ��ODRyλΪ0;
     ����:GPIOx_ODR:�˿�������ݼĴ���(GPIOx_ODR,x=A,B,C,D,E,F,G)
            ��16λ[31:16]:����
             ��16λ[15:0]:��Щλ�ɶ���д��ֻ����16λ����ֵ���в���.
 Ȼ����GPIOx_BSRR:�˿�λ����/����Ĵ���(GPIOx_BSRR,x=A,B,C,D,E,F,G)
            ��16λ[31:16]:����˿�x��λ,��Щλֻ��д�벢ֻ����16λ����ֵ�Ľ��в���
                  0:�Զ�Ӧ��ODRλ������Ӱ��
                  1:�����Ӧ��ODRλΪ0.
            ��16λ[15:0]:�˿�����x��λ,��Щλֻ��д�벢ֻ����16λ����ֵ���в���.
                   0:�Զ�Ӧ��ODRλ������Ӱ��
                   1:������Ӧ��ODRλΪ1
                  ע:���ͬʱ�����˸�16λ�͵�16λ,��ô��16λ������.
���ڻص�����Ĵ���:
  �����˽⵽������ģʽ��,���������������ģʽ,��ô���ǿ���ͨ��ODR�Ĵ����ж�Ӧ��λ,ȷ�����ž������������뻹����������.
          ��ODR��Ӧ��λΪ1ʱ,Ϊ��������,���Ϊ0,������������.

���һ��:

GPIOx->CRL = tmpreg;

��tmpreg��ֵ��GPIOx->CRL,������Ӧ��GPIO����.

������,��һ�δ���ͷ�������,��������CRL��,���滹��CRH�Ĵ���:

 if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* Get the port pins position */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* Clear the corresponding high control register bits */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* Write the mode configuration in the corresponding bits */
        tmpreg |= (currentmode << pos);
        /* Reset the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* Set the corresponding ODR bit */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

���Ǵ�ſ���һ��,���˵�һ��if�Ľ���������һ��,������һģһ��.�������ﲻ��һ��һ��ķ�����.
���������������һ����if�������:

 if (GPIO_InitStruct->GPIO_Pin > 0x00FF)

��GPIO_Pin��ֵ����0x00FFʱ���ܽ���if���.
Ҳ����GPIO���ŵĸ�8������:8~15��.

#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */

#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */

#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */

#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */

#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */

#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */

#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */

#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */

#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

�������ǿ�������и�GPIO_Pin_All������,Ҳ�Ǵ���0x00FF,��ô���ǲ���Ҳ�ܽ��뵽���if������.����������ôһ�δ���:

assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin)); //�������

#define IS_GET_GPIO_PIN(PIN) (((PIN) == GPIO_Pin_0) || \
                              ((PIN) == GPIO_Pin_1) || \
                              ((PIN) == GPIO_Pin_2) || \
                              ((PIN) == GPIO_Pin_3) || \
                              ((PIN) == GPIO_Pin_4) || \
                              ((PIN) == GPIO_Pin_5) || \
                              ((PIN) == GPIO_Pin_6) || \
                              ((PIN) == GPIO_Pin_7) || \
                              ((PIN) == GPIO_Pin_8) || \
                              ((PIN) == GPIO_Pin_9) || \
                              ((PIN) == GPIO_Pin_10) || \
                              ((PIN) == GPIO_Pin_11) || \
                              ((PIN) == GPIO_Pin_12) || \
                              ((PIN) == GPIO_Pin_13) || \
                              ((PIN) == GPIO_Pin_14) || \
                              ((PIN) == GPIO_Pin_15))               //����ϸ�ķ�Χ,�����Χ�в�û��GPIO_Pin_All�����Ա,����,���ǽ���ȥ��.


���δ��뵽����ͽ�����.
