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
** API des commandes utilisables par les modules
*/
#ifndef _S_API_H
#define _S_API_H


gint           FxRegisterCmd(const gchar *,GModule *);
gint           FxGetMaxParam(FxCommandPtr);
FxParameterPtr FxGetNthParam(FxCommandPtr,guint);
FxParameterPtr FxGetParamByName(FxCommandPtr,gchar *);
gchar          *FxGetParamName(FxParameterPtr);
FxParamType    FxGetParamType(FxParameterPtr);
gchar          *FxGetParamValue(FxParameterPtr);
gchar          *FxGetParamValueByName(FxCommandPtr,gchar *);
gchar          *FxGetCommandName(FxCommandPtr);
gchar          *FxGetRequestAttrValue(gchar *);
gchar          *FxGetCommandAttrValue(FxCommandPtr,gchar *);
gchar          *FxGetParamAttrValue(FxParameterPtr,gchar *);
void           FxSyslog(const gchar *,...);
gint           FxCreatexmlLevel(FxCommandPtr,gint,const gchar *);
gint           FxSetxmlLevel(FxCommandPtr,gint);
gint           FxAppendResult(FxCommandPtr,const gchar *,const gchar *,...);
gint           FxPrintError(FxCommandPtr,gint,const gchar *,...);

#endif
/*
** ----------------------------------------------------------------------------
*/
