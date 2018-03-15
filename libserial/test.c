/*
** test de libserial
** philippe Guillaumaud <pguillaumaud@april.org>
**
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>

#include "libserial.h"

SLPortPtr pPort;

int main(int argc, char **argv)
{
  unsigned char buff[128];
  unsigned char car;
  int n,lus,ret;

  if(argc<=1)
  {
    printf("usage: TEST SERIAL device\n");
    return -1;
  }

  if(slOpenPort(&pPort,argv[1],O_RDWR|O_NDELAY) <0)
  {
    perror("Impossible d'ouvrir le device");
    return -1;
  }

  printf("isatty=%d\n",isatty(pPort->fd));
  if(slSetPortAttr(&pPort,4800,'E',7,1) <0)
  {
    perror("Initialisation du device impossible");
    slClosePort(&pPort,0);
    return -1;
  }

  printf("taille de la structure=%d octets\n",sizeof(struct _SLPorts));

  printf("device =%s\n",slGetDeviceName(&pPort));
  printf("fd     =%d\n",slGetFileDesc(&pPort));
  printf("vitesse  =%d\n",slGetSpeed(&pPort));
  printf("parite   =%c\n",slGetParity(&pPort));
  printf("len      =%d\n",slGetLenData(&pPort));
  printf("stop     =%d\n",slGetBitStop(&pPort));
  printf("timeout  =%d\n",slGetTimeout(&pPort));

  sprintf(buff,"\x1B\x39\x7F\x0CTEST PORT SERIE\x1B\x39\x7B");
  printf("TransmitBuffer, retour=%d\n",slTransmitBuffer(&pPort,buff,strlen(buff),1));

  printf("lecture buffer (return pour finir...\n");
  while(1)
  {
    n = 0;
    memset(buff,0,128);
    ret = slReceiveBuffer(&pPort,buff,40,'\x0d',&n);
    printf("retour=%d n=%d buff=",ret,n);
    if((ret == SL_SUCCESSFUL) || (n>0))
    {
      printf("[");
      lus = 0;
      while(lus<n)
      {
	if((buff[lus]<32) || (buff[lus]>127))
	  printf("%x",buff[lus]);
	else
	  printf("%c",buff[lus]);
	lus++;
      }
      printf("]\n");
      fflush(stdout);
      if(buff[lus-1] == '\x0d')
	break;
    }
    else
      break;
  }

  printf("lecture caractere (return pour finir...\n");
  while(1)
  {
    ret = slReceiveChar(&pPort);
    printf("retour=%d ",ret);
    if(ret > 0)
    {
      if((ret<32) || (ret>127))
	printf("%x",ret);
      else
	printf("%c",ret);
      fflush(stdout);
      if(ret == '\x0d')
	break;
    }
    else
      break;
  }

  slClosePort(&pPort,1);
  printf("port ferme\n");
  return 0;
}
/*
** fin
*/
