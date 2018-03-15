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
** Traitement des signaux
*/
#include "common.h"
#include "s_globals.h"


/*
** ----------------------------------------------------------------------------
** Gestionnaire générique des signaux reçus
*/
void _ProcessSignaux(gint num,struct siginfo *info,gpointer dummy)
{
  gchar *_procname="_ProcessSignaux";
  union wait wstatus;
  int olderrno;
  pid_t monpid;

  olderrno = errno;
  monpid   = getpid();
  _PrintError(LOG_NOTICE,"(%s), Signal %d(%s) reçu",_procname,num,g_strsignal(num));
  if(num>=SIGRTMIN)
    _PrintError(LOG_NOTICE,"-- code=%d pid=%d uid=%d",info->si_code,info->si_pid,info->si_uid);

  switch(num)
  {
    case SIGHUP:
    {
      /* Réinitialisations diverses */
      _PrintError(LOG_NOTICE,"-- Fermeture du fichier log.");
      closelog();
      openlog(S_LOG_IDENT,S_LOG_OPTIONS,S_LOG_FACILITY);
      _PrintError(LOG_NOTICE,"(%s), Ouverture du fichier log.",_procname);

      if((_SearchChildStream(CHLD_PARSE_REQUEST) == NULL) && (_SearchChildStream(CHLD_RUN_CMD) == NULL))
      {
	/* Rechargement des modules */
	_CloseModules();
	_FreeTModules();
	_FreeTCmds();
	if(_InitModules() <0)
	{
	  _PrintError(LOG_CRIT,"(%s), erreur(s) dans InitModules. Sortie",_procname);
	}
      }
      else
	_PrintError(LOG_NOTICE,"-- Requête en cours de traitement, rechargement des modules impossible");
      break;
    }
    case SIGINT:
    case SIGQUIT:
    case SIGTERM:
    {
      /* Fin du traitement */
      _FreeTChilds();
      _CloseModules();
      _FreeTModules();
      _FreeTCmds();
      _FreeRessources();
      _PrintError(LOG_NOTICE,"FIN DU PROGRAMME (INTERROMPU PAR UN SIGNAL)");
      exit(num);
      break;
    }
    case SIGSEGV:
    {
      /*
      ** ouah...ca colle ici...ca sent la croquette...
      */
      if(monpid != _swPid)
      {
	/* Un thread est planté ... */
	_DumpChild(_SearchChild(pthread_self()));
	pthread_exit((void *)-1);
      }
      else
      {
	_PrintError(LOG_CRIT,"(%s), erreur(s) fatale(s). Sortie",_procname);
	exit(num);
      }
      break;
    }
    case SIGCHLD:
    {
      /*
      ** Capture le retour des fils tués, pour éviter les zombies.
      ** Normalement plus utile (les threads se terminent autrement)
      */
      while(wait3(&wstatus,WNOHANG,NULL) >0)
	;
      _PrintError(LOG_NOTICE,"-- (%s), Fin du Fils",_procname);
      break;
    }
  }
  errno = olderrno;
}
/*
** ----------------------------------------------------------------------------
** Initialisation des signaux à intercepter
*/
int _InitSignaux(void)
{
  gchar *_procname="_InitSignaux";
  int i;
  struct sigaction action;

  action.sa_sigaction = _ProcessSignaux;
  action.sa_flags     = SA_SIGINFO;
  sigemptyset(&action.sa_mask);
  for(i=1; i<NSIG; i++)
  {
    if(sigaction(i,&action,NULL) < 0)
      _PrintError(LOG_NOTICE,"(%s), signal %d(%s) non intercepté",_procname,i,g_strsignal(i));
  }
  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
