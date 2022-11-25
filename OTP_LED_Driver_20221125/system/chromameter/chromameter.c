#include "math.h"
#include "delay.h"
#include "test.h"
#include "CH375.h"

u8		UserBuffer[64];

u8 config_command[13][8]={{0x40, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
						  {0xc0, 0x05, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00},
						  {0x40, 0x04, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x03, 0x38, 0x41, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x03, 0x38, 0x41, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x01, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x04, 0x07, 0x02, 0x00, 0x00, 0x00, 0x00},
						  {0x40, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

u8 command_link[14] = {0x02, 0x30, 0x30, 0x35, 0x34, 0x31, 0x20, 0x20, 0x20, 0x03, 0x31, 0x33, 0x0d, 0x0a};				// link command
u8 command_hold[14] = {0x02, 0x39, 0x39, 0x35, 0x35, 0x31, 0x20, 0x20, 0x30, 0x03, 0x30, 0x32, 0x0d, 0x0a};				// hold command
u8 command_setEXT[14] = {0x02, 0x30, 0x30, 0x34, 0x30, 0x31, 0x30, 0x20, 0x20, 0x03, 0x30, 0x36, 0x0d, 0x0a};			// set Execute command
u8 command_measure[14] = {0x02, 0x39, 0x39, 0x34, 0x30, 0x32, 0x31, 0x20, 0x20, 0x03, 0x30, 0x34, 0x0d, 0x0a};			// measure command
u8 command_EvXY[14] = {0x02, 0x30, 0x30, 0x30, 0x32, 0x31, 0x33, 0x30, 0x30, 0x03, 0x30, 0x33, 0x0d, 0x0a};				// ExXY command
u8 command_EvTcp[14] = {0x02, 0x30, 0x30, 0x30, 0x38, 0x31, 0x33, 0x30, 0x30, 0x03, 0x30, 0x39, 0x0d, 0x0a};			// EvTcp command 

//------------------------------------------------------------------------------------------------------//
void ch375_config()
{
	u8 res, m, j;
	u16 l;
	if(CH375CheckConnect() == USBD_CONNECT)          /* 刚检测到一个设备接入，需要枚举 */
		{
			printf ("Device Connected\n");                         
			//开始枚举操作		
			res = CH375BusReset();                   /* 总线复位 */		
			if( res != USB_INT_SUCCESS ) 
			{
				printf("Bus Reset Erro:0x%02x\r\n",(uint16_t)res);
			}
			delay_ms( 50 );                          /* 等待设备稳定 */
 
			/* 获取设备描述符 */			
			res = CH375GetDeviceDesc(UserBuffer,&l); 
			if( res == USB_INT_SUCCESS )
			{				
				for( m = 0; m < l; m++ )
				{	
					printf("0x%02x ",(uint16_t)UserBuffer[m]);
				}
				printf ("\n");
			}
			else 
			{
				printf("Get Device Descr Erro:0x%02x\n",(uint16_t)res );
			}
		
			/* 获取配置描述符 */
			res = CH375GetConfDesc( UserBuffer,&l); 
			if( res== USB_INT_SUCCESS )
			{							
				for( m = 0; m < l; m++ )
				{
					printf("0x%02x ",(uint16_t)UserBuffer[m]);
				}
				printf ("\n");					
			}
			else 
			{
				printf ("Get Conf Descr Erro:0x%02x\n",(uint16_t)res );	
			}				
			/*获取奇怪的描述符*/
			
			res = CH375GetUnknowDesc(UserBuffer, &l); 
			if( res== USB_INT_SUCCESS )
			{							
				for( m = 0; m < l; m++ )
				{
					printf("0x%02x ",(uint16_t)UserBuffer[m]);
				}
				printf ("\n");					
			}
			else
			{				
				printf ("Get Unknown Descr Erro:0x%02x\n",(uint16_t)res );
			}				
			
			/* 设置配置 */
			res = CH375SetDeviceConf( 1 );
			if( res != USB_INT_SUCCESS ) 
			{
				printf("Set Config Erro\n");
			}
			/* 设置配置 */
			res = CH375SetBaudConf( 1 );
			if( res != USB_INT_SUCCESS ) 
			{
				printf("Set Config Baud Erro\n");
			}
			
			for(j=0; j<13; j++)					// config UART speed, data width and so on
			{
				CH375CtlTrans(config_command[j], &UserBuffer, 8);
				delay_ms(1);
			}
		}
}

//------------------------------------------------------------------------------------------------------//
/*
* param[in] type: 0: measure Evxy value, 1: measure EvTcp value
* return: read back an array with 34 byte from CL-200A Chromameter.
*/
u8* read_chromameter(u8 type)
{
	u8 res,m,n,k;
	u16 l;
	static u8 Read_Value[34];
	
	if( USBD.status == USBD_READY )     //设备初始化已完成
	{
		//根据设备信息结构体，找中断端点，并对其发送IN包
		CH375OutTrans(2,command_link,14,1000);					// send link command
		delay_ms(100);
		
		for( m=0; m!=USBD.itfmount; m++)						// How many usb device was mounted at CH375B host
		{				
			delay_ms(10);
			for(n=0; n!=USBD.itf[m].edpmount; n++)				// how many endpoint mounted at CH375B host
			{
				res = CH375InTrans( USBD.itf[m].edp[n].edpnum & 0x0F, UserBuffer, &l, 0);     	//对端点发IN包,NAK不重试					
			}			
		}
		
		CH375OutTrans(2,command_hold,14,1000);					// send hold command
		delay_ms(50);
		
		CH375OutTrans(2,command_setEXT,14,1000);				// send set EXT command
		delay_ms(50);
		
		for( m=0; m!=USBD.itfmount; m++)						// How many usb device was mounted at CH375B host
		{				
			delay_ms(10);
			for(n=0; n!=USBD.itf[m].edpmount; n++)				// how many endpoint mounted at CH375B host
			{
				res = CH375InTrans( USBD.itf[m].edp[n].edpnum & 0x0F, UserBuffer, &l, 0);     	//对端点发IN包,NAK不重试					
			}			
		}
		
		CH375OutTrans(2,command_measure,14,1000);				// send measure command
		delay_ms(50);
		
		if(type == 0)											// Read Evxy value
		{
			CH375OutTrans(2,command_EvXY,14,1000);					// send EvTcp command
			delay_ms(70);
			
			for( m=0; m!=USBD.itfmount; m++)						// How many usb device was mounted at CH375B host
			{				
				delay_ms(10);
				for(n=0; n!=USBD.itf[m].edpmount; n++)				// how many endpoint mounted at CH375B host
				{
					res = CH375InTrans( USBD.itf[m].edp[n].edpnum & 0x0F, UserBuffer, &l, 0);     	//对端点发IN包,NAK不重试
					if( res == USB_INT_SUCCESS)
					{
						for( k = 0; k < l; k++ )
						{
							Read_Value[k] = (u8)UserBuffer[k];
						}
					}					
				}			
			}
		}
		else
		{
			CH375OutTrans(2,command_EvTcp,14,1000);					// send EvTcp command
			delay_ms(70);
			
			for( m=0; m!=USBD.itfmount; m++)						// How many usb device was mounted at CH375B host
			{				
				delay_ms(10);
				for(n=0; n!=USBD.itf[m].edpmount; n++)				// how many endpoint mounted at CH375B host
				{
					res = CH375InTrans( USBD.itf[m].edp[n].edpnum & 0x0F, UserBuffer, &l, 0);     	//对端点发IN包,NAK不重试
					if( res == USB_INT_SUCCESS)
					{
						for( k = 0; k < l; k++ )
						{
							Read_Value[k] = (u8)UserBuffer[k];
						}
					}					
				}			
			}
		}
	}
	return Read_Value;
}
//------------------------------------------------------------------------------------------------------//
/*
* @param[in] type: 0: measure Evxy value, 1: measure EvTcp value
* @param[in] array: chromameter measured value
* @param[in] *val1: Ev
* @param[in] *val2: if type==0, val2 = x; else val2 = Tcp
* @param[in] *val3: if type==0, val3 = y; else val3 = delta(uv)
*/
void value_conversion(u8* array, u8 type, float *val1, float *val2, float *val3)
{
	u8 i;	
	u8 chrom_vale[34];
	u8 val1_array[6], val2_array[6], val3_array[6];
	
	for(i = 0; i < 34; i++)							// save data into array
	{
		chrom_vale[i] = *(array++);
	}
	for(i=0; i < 34; i++)
	{
		if(i > 10 && i <= 16)
		{
			val1_array[i-11] = chrom_vale[i];
			if(i==16)
			{
				if(val1_array[0] == 0x2b)					// sign is positive
				{
					if(val1_array[1] == 0x20)				// Bit4 is " "
					{
						if(val1_array[2] == 0x20)			// Bit3 is " "
						{
							*val1 = ((val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
						}
						else
						{
							*val1 = ((val1_array[2]-0x30)*100 + (val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
						}
					}
					else
					{
						*val1 = ((val1_array[1]-0x30)*1000 + (val1_array[2]-0x30)*100 + (val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
					}
				}
				else if(val1_array[0] == 0x2d)				// sign is negative
				{
					if(val1_array[1] == 0x20)
					{
						if(val1_array[2] == 0x20)
						{
							*val1 = ((val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
						}
						else
						{
							*val1 = ((val1_array[2]-0x30)*100 + (val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
						}
					}
					else
					{
						*val1 = ((val1_array[1]-0x30)*1000 + (val1_array[2]-0x30)*100 + (val1_array[3]-0x30)*10 + (val1_array[4]-0x30)) * pow(10,val1_array[5]-0x34);
					}
				}
			}
		}
		else if(i > 16 && i <= 22)
		{
			val2_array[i-17] = chrom_vale[i];
			if(i==22)
			{
				if(val2_array[0] == 0x2b)					// sign is positive
				{
					*val2 = ((val2_array[1]-0x30)*1000 + (val2_array[2]-0x30)*100 + (val2_array[3]-0x30)*10 + (val2_array[4]-0x30)) * pow(10,val2_array[5]-0x34);
				}
				else if(val2_array[0] == 0x2d)				// sign is negative
				{
					*val2 = -((val2_array[1]-0x30)*1000 + (val2_array[2]-0x30)*100 + (val2_array[3]-0x30)*10 + (val2_array[4]-0x30)) * pow(10,val2_array[5]-0x34);
				}
			}
		}
		else if(i > 22 && i <= 28)
		{
			val3_array[i-23] = chrom_vale[i];
			if(i==28)
			{
				if(val3_array[0] == 0x2b)					// sign is positive
				{
					*val3 = ((val3_array[1]-0x30)*1000 + (val3_array[2]-0x30)*100 + (val3_array[3]-0x30)*10 + (val3_array[4]-0x30)) * pow(10,val3_array[5]-0x34);
				}
				else if(val3_array[0] == 0x2d)				// sign is negative
				{
					*val3 = -((val3_array[1]-0x30)*1000 + (val3_array[2]-0x30)*100 + (val3_array[3]-0x30)*10 + (val3_array[4]-0x30)) * pow(10,val3_array[5]-0x34);
				}
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------//
/*
* @brife Tcp_to_xy()
* @param tcp
* @param x
* @param y
*/
void Tcp_to_xy(float Tcp, float *x, float *y)
{
	//https://github.com/TobiasWeis/planckBlackbodyColors
    //Kim et. al: US patent 7024034, Kim et al., "Color Temperature Conversion System and Method Using the Same", issued 2006-04-04
    //Approximation formula: https://en.wikipedia.org/wiki/Planckian_locus

    if (Tcp < 4000)
        *x = -0.2661239*(1e9 / pow(Tcp,3)) - 0.2343580*(1e6/pow(Tcp,2))+0.8776956*(1e3/Tcp) + 0.179910;
    else if(Tcp>=4000)
        *x = -3.0258469*(1e9 / pow(Tcp,3)) + 2.1070379*(1e6/pow(Tcp,2))+0.2226347*(1e3/Tcp) + 0.240390;

    //
    if (Tcp < 2222)
        *y = -1.1063814*(pow(*x,3)) - 1.34811020*(pow(*x,2)) + 2.18555832*(*x) - 0.20219683;
    else if( Tcp >= 2222 && Tcp < 4000)
        *y = -0.9549476*(pow(*x,3)) - 1.37418593*(pow(*x,2)) + 2.09137015*(*x) - 0.16748867;
    else if( Tcp >= 4000)
        *y = +3.0817580*(pow(*x,3)) - 5.87338670*(pow(*x,2)) + 3.75112997*(*x) - 0.37001483;
}

//------------------------------------------------------------------------------------------------------//
/*
* @brife Calculate_Duty12()
* @param Ev_mix: mixed Ev
* @param Tcp_mix: mixed Tcp
* @param *Duty_ch1: channel1 duty cycle
* @param *Duty_ch2: channel2 duty cycle
*/
void Calculate_Duty12(float Ev_mix, float Tcp_mix, float *Duty_ch1, float *Duty_ch2)
{
	float Ev_ch1 = 2197.0,  Ev_ch2 = 2319.0;					// channel 1 and 2 (lx)
//	float Tcp_ch1 = 2683.0, Tcp_ch2 = 3767.0;					// channe
	float x_ch1 = 0.4718,	x_ch2 = 0.3956;
	float y_ch1 = 0.4279,   y_ch2 = 0.3850;
	float R_ch1, R_ch2;
	float x_mix, y_mix;
	
	Tcp_to_xy(Tcp_mix, &x_mix, &y_mix);
	
	R_ch1 = Ev_ch1/y_ch1;
	R_ch2 = Ev_ch2/y_ch2;
	
	*Duty_ch1 = ((x_ch2 - x_mix)*R_ch2*Ev_mix)/((x_mix - x_ch1)*R_ch1*Ev_ch2 + (x_ch2 - x_mix)*R_ch2*Ev_ch1);
	*Duty_ch2 = (Ev_mix - (*Duty_ch1)*Ev_ch1)/Ev_ch2;
}
