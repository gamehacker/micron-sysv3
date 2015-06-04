![http://groups.google.com/group/micronsystem/web/LOGO.png](http://groups.google.com/group/micronsystem/web/LOGO.png)

# Micron System Version 3 Introduction #

Micron System was originally developed by [Martin Tang](PsnMartin.md)(the Version 1 and Version 2 which were based on a research project) during the last 2 years.

By Version 3, Micron System Team gathered a small group of motivated people who are willing to do the challenge to bring the classical UNIX operating system back to the modern history by replacing its old shell environment with a new _Microsoft Windows GDI+_ like GUI front end.

Declearing that our source codes **ARE NOT BASED ON ANY** third party's(including BSD and Linux) source code, the Micron System(including its source code) will be released under our [Micron System Public License Agreement(BSD)](MicronLicense.md).

The Team have chosen Qemu branching from 0.9.0 as our official system emulator(board based) and will add our new architecture supports(which will be supported by Micron System). The code will be maintain in our SVN repository, and be published under its original license respecting its original authors who did the great work.

Micron System has no intention to be a Windows or Linux replacement(which are both PC based), but a new POSIX system for the whole computer system(supporting many processors with exactly the same HAL), future applications will include game consoles, handhelds, industrial control systems, communication systems, etc. When you learned one architecture's development method from Micron System, there will be no more pain to pay for other processors supported, at the same time, the hardware designers will have more freedom to switch to their project's best choised processors.

There may have commercial activity involved as we don't refuse to commercial development, but in the future if you want to public Micron System within a commercial product, you first have to gain a copy of our signed permition prior as described in our [License Agreement](MicronLicense.md).

English is used as our natural language, and we welcome any people from any country to participate in our project, who can provide us help (both technical and documentary) by providing patches, technical hardware specs, even suggestions, at the same time, we are just cool guys willing to do something like you are =P

# Project News #

  * 2007.07.30 Wiki Created.
  * 2007.08.21 Project is now under heavy development.
  * 2007.08.24 Qemu patched source(mainly speedups in gdb debugging) added to SVN.
  * 2007.08.26 Arranging code, prepare for low development period.

# Micron Developer's Reference #

NOTICE: The best way to investigate Micron System is by reading [source code](http://micron-sysv3.googlecode.com/svn/trunk/sysv3), please use svn to check out your own copy then have a build yourself :)

  * [Project Plan & TODOs](Plan.md)
  * [Frequently Asked Questions](Faq.md)
  * [Debugging Kernel & Modules](Dbg.md)
  * [Developemnt Environment Setup](Env.md)
  * [Kernel Module Developer's Guide](Mod.md)
  * [Device Driver Developer's Guide](Dev.md)
  * [File System Developer's Guide](Fs.md)
  * [Network Protocol Developer's Guide](Net.md)
  * [System Service Developer's Guide](Srv.md)
  * [Micro Kernel Extension Framework Guide](Mic.md)
  * [User Mode Driver Framework Guide](Umd.md)

# Development Support #

[Development Support Forum](http://groups.google.com/group/micronsystem).

Project mail:

  * martintang25 (AT) gmail (DOT) com

Chinese developers can contact the development team and other developers through:

  * Tencent QQ Group: 18266455

# Micron Developers #

**Project Leader:**
  * [Martin Tang](PsnMartin.md)

**Assistant Project Leader:**
  * [Guan Huang](Psngdxxhg.md)

**Developers:**
  * [flyinsky88](Psnflyinsky88.md)

_Latest updated: 2007.08.27_