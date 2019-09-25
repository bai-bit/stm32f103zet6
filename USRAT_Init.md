
	  
#if 1
#pragma import(__use_no_semihosting)         //�ر�ʹ�ð�����ģʽ      
                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
 
_sys_exit(int x)                                         //�����ڲ�������ж�ʱ���������ģʽ
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)                           //���¶���fputc������ʹ�ô���send����
{      
	while((USART1->SR&0X40)==0);  
    	USART1->DR = (u8) ch;      
	return ch;
}
#endif 


 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
#if EN_USART1_RX   

 	
u8 USART_RX_BUF[USART_REC_LEN];    


u16 USART_RX_STA=0;     	  
  
void uart_init(u32 bound)
{

  	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	
  
	//USART1_TX   GPIOA.9
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  	//USART1_RX	  
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);

  
 	 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	
  
  

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1��
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

  	USART_Init(USART1, &USART_InitStructure);
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 	USART_Cmd(USART1, ENABLE);                   

}

void USART1_IRQHandler(void)                	
{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
	{
		Res =USART_ReceiveData(USART1);	
		
		if((USART_RX_STA&0x8000)==0)
		{
			if(USART_RX_STA&0x4000)
			{
				if(Res!=0x0a)
					USART_RX_STA=0;
				else 
					USART_RX_STA|=0x8000;	
			}
			else
			{	
				if(Res==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;  
				}		 
			}
		}   		 
    	 } 
#if SYSTEM_SUPPORT_OS 	
	OSIntExit();  											 
#endif
} 
#endif	


����Ĵ���������������:
	int fputc(int ch,FILE *f);
	void uart_init(u32 bound);
	void USART1_IRQHandler(void);
����������˵�ڶ�������:
	void uart_init(u32 bound);

�����ǳ�ʼ������,��������һ��һ������ǹ���,������:

	GPIO_InitTypeDef             GPIO_InitStructure;
	USART_InitTypeDef           USART_InitStructure;
	NVIC_InitTypeDef              NVIC_InitStructure;

һ����,���ȴ����������ṹ��:�������������Ƕ��Ǹ�ɶ��:'

��һ��:GPIO_InitTypeDef             GPIO_InitStructure;
  ��������GPIO��������Ϣ;

typedef struct
{
 	 uint16_t GPIO_Pin;            			 /*!< Specifies the GPIO pins to be configured.
                                      				This parameter can be any value of @ref GPIO_pins_define */

  	GPIOSpeed_TypeDef GPIO_Speed;  		/*!< Specifies the speed for the selected pins.
                                      				This parameter can be a value of @ref GPIOSpeed_TypeDef */

  	GPIOMode_TypeDef GPIO_Mode;    		/*!< Specifies the operating mode for the selected pins.
                                      				This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;


�ڶ���:USART_InitTypeDef           USART_InitStructure;
  �������Ǵ��ڵ�������Ϣ;

typedef struct
{
 	 uint32_t USART_BaudRate;           	//������	 /*!< This member configures the USART communication baud rate.
                                         			 	 The baud rate is computed using the following formula:
                                         			   	- IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                           			 	- FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

	  uint16_t USART_WordLength;             //�ֳ� 	/*!< Specifies the number of data bits transmitted or received in a frame.
                                          			 	This parameter can be a value of @ref USART_Word_Length */

 	 uint16_t USART_StopBits;          	//ֹͣλ  	/*!< Specifies the number of stop bits transmitted.
                                          			 	This parameter can be a value of @ref USART_Stop_Bits */

 	 uint16_t USART_Parity;              //��żУ��	/*!< Specifies the parity mode.
                                          				 This parameter can be a value of @ref USART_Parity
                                          			 	@note When parity is enabled, the computed parity is inserted
                                                 			at the MSB position of the transmitted data (9th bit when
                                                 			the word length is set to 9 data bits; 8th bit when the
                                                		 	word length is set to 8 data bits). */
 
 	 uint16_t USART_Mode;                //����ģʽ	/*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           				This parameter can be a value of @ref USART_Mode */

  	uint16_t USART_HardwareFlowControl; 	//Ӳ��������	/*!< Specifies wether the hardware flow control mode is enabled
                                           					or disabled.
                                           					This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitTypeDef;


������:NVIC_InitTypeDef              NVIC_InitStructure;
   ���������жϷ����������Ϣ;

typedef struct
{
 	 uint8_t NVIC_IRQChannel;                   	 /*!< Specifies the IRQ channel to be enabled or disabled.
                                                  			 This parameter can be a value of @ref IRQn_Type 
                                                  			 (For the complete STM32 Devices IRQ Channels list, please
                                                   			 refer to stm32f10x.h file) */

  	uint8_t NVIC_IRQChannelPreemptionPriority;  	/*!< Specifies the pre-emption priority for the IRQ channel
                                                   			specified in NVIC_IRQChannel. This parameter can be a value
                                                  			 between 0 and 15 as described in the table @ref NVIC_Priority_Table */

  	uint8_t NVIC_IRQChannelSubPriority;        	 /*!< Specifies the subpriority level for the IRQ channel specified
                                                 			  in NVIC_IRQChannel. This parameter can be a value
                                                  			 between 0 and 15 as described in the table @ref NVIC_Priority_Table */

  	FunctionalState NVIC_IRQChannelCmd;        	 /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                  			 will be enabled or disabled. 
                                                  			 This parameter can be set either to ENABLE or DISABLE */   
} NVIC_InitTypeDef;


������һ�����������ṹ��ĳ�Ա���.����������:

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

��仰�Ĺ�����ʹ����Ӧ��ʱ��.ֻ�а���Ӧ��ʱ��ʹ����,���ǲ���ʹ�ö�Ӧ������.�������ָ�����,Ϊʲôû��ʹ��AFIO��ʱ��
��Ϊ����,�Ǳ���Ҫʹ�ö˿ڸ��ù��ܵ�.
������ȥ���ϲ���һ��,AFIOʱ�ӵĿ���������,����ĳ�����Ҫ����AFIOʱ����Ӧ�ļĴ���ʱ,��ȥ������.�������AFIO�Ĵ�����������,
���ǾͲ��ÿ�����.
�����������������û�ж�AFIO�ļĴ�����������,�������ǲ��ö������п�������.������ǵĴ���1,��Ҫ��Ĭ�ϵ�PA9��PA10ӳ�䵽����������,
��ʱ,���Ǿ���Ҫ����AFIOʱ����,��Ϊ������Ҫ����AFIO_MAPR����Ĵ���,������1���շ�������ӳ�䵽��������:PB6��PB7


������������Ĵ���,����������������ô������Ӧ��ʱ�ӵ�:

void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  	/* Check the parameters */
  	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  	assert_param(IS_FUNCTIONAL_STATE(NewState));
  	if (NewState != DISABLE)
  	{
    		RCC->APB2ENR |= RCC_APB2Periph;
  	}
  	else
  	{
    		RCC->APB2ENR &= ~RCC_APB2Periph;
 	}	
}

��������������:
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  	assert_param(IS_FUNCTIONAL_STATE(NewState));

�������ȿ������ǵ�ȡֵ��Χ��:

	#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFC00002) == 0x00) && ((PERIPH) != 0x00))

����仰,���ǿ���֪��RCC_APB2Perigh��ȡֵ��Χ��1~(~0xFFC00002);�Ҳ�����2.

�������Χ֮����������ô��������ֵ,Ϊ��������:

#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC             ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD             ((uint32_t)0x00000020)
#define RCC_APB2Periph_GPIOE             ((uint32_t)0x00000040)
#define RCC_APB2Periph_GPIOF             ((uint32_t)0x00000080)
#define RCC_APB2Periph_GPIOG             ((uint32_t)0x00000100)
#define RCC_APB2Periph_ADC1              ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2              ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)
#define RCC_APB2Periph_TIM8              ((uint32_t)0x00002000)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB2Periph_ADC3              ((uint32_t)0x00008000)
#define RCC_APB2Periph_TIM15             ((uint32_t)0x00010000)
#define RCC_APB2Periph_TIM16             ((uint32_t)0x00020000)
#define RCC_APB2Periph_TIM17             ((uint32_t)0x00040000)
#define RCC_APB2Periph_TIM9              ((uint32_t)0x00080000)
#define RCC_APB2Periph_TIM10             ((uint32_t)0x00100000)
#define RCC_APB2Periph_TIM11             ((uint32_t)0x00200000)


	typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
	#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

������ͼ���,��������ֵ,�ж�����Ϊ0����Ϊ1,�൱��һ��boolֵ.

���¿�:

if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

  ����һ��if....else.....�ж����,ͨ���۲�,���������������ͨ���ж�NewState��ֵ��ѡ���,
  ���Ƕ�RCC->APB2ENR����Ĵ������в���.
  ����������һ������Ĵ���:
   RCC->APB2ENR:APB2����ʱ��ʹ�ܼĴ���(RCC_APB2ENR),
            ����ʹ����Ӧ������ʱ��,�翪��AFIOʱ��,��ô�Ͱ�RCC_APB2Periph_AFIO��ֵ����RCC->APB2ENR����.
              ����ر�AFIOʱ��,��ô�Ͱ�RCC_APB2Periph_AFIO��ֵȡ������RCC->AOB2ENR����.

//USATR_TX
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART1_RX	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

������Щ�Ƕ�GPIO���ŵ�һЩ������,���ں���GPIO_Init(),��֮ǰ�й���ϸ�Ľ�˵,����Ͳ��ن�����.
�и���Ҫע�����,�������ŵĹ���ģʽ�����Ǹ��ù��ܵ��������,����ΪʲôҪ��PA9��PA10����,����Ǹ��ݹٷ�
�ֲ�͵�·ԭ��ͼ��ȷ�ϵ�,������������������žͿ��Ե������õ�.

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

������ж����ȼ�����,����NVIC_Init()����֮ǰ����ϸ�Ľ�˵,����Ͳ��ټ���������,�ö��.

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

����ǶԴ��ڽ�������,���ǵĳ�Ա����,������,��������ֱ�ӽ��뵽�������������:

	USART_Init(USART1, &USART_InitStructure); 
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  	USART_Cmd(USART1, ENABLE);   

�ڹ�����,���ǻ����ÿ����Ա��ȡֵ��Χ,�����ǵĹ���,���������ȷ�����һ������:USART_Init():

void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  	uint32_t tmpreg = 0x00, apbclock = 0x00;
  	uint32_t integerdivider = 0x00;
 	uint32_t fractionaldivider = 0x00;
  	uint32_t usartxbase = 0;
  	RCC_ClocksTypeDef RCC_ClocksStatus;
  	/* Check the parameters */
  	assert_param(IS_USART_ALL_PERIPH(USARTx));
  	assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  	assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  	assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  	assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  	assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  	assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  	/* The hardware flow control is available only for USART1, USART2 and USART3 */
  	if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  	{
   		 assert_param(IS_USART_123_PERIPH(USARTx));
  	}

  	usartxbase = (uint32_t)USARTx;

	/*---------------------------- USART CR2 Configuration -----------------------*/
  	tmpreg = USARTx->CR2;
  	/* Clear STOP[13:12] bits */
  	tmpreg &= CR2_STOP_CLEAR_Mask;
  	/* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
  	/* Set STOP[13:12] bits according to USART_StopBits value */
  	tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;
  
  	/* Write to USART CR2 */
 	USARTx->CR2 = (uint16_t)tmpreg;

	/*---------------------------- USART CR1 Configuration -----------------------*/
  	tmpreg = USARTx->CR1;
  	/* Clear M, PCE, PS, TE and RE bits */
  	tmpreg &= CR1_CLEAR_Mask
  	/* Configure the USART Word Length, Parity and mode ----------------------- */
  	/* Set the M bits according to USART_WordLength value */
  	/* Set PCE and PS bits according to USART_Parity value */
  	/* Set TE and RE bits according to USART_Mode value */
 	 tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity | USART_InitStruct->USART_Mode;
  	/* Write to USART CR1 */
  	USARTx->CR1 = (uint16_t)tmpreg;

	/*---------------------------- USART CR3 Configuration -----------------------*/  
  	tmpreg = USARTx->CR3;
 	 /* Clear CTSE and RTSE bits */
  	tmpreg &= CR3_CLEAR_Mask;
  	/* Configure the USART HFC -------------------------------------------------*/
  	/* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
  	tmpreg |= USART_InitStruct->USART_HardwareFlowControl;
  	/* Write to USART CR3 */
  	USARTx->CR3 = (uint16_t)tmpreg;

	/*---------------------------- USART BRR Configuration -----------------------*/
  	/* Configure the USART Baud Rate -------------------------------------------*/
  	RCC_GetClocksFreq(&RCC_ClocksStatus);
  	if (usartxbase == USART1_BASE)
 	{
    		apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  	}
  	else
  	{
    		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
 	 }
  
  	/* Determine the integer part */
  	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
 	 {
    		/* Integer part computing in case Oversampling mode is 8 Samples */
    		integerdivider = ((25 * apbclock) / (2 * (USART_InitStruct->USART_BaudRate)));    
  	}
  	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  	{
    		/* Integer part computing in case Oversampling mode is 16 Samples */
   		 integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));    
  	}
  	tmpreg = (integerdivider / 100) << 4;

  	/* Determine the fractional part */
  	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

 	 /* Implement the fractional part in the register */
  	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
  	{
    		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  	}
  	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  	{
    		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  	}
  
  	/* Write to USART BRR */
  	USARTx->BRR = (uint16_t)tmpreg;
}

����ȥ��....
��ô�డ,��ʵ��,���Ѿ����ٵ���,��,������һ��һ��Ľ��ٰ�:

������,����һ�ѱ���:

	uint32_t tmpreg = 0x00, apbclock = 0x00;
  	uint32_t integerdivider = 0x00;
 	uint32_t fractionaldivider = 0x00;
  	uint32_t usartxbase = 0;
  	RCC_ClocksTypeDef RCC_ClocksStatus;

������ʲô��,��֪��!����и��ṹ��,��������������ṹ����ʲô��Ա������:

typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t ADCCLK_Frequency;  /*!< returns ADCCLK clock frequency expressed in Hz */
}RCC_ClocksTypeDef;

ͨ���۲����ĳ�Ա����,������������ʱ��Ƶ�ʵ�.��λ��Hz.

��һ��!

/* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

����һЩ�������,����ͨ���鿴��Щ�������ķ�Χ,����֪����ȷ��ѡ��.����һ��������:

assert_param(IS_USART_ALL_PERIPH(USARTx));

#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3) || \
                                     ((PERIPH) == UART4) || \
                                     ((PERIPH) == UART5))

ͨ������궨��,����֪���˴��������,���������ټ���׷����,������׷��ʲô.

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)

#define USART1_BASE           (APB2PERIPH_BASE + 0x3800)

#define USART1              ((USART_TypeDef *) USART1_BASE)

#define USART2_BASE           (APB1PERIPH_BASE + 0x4400)
#define USART3_BASE           (APB1PERIPH_BASE + 0x4800)
#define UART4_BASE            (APB1PERIPH_BASE + 0x4C00)
#define UART5_BASE            (APB1PERIPH_BASE + 0x5000)

#define USART2              ((USART_TypeDef *) USART2_BASE)
#define USART3              ((USART_TypeDef *) USART3_BASE)
#define UART4               ((USART_TypeDef *) UART4_BASE)
#define UART5               ((USART_TypeDef *) UART5_BASE)

׷������,���Ƿ���,���е������ַ�Ļ���ַ����PERIPH_BASE=0x4000 0000;
ÿ�����ڵ������ַ����һ��,���ǻ�����,������ĸ��ĺ궨��,ǿ��ת���ɽṹ������,����׷������ṹ��:
               
typedef struct
{
  __IO uint16_t SR;
  uint16_t  RESERVED0;
  __IO uint16_t DR;
  uint16_t  RESERVED1;
  __IO uint16_t BRR;
  uint16_t  RESERVED2;
  __IO uint16_t CR1;
  uint16_t  RESERVED3;
  __IO uint16_t CR2;
  uint16_t  RESERVED4;
  __IO uint16_t CR3;
  uint16_t  RESERVED5;
  __IO uint16_t GTPR;
  uint16_t  RESERVED6;
} USART_TypeDef;    

��������ṹ���õĶ���16λ�Ŀռ�,Ԥ����16λ�Ŀռ�.һ����7���Ĵ���.��ֻ����ǰ16λ�Ŀռ�.
����ȥ����ֲ�,������Щ�Ĵ����Ǹ�ɶ��:

USART_SR:״̬�Ĵ���

USART_DR:���ݼĴ���

USART_BRR:�����ʼĴ���

USART_CR1:���ƼĴ���

USART_CR2:���ƼĴ���

USART_CR3:���ƼĴ���

USART_GTPR:���ƼĴ���

������ʲô����,��Ҫ���ֲ�,���ﶼ�õ���Щ,�һ��ں������ʱ,��ϸ˵��һ��.


 assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  


#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))

��һ��,������˼�Ǹ��������޶���һ����Χ,ֻҪ�������Χ֮�ڵĶ�������.
Ϊʲô����Ҫ���ò�����,�����ֵ����ͨ�ŵ��ٶ�.��ֵԽ��,��λʱ���ڴ������Ϣ����Խ��.

assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));

#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)
                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

����������,��������ֳ�,Ҳ���ǳ��ڷ���һ����Ϣ,�м�������λ,8λ��9λ

assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));

#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))

����Ǽ���ֹͣλ������.һ������ô�ĸ�ѡ��.����0.5��1.5��ֹͣλ��ѡ��,��UARET4��UARST5������Ч��.

assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));

#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

����Ǽ�����żУ��λ������,����ô����ѡ��:����żУ��,żУ��,��У��.

assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));

#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))

ͨ���������������:���ڵ�ģʽ������:���պͷ���

assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

#define USART_HardwareFlowControl_None       ((uint16_t)0x0000)
#define USART_HardwareFlowControl_RTS        ((uint16_t)0x0100)
#define USART_HardwareFlowControl_CTS        ((uint16_t)0x0200)
#define USART_HardwareFlowControl_RTS_CTS    ((uint16_t)0x0300)
#define IS_USART_HARDWARE_FLOW_CONTROL(CONTROL)\
                              (((CONTROL) == USART_HardwareFlowControl_None) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_CTS) || \
                               ((CONTROL) == USART_HardwareFlowControl_RTS_CTS))

�����Ӳ�������Ƶ�����,��ſ���һ��,����������һЩ�Ĵ���,������������Щ�Ĵ�������ʲô��;:

�鿴�ֲ�:RTS:�����ж�ʹ��,�����ջ�����Ϊ��ʱ,�Ż������������
	CTS:����ʹ���ж�,��CTSΪ����ʱ,��ʼ����.
	��һЩ������Ӳ�������.
	��������һ�㲻��Ӳ������,���ڵ��շ�,����һ�㶼���������Լ�д�ĳ���������.

if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }

���if���,��Ȼ��һ��������,ִ��������,�����ǿ���Ӳ������ʱ,�Ż�ִ������������,�������������������:

#define IS_USART_123_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3))

ͨ������궨��,����֪����Ӳ��������ֻ�ܿ���USART1,USART2,USART3����������,ʣ�µ�USART4��USART5����Ч��.


usartxbase = (uint32_t)USARTx;

�����һ�θ�ֵ,Ҫ��ʼ���Ĵ��ں�.

������,�Ϳ�ʼ���ڵ�����:

tmpreg = USARTx->CR2;

��ȡCR2�Ĵ�����ԭʼ����

  /* Clear STOP[13:12] bits */
  tmpreg &= CR2_STOP_CLEAR_Mask;

��һ����һ��������Ӹ�ֵ�������.���ǿ�������궨��

#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */

����һ��ֹͣλ����.	

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
  /* Set STOP[13:12] bits according to USART_StopBits value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;

��仰�Ǹ�ֵ,����Ҫ���õ�ֹͣλ���󸳸�tmpreg.
  
  /* Write to USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;

�޸�CR2�Ĵ���,��������ֹͣλ.

 tmpreg = USARTx->CR1;

��ȡCR1��ֵ

  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= CR1_CLEAR_Mask;

���Ǹ�������Ӹ�ֵ����,���ǿ�������궨��:

#define CR1_CLEAR_Mask            ((uint16_t)0xE9F3)  /*!< USART CR1 Mask */

���Ǹ�CR1������,�������������ֵ��ʲô��:

0xE9F3  =    1110 1001 1111 0011

�����ȿ������CR1�Ĵ����Ǹ�ɶ��

ʹ�ܴ���,���ڵĻ���ģʽ:��ַ,ʹ����żУ������ж�.ʹ�ܷ����ж�,ʹ�ܷ�������ж�,ʹ�ܽ�������ж�,ʹ�ܿ����߼���ж�,����Ϊ��Ĭģʽ,���ͶϿ�֡.

��Щ�����ǳ�ʼ��Ĭ�ϵ�����,�������ǿ��Ը����Լ�����������޸�.

  /* Configure the USART Word Length, Parity and mode ----------------------- */
  /* Set the M bits according to USART_WordLength value */
  /* Set PCE and PS bits according to USART_Parity value */
  /* Set TE and RE bits according to USART_Mode value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;

����������صĲ���,�ֳ�,��żУ��,���ڵĹ���ģʽ

  /* Write to USART CR1 */
  USARTx->CR1 = (uint16_t)tmpreg;

�������úõĴ������ݸ���CR1�Ĵ���.�������.

  tmpreg = USARTx->CR3;

��ȡCR3�Ĵ�������ֵ.Ҳ����˵�ǻ�ȡCR3�Ĵ���������.

  /* Clear CTSE and RTSE bits */
  tmpreg &= CR3_CLEAR_Mask;

����tmpreg������һ�������,���и�ֵ������,�������������������ֵ:

#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */

�������ֵ����,���ǹر�������λ,�ֱ��ǵڰ�λ�͵ھ�λ,����ȥ�����ֲ�,������λ��ʲô��;:

������λ��Ӳ��������λ,Ϊ1ʱ,����Ӳ��������,��Ӳ�������ƴ��ڵ��շ�����,�������ǽ�����ֻΪ0,�ر�Ӳ��������.


  /* Configure the USART HFC -------------------------------------------------*/
  /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

��仰������Ӳ��������,����ֵ���ǿ��Է�������鿴����У������,�����ĸ�ֵ����ѡ��.

  /* Write to USART CR3 */
  USARTx->CR3 = (uint16_t)tmpreg;

��ֵ��CT3�Ĵ���,���CR3�Ĵ���������.

������Щ������Ҫ������CR1,CR2,CR3�Ĵ���

CR1:�����ֳ�,��żУ��,���ڵĹ���ģʽ,
CR2:����ֹͣλ
CR3:ʹ�ܴ��ڵ��շ��ж�,��������Ӳ��������.

������һ��δ��������ò����ʵĴ���,���ǲ��ż�,һ��һ������:

	RCC_GetClocksFreq(&RCC_ClocksStatus);
 	 if (usartxbase == USART1_BASE)
 	 {
   		 apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  	}
  	else
  	{
    		apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  	}
  
 	 /* Determine the integer part */
  	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
  	{
   		 /* Integer part computing in case Oversampling mode is 8 Samples */
    		integerdivider = ((25 * apbclock) / (2 * (USART_InitStruct->USART_BaudRate)));    
 	}
  	else /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  	{
    		/* Integer part computing in case Oversampling mode is 16 Samples */
    		integerdivider = ((25 * apbclock) / (4 * (USART_InitStruct->USART_BaudRate)));    
  	}
 	tmpreg = (integerdivider / 100) << 4;

  	/* Determine the fractional part */
  	fractionaldivider = integerdivider - (100 * (tmpreg >> 4));

  	/* Implement the fractional part in the register */
  	if ((USARTx->CR1 & CR1_OVER8_Set) != 0)
  	{
   		tmpreg |= ((((fractionaldivider * 8) + 50) / 100)) & ((uint8_t)0x07);
  	}
  	else	 /* if ((USARTx->CR1 & CR1_OVER8_Set) == 0) */
  	{
    		tmpreg |= ((((fractionaldivider * 16) + 50) / 100)) & ((uint8_t)0x0F);
  	}
  
  	/* Write to USART BRR */
 	 USARTx->BRR = (uint16_t)tmpreg;

���ȵ�һ��:

RCC_GetClocksFreq(&RCC_ClocksStatus);'

����һ����������,����׷��ȥ,��������ԭ��,���������Ǹ�ɶ:

void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
  uint32_t prediv1factor = 0;
#endif
    
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;
  
  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
    case 0x04:  /* HSE used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
      break;
    case 0x08:  /* PLL used as system clock */

      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
      
#ifndef STM32F10X_CL      
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {
 #if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
       prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
       /* HSE oscillator clock selected as PREDIV1 clock entry */
       RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull; 
 #else
        /* HSE selected as PLL clock entry */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE oscillator clock divided by 2 */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_VALUE * pllmull;
        }
 #endif
      }
#else
      pllmull = pllmull >> 18;
      
      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL multiplication factor = PLL input clock * 6.5 */
        pllmull = 13 / 2; 
      }
            
      if (pllsource == 0x00)
      {/* HSI oscillator clock divided by 2 selected as PLL clock entry */
        RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE >> 1) * pllmull;
      }
      else
      {/* PREDIV1 selected as PLL clock entry */
        
        /* Get PREDIV1 clock source and division factor */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
        
        if (prediv1source == 0)
        { /* HSE oscillator clock selected as PREDIV1 clock entry */
          RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE / prediv1factor) * pllmull;          
        }
        else
        {/* PLL2 clock selected as PREDIV1 clock entry */
          
          /* Get PREDIV2 division factor and PLL2 multiplication factor */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2; 
          RCC_Clocks->SYSCLK_Frequency = (((HSE_VALUE / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F10X_CL */ 
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
      break;
  }

  /* Compute HCLK, PCLK1, PCLK2 and ADCCLK clocks frequencies ----------------*/
  /* Get HCLK prescaler */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK clock frequency */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* Get PCLK1 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 clock frequency */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get PCLK2 prescaler */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 clock frequency */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* Get ADCCLK prescaler */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK clock frequency */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}









 