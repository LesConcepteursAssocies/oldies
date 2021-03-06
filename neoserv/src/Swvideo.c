// 嬪様様様様様様冤様様様様様様様様裕NEOSERV突様様様様様様様様様様様様様様様様邑
// �SWVIDEO.C    �                                                             �
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
// �Red�finition des routines vid�o compatible Silvercom(tm)                   �
// 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <disp.h>
#include <sound.h>
#include "generic.h"

//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
extern   void VPrint(mtBYTE,mtBYTE,mtPTRBYTE,mtUSHORT);
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Emet un bip.
//
void SWVRingBell(void)
{
   int freq=2000;

   sound_beep(freq);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Scrolling de l'�cran.
//
void SWVScroll(mtBYTE uSens,mtBYTE uLDeb,mtBYTE uCDeb,mtBYTE uLFin,mtBYTE uCFin,mtBYTE uNbL,mtBYTE uCoul)
{
   int Lines;

   if(uSens == SWSCROLLUP)
      Lines = uNbL;
   else
      Lines = (int) ((uNbL) * -1);
   disp_scroll(Lines,uLDeb,uCDeb,uLFin,uCFin,uCoul);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Ecriture d'un caract�re
//
void SWVPutChar(mtBYTE Lig,mtBYTE Col,mtBYTE Car,mtUSHORT Coul)
{
   mtBYTE vTmp[2];

   memset(vTmp,0,2);
   sprintf(vTmp,"%c",Car);
   VPrint(Lig,Col,vTmp,Coul);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Rend le curseur visible/invisible.
//
void SWVSetCursor(mtBOOL bVis)
{
   if(bVis)
      disp_showcursor();
   else
      disp_hidecursor();
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Efface l'�cran.
void clrscr()
{
   disp_move(0,0);
   disp_eeop();
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//

