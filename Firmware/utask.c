#include <stdint.h>
#include "utask.h"

static uint32_t utask_free_slots = UTASKS;
static uint32_t utask_free_tid = 0;
static uint32_t utask_last_used = 0;

static utask 	tasks[UTASKS];

void utask_init()
{
    uint32_t i;
    for (i=0;i<utask_last_used;i++)
		tasks[i].state = UTS_NONE;
}

void utask_sleep(utask* t,uint32_t ticks)
{
    t->sleep = ticks;
    t->state = UTS_SLEEP;
	t->sem = 0;
}

utask*	utask_add(utask_fun fun)
{
	utask* t = &tasks[utask_free_tid];
	uint32_t i;

	if (!utask_free_slots)
		return 0;			//NULL

	if (!fun)
		return 0;			//puste? :>

   	t->state = UTS_READY;
    t->istate = 0;
    t->sleep = 0;
	t->fun = fun;
	t->data = 0;
	t->args = 0;

	if (utask_last_used < utask_free_tid)
		utask_last_used = utask_free_tid;		//nowy ostatni element

    for (i=0;i<UTASKS;i++)			//wyznacz nowy pierwszy pusty element
		if (tasks[i].state == UTS_NONE)
		{
			utask_free_tid = i;
			break;
		}
	utask_free_slots--;
	return t;
}


void utask_schedule()
{
	int i;
	register uint32_t  cond = 0;
	register utask* t = tasks;
	for (i=0;i<=utask_last_used;++i,++t)	//tutaj iteracja po taskach! (t++)
	{
		switch (t->state)
		{
			case UTS_NONE:			//nie ma go :)
				continue;

			case UTS_SUSP:			//zawieszone wykonanie:
				continue;

			case UTS_READY:			//do wykonania
				t->fun(t);
				continue;

			case UTS_WAIT_Z:
				cond = !(*t->sem);
				break;
			case UTS_WAIT_NZ:
				cond = (*t->sem);
				break;
		}

		if (cond)
		{
			t->state = UTS_READY;
			t->fun(t);
		}
	}
}

void utask_sleep_process()
{
	int i;
	for (i=0;i<UTASKS;i++)
	{
		if (tasks[i].state < UTS_SLEEP)			//nie zyje, do wykonania, zawieszony
			continue;

		if (tasks[i].sleep > 0)
			tasks[i].sleep--;
		else
		{
			tasks[i].state = UTS_READY;
		}
	}
}



inline void utask_exit(utask* t)
{
	int i;
	t->state = UTS_NONE;
	utask_free_slots++;
	utask_free_tid = UTASKS;
	utask_last_used = 0;
	for (i=0;i<UTASKS;i++)
	{
		if (tasks[i].state == UTS_NONE)
		{
			if (utask_free_tid > i)
				utask_free_tid = i;
		}
		else
			utask_last_used = i;
	}
}


inline void utask_wait_nzero(utask* t,uint32_t* sem,uint32_t timeout)
{
	t->sem = sem;				//semafor
	t->sleep = timeout;
	t->state = UTS_WAIT_NZ;
}

inline void utask_wait_zero(utask* t,uint32_t* sem, uint32_t timeout)
{
	t->sem = sem;				//semafor
	t->sleep = timeout;
	t->state = UTS_WAIT_Z;
}

void	utask_suspend(utask* t)
{
	t->state = UTS_SUSP;
}

inline uint32_t	utask_get_free_slots(void)
{
		return utask_free_slots;
}

uint32_t    utask_get_task_cnt(utask_fun fun)
{
    uint32_t i;
    uint32_t cnt = 0;
	for (i=0;i<utask_last_used;i++)
		if (tasks[i].fun == fun)
			cnt++;
	return cnt;
}

