#include "sys.h"
#include "delay.h"
#include "chromameter.h"
//-----------------------------------------------------------------------------//

u8 *p_value, type;
float value1, value2, value3;
float Ev, x, y, Tcp, uv;
float cal_x, cal_y;

float Duty_ch1, Duty_ch2;

#define buzzer_time 3
u16 ch8_num=0, ch7_num=0, ch6_num=0, ch5_num=0, ch4_num=0, ch3_num=0, ch2_num=0, ch1_num=0;
u8 ch_select=1;
//-----------------------------------------------------------------------------//
// PA15 --> Buzzer
void buzzer_pin_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;															// GPIO structure
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);											// GPIOA clock enable
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;														// Buzzer pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;												// Pull-up output mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;												// GPIO Speed
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
}

//-----------------------------------------------------------------------------//
void buzzer_done(u8 time)
{
	GPIO_SetBits(GPIOF, GPIO_Pin_0);
	delay_ms(time);
	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
}

//-----------------------------------------------------------------------------//
void key_pin_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;																			// GPIO structure
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);															// GPIOD clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);															// GPIOE clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);															// GPIOG clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);																// AFIO clock enable

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;																		// key pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																	// Pull-up input mode
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	// key pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																	// Pull-up input mode
	GPIO_Init(GPIOE, &GPIO_InitStructure);			
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;			// key pin
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;																	// Pull-up input mode
	GPIO_Init(GPIOG, &GPIO_InitStructure);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);					// Key_SETUP  --> PE0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource1);					// Key_DP     --> PE1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);					// Key_CH     --> PE2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);					// Key_XYMENU --> PE3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource4);					// Key_CLB 	  --> PE4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource5);					// Key_LEARN  --> PE5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource6);					// Key_ONOFF  --> PE6
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7);					// Key_CW     --> PE0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource9);					// Key_BRI    --> PE1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource10);				// Key_BRI-   --> PE2
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource11);				// Key_CW-    --> PE3
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource12);				// Key_BRI+   --> PE4
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource13);				// Key_CW+    --> PE5
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource15);				// Key_DN     --> PE6
}
//-----------------------------------------------------------------------------//
void key_interrupt_init()
{
	EXTI_InitTypeDef EXTI_InitStructure;																			// define EXTI structure
	NVIC_InitTypeDef NVIC_InitStructure;																			// define NVIC structure
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line1|EXTI_Line2|EXTI_Line3|EXTI_Line4|EXTI_Line5|EXTI_Line6 \
	|EXTI_Line7|EXTI_Line9|EXTI_Line10|EXTI_Line11|EXTI_Line12|EXTI_Line13|EXTI_Line15;								// EXTI Line
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;																// Interrupt mode 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;															// Falling edge trigger (This is very important)
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;																		// enable exit interrupt
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;							
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x06;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI0_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line0)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0) == RESET)										// Key_SETUP --> PE0
		{
			buzzer_done(buzzer_time);	
			
			type = 0;													// 0: Evxy meausrement, 1: EvTcp measurement
			p_value = read_chromameter(type);							// read CL-200A EvXY Value
			printf("\r\n");								
			value_conversion(p_value, type, &value1, &value2, &value3);	// value conversion
			if(type == 0)
			{
				printf("Ev: %0.1f\r\n", value1);
				printf("x: %0.4f\r\n", value2);
				printf("y: %0.4f\r\n", value3);
				Ev = value1;
				x = value2;
				y = value3;
			}
			else
			{
				printf("Ev: %0.1f\r\n", value1);
				printf("Tcp: %0.1f\r\n", value2);
				printf("uv: %0.4f\r\n", value3);
				Ev = value1;
				Tcp = value2;
				uv = value3;
			}
			
			type = 1;													// 0: Evxy meausrement, 1: EvTcp measurement
			p_value = read_chromameter(type);							// read CL-200A EvXY Value
			printf ("\r\n");								
			value_conversion(p_value, type, &value1, &value2, &value3);	// value conversion
			if(type == 0)
			{
				printf("Ev: %0.1f\r\n", value1);
				printf("x: %0.4f\r\n", value2);
				printf("y: %0.4f\r\n", value3);
				Ev = value1;
				x = value2;
				y = value3;
			}
			else
			{
				printf("Ev: %0.1f\r\n", value1);
				printf("Tcp: %0.1f\r\n", value2);
				printf("uv: %0.4f\r\n", value3);
				Ev = value1;
				Tcp = value2;
				uv = value3;
			}
			Tcp_to_xy(Tcp, &cal_x, &cal_y);								// convert Tcp into xy 
			printf("Calculated delta x: %0.4f\r\n", cal_x-x);
			printf("Calculated delta y: %0.4f\r\n", cal_y-y);
		}
		EXTI_ClearITPendingBit(EXTI_Line0);   														// clear interrupt flag bit
    }     
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI1_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line1)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == RESET)										// Key_DP --> PE1
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line1);   														// clear interrupt flag bit
    }     
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI2_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line2)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == RESET)										// Key_CH --> PE2
		{
			buzzer_done(buzzer_time);
			
			Calculate_Duty12(3000, 3100, &Duty_ch1, &Duty_ch2);
			printf("Duty Channel 1: %0.3f\r\n", Duty_ch1);
			printf("Duty Channel 2: %0.3f\r\n", Duty_ch2);
			
			ch1_num = (u16)(Duty_ch1 * (799-657));
			ch2_num = (u16)(Duty_ch2 * (799-659));
			
			printf("ch1_num: %d\r\n", ch1_num);
			printf("ch2_num: %d\r\n", ch2_num);
		}	
		EXTI_ClearITPendingBit(EXTI_Line2);   														// clear interrupt flag bit
    }     
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI3_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line3)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == RESET)										// Key_XYMENU --> PE3
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line3);   														// clear interrupt flag bit
    }     
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI4_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == RESET)										// Key_CLB --> PE4
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line4);   														// clear interrupt flag bit
    }     
}
//-----------------------------------------------------------------------------//					// Interrupt service function
void EXTI9_5_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line5)!=RESET)															// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_5) == RESET)										// Key_LEARN --> PE5
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line5);   														// clear interrupt flag bit
    } 
	
	else if(EXTI_GetITStatus(EXTI_Line6)!=RESET)													// check interrupt
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6) == RESET)										// Key_ONOFF --> PE6
		{
			buzzer_done(buzzer_time);
		}
		EXTI_ClearITPendingBit(EXTI_Line6);   														// clear interrupt flag bit
	}
	
	else if(EXTI_GetITStatus(EXTI_Line7)!=RESET)													// check interrupt
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7) == RESET)										// Key_CW --> PD7
		{
			buzzer_done(buzzer_time);
			ch_select++;
			if(ch_select > 8)
			{
				ch_select = 1;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line7);   														// clear interrupt flag bit
	}
	else if(EXTI_GetITStatus(EXTI_Line9)!=RESET)													// check interrupt
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_9) == RESET)										// Key_BRI --> PG9
		{
			buzzer_done(buzzer_time);
		}
		EXTI_ClearITPendingBit(EXTI_Line9);   														// clear interrupt flag bit
	}
}

void EXTI15_10_IRQHandler()
{
    if(EXTI_GetITStatus(EXTI_Line10)!=RESET)														// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_10) == RESET)										// Key_BRI- --> PG10
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line10);   														// clear interrupt flag bit
    } 
	
	else if(EXTI_GetITStatus(EXTI_Line11)!=RESET)													// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11) == RESET)										// Key_CW- --> PG11
		{
			buzzer_done(buzzer_time);
			if(ch_select == 8)
			{
				ch8_num = ch8_num + 2;
			}
			else if(ch_select == 7)
			{
				ch7_num = ch7_num + 2;
			}	
			else if(ch_select == 6)
			{
				ch6_num = ch6_num + 2;
			}
			else if(ch_select == 5)
			{
				ch5_num = ch5_num + 2;
			}
			else if(ch_select == 4)
			{
				ch4_num = ch4_num + 2;
			}
			else if(ch_select == 3)
			{
				ch3_num = ch3_num + 2;
			}
			else if(ch_select == 2)
			{
				ch2_num = ch2_num + 2;
			}
			else if(ch_select == 1)
			{
				ch1_num = ch1_num + 2;
			}
		}	
		EXTI_ClearITPendingBit(EXTI_Line11);   														// clear interrupt flag bit
    }
	
	else if(EXTI_GetITStatus(EXTI_Line12)!=RESET)													// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_12) == RESET)										// Key_BRI+ --> PG12
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line12);   														// clear interrupt flag bit
    }
	
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)													// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_13) == RESET)										// Key_CW+ --> PG13
		{
			buzzer_done(buzzer_time);
			if(ch_select == 8)
			{
				ch8_num = ch8_num - 2;
			}
			else if(ch_select == 7)
			{
				ch7_num = ch7_num - 2;
			}	
			else if(ch_select == 6)
			{
				ch6_num = ch6_num - 2;
			}
			else if(ch_select == 5)
			{
				ch5_num = ch5_num - 2;
			}
			else if(ch_select == 4)
			{
				ch4_num = ch4_num - 2;
			}
			else if(ch_select == 3)
			{
				ch3_num = ch3_num - 2;
			}
			else if(ch_select == 2)
			{
				ch2_num = ch2_num - 2;
			}
			else if(ch_select == 1)
			{
				ch1_num = ch1_num - 2;
			}
		}	
		EXTI_ClearITPendingBit(EXTI_Line13);   														// clear interrupt flag bit
    }
	
	else if(EXTI_GetITStatus(EXTI_Line15)!=RESET)													// check interrupt
    {	
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_15) == RESET)										// Key_DN --> PG15
		{
			buzzer_done(buzzer_time);
		}	
		EXTI_ClearITPendingBit(EXTI_Line15);   														// clear interrupt flag bit
    }
}