#include"stm32f10x.h"
/******************************************************************************/
//ʱ�����ó���
//����ϵͳʱ��Ϊ72MHz
/******************************************************************************/
void RCC_Config(void)
{
	RCC_DeInit();																							//��λRCC����
	RCC_HSEConfig(RCC_HSE_ON);																//���ⲿ��������
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);					//�ȴ�������������
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);				//����PLL=8M*9=72MHz
	RCC_PLLCmd(ENABLE);																				//ʹ��PLL
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);					//�ȴ�PLL����
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);								//ϵͳʱ��ѡ��ΪPLL���
	RCC_HCLKConfig(RCC_SYSCLK_Div1);													//AHB=System Clock
	RCC_PCLK1Config(RCC_HCLK_Div1);														//APB1=AHB=System Clock
	RCC_PCLK2Config(RCC_HCLK_Div1);														//APB2=AHB=System Clock
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);												//PLL���2��Ƶ
}
