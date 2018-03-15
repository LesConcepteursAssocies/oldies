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
** Gestion des process fils
*/

#include "common.h"
#include "s_globals.h"

static void *_FxChildMain(void *);
static void _FxEndChild(void *);


/*
** ----------------------------------------------------------------------------
** Enlève le fils donné de la table, et libère ses ressources
*/
void _RemoveChild(pthread_t s_id)
{
  _FxChildPtr ptr;
  GSList *lst=_TChilds;

  while(lst)
  {
    ptr = lst->data;
    if(pthread_equal(s_id,ptr->thr_id))
    {
      pthread_mutex_lock(mutex_TChilds);
      _TChilds = g_slist_remove(_TChilds,ptr);
      pthread_mutex_unlock(mutex_TChilds);
      _FreeChild(ptr);
      break;
    }
    lst = g_slist_next(lst);
  }
}
/*
** ----------------------------------------------------------------------------
** Recherche d'un fils dans la table en fonction de l'ID donné
** Renvoie un pointeur sur le fils si trouvé.
*/
_FxChildPtr _SearchChild(pthread_t s_id)
{
  _FxChildPtr ptr;
  GSList *lst=_TChilds;

  while(lst)
  {
    ptr = lst->data;
    if(pthread_equal(s_id,ptr->thr_id))
      return (ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Recherche d'un fils dans la table dont le canal est dans l'état donné.
** Renvoie un pointeur sur le fils si trouvé.
*/
_FxChildPtr _SearchChildStream(_FxChildState etat)
{
  _FxChildPtr ptr;
  GSList *lst=_TChilds;

  while(lst)
  {
    ptr = lst->data;
    if(ptr->Stream->state == etat)
      return (ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
** Libération des ressources occupées par le fils donné
*/
void _FreeChild(_FxChildPtr ptr)
{
  if(ptr)
  {
    if(ptr->Stream)
      _FreeFxStream(ptr->Stream,1);
    if(ptr->Parser)
      g_free(ptr->Parser);
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
** Libération de la table des fils
*/
void _FreeTChilds(void)
{
  _FxChildPtr ptr;

  pthread_mutex_lock(mutex_TChilds);
  while(_TChilds)
  {
    ptr = _TChilds->data;
    _FreeChild(ptr);
    _TChilds = g_slist_next(_TChilds);
  }
  g_slist_free(_TChilds);
  _TChilds = NULL;
  pthread_mutex_unlock(mutex_TChilds);
}
/*
** ----------------------------------------------------------------------------
** Création d'un process fils qui va traiter le flux donné.
** Renvoi 0 si ok.
*/
gint _FxCreateNewChild(gint sock)
{
  gchar *_procname="_FxCreateNewchild";
  _FxChildPtr ptr;
  gint ret;

  ptr = (_FxChildPtr)g_malloc0(sizeof(struct _FxChild));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! _FxCreateNewChild, mémoire insuffisante");
    return (-1);
  }
  ptr->Stream = _NewFxStream(sock);
  if(ptr->Stream == NULL)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur _NewFxStream",_procname);
    _FreeChild(ptr);
    return (-1);
  }
  ptr->Parser = (_swParserStatePtr)g_malloc0(sizeof(struct _swParserState));
  if(ptr->Parser == NULL)
  {
    syslog(LOG_CRIT,"!! _FxCreateNewChild, mémoire insuffisante (Parser)");
    _FreeChild(ptr);
    return (-1);
  }
  ptr->Parser->Child = ptr;

  /* Ajout du fils dans la table */
  pthread_mutex_lock(mutex_TChilds);
  _TChilds = g_slist_append(_TChilds,ptr);
  pthread_mutex_unlock(mutex_TChilds);

  /* Lancement du thread */
  ret = pthread_create(&ptr->thr_id,NULL,_FxChildMain,ptr);
  if(ret != 0)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur pthread_create (%d %s)",_procname,ret,strerror(ret));
    _FreeChild(ptr);
    return (-1);
  }

  ret = pthread_detach(ptr->thr_id);
  if(ret != 0)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur pthread_detach (%d %s)",_procname,ret,strerror(ret));
  }

  return (0);
}
/*
** ----------------------------------------------------------------------------
** Début d'exécution du fils
*/
static void *_FxChildMain(void *pointeur)
{
  gchar *_procname="_FxChildMain";
  sigset_t sigmask;
  int ret;
  _FxChildPtr _self=(_FxChildPtr)pointeur;

  _PrintError(LOG_DEBUG,"Début du fils (id=%d)",_self->thr_id);
  /*
  ** On bloque tous les signaux.des threads, de façon à ce
  ** que seul le père les recoivent.
  */
  sigfillset(&sigmask);
  ret = pthread_sigmask(SIG_BLOCK,&sigmask,NULL);
  if(ret != 0)
  {
    _PrintError(LOG_CRIT,"-- (%s), erreur sur pthread_sigmask (%d %s). Sortie",_procname,ret,strerror(ret));
    _RemoveChild(_self->thr_id);
    pthread_exit(NULL);
  }

  pthread_cleanup_push(_FxEndChild,pointeur);
  _ProcessCall(_self);
  pthread_cleanup_pop(1);
  pthread_exit(NULL);
}
/*
** ----------------------------------------------------------------------------
** Fin d'un fils
*/
static void _FxEndChild(void *pointeur)
{
  _FxChildPtr _self=(_FxChildPtr)pointeur;

  _PrintError(LOG_DEBUG,"Fin du fils (id=%d)",_self->thr_id);
  _RemoveChild(_self->thr_id);
}
/*
** ----------------------------------------------------------------------------
*/
