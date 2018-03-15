/*
Watch serial device (arg1) for input matching some regular expression (arg2)

When found, exit and print what has been read so far to stdout

Diagnostic messages go to stderr

23/09/2001 initial release
*/

#define  S_SPEED   9600			/* port speed */
#define  S_TIMEOUT  30*60		/* seconds */
#define  S_BUFFLEN  512


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <termios.h>
#include <regex.h>


#include "libserial.h"

SLPortPtr pPort;

int
main(int argc,
	 char **argv)
{
  unsigned char buff[S_BUFFLEN];
  unsigned char *cp;
  int n, readc;

  if (argc <= 2) {
	fprintf(stderr,"usage: %s device\n", argv[0]);
	return -1;
  }

  if (slOpenPort(&pPort, argv[1], O_RDWR | O_NDELAY) < 0) {
	perror("cannot open device");
	return -1;
  }

  fprintf(stderr,"isatty=%d\n", isatty(pPort->fd));
  if (slSetPortAttr(&pPort, S_SPEED, 'E', 7, 1) < 0) {
	perror("cannot initialize device");
	slClosePort(&pPort, 0);
	return -1;
  }
  if (slSetTimeout(&pPort, S_TIMEOUT) < 0) {
	perror("cannot set timeout");
	slClosePort(&pPort, 0);
	return -1;
  }

  fprintf(stderr,"size of structure=%d octets\n", sizeof(struct _SLPorts));

  fprintf(stderr,"device  = %s\n", slGetDeviceName(&pPort));
  fprintf(stderr,"fd      = %d\n", slGetFileDesc(&pPort));
  fprintf(stderr,"speed   = %d\n", slGetSpeed(&pPort));
  fprintf(stderr,"parity  = %c\n", slGetParity(&pPort));
  fprintf(stderr,"len     = %d\n", slGetLenData(&pPort));
  fprintf(stderr,"stop    = %d\n", slGetBitStop(&pPort));
  fprintf(stderr,"timeout = %d\n", slGetTimeout(&pPort));


/*
# our caller id enabled modem will show something like
# RING
#
# DATE = 0719
# TIME = 2103
# NMBR = 5551212        or      NMBR = P
#
# RING
*/
  fprintf(stderr,"matching pattern <%s>", argv[2]);
  cp =  (char *)re_comp(argv[2]);
  if (cp != NULL) {
	perror(cp);
	slClosePort(&pPort, 0);
	return (-1);
  }




	/* enable caller id */
	sprintf(buff, "AT#CID=1\r\n");
	fprintf(stderr,"TransmitBuffer, result=%d\n",
		   slTransmitBuffer(&pPort, buff, strlen(buff), 1));

	/* read from port until we get what we are looking for */
	memset(buff, 0, S_BUFFLEN);
	n = 0;
	do {
	  readc = slReceiveChar(&pPort);
	  if (readc >= 0) {
		buff[n] = (char) readc;
		fprintf(stderr,"%c", (char) readc);
		if (re_exec((char *)buff) == 1) {
		  printf("%s", buff); fflush(stdout);
		  break;
		}
		n++;
	  }
	} while ((readc >= 0) && (n < S_BUFFLEN));


  slClosePort(&pPort, 1);
  return 0;
}

/*
** --
*/
