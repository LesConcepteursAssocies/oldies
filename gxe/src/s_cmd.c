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
** Traitement des commandes
*/
#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
** Recherche d'une commande dans la table � partir de son nom
** Renvoie un pointeur sur les informations de la commande, NULL sinon
*/
_FxCmdPtr _SearchCmdT(const gchar *name)
{
  _FxCmdPtr ptr;
  GSList *lst=_TFxCmds;

  while(lst)
  {
    ptr = lst->data;
    if(strcmp(ptr->name,name) == 0)
      return (ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Recherche d'une commande dans les modules pr�sents � partir de son nom
** Renvoie un pointeur sur les informations du module, NULL sinon
*/
_ModulePtr _SearchCmdM(const gchar *name)
{
  GSList *lst=_TModules;
  _ModulePtr mdl;
  gpointer psym;

  while(lst)
  {
    mdl = lst->data;
    if(g_module_symbol(mdl->pmodule,name,&psym))
      return(mdl);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Recherche d'une commande � partir de son nom.
** Renvoie un pointeur sur les informations de la commande, NULL sinon
*/
_FxCmdPtr _SearchCommand(const gchar *name)
{
  gchar *_procname="_SearchCommand";
  _FxCmdPtr ptr;
  _ModulePtr mdl;

  ptr = _SearchCmdT(name);
  if(ptr != NULL)
  {
#ifdef GXE_DEBUG
    _PrintError(LOG_DEBUG,"(%s), commande %s trouv�e dans la table",_procname,name);
#endif
    return (ptr);
  }
  mdl = _SearchCmdM(name);
  if(mdl != NULL)
  {
#ifdef GXE_DEBUG
    _PrintError(LOG_DEBUG,"(%s), commande %s trouv�e dans le module %s",_procname,name,mdl->name);
#endif
    /* On met la commande dans la table pour optimiser les prochaines recherches */
   if(FxRegisterCmd(name,mdl->pmodule)<0)
     _PrintError(LOG_NOTICE,"-- (%s), erreur � l'enrigstrement de la commande %s",_procname,name);
   return (_SearchCmdT(name));
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Allocation d'un contexte de commande
** Renvoie le pointeur sur la structure allou�e, NULL sinon.
*/
_CallCmdPtr _NewCallCmd(void)
{
  return ((_CallCmdPtr)g_malloc0(sizeof(struct _CallCmd)));
}
/*
** ----------------------------------------------------------------------------
** Lib�ration des ressources occup�es par le contexte de la  commande donn�e
*/
void _FreeCallCmd(_CallCmdPtr ptr)
{
  GSList *lst;

  if(ptr)
  {
    _FreeLAttr(ptr->LAttr);
    /* Lib�ration des param�tres */
    lst = ptr->LParam;
    while(lst)
    {
      _FreeParam(lst->data);
      lst = g_slist_next(lst);
    }
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
** Allocation d'un param�tre
** Renvoie le pointeur sur la structure allou�e, NULL sinon.
*/
_FxParamPtr _NewParam(void)
{
  _FxParamPtr ptr;

  ptr = (_FxParamPtr)g_malloc0(sizeof(struct _FxParam));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! _NewParam, m�moire insuffisante");
    return (NULL);
  }
  ptr->type    = T_SINGLE;
  ptr->v.value = g_string_new("");
  return (ptr);
}
/*
** ----------------------------------------------------------------------------
** Lib�ration des ressources occup�es par le param�te donn�
*/
void _FreeParam(_FxParamPtr ptr)
{
  GSList *lst;

  if(ptr)
  {
    g_free(ptr->name);
    _FreeLAttr(ptr->LAttr);
    switch(ptr->type)
    {
      case T_SINGLE:
      {
	g_string_free(ptr->v.value,1);
	break;
      }
      case T_LIST:
      {
	lst = ptr->v.LParam;
	while(lst)
	{
	  _FreeParam(lst->data);
	  lst = g_slist_next(lst);
	}
	g_slist_free(lst);
	break;
      }
      case T_UNDEF:
	break;
    }
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
** Recherche d'un param�tre dans la table donn�e � partir de son nom.
** Renvoie un pointeur sur le param�tre, NULL sinon
*/
_FxParamPtr _SearchParam(const gchar *name,GSList *lst)
{
  _FxParamPtr ptr;

  while(lst)
  {
    ptr = lst->data;
    if(strcmp(ptr->name,name) == 0)
      return (ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Empile le param�tre donn� dans la pile du parser donn�.
*/
void _PushParam(_swParserStatePtr pstat,_FxParamPtr ptr)
{
  pstat->stackParam = g_slist_prepend(pstat->stackParam,ptr);
}
/*
** ----------------------------------------------------------------------------
** D�pile le param�tre situ� au sommet de la pile du parser donn�.
*/
_FxParamPtr _PopParam(_swParserStatePtr pstat)
{
  _FxParamPtr ptr;

  if(pstat->stackParam)
  {
    ptr               = pstat->stackParam->data;
    pstat->stackParam = g_slist_remove(pstat->stackParam,ptr);
    return (ptr);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Compte le nombre de param�tres du param�tre donn�
*/
gint _CountParam(_FxParamPtr ptr)
{
  GSList *lst;
  gint n=0;

  if(ptr)
  {
    switch(ptr->type)
    {
      case T_SINGLE:
      {
	return (1);
	break;
      }
      case T_LIST:
      {
	n++;
	lst = ptr->v.LParam;
	while(lst)
	{
	  n  += _CountParam(lst->data);
	  lst = g_slist_next(lst);
	}
	return (n);
	break;
      }
      case T_UNDEF:
	break;
    }
  }
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Lib�ration des ressources occup�es par la table des commandes.
*/
void _FreeTCmds(void)
{
  _FxCmdPtr ptr;

  pthread_mutex_lock(mutex_TCmd);
  while(_TFxCmds)
  {
    ptr = _TFxCmds->data;
    g_free(ptr->name);
    g_free(ptr);
    _TFxCmds = g_slist_next(_TFxCmds);
  }
  g_slist_free(_TFxCmds);
  _TFxCmds = NULL;
  pthread_mutex_unlock(mutex_TCmd);
}
/*
** ----------------------------------------------------------------------------
*/
