// 嬪様様様様様様冤様様様様様様様様裕NEOSERV突様様様様様様様様様様様様様様様様邑
// �SCRNSAVE.C   �                                                             �
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
// �Gestion de la sauvegarde de l'�cran                                        �
// 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include <dos.h>
#include <conio.h>

#define  EVSTATC        0x3DA          // Registre d'�tat couleur EGA/VGA.
#define  EVSTATM        0x3BA          // Registre d'�tat mono EGA/VGA.
#define  EVATTR         0x3C0          // Contr�leur d'attribut EGA/VGA.

//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Eteint l'�cran
//
void ScreenOff()
{
   _disable();
   inp(EVSTATC);                   // RAZ du registre d'�tat couleur.
   inp(EVSTATM);                   // RAZ du registre d'�tat mono.
   outp(EVATTR,0x00);              // Reset du bit 5, ce qui supprime
   _enable();                           // la liaison avec le CTRC.
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Rallume l'�cran
//
void ScreenOn()
{
   _disable();
   inp(EVSTATC);                   // RAZ du registre d'�tat couleur.
   inp(EVSTATM);                   // RAZ du registre d'�tat mono.
   outp(EVATTR,0x20);              // Activation du bit 5, ce qui r�tablit
   _enable();                           // la liaison avec le CTRC.
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//

