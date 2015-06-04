# Introduction #

In this article, we are going to introduce you how to setup a Micron System development environment.

**Notice that the examples given here are all based on the x86 toolchain, because current status of development is on the x86 platform, using the mature open source emulators as test platform, and the current makefiles still need modification for supporting developments on other platforms, but this issue will shortly be solved, or if you really want to develop for another processor, please contact [Martin](PsnMartin.md) first, and we will give you senior instructions on how to do so. Currently there's no official documented support on how to develop for another processor.**

# Tool Installations #

First you need basic toolchains for building from source, acqiureing source code from SVN server etc., here are the instructions:

## Unix Environment ##

### Packages ###

  1. [Minimal SYStem](http://micron-sysv3.googlecode.com/files/MSYS.rar)
  1. [Minimal SYStem Developer's ToolKit](http://micron-sysv3.googlecode.com/files/MSYS-DTK.rar)

### Instructions ###

Downlaod the packages and put them together according to their directory' hierarchy. They are actually the same package just being divided for use by different people, as we are developers, we need the DTK package.

Modify and change the content of MSYS.bat to:

```
cd bin
bash --login -i
```

Enter the shell, and enter the following:
mv /bin/old-vim.exe /bin/vim.exe

create and add the following contents to each file:

File: ~/.profile
```
source ~/.bashrc
export PATH=$PATH:/qemu
export PATH=$PATH:/bochs/bin
export PATH=$PATH:/opt/svn/bin
export PATH=$PATH:/opt/psp/bin
export PATH=$PATH:/opt/arm/bin
export PATH=$PATH:/opt/x86/bin
export PATH=$PATH:/opt/x86_64/bin
export PATH=$PATH:/opt/mips/bin
export PATH=$PATH:/opt/powerpc/bin
export PATH=$PATH:/opt/tcltk/bin
export BXSHARE=/bochs/share/bochs
```

File: ~/.bashrc
```
alias clear=clsb
alias ls='ls --color -h -F -p'
alias la='ls -a'
alias ll='ls -l'
```

The last step, set syntax color on for vim:
cp /usr/share/vim/vim58/vimrc\_example.vim ~/.vimrc

## Toolchain ##
This part of installation contains the installation of binutils, gcc, bochs and svn.
Uncompress the packages downloaded from the links, make sure the uncompressed directorys contains bin, include, and so on sub-directories.

We currently use the bochs emulator as our test platform for x86 processor, in other processor developments, we may not even use a emulator, but a real one.

### Packages ###
  1. [Host gcc compiler](http://micron-sysv3.googlecode.com/files/mingw-gcc.rar)
  1. [x86 target gcc compiler](http://micron-sysv3.googlecode.com/files/i686-elf-gcc.rar)
  1. [Bochs Emulator](http://micron-sysv3.googlecode.com/files/bochs-2.3-Micron.rar)
  1. [SVN Client](http://micron-sysv3.googlecode.com/files/svn.rar)

_Notice: later developers can download packages for other processors_

### Instructions ###
The installation directory is exactly the same as described in ~/.profile above:
```
source ~/.bashrc
export PATH=$PATH:/qemu
export PATH=$PATH:/bochs/bin
export PATH=$PATH:/opt/svn/bin
export PATH=$PATH:/opt/psp/bin
export PATH=$PATH:/opt/arm/bin
export PATH=$PATH:/opt/x86/bin
export PATH=$PATH:/opt/x86_64/bin
export PATH=$PATH:/opt/mips/bin
export PATH=$PATH:/opt/powerpc/bin
export PATH=$PATH:/opt/tcltk/bin
export BXSHARE=/bochs/share/bochs
```

Then you will get it what to do and where to put them :) You can even modify them yourself as your wish.

# Getting Source Code #

We currently use SVN for source management, thank's to Google's kindness for providing such a nice free server.

To get source code from SVN, please read [Google's SVN Instructions](http://code.google.com/p/micron-sysv3/source) first, and prefix the target address to:
```
svn checkout http://micron-sysv3.googlecode.com/svn/trunk/sysv3 micron
```

# Create rootfs #

At this stage, as you have already got the source and the tools, it's up to you to do the test and following developments.

To build from source, you need to create the non-versioned rootfs directory first:
```
cd micron
mkdir rootfs
```

To build from sourc, type:
```
make
```

To debug the bootloader, type:
```
make bdbg
```

To debug the kernel, type:
```
make kdbg
```

From here on, it all on yourself now, you can have a look at our [Kernel Module Developer's Guide](Mod.md) or [Device Driver Developer's Guide](Dev.md).

Happy coding :)