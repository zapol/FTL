#ifndef MOTORS_H_INCLUDED
#define MOTORS_H_INCLUDED

#include <stdint.h>

#define PIN_MOTOR1  0x0004
#define PIN_MOTOR2  0x0020
#define PIN_MOTOR3  0x0040
#define PIN_MOTOR4  0x0080
#define PORT_MOTOR  GPIOB

#define PIN_PWMA    0x0100
#define PIN_PWMB    0x0200
#define PORT_PWM    GPIOA

void motorsInit(void);
void setSpeed(int16_t eng1,int16_t eng2);

#endif // MOTORS_H_INCLUDED
