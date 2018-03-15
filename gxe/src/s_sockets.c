/*
** ----------------------------------------------------------------------------
** Gnu Xml Engine
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
** Outils de gestion des sockets
*/
#include "common.h"
#include "s_globals.h"


/*
** ----------------------------------------------------------------------------
** Creation d'un socket sur le port donne
*/
gint _CreerSocket(gint portnum)
{
  gchar *_procname="_CreerSocket";
  gchar myname[MAXHOSTNAMELEN+1];
  gint s,myport;
  gint r, optval=1;
  struct sockaddr_in sa;
  struct hostent *hp;
  struct servent *service;

  memset(&sa,0,sizeof(struct sockaddr_in));
  if(gethostname(myname,MAXHOSTNAMELEN) < 0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur gethostname (%d %s)",_procname,errno,strerror(errno));
    return (-1);
  }
  hp = gethostbyname(myname);
  if(hp == NULL)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur gethostbyname (%d %s)",_procname,errno,strerror(errno));
    return (-1);
  }
  sa.sin_family = hp->h_addrtype;
  /*
  ** Recherche du numero de port
  */
  myport = portnum;
  if(myport == -1)
  {
    /*
    ** Pas de port spécifié, on regarde dans /etc/services,
    ** sinon on prend le port par défaut
    */
    service = getservbyname(SERV_NAME,"tcp");
    if(service != NULL)
      myport = service->s_port;
    else
      myport = SERV_PORT;
  }
  sa.sin_port = htons(myport);
  _swPort     = myport;
  /*
  ** Creation de la socket
  */
  if((s=socket(AF_INET,SOCK_STREAM,0)) < 0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur socket (%d %s)",_procname,errno,strerror(errno));
    return (-1);
  }
  /* Paramétrage de la socket */
  r = setsockopt(s, SOL_SOCKET, SO_KEEPALIVE,(gpointer)&optval,sizeof(optval));
  if (r == -1)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur setsockopt(SO_KEEPALIVE) (%d %s)",_procname,errno,strerror(errno));
    close(s);
    return (-1);
  }
  r = setsockopt(s, SOL_SOCKET, SO_REUSEADDR,(gpointer)&optval,sizeof(optval));
  if (r == -1)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur setsockopt(SO_REUSEADDR) (%d %s)",_procname,errno,strerror(errno));
    close(s);
    return (-1);
  }
  /* Attachement de la socket */
  if(bind(s,&sa,sizeof(struct sockaddr_in)) < 0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur bind (%d %s)",_procname,errno,strerror(errno));
    close(s);
    return (-1);
  }
  /* Ecoute du canal */
  if(listen(s,SOMAXCONN) < 0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur listen (%d %s)",_procname,errno,strerror(errno));
    close(s);
    return (-1);
  }
  return(s);
}
/*
** ----------------------------------------------------------------------------
** Lecture des connexions entrantes sur la socket donnée
**
** Renvoie la nouvelle socket
*/
gint _GetConnection(gint s)
{
  gchar *_procname="_GetConnection";
  struct sockaddr_in isa;
  gint i,t;

  i = sizeof(struct sockaddr_in);
  if((t=accept(s,&isa,&i)) <0)
  {
    if(errno != EINTR)
       _PrintError(LOG_CRIT,"(%s), erreur sur accept (%d %s)",_procname,errno,strerror(errno));
    return(-1);
  }
  return(t);
}
/*
** ----------------------------------------------------------------------------
** Envoi le buffer donné sur le socket donné.
** Renvoi le nombre de caractères écrits ou -1 si erreur
*/
gint _WriteSocket(gint s,const void *buffer,size_t taille)
{
  const void *debut   = buffer;
  size_t      restant = taille;
  ssize_t     ecrits  = 0;

  while(restant > 0)
  {
    while((ecrits = write(s,debut,restant)) == -1)
    {
      if(errno == EINTR) /* Interrompu par un signal */
	continue;
      if(errno !=EAGAIN)
	return (-1);
      sleep (1);
    }
    restant -= ecrits;
    debut += ecrits;
  }
  return (taille);
}
/*
** ----------------------------------------------------------------------------
*/
