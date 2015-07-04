#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/types.h>
#include <string.h>


#include "../driver/myspi.h"
#include "gpio.h"


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))


static unsigned short test[]={0x04A5};
static unsigned char rx[1025]={0};

const char *fun_items[] =
{\
   "0:Enable SPI_EN0",\
   "1:Enable SPI_EN1", \
   "2:EPCS Test",\
   "3:GPIO Test",\
   "4:GPIO Test in driver",\
   "5:Quit"\
};


static void pabort(const char *s)
{
	perror(s);
	abort();
}

static void configspi(int fd, const enum spi_selchip chipidx,\
	   const enum spi_dataformat datformat,const enum spi_cshold spi_cshold)
{
	ioctl(fd,Cmd_reset,Cmd_reset);
	ioctl(fd,Cmd_enspi,chipidx);
	ioctl(fd,Cmd_dataformat,datformat);
    ioctl(fd,Cmd_Cshold,spi_cshold);
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
	 memset(rx,0,ARRAY_SIZE(rx));
	 ret = read(fd,rx,ARRAY_SIZE(rx));
	 #if 1
	 printf("\n read, ret=%d \n",ret);
	 for(i=0;i<ret;i++)
	 {
	   printf("\n this is the %d read time, value=0x%x \n",i,rx[i]);
	 }
	 #endif
   }
}

static void testepcs(int fd)
{
   unsigned char cmd[10] = {0x006,0x00,0x00,0x00};
   int ret = 0 ;
   int i = 0;
   //write(fd,cmd,1);  //cmd to write enable ;

#if 1
   cmd[0] = 0xAB;
   cmd[1] = 0x00;
   cmd[2] = 0x00;
   cmd[3] = 0x00;
   cmd[4] = 0x00;
   write(fd,cmd,5);  //cmd to read status;
   ret = read(fd,rx,5);
   for(i=0;i<ret;i++)
   {
	 printf("\nrx=%x\n",rx[i]);
   }
#endif
}

static void testgpio(void)
{
#if 1
   gpio_export(PIN_AS_MODE);
   gpio_direction(PIN_AS_MODE,OUT);    // GPIO为输出状态
   gpio_write(PIN_AS_MODE,HIGH_LEVEL); //输出高电平

   gpio_export(PIN_EPCS_CONFIG);
   gpio_direction(PIN_EPCS_CONFIG,OUT);   // GPIO为输出状态
   gpio_write(PIN_EPCS_CONFIG,LOW_LEVEL); //输出低电平
#endif
   gpio_export(PIN_SPI_EN0);
   gpio_direction(PIN_SPI_EN0,OUT);   // GPIO为输出状态
   gpio_write(PIN_SPI_EN0,LOW_LEVEL); //输出低电平

}

static void  testgpio2(int fd)
{
 #if 0
   ioctl(fd,Cmd_Selpin,1);
   sleep(1);
   ioctl(fd,Cmd_Selpin,0);
	sleep(1);
   ioctl(fd,Cmd_Selpin,1);
	sleep(1);
	ioctl(fd,Cmd_Selpin,0);
 #endif
}

int main(int argc,char *argv[])
{
   int fd = -1;
   int i = 0 ;
   int bexit=1;
   int value = -1;
   unsigned short tt = 0x0400;

   #if 1
   fd = open("/dev/spi",O_RDWR);
   if (fd < 0)
   {
	pabort("can't open device");
   }
   #endif
   for(i=0;i<ARRAY_SIZE(fun_items);i++)
   {
	 printf("%s\n",fun_items[i]);
   }
   while(bexit)
   {
	 printf("input your choice:0-%d\n",ARRAY_SIZE(fun_items));
	 scanf("%d",&value);
	 if(value < 0 || value > ARRAY_SIZE(fun_items))
	 {
       value = -1;
	   continue;
	 }
	 switch(value)
	 {
		case 0:
		{
		  configspi(fd,Sel_chip0,format_8bit,Cs_active_hold);
		  testspi(fd,Sel_chip0,test,ARRAY_SIZE(test));
		}
		break;
		case 1:
		{
		  configspi(fd,Sel_chip1,format_16bit,Cs_active_unhold);
		  for(i=0,tt=0x0400;i<=3;i++)
		  {
			testspi(fd,Sel_chip1,&tt,1);
			tt++;
		  }
		}
		break;
		case 2:
		{
		  configspi(fd,Sel_chip0,format_8bit,Cs_active_hold);
		  testepcs(fd);
		}
		break;
		case 3:
		{
           testgpio();
		}
		break;
		case 4:
		{
           testgpio2(fd);
		}
		break;
		case 5:
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
