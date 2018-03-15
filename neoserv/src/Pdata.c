// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณPDATA.C      ณ                                                             ณ
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
// ณDernire modification : 01/03/1996                                         ณ
// ณ                                                                           ณ
// ณManipulation de la pile des paquets                                        ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <direct.h>
#include <malloc.h>
#include <mem.h>
#include <ctype.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "minitel.h"
#include "ports.h"
#include "pdata.h"

#define  X25DATA        0x30           // Trame de donnes.
#define  X25CNX         0x31           // Trame d'appel.
#define  X25LIB         0x32           // Trame de libration.
#define  X25CONF        0x33           // Trame de confirmation d'appel.
#define  X25ERRP        0x34           // Erreur de procdure.
#define  X25NOTERM      0x35           // Indisponibilit du terminal.
#define  X25TERM        0x36           // Disponibilit du terminal.
#define  X25NOPAD       0x37           // Suppression d'cho du PAD.
#define  X25PAD         0x38           // Positionnement d'cho du PAD.
#define  X25NORES       0x39           // Indisponibilit des ressources.
#define  X25RES         0x3A           // Disponibilit des ressources.

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   pPDATA     pSDATA;
extern   mtUWORD    iSDATA;
extern   mtUWORD    nPgeSD;
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   char       cBann[81];
extern   struct     dos_time_t HPreCnx;
extern   struct     dos_time_t HDerCnx;
extern   mtUDWORD   NbAppTot;
extern   mtPTRCV    pCurrCV;            // Pointe sur le cv courant.
extern   UnCv       TabCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   Ports      tPorts[MAXPORTS];
extern   mtBOOL     bBipOnCnx;
extern   mtBOOL     bAutoCNX;
extern   char       *NomAppAutoCNX;

extern   void       SWVRingBell(void);
extern   void       AppendCvData(mtPTRCV,mtPTRBYTE,mtWORD);
extern   void       FreeCvData(mtPTRCV);
extern   void       InitCnxCv(mtPTRCV);
extern   void       DeconnecteCv(mtPTRCV);
extern   void       InitOnLibCv(mtPTRCV);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       Send0(mtPTRBYTE,mtPTRCV);
extern   mtPTRAPP   SearchApp(mtPTRBYTE,mtBYTE);
extern   void       SeqEsc(mtPTRBYTE,mtPTRCV);
extern   void       _EnqRom(void);
extern   float      TimeToCent(struct dos_time_t *);
extern   void       WriteLogPaq(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void          PushPData(int,mtUWORD,mtPTRBYTE,mtWORD,mtBYTE);
void          FreePData(mtPTRPDATA);
mtPTRPDATA    PopPData(mtUWORD);
mtBOOL        SearchPData(mtUWORD,mtPTRWORD);
void          FreeSDATA(void);
void          GetPaquet(mtUWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre le paquet donn.
//
void FreePData(mtPTRPDATA pDT)
{
   MemFree(pDT->pData);
   MemFree(pDT);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le paquet donn.
//
void PushPData(int nPort,mtUWORD nCv,mtPTRBYTE pDt,mtWORD wLen,mtBYTE Type)
{
   mtPTRPDATA NewP;

   // WinInfo("PUSHPDATA=[%s]",pDt);
   NewP        = (mtPTRPDATA) MemAlloc(sizeof(PData));
   NewP->pData = (mtPTRBYTE) MemAlloc(wLen+1);
   if(pDt)
      memcpy(NewP->pData,pDt,wLen);
   NewP->wLen  = wLen;
   NewP->iPort = nPort;
   NewP->NumCv = nCv;
   NewP->Type  = Type;
   pSDATA[iSDATA++] = NewP;
   if((iSDATA % SDATAPAGE) == 0)
   {
      nPgeSD++;
      pSDATA = (pPDATA) MemReAlloc(pSDATA,(nPgeSD*SDATAPAGE)*sizeof(mtPTRPDATA));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile le paquet demand (iSD = 0..iSDATA-1).
//
mtPTRPDATA PopPData(mtUWORD iSD)
{
   mtPTRPDATA ptr = NULL;
   mtUWORD ui;

   if(iSD<iSDATA)
   {
      ptr = pSDATA[iSD];
      // WinInfo("POPPDATA=[%s]",ptr->pData);
      // On dcale les paquets aprs celui demand.
      ui = iSD+1;
      while(ui<iSDATA)
      {
         pSDATA[ui-1] = pSDATA[ui];
         ui++;
      }
      if((nPgeSD>1) && ((iSDATA % STACKPAGE) == 0))
      {
         nPgeSD--;
         pSDATA = (pPDATA) MemReAlloc(pSDATA,(nPgeSD*SDATAPAGE)*sizeof(mtPTRPDATA));
      }
      iSDATA--;
   }
   return(ptr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cherche un paquet … destination du cv donn.
// Renvoie TRUE et la position du paquet dans la pile si trouv.
//
mtBOOL SearchPData(mtUWORD nCv,mtPTRWORD pPos)
{
   mtBOOL bOk = mtFALSE;
   mtUWORD ui = 0;
   mtPTRPDATA pDT;

   *pPos = -1;
   if(iSDATA)
   {
      while(ui<iSDATA)
      {
         pDT = pSDATA[ui];
         if(pDT->NumCv == nCv)
         {
            *pPos = ui;
            bOk   = mtTRUE;
            break;
         }
         ui++;
      }
   }
   return(bOk);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre la pile des paquets.
//
void FreeSDATA(void)
{
   mtUWORD ui = 0;
   mtPTRPDATA pDT;

   if(iSDATA)
   {
      while(ui<iSDATA)
      {
         pDT = pSDATA[ui];
         FreePData(pDT);
         ui++;
      }
   }
   MemFree(pSDATA);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile un paquet pour le cv donn.
//
void GetPaquet(mtUWORD nCv)
{
   struct dos_time_t HPaq;
   char OBuff[50],*pSeq,tTmp[3], *pslash, *ps2;
   mtBOOL  bSEP=mtFALSE,bCR=mtFALSE,bCSI=mtFALSE;
   mtPTRCV p;
   mtWORD wPos,LenPaq=0;
   mtPTRPDATA pDT;

   if(SearchPData(nCv,&wPos))
   {
      pCurrCV  = &TabCv[nCv];
      p        = pCurrCV;
      p->NumCv = nCv;
      pDT      = PopPData(wPos);
      if(p->Actif)
      {
         _dos_gettime(&HPaq);
         HPaq.hsecond = 0;
         p->HRPaq     = TimeToCent(&HPaq)/100;
      }
      switch(pDT->Type)
      {
         case X25DATA:
         {
            if(p->ArretVoie)
            {
               // La voie du cv est suspendue, on attend la reprise...
               WinInfo("**Paquet non trait Type=%c,Cv=%d, %s**",pDT->Type,p->NumCv,pDT->pData);
               break;
            }
            if((p->Actif) && (p->bBinMode))
            {
               // Mode BINAIRE (tlchargement, ...)
               // aucun traitement sur le paquet.
               AppendCvData(p,pDT->pData,pDT->wLen);       // On ajoute les donnes reues.
               if(p->bWaitChar)
               {
                  // Attente d'un caractre sur le flux.
                  if(p->pData && strchr(p->pData,p->WaitChar))
                     // Caractre prsent.
                     p->bPaqLu = mtTRUE;
               }
               else
                  p->bPaqLu = mtTRUE;
               break;
            }
            SeqEsc(pDT->pData,p);
            LenPaq = strlen(pDT->pData);
            WinInfo("DATA: Voie=%d, %s",p->NumCv,pDT->pData);
            if(p->Actif && p->bWaitChar)
            {
               AppendCvData(p,pDT->pData,LenPaq);          // On ajoute les donnes reues.
               // Attente d'un caractre sur le flux.
               if(p->pData && strchr(p->pData,p->WaitChar))
                  // Caractre prsent.
                  p->bPaqLu = mtTRUE;
               break;
            }
            if(p->Actif)
            {
               p->bEtoile = mtFALSE;
               p->bDiese  = mtFALSE;
               if(p->Attente == 0)
                  FreeCvData(p);
               if(! LenPaq)
                  break;
               if(LenPaq >= 2)
               {
                  // Filtrage Squence CSI...
                  if(strstr(pDT->pData,"\x1B\x5B\x3F\x7A"))
                  {
                     // Ack passage mode VDT->TELEINFO
                     if(p->Actif)
                        p->cMode = TMODE_TELINFO;
                     break;
                  }
                  // Recherche d'un ventuel SEP+Code … la fin du paquet.
                  //
                  if(*(pDT->pData+(LenPaq-2)) == MSep)
                  {
                     *(pDT->pData+(LenPaq-2)) = 0;
                     p->TFonction             = *(pDT->pData+(LenPaq-1)); // Rcup. du code de touche.
                     //
                     // On filtre la rponse au reset du minitel (PRO1 7F renvoi SEP 5E)
                     // (ou ack passage mode TELINFO->VDT)
                     //
                     if(p->TFonction == 0x5E)
                     {
                        if(p->Actif)
                           // On repasse en mode VDT.
                           p->cMode = TMODE_VDT;
                        break;
                     }
                     // Ack passage mode VDT->MIXTE.
                     if(p->TFonction == 0x70)
                     {
                        if(p->Actif)
                           // On repasse en mode VDT.
                           p->cMode = TMODE_MIXTE;
                        break;
                     }
                     // Ack passage mode MIXTE->VDT.
                     if(p->TFonction == 0x71)
                     {
                        if(p->Actif)
                           // On repasse en mode VDT.
                           p->cMode = TMODE_VDT;
                        break;
                     }
                     //
                     // SEP+Code.
                     //
                     *(pDT->pData+(LenPaq-1)) = 0;
                     AppendCvData(p,pDT->pData,LenPaq);    // On ajoute les donnes reues.
                     p->Attente   = 0;
                     p->bPaqLu    = mtTRUE;
                     if(LenPaq > 2)
                     {
                        // Recherche de l'toile ou du dise
                        // (squence du type '#SEP+Code' ou '*SEP+Code'
                        //
                        if(*(pDT->pData+(LenPaq-3)) == MEtoile)
                           p->bEtoile = mtTRUE;
                        else
                        {
                           if(*(pDT->pData+(LenPaq-3)) == MDiese)
                              p->bDiese = mtTRUE;
                        }
                     }
                     tPorts[pDT->iPort].NbPaqLu++;
                     break;
                  }
                  // Recherche d'un ventuel Code+CR … la fin du paquet
                  //
                  if(*(pDT->pData+(LenPaq-1)) == mtCR)
                  {
                     *(pDT->pData+(LenPaq-1)) = 0;
                     p->TFonction       = *(pDT->pData+(LenPaq-2)); // Rcup. du code de touche.
                     *(pDT->pData+(LenPaq-2)) = 0;
                     AppendCvData(p,pDT->pData,LenPaq);    // On ajoute les donnes reues.
                     p->Attente   = 0;
                     p->bPaqLu    = mtTRUE;
                     if(LenPaq > 2)
                     {
                        // Recherche de l'toile ou du dise
                        // (squence du type '#Code+CR' ou '*Code+CR'
                        //
                        if(*(pDT->pData+(LenPaq-3)) == MEtoile)
                           p->bEtoile = mtTRUE;
                        else
                        {
                           if(*(pDT->pData+(LenPaq-3)) == MDiese)
                              p->bDiese = mtTRUE;
                        }
                     }
                     tPorts[pDT->iPort].NbPaqLu++;
                     break;
                  }
               }
               // Recherche d'un ventuel SEP … la fin du paquet.
               //
               // WinInfo("CHERCHE SEP");
               if(*(pDT->pData+(LenPaq-1)) == MSep)
               {
                  *(pDT->pData+(LenPaq-1)) = 0;
                  p->Attente = 2;                // On n'a reu que le SEP.
                  AppendCvData(p,pDT->pData,LenPaq);       // On ajoute les donnes reues.
                  tPorts[pDT->iPort].NbPaqLu++;
                  break;
               }
               if((LenPaq == 1) && (p->Attente == 2))
               {
                  // On attendais le code de la touche
                  //
                  p->TFonction = *(pDT->pData+(LenPaq-1)); // Rcup. du code de touche.
                  p->Attente   = 0;
                  p->bPaqLu    = mtTRUE;
                  tPorts[pDT->iPort].NbPaqLu++;
                  break;
               }
               // Le paquet ne contient aucun code de touche.
               //
               AppendCvData(p,pDT->pData,LenPaq);          // On ajoute les donnes reues.
               p->Attente = 1;
               tPorts[pDT->iPort].NbPaqLu++;
               break;
            }
            break;
         }
         case X25CNX:
         {
            // Paquet d'appel.
            // Format :
            // xDDPPPPPPpp/NC/0/../SERVICE
            // x       : '6'=X25 CORINTE, 'L'=Local, 'R'=RTC, 'M'=X25 MITEC
            // DD      : Dpartement (2 c.)
            // PPPPPP  : Pavi        (6 c.)
            // pp      : Palier      (2 c.)
            // NC      : Numro Complmentaire (2 c.)
            // SERVICE : Nom du service        (10 c. maxi)
            // On filtre les granules devant la donne d'appel.
            pslash = strrchr(pDT->pData,'/');
            if(pslash)
            {
               pslash++;
               ps2 = pslash;
               while((*pslash) && (! isalnum(*pslash)))
                  pslash++;
               memmove(ps2,pslash,strlen(pslash)+1);
            }
            WinInfo("CNX: Voie=%d, %s",p->NumCv,pDT->pData);
            if(bBipOnCnx)
               SWVRingBell();
            if(p->Actif)
            {
               WinInfo("CV DEJA ACTIF");
               if(p->pApp->bOnLib)
                  // Procdure ONLIB dfinie.
                  InitOnLibCv(p);
               else
                  // Libration du connect et dsallocation de son Cv.
                  DeconnecteCv(p);
            }
            p->Actif   = mtTRUE;
            _dos_gettime(&HPaq);
            HPaq.hsecond = 0;
            p->HRPaq     = TimeToCent(&HPaq)/100;
            // Rcupration des info d'appel.
            //
            // WinInfo("RECUP INFOS");
            strncpy(p->TrameCNX,pDT->pData,30);
            p->NumCv   = pDT->NumCv;
            p->NumPort = pDT->iPort;
            strncpy(tTmp,pDT->pData+1,2);
            tTmp[2]    = '\0';
            p->Dept    = (mtBYTE) atoi(tTmp);
            strncpy(p->Pavi,pDT->pData+3,6);
            p->Pavi[6] = '\0';
            strncpy(tTmp,pDT->pData+9,2);
            tTmp[2]    = '\0';
            p->Palier  = (mtBYTE) atoi(tTmp);
            strncpy(tTmp,pDT->pData+12,2);
            tTmp[2]    = '\0';
            p->NumComp = (mtBYTE) atoi(tTmp);
            pslash     = strrchr(pDT->pData,'/');         // On rcupre la donne d'appel.
            //
            // On prend le nom du service.
            // A voir : "Service*xxxx"
            //
            if(pslash)
            {
               pslash++;
               strncpy(p->Service,pslash,10);
            }
            p->Service[10] = '\0';
            strupr(p->Service);
            // WinInfo("NUMCV=[%d]",p->NumCv);
            // WinInfo("NUMPORT=[%d]",p->NumPort);
            // WinInfo("DEPT=[%d]",p->Dept);
            // WinInfo("NUMCOMP=[%d]",p->NumComp);
            // WinInfo("PALIER=[%d]",p->Palier);
            // WinInfo("PAVI=[%s]",p->Pavi);
            // WinInfo("SERVICE=[%s]",p->Service);
            _dos_getdate(&p->DateCnx);
            _dos_gettime(&p->HeureCnx);
            if(! NbAppTot)
               _dos_gettime(&HPreCnx);
            _dos_gettime(&HDerCnx);
            p->pApp = SearchApp(p->Service,p->NumComp);       // Connexion avec l'application appele.
            if((! p->pApp) && (bAutoCNX))
            {
               // Application non trouve, on initialise l'auto-connexion.
               p->pApp = SearchApp(NomAppAutoCNX,0);
               if(p->pApp)
                  strcpy(p->Service,p->pApp->Nom);
            }
            if(! p->pApp)
            {
               // Application non trouve !!
               tPorts[pDT->iPort].CommEtabli(pDT->iPort,p->NumCv);
               sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               if(p->Service)
                  sprintf(OBuff,"APPLICATION %s INCONNUE !!",p->Service);
               else
                  sprintf(OBuff,"APPLICATION INCONNUE !!");
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               sprintf(OBuff,"\x0A");
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               p->bPaqLu = mtFALSE;
               break;
            }
            if(! p->pApp->Active)
            {
               // L'application existe, mais elle ne possde pas
               // de procdure MAIN, c'est surement une librairie.
               tPorts[pDT->iPort].CommEtabli(pDT->iPort,p->NumCv);
               sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               if(p->Service)
                  sprintf(OBuff,"APPLICATION %s NON VALIDE !!",p->Service);
               else
                  sprintf(OBuff,"APPLICATION NON VALIDE !!");
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               sprintf(OBuff,"\x0A");
               tPorts[pDT->iPort].SendDATA(pDT->iPort,p->NumCv,OBuff);
               p->bPaqLu = mtFALSE;
               p->Actif  = mtFALSE;
               break;
            }
            if(p->pApp->ConfCNX)
               tPorts[pDT->iPort].CommEtabli(pDT->iPort,p->NumCv);
            InitCnxCv(p);                           // Init. des info.
            if(pslash)
               AppendCvData(p,pslash,strlen(pslash));      // On ajoute les donnes reues.
            else
               AppendCvData(p,pDT->pData,pDT->wLen);       // On ajoute les donnes reues.
            Send0(cBann,p);
            _EnqRom();                              // Identification du minitel.
            p->bPaqLu = mtTRUE;
            tPorts[pDT->iPort].NbPaqLu++;
            NbAppTot++;
            break;
         }
         case X25LIB:
         {
            WinInfo("LIB: Voie=%d, %s",p->NumCv,pDT->pData);
            if(p->Actif)
            {
               if(p->pApp->bOnLib)
                  // Procdure ONLIB dfinie.
                  InitOnLibCv(p);
               else
                  // Libration du connect et dsallocation de son Cv.
                  DeconnecteCv(p);
            }
            break;
         }
         case X25NOTERM:
         {
            //
            // Arret de la voie courante.
            if(p->Actif)
            {
               p->ArretVoie = mtTRUE;
               WinInfo("Arret Voie=%d, %s",p->NumCv,pDT->pData);
            }
            break;
         }
         case X25TERM:
         {
            //
            // Reprise de la voie courante.
            if(p->Actif)
            {
               p->ArretVoie = mtFALSE;
               WinInfo("Reprise Voie=%d, %s",p->NumCv,pDT->pData);
            }
            break;
         }
         case X25NORES:
         {
            //
            // Arret de la carte
            tPorts[pDT->iPort].ArretCarte = mtTRUE;
            WinInfo("Arret carte, %s",pDT->pData);
            break;
         }
         case X25RES:
         {
            //
            // Reprise de la carte
            tPorts[pDT->iPort].ArretCarte = mtFALSE;
            WinInfo("Reprise carte, %s",pDT->pData);
            break;
         }
         case X25ERRP:
         {
            //
            // Erreur de procdure.
            WinInfo("Erreur de procdure Voie=%d, %s",p->NumCv,pDT->pData);
            break;
         }
         case X25NOPAD:
         {
            //
            // Suppression de l'cho du PAD.
            WinInfo("Echo du PAD=OFF Voie=%d, %s",p->NumCv,pDT->pData);
            break;
         }
         case X25PAD:
         {
            //
            // Activation de l'cho du PAD.
            WinInfo("Echo du PAD=ON Voie=%d, %s",p->NumCv,pDT->pData);
            break;
         }
         default:
         {
            WinInfo("** ERREUR PAQUET Voie=%d, %s **",p->NumCv,pDT->pData);
            break;
         }
      }
      FreePData(pDT);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

