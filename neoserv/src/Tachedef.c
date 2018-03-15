// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณTACHEDEF.C   ณ                                                             ณ
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
// ณTraitement des diverses tches de fond.                                    ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <direct.h>
#include <io.h>
#include <math.h>
#include <fltpnt.h>
#include "d4all.h"
#pragma SC align
#include "minitel.h"
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "ports.h"

mtBOOL        bRCNX;                   // Utilises pour la vrification
mtUDWORD      TpRCNX;                  // priodique des connects
mtWORD        wVerCNX;                 // actifs.
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   Ports      tPorts[MAXPORTS];
extern   mtBYTE     Heures;
extern   mtBYTE     Minutes;
extern   mtBYTE     Secondes;
extern   void       WinInfo(mtPTRBYTE,...);
extern   UnCv       TabCv[TOTALCV];         // Dfini Dans le prog. principal.
extern   UnCv       LTask[TOTALCV];         // Dfini Dans le prog. principal.
extern   UnCv       OLCv[TOTALCV];          // Dfini Dans le prog. principal.
extern   mtPTRCV    pCurrCV;                // Pointe sur le cv courant.
extern   mtPTRAPP   pAppli;                 // Pointe sur la 1re application.
extern   void       MemFree(mtPOINTEUR);
extern   float      TimeToCent(struct dos_time_t *);
extern   void       DelSymValue(mtPTRDICO);
extern   void       DeconnecteCv(mtPTRCV);
extern   void       WriteLogNeo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoi VRAI si le pointeur donn est dans une des piles du connect Donn.
//
mtBOOL IsInStack(mtPTRCV LeCv,mtPOINTEUR ptr)
{
   mtBOOL bOk=mtFALSE;
   mtUWORD uSP;
   mtPOINTEUR IPtr;

   // Scanning de la pile de donnes.
   if(LeCv->PileD)
   {
      uSP = (LeCv->SP);
      while(uSP)
      {
         uSP--;
         IPtr = LeCv->PileD[uSP];
         if(IPtr == ptr)
         {
            bOk = mtTRUE;
            break;
         }
      }
   }
   if(! bOk)
   {
      // Scanning de la pile de retour.
      if(LeCv->PileR)
      {
         uSP = (LeCv->RP);
         while(uSP)
         {
            uSP--;
            IPtr = LeCv->PileR[uSP];
            if(IPtr == ptr)
            {
               bOk = mtTRUE;
               break;
            }
         }
      }
   }
   return(bOk);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des ressources mmoires du cv donn.
//
void RecupMem(mtPTRCV LeCv,mtPTRCV TaskCV)
{
   struct dos_time_t HDeb,HFin;
   float dHDeb,dHFin;
   int RMax;
   mtPTRDICO p;

   //
   // On se donne la moiti du temps
   // imparti pour l'excution du code.
   //
   RMax  = (int) (MAXEXEC/2);
   _dos_gettime(&HDeb);
   dHDeb = TimeToCent(&HDeb)*10;
   while(1)
   {
      if(LeCv->pIDico)
      {
         p = (mtPTRDICO) LeCv->pIDico[LeCv->IGC];
         if(p)
         {
            if(! IsInStack(LeCv,(mtPOINTEUR) p))
            {
               if(TaskCV)
               {
                  // Tche locale active.
                  if(! IsInStack(TaskCV,(mtPOINTEUR) p))
                  {
                     DelSymValue(p);
                     MemFree(p);
                     LeCv->pIDico[LeCv->IGC] = (mtPOINTEUR) 0;
                     LeCv->NbISym--;
                     // On met … jour les pointeurs de la tche locale.
                     TaskCV->pIDico = LeCv->pIDico;
                     TaskCV->NbISym = LeCv->NbISym;
                  }
               }
               else
               {
                  DelSymValue(p);
                  MemFree(p);
                  LeCv->pIDico[LeCv->IGC] = (mtPOINTEUR) 0;
                  LeCv->NbISym--;
               }
            }
         }
         // Symbole suivant.
         LeCv->IGC++;
         if(LeCv->IGC>=LeCv->ISP)
            // On recommence … partir du 1er symbole.
            LeCv->IGC = 0;
      }
      else
         break;
      _dos_gettime(&HFin);
      dHFin = TimeToCent(&HFin)*10;
      if(dHFin<dHDeb)
         // A cheval sur 2 jours.
         dHFin += (float) 86400000;
      if((dHFin - dHDeb) >= RMax)
         break;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Le Ramasse-Miettes (!)
// Rcupre la mmoire des symboles internes non utiliss.
//
void GarbageCollector(void)
{
   mtUWORD iCv;
   mtPTRAPP p;
   mtPTRCV lcv;

   // Balayage des connect principaux.
   for(iCv=0;iCv<TOTALCV;iCv++)
   {
      if(TabCv[iCv].Actif)
      {
         if(LTask[iCv].Actif)
            // Tche de fond active.
            RecupMem(&TabCv[iCv],&LTask[iCv]);
         else
            RecupMem(&TabCv[iCv],NULL);
      }
      // Rcupration mmoire sur les tches ONLIB.
      if(OLCv[iCv].Actif)
         RecupMem(&OLCv[iCv],NULL);
   }
   // Balayage des applications, et traitement des tches globales.
   p = pAppli;
   while(p)
   {
      if((p->Active) && (p->bGTask))
      {
         lcv = (mtPTRCV) p->pGTCv;
         if(lcv->Actif)
            RecupMem(lcv,NULL);
      }
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Vrification des connects.
//
void VerifCNX(void)
{
   mtUWORD iCv;
   char VBuff[81],cTmp[25];
   struct dos_time_t tCx;
   mtUDWORD wCx;
   mtWORD nDec=0;

   if(bRCNX)
   {
      memset(VBuff,0,81);
      sprintf(VBuff,"%02d:%02d:%02d, VerifCNX",Heures,Minutes,Secondes);
      _dos_gettime(&tCx);
      tCx.hsecond = 0;
      wCx         = TimeToCent(&tCx)/100;
      for(iCv=0;iCv<TOTALCV;iCv++)
      {
         if(TabCv[iCv].Actif)
         {
            if((wCx-TabCv[iCv].HRPaq) >= 300)
            {
               // On deconnecte le cv au bout de 5mn d'inactivit
               tPorts[TabCv[iCv].NumPort].LibereCV(iCv);
               nDec++;
            }
         }
      }
      sprintf(cTmp,", Termin … %02d:%02d:%02d",Heures,Minutes,Secondes);
      strcat(VBuff,cTmp);
      if(nDec)
      {
         sprintf(cTmp,", %d Voie(s) libre(s)",nDec);
         strcat(VBuff,cTmp);
      }
      bRCNX  = mtFALSE;
      TpRCNX = 0L;
      WinInfo("%s",VBuff);
      WriteLogNeo("%s",VBuff);
   }
   else
   {
      if((wVerCNX) && (! bRCNX))
      {
         _dos_gettime(&tCx);
         tCx.hsecond = 0;
         wCx         = TimeToCent(&tCx)/100;
         if(! TpRCNX)
            // On initialise le compteur.
            TpRCNX = wCx;
         else
         {
            // On vrifie les connects toutes les wVerCNX mn.
            if((wCx-TpRCNX) >= wVerCNX)
               bRCNX = mtTRUE;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Raffraichissement priodique des bases des applications.
//
void RefreshBaseApp(void)
{
   mtUWORD iCv;
   char VBuff[81],cTmp[25];
   struct dos_time_t tBs;
   mtUDWORD wBs;
   int dberr;

   for(iCv=0;iCv<TOTALCV;iCv++)
   {
      // Les connects.
      if((TabCv[iCv].Actif) && (TabCv[iCv].pApp->bAutoCommit))
      {
         if(TabCv[iCv].bACMIT)
         {
            memset(VBuff,0,81);
            sprintf(VBuff,"%02d:%02d:%02d, DbAutoCommit %s voie=%d",Heures,Minutes,Secondes,TabCv[iCv].pApp->Nom,iCv);
            dberr = d4flush_files(&TabCv[iCv].CBase);
            sprintf(cTmp,", Termin … %02d:%02d:%02d",Heures,Minutes,Secondes);
            strcat(VBuff,cTmp);
            TabCv[iCv].bACMIT  = mtFALSE;
            TabCv[iCv].TpACMIT = 0L;
            WinInfo("%s",VBuff);
            WriteLogNeo("%s",VBuff);
         }
         else
         {
            _dos_gettime(&tBs);
            tBs.hsecond = 0;
            wBs         = TimeToCent(&tBs)/100;
            if(! TabCv[iCv].TpACMIT)
               // On initialise le compteur.
               TabCv[iCv].TpACMIT = wBs;
            else
            {
               // On raffraichit toutes les wCMIT mn.
               if((wBs-TabCv[iCv].TpACMIT) >= TabCv[iCv].pApp->wCMIT)
                  TabCv[iCv].bACMIT = mtTRUE;
            }
         }
      }
      // Les tches de fond.
      if((LTask[iCv].Actif) && (LTask[iCv].pApp->bAutoCommit))
      {
         if(LTask[iCv].bACMIT)
         {
            memset(VBuff,0,81);
            sprintf(VBuff,"%02d:%02d:%02d, DbAutoCommit %s Tche de fond=%d",Heures,Minutes,Secondes,LTask[iCv].pApp->Nom,iCv);
            dberr = d4flush_files(&LTask[iCv].CBase);
            sprintf(cTmp,", Termin … %02d:%02d:%02d",Heures,Minutes,Secondes);
            strcat(VBuff,cTmp);
            LTask[iCv].bACMIT  = mtFALSE;
            LTask[iCv].TpACMIT = 0L;
            WinInfo("%s",VBuff);
            WriteLogNeo("%s",VBuff);
         }
         else
         {
            _dos_gettime(&tBs);
            tBs.hsecond = 0;
            wBs         = TimeToCent(&tBs)/100;
            if(! LTask[iCv].TpACMIT)
               // On initialise le compteur.
               LTask[iCv].TpACMIT = wBs;
            else
            {
               // On raffraichit toutes les wCMIT mn.
               if((wBs-LTask[iCv].TpACMIT) >= LTask[iCv].pApp->wCMIT)
                  LTask[iCv].bACMIT = mtTRUE;
            }
         }
      }
      // Les tches ONLIB.
      if((OLCv[iCv].Actif) && (OLCv[iCv].pApp->bAutoCommit))
      {
         if(OLCv[iCv].bACMIT)
         {
            memset(VBuff,0,81);
            sprintf(VBuff,"%02d:%02d:%02d, DbAutoCommit %s Tche ONLIB=%d",Heures,Minutes,Secondes,OLCv[iCv].pApp->Nom,iCv);
            dberr = d4flush_files(&OLCv[iCv].CBase);
            sprintf(cTmp,", Termin … %02d:%02d:%02d",Heures,Minutes,Secondes);
            strcat(VBuff,cTmp);
            OLCv[iCv].bACMIT  = mtFALSE;
            OLCv[iCv].TpACMIT = 0L;
            WinInfo("%s",VBuff);
            WriteLogNeo("%s",VBuff);
         }
         else
         {
            _dos_gettime(&tBs);
            tBs.hsecond = 0;
            wBs         = TimeToCent(&tBs)/100;
            if(! OLCv[iCv].TpACMIT)
               // On initialise le compteur.
               OLCv[iCv].TpACMIT = wBs;
            else
            {
               // On raffraichit toutes les wCMIT mn.
               if((wBs-OLCv[iCv].TpACMIT) >= OLCv[iCv].pApp->wCMIT)
                  OLCv[iCv].bACMIT = mtTRUE;
            }
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

