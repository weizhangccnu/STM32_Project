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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);	 				//enable GPIOA clock					
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);					//USART3 clock enable
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3,ENABLE);						//USART3 pin full mapping
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;											//MCO output pin 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//GPIO output mode
	GPIO_Init(GPIOA, &GPIO_InitStructure);												//Init GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;											//MCO output pin 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;							//GPIO output mode
	GPIO_Init(GPIOC, &GPIO_InitStructure);												//Init GPIOC
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;											//UART TX pin ----- PD8
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;								//GPIO output mode
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//Init GPIOD
	/*Note: The different GPIO Mode of IO pin should initial respectively.*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;											//UART RX pin ----- PD9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//GPIO speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	  			//GPIO input mode
	GPIO_Init(GPIOD, &GPIO_InitStructure);												//Init GPIOD
}
/***********************************************************************************/
//UART configuration
/***********************************************************************************/
void UART_Config()
{
	USART_InitTypeDef USART_InitStruct;												//define usart struct
	USART_ClockInitTypeDef USART_ClockInitStructure;					//define usart clockinit struct
	USART_InitStruct.USART_BaudRate = 9600;										//BaudRate 9600
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;//Sent data mode and Receive data mode
	USART_InitStruct.USART_Parity = USART_Parity_No;					//No parity bit
	USART_InitStruct.USART_StopBits = USART_StopBits_1;				//1 bit stopbit
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//eight bits data
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;//????????
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;//?????????????
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;//?????????????SCLK??
	USART_ClockInit(USART3, &USART_ClockInitStructure);
	
	USART_Init(USART3,&USART_InitStruct);											//Initial USART3
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 						//
	USART_Cmd(USART3, ENABLE);
}
/***********************************************************************************/
//NVIC configuration
/***********************************************************************************/
void NVIC_Config()
{
	NVIC_InitTypeDef  NVIC_InitStructure;												//define NVIC struct
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);							//interrupt group
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;						//??????2??????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/***********************************************************************************/
//main function
/***********************************************************************************/
int main()
{

	RCC_Config();
	GPIO_Config();
	UART_Config();
	NVIC_Config();
	Delay_Init();
	while(1)
	{
//		GPIO_ResetBits(GPIOC,GPIO_Pin_7);													//reset GPIOC_7
//		Delay_ms(50);																						//delay 100ms
//		GPIO_SetBits(GPIOC,GPIO_Pin_7);														//set GPIOC_7

	}
}

/***********************************************************************************/
//interrupt function
/***********************************************************************************/
void USART3_IRQHandler(void)
{
	static u8 i=0;
	u8 re_data = 0;
	if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET) 
	{
		re_data = USART_ReceiveData(USART3);
		if(re_data == 'a')
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_7);													//reset GPIOC_7
			Delay_ms(10);																							//delay 100ms
			GPIO_SetBits(GPIOC,GPIO_Pin_7);														//set GPIOC_7
			Delay_ms(10);																							//delay 100ms
		}
		//Delay_ms(500);																						//delay 100ms
		i++;
		if(i>=255)
		{
			i = 0;
		}
		USART_SendData(USART3, i);
	}
	USART_ClearFlag(USART3,USART_FLAG_RXNE);
}
