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
** Parseur de requètes XML
*/
#include "common.h"
#include "s_globals.h"


const gchar *_Strstates[] =
{
  "START",
  "FINISH",
  "CMD",
  "CMD_ERROR",
  "CMD_PARAM",
  "CMD_PARAM_ERROR",
  "ERROR",
  "UNKNOWN"
};

/*
** ----------------------------------------------------------------------------
** Prototypes des fonctions du parseur xml
*/
static void swStartDocument(_swParserStatePtr);
static void swEndDocument(_swParserStatePtr);
static void swStartElement(_swParserStatePtr,const gchar *,const gchar **);
static void swCharacters(_swParserStatePtr,const gchar *,gint);
static void swEndElement(_swParserStatePtr,const gchar *);
static void swComment(_swParserStatePtr,const gchar *);
static void swWarning(_swParserStatePtr,const gchar *,...);
static void swError(_swParserStatePtr,const gchar *,...);
static void swFatalError(_swParserStatePtr,const gchar *,...);

/*
** ----------------------------------------------------------------------------
*/
/* Initialisation du parser SAX */
static xmlSAXHandler _swHandler =
{
  0,                                     /* internalSubset */
  0,                                     /* isStandalone */
  0,                                     /* hasInternalSubset */
  0,                                     /* hasExternalSubset */
  0,                                     /* resolveEntity */
  0,                                     /* getEntity */
  0,                                     /* entityDecl */
  0,                                     /* notationDecl */
  0,                                     /* attributeDecl */
  0,                                     /* elementDecl */
  0,                                     /* unparsedEntityDecl */
  0,                                     /* setDocumentLocator */
  (startDocumentSAXFunc)swStartDocument, /* startDocument */
  (endDocumentSAXFunc)swEndDocument,     /* endDocument */
  (startElementSAXFunc)swStartElement,   /* startElement */
  (endElementSAXFunc)swEndElement,       /* endElement */
  0,                                     /* reference */
  (charactersSAXFunc)swCharacters,       /* characters */
  0,                                     /* ignorableWhitespace */
  0,                                     /* processingInstruction */
  (commentSAXFunc)swComment,             /* comment */
  (warningSAXFunc)swWarning,             /* warning */
  (errorSAXFunc)swError,                 /* error */
  (fatalErrorSAXFunc)swFatalError,       /* fatalError */
};

/*
** ----------------------------------------------------------------------------
*/
gchar *_StrParserState(gint etat)
{
  if(etat<PARSER_UNKNOWN)
    return((gchar *)_Strstates[etat]);
  return((gchar *)_Strstates[PARSER_UNKNOWN]);
}
/*
** ----------------------------------------------------------------------------
** Début de requète
*/
static void swStartDocument(_swParserStatePtr pstat)
{
#ifdef GXE_DEBUG
  gchar *_procname="swStartDocument";
#endif

  pstat->state        = PARSER_START;
  pstat->prev_state   = PARSER_UNKNOWN;
  pstat->tagerror     = NULL;
  pstat->xmlerror     = NULL;
  pstat->udepth       = 0;
  pstat->errors       = 0;
  pstat->currentcmd   = NULL;
  pstat->currentparam = NULL;
  pstat->stackParam   = NULL;
#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"-- (%s)",_procname);
#endif
}
/*
** ----------------------------------------------------------------------------
** Fin de requète
*/
static void swEndDocument(_swParserStatePtr pstat)
{
#ifdef GXE_DEBUG
  gchar *_procname="swEndDocument";

  _PrintError(LOG_DEBUG,"-- (%s), state=%s errors=%d",_procname,_StrParserState(pstat->state),pstat->errors);
#endif
}
/*
** ----------------------------------------------------------------------------
** Début d'élément (balise)
*/
static void swStartElement(_swParserStatePtr pstat,const gchar *name,const gchar **attr)
{
  gchar *_procname="swStartElement";
  _FxChildPtr _self;

#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"-- (%s), name=%s state=%s",_procname,name,_StrParserState(pstat->state));
#endif
  /* Pointe sur le fils courant */
  _self = (_FxChildPtr)pstat->Child;

  switch(pstat->state)
  {
    case PARSER_START:
    {
      pstat->state = PARSER_CMD;
      if(strcasecmp(name,S_FXML_START) != 0)
      {
	/* Il manque la balise de début <FxML> */
	_PrintError(LOG_NOTICE,"!! (%s), %s attendu au lieu de %s",_procname,S_FXML_START,name);
	/* Pas de break, on passe en analyse de la commande trouvée */
      }
      else
      {
	if(attr)
	  _ParseAttr(&_self->Stream->LAttr,attr);
	break;
      }
    }
    case PARSER_CMD:
    {
      /*
      ** <nomcommande>
      ** ...
      ** </nomcommande>
      */
      _FreeCallCmd(pstat->currentcmd);
      pstat->currentcmd = _NewCallCmd();
      if(pstat->currentcmd == NULL)
      {
	syslog(LOG_CRIT,"!! swStartElement, mémoire insuffisante");
	pstat->errors++;
	pstat->prev_state = pstat->state;
	pstat->state      = PARSER_ERROR;
	pstat->udepth++;
	break;	
      }
      pstat->currentcmd->cmd = _SearchCommand(name);
      if(pstat->currentcmd->cmd == NULL)
      {
	/* Commande inexistante */
	pstat->errors++;
	pstat->tagerror   = g_strdup(name);
	pstat->prev_state = pstat->state;
	pstat->state      = PARSER_CMD_ERROR;
	pstat->udepth++;
	break;
      }
      if(attr)
	_ParseAttr(&pstat->currentcmd->LAttr,attr);
      pstat->state = PARSER_CMD_PARAM;
      break;
    }
    case PARSER_CMD_ERROR:
    {
      /*
      ** Commande courante non trouvée
      */
      pstat->udepth++;
      break;
    }
    case PARSER_CMD_PARAM:
    {
      /*
      ** <nomparam>
      ** ...
      ** </nomparam>
      */
#if 0 
      /*
      ** On autorise les noms de paramètres identiques, afin de prendre
      ** en compte des requetes du genre (exemple de réponse à la demande
      ** de liste des présents d'un dialogue):
      ** <GetList>
      **   <Ligne>
      **     <Pseudo>Robert</Pseudo>
      **     <Dep>75</Dep>
      **     <Resume>h 35 ans sympa</Resume>
      **   </Ligne>
      **   <Ligne>
      **     <Pseudo>Marcel</Pseudo>
      **     <Dep>92</Dep>
      **     <Resume>cool</Resume>
      **   </Ligne>
      **   <Ligne>
      **     <Pseudo>Yvon</Pseudo>
      **     <Dep>78</Dep>
      **     <Resume>h ch f</Resume>
      **   </Ligne>
      **   ...
      ** </GetList>
      */
      if(_SearchParam(name,pstat->currentcmd->LParam) != NULL)
      {
	/* Paramètre déjà présent */
	pstat->errors++;
	pstat->tagerror   = g_strdup(name);
	pstat->prev_state = pstat->state;
	pstat->state      = PARSER_CMD_PARAM_ERROR;
	pstat->udepth++;
	break;
      }
#endif
      if(pstat->currentparam)
      {
	/*
	** Début de sous-balise
	**
	*/
	if(pstat->currentparam->type != T_LIST)
	{
	  /*
	  ** le type du paramètre courant devient T_LIST
	  */
	  g_string_free(pstat->currentparam->v.value,1);
	  pstat->currentparam->type     = T_LIST;
	  pstat->currentparam->v.LParam = NULL;
	}
	/*
	** On empile le paramètre courant
	*/
#ifdef GXE_DEBUG
	_PrintError(LOG_DEBUG,"-- (%s), tag=%s push=%s stack=%p",_procname,name,pstat->currentparam->name,pstat->stackParam);
#endif
	_PushParam(pstat,pstat->currentparam);
      }

      pstat->currentparam = _NewParam();
      if(pstat->currentparam == NULL)
      {
	_PrintError(LOG_CRIT,"!! (%s), erreur dans _NewParam",_procname);
	pstat->errors++;
	pstat->prev_state = pstat->state;
	pstat->state      = PARSER_ERROR;
	pstat->udepth++;
	break;
      }
      pstat->currentparam->name = g_strdup(name);
      if(attr)
	_ParseAttr(&pstat->currentparam->LAttr,attr);
      break;
    }
    case PARSER_CMD_PARAM_ERROR:
    {
      /* Erreur paramètre */
      pstat->udepth++;
      break;
    }
    case PARSER_ERROR:
    {
      /*
      ** Traitement des erreurs
      */
      pstat->udepth++;
      break;
    }
    case PARSER_UNKNOWN:
    {
      pstat->errors++;
      break;
    }
    case PARSER_FINISH:
    {
      break;
    }
  }
}
/*
** ----------------------------------------------------------------------------
** Chaine reçue entre 2 balises
*/
static void swCharacters(_swParserStatePtr pstat,const gchar *chars,gint len)
{
#ifdef GXE_DEBUG
  gchar *_procname="swCharacters";
#endif
  gint i;

#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"-- (%s), len=%d state=%s",_procname,len,_StrParserState(pstat->state));
#endif
  if(pstat->state == PARSER_CMD_PARAM)
  {
    /* Récupération du contenu du paramètre courant */
    if(pstat->currentparam)
    {
      if(pstat->currentparam->type == T_SINGLE)
      {
	for(i=0; i<len; i++)
	{
	  if(chars[i]>=32)
	    g_string_append_c(pstat->currentparam->v.value,chars[i]);
	}
      }
    }
  }
}
/*
** ----------------------------------------------------------------------------
** Fin d'élément (balise)
*/
static void swEndElement(_swParserStatePtr pstat,const gchar *name)
{
  gchar *_procname="swEndElement";
  _FxParamPtr ptr;
  _FxChildPtr _self;

#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"-- (%s), name=%s state=%s",_procname,name,_StrParserState(pstat->state));
#endif
  /* Pointe sur le fils courant */
  _self = (_FxChildPtr)pstat->Child;

  switch(pstat->state)
  {
    case PARSER_START:
    {
      break;
    }
    case PARSER_CMD:
    case PARSER_CMD_PARAM:
    {
      if(strcasecmp(name,S_FXML_START) == 0)
      {
	/* Fin de la requète courante */
#ifdef GXE_DEBUG
	_PrintError(LOG_DEBUG,"---- (%s), Fin de la requète courante",_procname);
#endif
	break;
      }

      if(pstat->currentcmd)
      {
	if(strcmp(name,pstat->currentcmd->cmd->name) == 0)
	{
	  /* Fin de la commande courante */
#ifdef GXE_DEBUG
	  _PrintError(LOG_DEBUG,"---- (%s), Fin de la commande courante",_procname);
#endif

	  /*
	  ** Exécution de la commande
	  ** ------------------------
	  */
	  if(_ExecuteCommand(pstat->currentcmd,_self) != FX_RETURN_SUCCESS)
	    _PrintError(LOG_NOTICE,"!!!! (%s), erreur(s) à l'exécution de %s",_procname,name);

	  /* Commande suivante ... */
	  _FreeCallCmd(pstat->currentcmd);
	  pstat->currentcmd     = NULL;
	  pstat->stackParam     = NULL;
	  _self->Stream->state  = CHLD_PARSE_REQUEST;
	  pstat->state          = PARSER_CMD;
	  break;
	}
      }

      if(pstat->currentparam)
      {
	if(strcmp(name,pstat->currentparam->name) == 0)
	{
	  /* Fin du paramètre courant */
#ifdef GXE_DEBUG
	  _PrintError(LOG_DEBUG,"---- (%s), Fin du paramètre courant",_procname);
	  _PrintError(LOG_DEBUG,"---- (%s), nom=%s",_procname,pstat->currentparam->name);
#endif
	  if(pstat->stackParam)
	  {
	    /* Paramètre imbriqué */
	    ptr                 = pstat->currentparam;
	    pstat->currentparam = _PopParam(pstat);
#ifdef GXE_DEBUG
	    _PrintError(LOG_DEBUG,"-- (%s), tag=%s pop precedent=%s stack=%p",_procname,name,pstat->currentparam->name,pstat->stackParam);
#endif
	    /* Ajout du paramètre au paramètre précédent */
	    pstat->currentparam->v.LParam = g_slist_append(pstat->currentparam->v.LParam,ptr);
	  }
	  else
	  {
	    /* Ajout du paramètre à la commande courante */
	    pstat->currentcmd->LParam = g_slist_append(pstat->currentcmd->LParam,pstat->currentparam);
	    pstat->currentparam = NULL;
	  }
	  break;
	}
      }

      /* Balise de fin non concordante */
      _PrintError(LOG_NOTICE,"!! (%s), balise %s non concordante",_procname,name);
      break;
    }
    case PARSER_CMD_ERROR:
    {
      if(strcmp(name,pstat->tagerror) == 0)
      {
	/*
	** Balise de fin de la commande en erreur
	*/
	pstat->udepth--;
	_PrintError(LOG_NOTICE,"!! (%s), commande %s non trouvée",_procname,name);
	_AppendxmlError(pstat,XML_ERR_CMD_NOT_FOUND);
	/* On peut traiter l'éventuelle commande suivante ... */
	_FreeCallCmd(pstat->currentcmd);
	pstat->currentcmd = NULL;
	g_free(pstat->tagerror);
	pstat->tagerror       = NULL;
	_self->Stream->state  = CHLD_PARSE_REQUEST;
	pstat->state          = PARSER_CMD;
	if(pstat->stackParam)
	  g_slist_free(pstat->stackParam);
	pstat->stackParam     = NULL;
	break;
      }
      pstat->udepth--;
      if(pstat->udepth == 0)
	pstat->state = pstat->prev_state;
      break;
    }
    case PARSER_CMD_PARAM_ERROR:
    {
      if(strcmp(name,pstat->tagerror) == 0)
      {
	/*
	** Balise de fin du paramètre en erreur
	*/
	pstat->udepth--;
	_PrintError(LOG_NOTICE,"!! (%s), paramètre %s déjà présent",_procname,name);
	_AppendxmlError(pstat,XML_ERR_PARAM_FOUND);
	/* On peut traiter les éventuels paramètres suivants ... */
	g_free(pstat->tagerror);
	pstat->tagerror       = NULL;
	_self->Stream->state  = CHLD_PARSE_REQUEST;
	pstat->state          = PARSER_CMD_PARAM;
	break;
      }
      pstat->udepth--;
      if(pstat->udepth == 0)
	pstat->state = pstat->prev_state;
      break;
    }
    case PARSER_ERROR:
    {
      /*
      ** Traitement des autres erreurs
      */
      pstat->udepth--;
      if(pstat->udepth == 0)
	pstat->state = pstat->prev_state;
      break;
    }
    case PARSER_UNKNOWN:
    {
      /* Erreur(s) rencontrées */
      pstat->udepth--;
      if(pstat->udepth == 0)
	pstat->state = pstat->prev_state;
      break;
    }
    case PARSER_FINISH:
    {
      break;
    }
  }
}
/*
** ----------------------------------------------------------------------------
** Commentaire xml
*/
static void swComment(_swParserStatePtr pstat,const gchar *msg)
{
#ifdef GXE_DEBUG
  gchar *_procname="swComment";

  _PrintError(LOG_DEBUG,"-- (%s), %s state=%s",_procname,msg,_StrParserState(pstat->state));
#endif
}
/*
** ----------------------------------------------------------------------------
** Warning
*/
static void swWarning(_swParserStatePtr pstat,const gchar *msg,...)
{
  gchar *_procname="swWarning";
  va_list args;
  gchar *buff=NULL,*pcr;
  gint nb,szbuff=64;

  /*
  ** Le buffer de sortie est dimensionné dynamiquement
  ** en fonction du nombre de paramètres
  */
  va_start(args,msg);
  while(1)
  {
    buff = g_realloc(buff,szbuff);
    if(buff == NULL)
    {
      syslog(LOG_CRIT,"!! swWarning, mémoire insuffisante");
      break;
    }
    nb = vsnprintf(buff,szbuff,msg,args);
    /* A-t'on tout écrit ? */
    if((nb>=0) && (nb<szbuff))
      break;
    /* Non, on augmente la taille du buffer */
    szbuff += 64;
  }
  if(buff)
  {
    /* On enlève le '\n' */
    pcr = strrchr(buff,'\n');
    if(pcr)
      *pcr = '\0';
    _PrintError(LOG_DEBUG,"!! (%s), %s state=%s",_procname,buff,_StrParserState(pstat->state));
  }
  va_end(args);
  g_free(buff);
}
/*
** ----------------------------------------------------------------------------
** Erreur(s)
*/
static void swError(_swParserStatePtr pstat,const gchar *msg,...)
{
  gchar *_procname="swError";
  va_list args;
  gchar *buff=NULL,*pcr;
  gint nb,szbuff=64;

  /*
  ** Le buffer de sortie est dimensionné dynamiquement
  ** en fonction du nombre de paramètres
  */
  va_start(args,msg);
  while(1)
  {
    buff = g_realloc(buff,szbuff);
    if(buff == NULL)
    {
      syslog(LOG_CRIT,"!! swError, mémoire insuffisante");
      break;
    }
    nb = vsnprintf(buff,szbuff,msg,args);
    /* A-t'on tout écrit ? */
    if((nb>=0) && (nb<szbuff))
      break;
    /* Non, on augmente la taille du buffer */
    szbuff += 64;
  }
  if(buff)
  {
    /* On enlève le '\n' */
    pcr = strrchr(buff,'\n');
    if(pcr)
      *pcr = '\0';
    _PrintError(LOG_DEBUG,"!! (%s), %s (state=%s)",_procname,buff,_StrParserState(pstat->state));
    pstat->xmlerror = g_strdup(buff);
  }
  va_end(args);
  g_free(buff);
  pstat->errors++;
}
/*
** ----------------------------------------------------------------------------
** Erreur(s) Fatales
*/
static void swFatalError(_swParserStatePtr pstat,const gchar *msg,...)
{
  gchar *_procname="swFatalError";
  va_list args;
  gchar *buff=NULL,*pcr;
  gint nb,szbuff=64;

  /*
  ** Le buffer de sortie est dimensionné dynamiquement
  ** en fonction du nombre de paramètres
  */
  va_start(args,msg);
  while(1)
  {
    buff = g_realloc(buff,szbuff);
    if(buff == NULL)
    {
      syslog(LOG_CRIT,"!! swFatalError, mémoire insuffisante");
      break;
    }
    nb = vsnprintf(buff,szbuff,msg,args);
    /* A-t'on tout écrit ? */
    if((nb>=0) && (nb<szbuff))
      break;
    /* Non, on augmente la taille du buffer */
    szbuff += 64;
  }
  if(buff)
  {
    /* On enlève le '\n' */
    pcr = strrchr(buff,'\n');
    if(pcr)
      *pcr = '\0';
    _PrintError(LOG_DEBUG,"!! (%s), %s (state=%s)",_procname,buff,_StrParserState(pstat->state));
    pstat->xmlerror = g_strdup(buff);
  }
  va_end(args);
  g_free(buff);
  pstat->errors++;
}
/*
** ----------------------------------------------------------------------------
** Analyse la requète du fils donné
*/
int _ParseRequete(_FxChildPtr _self)
{
  gchar *_procname="_ParseRequete";
  gint ret;

  _self->Stream->state = CHLD_PARSE_REQUEST;
  /* Analyse du code xml */
  xmlKeepBlanksDefault(0);

  ret = xmlSAXUserParseMemory(&_swHandler,_self->Parser,_self->Stream->Requete->str,strlen(_self->Stream->Requete->str));
  if(ret != FX_RETURN_SUCCESS)
  {
    /* Erreur interne du parser */
    _PrintError(LOG_NOTICE,"(%s), erreur %d",_procname,ret);
    _self->Parser->xmlerrno = ret;
    _AppendxmlError(_self->Parser,XML_ERR_INTERNAL);
    g_free(_self->Parser->xmlerror);
  }
  /* Envoi du résultat */
  _PrintxmlResult(_self->Stream);

  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
