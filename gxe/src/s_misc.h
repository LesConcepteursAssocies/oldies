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
** Routines diverses
*/
#ifndef _S_MISC_H
#define _S_MISC_H

gint         _InitMain(void);
void         _InitApp(void);
void         _FreeRessources(void);
gint          _NewStreamxmlOut(_FxStreamPtr);
_FxStreamPtr _NewFxStream(gint);
void         _FreeFxStream(_FxStreamPtr,gint);
gchar        *_StrChildState(gint);
gchar        *_StrStreamType(gint);
gchar        *_StrOutputMode(gint);
gchar        *_StrParamType(gint);
gint         _ReadCfgFile(void);
void         _DumpParam(_FxParamPtr);
void         _DumpChild(_FxChildPtr);

#endif
/*
** ----------------------------------------------------------------------------
*/
