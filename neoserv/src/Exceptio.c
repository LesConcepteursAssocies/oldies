// ��������������������������������͵NEOSERV����������������������������������͸
// �EXCEPTIO.C   �                                                             �
// ���������������                                                             �
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
// ���������������������������������������������������������������������������;

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
// ����������������������������������������������������������������������������
//
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void      WinInfo(mtPTRBYTE,...);
//
// ����������������������������������������������������������������������������
//
// Redirection du CTRL+BREAK
void _cdecl NewBREAK(void)
{
   WinMsgErr("","CTRL+BREAK");
}
//
// ����������������������������������������������������������������������������
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
// ����������������������������������������������������������������������������
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
// ����������������������������������������������������������������������������
//
// Restauration des Exceptions
void EndException(void)
{
   controlc_close();
//   cerror_close();
}
//
// ����������������������������������������������������������������������������
//

