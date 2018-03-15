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
#ifndef _S_CMD_H
#define _S_CMD_H


_FxCmdPtr    _SearchCmdT(const gchar *);
_ModulePtr   _SearchCmdM(const gchar *);
_FxCmdPtr    _SearchCommand(const gchar *);
_CallCmdPtr  _NewCallCmd(void);
void         _FreeCallCmd(_CallCmdPtr);
_FxParamPtr  _NewParam(void);
void         _FreeParam(_FxParamPtr);
_FxParamPtr  _SearchParam(const gchar *,GSList *);
void         _PushParam(_swParserStatePtr,_FxParamPtr);
_FxParamPtr  _PopParam(_swParserStatePtr);
gint         _CountParam(_FxParamPtr);
void         _FreeTCmds(void);

#endif
/*
** ----------------------------------------------------------------------------
*/
