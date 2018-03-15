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
** Traitement des modules dynamiques
*/
#include "common.h"
#include "s_globals.h"


/*
** ----------------------------------------------------------------------------
** Utilisé pour la sélection des ".so" dans Init_TModules
*/
static int selectmodule(const struct dirent *d)
{
  gchar *name,*pext;

  name = (gchar *)d->d_name;
  /* On cherche le dernier '.' */
  pext = strrchr(name,'.');
  /*
  ** On ne garde que les 'fichierxxx.so'
  ** (pas les '.soxxx' ou les 'xxx.soxxx')
  */
  if(pext && (pext!=name) && (strlen(pext)==strlen(S_MODULE_EXT)))
  {
    return 1;
  }
  return 0;
}
/*
** ----------------------------------------------------------------------------
** Libération des ressources mémoires occupées par la table des modules
*/
void _FreeTModules(void)
{
  _ModulePtr pmod;

  pthread_mutex_lock(mutex_TModules);
  while(_TModules)
  {
    pmod = _TModules->data;
    g_free(pmod->name);
    if(pmod->pmodule)
      g_module_close(pmod->pmodule);
    g_free(pmod);
    _TModules = g_slist_next(_TModules);
  }
  g_slist_free(_TModules);
  _TModules = NULL;
  pthread_mutex_unlock(mutex_TModules);
}
/*
** ----------------------------------------------------------------------------
** Ouverture des modules trouvés
*/
void _OpenModules(void)
{
  gchar *_procname="_OpenModules";
  GSList *lst;
  _ModulePtr mdl;

  pthread_mutex_lock(mutex_TModules);
  lst = _TModules;
  while(lst)
  {
    mdl = lst->data;
    mdl->pmodule = g_module_open(g_module_build_path(_ModulesPath,mdl->name),G_MODULE_BIND_LAZY);
    if(mdl->pmodule == NULL)
      _PrintError(LOG_NOTICE,"-- (%s), erreur sur g_module_open(%s) (%d %s)",_procname,g_module_build_path(_ModulesPath,mdl->name),errno,strerror(errno));
    lst = g_slist_next(lst);
  }
  pthread_mutex_unlock(mutex_TModules);
}
/*
** ----------------------------------------------------------------------------
** Fermeture des modules trouvés
*/
void _CloseModules(void)
{
  gchar *_procname="_CloseModules";
  GSList *lst;
  _ModulePtr mdl;

  pthread_mutex_lock(mutex_TModules);
  lst = _TModules;
  while(lst)
  {
    mdl = lst->data;
    if(mdl->pmodule != NULL)
    {
      if(! g_module_close(mdl->pmodule))
	_PrintError(LOG_NOTICE,"-- (%s), erreur sur g_module_close(%s) (%d %s)",_procname,g_module_build_path(_ModulesPath,mdl->name),errno,strerror(errno));
      else
	mdl->pmodule = NULL;
    }
    lst = g_slist_next(lst);
  }
  pthread_mutex_unlock(mutex_TModules);
}
/*
** ----------------------------------------------------------------------------
** Cherche dans le répertoire donné tous les fichiers ".so" et les stocke
** dans _TModules.
** raz = 1 pour vider _TModules avant la recherche, 0 sinon
** Renvoie le nombre de modules de la table.
*/
int _InitTModules(gchar *rep,gint raz)
{
  gchar *_procname="_InitTModules";
  int i,max;
  struct dirent **ldir;
  struct stat sts;
  _ModulePtr pmod;
  gchar *filename;

  if(raz)
  {
    _FreeTModules();
    _FreeTCmds();
  }

#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"(%s), recherche des modules dans [%s]...",_procname,rep);
#endif
  max = scandir(rep,&ldir,selectmodule,alphasort);
  if(max<0)
    return (-1);
  i = 0;
  while(i!=max)
  {
    filename = g_strconcat(_ModulesPath,"/",ldir[i]->d_name,NULL);
    if(stat(filename,&sts) != 0)
    {
      _PrintError(LOG_NOTICE,"-- (%s), erreur sur stat(%s) (%d %s)",_procname,filename,errno,strerror(errno));
      break;
    }
#ifdef GXE_DEBUG
    _PrintError(LOG_DEBUG,"-- (%s), ajout de %s",_procname,filename);
#endif
    g_free(filename);
    /* Ajout du module dans la table */
    pmod = (_ModulePtr) g_malloc0(sizeof(struct _Module));
    if(pmod == NULL)
    {
      _PrintError(LOG_CRIT,"-- (%s), erreur sur g_malloc0(%d)",_procname,sizeof(struct _Module));
      break;
    }
    pmod->name  = g_strdup(ldir[i]->d_name);
    pmod->mtime = sts.st_mtime;

    pthread_mutex_lock(mutex_TModules);
    _TModules = g_slist_append(_TModules,pmod);
    pthread_mutex_unlock(mutex_TModules);

    i++;
  }
  return ((int)g_slist_length(_TModules));
}
/*
** ----------------------------------------------------------------------------
** Initialisation des modules dynamiques
*/
int _InitModules(void)
{
  gchar *_procname="_InitModules";

  if(!g_module_supported())
    return (-1);

  _ModulesPath = g_strconcat(_CurrentDir,S_MODULE_PATH,NULL);
  /*
  ** On cherche tous les ".so" dans le répertoire
  */
  _PrintError(LOG_DEBUG,"(%s), %d module(s) trouvé(s)",_procname,_InitTModules(_ModulesPath,0));

  _OpenModules();
  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
