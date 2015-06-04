# Questions & Answers #
## Q1: Why doesn't Micron kernel include a byte based allocator? ##

For stablity issues, Micron System strongly recommended NOT to use dynamical byte based memory allocator in kernel, as it MUST lead to memory fragmentation and complexity of kernel design to deal with it, we can't deal with complex situations, but we have ways to avoid that :)

**Solution 1**:  Request your needed memory space from bss section statically:

Setup any global variable like this:
```
char mod_buf[4096];
```

**Solution 2**:  Request your needed memory space from stack:

In any function, request it by local variable:
```
int mod_func()
{
    char mod_buf[4096];
    ...
}
```