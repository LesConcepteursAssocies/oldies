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
** Routines diverses
*/
#include "common.h"
#include "s_globals.h"

static gchar *ChildStateStr[] =
{
  "READ_SOCKET",
  "PARSE_REQUEST",
  "RUN_CMD",
  "UNDEF"
};

static gchar *StreamTypeStr[] =
{
  "MONOCLIENT",
  "FRONTAL",
  "UNDEF"
};

static gchar *OutputModeStr[] =
{
  "XML",
  "ASCII",
  "UNDEF"
};

static gchar *ParamTypeStr[] =
{
  "SINGLE",
  "LIST",
  "UNDEF"
};

/*
** ----------------------------------------------------------------------------
** Initialisation du process principal
*/
gint _InitMain(void)
{
  gchar *_procname="_InitMain";

  _swPid         = getpid();
  /*
  ** Initialisation des verrous
  */
  mutex_TCmd     = g_malloc0(sizeof(pthread_mutex_t));
  if(mutex_TCmd == NULL)
  {
    syslog(LOG_CRIT,"!! _InitMain, mémoire insuffisante (mutex_TCmd)");
    return (-1);
  }
  mutex_TModules = g_malloc0(sizeof(pthread_mutex_t));
  if(mutex_TModules == NULL)
  {
    syslog(LOG_CRIT,"!! _InitMain, mémoire insuffisante (mutex_TModules)");
    pthread_mutex_destroy(mutex_TCmd);
    return (-1);
  }
  mutex_TChilds  = g_malloc0(sizeof(pthread_mutex_t));
  if(mutex_TChilds == NULL)
  {
    syslog(LOG_CRIT,"!! _InitMain, mémoire insuffisante (mutex_TChilds)");
    pthread_mutex_destroy(mutex_TCmd);
    pthread_mutex_destroy(mutex_TModules);
    return (-1);
  }
  pthread_mutex_init(mutex_TCmd,NULL);
  pthread_mutex_init(mutex_TModules,NULL);
  pthread_mutex_init(mutex_TChilds,NULL);
  if(setpgid(0,0) != 0)
  {
    _PrintError(LOG_CRIT,"(%s), erreur sur setpgid (%d %s)\n",_procname,errno,strerror(errno));
    pthread_mutex_destroy(mutex_TCmd);
    pthread_mutex_destroy(mutex_TModules);
    pthread_mutex_destroy(mutex_TChilds);
    return (-1);
  }
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Initialisations diverses
*/
void _InitApp(void)
{
  gchar *_procname="_InitApp";

  /*
  ** Valeurs par défauts des divers paramètres
  */
  _CurrentDir  = NULL;
  _swPort      = SERV_PORT;
  _swMaxRQ     = FX_MAX_REQUEST;
  _swRetryMax  = S_RETRY_CALL;
  _swTimeoutRQ = S_TIMEOUT_CALL;

  _swSession = NULL;
  _TModules  = NULL;
  _TFxCmds   = NULL;
  _TChilds   = NULL;
  _ReadCfgFile();
  if(_CurrentDir == NULL)
    _CurrentDir = g_get_current_dir();
  if(_CurrentDir)
    _PrintError(LOG_DEBUG,"(%s), CurrentDir=[%s]",_procname,_CurrentDir);
}
/*
** ----------------------------------------------------------------------------
** Libération de ressources diverses
*/
void _FreeRessources(void)
{
  g_free(_CurrentDir);
  _CurrentDir = NULL;
  g_free(_ModulesPath);
  _ModulesPath = NULL;
}
/*
** ----------------------------------------------------------------------------
** Création du document xml de sortie pour le canal courant
** Renvoie 0 si ok.
*/
gint _NewStreamxmlOut(_FxStreamPtr ptr)
{
  gchar *_procname="_NewStreamxmlOut";

  if(ptr->Out)
  {
    xmlFreeDoc(ptr->Out);
    ptr->Out = NULL;
  }
  ptr->Out = xmlNewDoc("1.0");
  if(ptr->Out == NULL)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur dans xmlNewDoc",_procname);
    return (-1);
  }
  ptr->Out->children = xmlNewDocNode(ptr->Out,NULL,S_FXML_START,NULL);
  if(ptr->Out->children == NULL)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur dans xmlNewDocNode",_procname);
    xmlFreeDoc(ptr->Out);
    ptr->Out = NULL;
    return (-1);
  }
  xmlSetProp(ptr->Out->children,"version",S_FXML_VERSION);
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Création d'un canal de communication pour le socket donné.
** Renvoie un pointeur sur la structure allouée, NULL sinon
*/
_FxStreamPtr _NewFxStream(gint s)
{
  gchar *_procname="_NewFxStream";
  _FxStreamPtr ptr;
  socklen_t longueur;

  ptr = (_FxStreamPtr)g_malloc0(sizeof(struct _FxStream));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! _NewFxStream, mémoire insuffisante");
    return (NULL);
  }
  /* On récupère l'addresse IP du client de la socket */
  longueur = sizeof(struct sockaddr_in);
  if (getpeername(s,&ptr->adresse,&longueur) < 0)
  {
    _PrintError(LOG_NOTICE,"-- (%s), erreur(s) sur getsockname (%d %s)",_procname,errno,strerror(errno));
  }
  /* MONOCLIENT par défaut */
  ptr->type    = FXS_MONOCLIENT;
  ptr->sock    = s;
  ptr->Requete = g_string_new("");
  ptr->omode   = OM_XML;

  /* Création du document xml de sortie */
  if(_NewStreamxmlOut(ptr) != 0)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur dans _NewStreamxmlOut",_procname);
    g_string_free(ptr->Requete,1);
    g_free(ptr);
    return (NULL);
  }
  ptr->strOut = g_string_new("");

  return (ptr);
}
/*
** ----------------------------------------------------------------------------
** Libération des ressources occupées par le canal donné
** si bclose est vrai, on ferme aussi le fichier associé au socket
*/
void _FreeFxStream(_FxStreamPtr ptr,gint bclose)
{
  if(ptr)
  {
    if(bclose)
      fclose(ptr->fp);
    g_string_free(ptr->Requete,1);
    _FreeLAttr(ptr->LAttr);
    if(ptr->Out)
      xmlFreeDoc(ptr->Out);
    g_string_free(ptr->strOut,1);
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
*/
gchar *_StrChildState(gint etat)
{
  if(etat<CHLD_UNDEF)
    return((gchar *)ChildStateStr[etat]);
  return((gchar *)ChildStateStr[CHLD_UNDEF]);
}
/*
** ----------------------------------------------------------------------------
*/
gchar *_StrStreamType(gint etat)
{
  if(etat<FXS_UNDEF)
    return((gchar *)StreamTypeStr[etat]);
  return((gchar *)ChildStateStr[FXS_UNDEF]);
}
/*
** ----------------------------------------------------------------------------
*/
gchar *_StrOutputMode(gint etat)
{
  if(etat<OM_UNDEF)
    return((gchar *)OutputModeStr[etat]);
  return((gchar *)OutputModeStr[OM_UNDEF]);
}
/*
** ----------------------------------------------------------------------------
*/
gchar *_StrParamType(gint etat)
{
  if(etat<T_UNDEF)
    return((gchar *)ParamTypeStr[etat]);
  return((gchar *)ParamTypeStr[T_UNDEF]);
}
/*
** ----------------------------------------------------------------------------
** Lecture du fichier de configuration
** Renvoie 0 si ok.
*/
gint _ReadCfgFile(void)
{
  gchar *_procname="_ReadCfgFile";
  FILE *fp;
  gchar Ligne[_FxBUFF_SIZE+1];
  gchar *line,**param;

  if((fp=fopen(FX_FILE_CFG,"r")) == NULL)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur fopen (%d %s)",_procname,errno,strerror(errno));
    return (-1);
  }

  while(!feof(fp))
  {
    memset(Ligne,0,_FxBUFF_SIZE+1);
    if(fgets(Ligne,_FxBUFF_SIZE,fp) == NULL)
    {
      if(errno == EINTR) /* Interrompu par un signal */
	continue;
      if(!feof(fp))
	_PrintError(LOG_CRIT,"-- (%s), erreur sur fgets (%d %s). Sortie",_procname,errno,strerror(errno));
      fclose(fp);
      return (0);
    }
    line = g_strstrip(Ligne);
    if(strlen(line) <= 0)
      continue;
    if(*line == '#')
      continue;
    /*
    ** Récupération des paramètres
    ** (forme 'nom = valeur')
    */
    param    = g_strsplit(line,"=",2);
    param[0] = g_strstrip(param[0]);
    param[1] = g_strstrip(param[1]);

    /*
    ** Définition du port d'écoute
    */
    if(strcasecmp(param[0],"port") == 0)
    {
      _swPort = atoi(param[1]);
      g_strfreev(param);
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"(%s), paramètre Port=%d",_procname,_swPort);
#endif
      continue;
    }
    /*
    ** Chemin principal de l'application
    */
    if(strcasecmp(param[0],"path") == 0)
    {
      _CurrentDir = g_strdup(param[1]);
      g_strfreev(param);
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"(%s), paramètre Path=%s",_procname,_CurrentDir);
#endif
      continue;
    }
    /*
    ** Nombre maximum de requêtes lisibles sur un canal
    ** avant fermeture
    */
    if(strcasecmp(param[0],"maxrequests") == 0)
    {
      _swMaxRQ = atoi(param[1]);
      g_strfreev(param);
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"(%s), paramètre MaxRequests=%d",_procname,_swMaxRQ);
#endif
      continue;
    }
    /*
    ** Nombre maximum de tentatives de lecture
    ** d'un canal
    */
    if(strcasecmp(param[0],"maxretry") == 0)
    {
      _swRetryMax = atoi(param[1]);
      g_strfreev(param);
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"(%s), paramètre MaxRetry=%d",_procname,_swRetryMax);
#endif
      continue;
    }
    /*
    ** Timeout sur lecture d'une requête
    */
    if(strcasecmp(param[0],"timeout") == 0)
    {
      _swTimeoutRQ = atoi(param[1]);
      g_strfreev(param);
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"(%s), paramètre Timeout=%d",_procname,_swTimeoutRQ);
#endif
      continue;
    }
  }
  fclose(fp);
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Dump des infos disponibles du paramètre donné
*/
void _DumpParam(_FxParamPtr ptr)
{
  GSList *lst;

  if(ptr)
  {
    _PrintError(LOG_NOTICE,"=== name            : %s",(ptr->name?ptr->name:"NULL"));
    _PrintError(LOG_NOTICE,"=== type            : %s",_StrParamType(ptr->type));
    switch(ptr->type)
    {
      case T_SINGLE:
      {
	_PrintError(LOG_NOTICE,"=== value           : %s",ptr->v.value->str);
	break;
      }
      case T_LIST:
      {
	lst = ptr->v.LParam;
	while(lst)
	{
	  _DumpParam(lst->data);
	  lst = g_slist_next(lst);
	}
	break;
      }
      case T_UNDEF:
	break;
    }
  }
}
/*
** ----------------------------------------------------------------------------
** Dump des infos disponibles du fils donné
** utilisé sur SIGSEGV, afin de permettre une
** analyse post-mortem (RIP)
*/
void _DumpChild(_FxChildPtr _self)
{
  GSList *lst;
  _ModulePtr mdl;

  _PrintError(LOG_NOTICE,"===== Dump du fils =====");

  _PrintError(LOG_NOTICE,"== CurrentDir       : %s",(_CurrentDir?_CurrentDir:"NULL"));
  _PrintError(LOG_NOTICE,"== Pid              : %d",_swPid);
  _PrintError(LOG_NOTICE,"== swPort           : %d",_swPort);
  _PrintError(LOG_NOTICE,"== swMaxRQ          : %d",_swMaxRQ);
  _PrintError(LOG_NOTICE,"== swRetryMax       : %d",_swRetryMax);
  _PrintError(LOG_NOTICE,"== swTimeoutRQ      : %d",_swTimeoutRQ);
  _PrintError(LOG_NOTICE,"== ModulesPath      : %s",(_ModulesPath?_ModulesPath:"NULL"));

  _PrintError(LOG_NOTICE,"===== LISTE DES MODULES DYNAMIQUES");
  lst = _TModules;
  while(lst)
  {
    mdl = lst->data;
    if(mdl)
    {
      _PrintError(LOG_NOTICE,"=== name            : %s",(mdl->name?mdl->name:"NULL"));
      _PrintError(LOG_NOTICE,"=== mtime           : %d",mdl->mtime);
      _PrintError(LOG_NOTICE,"=== pmodule         : %p",mdl->pmodule);
    }
    lst = g_slist_next(lst);
  }

  if(_swSession != NULL)
  {
    _PrintError(LOG_NOTICE,"===== ETAT DE LA SESSION");
    _PrintError(LOG_NOTICE,"=== nom du frontal   : %s",_swSession->frontalname);
    _PrintError(LOG_NOTICE,"=== session ID       : %s",_swSession->sessionID);
  }

  if(_self != NULL)
  {
    if(_self->Stream != NULL)
    {
      _PrintError(LOG_NOTICE,"== Type de canal    : %s",_StrStreamType(_self->Stream->type));
      _PrintError(LOG_NOTICE,"== Etat du fils     : %s",_StrChildState(_self->Stream->state));
      _PrintError(LOG_NOTICE,"== socket           : %d",_self->Stream->sock);
      _PrintError(LOG_NOTICE,"== addr. IP         : %s",inet_ntoa(_self->Stream->adresse.sin_addr));
      _PrintError(LOG_NOTICE,"== fp               : %p",_self->Stream->fp);
      _PrintError(LOG_NOTICE,"== requêtes traitées: %d",_self->Stream->nbrq);
      _PrintError(LOG_NOTICE,"== Mode de sortie   : %s",_StrOutputMode(_self->Stream->omode));

      switch(_self->Stream->state)
      {
	case CHLD_READ_SOCKET:
	{
	  _PrintError(LOG_NOTICE,"== buffer           : %s",_self->Stream->buff);
	  break;
	}
	case CHLD_PARSE_REQUEST:
	case CHLD_RUN_CMD:
	{
	  _PrintError(LOG_NOTICE,"===== ETAT DU PARSER XML");
	  _PrintError(LOG_NOTICE,"=== requête courante : %s",_self->Stream->Requete->str);
	  _PrintError(LOG_NOTICE,"=== etat courant     : %s",_StrParserState(_self->Parser->state));
	  _PrintError(LOG_NOTICE,"=== etat precedent   : %s",_StrParserState(_self->Parser->prev_state));
	  _PrintError(LOG_NOTICE,"=== errors           : %d",_self->Parser->errors);
	  _PrintError(LOG_NOTICE,"=== tagerror         : %s",(_self->Parser->tagerror?_self->Parser->tagerror:"NULL"));
	  _PrintError(LOG_NOTICE,"=== xmlerrno         : %d",_self->Parser->xmlerrno);
	  _PrintError(LOG_NOTICE,"=== xmlerror         : %s",(_self->Parser->xmlerror?_self->Parser->xmlerror:"NULL"));	

	  if(_self->Stream->state == CHLD_PARSE_REQUEST)
	    break;

	  if(_self->Parser->currentcmd == NULL)
	  {
	    _PrintError(LOG_NOTICE,"=== currentcmd NULL !!!");
	    break;
	  }
	  _PrintError(LOG_NOTICE,"===== ETAT DE LA COMMANDE COURANTE");
	  if(_self->Parser->currentcmd->cmd == NULL)
	    _PrintError(LOG_NOTICE,"=== currentcmd->cmd NULL !!!");
	  else
	  {
	    _PrintError(LOG_NOTICE,"=== name             : %s",(_self->Parser->currentcmd->cmd->name?_self->Parser->currentcmd->cmd->name:"NULL"));
	    _PrintError(LOG_NOTICE,"=== pmodule          : %p",_self->Parser->currentcmd->cmd->pmodule);
	  }
	  if(_self->Parser->currentcmd->LParam == NULL)
	    _PrintError(LOG_NOTICE,"=== pas de parametres");
	  else
	  {
	    _PrintError(LOG_NOTICE,"=== LISTE DES PARAMETRES");
	    lst = _self->Parser->currentcmd->LParam;
	    while(lst)
	    {
	      _DumpParam(lst->data);
	      lst = g_slist_next(lst);
	    }
	  }
	  break;
	}
	case CHLD_UNDEF:
	default:
	{
	  break;
	}
      }
    }
  }
  _PrintError(LOG_NOTICE,"===== Fin du Dump =====");
}
/*
** ----------------------------------------------------------------------------
*/
