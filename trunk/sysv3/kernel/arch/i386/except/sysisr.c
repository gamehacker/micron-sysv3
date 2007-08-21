/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

#include <libc.h>
#include <irq.h>

static void isr_handler(regs_t* r);

//! We control all the isr.
//! But we should take special care about the page fault isr.
int setup_isr_handler()
{
    int i;
    for( i=0; i<32; i++ )
    {
        isr_install( i, isr_handler );
    }
    return 1;
}


void dump_cpu( regs_t *r )
{
    printf("Dumping CPU:\ncs: 0x%X\t\teip: 0x%X\nss: 0x%X\tesp: 0x%X\n\
ds: 0x%X\tesi: 0x%X\nes: 0x%X\tedi: 0x%X\nfs: 0x%X\ngs: 0x%X\neax: 0x%X\tecx: 0x%X\tebx: 0x%X\t\
edx: 0x%X\neflags: 0x%X\n", r->cs, r->eip, r->ss, r->esp,
    r->ds, r->esi, r->es, r->edi, r->fs, r->gs, r->eax, r->ecx,
    r->ebx, r->edx, r->eflags);
}


//! When an isr fires, this finds the proper handler to deal with.
#include "except_msg.h"
extern int filter_fill_regs( regs_t* r);
static void isr_handler(regs_t* r)
{
    if (r->intn < 32)   //int_no>=32 is impossible.
    {
        // Try to find a handler
        if( filter_fill_regs( r ) )
        {
            // handled
            return;
        }else{
            // not handled
            kprintf("%C[EXCEPTION]%C ", 0x0E, 0x0F);
            printf("Not handled: %s (%d)\n", except_msg[ r->intn ], r->err );
            dump_cpu( r );
            PANIC(1, "System halted.");
        }
    }
}

