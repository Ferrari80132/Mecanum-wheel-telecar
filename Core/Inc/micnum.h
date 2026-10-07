#ifndef __MICNUM_H
#define __MICNUM_H

#include "stm32f1xx_hal.h"
#include "main.h"

void Car_Rotate_Left(void);
void Car_Rotate_Right(void);
void Car_Stop(void);
void Set_Motor_Speed(uint8_t motor, int speed);

#endif