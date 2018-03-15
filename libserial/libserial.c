/*
** ----------------------------------------------------------------------------
** libserial
** philippe Guillaumaud <pguillaumaud@april.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
**
** Outils de manipulation des ports séries sous Linux
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

/*
** ----------------------------------------------------------------------------
** Ouverture d'un port de communication de nom "devname".
** Renvoie 0 si ok.
** pptr: adresse du pointeur sur la structure SLPorts.
** flags est passé directement à open.
*/
int slOpenPort(SLPortPtr *pptr,char *devname,int flags)
{
  SLPortPtr ptr;

  ptr = *pptr;
  /* On alloue si nécessaire */
  if(ptr == NULL)
  {
    ptr = (SLPortPtr)malloc(sizeof(struct _SLPorts));
    if(ptr == NULL)
      return SL_ERROR;
    
  }
  memset(ptr,0,sizeof(struct _SLPorts));
  *pptr = ptr;
  strcpy(ptr->dev,devname);
  ptr->fd = open(ptr->dev,flags);
  if(ptr->fd <0)
  {
    free(ptr);
    *pptr = NULL;
    return SL_ERROR;
  }
  if(tcgetattr(ptr->fd,&ptr->old_io)<0)
  {
    close(ptr->fd);
    free(ptr);
    *pptr = NULL;
    return SL_ERROR;
  }
  memcpy(&ptr->cfg_io,&ptr->old_io,sizeof(struct termios));
  ptr->timeout = SL_DFLTTIMEOUT;
  return SL_SUCCESSFUL;
}
/*
** ----------------------------------------------------------------------------
** Ferme le port de communication donné
** Renvoie 0 si ok.
** bwwait: 1 si on attend que toutes les données en partance soient écrites
**         avant de fermer, 0 sinon
*/
int slClosePort(SLPortPtr *pptr,int bwwait)
{
  SLPortPtr ptr;
  int ret=SL_SUCCESSFUL;

  ptr = *pptr;
  if(ptr == NULL)
    return SL_ERROR;

  if(bwwait)
    tcdrain(ptr->fd);
  tcflush(ptr->fd,TCIOFLUSH);
  tcsetattr(ptr->fd,TCSANOW,&ptr->old_io);
  ret = close(ptr->fd);
  free(ptr);
  *pptr = NULL;
  return ret;
}
/*
** ----------------------------------------------------------------------------
** Paramètrage du port donné avec les informations données
** Renvoie 0 si ok.
** vit   : vitesse (50..230400)
** parite: parite ('N'one,'E'ven,'O'dd)
** bdat  : bits de données (5..8)
** stop  : nombre de bits de stop
*/
int slSetPortAttr(SLPortPtr *pptr,int vit,int parite,int bdat,int stop)
{
  SLPortPtr ptr;
  int ret=SL_SUCCESSFUL;

  ptr = *pptr;
  if(ptr == NULL)
    return SL_ERROR;

  if((vit<50) || (vit>230400))
    return SL_ERROR;
  if(strchr("NEO",parite) == NULL)
    return SL_ERROR;
  if((bdat<5) || (bdat>8))
    return SL_ERROR;
  if((stop<1) || (stop>2))
    return SL_ERROR;

  ptr->param.speed  = vit;
  ptr->param.parity = parite;
  ptr->param.bdata  = bdat;
  ptr->param.bstop  = stop;

  /* On passe en mode raw */
  cfmakeraw(&ptr->cfg_io);

  /* Vitesse */
  switch(ptr->param.speed)
  {
    case 50     : { cfsetispeed(&ptr->cfg_io,B50)    ; cfsetospeed(&ptr->cfg_io,B50)    ; break; }
    case 75     : { cfsetispeed(&ptr->cfg_io,B75)    ; cfsetospeed(&ptr->cfg_io,B75)    ; break; }
    case 110    : { cfsetispeed(&ptr->cfg_io,B110)   ; cfsetospeed(&ptr->cfg_io,B110)   ; break; }
    case 134    : { cfsetispeed(&ptr->cfg_io,B134)   ; cfsetospeed(&ptr->cfg_io,B134)   ; break; }
    case 150    : { cfsetispeed(&ptr->cfg_io,B150)   ; cfsetospeed(&ptr->cfg_io,B150)   ; break; }
    case 200    : { cfsetispeed(&ptr->cfg_io,B200)   ; cfsetospeed(&ptr->cfg_io,B200)   ; break; }
    case 300    : { cfsetispeed(&ptr->cfg_io,B300)   ; cfsetospeed(&ptr->cfg_io,B300)   ; break; }
    case 600    : { cfsetispeed(&ptr->cfg_io,B600)   ; cfsetospeed(&ptr->cfg_io,B600)   ; break; }
    case 1200   : { cfsetispeed(&ptr->cfg_io,B1200)  ; cfsetospeed(&ptr->cfg_io,B1200)  ; break; }
    case 1800   : { cfsetispeed(&ptr->cfg_io,B1800)  ; cfsetospeed(&ptr->cfg_io,B1800)  ; break; }
    case 2400   : { cfsetispeed(&ptr->cfg_io,B2400)  ; cfsetospeed(&ptr->cfg_io,B2400)  ; break; }
    case 4800   : { cfsetispeed(&ptr->cfg_io,B4800)  ; cfsetospeed(&ptr->cfg_io,B4800)  ; break; }
    case 9600   : { cfsetispeed(&ptr->cfg_io,B9600)  ; cfsetospeed(&ptr->cfg_io,B9600)  ; break; }
    case 19200  : { cfsetispeed(&ptr->cfg_io,B19200) ; cfsetospeed(&ptr->cfg_io,B19200) ; break; }
    case 38400  : { cfsetispeed(&ptr->cfg_io,B38400) ; cfsetospeed(&ptr->cfg_io,B38400) ; break; }
    case 57600  : { cfsetispeed(&ptr->cfg_io,B57600) ; cfsetospeed(&ptr->cfg_io,B57600) ; break; }
    case 115200 : { cfsetispeed(&ptr->cfg_io,B115200); cfsetospeed(&ptr->cfg_io,B115200); break; }
    case 230400 : { cfsetispeed(&ptr->cfg_io,B230400); cfsetospeed(&ptr->cfg_io,B230400); break; }
  }

  /* Parite */
  switch(ptr->param.parity)
  {
    case 'N' : { ptr->cfg_io.c_cflag &= ~PARENB; break; }
    case 'E' : { ptr->cfg_io.c_cflag |= PARENB; ptr->cfg_io.c_cflag &= ~PARODD; break; }
    case 'O' : { ptr->cfg_io.c_cflag |= PARENB; ptr->cfg_io.c_cflag |= PARODD ; break; }
  }

  /* Longueur */
  ptr->cfg_io.c_cflag &= ~CSIZE;
  switch(ptr->param.bdata)
  {
    case 5 : { ptr->cfg_io.c_cflag |= CS5; break; }
    case 6 : { ptr->cfg_io.c_cflag |= CS6; break; }
    case 7 : { ptr->cfg_io.c_cflag |= CS7; break; }
    case 8 : { ptr->cfg_io.c_cflag |= CS8; break; }
  }

  /* Bits de stop */
  if(ptr->param.bstop == 1)
    ptr->cfg_io.c_cflag &= ~ CSTOPB;
  else
    ptr->cfg_io.c_cflag |= CSTOPB;

  ret = tcsetattr(ptr->fd,TCSANOW,&ptr->cfg_io);
  return ret;
}
/*
** ----------------------------------------------------------------------------
** Initialise le timeout du port donné avec la valeur donnée
*/
int slSetTimeout(SLPortPtr *pptr,int tout)
{
  SLPortPtr ptr=*pptr;

  if(ptr == NULL)
    return SL_ERROR;
  ptr->timeout = tout;
  return SL_SUCCESSFUL;
}
/*
** ----------------------------------------------------------------------------
** Renvoie le nom du device du port donné
*/
char *slGetDeviceName(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->dev : (char *)NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoie le descripteur du device du port donné
*/
int slGetFileDesc(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->fd : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie la vitesse du port donné
*/
int slGetSpeed(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->param.speed : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie la parite du port donné
*/
int slGetParity(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->param.parity : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie la longueur des données du port donné
*/
int slGetLenData(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->param.bdata : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie le nombre de bits de stop du port donné
*/
int slGetBitStop(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->param.bstop : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie le timeout du port donné
*/
int slGetTimeout(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->timeout : 0);
}
/*
** ----------------------------------------------------------------------------
** Renvoie le flag breceive du port donné
*/
int slGetbReceive(SLPortPtr *pptr)
{
  SLPortPtr ptr=*pptr;

  return (ptr ? ptr->breceive : 0);
}
/*
** ----------------------------------------------------------------------------
** Flush la queue d'emission du port donné
*/
int slFlushTransmitQueue(SLPortPtr *pptr)
{
  SLPortPtr ptr;
  int ret=SL_SUCCESSFUL;

  ptr = *pptr;
  if(ptr)
    ret = tcflush(ptr->fd,TCOFLUSH);
  return ret;
}
/*
** ----------------------------------------------------------------------------
** Flush la queue de réception du port donné
*/
int slFlushReceiveQueue(SLPortPtr *pptr)
{
  SLPortPtr ptr;
  int ret=SL_SUCCESSFUL;

  ptr = *pptr;
  if(ptr)
    ret = tcflush(ptr->fd,TCIFLUSH);
  return ret;
}
/*
** ----------------------------------------------------------------------------
** Attente de réception sur un ensemble de ports
** tptr : tableau des ports à scanner
** count: taille de tptr
** tout : timeout en seconde
**
** Renvoie SL_SUCCESSFUL si ok
*/
int slWaitReceiving(SLPortPtr tptr[],int count,int tout)
{
  SLPortPtr ptr;
  fd_set rfds;
  int maxfd=0,ret,i;
  struct timeval tv;

  /* Initialisation de l'ensemble des descripteurs pour select */
  FD_ZERO(&rfds);
  for(i=0;i<count;i++)
  {
    if(tptr[i])
    {
      ptr           = tptr[i];
      ptr->breceive = 0;
      FD_SET(ptr->fd,&rfds);
      if(ptr->fd>maxfd)
	maxfd = ptr->fd;
    }
  }

  for(;;)
  {
    tv.tv_sec  = tout;
    tv.tv_usec = 0;
    errno      = 0;
    ret = select(maxfd+1,&rfds,NULL,NULL,&tv);

    if(ret < 0)
    {
      if(errno == EINTR)      /* Interrompu par un signal */
	continue;
      /* Erreur */
      return SL_ERROR;
    }
    /* Ici, on a reçu quelque chose, ou on est en timeout */
    break;
  }

  for(i=0;i<count;i++)
  {
    if(tptr[i])
    {
      ptr = tptr[i];
      if(FD_ISSET(ptr->fd,&rfds))
	ptr->breceive = 1;
    }
  }

  return 0;
}
/*
** ----------------------------------------------------------------------------
** Lecture du port donné.
** buf    : contiendra les données reçues
** count  : taille du buffer de réception
** cfin   : stoppe la réception si le caractère 'cfin' est reçu
** nbc    : contiendra le nombre de caractères recus
**
** Renvoie SL_SUCCESSFUL si ok
**
*/
int slReceiveBuffer(SLPortPtr *pptr,unsigned char *buf,int count,unsigned char cfin,int *nbc)
{
  SLPortPtr ptr;
  fd_set rfds;
  struct timeval tv;
  unsigned char carlu;
  int n,fin,nlus,i,ret;

  ptr = *pptr;
  if(ptr == NULL)
    return SL_ERROR;

  i    = 0;
  nlus = 0;
  /*
  ** Boucle de lecture
  */
  fin = 0;
  while(!fin)
  {
    FD_ZERO(&rfds);
    FD_SET(ptr->fd,&rfds);
    tv.tv_sec  = ptr->timeout;
    tv.tv_usec = 0;
    errno      = 0;

    ret = select(ptr->fd+1,&rfds,NULL,NULL,&tv);

    if(ret < 0)
    {
      if(errno == EINTR)      /* Interrompu par un signal */
	continue;
      /* Erreur */
      return SL_ERROR;
    }
    if(ret == 0)
    {
      /* Timeout? */
      if(!FD_ISSET(ptr->fd,&rfds))
      {
	/* Plus rien à lire, on sort */
	if(nbc != NULL)
	  *nbc = nlus;
	return SL_TIMEOUT;
      }
    }

    /*
    ** Lecture du port
    */
    for(;;)
    {
      /*
      ** RAZ des conditions d'erreurs
      ** (errno n'est pas réinitialisé, si un signal interrompt select, il reste à
      ** EINTR et ca fait boucler le read)
      */
      errno = 0;
      n = read(ptr->fd,&carlu,1);
      if(n<0)
      {
	if(errno == EINTR) /* Interrompu par un signal */
	  continue;
	if(errno == EAGAIN)
	{
	  /* Rien à lire en mode non-bloquant */
	  /* fin = 1; */
	  break;
	}
	/* Erreur */
	return SL_ERROR;
      }
      if((n == 0) || (n == EOF))
      {
	/* EOF */
	fin = 1;
	break;
      }
      /* Ajout du caractère au buffer */
      buf[i] = carlu;
      i++;
      nlus++;
      if(i>=count)
      {
	/* Buffer plein */
	fin = 1;
	break;
      }
      if(carlu == cfin)
      {
	/* Fin de lecture */
	if(nbc != NULL)
	  *nbc = nlus;
	return SL_TERMINATORREACHED;
      }
    }
  }
  if(nbc != NULL)
    *nbc = nlus;
  return SL_SUCCESSFUL;
}
/*
** ----------------------------------------------------------------------------
** Lecture d'un caractère sur le port donné.
**
** Renvoie le code du caractère lu ou un code d'erreur.
**
*/
int slReceiveChar(SLPortPtr *pptr)
{
  SLPortPtr ptr;
  fd_set rfds;
  struct timeval tv;
  unsigned char carlu;
  int n,fin,ret;

  ptr = *pptr;
  if(ptr == NULL)
    return SL_ERROR;

  /*
  ** Boucle de lecture
  */
  fin = 0;
  while(!fin)
  {
    FD_ZERO(&rfds);
    FD_SET(ptr->fd,&rfds);
    tv.tv_sec  = ptr->timeout;
    tv.tv_usec = 0;
    errno      = 0;

    ret = select(ptr->fd+1,&rfds,NULL,NULL,&tv);

    if(ret < 0)
    {
      if(errno == EINTR)      /* Interrompu par un signal */
	continue;
      /* Erreur */
      return SL_ERROR;
    }
    if(ret == 0)
    {
      /* Timeout? */
      if(!FD_ISSET(ptr->fd,&rfds))
      {
	/* Plus rien à lire, on sort */
	return SL_TIMEOUT;
      }
    }

    /*
    ** Lecture du port
    */
    for(;;)
    {
      /*
      ** RAZ des conditions d'erreurs
      ** (errno n'est pas réinitialisé, si un signal interrompt select, il reste à
      ** EINTR et ca fait boucler le read)
      */
      errno = 0;
      n = read(ptr->fd,&carlu,1);
      if(n<0)
      {
	if(errno == EINTR) /* Interrompu par un signal */
	  continue;
	if(errno == EAGAIN)
	{
	  /* Rien à lire en mode non-bloquant */
	  /* fin = 1; */
	  break;
	}
	/* Erreur */
	return SL_ERROR;
      }
      if((n == 0) || (n == EOF))
      {
	/* EOF */
	return SL_QUEUEISEMPTY;
	break;
      }
      return (int) carlu;
    }
  }
  return SL_SUCCESSFUL;
}
/*
** ----------------------------------------------------------------------------
** Envoi d'un buffer sur le port donné.
** buf    : contient les donnée à envoyer
** count  : longueur du buffer
** bwwait : 1 si on attend que tout soit parti, 0 sinon
**
** Renvoie SL_SUCCESSFUL ou un code d'erreur.
**
*/
int slTransmitBuffer(SLPortPtr *pptr,unsigned char *buf,int count,int bwwait)
{
  SLPortPtr ptr;
  fd_set wfds;
  struct timeval tv;
  int fin,ret,restants,ecrits;
  unsigned char *debut;

  ptr = *pptr;
  if(ptr == NULL)
    return SL_ERROR;

  /*
  ** Boucle d'emission
  */
  restants = count;
  debut    = buf;
  fin      = 0;
  while(!fin)
  {
    FD_ZERO(&wfds);
    FD_SET(ptr->fd,&wfds);
    tv.tv_sec  = ptr->timeout;
    tv.tv_usec = 0;
    errno      = 0;

    ret = select(ptr->fd+1,NULL,&wfds,NULL,&tv);

    if(ret < 0)
    {
      if(errno == EINTR)      /* Interrompu par un signal */
	continue;
      /* Erreur */
      return SL_ERROR;
    }
    if(ret == 0)
    {
      /* Timeout? */
      if(!FD_ISSET(ptr->fd,&wfds))
      {
	/* Plus rien à écrire, on sort */
	return SL_TIMEOUT;
      }
    }

    /*
    ** Envoi
    */
    for(;;)
    {
      /*
      ** RAZ des conditions d'erreurs
      ** (errno n'est pas réinitialisé, si un signal interrompt select, il reste à
      ** EINTR et ca fait boucler le write)
      */
      errno = 0;
      ecrits = write(ptr->fd,debut,restants);
      if(ecrits<0)
      {
	if(errno == EINTR) /* Interrompu par un signal */
	  continue;
	if(errno == EAGAIN)
	{
	  /* Mode non-bloquant */
	  /* fin = 1; */
	  break;
	}
	/* Erreur */
	return SL_ERROR;
      }
      restants -= ecrits;
      debut    += ecrits;
      if(restants<=0)
      {
	/* fini */
	fin = 1;
	if(bwwait)
	  tcdrain(ptr->fd);
	break;
      }
    }
  }
  return SL_SUCCESSFUL;
}
/*
** ----------------------------------------------------------------------------
*/
