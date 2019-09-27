

#

DAM

```c
    void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
    {
        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
        DMA_DeInit(DMA_CHx);   

        DMA1_MEM_LEN=cndtr;
        DMA_InitStructure.DMA_PeripheralBaseAddr = cpar; 
        DMA_InitStructure.DMA_MemoryBaseAddr = cmar; 
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
        DMA_InitStructure.DMA_BufferSize = cndtr; 
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
        DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(DMA_CHx, &DMA_InitStructure); 

    }
```

```

```

这个函数是一个DMA初始化函数.它需要传递四个参数,分别是:`DMA_CHx`(DMA通道号),`cpar`(目标地址),`cmpr`(源地址),`cndtr`(数据缓冲区大小)

具体用途稍后再说.

接下来先看第一个函数调用:

```c
​```
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
   
  /* Check the parameters */
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
        
  }
}

```

这个函数是一个设置**AHB**总线上相应的外设时钟.`ENABLE` or `DISABLE`

它有两个参数:`RCC_AHBPeriph`,`NewStates`  

第一个`RCC_AHBPeriph`,它表示你想要使用的外设.

第二个`NewStates`,它表示你想要使用的外设,让他的时钟处于你需要的状态.

然后是两行错误校验.

```c
assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
//第一个错误校验是检验外设,是否为AHB总线上的外设.
//我们把代码复制过来
#define IS_RCC_AHB_PERIPH_RESET(PERIPH) ((((PERIPH) & 0xFFFFAFFF) == 0x00) && ((PERIPH) != 0x00))
#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)
#define RCC_AHBPeriph_DMA2               ((uint32_t)0x00000002)
#define RCC_AHBPeriph_SRAM               ((uint32_t)0x00000004)
#define RCC_AHBPeriph_FLITF              ((uint32_t)0x00000010)
#define RCC_AHBPeriph_CRC                ((uint32_t)0x00000040)
//可以看到stm32f103zet6的AHB总线上的外设只有五个.所以传递的参数只能是这五个中的一个.
//第二个错误校验是检验状态参数.
//把代码复制过来看看
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//从代码上看,这个参数只有两个选择:ENABLE or DISABLE
//接下来的if...else...判断语句是根据要设置的状态来修改寄存器.
if (NewState != DISABLE)
    RCC->AHBENR |= RCC_AHBPeriph;
else
    RCC->AHBENR &= ~RCC_AHBPeriph;
//首先是RCC->AHBENR:
//我们先追一下代码,看看它到底是什么东西.
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
//通过上面这四句宏定义,我们知道RCC是一个RCC_TypeDef类型的指针.这个指针指向了RCC_BASE.
//这个地址上保存了什么呢,就是RCC_TypeDef的结构体.
//这个结构体上的寄存器都是用来控制时钟系统的.
//接下来我们把这个结构体复制过来.
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFGR;
  __IO uint32_t CIR;
  __IO uint32_t APB2RSTR;
  __IO uint32_t APB1RSTR;
  __IO uint32_t AHBENR;
  __IO uint32_t APB2ENR;
  __IO uint32_t APB1ENR;
  __IO uint32_t BDCR;
  __IO uint32_t CSR;

#ifdef STM32F10X_CL  
  __IO uint32_t AHBRSTR;
  __IO uint32_t CFGR2;
#endif /* STM32F10X_CL */ 

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)   
  uint32_t RESERVED0;
  __IO uint32_t CFGR2;
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL || STM32F10X_HD_VL */ 
} RCC_TypeDef;
//这里我们可以看到这么一个成员:
__IO uint32_t AHBENR;
//通过查看手册,了解到这个寄存器的第0为和第1位是用来控制DMA的时钟使能,第0位控制DMA1的时钟,第0位控制DMA2的时钟.
//开头的 __IO 其实是volatile的宏定义,这个关键字是告知编译器,这个寄存器的值是随时会被更改的,在用的时候,需要重新到存储器中读取,而不是采用处理器保存的临时值.
//好了,回归我们当前的代码
//看看这个RCC_AHBPeriph,它就是我们传递过来的外设名称翻到上面看看他们的定义,他们的值其实就是AHBENR寄存器对应的位控制的外设.
RCC->AHBENR |= RCC_AHBPeriph;
//我们传过来的值是RCC_AHBPeriph_DMA1,将宏定义的值和RCC->AHBENR的值相或,也就是将这个寄存器额某个位置一.
#define RCC_AHBPeriph_DMA1               ((uint32_t)0x00000001)
//将RCC->AHBENR的第0位置1,也就是开启DMA1的时钟.
//else里边还有一句
RCC->AHBENR &= ~RCC_AHBPeriph;
//它的含义是,关闭DMA1的时钟.
```

到这里,第一个函数调用已经结束了,我们的目的呢是开启`DMA1`的时钟,

接下来看下一句函数调用:

```c
DMA_DeInit(DMA_CHx);
```

接下来我们去追踪代码,并粘贴过来.

```c
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* Check the parameters */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  
  /* Disable the selected DMAy Channelx */
  DMAy_Channelx->CCR &= (uint16_t)(~DMA_CCR1_EN);
  
  /* Reset DMAy Channelx control register */
  DMAy_Channelx->CCR  = 0;
  
  /* Reset DMAy Channelx remaining bytes register */
  DMAy_Channelx->CNDTR = 0;
  
  /* Reset DMAy Channelx peripheral address register */
  DMAy_Channelx->CPAR  = 0;
  
  /* Reset DMAy Channelx memory address register */
  DMAy_Channelx->CMAR = 0;
  
  if (DMAy_Channelx == DMA1_Channel1)
  {
    /* Reset interrupt pending bits for DMA1 Channel1 */
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel2)
  {
    /* Reset interrupt pending bits for DMA1 Channel2 */
    DMA1->IFCR |= DMA1_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel3)
  {
    /* Reset interrupt pending bits for DMA1 Channel3 */
    DMA1->IFCR |= DMA1_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel4)
  {
    /* Reset interrupt pending bits for DMA1 Channel4 */
    DMA1->IFCR |= DMA1_Channel4_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel5)
  {
    /* Reset interrupt pending bits for DMA1 Channel5 */
    DMA1->IFCR |= DMA1_Channel5_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel6)
  {
    /* Reset interrupt pending bits for DMA1 Channel6 */
    DMA1->IFCR |= DMA1_Channel6_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel7)
  {
    /* Reset interrupt pending bits for DMA1 Channel7 */
    DMA1->IFCR |= DMA1_Channel7_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel1)
  {
    /* Reset interrupt pending bits for DMA2 Channel1 */
    DMA2->IFCR |= DMA2_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel2)
  {
    /* Reset interrupt pending bits for DMA2 Channel2 */
    DMA2->IFCR |= DMA2_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel3)
  {
    /* Reset interrupt pending bits for DMA2 Channel3 */
    DMA2->IFCR |= DMA2_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel4)
  {
    /* Reset interrupt pending bits for DMA2 Channel4 */
    DMA2->IFCR |= DMA2_Channel4_IT_Mask;
  }
  else
  { 
    if (DMAy_Channelx == DMA2_Channel5)
    {
      /* Reset interrupt pending bits for DMA2 Channel5 */
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
    }
  }
}

//看起来老长的一串.不怕,我们一句一句来看
assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
//错误校验:校验传递过来的参数范围,我们来看看这个范围是什么
#define IS_DMA_ALL_PERIPH(PERIPH) (((PERIPH) == DMA1_Channel1) || \
                                   ((PERIPH) == DMA1_Channel2) || \
                                   ((PERIPH) == DMA1_Channel3) || \
                                   ((PERIPH) == DMA1_Channel4) || \
                                   ((PERIPH) == DMA1_Channel5) || \
                                   ((PERIPH) == DMA1_Channel6) || \
                                   ((PERIPH) == DMA1_Channel7) || \
                                   ((PERIPH) == DMA2_Channel1) || \
                                   ((PERIPH) == DMA2_Channel2) || \
                                   ((PERIPH) == DMA2_Channel3) || \
                                   ((PERIPH) == DMA2_Channel4) || \
                                   ((PERIPH) == DMA2_Channel5))
//这个参数范围是DMA的通道数量.DMA1的七条通道,DMA2的五条通道.
DMAy_Channelx->CCR &= (uint16_t)(~DMA_CCR1_EN);
//先来看看关于DMA的相关寄存器叭:
//DMA_ISP中断状态寄存器
//DMA_IFCR中断标志清除寄存器
//DMA_CCRx(x=1...7)DMA通道x号配置寄存器
//DMA_CNDTRx(x=1...7)DMA通道x号传输数量寄存器
//DMA_CPARx(x=1...7)DMA通道x外设地址寄存器
//DMA_CMARx(x=1...7)DMA通道x存储器地址寄存器
//所以这一句,用到了CCR寄存器,用来配置DMA通道
//然后我们看看这个宏定义DMA_CCR1_EN的值:
#define  DMA_CCR1_EN                         ((uint16_t)0x0001)            /*!< Channel enable*/
//它的值为0x01,我们看看CCR寄存器的第0位是什么用途.
//通过查看手册得知,第0位是用来控制通道的开启和关闭,这里我们先取反,然后相与,
//所以,这句代码是关闭DMA1的x号通道.为什么要关闭呢?
//别慌,先往下看.
DMAy_Channelx->CCR  = 0;
DMAy_Channelx->CNDTR = 0;
DMAy_Channelx->CPAR  = 0;
DMAy_Channelx->CMAR = 0;
//这四句代码,很简单,就是将寄存器清零,为什么在赋值的时候先要关闭通道呢?
//通过查看手册得知,CNDTR寄存器呢,必须在通道关闭的时候才能写入,如果通道是开启的,该寄存器是只读的.不能被写入.所以我们需要先关闭通道.
//接下是一堆if条件语句.
//同样很简单,就是清除相应通道的中断标志位.通过将IFCR寄存器相应的位置一,来ISP寄存器对应的位设置为0,ISP的中断标志位是通过硬件置1,软件清0.
//同时IFCR寄存器写0,无任何反应.

/*
**到这,我们基本了解到这个DeInit函数的用途了,将我们要使用的MDA通道的寄存器恢复为缺省值.
**便于后续的设置.
*/
```



然后接下来继续看代码,先复制一下:

```c
 DMA1_MEM_LEN=cndtr;
```

这是个全局变量.用来保存的是数据存储区的范围.

用来修改CNDTR寄存器.

好了,下一个:

```c
        DMA_InitStructure.DMA_PeripheralBaseAddr = cpar; 
        DMA_InitStructure.DMA_MemoryBaseAddr = cmar; 
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
        DMA_InitStructure.DMA_BufferSize = cndtr; 
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
        DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

//这一片代码是对一个结构体赋值,我们把它复制过来,看看它都有什么成员
typedef struct
{
  uint32_t DMA_PeripheralBaseAddr; 
				//这个是外设地址,也就是cpar的值,目标地址
  uint32_t DMA_MemoryBaseAddr;     
				//这个是存储器地址,也就是cmar的值,源地址
  uint32_t DMA_DIR;                
				//这个数据的传递方向,是外设到存储器 or 外设到外设 or 存储器到存储器 or 存储器到外设,需要手动设置
  uint32_t DMA_BufferSize;       
				//这个是数据存储区的范围空间大小
  uint32_t DMA_PeripheralInc;      
				//外设的地址增量,是选择开启或者关闭
  uint32_t DMA_MemoryInc;          
				//数据区的地址增量,是选择开启或者关闭
  uint32_t DMA_PeripheralDataSize; 
    			//外设的数据宽度
  uint32_t DMA_MemoryDataSize;
    			//数据存储区的数据宽度
  uint32_t DMA_Mode;               
    			//DMA的工作模式,选择是否开启循环模式
  uint32_t DMA_Priority;          
				//DMA通道的优先级
  uint32_t DMA_M2M;                
                //DMA的存储器到存储器模式是否开启.                     
}DMA_InitTypeDef;


DMA_Init(DMA_CHx, &DMA_InitStructure);

```

