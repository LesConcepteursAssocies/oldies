// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณCLAVIER.C    ณ                                                             ณ
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
// ณDernire modification : 25/09/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des vnements clavier et affichage des informations systme       ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <ctype.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <disp.h>
#include <direct.h>
#include <float.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "keyboard.h"
#include "minitel.h"
#include "ports.h"

int      nErrCmd=0;                    // Code de la dernire erreur commande.
int      mtTouche;
mtBYTE   BuffCmd[80];                  // Commande courante.
mtBYTE   History[MAX_HISTO][80];       // Svg. des MAX_HISTO dernires commandes.
mtBYTE   pHisto=0;                     // Indice dans History.
mtBYTE   CBCmd=0;                      // Position courante du curseur.
mtBOOL   bInsert=mtTRUE;               // Mode INSERT par dfaut.
mtBYTE   pFCmd;                        // Gr par SearchTab;
int      pPageCv=0;                    // Gr par WinAffCnx.
int      pNCvAff=0;                    // Gr par WinAffCv.
int      pPageSGen=0;                  // Gr par WinStatGen.
int      pPageApp=0;                   // Gr par WinAllApp.
int      nAppPage=15;                  // Gr par WinAllApp.
int      nbAffApp=0;                   // Gr par WinAllApp.
mtBOOL   bPageApp=mtTRUE;              // Gr par WinAllApp.
mtPTRAPP pAffApp=NULL;                 // Gr par WinAffApp.
typedef  struct  _tTmp                 // Utilise par
{                                      // les stats palier.
   mtWORD H;
   mtWORD M;
   mtWORD S;
} tTmp;
typedef  tTmp * mtPTRTTMP;
mtBOOL   bScrSave=mtFALSE;             // Pour la sauvegarde de l'cran.
mtUDWORD TpScrSave=0L;
mtWORD   wSaveScreen;
mtBOOL   bRScreen=mtTRUE;              // Pour l'affichage des info. systmes.
mtUDWORD TpRScreen=0L;
// Variables du mode TRACE.
mtPTRCV  pCvTrace=NULL;                // CV En cours de tracage.
mtBOOL   bAffTrace=mtFALSE;            // FAUX=gle l'affichage
mtWORD   wPasTrace=1;                  // Pas de trace.
mtWORD   wNbInst=0;                    // Nbre d'instruction excutes depuis le dernier pas.
mtBYTE   LDebProc,CDebProc;            // Coordonnes de
mtBYTE   LFinProc,CFinProc;            // la fentre d'affichage des procdures.
mtBYTE   LDebPile,CDebPile;            // Coordonnes de
mtBYTE   LFinPile,CFinPile;            // la fentre d'affichage de la pile.
mtBYTE   LDebVar,CDebVar;              // Coordonnes de
mtBYTE   LFinVar,CFinVar;              // la fentre d'affichage des variables.
mtUSHORT CoulFProc,CoulFPile,CoulFVar; // Couleurs des fentres.
int      LAffVar;                      // Ligne courante d'affichage des variables.
int      NbAllLib;                     // Nombre total de modules prsents.
int      NbAllApp;                     //    "     "   d'applications prsentes.
mtUWORD  SizeAllCode;                  // Total des blocs de code prsents.
mtUWORD  SizeAllPData;                 // Total des donnes publiques prsentes.
mtUWORD  SizeAllLData;                 // Total des donnes locales prsentes.

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   char       *cMOI;
extern   char       cBann[81];
extern   mtBOOL     bTracePaq;
extern   char       *pFicPAQ;
extern   mtBOOL     bTraceNeo;
extern   char       *pFicNEO;
extern   unsigned   char _osmajor;
extern   unsigned   char _osminor;
extern   mtBOOL     bArretAuto;
extern   mtBOOL     bBipOnCnx;
extern   mtBOOL     bAutoCNX;
extern   char      *pAppAutoCNX;
extern   char      *NomAppAutoCNX;
extern   mtBOOL     bCreateDbSys;
extern   char      *PathAPPS;
extern   UnCv       TabCv[TOTALCV];    // Dfini Dans le prog. principal.
extern   UnCv       LTask[TOTALCV];    // Dfini Dans le prog. principal.
extern   UnCv       OLCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   StPalier   StatPalier[MAXPALIER];
extern   Ports      tPorts[MAXPORTS];
extern   mtBOOL     bStatGen;
extern   mtBOOL     bAffCnx;
extern   mtBOOL     bInfoGen;
extern   mtBOOL     bAffCv;
extern   mtBOOL     bAffApp;
extern   mtBOOL     bAllApp;
extern   mtBOOL     bAffHlp;
extern   mtBOOL     bAffCom;
extern   mtBOOL     bInDebug;
extern   mtBOOL     bNextStep;
extern   mtBOOL     bAffInfoSys;
extern   mtBOOL     bAffTPCnx;
extern   mtUDWORD   RAMDispo;
extern   int        NbCnx;
extern   int        PointeCnx;
extern   int        TPointeCnx[12][24];
extern   float      ChargeA;
extern   float      ChargeM;
extern   mtDWORD    NbPassage[2];
extern   float      TpPassage[2];
extern   struct     dos_time_t DebTrait[2];
extern   struct     dos_time_t FinTrait[2];
extern   float      TmtA[2];
extern   float      TmtM[2];
extern   struct     dos_time_t HDerCnx;
extern   struct     dos_time_t HPreCnx;
extern   mtUDWORD   NbAppTot;
extern   mtBOOL     bPaquet;
extern   mtBOOL     mtFin;
extern   mtBYTE     Heures;
extern   mtBYTE     Minutes;
extern   mtBYTE     Secondes;
extern   mtBYTE     Jour;
extern   mtBYTE     Mois;
extern   mtWORD     Annee;
extern   struct     dos_time_t HFinServ;
extern   mtBYTE     LigDH;              // Position de la date
extern   mtBYTE     ColDH;              // & de l'heure.
extern   mtUSHORT   CoulDH;             // Couleur         "
extern   mtUSHORT   CoulInf;            // Couleur des infos. gn.
extern   mtUSHORT   CoulSys;            //   "      "    "    systmes.
extern   mtBYTE     LigCmd;             // Position des commandes.
extern   mtBYTE     ColCmd;
extern   mtUSHORT   CoulCmd;            // Couleur         "
extern   mtBYTE     LDebCmd;            // Position
extern   mtBYTE     CDebCmd;            // de la fentre
extern   mtBYTE     LFinCmd;            // des
extern   mtBYTE     CFinCmd;            // commandes.
extern   mtUSHORT   CoulCur;            // Couleur du curseur.
extern   mtBYTE     LigErr;             // Position des message d'erreur.
extern   mtBYTE     ColErr;
extern   mtUSHORT   CoulErr;            // Couleur         "
extern   mtUSHORT   CoulBdx;            // Couleur des bandeaux d'affichage.
extern   mtPTRAPP   pAppli;             // Pointe sur la 1re appli.
extern   mtWORD     wSaveBase;          // Temps entre chaque sauvegarde des bases systmes.
extern   mtWORD     wVerCNX;            // Temps entre chaque vrification des connects.
extern   pPDICO     pGlobalDic;         // Pointe sur le dictionnaire global du systme.
extern   mtUWORD    uGDico;             // Nombre de symboles dans pGlobalDic.
extern   void       SWVRingBell(void);
extern   void       SWVPutChar(mtBYTE,mtBYTE,mtBYTE,mtUSHORT);
extern   void       SWVScroll(mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE);
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   void       VPrint(mtBYTE,mtBYTE,mtPTRBYTE,mtUSHORT);
extern   void       WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void       WinSysInfo(void);
extern   void       ClsWinInf(mtBOOL);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       ScreenOff(void);
extern   void       ScreenOn(void);
extern   void       Send0(mtPTRBYTE,mtPTRCV);
extern   void       IniPalier(mtPTRBYTE);
extern   void       IniCouleur(mtPTRBYTE);
extern   void       IniSystem(mtPTRBYTE);
extern   mtBOOL     LoadApp(mtPTRBYTE);
extern   void       UnLoadApp(mtPTRBYTE);
extern   mtPTRAPP   SearchApp(mtPTRBYTE,mtBYTE);
extern   void       DeconnecteCv(mtPTRCV);
extern   void       InitOnLibCv(mtPTRCV);
extern   void       LoadAutoApp(void);
extern   mtBOOL     OpenLogPaq(void);
extern   void       CloseLogPaq(void);
extern   mtBOOL     OpenLogNeo(void);
extern   void       CloseLogNeo(void);
extern   void       WriteLogNeo(mtPTRBYTE,...);
extern   mtPTRDICO     SearchDico(pPDICO,mtUWORD,mtPTRBYTE,mtPTRUWORD);
extern   mtPTRLISTDICO SearchListDico(pPLISTDICO,mtUWORD,mtPTRAPP);
extern   mtBOOL        LOCConnect(mtPTRBYTE,int);
extern   mtPTRBYTE     cDecrypt(mtPTRBYTE,mtWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
mtBOOL SearchTab(mtPTRBYTE [],mtPTRBYTE);
void   ProcessCmd(void);
void   MajTPointe(void);
void   MajSysInfo(void);
void   CmQUIT(void);
void   CmCLS(void);
void   CmVIEW(void);
void   CmSEND(void);
void   CmSET(void);
void   CmLOAD(void);
void   CmUNLOAD(void);
void   CmLIB(void);
void   CmTRACECV(void);
void   CmUNTRACECV(void);
void   CmDEBUGSTEP(void);
void   CmCONNECT(void);
void   CmHELP(void);
void   CmVStatGen(void);
void   CmVAffCnx(void);
void   CmVAffCv(void);
void   CmVAppli(void);
void   CmVVar(void);
void   CmVHisto(void);
void   CmSPalier(void);
void   CmSColor(void);
void   CmSSystem(void);
void   CmVPorts(void);
void   CmVAffInfSys(void);
void   CmVAffTPCnx(void);
void   WinStatGen(void);
void   WinAffCnx(void);
void   WinAffCv(void);
void   WinAffApp(void);
void   WinAllApp(void);
void   WinAffCom(void);
void   WinAffTraceCv(void);
void   WinAffInfSys(void);
void   WinAffTPCnx(void);
float  TimeToCent(struct dos_time_t *);
void   SecToTime(mtUDWORD,mtPTRTTMP);
//
// Les commandes reconnues par le systme
//
static mtPTRBYTE SysCmd[] =
{
   "QUIT",                   // Fin du programme.
   "CLS",                    // Efface la fentre d'info gn.
   "VIEW",                   // Visualisation d'infos spcifiques.
   "SEND",                   // Envoi d'un message en ligne 0.
   "SET",                    // Modif. d'une var. systme.
   "LOAD",                   // Compilation d'une application.
   "UNLOAD",                 // Libration d'une application.
   "LCX",                    // Libration d'un cv, de tous les cv, des cv d'une appli.
   "DEBUG",                  // Trace de l'excution d'un cv.
   "UNDEBUG",                // Arret de la trace de l'excution d'un cv.
   "DEBUGSTEP",              // Modification du pas de trace.
   "CONNECT",                // Connexion du port local … une application.
   "HELP",                   // Rappel des commandes clavier.
   NULL                      // Marqueur de fin du tableau.
};
//
// Les fonctions correspondant aux commandes
//
static void (*FctCmd[]) (void) =
{
   CmQUIT,
   CmCLS,
   CmVIEW,
   CmSEND,
   CmSET,
   CmLOAD,
   CmUNLOAD,
   CmLIB,
   CmTRACECV,
   CmUNTRACECV,
   CmDEBUGSTEP,
   CmCONNECT,
   CmHELP
};
//
// Les diverses options de la commande VIEW
//
static mtPTRBYTE OptView[] =
{
   "STAT",                   // Visu. des stats gnrales.
   "CNX",                    // Etat des connexions.
   "CV",                     // Visu. des infos. d'un cv.
   "APP",                    // Visu. des applications prsentes.
   "PORTS",                  // Visu. des ports de com.
   "SYSINFO",                // Visu. des infos systmes.
   "VAR",                    // Visu. d'une variable (mode trace).
   "HISTORY",                // Visu. de l'historique des commandes clavier.
   "POINTES",                // Visu. des pointes journalires.
   NULL
};
//
// Les fonctions associes
//
static   void (*FctView[]) (void) =
{
   CmVStatGen,
   CmVAffCnx,
   CmVAffCv,
   CmVAppli,
   CmVPorts,
   CmVAffInfSys,
   CmVVar,
   CmVHisto,
   CmVAffTPCnx
};
//
// Les diverses options de la commande SET
//
static mtPTRBYTE OptSet[] =
{
   "PALIER",
   "COLOR",
   "SYSTEM",
   NULL
};
//
// Les fonctions associes
//
static void (*FctSet[]) (void) =
{
   CmSPalier,
   CmSColor,
   CmSSystem
};
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Convertit un temps (HH:MM:SS.CC) en centimes de seconde.
//
float TimeToCent(struct dos_time_t *Ti)
{
   float cRet;

   cRet  = (float) Ti->hour * (float) 360000;
   cRet += (float) Ti->minute * (float) 6000;
   cRet += (float) Ti->second * (float) 100;
   cRet += (float) Ti->hsecond;
   return(cRet);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Convertit un nombre de seconde en temps (HH:MM:SS).
//
void SecToTime(mtUDWORD NbSec,mtPTRTTMP Ti)
{
   mtUDWORD sTmp;

   Ti->H = (NbSec/3600);
   sTmp  = (Ti->H*3600);
   Ti->M = ((NbSec-sTmp)/60);
   sTmp += (Ti->M*60);
   Ti->S = (NbSec-sTmp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Calcul des T.M.T
//
void CalcTMT(void)
{
   float tTrait,TDeb,TFin;

   if(NbCnx)
   {
      // Calcul du Temps Moyen de Traitement du
      // processus principal (excution du code des cv).
      TDeb   = TimeToCent(&DebTrait[0]);
      TFin   = TimeToCent(&FinTrait[0]);
      if(TFin<TDeb)
         // a cheval sur 2 jours.
         TFin += (float) 8640000;
      tTrait = TFin-TDeb;
      TpPassage[0] += tTrait;
      if(TpPassage[0])
      {
         TmtA[0] = (TpPassage[0]*10)/NbPassage[0];
         if(TmtA[0] > TmtM[0])
         {
            TmtM[0] = TmtA[0];
            if(TmtM[0]>=100.0)
               TmtM[0] = 99.9;
         }
      }
      // Calcul du Temps Moyen de Traitement de
      // TacheDeFond().
      TDeb   = TimeToCent(&DebTrait[1]);
      TFin   = TimeToCent(&FinTrait[1]);
      if(TFin<TDeb)
         // a cheval sur 2 jours.
         TFin += (float) 8640000;
      tTrait = TFin-TDeb;
      TpPassage[1] += tTrait;
      if(TpPassage[1])
      {
         TmtA[1] = (TpPassage[1]*10)/NbPassage[1];
         if(TmtA[1] > TmtM[1])
         {
            TmtM[1] = TmtA[1];
            if(TmtM[1]>=100.0)
               TmtM[1] = 99.9;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Mise … jour du tableau des pointes journalires.
//
void MajTPointe(void)
{
   struct dos_time_t tTm;
   int Lig,Col;
   div_t res;

   _dos_gettime(&tTm);
//   res = div((int) tTm.minute,5);
   res = div((int) Minutes,5);
   Lig = res.quot;
//   Col = (int) tTm.hour;
   Col = (int) Heures;
   if(NbCnx > TPointeCnx[Lig][Col])
      TPointeCnx[Lig][Col] = PointeCnx;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Raffraichissement des affichages
// (toutes les 5 secondes)
//
void RefreshScreen(void)
{
   struct dos_time_t tSc;
   mtUDWORD wSc;

   if((bRScreen) && (! bScrSave))
   {
      MajSysInfo();
      WinSysInfo();
      WinAffCnx();
      WinStatGen();
      WinAffCv();
      WinAffApp();
      WinAllApp();
      WinAffCom();
      WinAffInfSys();
      WinAffTPCnx();
      bRScreen  = mtFALSE;
      TpRScreen = 0L;
   }
   else
   {
      _dos_gettime(&tSc);
      tSc.hsecond = 0;
      wSc         = TimeToCent(&tSc)/100;
      if(! TpRScreen)
         // On initialise le compteur.
         TpRScreen = wSc;
      else
      {
         // On raffraichit l'cran toutes les 5 secondes.
         if((wSc-TpRScreen) >= 5)
            bRScreen = mtTRUE;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Mise … jour des informations systme.
//
void MajSysInfo()
{
   int nbcvc=TOTALCV;

   // Mise … jour de la pointe.
   if(NbCnx > PointeCnx)
      PointeCnx = NbCnx;
   // Mise … jour de la charge.
   ChargeA = (float) (((float)NbCnx/(float)nbcvc)*(float) 100);
   if(ChargeA > ChargeM)
      ChargeM = ChargeA;
   RAMDispo = (mtUDWORD) _x386_coreleft();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de l'ventuelle ligne de commande.
//
void ProcessCmdLine(int nargc,char **targv)
{
   int i;

   if(nargc > 1)
   {
      for(i=1;i<nargc;i++)
      {
         if((strlen(BuffCmd)+strlen(targv[i])) >= 80)
            break;
         strcat(BuffCmd,targv[i]);
         strcat(BuffCmd," ");
      }
      // On enlve le dernier espace.
      BuffCmd[strlen(BuffCmd)-1] = 0;
      VPrint(LigCmd,ColCmd,BuffCmd,CoulCmd);
      LigCmd++;
      strcpy(History[pHisto],BuffCmd);
      pHisto++;
      if(pHisto > (MAX_HISTO-1))
         pHisto = 0;
      // Curseur … ON.
      SWVPutChar(LigCmd,ColCmd,' ',CoulCur);
      // Analyse et interprtation de la ligne de commande.
      ProcessCmd();
      memset(BuffCmd,0,80);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution d'une commande clavier par un connect.
//
void ProcessKbdCmd(mtPTRBYTE lCmd)
{
   if(strlen(lCmd))
   {
      nErrCmd = 0;
      memset(BuffCmd,0,80);
      strncpy(BuffCmd,lCmd,79);
      VPrint(LigCmd,ColCmd,BuffCmd,CoulCmd);
      LigCmd++;
      if(LigCmd == LFinCmd+1)
      {
         SWVScroll(SWSCROLLUP,LDebCmd,CDebCmd,LFinCmd,CFinCmd,1,CoulCmd);
         LigCmd = LFinCmd;
      }
      strcpy(History[pHisto],BuffCmd);
      pHisto++;
      if(pHisto > (MAX_HISTO-1))
         pHisto = 0;
      // Curseur … ON.
      SWVPutChar(LigCmd,ColCmd,' ',CoulCur);
      // Analyse et interprtation de la ligne de commande.
      ProcessCmd();
      // On raffraichit l'cran.
      bRScreen = mtTRUE;
      CBCmd = 0;
      memset(BuffCmd,0,80);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Routine principale de gestion du clavier.
//
void ReadKeyboard()
{
   char DHBuff[25];
   char OldCar;
   mtBYTE bTmp;
   struct dos_time_t tSc;
   mtUDWORD wSc;

   if(kbhit())
   {
      if(bScrSave)
      {
         // On rallume l'cran.
         ScreenOn();
         bScrSave  = mtFALSE;
         TpScrSave = 0L;
         //
         // On vide le buffer clavier.
         //
         mtTouche = getch();
         if(! mtTouche)
            mtTouche = getch();           // Code tendu.
         // On raffraichit l'cran.
         bRScreen = mtTRUE;
      }
      else
      {
         TpScrSave = 0L;
         OldCar = (char) (disp_peekw(LigCmd,ColCmd+CBCmd) & 0xFF);
         SWVPutChar(LigCmd,ColCmd+CBCmd,OldCar,CoulCmd);
         mtTouche = getch();
         if(! mtTouche)
         {
            mtTouche = getch();           // Code tendu.
            switch(mtTouche)
            {
               case FLG:
               {
                  if(CBCmd > 0)
                     CBCmd--;
                  else
                     SWVRingBell();
                  break;
               }
               case FLD:
               {
                  if(CBCmd < strlen(BuffCmd))
                     CBCmd++;
                  else
                     SWVRingBell();
                  break;
               }
               case CFLD:
               {
                  while((CBCmd < strlen(BuffCmd)) && (BuffCmd[CBCmd] !=' '))
                     CBCmd++;
                  while((CBCmd < strlen(BuffCmd)) && (BuffCmd[CBCmd] ==' '))
                     CBCmd++;
                  break;
               }
               case CFLG:
               {
                  if(CBCmd > 0)
                  {
                     CBCmd--;
                     while((CBCmd > 0) && (BuffCmd[CBCmd] ==' '))
                        CBCmd--;
                     while((CBCmd > 0) && (BuffCmd[CBCmd] !=' '))
                        CBCmd--;
                     if((CBCmd < strlen(BuffCmd)) && (BuffCmd[CBCmd] ==' '))
                        CBCmd++;
                  }
                  break;
               }
               case FLH:
               {
                  bTmp = pHisto;
                  if(pHisto > 0)
                     pHisto--;
                  else
                     pHisto = MAX_HISTO-1;
                  if(History[pHisto][0])
                  {
                     strcpy(BuffCmd,History[pHisto]);
                     VPrint(LigCmd,ColCmd,BuffCmd,CoulCmd);
                     CBCmd = strlen(BuffCmd);
                     for(bTmp=CBCmd;bTmp<79;bTmp++)
                        SWVPutChar(LigCmd,ColCmd+bTmp,' ',CoulCmd);
                  }
                  else
                     pHisto = bTmp;
                  break;
               }
               case FLB:
               {
                  bTmp = pHisto;
                  if(pHisto < (MAX_HISTO-1))
                     pHisto++;
                  else
                     pHisto = 0;
                  if(History[pHisto][0])
                  {
                     strcpy(BuffCmd,History[pHisto]);
                     VPrint(LigCmd,ColCmd,BuffCmd,CoulCmd);
                     CBCmd = strlen(BuffCmd);
                     for(bTmp=CBCmd;bTmp<79;bTmp++)
                        SWVPutChar(LigCmd,ColCmd+bTmp,' ',CoulCmd);
                  }
                  else
                     pHisto = bTmp;
                  break;
               }
               case HOME:
               {
                  CBCmd = 0;
                  break;
               }
               case END:
               {
                  CBCmd = strlen(BuffCmd);
                  break;
               }
               case INS:
               {
                  bInsert = mtTRUE;
                  break;
               }
               case DEL:
               {
                  bInsert = mtFALSE;
                  break;
               }
               case PGUP:
               {
                  if(bAffCnx)
                  {
                     if(pPageCv)
                     {
                        pPageCv -= 15;
                        if(pPageCv < 0)
                           pPageCv = 0;
                     }
                     // On raffraichit l'cran.
                     bRScreen = mtTRUE;
                  }
                  if(bAllApp)
                  {
                     if(pPageApp)
                     {
                        pPageApp--;
                        if(pPageApp < 0)
                           pPageApp = 0;
                        // On raffraichit l'cran.
                        bPageApp = mtTRUE;
                        bRScreen = mtTRUE;
                     }
                  }
                  break;
               }
               case PGDN:
               {
                  if(bAffCnx)
                  {
                     if(pPageCv+14 < TOTALCV)
                        pPageCv += 15;
                     // On raffraichit l'cran.
                     bRScreen = mtTRUE;
                  }
                  if(bAllApp)
                  {
                     if(nbAffApp == nAppPage)
                     {
                        pPageApp++;
                        // On raffraichit l'cran.
                        bPageApp = mtTRUE;
                        bRScreen = mtTRUE;
                     }
                  }
                  break;
               }
               case TF1:
               {
                  // Trace les wPasTrace instructions suivantes.
                  bNextStep = mtTRUE;
                  bAffTrace = mtTRUE;
                  break;
               }
            }
         }
         else
         {
            switch(mtTouche)
            {
               case ESC:
               {
                  CBCmd = 0;
                  memset(BuffCmd,0,80);
                  for(bTmp=0;bTmp<79;bTmp++)
                     SWVPutChar(LigCmd,ColCmd+bTmp,' ',CoulCmd);
                  break;
               }
               case RET:
               {
                  LigCmd++;
                  if(LigCmd == LFinCmd+1)
                  {
                     SWVScroll(SWSCROLLUP,LDebCmd,CDebCmd,LFinCmd,CFinCmd,1,CoulCmd);
                     LigCmd = LFinCmd;
                  }
                  // On enlve les espaces de dbut
                  while(BuffCmd[0] != 0 && BuffCmd[0] == ' ')
                     memmove(&BuffCmd[0],&BuffCmd[1],strlen(BuffCmd));
                  if(BuffCmd[0])
                  {
                     strcpy(History[pHisto],BuffCmd);
                     pHisto++;
                     if(pHisto > (MAX_HISTO-1))
                        pHisto = 0;
                     // Efface la ligne des msg d'erreur
                     SWVScroll(SWSCROLLUP,LigErr,ColErr,LigErr,79,1,CoulErr);
                     // Analyse et interprtation de la ligne tape
                     ProcessCmd();
                     // On raffraichit l'cran.
                     bRScreen = mtTRUE;
                  }
                  CBCmd = 0;
                  memset(BuffCmd,0,80);
                  break;
               }
               case BCKSPC:
               {
                  if(CBCmd > 0)
                  {
                     memmove(&BuffCmd[CBCmd-1],&BuffCmd[CBCmd],(strlen(BuffCmd)-CBCmd)+1);
                     CBCmd--;
                     VPrint(LigCmd,ColCmd,BuffCmd,CoulCmd);
                     for(bTmp=strlen(BuffCmd);bTmp<79;bTmp++)
                        SWVPutChar(LigCmd,ColCmd+bTmp,' ',CoulCmd);
                  }
                  else
                     SWVRingBell();
                  break;
               }
               default:
               {
                  if(mtTouche >= 32)      // && mtTouche <= 127)
                  {
                     if(CBCmd < 79)
                     {
                        if(bInsert)
                        {
                           if(strlen(BuffCmd)+1 < 80)
                              memmove(&BuffCmd[CBCmd+1],&BuffCmd[CBCmd],(strlen(BuffCmd)-CBCmd)+1);
                           else
                           {
                              SWVRingBell();
                              break;
                           }
                           VPrint(LigCmd,ColCmd+CBCmd+1,&BuffCmd[CBCmd+1],CoulCmd);
                        }
                        BuffCmd[CBCmd] = mtTouche;
                        SWVPutChar(LigCmd,ColCmd+CBCmd,mtTouche,CoulCmd);
                        CBCmd++;
                     }
                     else
                        SWVRingBell();
                  }
               }
            }
         }
         OldCar = (char) (disp_peekw(LigCmd,ColCmd+CBCmd) & 0xFF);
         SWVPutChar(LigCmd,ColCmd+CBCmd,OldCar,CoulCur);
         if(bInsert)
            VPrint(3,75,"Inser",SWVBLDATR(SWBLACK,SWCYAN));
         else
            VPrint(3,75,"Suppr",SWVBLDATR(SWBLACK,SWCYAN));
      }
   }
   else
   {
      if((wSaveScreen) && (! bScrSave))
      {
         _dos_gettime(&tSc);
         tSc.hsecond = 0;
         wSc         = TimeToCent(&tSc)/100;
         if(! TpScrSave)
            // On initialise le compteur.
            TpScrSave = wSc;
         else
         {
            // On teint l'cran au bout de wSaveScreen mn d'inactivit clavier.
            if((wSc-TpScrSave) >= wSaveScreen)
            {
               bScrSave = mtTRUE;
               ScreenOff();
            }
         }
      }
   }
   // Mise … jour de l'heure systme.
   sprintf(DHBuff,"%02d/%02d/%04d %02d:%02d:%02d",Jour,Mois,Annee,Heures,Minutes,Secondes);
   VPrint(LigDH,ColDH,DHBuff,CoulDH);
//   if(! bScrSave)
      // Afichage en temps rel du mode trace
//      WinAffTraceCv();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void InitKeyboard()
{
   int i;

   memset(BuffCmd,0,80);
   for(i=0;i<MAX_HISTO;i++)
      memset(History[i],0,80);
   if(bInsert)
      VPrint(3,75,"Inser",SWVBLDATR(SWBLACK,SWCYAN));
   else
      VPrint(3,75,"Suppr",SWVBLDATR(SWBLACK,SWCYAN));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une chaine dans un tableau.
// Renvoie VRAI si la commande est trouve et initialise pFCmd.
//
mtBOOL SearchTab(mtPTRBYTE Tab[],mtPTRBYTE sCmd)
{
   mtBYTE pCmd = 0;

   while(Tab[pCmd])
   {
      if(stricmp(Tab[pCmd],sCmd) == 0)
      {
         pFCmd = pCmd;
         return(mtTRUE);
      }
      pCmd++;
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Interprte la commande saisie.
//
void ProcessCmd()
{
   mtPTRBYTE CurrCmd;

   CurrCmd = strtok(BuffCmd," ");
   if(CurrCmd)
   {
      if(SearchTab(SysCmd,CurrCmd))
         // Excution de la commande
         (FctCmd[pFCmd])();
      else
      {
         WinMsgErr(": COMMANDE INCONNUE","%s",CurrCmd);
         WriteLogNeo("%s: COMMANDE INCONNUE",CurrCmd);
         nErrCmd = -1;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void CmQUIT()
{
   mtFin   = mtTRUE;
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void CmCLS()
{
   ClsWinInf(mtFALSE);
   bAffCom     = mtFALSE;
   bAffCv      = mtFALSE;
   bStatGen    = mtFALSE;
   bAffCnx     = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bInDebug    = mtFALSE;
   bInfoGen    = mtTRUE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visu d'infos spcifiques.
//
void CmVIEW()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   // Rcupration du paramtre de la commande.
   Param = strtok(NULL," ");
   if(Param)
   {
      if(SearchTab(OptView,Param))
         // Excution de la commande
         (FctView[pFCmd])();
      else
      {
         WinMsgErr(": PARAMETRE NON VALIDE","VIEW %s",Param);
         WriteLogNeo("VIEW %s: PARAMETRE NON VALIDE",Param);
         nErrCmd = 21;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","VIEW");
      WriteLogNeo("VIEW: PARAMETRE MANQUANT");
      nErrCmd = 20;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation de l'historique des commandes clavier.
//
void CmVHisto(void)
{
   int i;

   for(i=0;i<MAX_HISTO;i++)
   {
      if(History[i][0])
         WinInfo("%2d:%s",i,History[i]);
   }
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation des statistiques gnrales
//
void CmVStatGen()
{
   ClsWinInf(mtTRUE);
   bStatGen    = mtTRUE;
   bInfoGen    = mtFALSE;
   bAffCnx     = mtFALSE;
   bAffCv      = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCom     = mtFALSE;
   bInDebug    = mtFALSE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinStatGen();
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des stats gn.
//
void WinStatGen()
{
   int i;
   char VBuff[81];
   tTmp tT;
   mtUDWORD sT;
   float fT;

   if(bStatGen)
   {
      sprintf(VBuff,"STATISTIQUES GENERALES");
      VPrint(4,30,VBuff,CoulInf);
      if(pPageSGen == 0)
      {
         sprintf(VBuff,"Nombre total d'appels : %6lu",NbAppTot);
         VPrint(5,0,VBuff,CoulInf);
         sprintf(VBuff,"Premire connexion …  : %02dh%02dm%02ds",HPreCnx.hour,HPreCnx.minute,HPreCnx.second);
         VPrint(6,0,VBuff,CoulInf);
         sprintf(VBuff,"Dernire connexion … : %02dh%02dm%02ds",HDerCnx.hour,HDerCnx.minute,HDerCnx.second);
         VPrint(6,48,VBuff,CoulInf);
         sprintf(VBuff,"    Palier  ณ Appels ณ Temps de cnx ณ   T.M.C   ณ Revt. ณ   Gain    ณ  Ratio    ");
         VPrint(8,0,VBuff,CoulBdx);
         for(i=0;i<MAXPALIER;i++)
         {
            sprintf(VBuff,"%s",StatPalier[i].Nom);
            VPrint(9+i,3,VBuff,CoulInf);
            VPrint(9+i,12,"ณ",CoulInf);
            sprintf(VBuff,"%6lu",StatPalier[i].NbAppel);
            VPrint(9+i,14,VBuff,CoulInf);
            VPrint(9+i,21,"ณ",CoulInf);
            SecToTime(StatPalier[i].TpsCnx,&tT);
            sprintf(VBuff,"%5dh%02dm%02ds",tT.H,tT.M,tT.S);
            VPrint(9+i,23,VBuff,CoulInf);
            VPrint(9+i,36,"ณ",CoulInf);
            // Calcul du Temps Moyen de Connexion
            if(StatPalier[i].NbAppel)
            {
               sT = StatPalier[i].TpsCnx/StatPalier[i].NbAppel;
               SecToTime(sT,&tT);
               sprintf(VBuff,"%2dh%02dm%02ds",tT.H,tT.M,tT.S);
               VPrint(9+i,38,VBuff,CoulInf);
            }
            VPrint(9+i,48,"ณ",CoulInf);
            sprintf(VBuff,"%5.2f",StatPalier[i].Tarif);
            VPrint(9+i,50,VBuff,CoulInf);
            VPrint(9+i,56,"ณ",CoulInf);
            // Calcul du gain
            if(StatPalier[i].TpsCnx)
            {
               fT = StatPalier[i].TpsCnx/(float) 60;     // On converti en minutes.
               sprintf(VBuff,"%9.2f",StatPalier[i].Tarif*fT);
               VPrint(9+i,58,VBuff,CoulInf);
            }
            VPrint(9+i,68,"ณ",CoulInf);
            // Calcul du % reprsent par ce palier par rapport
            // … l'ensemble.
            if(NbAppTot)
            {
               fT = (((float)StatPalier[i].NbAppel/(float)NbAppTot)*(float) 100);
               sprintf(VBuff,"%5.2f %%",fT);
               // Efface la zone avant affichage.
               VPrint(9+i,71,"        ",CoulInf);
               VPrint(9+i,71,VBuff,CoulInf);
            }
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation de l'tat des connexions.
//
void CmVAffCnx()
{
   ClsWinInf(mtTRUE);
   bAffCom     = mtFALSE;
   bStatGen    = mtFALSE;
   bInfoGen    = mtFALSE;
   bAffCv      = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCnx     = mtTRUE;
   bInDebug    = mtFALSE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinAffCnx();
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage de l'tat des connexions.
//
void WinAffCnx()
{
   char pPoints[77];
   char VBuff[81];
   int i;
   mtPTRCV p;

   if(bAffCnx)
   {
      memset(pPoints,0,77);
      sprintf(VBuff,"ETAT DES CONNEXIONS");
      VPrint(4,30,VBuff,CoulInf);
      sprintf(pPoints,"             ณ          ณ                    ณ                    ณ         ");
      sprintf(VBuff,"Cv ณ Application ณ   Pile   ณ      Procdure     ณ    Tche locale    ณPile Loc.");
      VPrint(5,0,VBuff,CoulBdx);
      for(i=0;i<15;i++)
      {
         sprintf(VBuff,"%03dณ",i+pPageCv);
         VPrint(6+i,0,VBuff,CoulInf);
         sprintf(VBuff,pPoints);
         if((i+pPageCv) < TOTALCV)
         {
            p = &TabCv[i+pPageCv];
            if(p->Actif)
            {
               sprintf(VBuff,"%s",p->Service);
               VPrint(6+i,5,VBuff,CoulInf);
               VPrint(6+i,17,"ณ",CoulInf);
               sprintf(VBuff,"SP = %5d",p->SP);
               VPrint(6+i,18,VBuff,CoulInf);
               VPrint(6+i,28,"ณ",CoulInf);
               // Efface la ligne avant.
               VPrint(6+i,29,"                    ",CoulInf);
               if(p->pNProc)
               {
                  //sprintf(VBuff,"%s",p->pNProc);
                  strncpy(VBuff,p->pNProc,20);
                  VPrint(6+i,29,VBuff,CoulInf);
               }
               VPrint(6+i,49,"ณ",CoulInf);
               // Efface la ligne avant.
               VPrint(6+i,50,"                    ณ         ",CoulInf);
               if(LTask[i+pPageCv].Actif)
               {
                  strncpy(VBuff,LTask[i+pPageCv].pNProc,20);
                  VPrint(6+i,50,VBuff,CoulInf);
                  VPrint(6+i,70,"ณ",CoulInf);
                  sprintf(VBuff,"SP =%5d",LTask[i+pPageCv].SP);
                  VPrint(6+i,71,VBuff,CoulInf);
               }
            }
            else
               VPrint(6+i,4,VBuff,CoulInf);
         }
         else
            VPrint(6+i,4,VBuff,CoulInf);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation des informations d'un cv.
//
void CmVAffCv()
{
   mtPTRBYTE Param;
   int NoCv=-1;

   nErrCmd = 0;
   // Rcupration du n๘ de cv.
   Param = strtok(NULL," ");
   if(Param)
   {
      NoCv = atoi(Param);
      if((isdigit(*Param)) && (NoCv >= 0 && NoCv < TOTALCV))
      {
         if(TabCv[NoCv].Actif)
         {
            ClsWinInf(mtTRUE);
            bStatGen    = mtFALSE;
            bInfoGen    = mtFALSE;
            bAffCv      = mtTRUE;
            bAffCnx     = mtFALSE;
            bAffApp     = mtFALSE;
            bAllApp     = mtFALSE;
            bAffHlp     = mtFALSE;
            bAffCom     = mtFALSE;
            bInDebug    = mtFALSE;
            bAffInfoSys = mtFALSE;
            bAffTPCnx   = mtFALSE;
            pNCvAff  = NoCv;
            if(pCvTrace)
            {
               // On dsactive le mode trace.
               pCvTrace->bInDebug = mtFALSE;
               pCvTrace           = NULL;
            }
            WinAffCv();
         }
         else
         {
            WinMsgErr(": CE CV EST INACTIF","VIEW CV %s",Param);
            WriteLogNeo("VIEW CV %s: CE CV EST INACTIF",Param);
            nErrCmd = 222;
         }
      }
      else
      {
         WinMsgErr(": N๘ DE CV NON VALIDE","VIEW CV %s",Param);
         WriteLogNeo("VIEW CV %s: N๘ DE CV NON VALIDE",Param);
         nErrCmd = 221;
      }
   }
   else
   {
      WinMsgErr(": N๘ DE CV MANQUANT","VIEW CV");
      WriteLogNeo("VIEW CV: N๘ DE CV MANQUANT");
      nErrCmd = 220;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des informations d'un cv.
//
void WinAffCv()
{
   char LVide[80];
   char VBuff[81];

   if(bAffCv)
   {
      memset(LVide,0,80);
      memset(LVide,' ',79);
      sprintf(VBuff,"INFORMATIONS DU CV N๘%03d",pNCvAff);
      VPrint(4,29,VBuff,CoulInf);
      if(LTask[pNCvAff].Actif)
         sprintf(VBuff,"(Tche locale active)");
      else
         sprintf(VBuff,"                     ");
      VPrint(4,59,VBuff,CoulInf);
      sprintf(VBuff,"Trame d'appel : %s",TabCv[pNCvAff].TrameCNX);
      VPrint(6,0,VBuff,CoulInf);
      sprintf(VBuff,"Date et heure de connexion : %02d/%02d/%02d … %02dh%02dm%02ds",TabCv[pNCvAff].DateCnx.day,TabCv[pNCvAff].DateCnx.month,TabCv[pNCvAff].DateCnx.year,TabCv[pNCvAff].HeureCnx.hour,TabCv[pNCvAff].HeureCnx.minute,TabCv[pNCvAff].HeureCnx.second);
      VPrint(7,0,VBuff,CoulInf);
      // Efface la ligne avant raffichage.
      VPrint(8,10,LVide,CoulInf);
      sprintf(VBuff,"Service : %s",TabCv[pNCvAff].Service);
      VPrint(8,0,VBuff,CoulInf);
      sprintf(VBuff,"Dpt.   : %d",TabCv[pNCvAff].Dept);
      VPrint(9,0,VBuff,CoulInf);
      sprintf(VBuff,"Pavi    : %s",TabCv[pNCvAff].Pavi);
      VPrint(10,0,VBuff,CoulInf);
      sprintf(VBuff,"Palier  : %d",TabCv[pNCvAff].Palier);
      VPrint(11,0,VBuff,CoulInf);
      sprintf(VBuff,"N๘ comp.: %d",TabCv[pNCvAff].NumComp);
      VPrint(12,0,VBuff,CoulInf);
      sprintf(VBuff,"Minitel : %d",TabCv[pNCvAff].TypeMinitel);
      VPrint(13,0,VBuff,CoulInf);
      switch(TabCv[pNCvAff].cMode)
      {
         case TMODE_VDT:     { VPrint(13,15,"(VIDEOTEX)",CoulInf); break; }
         case TMODE_MIXTE:   { VPrint(13,15,"(MIXTE)   ",CoulInf); break; }
         case TMODE_TELINFO: { VPrint(13,15,"(TELEINFO)",CoulInf); break; }
      }
      // Efface la ligne avant raffichage.
      VPrint(14,10,LVide,CoulInf);
      if(TabCv[pNCvAff].pData)
      {
         sprintf(VBuff,"Donnes : ");
         strncat(VBuff,TabCv[pNCvAff].pData,70);
         VPrint(14,0,VBuff,CoulInf);
      }
      // Efface la ligne avant raffichage.
      VPrint(15,10,LVide,CoulInf);
      if(TabCv[pNCvAff].TFonction)
      {
         sprintf(VBuff,"Touche  : ");
         if(TabCv[pNCvAff].bEtoile)
            strcat(VBuff,"*");
         if(TabCv[pNCvAff].bDiese)
            strcat(VBuff,"#");
         switch(TabCv[pNCvAff].TFonction)
         {
            case MEnvoi     : { strcat(VBuff,"ENVOI")         ; break ; }
            case MRetour    : { strcat(VBuff,"RETOUR")        ; break ; }
            case MRepetition: { strcat(VBuff,"REPETITION")    ; break ; }
            case MGuide     : { strcat(VBuff,"GUIDE")         ; break ; }
            case MAnnulation: { strcat(VBuff,"ANNULATION")    ; break ; }
            case MSommaire  : { strcat(VBuff,"SOMMAIRE")      ; break ; }
            case MCorrection: { strcat(VBuff,"CORRECTION")    ; break ; }
            case MSuite     : { strcat(VBuff,"SUITE")         ; break ; }
            case MCnxFin    : { strcat(VBuff,"CONNEXION/FIN") ; break ; }
         }
         VPrint(15,0,VBuff,CoulInf);
      }
      // Efface la ligne avant raffichage.
      VPrint(16,10,LVide,CoulInf);
      if(TabCv[pNCvAff].pOutBuff)
      {
         sprintf(VBuff,"Donn.Out: ");
         strncat(VBuff,TabCv[pNCvAff].pOutBuff,70);
         VPrint(16,0,VBuff,CoulInf);
      }
      // Efface la ligne avant raffichage.
      VPrint(17,10,LVide,CoulInf);
      sprintf(VBuff,"Ins / T : %d (Max=%d)",TabCv[pNCvAff].NbIPS,TabCv[pNCvAff].MaxIPS);
      VPrint(17,0,VBuff,CoulInf);
      // Efface la ligne avant raffichage.
      VPrint(18,10,LVide,CoulInf);
      sprintf(VBuff,"Nb ISym : %d",TabCv[pNCvAff].NbISym);
      VPrint(18,0,VBuff,CoulInf);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un message en ligne 0 … 1 ou tous les connects.
//
void CmSEND()
{
   mtPTRBYTE Param,Msg,Msg0;
   int i,NoCv=-1;

   nErrCmd = 0;
   // Rcupration du message.
   Msg = strtok(NULL,"\x22");        // Cherche le prochain ".
   if(Msg)
   {
      Msg0 = (mtPTRBYTE) MemAlloc(strlen(Msg)+5);
      strcpy(Msg0,"Srv:");
      strcat(Msg0,Msg);
      Param = strtok(NULL," ");
      if(Param)
      {
         if(stricmp("ALL",Param) == 0)
         {
            // Envoi … tous les connects
            for(i=0;i<TOTALCV;i++)
            {
               if(TabCv[i].Actif)
                  Send0(Msg0,&TabCv[i]);
            }
         }
         else
         {
            NoCv = atoi(Param);
            if((isdigit(*Param)) && (NoCv >= 0 && NoCv < TOTALCV))
            {
               if(TabCv[NoCv].Actif)
                  Send0(Msg0,&TabCv[NoCv]);
               else
               {
                  WinMsgErr(": CE CV EST INACTIF","SEND '...' %s",Param);
                  WriteLogNeo("SEND '...' %s: CE CV EST INACTIF",Param);
                  nErrCmd = 33;
               }
            }
            else
            {
               WinMsgErr(": N๘ DE CV NON VALIDE","SEND '...' %s",Param);
               WriteLogNeo("SEND '...' %s: N๘ DE CV NON VALIDE",Param);
               nErrCmd = 32;
            }
         }
      }
      else
      {
         WinMsgErr(": PARAMETRE MANQUANT","SEND");
         WriteLogNeo("SEND: PARAMETRE MANQUANT");
         nErrCmd = 31;
      }
      MemFree(Msg0);
   }
   else
   {
      WinMsgErr(": MESSAGE MANQUANT","SEND");
      WriteLogNeo("SEND: MESSAGE MANQUANT");
      nErrCmd = 30;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Modification de variables systme.
//
void CmSET()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   // Rcupration du paramtre de la commande.
   Param = strtok(NULL," ");
   if(Param)
   {
      if(SearchTab(OptSet,Param))
         // Excution de la commande
         (FctSet[pFCmd])();
      else
      {
         WinMsgErr(": PARAMETRE NON VALIDE","SET %s",Param);
         WriteLogNeo("SET %s: PARAMETRE NON VALIDE",Param);
         nErrCmd = 41;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","SET");
      WriteLogNeo("SET: PARAMETRE MANQUANT");
      nErrCmd = 40;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Modification d'un palier.
//
void CmSPalier()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      strupr(Param);
      IniPalier(Param);
   }
   else
   {
      WinMsgErr(": PARAMETRES MANQUANTS","SET PALIER");
      WriteLogNeo("SET PALIER: PARAMETRES MANQUANTS");
      nErrCmd = 400;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Modification d'une couleur.
void CmSColor()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      strupr(Param);
      IniCouleur(Param);
      // Raffichage de la fentre modifie
      if(strstr(Param,"SYS"))
         SWVScroll(SWSCROLLUP,1,0,3,79,3,CoulSys);
      if(strstr(Param,"INF"))
         ClsWinInf(mtTRUE);
      if(strstr(Param,"CMD"))
         SWVScroll(SWSCROLLUP,LDebCmd,CDebCmd,LFinCmd,CFinCmd,(LFinCmd-LDebCmd)+1,CoulCmd);
      if(strstr(Param,"ERR"))
         SWVScroll(SWSCROLLUP,LigErr,ColErr,LigErr,79,1,CoulErr);
      if(strstr(Param,"CUR"))
         SWVPutChar(LigCmd,ColCmd,' ',CoulCur);
   }
   else
   {
      WinMsgErr(": PARAMETRES MANQUANTS","SET COLOR");
      WriteLogNeo("SET COLOR: PARAMETRES MANQUANTS");
      nErrCmd = 410;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Modification d'un paramtre systme.
//
void CmSSystem(void)
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      strupr(Param);
      IniSystem(Param);
      if(strstr(Param,"APPAUTOCNX ="))
         LoadAutoApp();
      if(strstr(Param,"TRACEPAQUET ="))
      {
         if(bTracePaq)
         {
            if(! OpenLogPaq())
            {
               WinMsgErr(": ERREUR A L'OUVERTURE","OPEN %s",pFicPAQ);
               WriteLogNeo("OPEN %s: ERREUR A L'OUVERTURE",pFicPAQ);
               bTracePaq = mtFALSE;
               nErrCmd   = 421;
            }
         }
      }
      if(strstr(Param,"TRACESERVEUR ="))
      {
         if(bTraceNeo)
         {
            if(! OpenLogNeo())
            {
               WinMsgErr(": ERREUR A L'OUVERTURE","OPEN %s",pFicNEO);
               bTraceNeo = mtFALSE;
               nErrCmd   = 422;
            }
            WriteLogNeo("*****> ACTIVATION DU MODE TRACE LE %02d/%02d/%04d A %02d:%02d",Jour,Mois,Annee,Heures,Minutes);
         }
      }
   }
   else
   {
      WinMsgErr(": PARAMETRES MANQUANTS","SET SYSTEM");
      WriteLogNeo("SET SYSTEM: PARAMETRES MANQUANTS");
      nErrCmd = 420;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Chargement/Compilation d'une application.
//
void CmLOAD()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      ClsWinInf(mtTRUE);
      if(pCvTrace)
      {
         // On dsactive le mode trace.
         pCvTrace->bInDebug = mtFALSE;
         pCvTrace           = NULL;
      }
      bInDebug    = mtFALSE;
      bAffCom     = mtFALSE;
      bAffCv      = mtFALSE;
      bStatGen    = mtFALSE;
      bAffCnx     = mtFALSE;
      bAffApp     = mtFALSE;
      bAllApp     = mtFALSE;
      bAffHlp     = mtFALSE;
      bInfoGen    = mtTRUE;
      bAffInfoSys = mtFALSE;
      bAffTPCnx   = mtFALSE;
      // WinInfo("LoadApp %s",Param);
      strupr(Param);
      if(LoadApp(Param))
      {
         WinMsgErr(": ERREUR(S) SURVENUES A LA COMPILATION","LOAD");
         WriteLogNeo("LOAD: ERREUR(S) SURVENUES A LA COMPILATION");
         nErrCmd = 51;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRES MANQUANTS","LOAD");
      WriteLogNeo("LOAD: PARAMETRES MANQUANTS");
      nErrCmd = 50;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libration d'une application.
//
void CmUNLOAD()
{
   mtPTRBYTE Param;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      ClsWinInf(mtTRUE);
      if(pCvTrace)
      {
         // On dsactive le mode trace.
         pCvTrace->bInDebug = mtFALSE;
         pCvTrace           = NULL;
      }
      bAffCv      = mtFALSE;
      bStatGen    = mtFALSE;
      bAffCnx     = mtFALSE;
      bAffApp     = mtFALSE;
      bAllApp     = mtFALSE;
      bAffHlp     = mtFALSE;
      bAffCom     = mtFALSE;
      bInfoGen    = mtTRUE;
      bInDebug    = mtFALSE;
      bAffInfoSys = mtFALSE;
      bAffTPCnx   = mtFALSE;
      strupr(Param);
      UnLoadApp(Param);
   }
   else
   {
      WinMsgErr(": PARAMETRES MANQUANTS","UNLOAD");
      WriteLogNeo("UNLOAD: PARAMETRES MANQUANTS");
      nErrCmd = 60;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libration d'un cv, de tous les cv, des cv d'une application.
//
void CmLIB(void)
{
   mtPTRBYTE Param;
   int i,NoCv=-1;
   mtPTRAPP pLApp;
   mtPTRLSTCV LstCv;
   mtPTRCV pLeCv;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      strupr(Param);
      if(strcmp("ALL",Param) == 0)
      {
         // Libration de tous les connects.
         for(i=0;i<MAXPORTS;i++)
         {
            if(tPorts[i].Actif)
            {
               tPorts[i].CommLibereAll(i);
            }
         }
      }
      else
      {
         // On regarde si ce n'est pas un n๘ de cv
         NoCv = atoi(Param);
         if((isdigit(*Param)) && (NoCv >= 0 && NoCv < TOTALCV))
         {
            if(TabCv[NoCv].Actif)
               tPorts[TabCv[NoCv].NumPort].LibereCV(NoCv);
            else
            {
               WinMsgErr(": CE CV EST INACTIF","LCX %s",Param);
               WriteLogNeo("LCX %s: CE CV EST INACTIF",Param);
               nErrCmd = 73;
            }
         }
         else
         {
            // On regarde si ce n'est pas un nom d'application.
            pLApp = SearchApp(Param,0);
            if(pLApp)
            {
               LstCv = pLApp->pLstCnx;
               if(LstCv)
               {
                  while(LstCv)
                  {
                     pLeCv = (mtPTRCV) LstCv->pCv;
                     switch(tPorts[pLeCv->NumPort].Type)
                     {
                        case TPORTRTC   : { RTCCommLibere(pLeCv->NumPort,pLeCv->NumCv) ; break ; }
                        case TPORTLOCAL : { LOCCommLibere(pLeCv->NumPort,pLeCv->NumCv) ; break ; }
                        case TPORTX25ASM: { X25CommLibere(pLeCv->NumPort,pLeCv->NumCv) ; break ; }
                     }
                     if(pLApp->bOnLib)
                        InitOnLibCv(pLeCv);
                     else
                        DeconnecteCv(pLeCv);
                     LstCv = pLApp->pLstCnx;
                  }
               }
               else
               {
                  WinMsgErr(": PAS DE CONNECTES ACTIFS","LCX %s",Param);
                  WriteLogNeo("LCX %s: PAS DE CONNECTES ACTIFS",Param);
                  nErrCmd = 72;
               }
            }
            else
            {
               WinMsgErr(": PARAMETRE INVALIDE","LCX %s",Param);
               WriteLogNeo("LCX %s: PARAMETRE INVALIDE",Param);
               nErrCmd = 71;
            }
         }
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","LCX");
      WriteLogNeo("LCX: PARAMETRE MANQUANT");
      nErrCmd = 70;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cherche une application … partir de l'adresse d'un bloc de code.
// (utilise par le mode trace)
// Renvoie un pointeur sur l'application.
//
mtPTRAPP TSearchApp(ppFUNC pCode)
{
   mtPTRAPP pApp,pSApp=NULL;

   pApp = pAppli;
   while(pApp)
   {
      if(pApp->pHere == pCode)
      {
         pSApp = pApp;
         break;
      }
      pApp = pApp->pNextAPP;
   }
   return(pSApp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cherche le Nom d'une procdure, … partir de son adresse
// (utilise par le mode trace)
// Renvoie un pointeur sur le nom de la procdure.
//
mtPTRBYTE TSearchNomProc(pPDICO pDic,mtUWORD IDic,mtPOINTEUR pSeg,mtPOINTEUR pOff)
{
   mtPTRBYTE pProc=NULL;
   mtPTRDICO d;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      d = pDic[wIDic];
      if((d->Type == mt2POINTS) && (d->V.pVal == pSeg) && (d->XVal == pOff))
      {
         pProc = d->Nom;
         break;
      }
      wIDic++;
   }
   return(pProc);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage de la trace d'un connect
//
void WinAffTraceCv(void)
{
   char VBuff[81];
   char BNProc[40];
   char BSPile[20];
   char BCSym[20];
   int LAff;
   mtPTRDICO pSym;
   mtUWORD cSP;
   mtPOINTEUR pSeg,pOff;
   mtPTRBYTE pProc;
   mtPTRAPP pApp;
   mtUWORD uIP;

   if(pCvTrace)
   {
      if(bAffTrace)
      {
         sprintf(VBuff,"%4d",wNbInst+1);
         VPrint(5,0,VBuff,SWVBLDATR(SWWHITE,SWRED));
         sprintf(VBuff,"SP=%5d",pCvTrace->SP);
         VPrint(5,55,VBuff,SWVBLDATR(SWWHITE,SWRED));
         // Scrolling de la fenetre des procdures.
         SWVScroll(SWSCROLLUP,LDebProc,CDebProc,LFinProc,CFinProc,1,CoulFProc);
         memset(BNProc,0,40);
         memset(BSPile,0,20);
         memset(BCSym,0,20);
         sprintf(BSPile,"");
         sprintf(BCSym,"");
         if(pCvTrace->pNProc)
            strncpy(BNProc,pCvTrace->pNProc,39);
         else
            sprintf(BNProc,"<...>");
         if(strcmp(BNProc,":") == 0)
         {
            // Appel d'une procdure.
            pApp = TSearchApp(pCvTrace->pHere);
            if(pApp)
            {
               // On rcupre le segment et l'offset pour chercher
               // son nom.
               pSeg  = (mtPOINTEUR) pCvTrace->pHere;
               pOff  = (mtPOINTEUR) pCvTrace->IP;
               pProc = TSearchNomProc(pApp->pPDico,pApp->uPDico,pSeg,pOff);
               if(pProc)
               {
                  strcat(BNProc," ");
                  strncat(BNProc,pProc,37);
                  memset(VBuff,0,81);
                  memset(VBuff,32,40);
                  VPrint(5,10,VBuff,SWVBLDATR(SWWHITE,SWRED));
                  memset(VBuff,0,81);
                  sprintf(VBuff,"Procdure: %s::%s",pApp->Nom,pProc);
                  VPrint(5,10,VBuff,SWVBLDATR(SWWHITE,SWRED));
               }
            }
         }
         if(strcmp(BNProc,";") == 0)
         {
            // Retour de procdure.
            pApp = TSearchApp(pCvTrace->pHere);
            if(pApp)
            {
               // On cherche le nom de la procdure courante.
               pSeg = (mtPOINTEUR) pCvTrace->pHere;
               uIP  = (mtUWORD) pCvTrace->IP;
               while(1)
               {
                  pOff  = (mtPOINTEUR) uIP;
                  pProc = TSearchNomProc(pApp->pPDico,pApp->uPDico,pSeg,pOff);
                  if(pProc)
                  {
                     memset(VBuff,0,81);
                     memset(VBuff,32,40);
                     VPrint(5,10,VBuff,SWVBLDATR(SWWHITE,SWRED));
                     memset(VBuff,0,81);
                     sprintf(VBuff,"Procdure: %s::%s",pApp->Nom,pProc);
                     VPrint(5,10,VBuff,SWVBLDATR(SWWHITE,SWRED));
                     break;
                  }
                  if(uIP == 0)
                     break;
                  uIP--;
               }
            }
         }
         if((strcmp(BNProc,"BRANCH") == 0) || (strcmp(BNProc,"0BRANCH") == 0))
         {
            // Branchement
            sprintf(VBuff," NewIP=%5d",pCvTrace->IP);
            strcat(BNProc,VBuff);
         }
         VPrint(LFinProc,CDebProc,BNProc,CoulFProc);
         // Scrolling de la fenetre de la pile.
         SWVScroll(SWSCROLLUP,LDebPile,CDebPile,LFinPile,CFinPile,(LFinPile-LDebPile)+1,CoulFPile);
         if(pCvTrace->SP)
         {
            LAff = (LFinPile - pCvTrace->SP)+1;
            cSP  = pCvTrace->SP;
            if(LAff<5)
               LAff = 5;
            while((LAff<=LFinPile) && (cSP))
            {
               cSP--;
               pSym = (mtPTRDICO) pCvTrace->PileD[cSP];
               if(pSym)
               {
                  if(pSym->Nom)
                     strncpy(BSPile,pSym->Nom,15);
                  else
                     sprintf(BSPile,"SYMBOLE INTERNE");

                  switch(pSym->Type)
                  {
                     case mtTCHAR:
                     {
                        strcat(BSPile," C");
                        if(pSym->V.cVal)
                           strncpy(BCSym,pSym->V.cVal,19);
                        else
                           sprintf(BCSym,"<NULL>");
                        break;
                     }
                     case mtTNUMBER:
                     {
                        strcat(BSPile," N");
                        sprintf(BCSym,"%f",pSym->V.fVal);
                        break;
                     }
                     case mtTABCHAR:
                     {
                        strcat(BSPile," C[]");
                        break;
                     }
                     case mtTABNUMBER:
                     {
                        strcat(BSPile," N[]");
                        break;
                     }
                     case mt2POINTS:
                     {
                        strcat(BSPile," P/F");
                        break;
                     }
                  }
                  VPrint(LAff,CDebPile,BSPile,CoulFPile);
                  VPrint(LAff,CDebPile+20,BCSym,CoulFPile);
                  LAff++;
               }
            }
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Trace de l'excution d'un connect.
//
void CmTRACECV(void)
{
   char VBuff[81];
   mtPTRBYTE Param;
   int NoCv=-1;
   mtPOINTEUR pSeg,pOff;
   mtPTRBYTE pProc;
   mtUWORD uIP;
   mtPTRAPP pApp;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      NoCv = atoi(Param);
      if((isdigit(*Param)) && (NoCv >= 0 && NoCv < TOTALCV))
      {
         if(TabCv[NoCv].Actif)
         {
            ClsWinInf(mtTRUE);
            bStatGen    = mtFALSE;
            bInfoGen    = mtFALSE;
            bAffCv      = mtFALSE;
            bAffCnx     = mtFALSE;
            bAffApp     = mtFALSE;
            bAllApp     = mtFALSE;
            bAffHlp     = mtFALSE;
            bAffCom     = mtFALSE;
            bInDebug    = mtTRUE;
            bNextStep   = mtTRUE;
            bAffTrace   = mtTRUE;
            bAffInfoSys = mtFALSE;
            bAffTPCnx   = mtFALSE;
            if(pCvTrace)
               // On dsactive le prcdent.
               pCvTrace->bInDebug = mtFALSE;
            pCvTrace           = &TabCv[NoCv];
            pCvTrace->bInDebug = mtTRUE;
            LDebProc  = 7;
            CDebProc  = 0;
            LFinProc  = 20;
            CFinProc  = 40;
            LDebPile  = 7;
            CDebPile  = 41;
            LFinPile  = 14;
            CFinPile  = 79;
            LDebVar   = 15;
            CDebVar   = 41;
            LFinVar   = 20;
            CFinVar   = 79;
            LAffVar   = LDebVar+1;
            CoulFProc = CoulInf;
            CoulFPile = SWVBLDATR(SWBLACK,SWCYAN);
            CoulFVar  = SWVBLDATR(SWBLACK,SWBROWN);
            // Scrolling de la fenetre d'infos..
            SWVScroll(SWSCROLLUP,4,0,6,79,3,SWVBLDATR(SWWHITE,SWRED));
            sprintf(VBuff,"TRACE DU CV N๘ %3d (F1:Excute %4d inst.)",pCvTrace->NumCv,wPasTrace);
            VPrint(4,0,VBuff,SWVBLDATR(SWWHITE,SWRED));
            sprintf(VBuff,"Application: %s",pCvTrace->pApp->Nom);
            VPrint(4,(80-strlen(VBuff)),VBuff,SWVBLDATR(SWWHITE,SWRED));
            // Scrolling de la fenetre de la pile.
            SWVScroll(SWSCROLLUP,LDebPile,CDebPile,LFinPile,CFinPile,(LFinPile-LDebPile)+1,CoulFPile);
            sprintf(VBuff,"PILE");
            VPrint(6,59,VBuff,SWVBLDATR(SWWHITE,SWRED));
            sprintf(VBuff,"SP=%5d",pCvTrace->SP);
            VPrint(5,55,VBuff,SWVBLDATR(SWWHITE,SWRED));
            // Scrolling de la fenetre des variables.
            SWVScroll(SWSCROLLUP,LDebVar,CDebVar,LFinVar,CFinVar,(LFinVar-LDebVar)+1,CoulFVar);
            sprintf(VBuff,"VARIABLES");
            VPrint(15,56,VBuff,CoulFVar);
            // On cherche l'application en cours d'excution.
            pApp = TSearchApp(pCvTrace->pHere);
            if(pApp)
            {
               // On cherche le nom de la procdure courante.
               pSeg = (mtPOINTEUR) pCvTrace->pHere;
               uIP  = (mtUWORD) pCvTrace->IP;
               while(1)
               {
                  pOff  = (mtPOINTEUR) uIP;
                  pProc = TSearchNomProc(pApp->pPDico,pApp->uPDico,pSeg,pOff);
                  if(pProc)
                  {
                     memset(VBuff,0,81);
                     sprintf(VBuff,"Procdure: %s::%s",pApp->Nom,pProc);
                     VPrint(5,10,VBuff,SWVBLDATR(SWWHITE,SWRED));
                     break;
                  }
                  if(uIP == 0)
                     break;
                  uIP--;
               }
            }
         }
         else
         {
            WinMsgErr(": CE CV EST INACTIF","DEBUG '...' %s",Param);
            WriteLogNeo("DEBUG '...' %s: CE CV EST INACTIF",Param);
            nErrCmd = 82;
         }
      }
      else
      {
         WinMsgErr(": N๘ DE CV INVALIDE","DEBUG '...' %s",Param);
         WriteLogNeo("DEBUG '...' %s: N๘ DE CV INVALIDE",Param);
         nErrCmd = 81;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","DEBUG");
      WriteLogNeo("DEBUG: PARAMETRE MANQUANT");
      nErrCmd = 80;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un symbole (utilis par le mode trace).
//
mtPTRDICO SearchCVTSym(mtPTRCV pCv,mtPTRBYTE NomS,mtPTRWORD pPortee)
{
   mtPTRDICO SSym=(mtPTRDICO) 0;
   mtPTRLISTDICO d;
   mtUWORD wPos;

   *pPortee = 0;
   d = SearchListDico(pCv->pLDico,pCv->uLDico,pCv->pApp);
   if(d)
//      SSym = SearchDico(d->pLDico,d->uLDico,NomS,&wPos);
      // Les noms des variables ne sont pas copis dans les dico locaux.
      // On cherche la position de la variable dans le dico de l'appli.
      SSym = SearchDico(pCv->pApp->pLDico,pCv->pApp->uLDico,NomS,&wPos);
   if(! SSym)
   {
      SSym = SearchDico(pCv->pApp->pPDico,pCv->pApp->uPDico,NomS,&wPos);
      if(! SSym)
      {
         SSym = SearchDico(pGlobalDic,uGDico,NomS,&wPos);
         if(SSym)
            *pPortee = PGLOBAL;
      }
      else
         *pPortee = PPUBLIC;
   }
   else
   {
      *pPortee = PLOCAL;
      SSym     = d->pLDico[wPos];
   }
   return(SSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation d'une variable (en mode trace)
//
void CmVVar(void)
{
   char VBuff[81];
   char VBTmp[20];
   mtPTRBYTE NomVar;
   mtPTRDICO pSym;
   mtPTRITAB pITmp;
   mtWORD wPortee;

   nErrCmd = 0;
   if(pCvTrace)
   {
      // Rcupration du nom de la variable.
      NomVar = strtok(NULL," ");
      if(NomVar)
      {
         strupr(NomVar);
         pSym = SearchCVTSym(pCvTrace,NomVar,&wPortee);
         if(pSym)
         {
            if(LAffVar > LFinVar)
            {
               SWVScroll(SWSCROLLUP,LDebVar+1,CDebVar,LFinVar,CFinVar,1,CoulFVar);
               LAffVar = LFinVar;
            }
            memset(VBuff,0,81);
            sprintf(VBuff,"Variable: %s",NomVar);
            VPrint(LAffVar,CDebVar,VBuff,CoulFVar);
            LAffVar++;
            if(LAffVar > LFinVar)
            {
               SWVScroll(SWSCROLLUP,LDebVar+1,CDebVar,LFinVar,CFinVar,1,CoulFVar);
               LAffVar = LFinVar;
            }
            memset(VBuff,0,81);
            switch(wPortee)
            {
               case PLOCAL:  { sprintf(VBuff,"LOCALE")   ; break ; }
               case PPUBLIC: { sprintf(VBuff,"PUBLIQUE") ; break ; }
               case PGLOBAL: { sprintf(VBuff,"GLOBALE")  ; break ; }
            }
            switch(pSym->Type)
            {
               case mtTCHAR:     { strcat(VBuff," / CHAR")     ; break ; }
               case mtTNUMBER:   { strcat(VBuff," / NUMBER")   ; break ; }
               case mtTABCHAR:
               {
                  memset(VBTmp,0,20);
                  pITmp = (mtPTRITAB) pSym->XVal;
                  sprintf(VBTmp," / CHAR[%dx%d]",pITmp->MaxLig,pITmp->MaxCol);
                  strcat(VBuff,VBTmp);
                  break;
               }
               case mtTABNUMBER:
               {
                  memset(VBTmp,0,20);
                  pITmp = (mtPTRITAB) pSym->XVal;
                  sprintf(VBTmp," / NUMBER[%dx%d]",pITmp->MaxLig,pITmp->MaxCol);
                  strcat(VBuff,VBTmp);
                  break;
               }
            }
            VPrint(LAffVar,CDebVar+10,VBuff,CoulFVar);
            LAffVar++;
            if(LAffVar > LFinVar)
            {
               SWVScroll(SWSCROLLUP,LDebVar+1,CDebVar,LFinVar,CFinVar,1,CoulFVar);
               LAffVar = LFinVar;
            }
            VPrint(LAffVar,CDebVar,"Contenu",CoulFVar);
            LAffVar++;
            if(LAffVar > LFinVar)
            {
               SWVScroll(SWSCROLLUP,LDebVar+1,CDebVar,LFinVar,CFinVar,1,CoulFVar);
               LAffVar = LFinVar;
            }
            memset(VBuff,0,81);
            switch(pSym->Type)
            {
               case mtTCHAR:
               {
                  strncpy(VBuff,pSym->V.cVal,35);
                  if(strlen(pSym->V.cVal) > 35)
                     strcat(VBuff,"...");
                  break;
               }
               case mtTNUMBER:
               {
                  sprintf(VBuff,"%f",pSym->V.fVal);
                  break;
               }
               case mtTABCHAR:
               {
                  break;
               }
               case mtTABNUMBER:
               {
                  break;
               }
            }
            VPrint(LAffVar,CDebVar,VBuff,CoulFVar);
            LAffVar++;
         }
         else
         {
            WinMsgErr(": VARIABLE INCONNUE","VIEW VAR %s",NomVar);
            WriteLogNeo("VIEW VAR %s: VARIABLE INCONNUE",NomVar);
            nErrCmd = 262;
         }
      }
      else
      {
         WinMsgErr(": PARAMETRE MANQUANT","VIEW VAR");
         WriteLogNeo("VIEW VAR: PARAMETRE MANQUANT");
         nErrCmd = 261;
      }
   }
   else
   {
      WinMsgErr(": MODE TRACE INACTIF","VIEW VAR");
      WriteLogNeo("VIEW VAR: MODE TRACE INACTIF");
      nErrCmd = 260;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void CmUNTRACECV(void)
{
   ClsWinInf(mtTRUE);
   bStatGen    = mtFALSE;
   bInfoGen    = mtTRUE;
   bAffCv      = mtFALSE;
   bAffCnx     = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCom     = mtFALSE;
   bInDebug    = mtFALSE;
   bNextStep   = mtFALSE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le prcdent.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Modification du pas de trace.
//
void CmDEBUGSTEP(void)
{
   char VBuff[81];
   mtPTRBYTE Param;
   int iPas=-1;

   nErrCmd = 0;
   Param   = strtok(NULL,"");
   if(Param)
   {
      iPas = atoi(Param);
      if((isdigit(*Param)) && (iPas > 0))
      {
         wPasTrace = iPas;
         if((bInDebug) && (pCvTrace))
         {
            sprintf(VBuff,"TRACE DU CV N๘ %3d (F1:Excute %4d inst.)",pCvTrace->NumCv,wPasTrace);
            VPrint(4,0,VBuff,SWVBLDATR(SWWHITE,SWRED));
         }
         wNbInst   = 0;
         bNextStep = mtTRUE;
         bAffTrace = mtTRUE;
      }
      else
      {
         WinMsgErr(": PAS DE TRACE INVALIDE","DEBUGSTEP '...' %s",Param);
         WriteLogNeo("DEBUGSTEP %s: PAS DE TRACE INVALIDE",Param);
         nErrCmd = 101;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","DEBUGSTEP");
      WriteLogNeo("DEBUGSTEP: PARAMETRE MANQUANT");
      nErrCmd = 100;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Connexion du port local … l'application donne.
//
void CmCONNECT(void)
{
   int nPort=-1;
   mtPTRBYTE cApp,cNo;
   mtBOOL bLOC=mtFALSE;
   mtPTRAPP pAp;

   nErrCmd = 0;
   // Rcupre le nom de l'appli.
   cApp   = strtok(NULL," ");
   if(cApp)
   {
      // Rcupre le N๘ de port.
      cNo = strtok(NULL,"");
      if(cNo)
      {
         nPort = atoi(cNo);
         nPort--;
         if((isdigit(*cNo)) && (nPort >= 0 && nPort < MAXPORTS))
         {
            if((tPorts[nPort].Actif) && (tPorts[nPort].Type == TPORTLOCAL))
            {
               strupr(cApp);
               pAp = SearchApp(cApp,0);
               if(pAp)
               {
                  if(pAp->Active)
                  {
                     if(! LOCConnect(cApp,nPort))
                     {
                        WinMsgErr(": CONNEXION IMPOSSIBLE","CONNECT %s",cApp);
                        WriteLogNeo("CONNECT %s: CONNEXION IMPOSSIBLE",cApp);
                        nErrCmd = 115;
                     }
                  }
                  else
                  {
                     WinMsgErr(": APPLICATION NON VALIDE","CONNECT %s",cApp);
                     WriteLogNeo("CONNECT %s: APPLICATION NON VALIDE",cApp);
                     nErrCmd = 114;
                  }
               }
               else
               {
                  WinMsgErr(": APPLICATION INEXISTANTE","CONNECT %s",cApp);
                  WriteLogNeo("CONNECT %s: APPLICATION INEXISTANTE",cApp);
                  nErrCmd = 113;
               }
            }
            else
            {
               WinMsgErr(": TYPE DU PORT NON VALIDE","CONNECT");
               WriteLogNeo("CONNECT: TYPE DU PORT NON VALIDE");
               nErrCmd = 112;
            }
         }
         else
         {
            WinMsgErr(": NUMERO DE PORT NON VALIDE","CONNECT");
            WriteLogNeo("CONNECT: NUMERO DE PORT NON VALIDE");
            nErrCmd = 111;
         }
      }
      else
      {
         WinMsgErr(": PARAMETRE MANQUANT","CONNECT");
         WriteLogNeo("CONNECT: PARAMETRE MANQUANT");
         nErrCmd = 110;
      }
   }
   else
   {
      WinMsgErr(": PARAMETRE MANQUANT","CONNECT");
      WriteLogNeo("CONNECT: PARAMETRE MANQUANT");
      nErrCmd = 110;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rappel des commandes disponibles au clavier.
//
void CmHELP(void)
{
   ClsWinInf(mtTRUE);
   bStatGen    = mtFALSE;
   bInfoGen    = mtTRUE;
   bAffCv      = mtFALSE;
   bAffCnx     = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCom     = mtFALSE;
   bInDebug    = mtFALSE;
   bNextStep   = mtFALSE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le prcdent.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinInfo("RAPPEL DES COMMANDES DISPONIBLES:");
   WinInfo(" ");
   WinInfo("QUIT");
   WinInfo("CLS");
   WinInfo("VIEW STAT, CNX, CV <n>, APP [<Nom>], PORTS, SYSINFO, VAR <Nom>, HISTORY, POINTES");
   WinInfo("SEND <msg> <n> | ALL");
   WinInfo("SET PALIER, COLOR, SYSTEM <Param>");
   WinInfo("LOAD <NomApp>");
   WinInfo("UNLOAD <NomApp>");
   WinInfo("LCX <n> | ALL | <NomApp>");
   WinInfo("DEBUG <n>");
   WinInfo("UNDEBUG");
   WinInfo("DEBUGSTEP <Pas>");
   WinInfo("CONNECT <NomApp> <nPort>");
   WinInfo("HELP");
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation d'une ou des applications prsentes.
//
void CmVAppli()
{
   mtPTRBYTE Param;

   // Rcupration de l'ventuel nom donn.
   Param = strtok(NULL," ");
   if(Param)
   {
      strupr(Param);
      pAffApp = SearchApp(Param,0);
      if(pAffApp)
      {
         ClsWinInf(mtTRUE);
         bStatGen    = mtFALSE;
         bInfoGen    = mtFALSE;
         bAffCv      = mtFALSE;
         bAffCnx     = mtFALSE;
         bAffApp     = mtTRUE;
         bAllApp     = mtFALSE;
         bAffHlp     = mtFALSE;
         bAffCom     = mtFALSE;
         bInDebug    = mtFALSE;
         bAffInfoSys = mtFALSE;
         bAffTPCnx   = mtFALSE;
         if(pCvTrace)
         {
            // On dsactive le mode trace.
            pCvTrace->bInDebug = mtFALSE;
            pCvTrace           = NULL;
         }
         WinAffApp();
      }
      else
      {
         WinMsgErr(": APPLICATION INEXISTANTE","VIEW APP %s",Param);
         WriteLogNeo("VIEW APP %s: APPLICATION INEXISTANTE",Param);
         nErrCmd = 230;
      }
   }
   else
   {
      bStatGen    = mtFALSE;
      bInfoGen    = mtFALSE;
      bAffCv      = mtFALSE;
      bAffCnx     = mtFALSE;
      bAffApp     = mtFALSE;
      bAffHlp     = mtFALSE;
      bAffCom     = mtFALSE;
      bAllApp     = mtTRUE;
      bInDebug    = mtFALSE;
      bAffInfoSys = mtFALSE;
      bAffTPCnx   = mtFALSE;
      if(pCvTrace)
      {
         // On dsactive le mode trace.
         pCvTrace->bInDebug = mtFALSE;
         pCvTrace           = NULL;
      }
      bPageApp = mtTRUE;
      pPageApp = 0;
      WinAllApp();
   }
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des informations d'une application.
//
void WinAffApp()
{
   char VBuff[81];
   int LAffapp=10,i;
   mtPTRSTAPP ps;
   tTmp tT;
   mtUDWORD sT;
   float sTarif,fT;

   if(bAffApp)
   {
      sprintf(VBuff,"ETAT DE L'APPLICATION %s",pAffApp->Nom);
      if(pAffApp->bOnLib)
         strcat(VBuff," (ONLIB actif)");
      if(pAffApp->bGTask)
         strcat(VBuff," (GTASK actif)");
      VPrint(4,((80-strlen(VBuff))/2),VBuff,CoulInf);
      sprintf(VBuff,"Nombre total d'appels : %6lu",pAffApp->NbAppTot);
      VPrint(6,0,VBuff,CoulInf);
      sprintf(VBuff,"Cnx actives = %3d (Max = %3d)",pAffApp->NbCnx,pAffApp->PointeCnx);
      VPrint(6,51,VBuff,CoulInf);
      sprintf(VBuff,"Premire connexion …  : %02dh%02dm%02ds",pAffApp->HPreCnx.hour,pAffApp->HPreCnx.minute,pAffApp->HPreCnx.second);
      VPrint(7,0,VBuff,CoulInf);
      sprintf(VBuff,"Dernire connexion … : %02dh%02dm%02ds",pAffApp->HDerCnx.hour,pAffApp->HDerCnx.minute,pAffApp->HDerCnx.second);
      VPrint(7,48,VBuff,CoulInf);
      ps = pAffApp->pStat;
      if(ps)
      {
         sprintf(VBuff,"    Palier  ณ Appels ณ Temps de cnx ณ   T.M.C   ณ Revt. ณ   Gain                ");
         VPrint(9,0,VBuff,CoulBdx);
         while(ps)
         {
            sprintf(VBuff,"%d",ps->Palier);
            VPrint(LAffapp,3,VBuff,CoulInf);
            VPrint(LAffapp,12,"ณ",CoulInf);
            sprintf(VBuff,"%6lu",ps->NbAppel);
            VPrint(LAffapp,14,VBuff,CoulInf);
            VPrint(LAffapp,21,"ณ",CoulInf);
            SecToTime(ps->TpsCnx,&tT);
            sprintf(VBuff,"%5dh%02dm%02ds",tT.H,tT.M,tT.S);
            VPrint(LAffapp,23,VBuff,CoulInf);
            VPrint(LAffapp,36,"ณ",CoulInf);
            // Calcul du Temps Moyen de Connexion
            if(ps->NbAppel)
            {
               sT = ps->TpsCnx/ps->NbAppel;
               SecToTime(sT,&tT);
               sprintf(VBuff,"%2dh%02dm%02ds",tT.H,tT.M,tT.S);
               VPrint(LAffapp,38,VBuff,CoulInf);
            }
            // Recherche du montant revers du palier donn.
            sTarif = (float) 0;
            for(i=1;i<MAXPALIER;i++)
            {
               if(StatPalier[i].Palier == ps->Palier)
               {
                  sTarif = StatPalier[i].Tarif;
               }
            }
            VPrint(LAffapp,48,"ณ",CoulInf);
            sprintf(VBuff,"%5.2f",sTarif);
            VPrint(LAffapp,50,VBuff,CoulInf);
            VPrint(LAffapp,56,"ณ",CoulInf);
            // Calcul du gain
            if(ps->TpsCnx)
            {
               fT = ps->TpsCnx/(float) 60;     // On converti en minutes.
               sprintf(VBuff,"%9.2f",sTarif*fT);
               VPrint(LAffapp,58,VBuff,CoulInf);
            }
            ps = ps->pNextSt;
            LAffapp++;
            if(LAffapp > 21)
               break;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// totalisations diverses sur les applications prsentes.
//
void TotalAllApp(void)
{
   mtPTRAPP pAp=pAppli;

   NbAllLib     = 0;
   NbAllApp     = 0;
   SizeAllCode  = 0;
   SizeAllPData = 0;
   SizeAllLData = 0;
   while(pAp)
   {
      if(pAp->Active)
         NbAllApp++;
      else
         NbAllLib++;
      SizeAllCode  += pAp->SizeCode;
      SizeAllPData += pAp->SizePData;
      SizeAllLData += pAp->SizeLData;
      pAp = pAp->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des informations des applications prsentes.
//
void WinAllApp()
{
   char VBuff[81];
   static mtPTRAPP pApp=(mtPTRAPP) 0;
   int LAffapp=7,i=0;

   if((bAllApp) && (bPageApp))
   {
      ClsWinInf(mtTRUE);
      TotalAllApp();
      nbAffApp = 0;
      if(! pApp)
         pApp=pAppli;
      sprintf(VBuff,"ETAT DES APPLICATIONS");
      VPrint(4,30,VBuff,CoulInf);
      sprintf(VBuff,"LIB = %3d APP = %3d Code = %6d Publiques = %6d Locales = %6d",NbAllLib,NbAllApp,SizeAllCode,SizeAllPData,SizeAllLData);
      VPrint(5,5,VBuff,CoulInf);
      sprintf(VBuff,"    Nom    ณ  Type  ณ N.C. ณ  Code  ณ  Publiques   ณ Locales                    ");
      VPrint(6,0,VBuff,CoulBdx);
      pApp = pAppli;
      if(pApp && pPageApp)
      {
         while(i < (pPageApp*nAppPage))
         {
            if(! pApp->pNextAPP)
            {
               // On est sur la dernire appli
               break;
            }
            else
               pApp = pApp->pNextAPP;
            i++;
         }
      }
      while(pApp)
      {
         sprintf(VBuff,"%s",pApp->Nom);
         VPrint(LAffapp,0,VBuff,CoulInf);
         VPrint(LAffapp,11,"ณ",CoulInf);
         sprintf(VBuff,"%s",(pApp->Active ? "APP" : "LIB"));
         VPrint(LAffapp,14,VBuff,CoulInf);
         VPrint(LAffapp,20,"ณ",CoulInf);
         sprintf(VBuff,"%2d",pApp->NumComp);
         VPrint(LAffapp,23,VBuff,CoulInf);
         VPrint(LAffapp,27,"ณ",CoulInf);
         sprintf(VBuff,"%6d",pApp->SizeCode);
         VPrint(LAffapp,29,VBuff,CoulInf);
         VPrint(LAffapp,36,"ณ",CoulInf);
         sprintf(VBuff,"(%3d) %6d",pApp->uPDico,pApp->SizePData);
         VPrint(LAffapp,38,VBuff,CoulInf);
         VPrint(LAffapp,51,"ณ",CoulInf);
         sprintf(VBuff,"(%3d) %6d",pApp->uLDico,pApp->SizeLData);
         VPrint(LAffapp,53,VBuff,CoulInf);
         pApp = pApp->pNextAPP;
         nbAffApp++;
         LAffapp++;
         if(LAffapp > 20)
            break;
      }
      bPageApp = mtFALSE;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation de l'tat des ports de communication.
//
void CmVPorts()
{
   ClsWinInf(mtTRUE);
   bAffCom     = mtTRUE;
   bStatGen    = mtFALSE;
   bInfoGen    = mtFALSE;
   bAffCv      = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCnx     = mtFALSE;
   bInDebug    = mtFALSE;
   bAffInfoSys = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinAffCom();
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation des informations systme.
//
void CmVAffInfSys()
{
   ClsWinInf(mtTRUE);
   bAffInfoSys = mtTRUE;
   bAffCom     = mtFALSE;
   bStatGen    = mtFALSE;
   bInfoGen    = mtFALSE;
   bAffCv      = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCnx     = mtFALSE;
   bInDebug    = mtFALSE;
   bAffTPCnx   = mtFALSE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinAffInfSys();
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Visualisation des pointes journalires.
//
void CmVAffTPCnx()
{
   ClsWinInf(mtTRUE);
   bAffInfoSys = mtFALSE;
   bAffCom     = mtFALSE;
   bStatGen    = mtFALSE;
   bInfoGen    = mtFALSE;
   bAffCv      = mtFALSE;
   bAffApp     = mtFALSE;
   bAllApp     = mtFALSE;
   bAffHlp     = mtFALSE;
   bAffCnx     = mtFALSE;
   bInDebug    = mtFALSE;
   bAffTPCnx   = mtTRUE;
   if(pCvTrace)
   {
      // On dsactive le mode trace.
      pCvTrace->bInDebug = mtFALSE;
      pCvTrace           = NULL;
   }
   WinAffTPCnx();
   nErrCmd = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des informations systmes
//
void WinAffInfSys()
{
   static char *TCpu[] = { "8088/8086","80186","80286","80386/80386sx","80486","Pentium" };
   char VBuff[81];
   int i;

   if(bAffInfoSys)
   {
      memset(VBuff,0,81);
      sprintf(VBuff,"%s, Concepteur : %s",&cBann[16],cDecrypt(cMOI,strlen(cMOI)));
      VPrint(4,((80-strlen(VBuff))/2),VBuff,CoulInf);
      i = cputype();
      if(_8087)
         sprintf(VBuff,"ๆP : %s (Coprocesseur prsent)",TCpu[i]);
      else
         sprintf(VBuff,"ๆP : %s",TCpu[i]);
      VPrint(6,0,VBuff,CoulInf);
      sprintf(VBuff,"MS D.O.S version : %1d.%02d",_osmajor,_osminor);
      VPrint(6,57,VBuff,CoulInf);
//      sprintf(VBuff,"Vitesse moyenne d'excution du code principal  : %5.1f/%5.1f ms",TmtA[0],TmtM[0]);
//      VPrint(8,0,VBuff,CoulInf);
//      sprintf(VBuff,"Vitesse moyenne d'excution des tches de fond : %5.1f/%5.1f ms",TmtA[1],TmtM[1]);
//      VPrint(9,0,VBuff,CoulInf);
      sprintf(VBuff,"Arret automatique = ");
      VPrint(8,0,VBuff,CoulInf);
      if(bArretAuto)
         sprintf(VBuff,"OUI");
      else
         sprintf(VBuff,"NON");
      VPrint(8,20,VBuff,CoulInf);
      sprintf(VBuff,"PATHAPPS          = ");
      VPrint(9,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(9,20,VBuff,CoulInf);
      if(PathAPPS)
         VPrint(9,20,PathAPPS,CoulInf);
      else
         VPrint(9,20,"Aucun chemin spcifi",CoulInf);
      sprintf(VBuff,"BIPONCNX          = ");
      VPrint(10,0,VBuff,CoulInf);
      if(bBipOnCnx)
         sprintf(VBuff,"OUI");
      else
         sprintf(VBuff,"NON");
      VPrint(10,20,VBuff,CoulInf);
      sprintf(VBuff,"SAUVEECRAN        = ");
      VPrint(11,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(11,20,VBuff,CoulInf);
      if(wSaveScreen)
         sprintf(VBuff,"%d mn",(int) (wSaveScreen/60));
      else
         sprintf(VBuff,"Pas de sauvegarde");
      VPrint(11,20,VBuff,CoulInf);
      sprintf(VBuff,"AUTOCNX           = ");
      VPrint(12,0,VBuff,CoulInf);
      if(bAutoCNX)
         sprintf(VBuff,"OUI");
      else
         sprintf(VBuff,"NON");
      VPrint(12,20,VBuff,CoulInf);
      sprintf(VBuff,"APPAUTOCNX        = ");
      VPrint(13,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(13,20,VBuff,CoulInf);
      if(pAppAutoCNX)
         VPrint(13,20,pAppAutoCNX,CoulInf);
      sprintf(VBuff,"SAUVEBASES        = ");
      VPrint(14,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(14,20,VBuff,CoulInf);
      if(wSaveBase)
         sprintf(VBuff,"%d mn",(int) (wSaveBase/60));
      else
         sprintf(VBuff,"Pas de sauvegarde");
      VPrint(14,20,VBuff,CoulInf);
      sprintf(VBuff,"TRACEPAQUET       = ");
      VPrint(15,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(15,20,VBuff,CoulInf);
      if(bTracePaq)
         sprintf(VBuff,"OUI (fichier %s)",pFicPAQ);
      else
         sprintf(VBuff,"Pas de trace des paquets");
      VPrint(15,20,VBuff,CoulInf);
      sprintf(VBuff,"VERIFCNX          = ");
      VPrint(16,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(16,20,VBuff,CoulInf);
      if(wVerCNX)
         sprintf(VBuff,"%d mn",(int) (wVerCNX/60));
      else
         sprintf(VBuff,"Pas de vrification");
      VPrint(16,20,VBuff,CoulInf);
      sprintf(VBuff,"TRACESERVEUR      = ");
      VPrint(17,0,VBuff,CoulInf);
      // On efface la ligne avant
      memset(VBuff,0,81);
      memset(VBuff,' ',59);
      VPrint(17,20,VBuff,CoulInf);
      if(bTraceNeo)
         sprintf(VBuff,"OUI (fichier %s)",pFicNEO);
      else
         sprintf(VBuff,"Pas de trace du serveur");
      VPrint(17,20,VBuff,CoulInf);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des pointes journalires.
//
void WinAffTPCnx()
{
   static char *Tb[] = {"00-04","05-09","10-14","15-19","20-24","25-29","30-34","35-39","40-44","45-49","50-54","55-59"};
   int i,j;
   char VBuff[81];
   int LAff=7;

   if(bAffTPCnx)
   {
      sprintf(VBuff,"ETAT DES POINTES JOURNALIERES");
      VPrint(4,25,VBuff,CoulInf);
      // Affichage du cadre.
      sprintf(VBuff,"   H.ณ00ณ01ณ02ณ03ณ04ณ05ณ06ณ07ณ08ณ09ณ10ณ11ณ12ณ13ณ14ณ15ณ16ณ17ณ18ณ19ณ20ณ21ณ22ณ23   ");
      VPrint(LAff,0,VBuff,CoulBdx);
      LAff++;
      sprintf(VBuff,"Mn.  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ     ");
      VPrint(LAff,0,VBuff,CoulBdx);
      LAff++;
      for(i=0;i<12;i++)
      {
         VPrint(LAff+i,0,Tb[i],CoulBdx);
         VPrint(LAff+i,5,"ณ",CoulBdx);
         sprintf(VBuff,"  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ  ณ     ");
         VPrint(LAff+i,6,VBuff,CoulInf);
         for(j=0;j<24;j++)
         {
            sprintf(VBuff,"%02d",TPointeCnx[i][j]);
            VPrint(LAff+i,6+(j*3),VBuff,CoulInf);
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des informations des ports de communication.
//
void WinAffCom()
{
   int i;
   char VBuff[81];
   int LAffcom=6;

   if(bAffCom)
   {
      sprintf(VBuff,"ETAT DES PORTS DE COMMUNICATION");
      VPrint(4,24,VBuff,CoulInf);
      for(i = 0;i<MAXPORTS;i++)
      {
         if(tPorts[i].Actif)
         {
            sprintf(VBuff,"Port: %1d",i+1);
            VPrint(LAffcom,0,VBuff,CoulInf);
            sprintf(VBuff,", Type= ");
            switch(tPorts[i].Type)
            {
               case TPORTX25ASM : { strcat(VBuff,"X25-ASM") ; break ; }
               case TPORTLOCAL  : { strcat(VBuff,"LOCAL") ; break ; }
               case TPORTRTC    :
               {
                  strcat(VBuff,"RTC (");
                  strcat(VBuff,tPorts[i].Nom);
                  strcat(VBuff,")");
                  break;
               }
            }
            VPrint(LAffcom,8,VBuff,CoulInf);
            sprintf(VBuff,", Speed= %5lu bauds",tPorts[i].SpeedCom);
            VPrint(LAffcom,56,VBuff,CoulInf);
            LAffcom++;
            if(LAffcom > 21)
               break;
            sprintf(VBuff,"Parit= ");
            switch(tPorts[i].Parite)
            {
               case SWPARITYNONE : { strcat(VBuff,"SANS")    ; break ; }
               case SWPARITYODD  : { strcat(VBuff,"IMPAIRE") ; break ; }
               case SWPARITYEVEN : { strcat(VBuff,"PAIRE")   ; break ; }
            }
            VPrint(LAffcom,8,VBuff,CoulInf);
            sprintf(VBuff,", Len= %1d bits",tPorts[i].WordLen);
            VPrint(LAffcom,24,VBuff,CoulInf);
            sprintf(VBuff,", Bit de stop= %1d",tPorts[i].StopBit);
            VPrint(LAffcom,38,VBuff,CoulInf);
            LAffcom++;
            if(LAffcom > 21)
               break;
            sprintf(VBuff,"Paquets In= %10lu",tPorts[i].NbPaqLu);
            VPrint(LAffcom,8,VBuff,CoulInf);
            sprintf(VBuff,", Out= %10lu",tPorts[i].NbPaqOut);
            VPrint(LAffcom,31,VBuff,CoulInf);
            sprintf(VBuff,", Err= %10lu",tPorts[i].NbPaqErr);
            VPrint(LAffcom,49,VBuff,CoulInf);
            LAffcom += 2;
            if(LAffcom > 21)
               break;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

