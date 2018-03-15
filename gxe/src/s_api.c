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
** API des commandes utilisables par les modules
*/

#include "common.h"
#include "s_globals.h"

const gchar *_StrFxError[] =
{
  "Parametre manquant",
  "Memoire insuffisante",
  "Session non definie",
  "Session existante",
  "Erreur interne"
};

/*
** ----------------------------------------------------------------------------
** Ajoute la commande donnée dans la table des commandes
** Renvoie 0 si ok, -1 sinon
*/
gint FxRegisterCmd(const gchar *name,GModule *module)
{
  gchar *_procname="FxRegisterCmd";
  _FxCmdPtr ptr;

  if(_SearchCmdT(name) != NULL)
  {
    _PrintError(LOG_NOTICE,"(%s), commande %s déjà présente",_procname,name);    
    return (-1);
  }
  ptr = (_FxCmdPtr)g_malloc0(sizeof(struct _FxCmd));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! FxRegisterCmd, mémoire insuffisante");
    return (-1);
  }
  ptr->name    = g_strdup(name);
  ptr->pmodule = module;

  pthread_mutex_lock(mutex_TCmd);
  _TFxCmds = g_slist_append(_TFxCmds,ptr);
  pthread_mutex_unlock(mutex_TCmd);

  return (0);
}
/*
** ----------------------------------------------------------------------------
** Renvoi le nombre de paramètres de la commande donnée
*/
gint FxGetMaxParam(FxCommandPtr cmd)
{
  GSList *lst;
  gint n=0;
  
  lst = cmd->LParam;
  while(lst)
  {
    n  += _CountParam(lst->data);
    lst = g_slist_next(lst);
  }
  return (n);
}
/*
** ----------------------------------------------------------------------------
** Renvoi un pointeur sur le nième paramètre (n partant de 0)
** de la commande donnée.
*/
FxParameterPtr FxGetNthParam(FxCommandPtr cmd,guint n)
{
  return((FxParameterPtr)g_slist_nth_data(cmd->LParam,n));
}
/*
** ----------------------------------------------------------------------------
** Renvoi un pointeur sur le paramètre de nom donné
** de la commande donnée.
*/
FxParameterPtr FxGetParamByName(FxCommandPtr cmd,gchar *name)
{
  GSList *lst;
  _FxParamPtr ptr;

  lst = cmd->LParam;
  while(lst)
  {
    ptr = lst->data;
    if(strcasecmp(ptr->name,name) == 0)
      return(ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoi le nom du paramète donné
*/
gchar *FxGetParamName(FxParameterPtr param)
{
  return(param->name);
}
/*
** ----------------------------------------------------------------------------
** Renvoi le type du paramète donné
*/
FxParamType FxGetParamType(FxParameterPtr param)
{
  return(param->type);
}
/*
** ----------------------------------------------------------------------------
** Renvoi la valeur du paramète donné sous forme de chaine
*/
gchar *FxGetParamValue(FxParameterPtr param)
{
  if(param->type == T_SINGLE)
    return(param->v.value->str);
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoi la valeur (sous forme de chaine) du paramètre dont le nom est donné.
*/
gchar *FxGetParamValueByName(FxCommandPtr cmd,gchar *name)
{
  GSList *lst;
  _FxParamPtr ptr;

  lst = cmd->LParam;
  while(lst)
  {
    ptr = lst->data;
    if(strcasecmp(ptr->name,name) == 0)
    {
      if(ptr->type == T_SINGLE)
	return(ptr->v.value->str);
      return (NULL);
    }
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoie le nom de la commande courante
*/
gchar *FxGetCommandName(FxCommandPtr Cmd)
{
  return (Cmd->cmd->name);
}
/*
** ----------------------------------------------------------------------------
** Renvoi la valeur (sous forme de chaine) de l'attribut de la balise racine
** dont le nom est donné
*/
gchar *FxGetRequestAttrValue(gchar *name)
{
  gchar *_procname="FxGetRequestAttrValue";
  _FxChildPtr _self;
  _FxAttrPtr ptr;

  /* Pointe sur le fils courant */
  _self = _SearchChild(pthread_self());
  if(_self == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans _SearchChild",_procname);
    return (NULL);
  }
  ptr = _SearchAttr(_self->Stream->LAttr,name);
  if(ptr)
    return(ptr->value);
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoi la valeur (sous forme de chaine) de l'attribut de la commande donnée
** dont le nom est donné
*/
gchar *FxGetCommandAttrValue(FxCommandPtr Cmd,gchar *name)
{
  _FxAttrPtr ptr;

  ptr = _SearchAttr(Cmd->LAttr,name);
  if(ptr)
    return(ptr->value);
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Renvoi la valeur (sous forme de chaine) de l'attribut du paramètre donné
** dont le nom est donné
*/
gchar *FxGetParamAttrValue(FxParameterPtr param,gchar *name)
{
  _FxAttrPtr ptr;

  ptr = _SearchAttr(param->LAttr,name);
  if(ptr)
    return(ptr->value);
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Envoi d'un message d'information
*/
void FxSyslog(const gchar *msg,...)
{
  va_list args;
  gchar *buff=NULL;
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
      syslog(LOG_CRIT,"!! FxSyslog, mémoire insuffisante");
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
    syslog(LOG_INFO,buff);
  va_end(args);
  g_free(buff);
}
/*
** ----------------------------------------------------------------------------
** Création d'un niveau d'indentation xml nommé "name" pour la commande donnée.
** Renvoi 0 si ok.
**
** level indique à quel niveau de l'arbre placer le nouveau niveau.
*/
gint FxCreatexmlLevel(FxCommandPtr cmd,gint level,const gchar *name)
{
  gchar *_procname="FxCreatexmlLevel";
  xmlNodePtr tree,subtree;

  switch(level)
  {
    case FX_FIRSTLEVEL:
    {
      tree = cmd->FirstLevel;
      break;
    }
    case FX_CURRENTLEVEL:
    {
      tree = cmd->CurrentLevel;
      break;
    }
    default:
    {
      tree = cmd->FirstLevel;
      break;
    }
  }
  subtree = xmlNewChild(tree,NULL,(const xmlChar *)name,NULL);
  if(subtree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild",_procname);
    return (-1);
  }
  cmd->CurrentLevel = subtree;
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Initialisation du niveau courant d'indentation xml à partir du niveau donné
** Renvoie 0 si ok.
*/
gint FxSetxmlLevel(FxCommandPtr cmd,gint level)
{
  if(cmd->CurrentLevel != cmd->FirstLevel)
  {
    /* On n'est pas tout en haut de l'arbre, on peut donc remonter... */
    switch(level)
    {
      case FX_FIRSTLEVEL:
      {
	/* On remonte tout en haut */
	cmd->CurrentLevel = cmd->FirstLevel;
	break;
      }
      case FX_PREVIOUSLEVEL:
      {
	/* On remonte d'un cran par rapport au courant */
	cmd->CurrentLevel = cmd->CurrentLevel->parent;
	break;
      }
      case FX_CURRENTLEVEL:
      default:
      {
	/* On reste sur place ... */
	break;
      }
    }
  }
  return(0);
}
/*
** ----------------------------------------------------------------------------
** Ajoute le message donné au niveau courant de l'abre xml de la commande donnée.
** Renvoie 0 si ok.
*/
gint FxAppendResult(FxCommandPtr cmd,const gchar *name,const gchar *msg,...)
{
  gchar *_procname="FxAppendResult";
  va_list args;
  gchar *buff=NULL;
  gint nb,szbuff=64;
  xmlNodePtr result;

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
      syslog(LOG_CRIT,"!! FxAppendResult, mémoire insuffisante");
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
    result = xmlNewChild(cmd->CurrentLevel,NULL,(const xmlChar *)name,(const xmlChar *)buff);
    if(result == NULL)
    {
      _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild",_procname);
      va_end(args);
      g_free(buff);
      return (-1);
    }
  }
  va_end(args);
  g_free(buff);
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Envoi d'une indication d'erreur pour la commande donnée
*/
gint FxPrintError(FxCommandPtr cmd,gint code,const gchar *msg,...)
{
  gchar *_procname="FxPrintError";
  _FxChildPtr _self;
  va_list args;
  gchar *buff=NULL;
  gchar *btmp;
  gint nb,szbuff=64,errnum;
  xmlNodePtr Error;
  xmlNodePtr tree;

  /* Pointe sur le fils courant */
  _self = _SearchChild(pthread_self());
  if(_self == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans _SearchChild",_procname);
    return (-1);
  }

  errnum = code-100;
  /* Fabrication du buffer de sortie */
  Error = xmlNewChild(_self->Stream->Out->children,NULL,"error",NULL);
  if(Error == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(Error)",_procname);
    return (-1);
  }
  xmlSetProp(Error,"cmd",cmd->cmd->name);

  btmp = g_strdup_printf("%d",code);
  tree = xmlNewChild(Error,NULL,"code",(const xmlChar *)btmp);
  g_free(btmp);
  if(tree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(code)",_procname);
    return (-1);
  }

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
      syslog(LOG_CRIT,"!! FxAppendResult, mémoire insuffisante");
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
    btmp = g_strdup_printf("%s: %s",_StrFxError[errnum],buff);
  else
    btmp = g_strdup_printf("%s",_StrFxError[errnum]);

  tree = xmlNewChild(Error,NULL,"msg",(const xmlChar *)btmp);
  g_free(btmp);
  if(tree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(msg)",_procname);
    return (-1);
  }

  return(0);
}
/*
** ----------------------------------------------------------------------------
*/
