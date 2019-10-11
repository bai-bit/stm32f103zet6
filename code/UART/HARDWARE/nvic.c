#include<nvic.h>

void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
	//优先级分组
	SCB->AIRCR = NVIC_PriorityGroup | AIRCR_VECTKEY_MASK;
}

void NVIC_Init(uint8_t NVIC_IRQChannel,uint8_t PreemptionPriority,uint8_t SubPriority,FunctionalState NewState)
{
	//初始化中断处理函数
	//配置NVIC->IP寄存器，设置相应的外设的中断优先级
	//配置NVIC->ISRx寄存器，使能相应的外设中断。
	uint32_t tempriority = 0,premov = 0,submask = 0x0f;
	
	if(NewState == ENABLE)
	{
		tempriority = (0x700 - ((SCB->AIRCR) & 0x700)) >> 0x8;
		premov = 0x4 - tempriority;
		submask = submask >> tempriority;
		
		tempriority |= PreemptionPriority << premov;
		tempriority |= SubPriority & submask;
		tempriority = tempriority <<0x04;
		
		NVIC->IP[NVIC_IRQChannel] = tempriority;
		
		NVIC->ISER[NVIC_IRQChannel >> 0x05] = 0x01 << (NVIC_IRQChannel & 0x1F);
	}
	else
		NVIC->ICER[NVIC_IRQChannel >> 0x05] = 0x01 << (NVIC_IRQChannel & 0x1F);
}
