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
** Traitement des erreurs et messages divers
*/
#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
** Initialisation de la journalisation des erreurs
*/
void _InitError(void)
{
  openlog(S_LOG_IDENT,S_LOG_OPTIONS,S_LOG_FACILITY);
}
/*
** ----------------------------------------------------------------------------
** Envoi d'un message d'erreur
*/
void _PrintError(gint type,const gchar *msg,...)
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
      syslog(LOG_CRIT,"!! _PrintError, mémoire insuffisante");
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
    syslog(type,buff);
  va_end(args);
  g_free(buff);
}
/*
** ----------------------------------------------------------------------------
*/
