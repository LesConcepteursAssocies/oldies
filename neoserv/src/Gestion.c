// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณGESTION.C    ณ                                                             ณ
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
// ณGestion des infos systme (bases, chronos, ...)                            ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dos.h>
#include <malloc.h>
#include <disp.h>
#include <mem.h>
#include <direct.h>
#include <ctype.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "minitel.h"
#include "ports.h"

//
// Structures des bases systmes.
//
// NEOSYS.DBF
FIELD4INFO    NeoStru[] =
{
   { "DATEJOUR" , 'D',  8, 0 },
   { "CHMAX"    , 'N',  6, 2 },
   { "CNXMAX"   , 'N',  3, 0 },
   { "CPMAX"    , 'N',  4, 1 },
   { "CFMAX"    , 'N',  4, 1 },
   { "NBAPPTOT" , 'N',  5, 0 },
   { "HPRECNX"  , 'C',  6, 0 },
   { "HDERCNX"  , 'C',  6, 0 },
   { "P1NBPQIN" , 'N', 10, 0 },
   { "P1NBPQOUT", 'N', 10, 0 },
   { "P1NBPQERR", 'N', 10, 0 },
   { "P2NBPQIN" , 'N', 10, 0 },
   { "P2NBPQOUT", 'N', 10, 0 },
   { "P2NBPQERR", 'N', 10, 0 },
   { "P3NBPQIN" , 'N', 10, 0 },
   { "P3NBPQOUT", 'N', 10, 0 },
   { "P3NBPQERR", 'N', 10, 0 },
   { "P4NBPQIN" , 'N', 10, 0 },
   { "P4NBPQOUT", 'N', 10, 0 },
   { "P4NBPQERR", 'N', 10, 0 },
   { 0,0,0,0 }
};
TAG4INFO      NeoIdx[] =
{
   { "NEOSERV", "DATEJOUR", 0, 0, 0 },
   { 0,0,0,0,0 }
};
// NEOSTAT.DBF
FIELD4INFO    ChroStru[] =
{
   { "DATEJOUR" , 'D',  8, 0 },
   { "NOMPALIER", 'C',  8, 0 },
   { "NBAPPELS" , 'N', 10, 0 },
   { "TPSCNX"   , 'N', 10, 0 },
   { 0,0,0,0 }
};
TAG4INFO      ChroIdx[] =
{
   { "CHRONOS", "DTOS(DATEJOUR)+NOMPALIER", 0, 0, 0 },
   { 0,0,0,0,0 }
};

CODE4         NeoServ;                 // Bases systme.
mtBYTE        JourSvg;                 // Date de la
mtBYTE        MoisSvg;                 // dernire sauvegarde
mtWORD        AnneeSvg;                // des stats.
typedef  struct  _tTmp                 // Utilise par
{                                      // les stats palier.
   mtWORD H;
   mtWORD M;
   mtWORD S;
} tTmp;
typedef  tTmp * mtPTRTTMP;
mtBOOL        bRBase;                  // Utilises pour la sauvegarde
mtUDWORD      TpRBase;                 // priodique des bases
mtWORD        wSaveBase;               // systmes.
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   UnCv      TabCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   StPalier  StatPalier[MAXPALIER];
extern   Ports     tPorts[MAXPORTS];
extern   mtPTRAPP  pAppli;             // Pointe sur la 1re appli.
extern   int       NbCnx;
extern   int       PointeCnx;
extern   int       TPointeCnx[12][24];
extern   float     ChargeA;
extern   float     ChargeM;
extern   mtDWORD   NbPassage[2];
extern   float     TpPassage[2];
extern   float     TmtA[2];
extern   float     TmtM[2];
extern   struct    dos_time_t HDerCnx;
extern   struct    dos_time_t HPreCnx;
extern   mtUDWORD  NbAppTot;
extern   mtBYTE    Jour;
extern   mtBYTE    Mois;
extern   mtWORD    Annee;
extern   mtBYTE    Heures;
extern   mtBYTE    Minutes;
extern   mtBYTE    Secondes;
extern   mtBOOL    bCreateDbSys;

extern   float      TimeToCent(struct dos_time_t *);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void       RAZSysPalier(void);
extern   void       RAZAppPalier(void);
extern   mtPTRSTAPP NewStApp(mtPTRAPP,mtBYTE);
extern   mtPTRSTAPP SearchStApp(mtPTRAPP,mtBYTE);
extern   void       MajStatPalier(mtPTRCV);
extern   void       WriteLogNeo(mtPTRBYTE,...);
extern   void       RAZTPointe(void);

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti une chaine du type "HHMMSS" en heure,minute,seconde,
// et les stocke dans la structure donne.
//
void STimeToStruc(mtPTRBYTE pcTime,struct dos_time_t *sTime)
{
   char cBuff[3];

   strncpy(cBuff,pcTime,2);
   sTime->hour   = atoi(cBuff);
   strncpy(cBuff,pcTime+2,2);
   sTime->minute = atoi(cBuff);
   strncpy(cBuff,pcTime+4,2);
   sTime->second = atoi(cBuff);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation des bases systmes.
//
void InitBaseSys(void)
{
   d4init(&NeoServ);
   NeoServ.exclusive     = 0;
   NeoServ.read_only     = 0;
   NeoServ.read_lock     = 0;
   NeoServ.lock_attempts = 0;
   NeoServ.open_error    = 0;
   NeoServ.off_error     = 1;
   JourSvg               = 0;
   MoisSvg               = 0;
   AnneeSvg              = 0;
   bRBase                = mtFALSE;
   TpRBase               = 0L;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsinitialise les bases systmes.
//
void EndBaseSys(void)
{
   d4init_undo(&NeoServ);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ouverture des bases systmes
// Renvoie VRAI si tout c'est bien pass.
//
mtBOOL OpenBaseSys(void)
{
   DATA4 *DbTmp;
   TAG4 *Idx;

   // Les chronos des applications.
   DbTmp = d4open(&NeoServ,"APPSTAT");
   if(! DbTmp)
   {
         WinMsgErr(": Impossible d'ouvrir la base","OpenBaseSys APPSTAT");
         return(mtFALSE);
   }
   // Slection de l'index maitre.
   Idx = d4tag_next(DbTmp,NULL);
   if(Idx)
      d4tag_select(DbTmp,Idx);
   // Les infos systmes des applications.
   DbTmp = d4open(&NeoServ,"APPSYS");
   if(! DbTmp)
   {
         WinMsgErr(": Impossible d'ouvrir la base","OpenBaseSys APPSYS");
         return(mtFALSE);
   }
   // Slection de l'index maitre.
   Idx = d4tag_next(DbTmp,NULL);
   if(Idx)
      d4tag_select(DbTmp,Idx);
   // Les chronomtres gnraux de NoServ.
//   if((access("NEOSTAT.DBF",F_OK)) == -1)
//   {
      // La base n'existe pas.
//      if(bCreateDbSys)
//      {
         // On cr la base
//         DbTmp = d4create(&NeoServ,"NEOSTAT",ChroStru,ChroIdx);
//         if(! DbTmp)
//         {
//            WinInfo("Code=%d",NeoServ.error_code);
//            WinMsgErr(": Impossible de crer la base","OpenBaseSys NEOSTAT");
//            return(mtFALSE);
//         }
//      }
//      else
//      {
//         WinMsgErr(": Impossible d'ouvrir la base","OpenBaseSys NEOSTAT");
//         return(mtFALSE);
//      }
//   }
//   else
//   {
      DbTmp = d4open(&NeoServ,"NEOSTAT");
      if(! DbTmp)
      {
         WinMsgErr(": Impossible d'ouvrir la base","OpenBaseSys NEOSTAT");
         return(mtFALSE);
      }
//   }
   // Slection de l'index maitre.
   Idx = d4tag_next(DbTmp,NULL);
   if(Idx)
      d4tag_select(DbTmp,Idx);
   // Les infos systmes de NoServ.
   DbTmp = d4open(&NeoServ,"NEOSYS");
   if(! DbTmp)
   {
         WinMsgErr(": Impossible d'ouvrir la base","OpenBaseSys NEOSYS");
         return(mtFALSE);
   }
   // Slection de l'index maitre.
   Idx = d4tag_next(DbTmp,NULL);
   if(Idx)
      d4tag_select(DbTmp,Idx);
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fermeture des bases systmes.
//
void CloseBaseSys(void)
{
   d4close_all(&NeoServ);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos systmes
//
void SauveInfoSys(void)
{
   DATA4 *DbTmp;
   char DBuff[580],BTmp[3];
   int DbError,i,j;
   FIELD4 *Champ;
   double dVal;

   // On slectionne la base systme.
   DbTmp = d4data(&NeoServ,"NEOSYS");
   if(DbTmp)
   {
      // On cherche le jour courant.
      sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
      DbError = d4seek(DbTmp,DBuff);
      if(DbError != r4success)
      {
         // Non trouve, on cr un nouvel enregistrement.
         DbError = d4append_blank(DbTmp);
      }
      if(DbError == r4success)
      {
         // Trouv ou pas d'erreur … l'append blank.
         // Mise … jour des infos.
         //
         Champ = d4field(DbTmp,"DATEJOUR");
         f4assign(Champ,DBuff);
         Champ = d4field(DbTmp,"CHMAX");
         dVal  = (double) ChargeM;
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"CNXMAX");
         dVal  = (double) PointeCnx;
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"CPMAX");
         dVal  = (double) TmtM[0];
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"CFMAX");
         dVal  = (double) TmtM[1];
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"NBAPPTOT");
         dVal  = (double) NbAppTot;
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"HPRECNX");
         sprintf(DBuff,"%02d%02d%02d",HPreCnx.hour,HPreCnx.minute,HPreCnx.second);
         f4assign(Champ,DBuff);
         Champ = d4field(DbTmp,"HDERCNX");
         sprintf(DBuff,"%02d%02d%02d",HDerCnx.hour,HDerCnx.minute,HDerCnx.second);
         f4assign(Champ,DBuff);
         for(i = 0;i<MAXPORTS;i++)
         {
            if(tPorts[i].Actif)
            {
               sprintf(DBuff,"P%1dNBPQIN",i+1);
               Champ = d4field(DbTmp,DBuff);
               dVal  = (double) tPorts[i].NbPaqLu;
               f4assign_double(Champ,dVal);
               sprintf(DBuff,"P%1dNBPQOUT",i+1);
               Champ = d4field(DbTmp,DBuff);
               dVal  = (double) tPorts[i].NbPaqOut;
               f4assign_double(Champ,dVal);
               sprintf(DBuff,"P%1dNBPQERR",i+1);
               Champ = d4field(DbTmp,DBuff);
               dVal  = (double) tPorts[i].NbPaqErr;
               f4assign_double(Champ,dVal);
            }
         }
         memset(DBuff,0,580);
         for(i=0;i<12;i++)
         {
            for(j=0;j<24;j++)
            {
               sprintf(BTmp,"%02d",TPointeCnx[i][j]);
               strcat(DBuff,BTmp);
            }
         }
         Champ = d4field(DbTmp,"POINTES");
         f4memo_assign(Champ,DBuff);
      }
      else
      {
         WinInfo("Base NEOSYS.DBF: Problme d'criture des infos au %s",DBuff);
         WriteLogNeo("Base NEOSYS.DBF: Problme d'criture des infos au %s",DBuff);
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base NEOSYS.DBF");
      WriteLogNeo("Impossible de slectionner la base NEOSYS.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des infos systmes.
//
void RecupInfoSys(void)
{
   DATA4 *DbTmp;
   char DBuff[580],*pBuff=DBuff,BTmp[3];
   int DbError,i,j,np,num;
   FIELD4 *Champ;

   // On slectionne la base systme.
   DbTmp = d4data(&NeoServ,"NEOSYS");
   if(DbTmp)
   {
      // On cherche le jour courant.
      sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
      DbError = d4seek(DbTmp,DBuff);
      if(DbError == r4success)
      {
         // Trouv.
         // Rcupration des infos.
         //
         Champ     = d4field(DbTmp,"CHMAX");
         ChargeM   = (float) f4double(Champ);
         Champ     = d4field(DbTmp,"CNXMAX");
         PointeCnx = f4int(Champ);
         Champ     = d4field(DbTmp,"CPMAX");
         TmtM[0]   = (float) f4double(Champ);
         Champ     = d4field(DbTmp,"CFMAX");
         TmtM[1]   = (float) f4double(Champ);
         Champ     = d4field(DbTmp,"NBAPPTOT");
         NbAppTot  = (mtUDWORD) f4double(Champ);
         Champ     = d4field(DbTmp,"HPRECNX");
         pBuff     = f4str(Champ);
         STimeToStruc(pBuff,&HPreCnx);
         Champ     = d4field(DbTmp,"HDERCNX");
         pBuff     = f4str(Champ);
         STimeToStruc(pBuff,&HDerCnx);
         for(i = 0;i<MAXPORTS;i++)
         {
            if(tPorts[i].Actif)
            {
               sprintf(DBuff,"P%1dNBPQIN",i+1);
               Champ             = d4field(DbTmp,DBuff);
               tPorts[i].NbPaqLu = (mtUDWORD) f4double(Champ);
               sprintf(DBuff,"P%1dNBPQOUT",i+1);
               Champ              = d4field(DbTmp,DBuff);
               tPorts[i].NbPaqOut = (mtUDWORD) f4double(Champ);
               sprintf(DBuff,"P%1dNBPQERR",i+1);
               Champ              = d4field(DbTmp,DBuff);
               tPorts[i].NbPaqErr = (mtUDWORD) f4double(Champ);
            }
         }
         Champ = d4field(DbTmp,"POINTES");
         pBuff = f4memo_str(Champ);
         np    = 0;
         if(strlen(pBuff) >= 576)
         {
            for(i=0;i<12;i++)
            {
               for(j=0;j<24;j++)
               {
                  strncpy(BTmp,pBuff+np,2);
                  np += 2;
                  num = atoi(BTmp);
                  TPointeCnx[i][j] = num;
               }
            }
         }
      }
      else
      {
         WinInfo("Base NEOSYS.DBF: Pas d'infos au %s",DBuff);
         WriteLogNeo("Base NEOSYS.DBF: Pas d'infos au %s",DBuff);
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base NEOSYS.DBF");
      WriteLogNeo("Impossible de slectionner la base NEOSYS.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// RAZ des infos systmes.
//
void RAZInfoSys(void)
{
   int i;

   ChargeA      = (float) 0.00;
   ChargeM      = (float) 0.00;
   TmtA[0]      = (float) 0.00;
   TmtA[1]      = (float) 0.00;
   TmtM[0]      = (float) 0.00;
   TmtM[1]      = (float) 0.00;
   NbPassage[0] = 0;
   NbPassage[1] = 0;
   TpPassage[0] = (float) 0.00;
   TpPassage[1] = (float) 0.00;
   NbAppTot     = 0;
   PointeCnx    = 0;
   for(i = 0;i<MAXPORTS;i++)
   {
      if(tPorts[i].Actif)
      {
         tPorts[i].NbPaqLu  = 0;
         tPorts[i].NbPaqOut = 0;
         tPorts[i].NbPaqErr = 0;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// RAZ des infos des applications.
//
void RAZAppInfo(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      p->NbCnx     = 0;
      p->NbAppTot  = 0;
      p->PointeCnx = 0;
      p            = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// RAZ des heures de connexion des applications.
//
void RAZHeureApp(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      _dos_gettime(&p->HPreCnx);
      _dos_gettime(&p->HDerCnx);
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des paliers gnraux.
//
void SauveSysPaliers(void)
{
   DATA4 *DbTmp;
   char DBuff[80];
   int DbError,i;
   FIELD4 *Champ;
   double dVal;

   // On slectionne la base systme.
   DbTmp = d4data(&NeoServ,"NEOSTAT");
   if(DbTmp)
   {
      // Balayage des paliers
      for(i=0;i<MAXPALIER;i++)
      {
         // On cherche le jour courant.
         sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
         strcat(DBuff,StatPalier[i].Nom);
         DbError = d4seek(DbTmp,DBuff);
         if(DbError != r4success)
         {
            // Non trouve, on cr un nouvel enregistrement.
            DbError = d4append_blank(DbTmp);
         }
         if(DbError == r4success)
         {
            // Trouv ou pas d'erreur … l'append blank.
            // Mise … jour des infos.
            //
            sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
            Champ = d4field(DbTmp,"DATEJOUR");
            f4assign(Champ,DBuff);
            Champ = d4field(DbTmp,"NOMPALIER");
            f4assign(Champ,StatPalier[i].Nom);
            Champ = d4field(DbTmp,"NBAPPELS");
            dVal  = (double) StatPalier[i].NbAppel;
            f4assign_double(Champ,dVal);
            Champ = d4field(DbTmp,"TPSCNX");
            dVal  = (double) StatPalier[i].TpsCnx;
            f4assign_double(Champ,dVal);
         }
         else
         {
            WinInfo("Base NEOSTAT.DBF: Problme d'criture des stats %s",DBuff);
            WriteLogNeo("Base NEOSTAT.DBF: Problme d'criture des stats %s",DBuff);
         }
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base NEOSTAT.DBF");
      WriteLogNeo("Impossible de slectionner la base NEOSTAT.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des paliers gnraux.
//
void RecupSysPaliers(void)
{
   DATA4 *DbTmp;
   char DBuff[80];
   int DbError,i;
   FIELD4 *Champ;
   double dVal;

   // On slectionne la base systme.
   DbTmp = d4data(&NeoServ,"NEOSTAT");
   if(DbTmp)
   {
      // Balayage des paliers
      for(i=0;i<MAXPALIER;i++)
      {
         // On cherche le jour courant.
         sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
         strcat(DBuff,StatPalier[i].Nom);
         DbError = d4seek(DbTmp,DBuff);
         if(DbError == r4success)
         {
            // Trouv.
            // Rcupration des infos.
            //
            Champ                 = d4field(DbTmp,"NBAPPELS");
            StatPalier[i].NbAppel = (mtUDWORD) f4double(Champ);
            Champ                 = d4field(DbTmp,"TPSCNX");
            StatPalier[i].TpsCnx  = (mtUDWORD) f4double(Champ);
         }
         else
         {
            WinInfo("Base NEOSTAT.DBF: Pas de stats sur %s",DBuff);
            WriteLogNeo("Base NEOSTAT.DBF: Pas de stats sur %s",DBuff);
         }
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base NEOSTAT.DBF");
      WriteLogNeo("Impossible de slectionner la base NEOSTAT.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos des paliers d'une application.
//
void SauveAppStat(mtPTRAPP pApp)
{
   DATA4 *DbTmp;
   char DBuff[80];
   int DbError;
   FIELD4 *Champ;
   double dVal;
   mtPTRSTAPP s;

   // On slectionne la base des paliers des applications.
   DbTmp = d4data(&NeoServ,"APPSTAT");
   if(DbTmp)
   {
      s = pApp->pStat;
      while(s)
      {
         // On cherche le jour courant pour l'application et le palier courant.
         sprintf(DBuff,"%-10s%04d%02d%02d%02d",pApp->Nom,Annee,Mois,Jour,s->Palier);
         DbError = d4seek(DbTmp,DBuff);
         if(DbError != r4success)
         {
            // Non trouve, on cr un nouvel enregistrement.
            DbError = d4append_blank(DbTmp);
         }
         if(DbError == r4success)
         {
            // Trouv ou pas d'erreur … l'append blank.
            // Mise … jour des infos.
            //
            Champ = d4field(DbTmp,"NOMAPP");
            f4assign(Champ,pApp->Nom);
            sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
            Champ = d4field(DbTmp,"DATEJOUR");
            f4assign(Champ,DBuff);
            sprintf(DBuff,"%02d",s->Palier);
            Champ = d4field(DbTmp,"PALIER");
            f4assign(Champ,DBuff);
            Champ = d4field(DbTmp,"NBAPPELS");
            dVal  = (double) s->NbAppel;
            f4assign_double(Champ,dVal);
            Champ = d4field(DbTmp,"TPSCNX");
            dVal  = (double) s->TpsCnx;
            f4assign_double(Champ,dVal);
         }
         else
         {
            WinInfo("Base APPSTAT.DBF: Problme d'criture des stats %s",DBuff);
            WriteLogNeo("Base APPSTAT.DBF: Problme d'criture des stats %s",DBuff);
         }
         s = s->pNextSt;
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base APPSTAT.DBF");
      WriteLogNeo("Impossible de slectionner la base APPSTAT.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos des paliers de toutes les applications.
//
void SauveAllAppStat(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      if(p->Active)
         SauveAppStat(p);
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des infos des paliers d'une application.
//
void RecupAppStat(mtPTRAPP pApp)
{
   DATA4 *DbTmp;
   char DBuff[80],cClef[80];
   int DbError;
   FIELD4 *Champ;
   double dVal;
   mtPTRSTAPP s;
   mtPTRBYTE cPalier,cNomApp,cDate;

   DbTmp = d4data(&NeoServ,"APPSTAT");
   if(DbTmp)
   {
      // On cherche le jour courant pour l'application.
      sprintf(DBuff,"%-10s%04d%02d%02d",pApp->Nom,Annee,Mois,Jour);
      DbError = d4seek(DbTmp,DBuff);
      while(DbError == r4success)
      {
         Champ   = d4field(DbTmp,"PALIER");
         cPalier = f4str(Champ);
         s       = SearchStApp(pApp,(mtBYTE) atoi(cPalier));
         if(! s)
            s = NewStApp(pApp,(mtBYTE) atoi(cPalier));
         if(s)
         {
            Champ       = d4field(DbTmp,"NBAPPELS");
            s->NbAppel += (mtUDWORD) f4double(Champ);
            Champ       = d4field(DbTmp,"TPSCNX");
            s->TpsCnx  += (mtUDWORD) f4double(Champ);
         }
         // Enregistrement suivant.
         DbError = d4skip(DbTmp,1L);
         if(DbError != r4success)
            break;
         // On vrifie que l'enregistrement correspond toujours.
         Champ   = d4field(DbTmp,"NOMAPP");
         cNomApp = f4str(Champ);
         sprintf(cClef,"%-10s",cNomApp);
         Champ   = d4field(DbTmp,"DATEJOUR");
         cDate   = f4str(Champ);
         strcat(cClef,cDate);
         if(strcmp(DBuff,cClef) != 0)
            break;
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base APPSTAT.DBF");
      WriteLogNeo("Impossible de slectionner la base APPSTAT.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos d'une application.
//
void SauveAppSysInfo(mtPTRAPP pApp)
{
   DATA4 *DbTmp;
   char DBuff[80];
   int DbError;
   FIELD4 *Champ;
   double dVal;

   // On slectionne la base des infos des applications.
   DbTmp = d4data(&NeoServ,"APPSYS");
   if(DbTmp)
   {
      // On cherche le jour courant pour l'application et le palier courant.
      sprintf(DBuff,"%-10s%04d%02d%02d",pApp->Nom,Annee,Mois,Jour);
      DbError = d4seek(DbTmp,DBuff);
      if(DbError != r4success)
      {
         // Non trouve, on cr un nouvel enregistrement.
         DbError = d4append_blank(DbTmp);
      }
      if(DbError == r4success)
      {
         // Trouv ou pas d'erreur … l'append blank.
         // Mise … jour des infos.
         //
         Champ = d4field(DbTmp,"NOMAPP");
         f4assign(Champ,pApp->Nom);
         sprintf(DBuff,"%04d%02d%02d",Annee,Mois,Jour);
         Champ = d4field(DbTmp,"DATEJOUR");
         f4assign(Champ,DBuff);
         Champ = d4field(DbTmp,"NBAPPTOT");
         dVal  = (double) pApp->NbAppTot;
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"CNXMAX");
         dVal  = (double) pApp->PointeCnx;
         f4assign_double(Champ,dVal);
         Champ = d4field(DbTmp,"HPRECNX");
         sprintf(DBuff,"%02d%02d%02d",pApp->HPreCnx.hour,pApp->HPreCnx.minute,pApp->HPreCnx.second);
         f4assign(Champ,DBuff);
         Champ = d4field(DbTmp,"HDERCNX");
         sprintf(DBuff,"%02d%02d%02d",pApp->HDerCnx.hour,pApp->HDerCnx.minute,pApp->HDerCnx.second);
         f4assign(Champ,DBuff);
      }
      else
      {
         WinInfo("Base APPSYS.DBF: Problme d'criture des infos %s",DBuff);
         WriteLogNeo("Base APPSYS.DBF: Problme d'criture des infos %s",DBuff);
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base APPSYS.DBF");
      WriteLogNeo("Impossible de slectionner la base APPSYS.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des infos d'une application.
//
void RecupAppSysInfo(mtPTRAPP pApp)
{
   DATA4 *DbTmp;
   char DBuff[80],*pBuff=DBuff;
   int DbError;
   FIELD4 *Champ;

   DbTmp = d4data(&NeoServ,"APPSYS");
   if(DbTmp)
   {
      // On cherche le jour courant pour l'application.
      sprintf(DBuff,"%-10s%04d%02d%02d",pApp->Nom,Annee,Mois,Jour);
      DbError = d4seek(DbTmp,DBuff);
      if(DbError == r4success)
      {
         // Trouv.
         // Rcupration des infos.
         //
         Champ           = d4field(DbTmp,"NBAPPTOT");
         pApp->NbAppTot  = (mtUDWORD) f4double(Champ);
         Champ           = d4field(DbTmp,"CNXMAX");
         pApp->PointeCnx = f4int(Champ);
         Champ           = d4field(DbTmp,"HPRECNX");
         pBuff           = f4str(Champ);
         STimeToStruc(pBuff,&pApp->HPreCnx);
         Champ           = d4field(DbTmp,"HDERCNX");
         pBuff           = f4str(Champ);
         STimeToStruc(pBuff,&pApp->HDerCnx);
      }
      else
      {
         WinInfo("Base APPSYS.DBF: Pas d'infos pour %s",DBuff);
         WriteLogNeo("Base APPSYS.DBF: Pas d'infos pour %s",DBuff);
      }
   }
   else
   {
      WinInfo("Impossible de slectionner la base APPSYS.DBF");
      WriteLogNeo("Impossible de slectionner la base APPSYS.DBF");
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos systmes de toutes les applications prsentes.
//
void SauveAllAppSysInfo(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      if(p->Active)
         SauveAppSysInfo(p);
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde des infos … minuit.
//
void IlEstMinuit(void)
{
   int i;

   if((Jour != JourSvg) && (Mois != MoisSvg) && (Annee != AnneeSvg))
   {
      // Il va tre minuit, on sauve les stats et on
      // remet … zro.
      //
      // Les infos systmes.
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d SauveInfoSys",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d SauveInfoSys",Jour,Mois,Annee,Heures,Minutes,Secondes);
      SauveInfoSys();
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d RAZInfoSys",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d RAZInfoSys",Jour,Mois,Annee,Heures,Minutes,Secondes);
      RAZInfoSys();
      RAZTPointe();
      // Les paliers.
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d SauveSysPaliers",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d SauveSysPaliers",Jour,Mois,Annee,Heures,Minutes,Secondes);
      SauveSysPaliers();
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d RAZSysPaliers",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d RAZSysPaliers",Jour,Mois,Annee,Heures,Minutes,Secondes);
      RAZSysPalier();
      // Les paliers des applications
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d SauveAllAppStat",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d SauveAllAppStat",Jour,Mois,Annee,Heures,Minutes,Secondes);
      SauveAllAppStat();
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d RAZAppPalier",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d RAZAppPalier",Jour,Mois,Annee,Heures,Minutes,Secondes);
      RAZAppPalier();
      // Les infos des applications
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d SauveAllAppSysInfo",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d SauveAllAppSysInfo",Jour,Mois,Annee,Heures,Minutes,Secondes);
      SauveAllAppSysInfo();
      //
      // On met … jour les infos des connects actifs.
      //
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d RAZAppInfo",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d RAZAppInfo",Jour,Mois,Annee,Heures,Minutes,Secondes);
      RAZAppInfo();
      NbCnx = 0;
      for(i=0;i<TOTALCV;i++)
      {
         if(TabCv[i].Actif)
         {
            TabCv[i].pApp->NbCnx++;
            if(TabCv[i].pApp->NbCnx > TabCv[i].pApp->PointeCnx)
               TabCv[i].pApp->PointeCnx = TabCv[i].pApp->NbCnx;
            // if(! TabCv[i].pApp->NbAppTot)
            //   _dos_gettime(&TabCv[i].pApp->HPreCnx);
            //_dos_gettime(&TabCv[i].pApp->HDerCnx);
            TabCv[i].pApp->NbAppTot++;
            NbCnx++;
            NbAppTot++;
         }
      }
      if(! NbCnx)
         RAZHeureApp();
      JourSvg        = Jour;
      MoisSvg        = Mois;
      AnneeSvg       = Annee;
      HPreCnx.hour   = 0;
      HPreCnx.minute = 0;
      HPreCnx.second = 0;
      HDerCnx.hour   = 0;
      HDerCnx.minute = 0;
      HDerCnx.second = 0;
      WinInfo("%02d/%02d/%04d %02d:%02d:%02d FIN",Jour,Mois,Annee,Heures,Minutes,Secondes);
      WriteLogNeo("%02d/%02d/%04d %02d:%02d:%02d FIN",Jour,Mois,Annee,Heures,Minutes,Secondes);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Sauvegarde priodique des infos systmes et des chronos
//
void RefreshBaseSys(void)
{
   char VBuff[81],cTmp[25];
   struct dos_time_t tBs;
   mtUDWORD wBs;

   if(bRBase)
   {
      memset(VBuff,0,81);
      sprintf(VBuff,"%02d:%02d:%02d, RefreshBaseSys",Heures,Minutes,Secondes);
      SauveInfoSys();
      SauveAllAppSysInfo();
      SauveSysPaliers();
      SauveAllAppStat();
      sprintf(cTmp,", Termin … %02d:%02d:%02d",Heures,Minutes,Secondes);
      strcat(VBuff,cTmp);
      bRBase  = mtFALSE;
      TpRBase = 0L;
      WinInfo("%s",VBuff);
      WriteLogNeo("%s",VBuff);
   }
   else
   {
      if((wSaveBase) && (! bRBase))
      {
         _dos_gettime(&tBs);
         tBs.hsecond = 0;
         wBs         = TimeToCent(&tBs)/100;
         if(! TpRBase)
            // On initialise le compteur.
            TpRBase = wBs;
         else
         {
            // On sauve les bases toutes les wSaveBase mn.
            if((wBs-TpRBase) >= wSaveBase)
               bRBase = mtTRUE;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

