#ifndef _MPU6050_USERBUTTON_H
#define _MPU6050_USERBUTTON_H

#include "misc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_exti.h"

void user_button_Interrupts_Configure();
void change_mode();

#endif
