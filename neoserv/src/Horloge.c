// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณHORLOGE.C    ณ                                                             ณ
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
// ณGestion de l'horloge temps rel.                                           ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <int.h>
#include <time.h>
#include "generic.h"

//
// Paramtres de l'horloge des PC.
//
#define  RTCAdrPort     0x70           // Registre d'adresse de la RTC.
#define  RTCDtaPort     0x71           // Registre de donnes de la RTC.

#define  SECONDES        0             // Adresses de cellules de mmoire RTC.
#define  MINUTES         2
#define  HEURES          4
#define  JOURSEMAINE     6
#define  JOUR            7
#define  MOIS            8
#define  ANNEE           9
#define  ETATA          10
#define  ETATB          11
#define  ETATC          12
#define  ETATD          13
#define  DIAGNOSTIC     14
#define  SIECLE         50
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   mtBYTE    Heures;
extern   mtBYTE    Minutes;
extern   mtBYTE    Secondes;
extern   mtBYTE    Jour;
extern   mtBYTE    Mois;
extern   mtWORD    Annee;
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'une des cellules de la RTC.
//
mtBYTE RTCRead(mtBYTE Adresse)
{
  outp(RTCAdrPort, Adresse);
  return(inp(RTCDtaPort));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'une des cellules de la date ou de l'heure de la RTC.
// Conversion du rsultat en binaire si l'horloge travaille en BCD.
//
mtBYTE RTCDt(mtBYTE Adresse)
{
  if(!(RTCRead(ETATB) & 4))             // Mode BCD ou binaire ?
    return((RTCRead(Adresse) >> 4) * 10 + (RTCRead(Adresse) & 15));
  else
    return(RTCRead(Adresse));          // C'est le mode binaire.
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture de l'heure courante.
//
void RTCHeure(mtPTRBYTE Heures, mtPTRBYTE Minutes, mtPTRBYTE Secondes)
{
   if(!(RTCRead(ETATA) & 128))         // En cours de m.a.j ?
      if(!(RTCRead(DIAGNOSTIC) & 128)) // Piles en bon tat ?.
      {
         *Heures   = RTCDt(HEURES);
         *Minutes  = RTCDt(MINUTES);
         *Secondes = RTCDt(SECONDES);
      }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture de la date courante.
//
void RTCDate(mtPTRBYTE Jour, mtPTRBYTE Mois, mtPTRWORD Annee)
{
   if(!(RTCRead(ETATA) & 128))         // En cours de m.a.j ?
      if(!(RTCRead(DIAGNOSTIC) & 128)) // Piles en bon tat ?.
      {
         *Jour  = RTCDt(JOUR);
         *Mois  = RTCDt(MOIS);
         *Annee = (mtUWORD) ((RTCDt(SIECLE)*100)+RTCDt(ANNEE));
      }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Nouvelle interruption timer.
//
int _cdecl NewTimer(struct INT_DATA *pd)
{
   RTCHeure(&Heures,&Minutes,&Secondes);
   RTCDate(&Jour,&Mois,&Annee);
   return(0);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation de l'horloge.
//
void TimerInit(void)
{
   RTCHeure(&Heures,&Minutes,&Secondes);
   RTCDate(&Jour,&Mois,&Annee);
   int_off();
   int_intercept(0x1C,NewTimer,0);
   int_on();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Remet en place l'ancien timer.
//
void TimerEnd(void)
{
   int_off();
   int_restore(0x1C);
   int_on();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

