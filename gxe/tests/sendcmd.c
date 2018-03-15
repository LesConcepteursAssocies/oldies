/*
** envoi d'une commande xml, et réception de la réponse
** utilisé pour tester GXE
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/param.h>
#include <arpa/inet.h>

#define SERV_TCP_PORT   40000
/* host addr for server */

#define SERV_HOST_ADDR	"127.0.0.1"

/*
#define SERV_HOST_ADDR	"machine.domaine"
*/

#define	MAXLINE 1024

int Leport=0;
char *ficmd;

void err_dump(char *p)
{
  fprintf(stderr,"%s\n",p);
  exit(1);
}

int main(int argc,char **argv)
{
  int sockfd,ret;
  struct sockaddr_in serv_addr;
  struct hostent *hote;
  register FILE	*fp;
  fd_set rfds,wfds;
  struct timeval tv;
  int	n;
  char	sendline[MAXLINE+1], recvline[MAXLINE+1];

  Leport = SERV_TCP_PORT;
  ficmd  = argv[1];

  if(argc>2)
  {
    Leport = atoi(argv[1]);
	ficmd  = argv[2];
  }
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  if((hote=gethostbyname(SERV_HOST_ADDR)) == NULL)
  {
    perror("gethostbyname");
	exit(1);
  }
  serv_addr.sin_family	    = AF_INET;
  serv_addr.sin_addr.s_addr = ((struct in_addr *) (hote -> h_addr)) -> s_addr;
  serv_addr.sin_port	    = htons(Leport);

  /* 
   * Open a TCP socket (an Internet stream socket).
   */
	
  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_dump("client: can't open stream socket");

  /*
   * Connect to the server
   */
  if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    err_dump("client: can't connect to server");

  if((fp=fopen(ficmd,"r")) == NULL)
  {
    close(sockfd);
    err_dump("client: impossible d'ouvrir le fichier");
  }

  /*
  ** boucle de lecture/écriture
  */
  while(1)
  {
    memset(sendline,0,MAXLINE+1);
    memset(recvline,0,MAXLINE+1)
;
    /* Timeout de 10 secondes */
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    FD_SET(sockfd,&rfds);
    FD_SET(sockfd,&wfds);
    tv.tv_sec  = 10;
    tv.tv_usec = 0;
    ret = select(sockfd+1,&rfds,&wfds,NULL,&tv);

    if(ret<0)
      break;

    if(ret)
    {
      if(FD_ISSET(sockfd,&wfds))
      {
	/* on peut écrire */
	if(fp)
	{
	  if(!feof(fp))
	  {
	    if(fgets(sendline, MAXLINE, fp) != NULL)
	    {
	      n = write(sockfd,sendline,strlen(sendline));
	      if(n<0)
		break;
	      fprintf(stdout,"EMIS:%s",sendline);
	      if(strchr(sendline,'\n') == NULL)
		fprintf(stdout,"\n");
	    }
	  }
	  else
	  {
	    fclose(fp);
	    fp = NULL;
	  }
	}
      }

      if(FD_ISSET(sockfd,&rfds))
      {
	/* données à lire */
	n = read(sockfd,recvline,MAXLINE);
	if(n<=0)
	  break;
	if(n>0)
	{
	  fprintf(stdout,"RECU:%s",recvline);
	  if(strchr(recvline,'\n') == NULL)
	    fprintf(stdout,"\n");
	}
      }
    }
    else
      fprintf(stdout,"timeout\n");
  }

  fprintf(stdout,"\nFIN\n");
  if(fp)
    fclose(fp);
  close(sockfd);
  exit(0);
}
