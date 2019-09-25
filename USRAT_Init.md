
	  
#if 1
#pragma import(__use_no_semihosting)         //关闭使用半主机模式      
                
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
 
_sys_exit(int x)                                         //避免在产生软件中断时进入半主机模式
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)                           //重新定义fputc函数，使用串口send数据
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
	USART_InitStructure.USART_StopBits = USART_StopBits_1；
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None；
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


上面的代码中有三个函数:
	int fputc(int ch,FILE *f);
	void uart_init(u32 bound);
	void USART1_IRQHandler(void);
我们现在先说第二个函数:
	void uart_init(u32 bound);

功能是初始化串口,我们现在一句一句的来扒光它,哈哈哈:

	GPIO_InitTypeDef             GPIO_InitStructure;
	USART_InitTypeDef           USART_InitStructure;
	NVIC_InitTypeDef              NVIC_InitStructure;

一上来,就先创建了三个结构体:我们来看看他们都是干啥的:'

第一个:GPIO_InitTypeDef             GPIO_InitStructure;
  包含的是GPIO的配置信息;

typedef struct
{
 	 uint16_t GPIO_Pin;            			 /*!< Specifies the GPIO pins to be configured.
                                      				This parameter can be any value of @ref GPIO_pins_define */

  	GPIOSpeed_TypeDef GPIO_Speed;  		/*!< Specifies the speed for the selected pins.
                                      				This parameter can be a value of @ref GPIOSpeed_TypeDef */

  	GPIOMode_TypeDef GPIO_Mode;    		/*!< Specifies the operating mode for the selected pins.
                                      				This parameter can be a value of @ref GPIOMode_TypeDef */
}GPIO_InitTypeDef;


第二个:USART_InitTypeDef           USART_InitStructure;
  包含的是串口的配置信息;

typedef struct
{
 	 uint32_t USART_BaudRate;           	//波特率	 /*!< This member configures the USART communication baud rate.
                                         			 	 The baud rate is computed using the following formula:
                                         			   	- IntegerDivider = ((PCLKx) / (16 * (USART_InitStruct->USART_BaudRate)))
                                           			 	- FractionalDivider = ((IntegerDivider - ((u32) IntegerDivider)) * 16) + 0.5 */

	  uint16_t USART_WordLength;             //字长 	/*!< Specifies the number of data bits transmitted or received in a frame.
                                          			 	This parameter can be a value of @ref USART_Word_Length */

 	 uint16_t USART_StopBits;          	//停止位  	/*!< Specifies the number of stop bits transmitted.
                                          			 	This parameter can be a value of @ref USART_Stop_Bits */

 	 uint16_t USART_Parity;              //奇偶校验	/*!< Specifies the parity mode.
                                          				 This parameter can be a value of @ref USART_Parity
                                          			 	@note When parity is enabled, the computed parity is inserted
                                                 			at the MSB position of the transmitted data (9th bit when
                                                 			the word length is set to 9 data bits; 8th bit when the
                                                		 	word length is set to 8 data bits). */
 
 	 uint16_t USART_Mode;                //工作模式	/*!< Specifies wether the Receive or Transmit mode is enabled or disabled.
                                           				This parameter can be a value of @ref USART_Mode */

  	uint16_t USART_HardwareFlowControl; 	//硬件流控制	/*!< Specifies wether the hardware flow control mode is enabled
                                           					or disabled.
                                           					This parameter can be a value of @ref USART_Hardware_Flow_Control */
} USART_InitTypeDef;


第三个:NVIC_InitTypeDef              NVIC_InitStructure;
   包含的是中断分组的配置信息;

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


上面这一堆是这三个结构体的成员情况.接着往下走:

RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

这句话的功能是使能相应的时钟.只有把相应的时钟使能了,我们才能使用对应的外设.这里我又个疑问,为什么没有使能AFIO的时钟
因为串口,是必须要使用端口复用功能的.
后来我去网上查了一下,AFIO时钟的开启条件是,当你的程序需要配置AFIO时钟相应的寄存器时,才去开启它.如果不对AFIO寄存器进行配置,
我们就不用开启它.
很明显我们这个程序并没有对AFIO的寄存器进行配置,所以我们不用对他进行开启设置.如果我们的串口1,需要把默认的PA9和PA10映射到其他的引脚,
这时,我们就需要开启AFIO时钟了,因为我们需要配置AFIO_MAPR这个寄存器,将串口1的收发引脚重映射到其他引脚:PB6和PB7


附上这个函数的代码,我们来看看他是怎么开启相应的时钟的:

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

先来两句错误检验:
	assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  	assert_param(IS_FUNCTIONAL_STATE(NewState));

那我们先看看他们的取值范围吧:

	#define IS_RCC_APB2_PERIPH(PERIPH) ((((PERIPH) & 0xFFC00002) == 0x00) && ((PERIPH) != 0x00))

由这句话,我们可以知道RCC_APB2Perigh的取值范围是1~(~0xFFC00002);且不等于2.

在这个范围之内我们有这么几个特殊值,为我们所用:

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

这两句就简单了,就是两个值,判断它是为0还是为1,相当于一个bool值.

往下看:

if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

  这是一个if....else.....判断语句,通过观察,这个代码块的作用是通过判断NewState的值来选择的,
  都是对RCC->APB2ENR这个寄存器进行操作.
  下面来科普一下这个寄存器:
   RCC->APB2ENR:APB2外设时钟使能寄存器(RCC_APB2ENR),
            用来使能相应的外设时钟,如开启AFIO时钟,那么就把RCC_APB2Periph_AFIO的值赋给RCC->APB2ENR即可.
              如果关闭AFIO时钟,那么就把RCC_APB2Periph_AFIO的值取反赋给RCC->AOB2ENR即可.

//USATR_TX
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
   
  //USART1_RX	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

上面这些是对GPIO引脚的一些的配置,关于函数GPIO_Init(),在之前有过详细的解说,这里就不再啰嗦了.
有个需要注意的是,发送引脚的工作模式必须是复用功能的推挽输出,至于为什么要用PA9和PA10引脚,这个是根据官方
手册和电路原理图来确认的,不是随便找了两个引脚就可以当串口用的.

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	

这个是中断优先级配置,关于NVIC_Init()函数之前有详细的解说,这里就不再继续啰嗦了,好多的.

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

这个是对串口进行配置,他们的成员变量,上面有,我们这里直接进入到下面的三个函数:

	USART_Init(USART1, &USART_InitStructure); 
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  	USART_Cmd(USART1, ENABLE);   

在过程中,我们会分析每个成员的取值范围,和他们的功能,下面我们先分析第一个函数:USART_Init():

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

这我去嘞....
这么多啊,其实呢,这已经够少的了,来,让我们一句一句的解刨吧:

首先呢,这是一堆变量:

	uint32_t tmpreg = 0x00, apbclock = 0x00;
  	uint32_t integerdivider = 0x00;
 	uint32_t fractionaldivider = 0x00;
  	uint32_t usartxbase = 0;
  	RCC_ClocksTypeDef RCC_ClocksStatus;

具体有什么用,不知道!最后还有个结构体,我们来看看这个结构体有什么成员变量吧:

typedef struct
{
  uint32_t SYSCLK_Frequency;  /*!< returns SYSCLK clock frequency expressed in Hz */
  uint32_t HCLK_Frequency;    /*!< returns HCLK clock frequency expressed in Hz */
  uint32_t PCLK1_Frequency;   /*!< returns PCLK1 clock frequency expressed in Hz */
  uint32_t PCLK2_Frequency;   /*!< returns PCLK2 clock frequency expressed in Hz */
  uint32_t ADCCLK_Frequency;  /*!< returns ADCCLK clock frequency expressed in Hz */
}RCC_ClocksTypeDef;

通过观察它的成员变量,它是用来保存时钟频率的.单位是Hz.

下一个!

/* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));

这是一些错误检验,我们通过查看这些错误检验的范围,可以知道正确的选择.我们一个个分析:

assert_param(IS_USART_ALL_PERIPH(USARTx));

#define IS_USART_ALL_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3) || \
                                     ((PERIPH) == UART4) || \
                                     ((PERIPH) == UART5))

通过这个宏定义,我们知道了串口有五个,下面我们再继续追代码,看看能追到什么.

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

追到这里,我们发现,所有的外设地址的基地址都是PERIPH_BASE=0x4000 0000;
每个串口的物理地址都不一样,我们还发现,最后这四个的宏定义,强制转换成结构体类型,我们追查这个结构体:
               
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

发现这个结构体用的都是16位的空间,预留了16位的空间.一共有7个寄存器.都只用了前16位的空间.
我们去查查手册,看看这些寄存器是干啥的:

USART_SR:状态寄存器

USART_DR:数据寄存器

USART_BRR:波特率寄存器

USART_CR1:控制寄存器

USART_CR2:控制寄存器

USART_CR3:控制寄存器

USART_GTPR:控制寄存器

具体有什么作用,需要查手册,这里都用到哪些,我会在后边遇到时,详细说明一下.


 assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  


#define IS_USART_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))

这一话,它的意思是给波特率限定了一个范围,只要在这个范围之内的都可以用.
为什么串口要设置波特率,这个数值决定通信的速度.数值越大,单位时间内传输的信息量就越大.

assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));

#define USART_WordLength_8b                  ((uint16_t)0x0000)
#define USART_WordLength_9b                  ((uint16_t)0x1000)
                                    
#define IS_USART_WORD_LENGTH(LENGTH) (((LENGTH) == USART_WordLength_8b) || \
                                      ((LENGTH) == USART_WordLength_9b))

这个错误检验,检验的是字长,也就是出口发送一次信息,有几个数据位,8位或9位

assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));

#define USART_StopBits_1                     ((uint16_t)0x0000)
#define USART_StopBits_0_5                   ((uint16_t)0x1000)
#define USART_StopBits_2                     ((uint16_t)0x2000)
#define USART_StopBits_1_5                   ((uint16_t)0x3000)
#define IS_USART_STOPBITS(STOPBITS) (((STOPBITS) == USART_StopBits_1) || \
                                     ((STOPBITS) == USART_StopBits_0_5) || \
                                     ((STOPBITS) == USART_StopBits_2) || \
                                     ((STOPBITS) == USART_StopBits_1_5))

这个是检验停止位的设置.一共有这么四个选项.不过0.5和1.5个停止位的选项,在UARET4和UARST5中是无效的.

assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));

#define USART_Parity_No                      ((uint16_t)0x0000)
#define USART_Parity_Even                    ((uint16_t)0x0400)
#define USART_Parity_Odd                     ((uint16_t)0x0600) 
#define IS_USART_PARITY(PARITY) (((PARITY) == USART_Parity_No) || \
                                 ((PARITY) == USART_Parity_Even) || \
                                 ((PARITY) == USART_Parity_Odd))

这个是检验奇偶校验位的设置,有这么三个选项:无奇偶校验,偶校验,奇校验.

assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));

#define USART_Mode_Rx                        ((uint16_t)0x0004)
#define USART_Mode_Tx                        ((uint16_t)0x0008)
#define IS_USART_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))

通过这三句代码来看:串口的模式有两种:接收和发送

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

这个是硬件流控制的设置,大概看了一下,都是设置了一些寄存器,我们来看看这些寄存器都有什么用途:

查看手册:RTS:接收中断使能,当接收缓存区为空时,才会请求接收数据
	CTS:发送使能中断,当CTS为输入时,开始发送.
	这一些过程有硬件来完成.
	不过我们一般不用硬件控制,串口的收发,我们一般都是用我们自己写的程序来控制.

if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }

这个if语句,仍然是一个错误检测,执行条件是,当我们开启硬件控制时,才会执行这个错误检验,让我们来看看这个检验:

#define IS_USART_123_PERIPH(PERIPH) (((PERIPH) == USART1) || \
                                     ((PERIPH) == USART2) || \
                                     ((PERIPH) == USART3))

通过这个宏定义,我们知道了硬件流控制只能控制USART1,USART2,USART3这三个串口,剩下的USART4和USART5是无效的.


usartxbase = (uint32_t)USARTx;

完成了一次赋值,要初始化的串口号.

接下来,就开始串口的配置:

tmpreg = USARTx->CR2;

获取CR2寄存器的原始数据

  /* Clear STOP[13:12] bits */
  tmpreg &= CR2_STOP_CLEAR_Mask;

这一句是一个与运算加赋值运算操作.我们看看这个宏定义

#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */

这是一个停止位掩码.	

  /* Configure the USART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
  /* Set STOP[13:12] bits according to USART_StopBits value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;

这句话是赋值,把需要设置的停止位需求赋给tmpreg.
  
  /* Write to USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;

修改CR2寄存器,重新设置停止位.

 tmpreg = USARTx->CR1;

获取CR1的值

  /* Clear M, PCE, PS, TE and RE bits */
  tmpreg &= CR1_CLEAR_Mask;

这是个与运算加赋值运算,我们看看这个宏定义:

#define CR1_CLEAR_Mask            ((uint16_t)0xE9F3)  /*!< USART CR1 Mask */

这是个CR1的掩码,我们来看看这个值有什么用:

0xE9F3  =    1110 1001 1111 0011

我们先看看这个CR1寄存器是干啥的

使能串口,串口的唤醒模式:地址,使能奇偶校验错误中断.使能发送中断,使能发送完成中断,使能接收完成中断,使能空闲线检测中断,设置为静默模式,发送断开帧.

这些功能是初始化默认的设置,后期我们可以根据自己的需求进行修改.

  /* Configure the USART Word Length, Parity and mode ----------------------- */
  /* Set the M bits according to USART_WordLength value */
  /* Set PCE and PS bits according to USART_Parity value */
  /* Set TE and RE bits according to USART_Mode value */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;

重新设置相关的参数,字长,奇偶校验,串口的工作模式

  /* Write to USART CR1 */
  USARTx->CR1 = (uint16_t)tmpreg;

最后把设置好的串口数据赋给CR1寄存器.完成设置.

  tmpreg = USARTx->CR3;

获取CR3寄存器的数值.也可以说是获取CR3寄存器的数据.

  /* Clear CTSE and RTSE bits */
  tmpreg &= CR3_CLEAR_Mask;

这是tmpreg与上了一个掩码后,进行赋值运算了,让我们来看看这个掩码值:

#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */

根据这个值来看,他是关闭了两个位,分别是第八位和第九位,我们去看看手册,这两个位有什么用途:

这两个位是硬件流控制位,为1时,开启硬件流控制,由硬件来控制串口的收发数据,这里我们将他们只为0,关闭硬件流控制.


  /* Configure the USART HFC -------------------------------------------------*/
  /* Set CTSE and RTSE bits according to USART_HardwareFlowControl value */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;

这句话是设置硬件流控制,它的值我们可以翻到上面查看错误校验那里,他有四个值可以选择.

  /* Write to USART CR3 */
  USARTx->CR3 = (uint16_t)tmpreg;

赋值给CT3寄存器,完成CR3寄存器的设置.

上面这些代码主要是设置CR1,CR2,CR3寄存器

CR1:设置字长,奇偶校验,串口的工作模式,
CR2:设置停止位
CR3:使能串口的收发中断,重新设置硬件流控制.

下面这一大段代码是设置波特率的代码,我们不着急,一句一句来看:

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

首先第一句:

RCC_GetClocksFreq(&RCC_ClocksStatus);'

这是一个函数调用,我们追过去,看看他的原型,他到底又是个啥:

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









 