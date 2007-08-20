/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_TIME_H__
#define __MICRON_KERNEL_TIME_H__

#include <types.h>

//! the old time struct
#ifndef _TM_DEFINED
struct tm
{
	int	tm_sec,     //
        tm_min,     //
        tm_hour,    //
        tm_mday,    //day of the month
        tm_mon,	    //month of the year
        tm_year,    //
        tm_wday,    //day of the week?  0~6
        tm_yday,    //day of the year
        tm_isdst;   //is leap year?
};
#define _TM_DEFINED
#endif  //_TM_DEFINED

//! ...
time_t time(time_t* );
//! Get a format string by a time,
//! The function returns the length of the string, 0 means failed, others > 0 means success
//! Note: the str buffer must be enough big, normally 48 bytes
int strtime( time_t*, char* str );
//! From struct tm to time_t
time_t mktime( const struct tm* );
//! From time_t to struct tm
//! If failed, it will return 0, others > 0 means success
int gettime( const time_t*, struct tm* );

#endif

