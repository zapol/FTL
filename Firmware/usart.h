#ifndef USART_H_INCLUDED
#define USART_H_INCLUDED

#include "stm32f10x.h"

typedef struct
{
    uint8_t *buf;
    uint16_t len;
    DMA_Channel_TypeDef *dmaChannel;
    uint16_t dmaHead;
    uint16_t head;
    uint16_t tail;
    uint8_t full;
} dmaUSART_BufferStruct;

typedef struct
{
    USART_TypeDef *usart;
    void (*irqFun)(void*);
    void *rxStruct;
    void *txStruct;
} USART_struct;

extern USART_struct *U1;
extern USART_struct *U2;
extern USART_struct *U3;

void dmaUsartInit(USART_struct *usart, int baudrate);
void dmaUsartHandler(void *usart);
int dmaUsartPutch(USART_struct *usart, uint8_t c);
int dmaUsartGetch(USART_struct *usart);
void dmaUsartTxTrigger(USART_struct *u);

void intUsartInit(USART_struct *usart, int baudrate);
void intUsartHandler(void *usart);
int intUsartPutch(USART_struct *usart, uint8_t c);
int intUsartGetch(USART_struct *usart);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);

void DMA1_Channel4_IRQHandler(void);
void DMA1_Channel7_IRQHandler(void);
void DMA1_Channel2_IRQHandler(void);
#endif // USART_H_INCLUDED
