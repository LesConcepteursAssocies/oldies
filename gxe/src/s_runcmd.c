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
** Exécution des commandes
*/
#include "common.h"
#include "s_globals.h"


/*
** ----------------------------------------------------------------------------
** Exécution de la commande donnée
*/
gint _ExecuteCommand(_CallCmdPtr Cmd,_FxChildPtr _self)
{
  gchar *_procname="_ExecuteCommand";
  gint ret;
  _RunCmdPtr runcmd;

  _self->Stream->state = CHLD_RUN_CMD;
  /* Quelques vérifications... */
  if(Cmd->cmd == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), structure non renseignée (cmd=NULL)",_procname);
    return (-1);
  }
  if(Cmd->cmd->pmodule == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), structure non renseignée (cmd->pmodule=NULL)",_procname);
    return (-1);
  }
  if(Cmd->cmd->name == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), structure non renseignée (cmd->name=NULL)",_procname);
    return (-1);
  }
  /* Tout a l'air ok, on y va... */
  if(g_module_symbol(Cmd->cmd->pmodule,Cmd->cmd->name,(gpointer)&runcmd))
  {
    /* Préparation du buffer de sortie */
    if(_CreatexmlFirstLevel(Cmd,"result") != 0)
    {
      _PrintError(LOG_CRIT,"!!!! (%s), erreur dans _CreatexmlFirstLevel",_procname);
      return (-1);
    }

    ret = runcmd(Cmd);

    return (ret);
  }
  else
  {
    _PrintError(LOG_CRIT,"!!!! (%s), symbole %s non trouvé dans %s",_procname,Cmd->cmd->name,g_module_name(Cmd->cmd->pmodule));
    return (-1);
  }
  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
