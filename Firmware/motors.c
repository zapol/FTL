#include "motors.h"
#include "stm32f10x.h"

void motorsInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = PIN_PWMA|PIN_PWMB;
    GPIO_Init(PORT_PWM, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = PIN_MOTOR1 | PIN_MOTOR2 | PIN_MOTOR3 | PIN_MOTOR4;
    GPIO_Init(PORT_MOTOR, &GPIO_InitStructure);

    TIM1->CR1 = 0;
    TIM1->CR2 = 0;
    TIM1->SMCR = 0;
    TIM1->DIER = 0;
    TIM1->EGR = 0;
    TIM1->CCMR1 = (6<<4)|(6<<12);       // OC1 and OC2 in PWM mode 1
    //TIM1->CCMR2 = (6<<4)|(6<<12);       // OC3 and OC4 in PWM mode 1
    TIM1->BDTR = TIM_BDTR_MOE;
    TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;
    TIM1->PSC = 0x00;
    TIM1->ARR = 0xff;
    TIM1->CR1 |= TIM_CR1_CEN;
}

void setSpeed(int16_t eng1,int16_t eng2)
{
    if(eng1>0)
    {
        PORT_MOTOR->BSRR=PIN_MOTOR1|(PIN_MOTOR2<<16);
        TIM1->CCR1 = eng1&0xff;
    }
    else
    {
        PORT_MOTOR->BSRR=PIN_MOTOR2|(PIN_MOTOR1<<16);
        TIM1->CCR1 = (-eng1)&0xff;
    }
    if(eng2>0)
    {
        PORT_MOTOR->BSRR=PIN_MOTOR3|(PIN_MOTOR4<<16);
        TIM1->CCR2 = eng2&0xff;
    }
    else
    {
        PORT_MOTOR->BSRR=PIN_MOTOR4|(PIN_MOTOR3<<16);
        TIM1->CCR2 = (-eng2)&0xff;
    }
}
