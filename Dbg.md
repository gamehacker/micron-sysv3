# try and catch #

In C++, programmers use try, catch and throw to monitor and deal with the exceptions, many people think it is a good way to avoid the program fault or exit from an error that can be ignored.

In C language, there aren't these keywords. Some programmers use setjmp and longjmp or signal, sigsetjmp and siglongjmp to handle the exceptions, but these functions make the code unreadable, look at these examples below:

C++ Programmer:
```
int main(int argc, char *argv[])
{
   try
   {
       int ret = 10 / 0;
   }
   catch
   {
       printf("catch excetion\n");
   }
}
```

C Programmer
```
static sigjmp_buf jmpbuf;
void sig_fpe(int signo)
{
   siglongjmp(jmpbuf, 1);
}
int main(int argc, char *argv[])
{
    signal(SIGFPE, sig_fpe);           
    if (sigsetjmp(jmpbuf, 1) == 0)    
    {                                  
        int ret = 10 / 0;
    }
    else
    { 
        printf("catch exception\n");
    }
}
```

From these two problems, we know that, the C++ code is clearer and more readable than the C code.



# `_try and _catch` #

In Micron kernel, developers are allowed to use `_try` and `_catch` to handle the knowable or unknowable exceptions to avoid the system fault and print some more detail error messages. `_try` and `_catch` are not keywords, we haven't modified the compiler code, but they are defined as macros, see micron/kernel/include/arch-i386/except.h . The usage of `_try` and `_catch` is the same as that in C++, so the program above we can write as below:

C Program in Micon Kernel
```
#include <libc.h>
#include <except.h>  //for us to use _try and _catch
int ExceptionTest()
{
   _try
   {
       int ret = 10 / 0;
   }
   _catch
   {
       kprintf("catch excetion\n");
   }
}
```

Isn't it a very nice style?

There are some differences between try and `_try`:

  1. In C++, we can use more than one catch after a try, but with these emulate macros, we can only use `_catch` once after `_try`.
  1. Arguments are not allowed in `_catch`.
  1. `_throw` is not supported yet.
  1. In one function, we can only use `_try` and `_catch` once.
  1. We should include except.h when we use.

The Micron kernel mode programs( kernel-self code and other extra modules ) can use `_try` and `_catch` to handle the exceptions.

OK, try yours!







