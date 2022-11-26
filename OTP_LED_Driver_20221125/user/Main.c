#include "main.h"
#include "usart.h"
#include "math.h"
#include "pwm.h"
#include "key.h"
#include "led.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "delay.h"
#include "chromameter.h"

u8 buad[32]={0xFF,0xC7,0x83,0xCC,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
			 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

//------------------------------------------------------------------------------------------------------//
int main(void)
{
	u8 res;	
	delay_init();
	uart_init(115200);
	printf("SystemClk:%d\r\n",SystemCoreClock);	
	CH375_GPIO_Config();
	res=CH375_Init_Host();
	if( res!=USB_INT_SUCCESS) 
	{
		printf("ch375 init erro\n");
	}	
	delay_ms( 200 );  /* 有些USB设备要等待数百毫秒才能正常工作 */
	CH375InitSysVar();
	
	// LED Display Initial
	LCD_Init();								// initial LCD
	 
	LCD_direction(1);
	Show_Str(140, 0, RED, WHITE, "Wuhan Textile University", 16,0);
	Show_Str(10, 24, BLUE, YELLOW, "Channel Select:", 16,0);
	Show_Str(10, 48, BLUE, WHITE, "Channel 1:", 16,0);
	Show_Str(10, 72, BLUE, WHITE, "Channel 2:", 16,0);
	Show_Str(10, 96, BLUE, WHITE, "Channel 3:", 16,0);
	Show_Str(10, 120, BLUE, WHITE, "Channel 4:", 16,0);
	Show_Str(10, 144, BLUE, WHITE, "Channel 5:", 16,0);
	Show_Str(10, 168, BLUE, WHITE, "Channel 6:", 16,0);
	Show_Str(10, 192, BLUE, WHITE, "Channel 7:", 16,0);
	Show_Str(10, 216, BLUE, WHITE, "Channel 8:", 16,0);
	
	// Timer Initial 
	timer3_init(799,0);						// TIM3 init			
	TIM_SetCompare1(TIM3,799);				// TIM3_CH1 PC6 --> Channel 6
	TIM_SetCompare2(TIM3,799);				// TIM3_CH2 PC7 --> Channel 7
	TIM_SetCompare3(TIM3,799);				// TIM3_CH3 PC8 --> Channel 8
	
	timer4_init(799,0);						// TIM4 init			
	TIM_SetCompare1(TIM4,799);				// TIM3_CH1 PD12 --> Channel 2
	TIM_SetCompare2(TIM4,799);				// TIM3_CH2 DD13 --> Channel 3
	TIM_SetCompare3(TIM4,799);				// TIM3_CH3 PD14 --> Channel 4
	TIM_SetCompare4(TIM4,799);				// TIM3_CH3 PD15 --> Channel 5
	
	timer2_init(799,0);						// TIM2 init
	TIM_SetCompare3(TIM2,798);				// TIM2_CH3 PB10 --> Channel 1
	
	timer6_NVIC_config();					// TIM6 NVIC config
	timer6_config();						// TIM6 configuration
	
	// Key Initial 
	key_pin_init();							// key pin initial
	key_interrupt_init();					// key interrupt initial 
	
	// Delay Initial
	delay_init();
	
	// Buzzer Initial
	buzzer_pin_init();
	
	// Configure CH375
	ch375_config();
	
	while(1)
	{

	}
}

