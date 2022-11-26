#include "sys.h"
#include "pwm.h"
#include "led.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"

u16 timer = 0;

volatile extern u16 ch8_num, ch7_num, ch6_num, ch5_num, ch4_num, ch3_num, ch2_num, ch1_num;
volatile extern u8 ch_select;
//-----------------------------------------------------------------------------//
void timer2_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;					// GPIO structure
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	// enable TIM2 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);		// enable GPIOC and AFIO clock 
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);		// TIM3 Pin full remap
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PB10-->TIM2_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr; 				//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 				//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 			//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 		//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);						//根据T指定的参数初始化外设TIM3 OC3

	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);				//使能TIM2在CCR2上的预装载寄存器
	TIM_Cmd(TIM2, ENABLE);  										//使能TIM2
}

//-----------------------------------------------------------------------------//
void timer3_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;					// GPIO structure
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	// enable TIM3 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE);		// enable GPIOC and AFIO clock 
	
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE);		// TIM3 Pin full remap
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8; //PC6-->TIM3_CH1, PC7-->TIM3_CH2, PC8-->TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

//-----------------------------------------------------------------------------//
void timer4_init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;											// GPIO structure
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);							// enable TIM3 clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);		// enable GPIOC and AFIO clock 
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);									// TIM3 Pin full remap
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15; 	//PC6-->TIM3_CH1, PC7-->TIM3_CH2, PC8-->TIM3_CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	//初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel1-4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 		//输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);  						//根据T指定的参数初始化外设TIM3 OC2
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);

	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}
//-----------------------------------------------------------------------------//
void timer6_NVIC_config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;							// define NVIC initial struct
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);			
	NVIC_InitStruct.NVIC_IRQChannel = TIM6_IRQn;				// Interrupt source
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;				// Enable Interrupt
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x02;	// 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x03;			
	NVIC_Init(&NVIC_InitStruct);								// initial 
}
//-----------------------------------------------------------------------------//
void timer6_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStruct.TIM_Period = (10000-1);
	TIM_TimeBaseInitStruct.TIM_Prescaler = (72-1);
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	
	TIM_Cmd(TIM6, ENABLE);
}	

//-----------------------------------------------------------------------------//
void TIM6_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM6, TIM_IT_Update))
	{
		timer++;
		if(timer == 80)
		{
			timer = 0;
			LCD_ShowNum(127, 24, ch_select, 2, 16);					// channel select
			LCD_ShowNum(92, 48, 799-ch1_num, 3, 16);				// channel one number
			LCD_ShowNum(92, 72, 799-ch2_num, 3, 16);				// channel two number
			LCD_ShowNum(92, 96, 799-ch3_num, 3, 16);				// channel three number
			LCD_ShowNum(92, 120, 799-ch4_num, 3, 16);				// channel four number
			LCD_ShowNum(92, 144, 799-ch5_num, 3, 16);				// channel five number
			LCD_ShowNum(92, 168, 799-ch6_num, 3, 16);				// channel six number
			LCD_ShowNum(92, 192, 799-ch7_num, 3, 16);				// channel seven number
			LCD_ShowNum(92, 216, 799-ch8_num, 3, 16);				// channel eight number
			
			TIM_SetCompare3(TIM3,799-ch8_num);						// TIM3_CH3 PC8  --> Channel 8
			TIM_SetCompare2(TIM3,799-ch7_num);						// TIM3_CH3 PC7  --> Channel 7
			TIM_SetCompare1(TIM3,799-ch6_num);						// TIM3_CH3 PC6  --> Channel 6
			
			TIM_SetCompare4(TIM4,799-ch5_num);						// TIM4_CH4 PD15 --> Channel 5
			TIM_SetCompare3(TIM4,799-ch4_num);						// TIM4_CH4 PD14 --> Channel 4
			TIM_SetCompare2(TIM4,799-ch3_num);						// TIM4_CH4 PD13 --> Channel 3
			TIM_SetCompare1(TIM4,799-ch2_num);						// TIM4_CH4 PD12 --> Channel 2
			
			TIM_SetCompare3(TIM2,799-ch1_num);						// TIM2_CH3 PB10 --> Channel 1
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
}
