// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณSTATS.C      ณ                                                             ณ
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
// ณGestion des statistiques                                                   ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <dos.h>
#include <direct.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"

StPalier StatPalier[MAXPALIER] =
{
   { "AUTRES"  , 0,0.00,0L,0L },
   { "3614 T20",82,0.00,0L,0L },     // Tarifs de 93 par dfaut.
   { "3615 T34",83,0.61,0L,0L },
   { "3615 T36",70,0.88,0L,0L },
   { "3615 T44",86,1.71,0L,0L },
   { "3616 T34",92,0.61,0L,0L },
   { "3616 T36",84,0.88,0L,0L },
   { "3617 T44",85,1.71,0L,0L },
   { "3617 T46",87,2.80,0L,0L },
   { "3617 T60",88,4.68,0L,0L },
   { "3628 T60",90,4.68,0L,0L },
   { "3629 T70",91,7.95,0L,0L }
};
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   mtPTRAPP   pAppli;             // Pointe sur la 1re appli.
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   float      TimeToCent(struct dos_time_t *);
extern   void       WinInfo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void          MajStatPalier(mtPTRCV);
mtPTRSTAPP    NewStApp(mtPTRAPP,mtBYTE);
mtPTRSTAPP    SearchStApp(mtPTRAPP,mtBYTE);
void          DelStApp(mtPTRAPP);
void          MajStApp(mtPTRCV);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// RAZ des infos des paliers gnraux.
//
void RAZSysPalier(void)
{
   int i;

   for(i=0;i<MAXPALIER;i++)
   {
      StatPalier[i].NbAppel = (mtUDWORD) 0;
      StatPalier[i].TpsCnx  = (mtUDWORD) 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Mise … jour des statistiques du palier du connect donn.
//
void MajStatPalier(mtPTRCV LeCv)
{
   int i;
   mtBOOL bMaj = mtFALSE;
   float TDeb,TFin;

   // D'abord les stats gnrales.
   //
   // On n'a pas besoin des centimes de seconde.
   //
   // WinInfo("MAJSTATPALIER");
   LeCv->HeureCnx.hsecond = 0;
   LeCv->HeureLib.hsecond = 0;
   for(i=1;i<MAXPALIER;i++)
   {
      if(StatPalier[i].Palier == LeCv->Palier)
      {
         bMaj = mtTRUE;
         StatPalier[i].NbAppel++;
         TDeb = TimeToCent(&LeCv->HeureCnx);
         TFin = TimeToCent(&LeCv->HeureLib);
         if(TFin<TDeb)
            // a cheval sur 2 jours.
            TFin += (float) ((float) 86400 * (float) 100);
         // WinInfo("TDEB=%f TFIN=%f",TDeb,TFin);
         StatPalier[i].TpsCnx += (TFin-TDeb)/100;
         break;
      }
   }
   if(! bMaj)
   {
      // Palier non trouv,
      // on le met dans "Autres"
      StatPalier[0].NbAppel++;
      TDeb = TimeToCent(&LeCv->HeureCnx);
      TFin = TimeToCent(&LeCv->HeureLib);
      if(TFin<TDeb)
         // a cheval sur 2 jours.
         TFin += (float) ((float) 86400 * (float) 100);
      // WinInfo("TDEB=%f TFIN=%f",TDeb,TFin);
      StatPalier[0].TpsCnx += (TFin-TDeb)/100;
   }
   // Ensuite les stats de l'application
   MajStApp(LeCv);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cration d'un nouveau bloc de stat pour une application et un palier donn.
// Renvoie le pointeur sur le nouveau bloc.
//
mtPTRSTAPP NewStApp(mtPTRAPP pApp,mtBYTE pPalier)
{
   mtPTRSTAPP NewS,s;

   NewS         = (mtPTRSTAPP) MemAlloc(sizeof(StApp));
   NewS->Palier = pPalier;
   if(! pApp->pStat)
      // C'est le 1er bloc
      pApp->pStat = NewS;
   else
   {
      s = pApp->pStat;
      while(s->pNextSt)
         s = s->pNextSt;              // On se place … la fin de la liste.
      s->pNextSt = NewS;
   }
   return(NewS);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un palier dans la chaine des blocs d'une appli.
// Renvoie le pointeur sur le bloc trouv ou NULL.
//
mtPTRSTAPP SearchStApp(mtPTRAPP pApp,mtBYTE pPalier)
{
   mtPTRSTAPP s,SSt=(mtPTRSTAPP) 0;

   s = pApp->pStat;
   while(s)
   {
      if(s->Palier == pPalier)
      {
         SSt = s;
         break;
      }
      s = s->pNextSt;
   }
   return(SSt);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression des blocs de stats d'une application donne.
//
void DelStApp(mtPTRAPP pApp)
{
   mtPTRSTAPP s,sNext;

   s = pApp->pStat;
   while(s)
   {
      sNext = s->pNextSt;
      MemFree(s);
      s = sNext;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Mise … jour des statistiques d'un palier de l'application.
//
void MajStApp(mtPTRCV pCv)
{
   mtPTRSTAPP s;
   float TDeb,TFin;

   // WinInfo("MAJSTATAPP");
   if(pCv->pApp)
   {
      s = SearchStApp(pCv->pApp,pCv->Palier);
      if(! s)
         s = NewStApp(pCv->pApp,pCv->Palier);
      if(s)
      {
         s->NbAppel++;
         TDeb = TimeToCent(&pCv->HeureCnx);
         TFin = TimeToCent(&pCv->HeureLib);
         if(TFin<TDeb)
            // a cheval sur 2 jours.
            TFin += (float) ((float) 86400 * (float) 100);
         s->TpsCnx += (TFin-TDeb)/100;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// RAZ des infos des paliers de toutes les applications prsentes.
//
void RAZAppPalier(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      DelStApp(p);
      p->pStat = NULL;
      p        = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

