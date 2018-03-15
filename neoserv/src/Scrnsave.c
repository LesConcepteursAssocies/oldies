// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณSCRNSAVE.C   ณ                                                             ณ
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
// ณGestion de la sauvegarde de l'cran                                        ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <dos.h>
#include <conio.h>

#define  EVSTATC        0x3DA          // Registre d'tat couleur EGA/VGA.
#define  EVSTATM        0x3BA          // Registre d'tat mono EGA/VGA.
#define  EVATTR         0x3C0          // Contr“leur d'attribut EGA/VGA.

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Eteint l'cran
//
void ScreenOff()
{
   _disable();
   inp(EVSTATC);                   // RAZ du registre d'tat couleur.
   inp(EVSTATM);                   // RAZ du registre d'tat mono.
   outp(EVATTR,0x00);              // Reset du bit 5, ce qui supprime
   _enable();                           // la liaison avec le CTRC.
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rallume l'cran
//
void ScreenOn()
{
   _disable();
   inp(EVSTATC);                   // RAZ du registre d'tat couleur.
   inp(EVSTATM);                   // RAZ du registre d'tat mono.
   outp(EVATTR,0x20);              // Activation du bit 5, ce qui rtablit
   _enable();                           // la liaison avec le CTRC.
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

