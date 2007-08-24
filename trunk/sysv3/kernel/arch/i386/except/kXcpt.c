/*****************************************************************************
 * Micron System V3 - System Core - Kernel Mode Exception Handler
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <module.h>
#include <libc.h>
#include <except.h>

extern int setup_isr_handler();

int kXcpt_init()
{
    setup_isr_handler();
    return 0;
}

