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
** Traitement des appels entrants
*/
#include "common.h"
#include "s_globals.h"


/*
** ----------------------------------------------------------------------------
** Traitement d'un appel entrant pour le fils donné
*/
gint _ProcessCall(_FxChildPtr _self)
{
  gchar *_procname="_ProcessCall";
  gchar *fin;
  fd_set rfds;
  struct timeval tv;
  gint retry,end_rq,ret,nb_rq;

  _PrintError(LOG_DEBUG,"-- (%s), Appel entrant (pid=%d ip=%s)",_procname,getpid(),inet_ntoa(_self->Stream->adresse.sin_addr));
  _self->Stream->state = CHLD_READ_SOCKET;
  end_rq               = 0;

  /*
  ** On associe un FILE* au descripteur de la socket,
  ** plus pratique pour la lecture de chaines
  */
  if((_self->Stream->fp = fdopen(_self->Stream->sock, "r+")) == NULL)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur fdopen (%d %s)",_procname,errno,strerror(errno));
    return (1);
  }

  nb_rq  = 0;
  /* On tente de lire _swRetryMax fois avec _swTimeoutRQ secondes de délai */
  retry  = 0;
  while(retry<_swRetryMax)
  {
    /* Timeout de _swTimeoutRQ secondes */
    FD_ZERO(&rfds);
    FD_SET(_self->Stream->sock,&rfds);
    tv.tv_sec  = _swTimeoutRQ;
    tv.tv_usec = 0;
    errno      = 0;
    ret = select(_self->Stream->sock+1,&rfds,NULL,NULL,&tv);

#ifdef GXE_DEBUG
    _PrintError(LOG_NOTICE,"== select renvoi [%d] ==",ret);
#endif

    if(ret < 0)
    {
      if(errno == EINTR) /* Interrompu par un signal */
	continue;
      _PrintError(LOG_CRIT,"-- (%s), erreur sur select (%d %s). Sortie",_procname,errno,strerror(errno));
      break;
    }

    if(ret == 0)
    {
      /* Timeout? */
      if(!FD_ISSET(_self->Stream->sock,&rfds))
      {
	retry++;
	_PrintError(LOG_NOTICE,"-- (%s), timeout (retry=%d)",_procname,retry);
	continue;
      }
    }

    /* On 'réarme' la tempo */
    retry = 0;
    /*
    ** Lecture de la socket
    */
    for(;;)
    {
      /*
      ** RAZ des contidions d'erreurs
      ** (errno n'est pas réinitialisé, si un signal interrompt select, il reste à
      ** EINTR et ca fait boucler le fgets)
      */
      clearerr(_self->Stream->fp);
      errno = 0;
      memset(_self->Stream->buff,0,_FxBUFF_SIZE+1);
      if(fgets(_self->Stream->buff,_FxBUFF_SIZE,_self->Stream->fp) == NULL)
      {
	if(errno == EINTR) /* Interrompu par un signal */
	  continue;
	if(!feof(_self->Stream->fp))
	  _PrintError(LOG_CRIT,"-- (%s), erreur sur fgets (%d %s). Sortie",_procname,errno,strerror(errno));
	else
	  _PrintError(LOG_CRIT,"-- (%s), erreur sur fgets (EOF). Sortie",_procname);
	return (0);
      }
      if(strspn(_self->Stream->buff,"\n\r") == strlen(_self->Stream->buff))
      {
	/*
	** La chaine reçue ne contient que CR
	*/
	end_rq++;
	if(end_rq == strlen(_FxEND_RQ))
	{
#ifdef GXE_DEBUG
	  _PrintError(LOG_DEBUG,"-- Analyse de la requête");
#endif
	  /*
	  ** Analyse de la requète
	  ** =====================
	  */
	  _ParseRequete(_self);

	  /*
	  ** Requète suivante
	  */
	  _self->Stream->state = CHLD_READ_SOCKET;
	  _self->Stream->nbrq++;
	  _self->Stream->Requete = g_string_truncate(_self->Stream->Requete,0);
	  if(_NewStreamxmlOut(_self->Stream) != 0)
	  {
	    _PrintError(LOG_CRIT,"-- (%s), erreur sur _NewStreamxmlOut. Sortie",_procname);
	    return (0);
	  }
	  end_rq  = 0;
	  nb_rq++;
	  if(nb_rq >= _swMaxRQ)
	  {
	    /* On a lu le maximum autorisé pour ce canal, on sort */
	    retry = _swRetryMax;
	    _PrintError(LOG_NOTICE,"(%s), %d requête(s) lue(s). Sortie.",_procname,_swMaxRQ);
	  }
	  /* On sort de la boucle de lecture socket */
	  break;
	}
      }
      /* Elimination du CRLF */
      if((fin = strpbrk(_self->Stream->buff,"\n\r")) != NULL)
	fin[0] = '\0';

#ifdef GXE_DEBUG
      if(strlen(_self->Stream->buff))
	_PrintError(LOG_DEBUG,"-- reçu CHAINE=[%s]",_self->Stream->buff);
#endif

      /* Ajout à la requète courante */
      _self->Stream->Requete = g_string_append(_self->Stream->Requete,_self->Stream->buff);
    }
  }

  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
