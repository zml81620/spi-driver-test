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


static unsigned short test[]={0x04AA};
static unsigned char rx[1025]={0};

const char *fun_items[] =
{\
   "0:Enable SPI_EN0",\
   "1:Enable SPI_EN1", \
   "2:EPCS Test",\
   "3:Quit"
};


static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void configspi(int fd, const enum spi_selchip chipidx,\
	   const enum spi_dataformat datformat)
{
	ioctl(fd,Cmd_reset,Cmd_reset);
	ioctl(fd,Cmd_enspi,chipidx);
	ioctl(fd,Cmd_dataformat,datformat);
    ioctl(fd,Cmd_Selchip,chipidx);
}

static void testspi(int fd,const enum spi_selchip chipidx,\
            const unsigned short *pdata,const int len)
{
   int i=0;
   int ret = -1;

   for(i=0;i<len;i++)
   {
	 write(fd,pdata,len);
	 printf("\n this is the %d write time, value=0x%x \n",i,pdata[i]);
   }
   if(Sel_chip1 == chipidx)
   {
	 ret = read(fd,rx,ARRAY_SIZE(rx));
	 printf("\n read, ret=%d \n",ret);
	 for(i=0;i<ARRAY_SIZE(rx);i++)
	 {
	   printf("\n this is the %d read time, value=0x%x \n",i,rx[i]);
	 }
   }
}

static void testepcs(int fd)
{
   unsigned char cmd = {0x06};
   write(fd,&cmd,1);  //cmd to write enable ;
   cmd = 0x05;
   //write(fd,&cmd,1);  //cmd to read status;
   //read(fd,rx,1);
   //printf("\nrx=%x\n",rx[0]);
}

int main(int argc,char *argv[])
{
   int fd = -1;
   int i = 0 ;
   int bexit=1;
   int value = -1;


   fd = open("/dev/spi",O_RDWR);
   if (fd < 0)
   {
	pabort("can't open device");
   }

   for(i=0;i<ARRAY_SIZE(fun_items);i++)
   {
	 printf("%s\n",fun_items[i]);
   }
   while(bexit)
   {
	 printf("input your choice:0-3\n");
	 scanf("%d",&value);
	 if(value<0 || value>3)
	 {
	   continue;
	 }
	 switch(value)
	 {
		case 0:
		{
		  configspi(fd,Sel_chip0,format_8bit);
		  testspi(fd,Sel_chip0,test,ARRAY_SIZE(test));
		}
		break;
		case 1:
		{
		  configspi(fd,Sel_chip1,format_16bit);
		  testspi(fd,Sel_chip1,test,ARRAY_SIZE(test));
		}
		break;
		case 2:
		{
		  configspi(fd,Sel_chip0,format_8bit);
		  testepcs(fd);
		}
		break;
		case 3:
		default:
		{
           bexit = 0;
		}
		break;
	 }//switch
   }//while

   close(fd);
   return 0 ;
}
