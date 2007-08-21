/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_EXCEPT_H__
#define __MICRON_KERNEL_EXCEPT_H__


//! Structure of an Exception Registration
struct __Xcpt_reg{
    struct __Xcpt_reg* pre; //the previous exception registration
    int handler;            //the address of the exception handler
    int ebp,
        esp;                //we save these two, for running in the handler function
    int ret_addr;           //real_ret_addr
    int fired;              //indicates the filter is already used.
};

int __Xcpt_push_filter( struct __Xcpt_reg* p );
int __Xcpt_pop_filter( void );


//! Note: This will only work on I386(above) Processor
//! Note: This is invented by Huang Guan, if you interest in these things, contact him.

//! _try
//! 1.save handler and ebp in __d_Xcpt_reg
//! 2.modify ret_addr
#define _try \
    struct __Xcpt_reg __d_Xcpt_reg; \
    int __Xcpt_happened; \
    __asm__("movl $__Xcpt_catch_do, %%eax\n" \
            "movl %%eax, 0x4(%%ebx)\n" \
            "movl %%ebp, 0x8(%%ebx)\n" \
            "movl %%esp, 0xC(%%ebx)\n" \
            "movl 0x4(%%ebp), %%eax\n" \
            "movl %%eax, 0x10(%%ebx)\n" \
            "pushl %%ebx\n" \
            "call __Xcpt_push_filter\n" \
            "addl $0x4, %%esp" \
            ::"b"( &__d_Xcpt_reg ) ); \
    __asm__("movl $__Xcpt_real_ret, %eax\n" \
            "movl %eax, 0x4(%ebp)" );

//! _catch
#define _catch \
    __Xcpt_happened = 0; \
    __asm__("jmp __Xcpt_catch_if"); \
    __asm__("__Xcpt_real_ret:\n" \
            "call __Xcpt_pop_filter\n" \
            "jmp *%eax"); \
    __asm__("__Xcpt_catch_do:\n"); \
    __Xcpt_happened = 1; \
    __asm__("__Xcpt_catch_if:\n"); \
    if( __Xcpt_happened )


#endif

