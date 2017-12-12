#include "stm32f10x.h"
/******************************************************************************/
//中断优先级配置
//定时器2的中断优先级为0(最高)
/******************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;												
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// 设置中断分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;						//设置外部中断2的中断优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// 设置中断分组
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;						//设置外部中断2的中断优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							// 设置中断分组
//	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;;				//设置PC9的中断优先级
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							// 设置中断分组
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;				//设置PC9的中断优先级
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

}
