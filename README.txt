This project registers a device file of class character which can be used by userspace programmer to perform file operation.
It also include implementation of IOCTL to allow user to send and recieve different type of data instead of character.

Following is list of information of important files(source file)
chardev.c - it contains the kernel space programs.
chardev.h - it contains declartion of IOCTL macros.
UserFile - it contains userspace programs.
chardev.ko - it is the kernel module wich should be inserted to kernel space.