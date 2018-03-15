// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณLOCALSER.C   ณ                                                             ณ
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
// ณDernire modification : 21/11/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des donnes sries issue d'un port local.                          ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <direct.h>
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
extern   char      cBann[81];
extern   struct    dos_time_t HPreCnx;
extern   struct    dos_time_t HDerCnx;
extern   mtUDWORD  NbAppTot;
extern   UnCv      TabCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   Ports     tPorts[MAXPORTS];
extern   mtBOOL    bBipOnCnx;
extern   mtBOOL    bAutoCNX;
extern   char      *NomAppAutoCNX;

extern   void      SWVRingBell(void);
extern   int       SWCPeekCharacter(int);
extern   int       SWCSearchForCharacter(int,mtBYTE);
extern   int       SWCReceiveCharacter(int);
extern   int       SWCReceiveBuffer(int,mtPTRBYTE,mtDWORD,int,mtUDWORD *);
extern   mtUDWORD  SWCTransmitBuffer(int,mtPTRBYTE);
extern   void      SWCPurgeTransmitQueue(int);
extern   void      SWCFlushTransmitQueue(int);
extern   void      FreeCvData(mtPTRCV);
extern   void      InitCnxCv(mtPTRCV);
extern   void      DeconnecteCv(mtPTRCV);
extern   void      InitOnLibCv(mtPTRCV);
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      Send0(mtPTRBYTE,mtPTRCV);
extern   mtPTRAPP  SearchApp(mtPTRBYTE,mtBYTE);
extern   void      SeqEsc(mtPTRBYTE,mtPTRCV);
extern   void      _EnqRom(void);
extern   float     TimeToCent(struct dos_time_t *);
extern   void      WriteLogPaq(mtPTRBYTE,...);
extern   void      PushPData(int,mtUWORD,mtPTRBYTE,mtWORD,mtBYTE);
extern   mtBOOL    SearchPData(mtUWORD,mtPTRWORD);
extern   void      FreePData(mtPTRPDATA);
extern   mtPTRPDATA PopPData(mtUWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
mtBOOL LOCReadPaquet(int);
void   LOCSendPaquet(int,mtBYTE,mtUWORD,mtPTRBYTE);
void   LOCCommLibere(int,mtUWORD);
void   LOCCommLibereAll(int);
mtBOOL LOCInitialisePort(int);
mtBOOL LOCTestTransmission(int, mtUWORD, mtPTRWORD);
mtBOOL LOCConnect(mtPTRBYTE,int);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet sur le port local nPort.
// Renvoie VRAI si un paquet est lu.
//
mtBOOL LOCReadPaquet(int nPort)
{
   char PaqTmp[80];
   int  c;
   mtBOOL  bCSI=mtFALSE;
   mtUWORD NumCv;
   mtPTRCV p;

   // Le Numro de CV est dtermin par le numro du port
   // (dans ce cas, les ports locaux = COM1..COM4)
   // + le nombre de CV grs par X25
   //
   NumCv    = (mtUWORD) (nPort+MAXCV);
   p        = &TabCv[NumCv];

   //
   // On vrifie qu'il y ai quelque chose … lire.
   //
   c = SWCReceiveCharacter(nPort);
   if(c == SWCQUEUEISEMPTY)
      return(mtFALSE);
   //
   // Stockage dans le buffer temporaire.
   //
   tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = c;
   tPorts[nPort].wInBuf++;
   if(tPorts[nPort].wInBuf >= LEN_PBUFF)
   {
      // Buffer plein.
      WinInfo("LOCAL: Buffer plein");
      WriteLogPaq("LOCAL: Buffer plein");
      // On empile le paquet.
      PushPData(nPort,NumCv,tPorts[nPort].Buffer,tPorts[nPort].wInBuf,X25DATA);
      //
      // On remet … zro le buffer temporaire.
      //
      memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
      tPorts[nPort].wInBuf = 0;
      return(mtTRUE);
   }
   if(p->Actif)
   {
      if(p->bWaitChar)
      {
         // Attente d'un caractre sur le flux.
         if(c == p->WaitChar)
         {
            // On empile le paquet.
            PushPData(nPort,NumCv,tPorts[nPort].Buffer,tPorts[nPort].wInBuf,X25DATA);
            //
            // On remet … zro le buffer temporaire.
            //
            memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
            tPorts[nPort].wInBuf = 0;
            return(mtTRUE);
         }
         else
            return(mtFALSE);
      }
   }
   if(strstr(tPorts[nPort].Buffer,CSI))
      bCSI = mtTRUE;
   //
   // On attend qu'un SEP, un CR ou CSI soit prsent
   // dans le buffer de rception.
   //
   if(c == MSep)
   {
      //
      // Ici, on a reu un SEP.
      // Le type du paquet est dtermin par le code de la touche de fonction
      // reue.
      //
      c = SWCReceiveCharacter(nPort);
      while(c == SWCQUEUEISEMPTY)
         c = SWCReceiveCharacter(nPort);
      tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = c;
      tPorts[nPort].wInBuf++;
   }
   else
   {
      if(c != mtCR)
         // Ni Sep ni Cr.
         return(mtFALSE);
   }
   if(((! bCSI) && (c == MCnxFin)) || ((! bCSI) && (strstr(tPorts[nPort].Buffer,"\x1B\x29\x34"))))
   {
      // Paquet d'appel.
      // On enlve le SEP+I du buffer.
      tPorts[nPort].wInBuf -= 2;
      tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = '\0';
      // Paquet d'appel (simul par Shift+CnxFin (SEP+I).
      memset(PaqTmp,0,80);
      // Fabrication du paquet d'appel.
      strcat(PaqTmp,"L99LOCPAV99/99/0/00/");
      strcat(PaqTmp,tPorts[nPort].Buffer);
      // On empile le paquet.
      PushPData(nPort,NumCv,PaqTmp,strlen(PaqTmp),X25CNX);
      //
      // On remet … zro le buffer temporaire.
      //
      memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
      tPorts[nPort].wInBuf = 0;
      return(mtTRUE);
   }
   // Paquet de donnes.
   // On empile le paquet.
   PushPData(nPort,NumCv,tPorts[nPort].Buffer,tPorts[nPort].wInBuf,X25DATA);
   //
   // On remet … zro le buffer temporaire.
   //
   memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
   tPorts[nPort].wInBuf = 0;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi un paquet sur le port nPort.
//
void LOCSendPaquet(int nPort, mtBYTE pType, mtUWORD nCv, mtPTRBYTE pBufData)
{
   SWCTransmitBuffer(nPort,pBufData);
   tPorts[nPort].NbPaqOut++;
   WriteLogPaq("Ecriture Paquet, Type=%c,Cv=%d,%s",pType,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un paquet de donnes.
//
void LOCSendDATA(int nPort, mtUWORD nCv, mtPTRBYTE pBufData)
{
   LOCSendPaquet(nPort,X25DATA,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre la connexion de nCv sur le port nPort.
//
void LOCCommLibere(int nPort, mtUWORD nCv)
{
   char OBuff[50];
   mtWORD wPos;

   SWCFlushTransmitQueue(nPort);
   sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
   strcat(OBuff,"FIN DE COMMUNICATION");
   LOCSendPaquet(nPort,X25LIB,nCv,OBuff);
   SWCPurgeTransmitQueue(nPort);
   // On limine de la pile les paquets de ce cv.
   while(SearchPData(nCv,&wPos))
      FreePData(PopPData(wPos));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre le cv donn.
//
void LOCLibereCV(mtUWORD nCv)
{
   LOCCommLibere(TabCv[nCv].NumPort,nCv);
   if(TabCv[nCv].pApp->bOnLib)
      InitOnLibCv(&TabCv[nCv]);
   else
      DeconnecteCv(&TabCv[nCv]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre toutes les voies actives du port nPort.
//
void LOCCommLibereAll(int nPort)
{
   mtUWORD i;

   for(i=MAXCV;i<TOTALCV;i++)
   {
      if(TabCv[i].Actif)
         LOCLibereCV(i);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisations diverse du port.
// Renvoie VRAI si tout c'est bien pass, FAUX sinon.
//
mtBOOL LOCInitialisePort(int nPort)
{
   char OBuff[50];

   // Reset du minitel connect.
   SWCTransmitBuffer(nPort,"\x1B\x39\x7F");
//   SWCTransmitBuffer(nPort,"\x1B\x5B\x3F\x7B");
   // CLS.
   SWCTransmitBuffer(nPort,"\x0C");
   // Affichage de la bannire en ligne 0.
   sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
   SWCTransmitBuffer(nPort,OBuff);
   SWCTransmitBuffer(nPort,cBann);
   SWCTransmitBuffer(nPort,"\x0A");
   // Affichage d'un message ligne 10.
   sprintf(OBuff,"\x1F%c%c\x18",0x4A,0x41);
   SWCTransmitBuffer(nPort,OBuff);
   SWCTransmitBuffer(nPort,"    ENTREZ LE NOM D'UNE APPLICATION");
   SWCTransmitBuffer(nPort,"\x0D\x0A");
   SWCTransmitBuffer(nPort,"    PUIS TAPEZ SHIFT+CONNEXION/FIN");
   SWCTransmitBuffer(nPort,"\x0D\x0A");
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on peut emmettre pour le cv donn.
//
mtBOOL LOCTestTransmission(int nPort, mtUWORD nCv, mtPTRWORD cLen)
{
   *cLen = 128;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Connexion d'un port local … une application.
// (commande CONNECT)
//
mtBOOL LOCConnect(mtPTRBYTE NomApp,int nPort)
{
   struct dos_time_t HPaq;
   char OBuff[50];
   mtBOOL bPaqLu=mtFALSE;
   mtUWORD NumCv;
   mtPTRCV p;

   // Le Numro de CV est dtermin par le numro du port
   // (dans ce cas, les ports locaux = COM1..COM4)
   // + le nombre de CV grs par X25
   //
   NumCv    = (mtUWORD) (nPort+MAXCV);
   p        = &TabCv[NumCv];
   p->NumCv = NumCv;

   if(bBipOnCnx)
      SWVRingBell();
   if(p->Actif)
   {
      WinInfo("CV DEJA ACTIF");
      if(p->pApp->bOnLib)
         // Procdure ONLIB dfinie.
         InitOnLibCv(p);
      else
         // On libre la prcdente connexion.
         DeconnecteCv(p);
   }
   p->NumCv   = NumCv;
   p->Actif   = mtTRUE;
   p->NumPort = nPort;
   p->Dept    = 99;
   strcpy(p->Pavi,"999999");
   p->Pavi[6] = '\0';
   p->Palier  = 99;
   p->NumComp = 100;
   strncpy(p->Service,NomApp,10);
   p->Service[10] = '\0';
   strupr(p->Service);
   _dos_getdate(&p->DateCnx);
   _dos_gettime(&p->HeureCnx);
   if(! NbAppTot)
      _dos_gettime(&HPreCnx);
   _dos_gettime(&HDerCnx);
   p->pApp = SearchApp(p->Service,p->NumComp);     // Connexion avec l'application appele.
   if((! p->pApp) && (bAutoCNX))
   {
      // Application non trouve, on initialise l'auto-connexion.
      p->pApp = SearchApp(NomAppAutoCNX,0);
      if(p->pApp)
         strcpy(p->Service,p->pApp->Nom);
   }
   if(! p->pApp)
   {
      // Application non trouve et pas d'auto-connexion
      sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      if(p->Service)
         sprintf(OBuff,"APPLICATION %s INCONNUE !!",p->Service);
      else
         sprintf(OBuff,"APPLICATION INCONNUE !!");
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      sprintf(OBuff,"\x0A");
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      bPaqLu    = mtFALSE;
      p->bPaqLu = mtFALSE;
      return(bPaqLu);
   }
   if(! p->pApp->Active)
   {
      // L'application existe, mais elle ne possde pas
      // de procdure MAIN, c'est surement une librairie.
      sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      if(p->Service)
         sprintf(OBuff,"APPLICATION %s NON VALIDE !!",p->Service);
      else
         sprintf(OBuff,"APPLICATION NON VALIDE !!");
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      sprintf(OBuff,"\x0A");
      LOCSendPaquet(nPort,X25DATA,NumCv,OBuff);
      bPaqLu    = mtFALSE;
      p->bPaqLu = mtFALSE;
      p->Actif  = mtFALSE;
      return(bPaqLu);
   }
   InitCnxCv(p);                                     // Init. des info.
   Send0(cBann,p);
   if(p->cMode != TMODE_TELINFO)
      _EnqRom();                                     // Identification du minitel.
   bPaqLu    = mtTRUE;
   p->bPaqLu = mtTRUE;
   tPorts[nPort].NbPaqLu++;
   NbAppTot++;
   return(bPaqLu);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Confirme la connexion de nCv sur le port nPort.
//
void LOCCommEtabli(int nPort, mtUWORD nCv)
{
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

