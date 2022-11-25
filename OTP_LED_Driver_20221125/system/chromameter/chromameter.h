#include "sys.h"
#ifndef __CHROMAMETER_H
#define __CHROMAMETER_H

void ch375_config();							// CH375 Configuration
u8* read_chromameter(u8 type);					// read chromameter
void value_conversion(u8* array, u8 type, float *val1, float *val2, float *val3);	// value conversion
void Tcp_to_xy(float Tcp, float *x, float *y);	

void Calculate_Duty12(float Ev_mix, float Tcp_mix, float *Duty_ch1, float *Duty_ch2);

#endif
