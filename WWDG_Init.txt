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


这是一个窗口看门狗的初始化函数.首先看第一个函数调用:

RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);

因为窗口看门狗使用的时钟是总线时钟,所以它需要使能相应的时钟,
而独立看门狗只用的低速的内部时钟,只要芯片一上电,自动产生时钟,不需要通过软件来使能.

第二行代码:
WWDG_CNT = tr & WWDG_CNT;

这行代码是赋值,由于WWDG_CNT是一个全局变量,所以它的值可以被其他函数使用.

tr是我们自己设置的计数值,通过这句代码,将传递过来的计数值赋给一个全局变量.供其他函数使用.

第三行代码:

WWDG_SetPrescaler(fprer);

这是个函数调用,我们去追函数原型:

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

这句是错误校验:

 assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));

通过它,我们可以知道预分频值是有范围的:

#define WWDG_Prescaler_1    ((uint32_t)0x00000000)
#define WWDG_Prescaler_2    ((uint32_t)0x00000080)
#define WWDG_Prescaler_4    ((uint32_t)0x00000100)
#define WWDG_Prescaler_8    ((uint32_t)0x00000180)
#define IS_WWDG_PRESCALER(PRESCALER) (((PRESCALER) == WWDG_Prescaler_1) || \
                                      ((PRESCALER) == WWDG_Prescaler_2) || \
                                      ((PRESCALER) == WWDG_Prescaler_4) || \
                                      ((PRESCALER) == WWDG_Prescaler_8))

传递过来的参数是有范围的,就是这四个值.

下一句代码:

  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;

#define CFR_WDGTB_Mask    ((uint32_t)0xFFFFFE7F)

通过这个宏定义,我们知道,接下来操作的是某个寄存器的第七位和第八位.

#define WWDG                ((WWDG_TypeDef *) WWDG_BASE)

#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)

#define APB1PERIPH_BASE       PERIPH_BASE

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

通过上面的这几句代码,我们知道了WWDG的物理地址.

typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFR;
  __IO uint32_t SR;
} WWDG_TypeDef;

这是个结构体,里边有三个成员变量.代表三个寄存器
我们通过查看手册:
WWDG_CR:控制寄存器
	[31:8]保留
	[7]:WDGA,激活位,
		0:禁止看门狗
		1:启用看门狗
	[6:0]:7位计数器,
		为递减计数器,当计数器的值从40h变成3fh时,产生看门狗复位.
WWDG_CFR:配置寄存器
	[31:10]保留
	[9] :EWI提前唤醒中断.
		若此为置1,则当计数器值达到40h时,即产生中断.
	[8:7];WDGTB[1:0]:时基
		预分频器,00:CK计时器时钟(PCLK1除以4096)除以1
			01:CK计时器时钟(PCLK1除以4096)除以2
			10:CK计时器时钟(PCLK1除以4096)除以4
			11:CK计时器时钟(PCLK1除以4096)除以8
	[6:0]:W[6:0],7位窗口值
		这几个位组成一个窗口值,用来与递减计数器进行比较.
WWDG_SR:状态寄存器
	[31:1]保留
	[0]:EWIF,提前唤醒中断标志
		当计数器值达到40h时,此位由硬件置1,它必须通过软件写0来清除.对此位写1无效.若中断未被使能,此位也会被置1.

所以这句代码的含义:获取当前该寄存器的数值,将第七位和第八位置0.然后赋值给tmpreg.

下一句代码:
tmpreg |= WWDG_Prescaler;

下一句代码:
 WWDG->CFR = tmpreg;

这两句代码是把我们自定义的数值写入CFR寄存器.设置窗口看门狗的预分频值.

接下来看下一个函数调用:

WWDG_SetWindowValue(wr);

我们把它的原型追出来:

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

这个函数的作用是设置窗口值,这个窗口值的作用是计数器的值和窗口值进行比较.窗口值必须大于0x40.否则就没有窗口.
当计数器的值大于窗口值的时候,操作WWDG_CR寄存器的时候的,产生复位信号.
当计数器的值小于窗口值的时候,操作WWDG_CR寄存器的时候,按照自定义的行为的来进行.

assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));

#define IS_WWDG_WINDOW_VALUE(VALUE) ((VALUE) <= 0x7F)

窗口值的范围0~111 1111

 tmpreg = WWDG->CFR & CFR_W_Mask;

获取当前窗口值.

tmpreg |= WindowValue & (uint32_t) BIT_Mask;

#define BIT_Mask          ((uint8_t)0x7F)

赋值运算,把传递过来的参数和0x7F相与,如果窗口值小于0x7F,就把传递过来的窗口值赋给tmpreg.
如果大于0x7F,在它的上一步就报错,如果上一步失效,那么在这一步,把0x7F赋给tmpreg.

  WWDG->CFR = tmpreg;

将新的的窗口值写入WWDR_CFR寄存器.

接下来看下一个函数调用:

WWDG_ClearFlag();

追查函数原型:

void WWDG_ClearFlag(void)
{
  WWDG->SR = (uint32_t)RESET;
}

这个函数非常简单,就一句话.将WWDG_SR寄存器写入RESET.

我们看看这个宏定义:

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

表示写入0或者写入1.这一句话是写入0,清除中断标志位.

下一个函数调用:

WWDG_NVIC_Init();

函数原型:
void WWDG_NVIC_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;    //WWDG?D??
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;   //?à??2￡?×óó??è??3￡?×é2	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	 //?à??2￡?×óó??è??3￡?×é2	
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
	NVIC_Init(&NVIC_InitStructure);//NVIC3?ê??ˉ
}

窗口看门狗中断初始化函数.中断通道是WWDG_IRQx.通过对应的通道号,调用对应的中断处理函数.

下一个函数调用:

WWDG_EnableIT();

函数原型:

void WWDG_EnableIT(void)
{
  *(__IO uint32_t *) CFR_EWI_BB = (uint32_t)ENABLE;
}

查看这个宏定义:

#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

#define PERIPH_BB_BASE        ((uint32_t)0x42000000) /*!< Peripheral base address in the bit-band region */

#define WWDG_BASE             (APB1PERIPH_BASE + 0x2C00)
#define APB1PERIPH_BASE       PERIPH_BASE
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */

通过上面这一系列的宏定义,我们知道,这几步是位带操作,直接操作某一个位,
通过推算,得出,这一步实际上就是在操作WWDG_CFR寄存器的第九位.将这一位置一,使能提前唤醒中断.

下一个函数调用:

WWDG_Enable(WWDG_CNT);

查看函数原型:

void WWDG_Enable(uint8_t Counter)
{
  /* Check the parameters */
  assert_param(IS_WWDG_COUNTER(Counter));
  WWDG->CR = CR_WDGA_Set | Counter;
}

查看错误校验:

#define IS_WWDG_COUNTER(COUNTER) (((COUNTER) >= 0x40) && ((COUNTER) <= 0x7F))

校验传递的参数范围,大于0x40,小于0x7F.这个值是计数值,是我们自定义的计数值,它和窗口值是两回事.

下一句:

WWDG->CR = CR_WDGA_Set | Counter;

先看看宏定义:

#define CR_WDGA_Set       ((uint32_t)0x00000080)

将第8位置1,通过查看手册,我们知道这个寄存器的第八位是使能窗口看门狗.