/*****************************************************************************
 * Micron System V3 - System Core - Kernel Mode Exception Handler
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Huang Guan
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <module.h>
#include <libc.h>
#include <except.h>

//! If _try _catch won't work, do this test.
static int kXcpt_test()
{
    _try    //my own try
    {
        //try context
        printf("\ntry to make an exception.\n");
        int c=0;
        c = 5/c;
    }
    _catch  //my own catch
    {
        printf("Exception happened.\n");
        return 0;
    }
    printf("Reach the end!!\n");
    return 1;
}

extern int setup_isr_handler();
int kXcpt_init()
{
    setup_isr_handler();
    if(0)
        kXcpt_test();
    return 0;
}


int kXcpt_exit()
{
    return 0;
}


REGISTER_MODULE(kXcpt, "Kernel Exception", kXcpt_init, kXcpt_exit);

