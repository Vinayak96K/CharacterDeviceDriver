#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<syscall.h>
#include<string.h>
#include"chardev.h"
#include<sys/ioctl.h>

int main(int argc, char const *argv[])
{
    int fd,ret=0;
    int no = 0;
    char buffer[256];
    fd=open("/dev/Marvellous Driver 1",O_RDWR);
    perror("open");
    printf("%d\n",fd);
    if(fd>0)
    {
        printf("Enter data:");
        scanf("%s",buffer);

        ret = write(fd,buffer,strlen(buffer));
        printf("%d characters send to driver!\n",ret);

        ret = read(fd,buffer,256);
        printf("Recieved data: %s %d\n",buffer,ret);

        ret = ioctl(fd,IOCTL_GET_MSG,&no);
        printf("Recieved number: %d",no);

        no=30;
        ret = ioctl(fd,IOCTL_SET_MSG,&no);
        printf("Send number: %d",no);
    }
    close(fd);
    return 0;
}