#include "stm32f10x.h"
/******************************************************************************/
//�ж����ȼ�����
//��ʱ��2���ж����ȼ�Ϊ0(���)
/******************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;												
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// �����жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;						//�����ⲿ�ж�2���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							// �����жϷ���
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;						//�����ⲿ�ж�2���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);							// �����жϷ���
//	NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;;				//����PC9���ж����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);							// �����жϷ���
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;				//����PC9���ж����ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

}
