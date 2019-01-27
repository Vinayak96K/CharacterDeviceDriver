#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/device.h>
#include<linux/fs.h>
#include<asm/uaccess.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include"chardev.h"
#include <linux/ioctl.h>

// The device will appear in /dev/char/ using the value
#define DEVICE_NAME "Marvellous Driver 1"

// The device class-- this is a character device driver
#define CLASS_NAME "Marvellous Driver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VINAYAK PATIL");
MODULE_DESCRIPTION("Marvellous: Use of IOCTL in Device Driver");
MODULE_VERSION("0.1");

static int majorNumber;			//stores the device number:determined automatically
static char message[256] = {0};		//memory for the string that is passed from the user space
static short size_of_message;		//used to remember the size of string stored
static int numberOpens = 0;		//Counts number of times the device is opened
static struct class* charClass = NULL;	//the device driver class struct pointer
static struct device* charDevice = NULL;	//

//the prototype functions for characetr device driver-- must come before function defination
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *,char *,size_t, loff_t *);
static ssize_t dev_write(struct file *,const char *,size_t, loff_t *);
//static int dev_ioctl(struct inode *,struct file *, unsigned int, unsigned long)

//Function is called when the ioctl system call 
static long dev_ioctl(struct file *f, unsigned int ioctl_num, unsigned long data)
{
	int no=20;
	
	switch(ioctl_num)
	{
		case IOCTL_SET_MSG:
			raw_copy_from_user(&no,(int *)data,sizeof(int));
			printk(KERN_INFO "Marvellous: Recieved data %d\n",no);
			break;
		case IOCTL_GET_MSG:
			raw_copy_to_user((int *)data,&no,sizeof(int));
			printk(KERN_INFO "Marvellous: Sending data...\n");
			break;
	}
	return 0;
}

//Initialize the file_operations structure
static struct file_operations fops={
	.open = dev_open,
	.read = dev_read,
	.unlocked_ioctl = dev_ioctl,
	.write = dev_write,
	.release = dev_release,
};

//Driver initialization function
static int __init char_init(void)
{
	printk(KERN_INFO "Marvellous: Driver loaded succcessfully\n");
	majorNumber = register_chrdev(0,DEVICE_NAME,&fops);	//Allocate a major number for the device
	if(majorNumber<0)
	{
		printk(KERN_ALERT "Marvellous: Failed to allocate major number\n");
		return majorNumber;	//return if problem occured during the major number
	}
	printk(KERN_INFO "Marvellous: Registered correctly with major number %d\n",majorNumber);
		
	charClass = class_create(THIS_MODULE,CLASS_NAME);	//Register the device class
	if(IS_ERR(charClass))
	{
		unregister_chrdev(majorNumber,DEVICE_NAME);
		printk(KERN_ALERT "Marvellous: Failed to register device class\n");
		return PTR_ERR(charClass);
	}
	printk(KERN_INFO "Marvellous: Successfully registered device class\n");

	charDevice = device_create(charClass,NULL,MKDEV(majorNumber,0),NULL,DEVICE_NAME);	//Register the device driver
	if(IS_ERR(charDevice))
	{
		class_destroy(charClass);
		unregister_chrdev(majorNumber,DEVICE_NAME);
		printk(KERN_ALERT "Marvellous: Failed to create device\n");
		return PTR_ERR(charDevice);
	}
	printk(KERN_INFO "Marvellous: Successfully created device driver\n");

	return 0;
}

//Driver cleanup function
static void __exit char_exit(void)
{
	device_destroy(charClass,MKDEV(majorNumber,0));		//removes the device
	class_unregister(charClass);				//unregister the device class
	class_destroy(charClass);				//remove the device class
	unregister_chrdev(majorNumber,DEVICE_NAME);		//unregister the major number
	
	printk(KERN_INFO "Marvellous: Driver unloaded");
}

//Function which gets called when we open the device
static int dev_open(struct inode *inodep,struct file *filep)
{
	numberOpens++;
	printk(KERN_INFO "Marvellous: Device has been opened %d times\n",numberOpens);
	return 0;
}

//Function is called whenever device is being read from user space
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int err_cnt;
	printk(KERN_INFO "Marvellous: Reading from device\n");
	err_cnt = raw_copy_to_user(buffer,message,size_of_message);		//copies data to user space
	if(err_cnt==0)
	{
		printk(KERN_INFO "Marvellous: Sent %d characters to the user\n",size_of_message);
		return (size_of_message=0);
	}
	else
	{
		printk(KERN_ALERT "Marvellous: Failed to send data\n");
		return -EFAULT;
	}	
}

//Function is called whenever data is being written from the user space
static ssize_t dev_write(struct file *filep, const char *buffer,size_t len, loff_t *offset)
{
	printk(KERN_INFO "Marvellous: Writting into the device\n");
	sprintf(message,"%s(%d letters)",buffer,len);
	size_of_message = strlen(message);
	printk(KERN_INFO "Marvellous: Recieved %d characters from the user\n",len);
	return len;
}

//Function is called when the device is closed from the user space
static int dev_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "Marvellous: Device successfully released\n");
	return 0;
}

module_init(char_init);
module_exit(char_exit);