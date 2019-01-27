#ifndef CHARDEV_H
#define CHARDEV_H
#include <linux/ioctl.h>
/*
* The major device number. We can't rely on dynamic
* registration any more, because ioctls need to know
* it.
*/
#define MAJOR_NUM 300
/*
* Set the message of the device driver
*/
#define IOCTL_SET_MSG _IOR('k', 0, int *)
/*
* _IOR means that we're creating an ioctl command
* number for passing information from a user process
* to the kernel module.
*
* The first arguments, MAJOR_NUM, is the major device
* number we're using.
*
* The second argument is the number of the command
* (there could be several with different meanings).
*
* The third argument is the type we want to get from
* the process to the kernel.
*/
/*
* Get the message of the device driver
*/
#define IOCTL_GET_MSG _IOR('k', 1, int *)
/*
* This IOCTL is used for output, to get the message
* of the device driver. However, we still need the
* buffer to place the message in to be input,
* as it is allocated by the process.
*/
/*
* Get the n'th byte of the message
*/
#define IOCTL_GET_NTH_BYTE _IOWR('k', 2, int *)
/*
* The IOCTL is used for both input and output. It
* receives from the user a number, n, and returns
* Message[n].
*/
/*
* The name of the device file
*/
#define DEVICE_FILE_NAME "Marvellous Driver 1"
#endif