/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

#include <types.h>
#include <libc.h>
#include <except.h>
#include <irq.h>


//! Note, when we have muti-threads, don't do it like this!
static struct __Xcpt_reg* __cur_Xcpt_filter = NULL;

//! When try, set a filter for handling of the current thread!!
//! The filter link table is built on thread stack.
//! If the thread stack is unexpectly changed or zeroed, and the filter link table might be
//! destroyed, we can't imagined what will happen then. So we should care about this.
int __Xcpt_push_filter( struct __Xcpt_reg* p )
{
    //Make Link Table
    if( __cur_Xcpt_filter )
    {
        p->pre = __cur_Xcpt_filter;
    }else{
        p->pre = NULL;
    }
    __cur_Xcpt_filter = p;
    __cur_Xcpt_filter->fired = 0;
    return 1;
}

//! When an exception is happened, or no exception happened but the function is returning, then
//! we should pop this filter that we no more use it in other functions.
int __Xcpt_pop_filter( void )
{
    int ret_addr = 0;
    if( __cur_Xcpt_filter )
    {
        ret_addr = __cur_Xcpt_filter->ret_addr;
        __cur_Xcpt_filter = __cur_Xcpt_filter->pre;
    }
    return ret_addr;
}


//!
int filter_fill_regs( regs_t * r )
{
    if( __cur_Xcpt_filter && __cur_Xcpt_filter->fired )
        __Xcpt_pop_filter();
    if( __cur_Xcpt_filter )
    {
        r->ebp = (unsigned int)__cur_Xcpt_filter->ebp;
        r->eip = (unsigned int)__cur_Xcpt_filter->handler;
        __cur_Xcpt_filter->fired = 1;
        return 1;
    }else{
        return 0;   //no filter found
    }
}


