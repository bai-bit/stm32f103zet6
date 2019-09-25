
void IWDG_Init(u8 prer,u16 rlr) 
{	
 	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 
	
	IWDG_SetPrescaler(prer);  
	
	IWDG_SetReload(rlr); 
	
	IWDG_ReloadCounter();
	
	IWDG_Enable(); 
}




这个是独立看门狗的库函数.现在我们一个一个来分析他们

首先第一函数:

void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
  /* Check the parameters */
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

#define IWDG_WriteAccess_Enable     ((uint16_t)0x5555)
#define IWDG_WriteAccess_Disable    ((uint16_t)0x0000)
#define IS_IWDG_WRITE_ACCESS(ACCESS) (((ACCESS) == IWDG_WriteAccess_Enable) || \
                                      ((ACCESS) == IWDG_WriteAccess_Disable))

通过查看函数原型,我们发现这个函数只是将我们传递过来的数值写入了一个寄存器IWDG->IWDG寄存器,

这个寄存器叫做键值寄存器.为只写寄存器,读出值为0x0000.

[31:16]保留
[15~0]:是键值
软件必须在一定时间内写入0xAAAA,否则,当计数器为0时,寄存器会产生复位信号.
当写入0x5555时,表示允许访问预分频寄存器IWDG_PR和重装载寄存器IWDG_RLR
当写入0xCCCC时,表示启动独立看门狗
:
再看第二个函数:

void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
  /* Check the parameters */
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

#define IWDG_Prescaler_4            ((uint8_t)0x00)
#define IWDG_Prescaler_8            ((uint8_t)0x01)
#define IWDG_Prescaler_16           ((uint8_t)0x02)
#define IWDG_Prescaler_32           ((uint8_t)0x03)
#define IWDG_Prescaler_64           ((uint8_t)0x04)
#define IWDG_Prescaler_128          ((uint8_t)0x05)
#define IWDG_Prescaler_256          ((uint8_t)0x06)
#define IS_IWDG_PRESCALER(PRESCALER) (((PRESCALER) == IWDG_Prescaler_4)  || \
                                      ((PRESCALER) == IWDG_Prescaler_8)  || \
                                      ((PRESCALER) == IWDG_Prescaler_16) || \
                                      ((PRESCALER) == IWDG_Prescaler_32) || \
                                      ((PRESCALER) == IWDG_Prescaler_64) || \
                                      ((PRESCALER) == IWDG_Prescaler_128)|| \
                                      ((PRESCALER) == IWDG_Prescaler_256))

这个函数原型的功能是设置预分频系数,把我们传递过来的数值写入了一个叫IWDG_PR的寄存器

我们看看手册,了解一下这个寄存器:

IWDG_PR:预分频寄存器
	[31:3]保留
	[2:0]预分频因子
	要想改变预分频因子,IWDG_SR寄存器的PVU必须为0.
	这三个通过0和1不同的组合代表不同的预分频因子.

接着看第三个函数:

void IWDG_SetReload(uint16_t Reload)
{
  /* Check the parameters */
  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

#define IWDG_FLAG_PVU               ((uint16_t)0x0001)
#define IWDG_FLAG_RVU               ((uint16_t)0x0002)
#define IS_IWDG_FLAG(FLAG) (((FLAG) == IWDG_FLAG_PVU) || ((FLAG) == IWDG_FLAG_RVU))
#define IS_IWDG_RELOAD(RELOAD) ((RELOAD) <= 0xFFF)

这个函数原型的功能是设置重装载值,把我们传递过来的重装载值写入IWDG->RLR寄存器

我们来看看这个寄存器的作用

IWDG->RLR:重装载寄存器
	[31:12]保留
	[11:0]重装载值,这些位具有写保护作用,只有当IWDG_SR寄存器中的RVU位为0时,才能修改这个重装载寄存器.
	
第四个函数:

void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

这个函数它没有错误校验,只有一条赋值语句,
我们先看看这个宏定义是什么意思:

/* KR register bit mask */
#define KR_KEY_Reload    ((uint16_t)0xAAAA)
#define KR_KEY_Enable    ((uint16_t)0xCCCC)

我们现在可以知道,这个函数原型的功能是启动看门狗,和重置计数器的计数值.

看最后一个函数:

void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

我们发现这个函数也只有一条赋值语句,这个宏我们在上一个函数那里见过.
所以我们知道函数的功能是使能独立看门狗.

看下一个函数模块:


void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();//reload										   
}

这个函数调用了另一个函数,我们把调用函数的原型找出来:

void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

我们发现这个函数,我们刚才在上一个函数模块中已经分析过了,这是一个喂狗的函数.

思路:它应该由一个定时器中断不同的调用,防止喂狗,也能防止程序跑飞.

