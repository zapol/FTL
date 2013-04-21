#include "utils.h"


void memcopy(uint8_t *dest, const uint8_t *src, uint32_t n)
{
    while(n--) *dest++ = *src++;
}

void delay(int i)
{
    while (i-- > 0)
    {
        asm("nop");
    }
}

int16_t fifo8Read_(fifo_t *fifo, uint8_t reversible)
{
    uint8_t c;
    uint8_t *buf = fifo->buf;

    if(fifoFree(fifo)==fifo->len)   // Check if empty
    {
        //debug("FIFO EMPTY!\r\n");
        return -1;
    }
    c = buf[fifo->rptr];
    fifo->rptr++;
    fifo->rptr %= fifo->len;
    if(!reversible)
    {
        fifo->rptr_r = fifo->rptr;
        fifo->full = 0;
    }
    return c;
}

int16_t fifo8Read(fifo_t *fifo)
{
    return fifo8Read_(fifo, 0);
}

int16_t fifo8RevRead(fifo_t *fifo)
{
    return fifo8Read_(fifo, 1);
}

int8_t fifo8Write_(fifo_t *fifo, uint8_t c, uint8_t reversible)
{
    uint8_t *buf = fifo->buf;
    if(fifo->full)  // Check if full
    {
        return -1;
    }
    buf[fifo->wptr] = c;
    fifo->wptr++;
    fifo->wptr %= fifo->len;
    if(fifo->wptr == fifo->rptr)fifo->full=1;
    if(!reversible)fifo->wptr_r = fifo->wptr;
    return 0;
}

int8_t fifo8Write(fifo_t *fifo, uint8_t c)
{
    return fifo8Write_(fifo,c,0);
}

int8_t fifo8RevWrite(fifo_t *fifo, uint8_t c)
{
    return fifo8Write_(fifo,c,1);
}

void fifoFlush(fifo_t *fifo)
{
    fifo->wptr=0;
    fifo->rptr=0;
    fifo->wptr_r=0;
    fifo->rptr_r=0;
    fifo->full=0;
}

int8_t fifoRead_(fifo_t *fifo, void *dst, uint8_t reversible)
{
    int i;
    uint8_t *srcbuf = fifo->buf;
    uint8_t *dstbuf = dst;

    if(fifoFree(fifo)==fifo->len)   // Check if empty
    {
        //debug("Empty!\r\n");
        return -1;
    }

    for(i=0; i!=fifo->size; i++)
    {
        dstbuf[i] = srcbuf[fifo->rptr*fifo->size+i];
    }

    fifo->rptr++;
    fifo->rptr %= fifo->len;
    if(!reversible)
    {
        fifo->rptr_r = fifo->rptr;
        fifo->full = 0;
    }
    return 0;
}
int8_t fifoRead(fifo_t *fifo, void *dst)
{
    return fifoRead_(fifo,dst,0);
}
int8_t fifoRevRead(fifo_t *fifo, void *dst)
{
    return fifoRead_(fifo,dst,1);
}

int8_t fifoWrite_(fifo_t *fifo, void *c, uint8_t reversible)
{
    uint8_t *dstbuf = fifo->buf;
    uint8_t *srcbuf = c;
    int i;

    if(fifo->full)  // Check if full
    {
        return -1;
    }

    for(i=0; i!=fifo->size; i++)
    {
        dstbuf[fifo->wptr*fifo->size+i] = srcbuf[i];
    }
    fifo->wptr++;
    fifo->wptr %= fifo->len;
    if(fifo->wptr == fifo->rptr)fifo->full=1;
    if(!reversible)fifo->wptr_r = fifo->wptr;
    return 0;
}
int8_t fifoWrite(fifo_t *fifo, void *c)
{
    return fifoWrite_(fifo,c,0);
}
int8_t fifoRevWrite(fifo_t *fifo, void *c)
{
    return fifoWrite_(fifo,c,1);
}

inline uint32_t fifoFree(fifo_t *fifo)
{
    uint32_t free;
    free = (fifo->rptr - fifo->wptr_r) % fifo->len;
    if(!free && !fifo->full) free=fifo->len;
    return free;
}

void fifoReadCommit(fifo_t *fifo)
{
    fifo->rptr_r = fifo->rptr;
    fifo->full=0;
}
void fifoReadRevert(fifo_t *fifo)
{
    fifo->rptr = fifo->rptr_r;
}

void fifoWriteCommit(fifo_t *fifo)
{
    fifo->wptr_r = fifo->wptr;
}

void fifoWriteRevert(fifo_t *fifo)
{
    fifo->wptr = fifo->wptr_r;
    fifo->full = 0;
}

void sort(uint8_t *arr, int elements)
{
    uint8_t  piv, beg[128], end[128];
    int i=0, L, R;

    beg[0]=0;
    end[0]=elements;

    while (i>=0)
    {
        L=beg[i];
        R=end[i]-1;
        if (L<R)
        {
            piv=arr[L];
            while (L<R)
            {
                while (arr[R]>=piv && L<R) R--;
                if (L<R) arr[L++]=arr[R];
                while (arr[L]<=piv && L<R) L++;
                if (L<R) arr[R--]=arr[L];
            }
            arr[L]=piv;
            beg[i+1]=L+1;
            end[i+1]=end[i];
            end[i++]=L;
        }
        else
        {
            i--;
        }
    }
}

char toUpper(char c)
{
    if(c>='a' && c<='z') return c-'a'+'A';
    else return c;
}
