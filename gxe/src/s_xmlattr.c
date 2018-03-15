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
** Gestion des attributs (ou propriétés) des balises XML
*/
#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
** Allocation d'un attribut avec la informations données
** Renvoie le pointeur sur la structure allouée, NULL sinon.
*/
_FxAttrPtr _NewAttr(const gchar *nom,const gchar *val)
{
  _FxAttrPtr ptr;

  ptr = (_FxAttrPtr)g_malloc0(sizeof(struct _FxAttr));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! _NewAttr, mémoire insuffisante");
    return (NULL);
  }
  if(nom)
    ptr->name  = g_strdup(nom);
  if(val)
    ptr->value = g_strdup(val);
  return (ptr);
}
/*
** ----------------------------------------------------------------------------
** Libération de l'attribut donné
*/
void _FreeAttr(_FxAttrPtr ptr)
{
  if(ptr)
  {
    g_free(ptr->name);
    g_free(ptr->value);
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
** Libération de la liste des attributs donnée
*/
void _FreeLAttr(GSList *lst)
{
  _FxAttrPtr ptr;

  while(lst)
  {
    ptr = lst->data;
    _FreeAttr(ptr);
    lst = g_slist_next(lst);
  }
  g_slist_free(lst);
}
/*
** ----------------------------------------------------------------------------
** Parse le tableau d'attributs donné, et les place dans la liste donnée
*/
void _ParseAttr(GSList **plst,const gchar **attr)
{
  gchar *_procname="_ParseAttr";
  GSList *lst=*plst;
  _FxAttrPtr ptr;
  gint i=0;

  /*
  ** Organisation du tableau attr:
  ** attr[0]=nom attr[1]=valeur
  ** attr[2]=nom attr[3]=valeur
  ** ...
  */
  while(attr[i])
  {
    if((i%2)==0)
    {
      /* on est sur le nom de l'attribut courant */
      ptr = _NewAttr(attr[i],attr[i+1]);
      if(ptr == NULL)
      {
	_PrintError(LOG_CRIT,"!! (%s), erreur dans _NewAttr",_procname);
	break;
      }
#ifdef GXE_DEBUG
      _PrintError(LOG_DEBUG,"-- (%s), name=%s value=%s",_procname,ptr->name,ptr->value);
#endif
      lst = g_slist_append(lst,ptr);
    }
    i++;
  }
  *plst = lst;
}
/*
** ----------------------------------------------------------------------------
** Cherche l'attribut dont le nom est donné dans la liste donnée.
** Renvoi le pointeur sur l'attribut si trouvé.
*/
_FxAttrPtr _SearchAttr(GSList *lst,gchar *name)
{
  _FxAttrPtr ptr;

  while(lst)
  {
    ptr = lst->data;
#ifdef GXE_DEBUG
    _PrintError(LOG_DEBUG,"-- _SearchAttr, name=%s name=%s",ptr->name,name);
#endif
    if(strcasecmp(ptr->name,name) == 0)
      return(ptr);
    lst = g_slist_next(lst);
  }
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
*/
