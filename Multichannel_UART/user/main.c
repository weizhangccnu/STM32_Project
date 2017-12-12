#include "stm32f10x.h"
#include "RCC_Config.h"
#include "delay.h"
#include <stdio.h>
/***********************************************************************************/
//gpio configuration
/***********************************************************************************/
void GPIO_Config()
{
	GPIO_InitTypeDef  GPIO_InitStructure;													//define GPIO initial struct
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 				//enable GPIO clock					
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);					//enable periph clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;											//MCO output pin 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//GPIO output mode
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//Init GPIOA
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 				//enable GPIO clock	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;											//MCO output pin 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							//GPIO output mode
	GPIO_Init(GPIOC, &GPIO_InitStructure);												//Init GPIOC
}

/***********************************************************************************/
//main function
/***********************************************************************************/
int main()
{
	RCC_Config();
	GPIO_Config();
	Delay_Init();
	while(1)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_7);													//reset GPIOC_7
		Delay_ms(100);																						//delay 100ms
		GPIO_SetBits(GPIOC,GPIO_Pin_7);														//set GPIOC_7
		Delay_ms(100);																						//delay 100ms
	}
}
