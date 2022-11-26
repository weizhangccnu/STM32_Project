#include "sys.h"
#ifndef __KEY_H
#define __KEY_H

void key_pin_init(void);	
void key_interrupt_init(void);

void buzzer_pin_init(void);
void buzzer_done(u8 time);

#endif
