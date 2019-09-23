
#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)       //如果启用端口复用功能,必须开启复用功能时钟
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
} RCC_TypeDef;                                                                                                          //时钟使能寄存器

#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define RCC_BASE              (AHBPERIPH_BASE + 0x1000)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)                                                                 //RCC的基地址


void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)     //打开相应的外设时钟
{
 	 /* Check the parameters */
	  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
	  assert_param(IS_FUNCTIONAL_STATE(NewState));   
	  if (NewState != DISABLE)
	  {
		    RCC->APB2ENR |= RCC_APB2Periph;     //打开相应的外设时钟.
	  }
	  else
	  {
		    RCC->APB2ENR &= ~RCC_APB2Periph;   //关闭相应的外设时钟
	  }
}


#define GPIO_PortSourceGPIOA       ((uint8_t)0x00)
#define GPIO_PortSourceGPIOB       ((uint8_t)0x01)
#define GPIO_PortSourceGPIOC       ((uint8_t)0x02)
#define GPIO_PortSourceGPIOD       ((uint8_t)0x03)
#define GPIO_PortSourceGPIOE       ((uint8_t)0x04)
#define GPIO_PortSourceGPIOF       ((uint8_t)0x05)
#define GPIO_PortSourceGPIOG       ((uint8_t)0x06)                //IO组

#define GPIO_PinSource0            ((uint8_t)0x00)
#define GPIO_PinSource1            ((uint8_t)0x01)
#define GPIO_PinSource2            ((uint8_t)0x02)
#define GPIO_PinSource3            ((uint8_t)0x03)
#define GPIO_PinSource4            ((uint8_t)0x04)
#define GPIO_PinSource5            ((uint8_t)0x05)
#define GPIO_PinSource6            ((uint8_t)0x06)
#define GPIO_PinSource7            ((uint8_t)0x07)
#define GPIO_PinSource8            ((uint8_t)0x08)
#define GPIO_PinSource9            ((uint8_t)0x09)
#define GPIO_PinSource10           ((uint8_t)0x0A)
#define GPIO_PinSource11           ((uint8_t)0x0B)
#define GPIO_PinSource12           ((uint8_t)0x0C)
#define GPIO_PinSource13           ((uint8_t)0x0D)
#define GPIO_PinSource14           ((uint8_t)0x0E)
#define GPIO_PinSource15           ((uint8_t)0x0F)                    //IO口

typedef struct
{
  __IO uint32_t EVCR;          //事件控制寄存器
  __IO uint32_t MAPR;         //复用重映射
  __IO uint32_t EXTICR[4];   //外部中断控制寄存器,有4个寄存器,一个寄存器控制四条中断线
  uint32_t RESERVED0;
  __IO uint32_t MAPR2;  
} AFIO_TypeDef;                     //AFIO控制寄存器

//函数功能:把中断通道和GPIO引脚通过映射,让他们关联起来.
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)     //参数传递:GPIO组号和GPIO引脚号
{
	  uint32_t tmp = 0x00;
	  /* Check the parameters */
	  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
	  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
 	 tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));    //0xf00
	 AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;                            //把要设置的位先置零AFIO->EXTICR[0]=0x**** *0**
	 AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03))); 
                                                                 //置位  AFIO->EXTICR[0] = 0x**** *1**   把PE组的2号引脚映射到中断通道2号
}



typedef struct
{
 	 uint32_t EXTI_Line;         		  //中断通道号    /*!< Specifies the EXTI lines to be enabled or disabled.
                                                                                    		This parameter can be any combination of @ref EXTI_Lines */
   
	 EXTIMode_TypeDef EXTI_Mode;    	 //工作模式        /*!< Specifies the mode for the EXTI lines.
                                         			               	This parameter can be a value of @ref EXTIMode_TypeDef */

	 EXTITrigger_TypeDef EXTI_Trigger; 	//触发方式         /*!< Specifies the trigger signal active edge for the EXTI lines.
                                      			               	 This parameter can be a value of @ref EXTIMode_TypeDef */

 	 FunctionalState EXTI_LineCmd;    	//使能位             /*!< Specifies the new state of the selected EXTI lines.
                                        					 This parameter can be set either to ENABLE or DISABLE */ 
}EXTI_InitTypeDef;



#define EXTI_Line0       ((uint32_t)0x00001)  /*!< External interrupt line 0 */
#define EXTI_Line1       ((uint32_t)0x00002)  /*!< External interrupt line 1 */
#define EXTI_Line2       ((uint32_t)0x00004)  /*!< External interrupt line 2 */
#define EXTI_Line3       ((uint32_t)0x00008)  /*!< External interrupt line 3 */
#define EXTI_Line4       ((uint32_t)0x00010)  /*!< External interrupt line 4 */
#define EXTI_Line5       ((uint32_t)0x00020)  /*!< External interrupt line 5 */
#define EXTI_Line6       ((uint32_t)0x00040)  /*!< External interrupt line 6 */
#define EXTI_Line7       ((uint32_t)0x00080)  /*!< External interrupt line 7 */
#define EXTI_Line8       ((uint32_t)0x00100)  /*!< External interrupt line 8 */
#define EXTI_Line9       ((uint32_t)0x00200)  /*!< External interrupt line 9 */
#define EXTI_Line10      ((uint32_t)0x00400)  /*!< External interrupt line 10 */
#define EXTI_Line11      ((uint32_t)0x00800)  /*!< External interrupt line 11 */
#define EXTI_Line12      ((uint32_t)0x01000)  /*!< External interrupt line 12 */
#define EXTI_Line13      ((uint32_t)0x02000)  /*!< External interrupt line 13 */
#define EXTI_Line14      ((uint32_t)0x04000)  /*!< External interrupt line 14 */
#define EXTI_Line15      ((uint32_t)0x08000)  /*!< External interrupt line 15 */
#define EXTI_Line16      ((uint32_t)0x10000)  /*!< External interrupt line 16 Connected to the PVD Output */
#define EXTI_Line17      ((uint32_t)0x20000)  /*!< External interrupt line 17 Connected to the RTC Alarm event */
#define EXTI_Line18      ((uint32_t)0x40000)  /*!< External interrupt line 18 Connected to the USB Device/USB OTG FS
                                                   Wakeup from suspend event */                                    
#define EXTI_Line19      ((uint32_t)0x80000)  /*!< External interrupt line 19 Connected to the Ethernet Wakeup event */



typedef enum
{
	  EXTI_Mode_Interrupt = 0x00,
	  EXTI_Mode_Event = 0x04
}EXTIMode_TypeDef;


typedef enum
{
	  EXTI_Trigger_Rising = 0x08,
	  EXTI_Trigger_Falling = 0x0C,  
	  EXTI_Trigger_Rising_Falling = 0x10
}EXTITrigger_TypeDef;

typedef enum 
{
	   DISABLE = 0,
	   ENABLE = !DISABLE
} FunctionalState;


#define PERIPH_BASE           ((uint32_t)0x40000000) /*!< Peripheral base address in the alias region */
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
#define EXTI_BASE             (APB2PERIPH_BASE + 0x0400)

#define EXTI                ((EXTI_TypeDef *) EXTI_BASE)

void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)              //配置中断寄存器
{
 	 uint32_t tmp = 0;

	  /* Check the parameters */
 	 assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
 	 assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
 	 assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
 	 assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

 	 tmp = (uint32_t)EXTI_BASE;                                        //0x40010400   中断寄存器的基地址
     
 	 if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
	 {
  		  /* Clear EXTI line configuration */
   		 EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;    //屏蔽来自二号线上的中断请求      中断屏蔽寄存器
   		 EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;   //屏蔽来自二号线上的事件请求      事件屏蔽寄存器
    
   		 tmp += EXTI_InitStruct->EXTI_Mode;                     //0x40010400 + 0x00

  	 	 *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;           //__IO    volatile 修饰tmp,表示这个参数是一个易变的变量,将tmp地址上的值取出来进行运算,相当于操作寄存器

   	 	/* Clear Rising Falling edge configuration */
   		 EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;             //清除上升沿触发寄存器对应的位
    		 EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;             //清除下降沿触发寄存器对应的位
    
   		 /* Select the trigger for the selected external interrupts */
  	 	 if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
  	 	 {
    	 		 /* Rising Falling edge */
     			 EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
     			 EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
   		 }
   		 else
  		 {
     			 tmp = (uint32_t)EXTI_BASE;
     			 tmp += EXTI_InitStruct->EXTI_Trigger;    //基础地址  +  偏移地址

     			 *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;   //对地址进行解引用,然后进行运算
   		 }
 	 }
 	 else
 	 {
  		  tmp += EXTI_InitStruct->EXTI_Mode;               //  基础地址  +   偏移地址,确定到对应的模式寄存器

   		 /* Disable the selected external lines */
   		 *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;    //将对应的中断线屏蔽请求
 	 }
}



//配置中断管理组


typedef struct
{
	  uint8_t NVIC_IRQChannel;                   	//中断通道号	 /*!< Specifies the IRQ channel to be enabled or disabled.
                                              		   	 		 This parameter can be a value of @ref IRQn_Type 
                                              		   			  (For the complete STM32 Devices IRQ Channels list, please
                                              		   			   refer to stm32f10x.h file) */

 	 uint8_t NVIC_IRQChannelPreemptionPriority; 	//中断分组	 /*!< Specifies the pre-emption priority for the IRQ channel
                                                  					specified in NVIC_IRQChannel. This parameter can be a value
                                                 			 		 between 0 and 15 as described in the table @ref NVIC_Priority_Table */

 	 uint8_t NVIC_IRQChannelSubPriority;       	//中断的子优先级	  /*!< Specifies the subpriority level for the IRQ channel specified
                              						     in NVIC_IRQChannel. This parameter can be a value
                                              					     between 0 and 15 as described in the table @ref NVIC_Priority_Table */

 	 FunctionalState NVIC_IRQChannelCmd;        	//中断的使能位	 /*!< Specifies whether the IRQ channel defined in NVIC_IRQChannel
                                                  					   will be enabled or disabled. 
                                   						   This parameter can be set either to ENABLE or DISABLE */   
} NVIC_InitTypeDef;



typedef struct
{
  __IO uint32_t ISER[8];                      /*!< Offset: 0x000  Interrupt Set Enable Register           */
       uint32_t RESERVED0[24];                                   
  __IO uint32_t ICER[8];                      /*!< Offset: 0x080  Interrupt Clear Enable Register         */
       uint32_t RSERVED1[24];                                    
  __IO uint32_t ISPR[8];                      /*!< Offset: 0x100  Interrupt Set Pending Register          */
       uint32_t RESERVED2[24];                                   
  __IO uint32_t ICPR[8];                      /*!< Offset: 0x180  Interrupt Clear Pending Register        */
       uint32_t RESERVED3[24];                                   
  __IO uint32_t IABR[8];                      /*!< Offset: 0x200  Interrupt Active bit Register           */
       uint32_t RESERVED4[56];                                   
  __IO uint8_t  IP[240];                      /*!< Offset: 0x300  Interrupt Priority Register (8Bit wide) */
       uint32_t RESERVED5[644];                                  
  __O  uint32_t STIR;                         /*!< Offset: 0xE00  Software Trigger Interrupt Register     */
}  NVIC_Type;

#define SCS_BASE            (0xE000E000)                              /*!< System Control Space Base Address */
#define NVIC_BASE           (SCS_BASE +  0x0100)                      /*!< NVIC Base Address                 */
#define NVIC                ((NVIC_Type *)          NVIC_BASE)        /*!< NVIC configuration struct         */

#define NVIC_PriorityGroup_0         ((uint32_t)0x700) /*!< 0 bits for pre-emption priority
                                                            4 bits for subpriority */
#define NVIC_PriorityGroup_1         ((uint32_t)0x600) /*!< 1 bits for pre-emption priority
                                                            3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x500) /*!< 2 bits for pre-emption priority
                                                            2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x400) /*!< 3 bits for pre-emption priority
                                                            1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x300) /*!< 4 bits for pre-emption priority
                                                            0 bits for subpriority */

#define IS_NVIC_PRIORITY_GROUP(GROUP) (((GROUP) == NVIC_PriorityGroup_0) || \
                                       ((GROUP) == NVIC_PriorityGroup_1) || \
                                       ((GROUP) == NVIC_PriorityGroup_2) || \
                                       ((GROUP) == NVIC_PriorityGroup_3) || \
                                       ((GROUP) == NVIC_PriorityGroup_4))

 EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt 

typedef struct
{
  __I  uint32_t CPUID;                        /*!< Offset: 0x00  CPU ID Base Register                                  */
  __IO uint32_t ICSR;                         /*!< Offset: 0x04  Interrupt Control State Register                      */
  __IO uint32_t VTOR;                         /*!< Offset: 0x08  Vector Table Offset Register                          */
  __IO uint32_t AIRCR;                        /*!< Offset: 0x0C  Application Interrupt / Reset Control Register        */
  __IO uint32_t SCR;                          /*!< Offset: 0x10  System Control Register                               */
  __IO uint32_t CCR;                          /*!< Offset: 0x14  Configuration Control Register                        */
  __IO uint8_t  SHP[12];                      /*!< Offset: 0x18  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IO uint32_t SHCSR;                        /*!< Offset: 0x24  System Handler Control and State Register             */
  __IO uint32_t CFSR;                         /*!< Offset: 0x28  Configurable Fault Status Register                    */
  __IO uint32_t HFSR;                         /*!< Offset: 0x2C  Hard Fault Status Register                            */
  __IO uint32_t DFSR;                         /*!< Offset: 0x30  Debug Fault Status Register                           */
  __IO uint32_t MMFAR;                        /*!< Offset: 0x34  Mem Manage Address Register                           */
  __IO uint32_t BFAR;                         /*!< Offset: 0x38  Bus Fault Address Register                            */
  __IO uint32_t AFSR;                         /*!< Offset: 0x3C  Auxiliary Fault Status Register                       */
  __I  uint32_t PFR[2];                       /*!< Offset: 0x40  Processor Feature Register                            */
  __I  uint32_t DFR;                          /*!< Offset: 0x48  Debug Feature Register                                */
  __I  uint32_t ADR;                          /*!< Offset: 0x4C  Auxiliary Feature Register                            */
  __I  uint32_t MMFR[4];                      /*!< Offset: 0x50  Memory Model Feature Register                         */
  __I  uint32_t ISAR[5];                      /*!< Offset: 0x60  ISA Feature Register                                  */
} SCB_Type; 

#define SCS_BASE            (0xE000E000)                              /*!< System Control Space Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00)                      /*!< System Control Block Base Address */
#define SCB                 ((SCB_Type *)           SCB_BASE)         /*!< SCB configuration struct          */
.
/******  STM32 specific Interrupt Numbers *********************************************************/
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                            */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt            */
  TAMPER_IRQn                 = 2,      /*!< Tamper Interrupt                                     */
  RTC_IRQn                    = 3,      /*!< RTC global Interrupt                                 */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                               */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                 */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                 */
  EXTI1_IRQn                  = 7,      /*!< EXTI Line1 Interrupt                                 */
  EXTI2_IRQn                  = 8,      /*!< EXTI Line2 Interrupt                                 */
  EXTI3_IRQn                  = 9,      /*!< EXTI Line3 Interrupt                                 */
  EXTI4_IRQn                  = 10,     /*!< EXTI Line4 Interrupt                                 */
  DMA1_Channel1_IRQn          = 11,     /*!< DMA1 Channel 1 global Interrupt                      */
  DMA1_Channel2_IRQn          = 12,     /*!< DMA1 Channel 2 global Interrupt                      */
  DMA1_Channel3_IRQn          = 13,     /*!< DMA1 Channel 3 global Interrupt                      */
  DMA1_Channel4_IRQn          = 14,     /*!< DMA1 Channel 4 global Interrupt                      */
  DMA1_Channel5_IRQn          = 15,     /*!< DMA1 Channel 5 global Interrupt                      */
  DMA1_Channel6_IRQn          = 16,     /*!< DMA1 Channel 6 global Interrupt                      */
  DMA1_Channel7_IRQn          = 17,     /*!< DMA1 Channel 7 global Interrupt                      */


#ifdef STM32F10X_HD
  ADC1_2_IRQn                 = 18,     /*!< ADC1 and ADC2 global Interrupt                       */
  USB_HP_CAN1_TX_IRQn         = 19,     /*!< USB Device High Priority or CAN1 TX Interrupts       */
  USB_LP_CAN1_RX0_IRQn        = 20,     /*!< USB Device Low Priority or CAN1 RX0 Interrupts       */
  CAN1_RX1_IRQn               = 21,     /*!< CAN1 RX1 Interrupt                                   */
  CAN1_SCE_IRQn               = 22,     /*!< CAN1 SCE Interrupt                                   */
  EXTI9_5_IRQn                = 23,     /*!< External Line[9:5] Interrupts                        */
  TIM1_BRK_IRQn               = 24,     /*!< TIM1 Break Interrupt                                 */
  TIM1_UP_IRQn                = 25,     /*!< TIM1 Update Interrupt                                */
  TIM1_TRG_COM_IRQn           = 26,     /*!< TIM1 Trigger and Commutation Interrupt               */
  TIM1_CC_IRQn                = 27,     /*!< TIM1 Capture Compare Interrupt                       */
  TIM2_IRQn                   = 28,     /*!< TIM2 global Interrupt                                */
  TIM3_IRQn                   = 29,     /*!< TIM3 global Interrupt                                */
  TIM4_IRQn                   = 30,     /*!< TIM4 global Interrupt                                */
  I2C1_EV_IRQn                = 31,     /*!< I2C1 Event Interrupt                                 */
  I2C1_ER_IRQn                = 32,     /*!< I2C1 Error Interrupt                                 */
  I2C2_EV_IRQn                = 33,     /*!< I2C2 Event Interrupt                                 */
  I2C2_ER_IRQn                = 34,     /*!< I2C2 Error Interrupt                                 */
  SPI1_IRQn                   = 35,     /*!< SPI1 global Interrupt                                */
  SPI2_IRQn                   = 36,     /*!< SPI2 global Interrupt                                */
  USART1_IRQn                 = 37,     /*!< USART1 global Interrupt                              */
  USART2_IRQn                 = 38,     /*!< USART2 global Interrupt                              */
  USART3_IRQn                 = 39,     /*!< USART3 global Interrupt                              */
  EXTI15_10_IRQn              = 40,     /*!< External Line[15:10] Interrupts                      */
  RTCAlarm_IRQn               = 41,     /*!< RTC Alarm through EXTI Line Interrupt                */
  USBWakeUp_IRQn              = 42,     /*!< USB Device WakeUp from suspend through EXTI Line Interrupt */
  TIM8_BRK_IRQn               = 43,     /*!< TIM8 Break Interrupt                                 */
  TIM8_UP_IRQn                = 44,     /*!< TIM8 Update Interrupt                                */
  TIM8_TRG_COM_IRQn           = 45,     /*!< TIM8 Trigger and Commutation Interrupt               */
  TIM8_CC_IRQn                = 46,     /*!< TIM8 Capture Compare Interrupt                       */
  ADC3_IRQn                   = 47,     /*!< ADC3 global Interrupt                                */
  FSMC_IRQn                   = 48,     /*!< FSMC global Interrupt                                */
  SDIO_IRQn                   = 49,     /*!< SDIO global Interrupt                                */
  TIM5_IRQn                   = 50,     /*!< TIM5 global Interrupt                                */
  SPI3_IRQn                   = 51,     /*!< SPI3 global Interrupt                                */
  UART4_IRQn                  = 52,     /*!< UART4 global Interrupt                               */
  UART5_IRQn                  = 53,     /*!< UART5 global Interrupt                               */
  TIM6_IRQn                   = 54,     /*!< TIM6 global Interrupt                                */
  TIM7_IRQn                   = 55,     /*!< TIM7 global Interrupt                                */
  DMA2_Channel1_IRQn          = 56,     /*!< DMA2 Channel 1 global Interrupt                      */
  DMA2_Channel2_IRQn          = 57,     /*!< DMA2 Channel 2 global Interrupt                      */
  DMA2_Channel3_IRQn          = 58,     /*!< DMA2 Channel 3 global Interrupt                      */
  DMA2_Channel4_5_IRQn        = 59      /*!< DMA2 Channel 4 and Channel 5 global Interrupt        */
#endif /* STM32F10X_HD */

void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)                        //中断管理器分组,在程序执行期间只能在开始时调用一次.
{
  /* Check the parameters */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* Set the PRIGROUP[10:8] bits according to NVIC_PriorityGroup value */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;                     //修改0xE000ED0C
}


void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* Check the parameters */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* Compute the Corresponding IRQ Priority --------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;     //0x02
    tmppre = (0x4 - tmppriority);                                                           //0x02
    tmpsub = tmpsub >> tmppriority;                                                    //0x03

    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;//
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;                         //
    tmppriority = tmppriority << 0x04;                                                                                //确定四位数值,为接下来的优先级分组提供一个数值
        
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;                                          //对应的中断通道,设置中断优先级控制寄存器的高8位.设置中断通道的抢占优先级和响应优先级
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);                          //设置中断通道对应的使能寄存器,每一个位控制一个中断通道
  }
  else
  {
    /* Disable the Selected IRQ Channels -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);                          //关闭对应的中断通道
  }
}



void EXTI_ClearITPendingBit(uint32_t EXTI_Line)       //清除中断标志位
{
  /* Check the parameters */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}