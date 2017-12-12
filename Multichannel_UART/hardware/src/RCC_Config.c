#include"stm32f10x.h"
/******************************************************************************/
//时钟配置程序
//配置系统时钟为72MHz
/******************************************************************************/
void RCC_Config(void)
{
	RCC_DeInit();																							//复位RCC设置
	RCC_HSEConfig(RCC_HSE_ON);																//开外部晶体振荡器
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY)==RESET);					//等待晶体振荡器起振
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);				//配置PLL=8M*9=72MHz
	RCC_PLLCmd(ENABLE);																				//使能PLL
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET);					//等待PLL工作
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);								//系统时钟选择为PLL输出
	RCC_HCLKConfig(RCC_SYSCLK_Div1);													//AHB=System Clock
	RCC_PCLK1Config(RCC_HCLK_Div1);														//APB1=AHB=System Clock
	RCC_PCLK2Config(RCC_HCLK_Div1);														//APB2=AHB=System Clock
	RCC_MCOConfig(RCC_MCO_PLLCLK_Div2);												//PLL输出2分频
}
