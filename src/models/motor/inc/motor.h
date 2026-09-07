#ifndef MOTOR_H
#define MOTOR_H

#include "stdint.h"

void MotorInit(void);
void SetPin(int8_t id, int8_t state);


#endif /* MOTOR_H */