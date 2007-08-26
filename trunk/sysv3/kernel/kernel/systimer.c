/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/

/** A System Timer Scheduler, Only for Kernel Mode **/

#include <libc.h>
#include <time.h>
#include <systimer.h>
#include <irq.h>
#include <config.h>

struct timer{
	int ms;
	int tid;	//thread id
	void (*handler)(void);
	//internal
	int counter; //if zero, call handler.
};

//increase this if we have more memory
#define MAX_TIMER 64
static struct timer timer_list[MAX_TIMER];
static int timer_num = 0;	//number of timers we used.

int register_timer( int ms, void (*handler)(void) )
{
	long flags;
	//disable interrupt
	local_irq_save( flags );
	if( timer_num>=MAX_TIMER || ms<=0 )
	{
		local_irq_restore( flags );
		return 0;
	}
	//add a timer to list
	struct timer* tmr = &timer_list[ timer_num++ ];
	tmr->ms = ms;
	tmr->handler = handler;
	//tmr->tid = get_current_thread()->tid;
	//initialize the timer
	tmr->counter = tmr->ms;
	//enable interrupt
	local_irq_restore( flags );
	return 1;
}


int unregister_timer( void (*handler)(void ) )
{
	int i, flags;
	for( i = 0; i < timer_num; i++ )
	{
		if( handler == timer_list[ i ].handler )
		{
			local_irq_save( flags );
			//for stablity, we check again
			if( handler == timer_list[ i ].handler )
			{
				timer_list[ i ] = timer_list[ timer_num ];
				timer_num --;
			}
			local_irq_restore( flags );
			return 1;
		}
	}
	return 0;
}


//This should be called by clock driver
//we should have a high irq level, and avoid reentering this function.
void timer_rolling()
{
	register int i;
	static int dec = 0;
	if( !dec )
		dec = 1000/RTC_FREQUENCY;
	for( i=0 ; i< timer_num; i++ )
	{
		if( (timer_list[i].counter-=dec) <= 0 )
		{
			struct timer * tmr = &timer_list[i];
			tmr->counter = tmr->ms;
			tmr->handler();
		}
	}
}


