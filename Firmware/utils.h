#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdint.h>

typedef struct
{
    void *buf;          // Data buffer
    uint32_t size;      // Size of element in FIFO (in bytes)
    uint32_t len;       // Number of elements in FIFO
    uint32_t rptr;      // Read pointer
    uint32_t rptr_r;    // Read revert pointer
    uint32_t wptr;      // Write pointer
    uint32_t wptr_r;    // Write revert pointer
    uint8_t full;       // FIFO full flag
} fifo_t;

void memcopy(uint8_t *dest, const uint8_t *src, uint32_t n);
void delay(int i);

int16_t fifo8Read_(fifo_t *fifo, uint8_t reversible);
int16_t fifo8Read(fifo_t *fifo);
int16_t fifo8RevRead(fifo_t *fifo);
int8_t fifo8Write_(fifo_t *fifo, uint8_t c, uint8_t reversible);
int8_t fifo8Write(fifo_t *fifo, uint8_t c);
int8_t fifo8RevWrite(fifo_t *fifo, uint8_t c);

int8_t fifoRead_(fifo_t *fifo, void *dst, uint8_t reversible);
int8_t fifoRead(fifo_t *fifo, void *dst);
int8_t fifoRevRead(fifo_t *fifo, void *dst);
int8_t fifoWrite_(fifo_t *fifo, void *c, uint8_t reversible);
int8_t fifoWrite(fifo_t *fifo, void *c);
int8_t fifoRevWrite(fifo_t *fifo, void *c);

void fifoFlush(fifo_t *fifo);
uint32_t fifoFree(fifo_t *fifo);
void fifoReadCommit(fifo_t *fifo);
void fifoReadRevert(fifo_t *fifo);
void fifoWriteCommit(fifo_t *fifo);
void fifoWriteRevert(fifo_t *fifo);

void sort(uint8_t* buf,int len);
char toUpper(char c);

#endif // UTILS_H_INCLUDED
