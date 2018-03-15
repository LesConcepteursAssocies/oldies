// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณINITIAL.C    ณ                                                             ณ
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
// ณDernire modification : 02/10/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des initialisations du serveur                                     ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <direct.h>
#include "toolkit.h"
#include "filespec.h"
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <fcntl.h>
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "ports.h"

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
typedef struct
{
   char   Nom[8];
   mtUSHORT Couleur;
} Tcoul;
Tcoul    tCC[8] =                      // Tableau des couleurs.
{
   { "NOIR"   ,SWBLACK   },
   { "BLEU"   ,SWBLUE    },
   { "VERT"   ,SWGREEN   },
   { "CYAN"   ,SWCYAN    },
   { "ROUGE"  ,SWRED     },
   { "MAGENTA",SWMAGENTA },
   { "MARRON" ,SWBROWN   },
   { "BLANC"  ,SWWHITE   }
};
mtWORD   NoLig;                        // No de ligne courante dans le fichier.
char     *pFicIni="NEOSERV.INI";
mtWORD   FicIni;
mtBOOL   bIniLocal=mtFALSE;            // Vrai si on est en initialisation d'un port local.
mtBOOL   bIniCorinte=mtFALSE;          // Vrai si on est en initialisation d'un port X25 CORINTE.
mtBOOL   bIniMitec=mtFALSE;            // Vrai si on est en initialisation d'un port X25 MITEC.
mtBOOL   bIniRTC=mtFALSE;              // Vrai si on est en initialisation d'un port RTC.
mtBOOL   bIniPalier=mtFALSE;           // Vrai si on est en init. des tarifs palier.
mtBOOL   bIniCoul=mtFALSE;             // Vrai si on est en init. des couleurs.
mtBOOL   bIniSys=mtFALSE;              // Vrai si on est en init. des infos systme.
mtBOOL   bIniLoad=mtFALSE;             // Vrai si on est en init. des chargt. fichiers.
mtUSHORT CoulSys=SWVBLDATR(SWWHITE,SWBLUE);    // Couleur des infos systme
mtUSHORT CoulInf=SWVBLDATR(SWCYAN,SWBLACK);    // Couleur des msg. d'information
mtUSHORT CoulCmd=SWVBLDATR(SWWHITE,SWBLUE);    // Couleur des commandes
mtUSHORT CoulErr=SWVBLDATR(SWWHITE,SWRED);     // Couleur des msg. d'erreur
mtUSHORT CoulCur=SWVBLDATR(SWWHITE,SWRED);     // Couleur du curseur
mtUSHORT CoulBdx=SWVBLDATR(SWBLACK,SWCYAN);    // Couleur des bandeaux d'affichages
extern   char      TLoadFic[MAXFICLOAD][80];   // Tableau des fichiers … charger.
extern   mtUWORD   iLoad;                      // Indice dans TLoadFic.
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   mtBYTE     Heures;
extern   mtBYTE     Minutes;
extern   mtBYTE     Secondes;
extern   mtBYTE     Jour;
extern   mtBYTE     Mois;
extern   mtWORD     Annee;
extern   char       *ExtPrg;
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   Ports     tPorts[MAXPORTS];
extern   mtUSHORT  usStdUARTBaseIOAddress[MAXPORTS];
extern   mtUSHORT  usStdIRQNumber[MAXPORTS];
extern   StPalier  StatPalier[MAXPALIER];
extern   struct    dos_time_t HFinServ;
extern   mtBOOL    bArretAuto;
extern   mtBOOL    bBipOnCnx;
extern   mtBOOL    bAutoCNX;
extern   char      *pAppAutoCNX;
extern   char      *NomAppAutoCNX;
extern   mtBOOL    bCreateDbSys;
extern   mtBOOL    bTracePaq;
extern   mtBOOL    bTraceNeo;
extern   char      *PathAPPS;
extern   mtWORD    wSaveScreen;
extern   mtWORD    wSaveBase;
extern   mtWORD    wVerCNX;

extern   mtBOOL    _FileOpen(mtPTRBYTE,mtWORD,mtPTRWORD,mtBOOL);
extern   void      _FileClose(mtWORD);
extern   mtBOOL    _IsEOF(mtWORD);
extern   void      _FileRead(mtWORD,mtPTRBYTE,size_t);
extern   void      _FileReadLine(mtWORD,mtPTRBYTE,size_t);
extern   void      _FileWrite(mtWORD,mtPTRBYTE,size_t);
extern   void      RazPort(void);
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   mtBOOL    X25ReadPaquet(int);
extern   void      X25SendDATA(int,mtUWORD,mtPTRBYTE);
extern   void      X25LibereCV(mtUWORD);
extern   void      X25CommLibereAll(int);
extern   mtBOOL    X25InitialisePort(int);
extern   mtBOOL    X25TestTransmission(int, mtUWORD, mtPTRWORD);
extern   void      X25CommEtabli(int, mtUWORD);
extern   mtBOOL    LOCReadPaquet(int);
extern   void      LOCSendDATA(int,mtUWORD,mtPTRBYTE);
extern   void      LOCLibereCV(mtUWORD);
extern   void      LOCCommLibereAll(int);
extern   mtBOOL    LOCInitialisePort(int);
extern   mtBOOL    LOCTestTransmission(int, mtUWORD, mtPTRWORD);
extern   void      LOCCommEtabli(int, mtUWORD);
extern   mtBOOL    RTCReadPaquet(int);
extern   void      RTCSendDATA(int,mtUWORD,mtPTRBYTE);
extern   void      RTCLibereCV(mtUWORD);
extern   void      RTCCommLibereAll(int);
extern   mtBOOL    RTCInitialisePort(int);
extern   mtBOOL    RTCTestTransmission(int, mtUWORD, mtPTRWORD);
extern   void      RTCCommEtabli(int, mtUWORD);
extern   mtBOOL    CCIReadPaquet(int);
extern   void      CCISendDATA(int,mtUWORD,mtPTRBYTE);
extern   void      CCILibereCV(mtUWORD);
extern   void      CCICommLibereAll(int);
extern   mtBOOL    CCIInitialisePort(int);
extern   mtBOOL    CCITestTransmission(int, mtUWORD, mtPTRWORD);
extern   void      CCICommEtabli(int, mtUWORD);
extern   mtPTRAPP  SearchApp(mtPTRBYTE,mtBYTE);
extern   mtBOOL    LoadApp(mtPTRBYTE);
extern   void      CloseLogPaq(void);
extern   void      CloseLogNeo(void);
extern   void      WriteLogNeo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise un port de communication local.
// Renvoie vrai si tout c'est bien pass.
//
mtBOOL InitPortLocal(mtPTRBYTE cInit)
{
   mtPTRBYTE pTmp,pCmd;
   mtUDWORD ulTmp;
   int iPort, iTmp;

   pTmp = strupr(cInit);
   pCmd = strstr(pTmp,"COM =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction COM = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iPort);
   iPort--;
   if(iPort<0 || iPort>=MAXPORTS)
   {
      WinMsgErr(": N๘ de port non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(tPorts[iPort].Actif)
   {
      WinMsgErr(": Port dj… dfini","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCmd = strstr(pTmp,"SPEED =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction SPEED = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+7,"%lu",&ulTmp);
   if(ulTmp<50L || ulTmp>38400L)
   {
      WinMsgErr(": Vitesse non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].SpeedCom = ulTmp;
   pCmd = strstr(pTmp,"PARITY =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction PARITY = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(strstr(pCmd+8,"NONE"))
      tPorts[iPort].Parite = SWPARITYNONE;
   else
   {
      if(strstr(pCmd+8,"ODD"))
         tPorts[iPort].Parite = SWPARITYODD;
      else
      {
         if(strstr(pCmd+8,"EVEN"))
            tPorts[iPort].Parite = SWPARITYEVEN;
         else
         {
            WinMsgErr(": Parit non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
            return(mtFALSE);
         }
      }
   }
   pCmd = strstr(pTmp,"LEN =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction LEN = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<5 || iTmp>8)
   {
      WinMsgErr(": Longueur non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].WordLen = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"STOP =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction STOP = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+6,"%d",&iTmp);
   if(iTmp<1 || iTmp>2)
   {
      WinMsgErr(": Bit de stop non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].StopBit  = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"IOBASE =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IOBASE = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+8,"%x",&iTmp);
   usStdUARTBaseIOAddress[iPort] = iTmp;
   pCmd = strstr(pTmp,"IRQ =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IRQ = manquante","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<2 || iTmp>7)
   {
      WinMsgErr(": IRQ non valide","InitPortLocal: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   usStdIRQNumber[iPort] = iTmp;
   tPorts[iPort].Type           = TPORTLOCAL;
   tPorts[iPort].InitialisePort = LOCInitialisePort;
   tPorts[iPort].ReadPaquet     = LOCReadPaquet;
   tPorts[iPort].TestTX         = LOCTestTransmission;
   tPorts[iPort].SendDATA       = LOCSendDATA;
   tPorts[iPort].LibereCV       = LOCLibereCV;
   tPorts[iPort].CommLibereAll  = LOCCommLibereAll;
   tPorts[iPort].CommEtabli     = LOCCommEtabli;
   tPorts[iPort].Actif = mtTRUE;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise un port de communication X25 CORINTE.
// Renvoie vrai si tout c'est bien pass.
//
mtBOOL InitPortCorinte(mtPTRBYTE cInit)
{
   mtPTRBYTE pTmp,pCmd;
   mtUDWORD ulTmp;
   int iPort, iTmp;

   pTmp = strupr(cInit);
   pCmd = strstr(pTmp,"COM =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction COM = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iPort);
   iPort--;
   if(iPort<0 || iPort>=MAXPORTS)
   {
      WinMsgErr(": N๘ de port non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(tPorts[iPort].Actif)
   {
      WinMsgErr(": Port dj… dfini","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCmd = strstr(pTmp,"SPEED =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction SPEED = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+7,"%lu",&ulTmp);
   if(ulTmp<50L || ulTmp>38400L)
   {
      WinMsgErr(": Vitesse non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].SpeedCom = ulTmp;
   pCmd = strstr(pTmp,"PARITY =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction PARITY = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(strstr(pCmd+8,"NONE"))
      tPorts[iPort].Parite = SWPARITYNONE;
   else
   {
      if(strstr(pCmd+8,"ODD"))
         tPorts[iPort].Parite = SWPARITYODD;
      else
      {
         if(strstr(pCmd+8,"EVEN"))
            tPorts[iPort].Parite = SWPARITYEVEN;
         else
         {
            WinMsgErr(": Parit non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
            return(mtFALSE);
         }
      }
   }
   pCmd = strstr(pTmp,"LEN =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction LEN = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<5 || iTmp>8)
   {
      WinMsgErr(": Longueur non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].WordLen = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"STOP =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction STOP = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+6,"%d",&iTmp);
   if(iTmp<1 || iTmp>2)
   {
      WinMsgErr(": Bit de stop non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].StopBit  = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"IOBASE =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IOBASE = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+8,"%x",&iTmp);
   usStdUARTBaseIOAddress[iPort] = iTmp;
   pCmd = strstr(pTmp,"IRQ =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IRQ = manquante","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<2 || iTmp>7)
   {
      WinMsgErr(": IRQ non valide","InitPortX25ASM: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   usStdIRQNumber[iPort] = iTmp;
   tPorts[iPort].Type           = TPORTX25ASM;
   tPorts[iPort].InitialisePort = X25InitialisePort;
   tPorts[iPort].ReadPaquet     = X25ReadPaquet;
   tPorts[iPort].TestTX         = X25TestTransmission;
   tPorts[iPort].SendDATA       = X25SendDATA;
   tPorts[iPort].LibereCV       = X25LibereCV;
   tPorts[iPort].CommLibereAll  = X25CommLibereAll;
   tPorts[iPort].CommEtabli     = X25CommEtabli;
   tPorts[iPort].Actif = mtTRUE;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise un port de communication RTC.
// Renvoie vrai si tout c'est bien pass.
//
mtBOOL InitPortRTC(mtPTRBYTE cInit)
{
   mtPTRBYTE pTmp,pCmd,pCH=NULL,pFI=NULL;
   mtUDWORD ulTmp;
   int iPort, iTmp, iLen;

   pTmp = strupr(cInit);
   pCmd = strstr(pTmp,"COM =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction COM = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iPort);
   iPort--;
   if(iPort<0 || iPort>=MAXPORTS)
   {
      WinMsgErr(": N๘ de port non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(tPorts[iPort].Actif)
   {
      WinMsgErr(": Port dj… dfini","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCmd = strstr(pTmp,"SPEED =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction SPEED = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+7,"%lu",&ulTmp);
   if(ulTmp<50L || ulTmp>38400L)
   {
      WinMsgErr(": Vitesse non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].SpeedCom = ulTmp;
   pCmd = strstr(pTmp,"PARITY =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction PARITY = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(strstr(pCmd+8,"NONE"))
      tPorts[iPort].Parite = SWPARITYNONE;
   else
   {
      if(strstr(pCmd+8,"ODD"))
         tPorts[iPort].Parite = SWPARITYODD;
      else
      {
         if(strstr(pCmd+8,"EVEN"))
            tPorts[iPort].Parite = SWPARITYEVEN;
         else
         {
            WinMsgErr(": Parit non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
            return(mtFALSE);
         }
      }
   }
   pCmd = strstr(pTmp,"LEN =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction LEN = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<5 || iTmp>8)
   {
      WinMsgErr(": Longueur non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].WordLen = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"STOP =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction STOP = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+6,"%d",&iTmp);
   if(iTmp<1 || iTmp>2)
   {
      WinMsgErr(": Bit de stop non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].StopBit  = (mtUSHORT) iTmp;
   pCmd = strstr(pTmp,"IOBASE =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IOBASE = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+8,"%x",&iTmp);
   usStdUARTBaseIOAddress[iPort] = iTmp;
   pCmd = strstr(pTmp,"IRQ =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction IRQ = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iTmp);
   if(iTmp<2 || iTmp>7)
   {
      WinMsgErr(": IRQ non valide","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCmd = strstr(pTmp,"NAME =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction NAME = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCH = pCmd+6;
   LTrim(pCH);
   pFI = strchr(pCH,' ');
   if(! pFI)
   {
      WinMsgErr(": Instruction APP = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   iLen = (pFI-pCH)+1;
   if(iLen > 25)
      iLen = 25;
   strncpy(tPorts[iPort].Nom,pCH,iLen);
   pCmd = strstr(pTmp,"APP =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction APP = manquante","InitPortRTC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   pCH = pCmd+5;
   LTrim(pCH);
   //
   // On enlve le CRLF de fin.
   //
   pTmp = strrchr(pCH,10);
   if(pTmp)
      *(pTmp) = '\0';
   pTmp = strrchr(pCH,13);
   if(pTmp)
      *(pTmp) = '\0';
   strncpy(tPorts[iPort].NApp,pCH,10);

   usStdIRQNumber[iPort] = iTmp;
   tPorts[iPort].Type           = TPORTRTC;
   tPorts[iPort].InitialisePort = RTCInitialisePort;
   tPorts[iPort].ReadPaquet     = RTCReadPaquet;
   tPorts[iPort].TestTX         = RTCTestTransmission;
   tPorts[iPort].SendDATA       = RTCSendDATA;
   tPorts[iPort].LibereCV       = RTCLibereCV;
   tPorts[iPort].CommLibereAll  = RTCCommLibereAll;
   tPorts[iPort].CommEtabli     = RTCCommEtabli;
   tPorts[iPort].Actif = mtTRUE;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise un port de communication MITEC.
// Renvoie vrai si tout c'est bien pass.
//
mtBOOL InitPortMitec(mtPTRBYTE cInit)
{
   mtPTRBYTE pTmp,pCmd;
   mtUDWORD ulTmp;
   int iPort, iTmp;

   pTmp = strupr(cInit);
   pCmd = strstr(pTmp,"COM =");
   if(! pCmd)
   {
      WinMsgErr(": Instruction COM = manquante","InitPortX25MITEC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   sscanf(pCmd+5,"%d",&iPort);
   iPort--;
   if(iPort<0 || iPort>=MAXPORTS)
   {
      WinMsgErr(": N๘ de port non valide","InitPortX25MITEC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   if(tPorts[iPort].Actif)
   {
      WinMsgErr(": Port dj… dfini","InitPortX25MITEC: Ligne %d '%s'",NoLig,pTmp);
      return(mtFALSE);
   }
   tPorts[iPort].Type           = TPORTX25MITEC;
   tPorts[iPort].MaxCV          = 16;
   tPorts[iPort].InitialisePort = CCIInitialisePort;
   tPorts[iPort].ReadPaquet     = CCIReadPaquet;
   tPorts[iPort].TestTX         = CCITestTransmission;
   tPorts[iPort].SendDATA       = CCISendDATA;
   tPorts[iPort].LibereCV       = CCILibereCV;
   tPorts[iPort].CommLibereAll  = CCICommLibereAll;
   tPorts[iPort].CommEtabli     = CCICommEtabli;
   tPorts[iPort].Actif = mtTRUE;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise les tarifs palier.
void IniPalier(mtPTRBYTE cInit)
{
   mtPTRBYTE pEgal;
   int i;
   float fTmp=0.00;

   pEgal = strstr(cInit,"=");
   if(!pEgal)
      pEgal = strstr(cInit," ");
   for(i=0;i<MAXPALIER;i++)
   {
      if(strstr(cInit,StatPalier[i].Nom))
      {
         fTmp = atof(pEgal+1);         // On passe aussi le ' ='.
         StatPalier[i].Tarif = fTmp;
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise les couleurs.
void IniCouleur(mtPTRBYTE cInit)
{
   mtPTRBYTE pCmd=NULL,pCC=NULL,pCF=NULL;
   int i;
   mtBOOL bcC=mtFALSE,bcF=mtFALSE;
   mtUSHORT cC,cF;

   // On rcupre le nom de la couleur du fond
   pCmd = strstr(cInit,"/");
   if(pCmd)
   {
      *(pCmd) = '\0';
      pCF     = pCmd+1;
   }
   // On rcupre le nom de la couleur de caractre
   pCmd = strstr(cInit,"=");
   if(pCmd)
   {
      *(pCmd) = '\0';
      pCC     = pCmd+1;
   }
   for(i=0;i<8;i++)
   {
      if((pCmd = strstr(pCC,tCC[i].Nom)) != NULL)
      {
         cC  = tCC[i].Couleur;
         bcC = mtTRUE;
         if(bcF)
            break;
      }
      if((pCmd = strstr(pCF,tCC[i].Nom)) != NULL)
      {
         cF  = tCC[i].Couleur;
         bcF = mtTRUE;
         if(bcC)
            break;
      }
   }
   if(strstr(cInit,"SYS"))
      CoulSys = SWVBLDATR((bcC ? cC : SWWHITE),(bcF ? cF : SWBLUE));
   if(strstr(cInit,"INF"))
      CoulInf = SWVBLDATR((bcC ? cC : SWCYAN),(bcF ? cF : SWBLACK));
   if(strstr(cInit,"CMD"))
      CoulCmd = SWVBLDATR((bcC ? cC : SWWHITE),(bcF ? cF : SWBLUE));
   if(strstr(cInit,"ERR"))
      CoulErr = SWVBLDATR((bcC ? cC : SWWHITE),(bcF ? cF : SWRED));
   if(strstr(cInit,"CUR"))
      CoulCur = SWVBLDATR((bcC ? cC : SWWHITE),(bcF ? cF : SWRED));
   if(strstr(cInit,"BDX"))
      CoulBdx = SWVBLDATR((bcC ? cC : SWBLACK),(bcF ? cF : SWCYAN));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture des infos de paramtrage du systme.
//
void IniSystem(mtPTRBYTE cInit)
{
   mtPTRBYTE pCmd=NULL,pCH=NULL,pCM=NULL,pTmp;
   int iTmp;

   pCmd = strstr(cInit,"ARRETSERVEUR =");
   if(pCmd)
   {
      pCmd = strstr(cInit,":");        // "HH:MM"
      if(pCmd)
      {
         *(pCmd) = '\0';
         pCH     = pCmd-2;
         pCM     = pCmd+1;
         sscanf(pCH,"%d",&iTmp);
         if(iTmp>=0 && iTmp<=23)
            HFinServ.hour = iTmp;
         sscanf(pCM,"%d",&iTmp);
         if(iTmp>=0 && iTmp<=59)
            HFinServ.minute = iTmp;
      }
   }
   pCmd = strstr(cInit,"ARRETAUTO =");
   if(pCmd)
   {
      if(strstr(pCmd+11,"OUI"))
         bArretAuto = mtTRUE;
      if(strstr(pCmd+11,"NON"))
         bArretAuto = mtFALSE;
   }
   pCmd = strstr(cInit,"PATHAPPS =");
   if(pCmd)
   {
      pCH = pCmd+10;
      LTrim(pCH);
      if(PathAPPS)
         PathAPPS = (char *) MemReAlloc(PathAPPS,strlen(pCH)+1);
      else
         PathAPPS = (char *) MemAlloc(strlen(pCH)+1);
      strcpy(PathAPPS,pCH);
      WinInfo("PATHAPPS = %s",PathAPPS);
   }
   pCmd = strstr(cInit,"BEEPONCNX =");
   if(pCmd)
   {
      if(strstr(pCmd+11,"OUI"))
         bBipOnCnx = mtTRUE;
      if(strstr(pCmd+11,"NON"))
         bBipOnCnx = mtFALSE;
   }
   pCmd = strstr(cInit,"CREATEDBFSYS =");
   if(pCmd)
   {
      if(strstr(pCmd+11,"OUI"))
         bCreateDbSys = mtTRUE;
      if(strstr(pCmd+11,"NON"))
         bCreateDbSys = mtFALSE;
   }
   pCmd = strstr(cInit,"SAUVEECRAN =");
   if(pCmd)
   {
      sscanf(pCmd+12,"%d",&iTmp);
      if(iTmp>=0 && iTmp<=60)
         wSaveScreen = (iTmp*60);
   }
   pCmd = strstr(cInit,"SAUVEBASES =");
   if(pCmd)
   {
      sscanf(pCmd+12,"%d",&iTmp);
      if(iTmp>=0 && iTmp<=60)
         wSaveBase = (iTmp*60);
   }
   pCmd = strstr(cInit,"AUTOCNX =");
   if(pCmd)
   {
      if(strstr(pCmd+9,"OUI"))
         bAutoCNX = mtTRUE;
      if(strstr(pCmd+9,"NON"))
         bAutoCNX = mtFALSE;
   }
   pCmd = strstr(cInit,"APPAUTOCNX =");
   if(pCmd)
   {
      pCH = pCmd+12;
      LTrim(pCH);
      //
      // On enlve le CRLF de fin.
      //
      pTmp = strrchr(pCH,10);
      if(pTmp)
         *(pTmp) = '\0';
      pTmp = strrchr(pCH,13);
      if(pTmp)
         *(pTmp) = '\0';
      pAppAutoCNX = (char *) MemReAlloc(pAppAutoCNX,strlen(pCH)+1);
      strcpy(pAppAutoCNX,pCH);
   }
   pCmd = strstr(cInit,"TRACEPAQUET =");
   if(pCmd)
   {
      if(strstr(pCmd+13,"OUI"))
         bTracePaq = mtTRUE;
      if(strstr(pCmd+13,"NON"))
      {
         CloseLogPaq();
         bTracePaq = mtFALSE;
      }
   }
   pCmd = strstr(cInit,"TRACESERVEUR =");
   if(pCmd)
   {
      if(strstr(pCmd+14,"OUI"))
         bTraceNeo = mtTRUE;
      if(strstr(pCmd+14,"NON"))
      {
         WriteLogNeo("*****> DESACTIVATION DU MODE TRACE LE %02d/%02d/%04d A %02d:%02d",Jour,Mois,Annee,Heures,Minutes);
         CloseLogNeo();
         bTraceNeo = mtFALSE;
      }
   }
   pCmd = strstr(cInit,"VERIFCNX =");
   if(pCmd)
   {
      sscanf(pCmd+10,"%d",&iTmp);
      if(iTmp>=0 && iTmp<=60)
         wVerCNX = (iTmp*60);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si le fichier donn existe dans le tableau TLoadFic
//
mtBOOL IsInTLoadFic(mtPTRBYTE pFic)
{
   mtUWORD i=0;

   while(i<iLoad)
   {
      if(strcmp(pFic,TLoadFic[i]) == 0)
         return(mtTRUE);
      i++;
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture des fichiers … charger.
//
void IniLoadFic(mtPTRBYTE cInit)
{
   mtPTRBYTE pTmp;

   if(iLoad < MAXFICLOAD)
   {
      //
      // On enlve le CRLF de fin.
      //
      pTmp = strrchr(cInit,10);
      if(pTmp)
         *(pTmp) = '\0';
      pTmp = strrchr(cInit,13);
      if(pTmp)
         *(pTmp) = '\0';
      if(! IsInTLoadFic(cInit))
      {
         strcpy(TLoadFic[iLoad],cInit);
         iLoad++;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture du fichier d'initialisation.
// Renvoie Vrai si tout c'est bien pass.
//
mtBOOL ReadIni(void)
{
   mtBOOL bRet=mtFALSE;
   char BufTmp[129], *pTmp;

   // Valeurs par dfaut.
   wSaveBase   = 5*60;
   wSaveScreen = 5*60;
   wVerCNX     = 5*60;
   // Couleurs par dfaut.
   CoulSys  = SWVBLDATR(SWWHITE,SWCYAN);
   CoulInf  = SWVBLDATR(SWWHITE,SWBLACK);
   CoulCmd  = SWVBLDATR(SWWHITE,SWCYAN);
   CoulErr  = SWVBLDATR(SWWHITE,SWRED);
   CoulCur  = SWVBLDATR(SWWHITE,SWBLUE);
   CoulBdx  = SWVBLDATR(SWBLACK,SWCYAN);
   iLoad    = 0;
   if(_FileOpen(pFicIni,O_BINARY,&FicIni,mtFALSE))
   {
      RazPort();
      NoLig = 0;
      while(! _IsEOF(FicIni))
      {
         NoLig++;
         pTmp = BufTmp;
         memset(pTmp,0,129);
         _FileReadLine(FicIni,pTmp,128);
         if(strlen(pTmp) <=1)
            continue;
         LTrim(pTmp);
         if(strlen(pTmp) <=1)
            continue;
         if(strncmp(pTmp,"//",2) == 0)
            continue;
         if(strstr(pTmp,"[Local]"))
         {
            bIniLocal   = mtTRUE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[X25-ASM]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtTRUE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[RTC]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtTRUE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[X25-MITEC]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtTRUE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[TarifsPalier]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtTRUE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[Couleurs]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtTRUE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[System]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtTRUE;
            bIniLoad    = mtFALSE;
            continue;
         }
         if(strstr(pTmp,"[Load]"))
         {
            bIniLocal   = mtFALSE;
            bIniCorinte = mtFALSE;
            bIniMitec   = mtFALSE;
            bIniRTC     = mtFALSE;
            bIniPalier  = mtFALSE;
            bIniCoul    = mtFALSE;
            bIniSys     = mtFALSE;
            bIniLoad    = mtTRUE;
            continue;
         }
         if(bIniLocal)
            bRet = InitPortLocal(pTmp);
         if(bIniCorinte)
            bRet = InitPortCorinte(pTmp);
         if(bIniMitec)
            bRet = InitPortMitec(pTmp);
         if(bIniRTC)
            bRet = InitPortRTC(pTmp);
         if(bIniPalier)
            IniPalier(strupr(pTmp));
         if(bIniCoul)
            IniCouleur(strupr(pTmp));
         if(bIniSys)
            IniSystem(strupr(pTmp));
         if(bIniLoad)
            IniLoadFic(strupr(pTmp));
      }
      _FileClose(FicIni);
   }
   return(bRet);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

