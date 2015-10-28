#ifndef UTASK_H
#define UTASK_H

#include <stdint.h>


#define	UTS_NONE	0
#define	UTS_SUSP	1
#define UTS_READY	2
#define UTS_SLEEP	3

#define UTS_WAIT_Z	4
#define UTS_WAIT_NZ	5

#define UTASKS		40

typedef struct utask
{
    volatile	uint32_t	state;
    volatile	uint32_t	istate;
	volatile 	uint32_t*	sem;
    volatile	uint32_t	sleep;
    void					(*fun)(struct utask* t);
	void*					data;
	uint32_t				args;

} utask;

typedef void (*utask_fun)(struct utask* t);

void	utask_init(void);
void 	utask_sleep	(utask* t,uint32_t ticks);
utask*	utask_add(utask_fun fun);

#define utask_wait utask_wait_zero

void	utask_wait_zero(utask* t,uint32_t* sem,uint32_t timeout);
void	utask_wait_nzero(utask* t,uint32_t* sem, uint32_t timeout);


void	utask_suspend(utask* t);
void	utask_exit(utask* t);
void 	utask_schedule(void);
void	utask_sleep_process(void);
uint32_t	utask_get_free_slots(void);
uint32_t    utask_get_task_cnt(utask_fun fun);

#endif					//UTASK_H
