#include "maint_usart.h"

#include "usart.h"
#include "utils.h"
//#include "kaseta.h"
//#include "content.h"

#define MAINT_TXDEPTH   9
#define MAINT_TXLEN     (1<<MAINT_TXDEPTH)
#define MAINT_RXDEPTH   6
#define MAINT_RXLEN     (1<<MAINT_RXDEPTH)

uint8_t maint_txbuf[MAINT_TXLEN];
uint8_t maint_rxbuf[MAINT_RXLEN];

fifo_t maint_txBufStruct = {maint_txbuf,1,MAINT_TXLEN,0,0,0,0,0};
fifo_t maint_rxBufStruct = {maint_rxbuf,1,MAINT_RXLEN,0,0,0,0,0};

void maintInit()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_struct *u;
    u=U1;

    u->txStruct = &maint_txBufStruct;
    u->rxStruct = &maint_rxBufStruct;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = (1<<9);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = (1<<10);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    intUsartInit(u, 115200);
    utask_add(maintTask);
}

void maintPutch(int c)
{
    intUsartPutch(U1,c);
}

int maintGetch()
{
    return intUsartGetch(U1);
}

void maintTask(utask *t)
{
    int c;

    static int freq=600;

    c=maintGetch();

    if(c>=0)
    {
//        debug("Echo: %c\r\n",c);
        if(c>='0' && c<='9') pressButton(c-'0');
        else if(c=='C' || c=='c') pressButton(10);
        else if(c=='K' || c=='k') pressButton(11);
        else if(c=='+') TIM17->ARR=freq++;
        else if(c=='-') TIM17->ARR=freq--;
        else if(c=='l'||c=='L') xmodemDownloadContent();
    }

}
