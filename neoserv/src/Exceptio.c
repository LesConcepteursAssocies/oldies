// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณEXCEPTIO.C   ณ                                                             ณ
// รฤฤฤฤฤฤฤฤฤฤฤฤฤู                                                             ณ
// ณAuteur                : Guillaumaud Philippe                               ณ
// ณ                        teknik@yaka2.com                                   ณ
// ณ                        http://www.yaka2.com/teknik                        ณ
// ณ                                                                           ณ
// ณVous tes autoris … diffuser et … utiliser ce logiciel sans restrictions. ณ
// ณ                                                                           ณ
// ณCe logiciel est fourni tel quel, sans aucune garantie.                     ณ
// ณL'auteur ne saurait voir sa responsabilit engage en cas de               ณ
// ณdommages de quelque nature que ce soit subis par l'utilisateur             ณ
// ณou des tiers et rsultant directement ou                                   ณ
// ณindirectement de son utilisation, notamment la perte de donnes,           ณ
// ณou toute perte financire rsultant de son                                 ณ
// ณutilisation ou de l'impossibilit de l'utiliser, et ceci mme si           ณ
// ณl'auteur a t prvenu de la possibilit de tels dommages.                 ณ
// ณ                                                                           ณ
// ณCommenc le           : 09/06/1993                                         ณ
// ณDernire modification : 31/07/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des Exceptions hard et soft.                                       ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <controlc.h>
// #include <cerror.h>
#include "generic.h"

// int (* _far _cdecl _cerror_handler)(int *,int *);

#define  mtIGNORE       0
#define  mtRETRY        1
#define  mtABORT        2

static   char           *mtHardErrMsg[] =
{
   "Attempt to write to a write-protected disk",
   "Unknown unit",
   "Drive not ready",
   "Unknown command",
   "CRC error in data",
   "Bad drive-request structure length",
   "Seek error",
   "Unknown media type",
   "Sector not found",
   "Printer out of paper",
   "Write fault",
   "Read fault",
   "General failure"
};
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void      WinInfo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Redirection du CTRL+BREAK
void _cdecl NewBREAK(void)
{
   WinMsgErr("","CTRL+BREAK");
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Redirection des erreurs matrielles.
//
int _far _cdecl NewHardErr(int *ax, int *di)
{
   *ax = mtABORT;
   WinMsgErr("","Erreur de priphrique:");
   return(1);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation des Exceptions
//
void InitException(void)
{
   _controlc_handler = NewBREAK;
   controlc_open();
//   _cerror_handler = NewHardErr;
//   cerror_open();
//   WinInfo("NewHardErr=%p",NewHardErr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Restauration des Exceptions
void EndException(void)
{
   controlc_close();
//   cerror_close();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

