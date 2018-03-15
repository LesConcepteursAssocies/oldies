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
** Gestion des sessions
*/

#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
** Création d'une session.
** Renvoie un pointeur sur la session si ok.
*/
_FxSessionPtr _NewFxSession(gchar *name)
{
  _FxSessionPtr ptr;

  ptr = (_FxSessionPtr)g_malloc0(sizeof(struct _FxSession));
  if(ptr == NULL)
  {
    syslog(LOG_CRIT,"!! _NewFxSession, mémoire insuffisante");
    return (NULL);
  }

  ptr->frontalname = g_strdup(name);
  return (ptr);
}
/*
** ----------------------------------------------------------------------------
** Libération des ressources occupée par la session donnée
*/
void _FreeFxSession(_FxSessionPtr ptr)
{
  if(ptr)
  {
    g_free(ptr->frontalname);
    g_free(ptr->sessionID);
    g_free(ptr);
  }
}
/*
** ----------------------------------------------------------------------------
*/
