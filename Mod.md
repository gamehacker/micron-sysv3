# Introduction #

In Micron System, the word 'Module' means kernel extensions that linked to the running kernel in both the statically linked (compiled together with kernel) and dynamically (loaded as a module file).

There are generally 2 types of modules:
  * Static module
  * Thread module

Kernel module includes a variety of kernel extensions, including but not limited to:
  * Device driver supports
  * File system supports
  * Network protocol supports
  * Kernel functionality expansion (system call)

# Kernel Module Framework #

Any kernel module should have AT LEAST the following basic structure:

```
#include <module.h>
int module_init()
{
        return 0;    /* 0 upon successful operation, -1 if failure */
}
int module_exit()
{
        return 0;    /* 0 upon successful operation, -1 if failure */
}
REGISTER_MODULE(module, "Description of Module", module_init, module_exit);
```

Notice the init method is being called upon every load of module(dynamic) or kernel initialization(statical), and the exit method is being called upon every unload of module(dynamic) or kernel uninitalization(staticall).

You can also manually initializa or uninitalize a module through functions:
  * modstart() - not implemented yet
  * modkill()  - not implemented yet

# Loading A Module #

There are generally 2 way of doing this:
  * Compile module together with kernel without any modification of kernel source.
  * Dynamically load it from a file system (not implemented yet)

To compile the module with a kernel, you need to modify 2 files depending on what type of module you are developing:
  * Single architecture specific
  * All platform supported

On the Single architecture specific condition, you should add your file to be compiled together to the file:

```
// In arch/$Architecture/Makefile :
// Search and find the lines:
OBJECTS+=arch/i386/init/init.o \
         arch/i386/init/main.o \
         arch/i386/init/memsize.o \
// Append your module to the place and order it should be compiled, name .c to .o
```

On the All platform supported condition, you should add your file to be compiled together to the file:

```
// In Makefile :
// Search and find the lines:
OBJECTS+=driver/device.o \
         filesys/icache.o \
         filesys/iget.o \
// Append your module to the place and order it should be compiled, name .c to .o
```