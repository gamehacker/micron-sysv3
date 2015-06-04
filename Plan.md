# Previous #

After one month's hard development, the basic structures such as interrupts etc. has been implemented successfully.

In the need of developing dymanic part of the kernel, there should be some dependencies be developed prior to dynamic parts:
  * Disk driver
  * File system support
  * Virtual File System support

# Current #

On the course of developing Virtual File System mechanism, a few problem emerged, as the implementation of traditional file systems will greatly reduce the effectiency of realtime requirement, we have to reconsider the designing factors of Micron System.

The development of Micron System is completely based on the idea of developing a multi-role realtime system oriented both to semi-realtime and full-realtime modes, including the application fields of mechanical motion control, sensor feedback system, radar system etc. which are both in acqiure of the following requirements:
  1. Realtime capability (both low and high)
  1. Data processing
  1. Data storage
  1. Remote control (Console)

As the current problems that we have faced in choosing the proper file system design, the following problems has been found:
  1. Traditionan file systems may greatly reduce the effectiency of realtime capability
  1. Traditional file system designs are too much complicated for a realtime system
  1. The implementation of console system interface requires a UNIX-like tree mechanism

To solve the problem, the team has made a decsion to divide the Micron System development into 2 branches, the full-realtime branch and the semi-realtime branch, the latter one is choosed to be implemented first as a research frontier and provide information to complement the development of fully realtime kernel knowledge base.

In the future, semi-realtime branch will be mainly used for terminal and host system, like the core of a system composed of serval subsystems, the part of subsystem which requires high realtime performance will use the fully realtime branch.

There won't be too much difference in interfaces between two branch, main differs will be existed in file system design and process control.

As for the semi-realtime system, techniques such as fast lur based disk sector cache, high compact policy-driver mechanism will be used to provide performance needs of realtime data processing.

# Future #



