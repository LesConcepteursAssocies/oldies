// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณSWVIDEO.C    ณ                                                             ณ
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
// ณRedfinition des routines vido compatible Silvercom(tm)                   ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <disp.h>
#include <sound.h>
#include "generic.h"

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   void VPrint(mtBYTE,mtBYTE,mtPTRBYTE,mtUSHORT);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Emet un bip.
//
void SWVRingBell(void)
{
   int freq=2000;

   sound_beep(freq);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Scrolling de l'cran.
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
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ecriture d'un caractre
//
void SWVPutChar(mtBYTE Lig,mtBYTE Col,mtBYTE Car,mtUSHORT Coul)
{
   mtBYTE vTmp[2];

   memset(vTmp,0,2);
   sprintf(vTmp,"%c",Car);
   VPrint(Lig,Col,vTmp,Coul);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
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
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Efface l'cran.
void clrscr()
{
   disp_move(0,0);
   disp_eeop();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

