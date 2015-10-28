#include "usart.h"
#include "stm32f10x.h"
#include "utils.h"

USART_struct USART1_struct={USART1,0,0,0};
USART_struct USART2_struct={USART2,0,0,0};
USART_struct USART3_struct={USART3,0,0,0};

USART_struct *U1=&USART1_struct;
USART_struct *U2=&USART2_struct;
USART_struct *U3=&USART3_struct;

void dmaUsartInit(USART_struct *u, int baudrate)
{
    dmaUSART_BufferStruct *txStruct,*rxStruct;
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(u->usart, &USART_InitStructure);
    // Set standard USART ISR

    u->irqFun = dmaUsartHandler;

    txStruct = (dmaUSART_BufferStruct*)u->txStruct;
    rxStruct = (dmaUSART_BufferStruct*)u->rxStruct;

    if(u->usart == USART1)
    {
        if(u->txStruct) txStruct->dmaChannel = DMA1_Channel4;
        if(u->rxStruct) rxStruct->dmaChannel = DMA1_Channel5;
    }
    else if(u->usart == USART2)
    {
        if(u->txStruct) txStruct->dmaChannel = DMA1_Channel7;
        if(u->rxStruct) rxStruct->dmaChannel = DMA1_Channel6;
    }
    else if(u->usart == USART3)
    {
        if(u->txStruct) txStruct->dmaChannel = DMA1_Channel2;
        if(u->rxStruct) txStruct->dmaChannel = DMA1_Channel3;
    }

    RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE);
    if(u->rxStruct)
    {
        rxStruct->dmaChannel->CPAR = (uint32_t)&(u->usart->DR);
        rxStruct->dmaChannel->CMAR = (uint32_t)(rxStruct->buf);
        rxStruct->dmaChannel->CNDTR = rxStruct->len;
        rxStruct->dmaChannel->CCR =    DMA_DIR_PeripheralSRC | \
                                DMA_PeripheralInc_Disable | \
                                DMA_MemoryInc_Enable | \
                                DMA_PeripheralDataSize_Byte | \
                                DMA_MemoryDataSize_Byte | \
                                DMA_Mode_Circular | \
                                DMA_Priority_VeryHigh | \
                                DMA_M2M_Disable;
        USART_DMACmd(u->usart,USART_DMAReq_Rx,ENABLE);
        rxStruct->dmaChannel->CCR |= DMA_CCR1_EN;
    }

    if(u->txStruct)
    {
        txStruct->dmaChannel->CPAR = (uint32_t)&(u->usart->DR);
        txStruct->dmaChannel->CCR =    DMA_DIR_PeripheralDST | \
                                DMA_PeripheralInc_Disable | \
                                DMA_MemoryInc_Enable | \
                                DMA_PeripheralDataSize_Byte | \
                                DMA_MemoryDataSize_Byte | \
                                DMA_Mode_Normal | \
                                DMA_Priority_VeryHigh | \
                                DMA_CCR7_TCIE | \
                                DMA_M2M_Disable;
        USART_DMACmd(u->usart,USART_DMAReq_Tx,ENABLE);
    }

    // Enable USART
    USART_Cmd(u->usart, ENABLE);
}

void intUsartInit(USART_struct *u, int baudrate)
{
    USART_InitTypeDef USART_InitStructure;

    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(u->usart, &USART_InitStructure);
    // Set standard USART ISR

    u->irqFun = intUsartHandler;

    // Enable the USART Interrupts
    USART_ITConfig(u->usart, USART_IT_RXNE, ENABLE);
    USART_ITConfig(u->usart, USART_IT_TXE, ENABLE);

    // Enable USART
    USART_Cmd(u->usart, ENABLE);
}

inline void dmaUsartHandler(void *usart)
{
    USART_struct *u;
    dmaUSART_BufferStruct *txStruct;
    u=usart;
    txStruct = u->txStruct;

    txStruct->tail = txStruct->dmaHead;
    txStruct->dmaChannel->CCR &= ~DMA_CCR1_EN;
    if(u==U1)DMA_ClearFlag(DMA1_FLAG_TC4);
    if(u==U2)DMA_ClearFlag(DMA1_FLAG_TC7);
    if(u==U3)DMA_ClearFlag(DMA1_FLAG_TC2);
    dmaUsartTxTrigger(u);
}

int dmaUsartPutch(USART_struct *u, uint8_t c)
{
    dmaUSART_BufferStruct *txStruct;
    txStruct = u->txStruct;

    if(!u->txStruct) return -1;
    if(txStruct->full)return -1;

    txStruct->buf[txStruct->head++]=c;
    if(txStruct->head == txStruct->len) txStruct->head=0;
    if(txStruct->head == txStruct->tail) txStruct->full=1;

    return 0;
}

int dmaUsartGetch(USART_struct *u)
{
    int c;
    dmaUSART_BufferStruct *rxStruct;
    rxStruct = u->rxStruct;

    rxStruct->dmaHead = rxStruct->len - rxStruct->dmaChannel->CNDTR;
    if(rxStruct->dmaHead == rxStruct->len) rxStruct->dmaHead = 0;
    if(rxStruct->tail == rxStruct->dmaHead) return -1;

    c = rxStruct->buf[rxStruct->tail++];
    if(rxStruct->tail == rxStruct->len) rxStruct->tail=0;

    return c;
}

void dmaUsartTxTrigger(USART_struct *u)
{
    dmaUSART_BufferStruct *txStruct;
    txStruct = u->txStruct;

    if(txStruct->dmaChannel->CCR & DMA_CCR1_EN) return;

    if(txStruct->head == txStruct->tail && !txStruct->full) return;

    txStruct->dmaChannel->CMAR = (uint32_t)&(txStruct->buf[txStruct->tail]);

    if(txStruct->head <= txStruct->tail)
    {
        txStruct->dmaChannel->CNDTR = txStruct->len - txStruct->tail;
        txStruct->dmaHead = 0;
    }

    else
    {
        txStruct->dmaChannel->CNDTR = txStruct->head - txStruct->tail;
        txStruct->dmaHead = txStruct->head;
    }

    txStruct->full=0;
    txStruct->dmaChannel->CCR |= DMA_CCR1_EN;
}

inline void intUsartHandler(void *usart)
{
    int c;
    USART_struct *u;
    u=usart;

    if(USART_GetITStatus(u->usart, USART_IT_RXNE) != RESET)
    {
        if(u->rxStruct==0)return;

        if(fifoFree(u->rxStruct)==0)
        {
            return;
        }
        fifo8Write(u->rxStruct, USART_ReceiveData(u->usart));
    }

    if(USART_GetITStatus(u->usart, USART_IT_TXE) != RESET)
    {
        c = fifo8Read(u->txStruct);
        if(c != -1)
        {
            USART_SendData(u->usart,c);
        }
        else
        {
            USART_ITConfig(u->usart,USART_IT_TXE,DISABLE);
        }
    }
//    USART_ClearFlag(u->usart, USART_FLAG_TC);
}

int intUsartPutch(USART_struct *usart, uint8_t c)
{
    int n;
    n=fifo8Write(usart->txStruct,c);
    USART_ITConfig(usart->usart, USART_IT_TXE,ENABLE);
    return n;
}

int intUsartGetch(USART_struct *usart)
{
    return fifo8Read(usart->rxStruct);
}


void USART1_IRQHandler(void)
{
    U1->irqFun(U1);
}

void USART2_IRQHandler(void)
{
    U2->irqFun(U2);
}

void USART3_IRQHandler(void)
{
    U3->irqFun(U3);
}

void DMA1_Channel4_IRQHandler(void)
{
    U1->irqFun(U1);
}

void DMA1_Channel7_IRQHandler(void)
{
    U2->irqFun(U2);
}

void DMA1_Channel2_IRQHandler(void)
{
    U3->irqFun(U3);
}
