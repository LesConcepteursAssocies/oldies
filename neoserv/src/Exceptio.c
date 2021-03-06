// 嬪様様様様様様冤様様様様様様様様裕NEOSERV突様様様様様様様様様様様様様様様様邑
// �EXCEPTIO.C   �                                                             �
// 団陳陳陳陳陳陳�                                                             �
// �Auteur                : Guillaumaud Philippe                               �
// �                        teknik@yaka2.com                                   �
// �                        http://www.yaka2.com/teknik                        �
// �                                                                           �
// �Vous �tes autoris� � diffuser et � utiliser ce logiciel sans restrictions. �
// �                                                                           �
// �Ce logiciel est fourni tel quel, sans aucune garantie.                     �
// �L'auteur ne saurait voir sa responsabilit� engag�e en cas de               �
// �dommages de quelque nature que ce soit subis par l'utilisateur             �
// �ou des tiers et r�sultant directement ou                                   �
// �indirectement de son utilisation, notamment la perte de donn�es,           �
// �ou toute perte financi�re r�sultant de son                                 �
// �utilisation ou de l'impossibilit� de l'utiliser, et ceci m�me si           �
// �l'auteur a �t� pr�venu de la possibilit� de tels dommages.                 �
// �                                                                           �
// �Commenc� le           : 09/06/1993                                         �
// �Derni�re modification : 31/07/1995                                         �
// �                                                                           �
// �Gestion des Exceptions hard et soft.                                       �
// 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

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
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void      WinInfo(mtPTRBYTE,...);
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Redirection du CTRL+BREAK
void _cdecl NewBREAK(void)
{
   WinMsgErr("","CTRL+BREAK");
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Redirection des erreurs mat�rielles.
//
int _far _cdecl NewHardErr(int *ax, int *di)
{
   *ax = mtABORT;
   WinMsgErr("","Erreur de p�riph�rique:");
   return(1);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
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
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Restauration des Exceptions
void EndException(void)
{
   controlc_close();
//   cerror_close();
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//

