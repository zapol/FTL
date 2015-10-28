#ifndef MAINT_USART_H_INCLUDED
#define MAINT_USART_H_INCLUDED

#include "usart.h"
#include "utask.h"

void maintInit();
void maintPutch(int c);
int maintGetch();
void maintTask(utask *t);
#endif // MAINT_USART_H_INCLUDED
