/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "utask.h"
#include "debug.h"
#include "maint_usart.h"
#include "pprintf.h"
#include "motors.h"

/* Private function prototypes -----------------------------------------------*/
void NVIC_Configuration(void);
/* Private functions ---------------------------------------------------------*/

void testTask(utask *t);

extern struct content_s *content;

void testTask(utask *t)
{


    switch(t->istate)
    {
    case 0:
        setSpeed(0xff,0);
        break;
    case 1:
        setSpeed(-0xff,0);
        break;
    case 2:
        setSpeed(0,0xff);
        break;
    case 3:
        setSpeed(0,-0xff);
        break;
    }


    t->istate++;
    t->istate&=0x03;
    utask_sleep(t,1000);
}

int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock/1000);
    //NVIC_Configuration();
    utask_init();

    //maintInit();
    motorsInit();
    setSpeed(0,0);
    utask_add(testTask);

    //debug("FTL on the run!\r\n");

    while(1)
    {
        utask_schedule();
    }
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
/*
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
*/
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None

  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    RCC->APB2ENR |= 0x10 | 0x04;
    GPIOC->CRH = 0x11;
    GPIOA->CRL = 0x04;
    while (1)
    {
//        delay(1000000);
        GPIOC->BSRR = 1<<8 | 1<<(9+16) ;
//        delay(1000000);
        GPIOC->BSRR = 1<<(8+16) | 1<<9 ;
    }

}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
