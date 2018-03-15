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
** Programme principal
*/
#define S_ALLOCATE 1
#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
*/
int main(int argc, char **argv)
{
  gchar *_procname="main";
  gint s;
  gint t;

  _InitError();
  _PrintError(LOG_NOTICE,"(%s), Lancement de %s",_procname,SERV_NAME);

  if(_InitMain()<0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur(s) dans _InitMain. Sortie",_procname);
    exit(1);
  }
  _InitSignaux();

  _InitApp();

  /* Création de la socket */
  if((s=_CreerSocket(_swPort)) <0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur(s) dans CreerSocket. Sortie",_procname);
    exit(1);
  }

  if(_InitModules() <0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur(s) dans InitModules. Sortie",_procname);
    close(t);
    exit(1);
  }

  /*
  ** Boucle de lecture de la socket
  ** ------------------------------
  */
  for(;;)
  {
    _PrintError(LOG_DEBUG,"(%s), Attente de connexions (port=%d) ...",_procname,_swPort);

    t = _GetConnection(s);
    if(t<0)
    {
      if(errno == EINTR) /* Interrompu par un signal */
	continue;
      _PrintError(LOG_CRIT,"(%s), erreur(s) dans GetConnection. Sortie",_procname);
      exit(1);
    }

    if(_FxCreateNewChild(t)<0)
    {
      _PrintError(LOG_CRIT,"(%s), erreur(s) dans _FxCreateNewchild",_procname);
      close(t);
    }
  }

  _FreeTChilds();
  _CloseModules();
  _FreeTModules();
  _FreeTCmds();
  _FreeRessources();
  close(s);
  pthread_mutex_destroy(mutex_TCmd);
  pthread_mutex_destroy(mutex_TModules);
  pthread_mutex_destroy(mutex_TChilds);
  _PrintError(LOG_NOTICE,"(%s), Sortie normale de %s",_procname,SERV_NAME);
  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
