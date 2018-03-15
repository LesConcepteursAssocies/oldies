// ��������������������������������͵NEOSERV����������������������������������͸
// �HORLOGE.C    �                                                             �
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
// �Gestion de l'horloge temps r�el.                                           �
// ���������������������������������������������������������������������������;

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <int.h>
#include <time.h>
#include "generic.h"

//
// Param�tres de l'horloge des PC.
//
#define  RTCAdrPort     0x70           // Registre d'adresse de la RTC.
#define  RTCDtaPort     0x71           // Registre de donn�es de la RTC.

#define  SECONDES        0             // Adresses de cellules de m�moire RTC.
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
// ����������������������������������������������������������������������������
//
extern   mtBYTE    Heures;
extern   mtBYTE    Minutes;
extern   mtBYTE    Secondes;
extern   mtBYTE    Jour;
extern   mtBYTE    Mois;
extern   mtWORD    Annee;
//
// ����������������������������������������������������������������������������
//
// Lecture d'une des cellules de la RTC.
//
mtBYTE RTCRead(mtBYTE Adresse)
{
  outp(RTCAdrPort, Adresse);
  return(inp(RTCDtaPort));
}
//
// ����������������������������������������������������������������������������
//
// Lecture d'une des cellules de la date ou de l'heure de la RTC.
// Conversion du r�sultat en binaire si l'horloge travaille en BCD.
//
mtBYTE RTCDt(mtBYTE Adresse)
{
  if(!(RTCRead(ETATB) & 4))             // Mode BCD ou binaire ?
    return((RTCRead(Adresse) >> 4) * 10 + (RTCRead(Adresse) & 15));
  else
    return(RTCRead(Adresse));          // C'est le mode binaire.
}
//
// ����������������������������������������������������������������������������
//
// Lecture de l'heure courante.
//
void RTCHeure(mtPTRBYTE Heures, mtPTRBYTE Minutes, mtPTRBYTE Secondes)
{
   if(!(RTCRead(ETATA) & 128))         // En cours de m.a.j ?
      if(!(RTCRead(DIAGNOSTIC) & 128)) // Piles en bon �tat ?.
      {
         *Heures   = RTCDt(HEURES);
         *Minutes  = RTCDt(MINUTES);
         *Secondes = RTCDt(SECONDES);
      }
}
//
// ����������������������������������������������������������������������������
//
// Lecture de la date courante.
//
void RTCDate(mtPTRBYTE Jour, mtPTRBYTE Mois, mtPTRWORD Annee)
{
   if(!(RTCRead(ETATA) & 128))         // En cours de m.a.j ?
      if(!(RTCRead(DIAGNOSTIC) & 128)) // Piles en bon �tat ?.
      {
         *Jour  = RTCDt(JOUR);
         *Mois  = RTCDt(MOIS);
         *Annee = (mtUWORD) ((RTCDt(SIECLE)*100)+RTCDt(ANNEE));
      }
}
//
// ����������������������������������������������������������������������������
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
// ����������������������������������������������������������������������������
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
// ����������������������������������������������������������������������������
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
// ����������������������������������������������������������������������������
//

