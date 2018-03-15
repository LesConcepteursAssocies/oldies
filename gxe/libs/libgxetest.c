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
** Librairie de test
*/
#include "common.h"
#include "s_globals.h"

/*
** ----------------------------------------------------------------------------
** commande de test
*/
gint TestLib(FxCommandPtr cmd)
{
  gint i,n;
  FxParameterPtr param;
  gchar *prid;

  FxSyslog("** (%s) TEST COMMANDE",FxGetCommandName(cmd));
  n = FxGetMaxParam(cmd);
  FxSyslog("**** il y a %d paramètres",n);
  for(i=0;i<n;i++)
  {
    param = FxGetNthParam(cmd,i);
    if(param)
    {
      /*
	FxSyslog("***** [%d]: nom=%s valeur=%s",i,FxGetParamName(param),FxGetParamValue(param));
      */
      FxAppendResult(cmd,FxGetParamName(param),"%s",FxGetParamValue(param));
    }
  }

  prid = FxGetRequestAttrValue("id");
  if(prid)
    FxAppendResult(cmd,"idrequete","%s",prid);

#if 0
  /*
  ** test de sous-arbre
  */
  FxCreatexmlLevel(cmd,FX_CURRENTLEVEL,"ligne 1");
  FxAppendResult(cmd,"pseudo","PHILIPPE");
  FxAppendResult(cmd,"dept","93");

  FxSetxmlLevel(cmd,FX_PREVIOUSLEVEL);
  FxAppendResult(cmd,"commentaire","--- entre les 2 lignes");

  FxCreatexmlLevel(cmd,FX_FIRSTLEVEL,"ligne 2");
  FxAppendResult(cmd,"pseudo","FRANCOIS");
  FxAppendResult(cmd,"dept","75");

  FxCreatexmlLevel(cmd,FX_CURRENTLEVEL,"sous-ligne 2");
  FxAppendResult(cmd,"addresse","rue machin");
  FxAppendResult(cmd,"ville","la-bas");

  FxSetxmlLevel(cmd,FX_PREVIOUSLEVEL);
  FxAppendResult(cmd,"profession","chef");

  FxSetxmlLevel(cmd,FX_PREVIOUSLEVEL);
  FxAppendResult(cmd,"divers","voila du code qui va bien");
#endif

  return (FX_RETURN_SUCCESS);
}
/*
** ----------------------------------------------------------------------------
** génère un SIGSEGV (pour test des erreurs)
*/
gint TestErreur(FxCommandPtr cmd)
{
  gchar *pcv;
  int moncv;

  pcv = FxGetParamValueByName(cmd,"cv");
  moncv = atoi(pcv);
  return (FX_RETURN_SUCCESS);
}
/*
** ----------------------------------------------------------------------------
** Fonction appelée automatiquement au chargement du module
*/
const gchar *g_module_check_init(GModule *module)
{
  gchar *_procname="g_module_check_init";

  FxSyslog("-- (%s), CHARGEMENT DU MODULE %s.",_procname,g_module_name(module));

#if 0
  if(FxRegisterCmd("TestLib",module)<0)
    FxSyslog("-- (%s), erreur à l'enregistrement de la commande TestLib.",_procname);
#endif
  return (NULL);
}
/*
** ----------------------------------------------------------------------------
*/
