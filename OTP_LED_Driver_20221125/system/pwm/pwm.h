#include "sys.h"
#ifndef __PWM_H
#define __PWM_H

void timer2_init(u16 arr,u16 psc);
void timer3_init(u16 arr,u16 psc);
void timer4_init(u16 arr,u16 psc);

void timer6_NVIC_config(void);
void timer6_config(void);
	

#endif
