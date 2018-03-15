// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณNEOSERV.C    ณ                                                             ณ
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
// ณDernire modification : 21/03/1996                                         ณ
// ณ                                                                           ณ
// ณPROGRAMME PRINCIPAL                                                        ณ
// ณ                                                                           ณ
// ณBut: Permettre l'excution de programmes, crits dans le langage de        ณ
// ณ     dveloppement intgr … NoServ, en fournissant un support            ณ
// ณ     d'excution multi-tches, multi-utilisateurs et multi-mdia (gestion  ณ
// ณ     de flux en provenance du rseau Tltel (minitels), Tlphonique      ณ
// ณ     (R.T.C), Transpac (X25).                                              ณ
// ณ     Un systme de Gestion de Bases de Donnes Relationnelles est intgr  ณ
// ณ     … NoServ, permettant la gestion de bases de donnes … la norme       ณ
// ณ     FOXPRO(tm).                                                           ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

extern unsigned int _stack = 0xFFFF;

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dos.h>
#include <malloc.h>
#include <disp.h>
#include <mem.h>
#include <direct.h>
#include <io.h>
#include <bios.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "minitel.h"
#include "ports.h"
#include "pdata.h"

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// VARIABLES GLOBALES.
// อออออออออออออออออออ
//

mtBYTE        *cMOI     = "Y7P^RUUJ^VJ^M";
mtBYTE        *Banniere = "$I$_))))))))))))WNX\\N[_):7:799";
mtBYTE        cBann[81];
char          *ExtPrg   = "NPG";       // Extension par dfaut des sources.
char          *PathAPPS;               // Chemin des applis
mtUWORD       wCurrCv;                 // Cv courant.
UnCv          TabCv[TOTALCV];          // Tableau des connects.
UnCv          LTask[TOTALCV];          // Tableau des tches locales.
UnCv          OLCv[TOTALCV];           // Tableau des tches ONLIB.
pPDATA        pSDATA;                  // Pile des paquets.
mtUWORD       iSDATA;                  // SP de pSDATA.
mtUWORD       nPgeSD;                  // Nombre de pages dans pSDATA.
mtPTRCV       pCurrCV;                 // Pointe sur le cv courant.
mtPTRCV       pCurrLT;                 // Pointe sur la tche locale du cv courant.
mtBYTE        Heures;                  // Date
mtBYTE        Minutes;                 // et
mtBYTE        Secondes;                // Heure
mtBYTE        Jour;                    // courante.
mtBYTE        Mois;
mtWORD        Annee;
struct        dos_time_t HFinServ;     // Heure d'arret du serveur.
mtBOOL        bArretAuto=mtTRUE;       // Vrai si heure d'arret active.
mtBOOL        bBipOnCnx=mtFALSE;       // Vrai si beep … chaque connexion.
mtBOOL        bCreateDbSys=mtTRUE;     // Vrai si on peut crer les bases systmes au lancement.
mtBOOL        mtFin=mtFALSE;           // Vrai si Fin du programme.
mtBOOL        bTracePaq=mtFALSE;       // Vrai si on trace les paquets dans le fichier pFicPAQ;
char          *pFicPAQ="TRAMES.LOG";
mtBOOL        bTraceNeo=mtFALSE;       // Vrai si on trace l'activit du serveur dans la fichier pFicNEO.
char          *pFicNEO="NEOSERV.LOG";
int           NbCnx=0;                 // Nombre de connects actuel.
int           PointeCnx=0;             // Nombre de connects maxi.
int           TPointeCnx[12][24];      // Pointes de la journe courante.
                                       // (par tranche de 5 mn).
float         ChargeA=(float) 0;       // Charge actuelle.
float         ChargeM=(float) 0;       // Charge Maxi.
struct        dos_date_t DLanc;        // Date et
struct        dos_time_t HLanc;        // Heure de lancement du serveur.
mtUDWORD      RAMDispo;
mtDWORD       NbPassage[2]={0L,0L};                 // Utiliss pour le calcul
float         TpPassage[2]={(float) 0,(float) 0}; // du Temps Moyen de Traitement.
struct        dos_time_t DebTrait[2];
struct        dos_time_t FinTrait[2];
float         TmtA[2];                 // T.M.T actuel.
float         TmtM[2];                 // T.M.T maxi.
mtBOOL        bPaquet=mtFALSE;         // Vrai si on a lu un paquet.
struct        dos_time_t HPreCnx;      // Heure de la premire connexion.
struct        dos_time_t HDerCnx;      // Heure de la dernire connexion.
mtUDWORD      NbAppTot=(mtUDWORD) 0;   // Nombre total d'appels.
mtBOOL        bStatGen=mtFALSE;        // Utiliss par les diverses
mtBOOL        bInfoGen=mtTRUE;         // fonctions d'affichage.
mtBOOL        bAffCnx=mtFALSE;
mtBOOL        bAffCv=mtFALSE;
mtBOOL        bAffApp=mtFALSE;
mtBOOL        bAllApp=mtFALSE;
mtBOOL        bAffHlp=mtFALSE;
mtBOOL        bAffCom=mtFALSE;
mtBOOL        bInDebug=mtFALSE;
mtBOOL        bNextStep=mtFALSE;
mtBOOL        bAffInfoSys=mtFALSE;
mtBOOL        bAffTPCnx=mtFALSE;
mtBOOL        bAutoCNX=mtFALSE;        // Auto-connexion sur pAppAutoCNX
char          *pAppAutoCNX;            // Chemin complet de l'appli.
char          *NomAppAutoCNX;          // Nom de l'appli.
jmp_buf       EXECERROR;               // Retour sur erreur d'excution.
char          TLoadFic[MAXFICLOAD][80];
mtUWORD       iLoad;
mtBOOL        bInCompile=mtFALSE;      // VRAI si on est en compilation.
int           PrnStatus=0;             // Status de l'imprimante.
int           LPT=0;                   // Numro de l'imprimante (0=LPT1,1=LPT2,...).
unsigned long lChkMOI   = 0x0000042DL;
unsigned long lChkBANN  = 0x00000739L;
char          cNameEXE[80];
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   Ports     tPorts[MAXPORTS];
extern   mtPTRAPP  pAppli;                  // Pointe sur la 1re application.
extern   mtUWORD   uGDico;                  // Nombre de symboles dans pGlobalDic.
extern   void      clrscr(void);
extern   void      SWVSetCursor(mtBOOL);
extern   mtBOOL    ReadIni(void);
extern   mtBOOL    OpenPorts(void);
extern   void      ClosePorts(void);
extern   mtBOOL    ReadPort(void);
extern   void      LibPort(void);
extern   void      InitialiseAllPort(void);
extern   void      VideCvBuffer(int);
extern   void      VideBuffer(void);
extern   void      TimerInit(void);
extern   void      TimerEnd(void);
extern   void      InitException(void);
extern   void      EndException(void);
extern   void      InitWin(void);
extern   void      EndWin(void);
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      ReadKeyboard(void);
extern   void      InitKeyboard(void);
extern   void      VPrint(mtBYTE,mtBYTE,mtPTRBYTE,mtUSHORT);
extern   void      ClsAllWin(void);
extern   void      DelAllApp(void);
extern   void      DelGlobalDic(void);
extern   void      InitGlobalDic(void);
extern   void      ExecCode(void);
extern   void      ProcessFKey(void);
extern   void      ProcessKeyW(void);
extern   void      DeconnecteCv(mtPTRCV);
extern   void      DeconnecteGTCv(mtPTRAPP);
extern   float     TimeToCent(struct dos_time_t *);
extern   void      CalcTMT(void);
extern   void      RefreshScreen(void);
extern   void      MemFree(mtPOINTEUR);
extern   void      GarbageCollector(void);
extern   void      SWCPurgeTransmitQueue(int);
extern   void      LoadFichier(void);
extern   mtBOOL    bScrSave;
extern   mtWORD    wSaveScreen;
extern   void      ScreenOn(void);
extern   void      InitBaseSys(void);
extern   void      EndBaseSys(void);
extern   mtBOOL    OpenBaseSys(void);
extern   void      CloseBaseSys(void);
extern   void      SauveInfoSys(void);
extern   void      RecupInfoSys(void);
extern   void      IlEstMinuit(void);
extern   void      SauveSysPaliers(void);
extern   void      RecupSysPaliers(void);
extern   void      SauveAllAppStat(void);
extern   void      SauveAllAppSysInfo(void);
extern   void      LoadAutoApp(void);
extern   void      InitNbFiles(void);
extern   void      RefreshBaseSys(void);
extern   void      Send0(mtPTRBYTE,mtPTRCV);
extern   void      VerifCNX(void);
extern   mtWORD    wVerCNX;
extern   mtBOOL    OpenLogPaq(void);
extern   void      CloseLogPaq(void);
extern   mtBOOL    OpenLogNeo(void);
extern   void      CloseLogNeo(void);
extern   void      WriteLogNeo(mtPTRBYTE,...);
extern   void      RefreshBaseApp(void);
extern   void      FOSSILGetInfo(int);
extern   void      ProcessCmdLine(int,char **);
extern   void      FreeCvOutB(mtPTRCV);
extern   mtBOOL    IsShareInstalled(void);
extern   void      FreeSDATA(void);
extern   void      GetPaquet(mtUWORD);
extern   void      SetXXEncode(void);
extern   void      SetUUEncode(void);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cryptage de la chaine donne.
//
mtPTRBYTE cCrypt(mtPTRBYTE pCh,mtWORD Len)
{
   static mtBYTE cBuff[1024];
   mtBYTE cClef=9;
   register i;

   for(i=0;i<Len;i++)
      cBuff[i] = pCh[i] + cClef;
   return(&cBuff[0]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcryptage de la chaine donne.
//
mtPTRBYTE cDecrypt(mtPTRBYTE pCh,mtWORD Len)
{
   static mtBYTE cBuff[1024];
   mtBYTE cClef=9;
   register i;

   memset(cBuff,0,1024);
   for(i=0;i<Len;i++)
      cBuff[i] = pCh[i] - cClef;
   return(&cBuff[0]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Calcul du CheckSum d'une chaine donne.
//
mtUDWORD CalcChkSum(mtPTRBYTE pCh,mtWORD Len)
{
   register i;
   mtUDWORD rCheck=0L;

   for(i=0;i<Len;i++)
      rCheck += pCh[i];
   return(rCheck);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Une petite vrification de paternit!.
//
mtBOOL VerifPaternite(void)
{
   mtUDWORD ulCheckM=0L,ulCheckB=0L;

   ulCheckM = CalcChkSum(cMOI,strlen(cMOI));
   if(ulCheckM != lChkMOI)
      return(mtFALSE);
   ulCheckB = CalcChkSum(Banniere,strlen(Banniere));
   if(ulCheckB != lChkBANN)
      return(mtFALSE);
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Remise … 0 du tableau des pointes journalires.
//
void RAZTPointe(void)
{
   int i,j;

   for(i=0;i<12;i++)
   {
      for(j=0;j<24;j++)
         TPointeCnx[i][j] = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du code des connects.
//
void TraiteCV(void)
{
   wCurrCv = 0;
   while(wCurrCv<TOTALCV)
   {
      pCurrCV = &TabCv[wCurrCv];
      pCurrLT = &LTask[wCurrCv];
      GetPaquet(wCurrCv);
      if(pCurrCV->Actif && pCurrCV->pApp)
      {
         if((pCurrCV->bPaqLu) && (pCurrCV->pApp->bAnticipe))
            FreeCvOutB(pCurrCV);
         _dos_gettime(&DebTrait[0]);
         pCurrCV->IsLTask   = mtFALSE;
         pCurrCV->IsGTask   = mtFALSE;
         pCurrCV->IsOnLib   = mtFALSE;
         pCurrCV->TimeExec  = MAXEXEC;
         pCurrCV->ExecError = mtFALSE;
         ProcessKeyW();
         if(! pCurrCV->bKeyWOk)
            // On n'a pas trouv de mot-clef,
            // on peut tester les touches de fonction.
            ProcessFKey();
         ExecCode();
         _dos_gettime(&FinTrait[0]);
         NbPassage[0] += pCurrCV->NbIPS;
      }
      else
         pCurrCV->Actif = mtFALSE;
      wCurrCv++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du code des tches de fond locales.
//
void TraiteLocalTask(void)
{
   wCurrCv = 0;
   while(wCurrCv<TOTALCV)
   {
      pCurrCV = &LTask[wCurrCv];
      pCurrLT = &TabCv[wCurrCv];
      if(pCurrCV->Actif && pCurrCV->pApp)
      {
         _dos_gettime(&DebTrait[1]);
         pCurrCV->IsLTask   = mtTRUE;
         pCurrCV->IsGTask   = mtFALSE;
         pCurrCV->IsOnLib   = mtFALSE;
         pCurrCV->TimeExec  = MAXEXEC;
         pCurrCV->ExecError = mtFALSE;
         ExecCode();
         _dos_gettime(&FinTrait[1]);
         NbPassage[1] += pCurrCV->NbIPS;
      }
      else
         pCurrCV->Actif = mtFALSE;
      wCurrCv++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du code des tches ONLIB.
//
void TraiteONLIBTask(void)
{
   wCurrCv = 0;
   while(wCurrCv<TOTALCV)
   {
      pCurrCV = &OLCv[wCurrCv];
      if(pCurrCV->Actif && pCurrCV->pApp)
      {
         pCurrCV->IsLTask   = mtFALSE;
         pCurrCV->IsGTask   = mtFALSE;
         pCurrCV->IsOnLib   = mtTRUE;
         pCurrCV->TimeExec  = MAXEXEC;
         pCurrCV->ExecError = mtFALSE;
         ExecCode();
      }
      else
         pCurrCV->Actif = mtFALSE;
      wCurrCv++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement des tche globales.
//
void TraiteGTaskCV(void)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      if((p->Active) && (p->bGTask))
      {
         pCurrCV = (mtPTRCV) p->pGTCv;
         if(pCurrCV->Actif)
         {
            pCurrCV->IsLTask   = mtFALSE;
            pCurrCV->IsGTask   = mtTRUE;
            pCurrCV->IsOnLib   = mtFALSE;
            pCurrCV->TimeExec  = MAXEXEC;
            pCurrCV->ExecError = mtFALSE;
            ExecCode();
         }
      }
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                          LE PROGRAMME PRINCIPAL
//                          ออออออออออออออออออออออ
//
int main(int argc,char **argv)
{
   char OBuff[40];
   int i,j,JCode;
   int c;
   int iMn;

   strcpy(cNameEXE,argv[0]);
   strcpy(cBann,cDecrypt(Banniere,strlen(Banniere)));
   if(! VerifPaternite())
   {
      printf("\n");
      printf("****************************************\n");
      printf("*           A T T E N T I O N          *\n");
      printf("****************************************\n");
      printf("*         NEOSERV A ETE ALTERE         *\n");
      printf("*   (UN VIRUS EST PEUT-ETRE PRESENT)   *\n");
      printf("****************************************\n");
      printf("\n");
      return(EXIT_FAILURE);
   }
   if(! IsShareInstalled())
   {
      printf("\nInstallez SHARE avant de lancer NEOSERV\n");
      return(EXIT_FAILURE);
   }
   _dos_getdate(&DLanc);
   _dos_gettime(&HLanc);
   randomize();
   RAZTPointe();
   for(wCurrCv=0;wCurrCv<TOTALCV;wCurrCv++)
   {
      memset(&TabCv[wCurrCv],0,sizeof(UnCv));
      memset(&LTask[wCurrCv],0,sizeof(UnCv));
      memset(&OLCv[wCurrCv],0,sizeof(UnCv));
   }
   TmtA[0] = 0.00;
   TmtA[1] = 0.00;
   TmtM[0] = 0.00;
   TmtM[1] = 0.00;
   wCurrCv = 0;
   mem_init();
   disp_open();
   clrscr();
   SWVSetCursor(mtFALSE);
   SetXXEncode();
   SetUUEncode();
   pAppli  = NULL;
   pSDATA  = NULL;
   iSDATA  = 0;
   nPgeSD  = 1;
   // Allocation de la 1re page de la pile des paquets.
   pSDATA  = (pPDATA) MemAlloc((nPgeSD*SDATAPAGE)*sizeof(mtPTRPDATA));
   PrnStatus = biosprint(_PRINTER_INIT,0,LPT);
   printf("Initialisation des Fentres...\n");
   InitWin();
   InitKeyboard();

   InitNbFiles();
   // Heure d'arret par dfaut : 04:00 du matin
   HFinServ.hour    = 04;
   HFinServ.minute  = 00;
   HFinServ.second  = 00;
   HFinServ.hsecond = 00;
   iMn              = 0;
   // Sauvegarde de l'cran par dfaut toutes les 5 mn.
   wSaveScreen      = 300;
   // Vrification des connects par dfaut toutes les 5 mn.
   wVerCNX          = 300;
   WinInfo("Lecture de NEOSERV.INI...");
   if(! ReadIni())
   {
      WinInfo("Erreur … l'initialisation du serveur");
      SWVSetCursor(mtTRUE);
      return(EXIT_FAILURE);
   }
   //
   // Ouverture du fichier de trace de noserv.
   //
   if(bTraceNeo)
   {
      if(! OpenLogNeo())
      {
         WinInfo("Erreur … l'ouverture du fichier %s",pFicNEO);
         bTraceNeo = mtFALSE;
      }
   }
   WriteLogNeo("*****> LANCEMENT DE NEOSERV LE %02d/%02d/%04d A %02d:%02d",DLanc.day,DLanc.month,DLanc.year,HLanc.hour,HLanc.minute);
   WinInfo("Initialisation des Exceptions...");
   WriteLogNeo("Initialisation des Exceptions...");
   InitException();
   WinInfo("Initialisation du Dictionnaire Global...");
   WriteLogNeo("Initialisation du Dictionnaire Global...");
   InitGlobalDic();
   WinInfo("%d symboles globaux",uGDico);
   WriteLogNeo("%d symboles globaux",uGDico);
   WinInfo("Initialisation des Bases Systme...");
   WriteLogNeo("Initialisation des Bases Systme...");
   InitBaseSys();
   WinInfo("Ouverture des Bases Systme...");
   WriteLogNeo("Ouverture des Bases Systme...");
   if (! OpenBaseSys())
   {
      EndBaseSys();
      EndException();
      WinInfo("Erreur … l'ouverture des bases systmes");
      WriteLogNeo("Erreur … l'ouverture des bases systmes");
      CloseLogNeo();
      SWVSetCursor(mtTRUE);
      return(EXIT_FAILURE);
   }
   WinInfo("Ouverture des Canaux...");
   WriteLogNeo("Ouverture des Canaux...");
   if (! OpenPorts())
   {
      EndBaseSys();
      EndException();
      WinInfo("Erreur … l'ouverture des canaux");
      WriteLogNeo("Erreur … l'ouverture des canaux");
      CloseLogNeo();
      SWVSetCursor(mtTRUE);
      return(EXIT_FAILURE);
   }
   WinInfo("Initialisation de l'Horloge Systme...");
   WriteLogNeo("Initialisation de l'Horloge Systme...");
   TimerInit();
   WinInfo("Recupration des Informations Systme...");
   WriteLogNeo("Recupration des Informations Systme...");
   RecupInfoSys();
   WinInfo("Recupration des Paliers Systme...");
   WriteLogNeo("Recupration des Paliers Systme...");
   RecupSysPaliers();
   WinInfo("Initalisation des Canaux...");
   WriteLogNeo("Initalisation des Canaux...");
   InitialiseAllPort();

   RAMDispo = _x386_coreleft();
   VPrint(0,0,&cBann[16],SWVBLDATR(SWBLACK,SWWHITE));
   //
   // Raffraichissement des affichages.
   //
   RefreshScreen();
   //
   // Chargement des ventuels fichiers spcifis par la section [Load]
   //
   LoadFichier();
   LoadAutoApp();
   //
   // Traitement de l'ventuelle ligne de commande.
   //
   ProcessCmdLine(argc,argv);
   //
   // Ouverture du fichier de trace des paquets.
   //
   if(bTracePaq)
   {
      if(! OpenLogPaq())
      {
         WinInfo("Erreur … l'ouverture du fichier %s",pFicPAQ);
         WriteLogNeo("Erreur … l'ouverture du fichier %s",pFicPAQ);
         bTracePaq = mtFALSE;
      }
   }
   //
   // Traitement des erreurs d'excution du code.
   //
   JCode = setjmp(EXECERROR);
   if(JCode != 0)
   {
      WinInfo("EXECERROR");
      // Erreur d'excution
      if(! bInCompile)
      {
         if(pCurrCV)
         {
            if((! pCurrCV->IsLTask) && (! pCurrCV->IsOnLib) && (! pCurrCV->IsGTask))
            {
               // On purge le buffer de transmission.
               VideCvBuffer(pCurrCV->NumCv);
               if(tPorts[pCurrCV->NumPort].Type != TPORTX25MITEC)
                  SWCPurgeTransmitQueue(pCurrCV->NumPort);
               tPorts[pCurrCV->NumPort].LibereCV(pCurrCV->NumCv);
            }
            else
            {
               // On libre le cv courant.
               if(pCurrCV->IsGTask)
                  DeconnecteGTCv(pCurrCV->pApp);
               else
                  DeconnecteCv(pCurrCV);
            }
         }
      }
   }
   // WinInfo("Stack=%u",_stack);
   //
   // Boucle principale
   //
   while(! mtFin)
   {
      //
      // Lecture/Traitement d'un paquet entrant.
      //
      bPaquet = ReadPort();
      //
      // Traitement du code des connects.
      //
      TraiteCV();
      //
      // On vide les buffers des connects.
      //
      VideBuffer();
      //
      // Traitement des tches de fond locales.
      //
      TraiteLocalTask();
      //
      // On vide les buffers des connects.
      //
      VideBuffer();
      //
      // Traitement des tches ONLIB.
      //
      TraiteONLIBTask();
      //
      // Traitement des tches globales.
      //
      TraiteGTaskCV();
      //
      // On vide les buffers des connects.
      //
      VideBuffer();
      //
      // Rcupration des ressources mmoire.
      //
      GarbageCollector();
      //
      // Calcul des temps d'excution.
      //
      CalcTMT();
      //
      // Traitement des vnements claviers
      //
      ReadKeyboard();
      //
      // Raffraichissement des affichages.
      //
      RefreshScreen();
      //
      // Rafraichissement des bases systmes.
      //
      RefreshBaseSys();
      //
      // Vrification des connects actifs.
      //
      VerifCNX();
      //
      // Raffraichissement des bases des appli.
      //
      RefreshBaseApp();
      //
      // Est-ce la fin de la journe?
      //
      if((Heures == 23) && (Minutes == 59) && (Secondes > 55))
         IlEstMinuit();
      //
      // Test de l'heure d'arret
      //
      if((bArretAuto) && ((Heures == HFinServ.hour) && (Minutes == HFinServ.minute)))
         mtFin = mtTRUE;
      //
      // On prvient les connects actifs de l'arret du serveur, 5 mn avant.
      // (le message est envoy une fois/mn jusqu'… l'arret)
      //
      if((! mtFin) && (bArretAuto) && ((Heures == HFinServ.hour) && (Minutes >= (HFinServ.minute-5)) && (Minutes != iMn)))
      {
         sprintf(OBuff,"\x07\x1B\x40\x1B\x56       ARRET DU SERVEUR … %02d:%02d",HFinServ.hour,HFinServ.minute);
         // Envoi … tous les connects
         for(i=0;i<TOTALCV;i++)
         {
            if(TabCv[i].Actif)
               Send0(OBuff,&TabCv[i]);
         }
         iMn = Minutes;
      }
   }
   if(bScrSave)
      ScreenOn();
   WinInfo("Libration des Canaux...");
   WriteLogNeo("Libration des Canaux...");
   LibPort();
   CloseLogPaq();
   WinInfo("Sauvegarde des Informations Systme du Serveur...");
   WriteLogNeo("Sauvegarde des Informations Systme du Serveur...");
   SauveInfoSys();
   WinInfo("Sauvegarde des Informations Systme des Applications...");
   WriteLogNeo("Sauvegarde des Informations Systme des Applications...");
   SauveAllAppSysInfo();
   WinInfo("Libration des Tches ONLIB...");
   WriteLogNeo("Libration des Tches ONLIB...");
   // Libration des tches ONLIB.
   for(wCurrCv=0;wCurrCv<TOTALCV;wCurrCv++)
   {
      if(OLCv[wCurrCv].Actif)
      {
         OLCv[wCurrCv].IsOnLib = mtTRUE;
         DeconnecteCv(&OLCv[wCurrCv]);
      }
   }
   WinInfo("Libration de la pile des paquets...");
   WriteLogNeo("Libration de la pile des paquets...");
   FreeSDATA();
   WinInfo("Sauvegarde des Paliers Systmes...");
   WriteLogNeo("Sauvegarde des Paliers Systmes...");
   SauveSysPaliers();
   WinInfo("Sauvegarde des Statistiques des Applications...");
   WriteLogNeo("Sauvegarde des Statistiques des Applications...");
   SauveAllAppStat();
   TimerEnd();
   WinInfo("Fermeture des Canaux...");
   WriteLogNeo("Fermeture des Canaux...");
   ClosePorts();
   WinInfo("Suppression des Applications...");
   WriteLogNeo("Suppression des Applications...");
   DelAllApp();
   WinInfo("Suppression du Dictionnaire Global...");
   WriteLogNeo("Suppression du Dictionnaire Global...");
   DelGlobalDic();
   EndException();
   WinInfo("Fermeture des Bases Systme...");
   WriteLogNeo("Fermeture des Bases Systme...");
   CloseBaseSys();
   EndBaseSys();
   MemFree(PathAPPS);
   MemFree(pAppAutoCNX);
   MemFree(NomAppAutoCNX);
   WriteLogNeo("*****> SORTIE DE NEOSERV LE %02d/%02d/%04d A %02d:%02d",Jour,Mois,Annee,Heures,Minutes);
   EndWin();
   SWVSetCursor(mtTRUE);
   disp_close();
   mem_term();
   return(EXIT_SUCCESS);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

