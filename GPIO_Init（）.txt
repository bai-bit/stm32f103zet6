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



咱一行一行的来看，

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)

这是函数头，不解释，下一个！

uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
uint32_t tmpreg = 0x00, pinmask = 0x00；

自定义的变量，暂时先不管他们的作用，接着往下看，

assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));

这三行是错误验证，校验传递的参数是否有效。

先看GPIOx的有效范围：

#define IS_GPIO_ALL_PERIPH(PERIPH) (((PERIPH) == GPIOA) || \

                                    ((PERIPH) == GPIOB) || \

                                    ((PERIPH) == GPIOC) || \

                                    ((PERIPH) == GPIOD) || \

                                    ((PERIPH) == GPIOE) || \

                                    ((PERIPH) == GPIOF) || \

                                    ((PERIPH) == GPIOG))

然后看GPIO_Mode的有效选择：

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

再看GPIO_Pin的选择范围：

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

由以上这一堆代码，可以知道我这个板子，它把所有的IO口分成了7组，每一组由15个引脚，每一个引脚有八种工作模式。而且每一个引脚都有唯一的地址。
当我们操作寄存器的某个位时，寄存器根据这个位对应的地址来控制某个GPIO引脚。



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

通过上面这些宏定义，我们可以确定每一个GPIO引脚的地址。

typedef struct
{
  __IO uint32_t CRL;
  __IO uint32_t CRH;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t BRR;
  __IO uint32_t LCKR;
} GPIO_TypeDef;   //提供一下GPIO_TypeDef的结构体，

接着看：

currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

这一行，是为变量currentmode赋了一个值。

分析一下这个值的范围：

先看看GPIO_Mode的取值范围：

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

根据这个枚举体，我们确定了GPIO_Mode的取值范围，分别是：

0x0,   0x04,   0x28,   0x48,   0x14,    0x10,     0x1c,    0x18,

这么几个奇怪的数值。

这几个数值要和0x0f相与，我们一起来计算一下，都能与处一个什么值，

0x0,    0x4,    0x8,    0x8,      0x4,      0x0,       0xc,       0x8

我们发现，相与之后的结果是保留低四位，高四位被清零。有什么用，暂时还不清楚，让我们继续往下看


if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
{ 
	/* Check the parameters */
	assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
	/* Output mode */
	currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
}
这是一个if条件判断语句，入口条件为：
((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00
保留第五位，其他的位全部清零。
通过观察GPIOMode_TypeDef枚举体，发现第五位为1的模式都是输出模式。
往下看，错误检验：
assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
先看看GPIO_Speed的取值范围：
typedef enum
{ 
	GPIO_Speed_10MHz = 1,
	GPIO_Speed_2MHz, 
	GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
#define IS_GPIO_SPEED(SPEED) (((SPEED) == GPIO_Speed_10MHz) || ((SPEED) == GPIO_Speed_2MHz) || \
					((SPEED) == GPIO_Speed_50MHz))


通过查看GPIOSpeed_TypeDef枚举体，GPIO_Speed有三个选择。
接下来，看最后一句：
currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
currentmode的值或上GPIO_Speed的值，然后赋给currentmode。得出一个结论,只有在输出模式中,才会设置GPIO_Speed这个成员.
GPIO_Speed:输出驱动电路的响应速度，可理解为: 输出驱动电路的带宽：即一个驱动电路可以不失真地通过信号的最大频率
详细的介绍来这里https://blog.csdn.net/KantsWang/article/details/83022803
接着往下走:

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

这一段好长,不过不着急,咱们一句一句的啃,先第一个if条件:

if(((uint32_t)GPIO_InitStruct->GPIO_Pin & (uint32_t)0x00FF)) != 0x00)

GPIO_Pin  与  0x00FF相与,保留低八位,高八位清零,判断通过条件:结果不能为0,就进入if条件语句

tmpreg = GPIOx->CRL;

获取某一GPIO组的CRL寄存器信息;   CRL和CRH都是端口配置寄存器,设置引脚的工作模式和频率.具体内容查看STM32中文参考手册的第八章,GPIO寄存器说明.

for (pinpos = 0x00; pinpos < 0x08; pinpos++)

for循环语句,根据条件,这个for循环,最多要执行八次.

 pos = ((uint32_t)0x01) << pinpos;

把0x01  左移 pinpos位,然后赋值给pos.具体有什么用,不清楚,继续往下看.

currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;

把GPIO_Pin与pos相与,然后将得到的结果赋值给currentpin,到这里我们基本能猜到pos的用途,他是用来获取GPIO_Pin的
值,但是这个值是有范围的,必须让它在pos的范围之内,否则就是0x00,

if (currentpin == pos)

判断current的赋值情况,如果和pos相等,也就是说GPIO_Pin的值和的值相等,表示获取到GPIO的引脚信息

pos = pinpos << 2;

将pinpos的值左移2位,将pinpos的值放大4倍,然后赋值给pos,用途不知道,继续往下看:

pinmask = ((uint32_t)0x0F) << pos;

这一步,是将0xF这个值左移pos个位,为什么这样干,我们先来了解一下GPIO_CRL和GPIO_CRH这两个寄存器的结构:

GPIO_CRL:这个寄存器叫做端口配置低寄存器GPIOx_CRL(x=A,B,C,D,E,F,G)
每4个位控制一个GPIO引脚.这四个位分成两个部分:前两位设置它具体的输入输出模式,因为它有八种工作模式,后两位设置是
确定是输出还是输入,如果是输出,顺便把工作频率Speed给确定了.
所以它一共可以设置8个GPIO引脚.也就是低八个GPIO引脚:0~7

GPIO_CRH:这个寄存器叫做端口配置高寄存器GPIOx_CRH(x=A,B,C,D,E,F,G)
功能上和CRL一样,只不过这个寄存器设置的是高八个GPIO引脚:8~15

因为一组GPIO引脚有16个:0~15;

所以这条代码的意思是把相应的4个位确定了,后面将会取反.

tmpreg &= ~pinmask;

.这条语句把pinmask取反赋值给tmpreg,现在只是将具体的某4位清零的掩码值赋值给tmpreg,还没有用到寄存器上

继续往下看:

tmpreg |= (currentmode << pos);

现在我们再回头看看currentmode这个变量:

currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);

 currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;

有这么两条赋值语句:现在这个currentmode上的值是低四位有效,刚才上面说它得到一些奇怪的数值,现在我们把它挪过来分析一下:

我们确定了GPIO_Mode的取值范围，分别是：

0x0,   0x04,   0x28,   0x48,   0x14,    0x10,     0x1c,    0x18,

这几个数值要和0x0f相与，我们一起来计算一下，都能与处一个什么值，

0x0,    0x4,    0x8,    0x8,      0x4,      0x0,       0xc,       0x8

通过观察GPIO_CRL寄存器的说明,我们知道:这些数值是确定引脚的配置,但是都是输入模式,从这里我们可以知道,输入模式不用设置工作频率,

但是输出是要设置频率的,所以:

 currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;

这句话被放在if条件语句中执行,根据GPIO_Mode的第五位是否为1来确定是否为输出模式.也就是下面这句话:

if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)

接下来,回到我们刚才分析的语句:

tmpreg |= (currentmode << pos);

让currentmode左移pos个位,这个pos我们知道,他是确定了我们要设置的某一个GPIO的引脚,

所以这句话的功能是:将某一个GPIO引脚的配置信息赋值给tmpreg;前提是,对应的位必须先清零,所以才有了这几句话:

	pos = pinpos << 2;
        	pinmask = ((uint32_t)0x0F) << pos;
        	tmpreg &= ~pinmask;

接着看:

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

最后一组if.....else.....语句:

大概意思是在上下拉输入模式中,我们还需要细分,所以这里有引入了两个寄存器:GPIOx_BRR和GPIOx_BSRR寄存器,我们先把这两个寄存器
科普一下,再说代码,
首先是GPIOx_BRR:端口位清除寄存器(GPIOx_BRR,x=A,B,C,D,E,F,G)
            只用到它的0~15的位,高16位为保留,这些位只能写入并只能以16位数值进行操作
         功能是:为0时,对对应的ODRy位不产生影响.(好叭,又出来个ODR寄存器,马上科普)
                   为1时,清除对应的ODRy位为0;
     增加:GPIOx_ODR:端口输出数据寄存器(GPIOx_ODR,x=A,B,C,D,E,F,G)
            高16位[31:16]:保留
             低16位[15:0]:这些位可读可写并只能以16位的数值进行操作.
 然后是GPIOx_BSRR:端口位设置/清除寄存器(GPIOx_BSRR,x=A,B,C,D,E,F,G)
            高16位[31:16]:清除端口x的位,这些位只能写入并只能以16位的数值的进行操作
                  0:对对应的ODR位不产生影响
                  1:清除对应的ODR位为0.
            低16位[15:0]:端口设置x的位,这些位只能写入并只能以16位的数值进行操作.
                   0:对对应的ODR位不产生影响
                   1:设置相应的ODR位为1
                  注:如果同时设置了高16位和低16位,那么低16位起作用.
现在回到上面的代码:
  我们了解到在输入模式中,如果是上下拉输入模式,那么我们可以通过ODR寄存器中对应的位,确定引脚究竟是上拉输入还是下拉输入.
          当ODR对应的位为1时,为上拉输入,如果为0,就是下拉输入.

最后一句:

GPIOx->CRL = tmpreg;

将tmpreg赋值给GPIOx->CRL,配置相应的GPIO引脚.

到这里,这一段代码就分析完了,这是配置CRL的,后面还有CRH的代码:

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

我们大概看了一下,除了第一个if的进入条件不一样,其他的一模一样.所以这里不再一句一句的分析了.
我们来看看这个不一样的if条件语句:

 if (GPIO_InitStruct->GPIO_Pin > 0x00FF)

当GPIO_Pin的值大于0x00FF时才能进入if语句.
也就是GPIO引脚的高8个引脚:8~15号.

#define GPIO_Pin_8                 ((uint16_t)0x0100)  /*!< Pin 8 selected */

#define GPIO_Pin_9                 ((uint16_t)0x0200)  /*!< Pin 9 selected */

#define GPIO_Pin_10                ((uint16_t)0x0400)  /*!< Pin 10 selected */

#define GPIO_Pin_11                ((uint16_t)0x0800)  /*!< Pin 11 selected */

#define GPIO_Pin_12                ((uint16_t)0x1000)  /*!< Pin 12 selected */

#define GPIO_Pin_13                ((uint16_t)0x2000)  /*!< Pin 13 selected */

#define GPIO_Pin_14                ((uint16_t)0x4000)  /*!< Pin 14 selected */

#define GPIO_Pin_15                ((uint16_t)0x8000)  /*!< Pin 15 selected */

#define GPIO_Pin_All               ((uint16_t)0xFFFF)  /*!< All pins selected */

这里我们看到最后有个GPIO_Pin_All的引脚,也是大于0x00FF,那么它是不是也能进入到这个if条件呢.我们来看这么一段代码:

assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin)); //错误检验

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
                              ((PIN) == GPIO_Pin_15))               //检验合格的范围,这个范围中并没有GPIO_Pin_All这个成员,所以,它是进不去的.


整段代码到这里就结束了.
