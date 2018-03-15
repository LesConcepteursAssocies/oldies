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
** Traitement des résultats des commandes
*/
#include "common.h"
#include "s_globals.h"

const gchar *_StrxmlError[]=
{
  "Ok",
  "Commande non trouvee",
  "Parametre deja present",
  "Erreur d'execution",
  "Erreur interne"
};


/*
** ----------------------------------------------------------------------------
** Création de la balise de premier niveau de la commande donnée.
** Renvoie 0 si ok.
*/
gint _CreatexmlFirstLevel(_CallCmdPtr Cmd,const gchar *name)
{
  gchar *_procname="_CreatexmlFirstLevel";
  xmlNodePtr tree;
  _FxChildPtr _self;

  /* Pointe sur le fils courant */
  _self = _SearchChild(pthread_self());
  if(_self == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans _SearchChild",_procname);
    return (-1);
  }

  tree = xmlNewChild(_self->Stream->Out->children,NULL,name,NULL);
  if(tree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild",_procname);
    return (-1);
  }
  xmlSetProp(tree,"cmd",Cmd->cmd->name);
  Cmd->FirstLevel   = tree;
  Cmd->CurrentLevel = tree;
  return 0;
}
/*
** ----------------------------------------------------------------------------
** Envoi du résultat de la requète donnée sur le flux de sortie
*/
gint _PrintxmlResult(_FxStreamPtr ptr)
{
  gchar *_procname="_PrintxmlResult";
  xmlChar *buff;
  gint szbuff;

  xmlDocDumpMemory(ptr->Out,&buff,&szbuff);
#ifdef GXE_DEBUG
  _PrintError(LOG_DEBUG,"(%s), resultat=%d octet(s)",_procname,szbuff);
#endif
  if(_WriteSocket(ptr->sock,(const void *)buff,szbuff) == -1)
    _PrintError(LOG_CRIT,"-- (%s), erreur sur _WriteSocket (%d %s)",_procname,errno,strerror(errno));
  free(buff);
  return (0);
}
/*
** ----------------------------------------------------------------------------
** Ajoute une indication d'erreur dans le document de sortie
*/
gint _AppendxmlError(_swParserStatePtr pstat,_ParserError xmlerrno)
{
  gchar *_procname="_AppendxmlError";
  xmlNodePtr Error;
  xmlNodePtr tree;
  _FxChildPtr _self;
  gchar buff[_FxBUFF_SIZE+1];

  /* Pointe sur le fils courant */
  _self = _SearchChild(pthread_self());
  if(_self == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans _SearchChild",_procname);
    return (-1);
  }

  /* Fabrication du buffer de sortie */
  Error = xmlNewChild(_self->Stream->Out->children,NULL,"error",NULL);
  if(Error == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(Error)",_procname);
    return (-1);
  }
  xmlSetProp(Error,"cmd","internal");

  sprintf(buff,"%d",(xmlerrno!=XML_ERR_INTERNAL?xmlerrno:pstat->xmlerrno));
  tree = xmlNewChild(Error,NULL,"code",(const xmlChar *)buff);
  if(tree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(code)",_procname);
    return (-1);
  }

  if(xmlerrno == XML_ERR_INTERNAL)
    sprintf(buff,"%s",pstat->xmlerror);
  else
    sprintf(buff,"%s: %s",_StrxmlError[xmlerrno],pstat->tagerror);
  tree = xmlNewChild(Error,NULL,"msg",(const xmlChar *)buff);
  if(tree == NULL)
  {
    _PrintError(LOG_CRIT,"!!!! (%s), erreur dans xmlNewChild(msg)",_procname);
    return (-1);
  }

  return (0);
}
/*
** ----------------------------------------------------------------------------
*/
