#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/types.h>
//#include <system.h>

#include "../driver/myspi.h"


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static unsigned short test[]={0x04AA,0};
static unsigned char rx[1025]={0};

int main(int argc,char *argv[])
{   
   int fd = -1;
   int bexit = -1 ;
   int i=0;
   int ret = -1;

   fd = open("/dev/spi",O_RDWR);
   //printf("\nfd=%d\n",fd);
   if (fd < 0)
   {
	pabort("can't open device");
   }

   bexit = ioctl(fd,Cmd_reset,Cmd_reset);
   bexit = ioctl(fd,Cmd_enspi,Sel_chip1);
   bexit = ioctl(fd,Cmd_dataformat,format_16bit);
   bexit = ioctl(fd,Cmd_Selchip,Sel_chip1);
   for(i=0;i<1025;i++)
   {
	 write(fd,test,ARRAY_SIZE(test));
	 printf("\n this is the %d write time, value=0x%x \n",i,rx[0]);
   }
   ret = read(fd,rx,1025);
   printf("\n read, ret=%d \n",ret);
   for(i=0;i<1025;i++)
   {
	 printf("\n this is the %d read time, value=0x%x \n",i,rx[i]);
   }

   bexit = ioctl(fd,Cmd_reset,Cmd_reset);
   bexit = ioctl(fd,Cmd_enspi,Sel_chip0);
   bexit = ioctl(fd,Cmd_dataformat,format_8bit);
   bexit = ioctl(fd,Cmd_Selchip,Sel_chip0);
   for(i=0;i<1025;i++)
   {
	 write(fd,test,ARRAY_SIZE(test));
	 printf("\n this is the %d time to write data to SPEN0\n",i);
   }


   close(fd);
   return 0 ;
}
