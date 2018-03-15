// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณMODEM.C      ณ                                                             ณ
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
// ณGestion des donnes sries issue d'un modem.                               ณ
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
extern   CODE4     NeoServ;
extern   char      cBann[81];
extern   struct    dos_time_t HPreCnx;
extern   struct    dos_time_t HDerCnx;
extern   mtUDWORD  NbAppTot;
extern   UnCv      TabCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   Ports     tPorts[MAXPORTS];
extern   mtBOOL    bBipOnCnx;
extern   mtBOOL    bAutoCNX;
extern   char      *NomAppAutoCNX;

extern   int       FOSSILSendChar(int,mtBYTE);
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
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void      mDelay(unsigned);
extern   void      PushPData(int,mtUWORD,mtPTRBYTE,mtWORD,mtBYTE);
extern   mtBOOL    SearchPData(mtUWORD,mtPTRWORD);
extern   void      FreePData(mtPTRPDATA);
extern   mtPTRPDATA PopPData(mtUWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
mtBOOL RTCReadPaquet(int);
void   RTCSendPaquet(int,mtBYTE,mtUWORD,mtPTRBYTE);
void   RTCCommLibere(int,mtUWORD);
void   RTCCommLibereAll(int);
mtBOOL RTCInitialisePort(int);
mtBOOL RTCWait(int,mtPTRBYTE,unsigned);
mtBOOL RTCTestTransmission(int, mtUWORD, mtPTRWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet sur le port RTC nPort.
// Renvoie VRAI si un paquet est lu.
//
mtBOOL RTCReadPaquet(int nPort)
{
   char PaqTmp[80];
   int  c,ReadStatus;
   mtUWORD NumCv;
   mtPTRCV p;

   // Le Numro de CV est dtermin par le numro du port
   // (dans ce cas, les ports locaux = COM1..COM4)
   // + le nombre de CV grs par X25
   //
   NumCv = (mtUWORD) (nPort+MAXCV);
   p     = &TabCv[NumCv];

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
   // Echo du caractre.
   if(p->Actif)
      ReadStatus = FOSSILSendChar(nPort,c);
   if(tPorts[nPort].wInBuf >= LEN_PBUFF)
   {
      // Buffer plein.
      WinInfo("RTC: Buffer plein");
      WriteLogPaq("RTC: Buffer plein");
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

   if(strncmp(tPorts[nPort].Buffer,"CONNECT",7) == 0)
   {
      // Paquet d'appel ("CONNECT ...").
      //
      // On enlve le CR du buffer.
      tPorts[nPort].wInBuf--;
      tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = '\0';
      // Fabrication du paquet d'appel.
      memset(PaqTmp,0,80);
      strcat(PaqTmp,"R99RTCPAV99/99/0/00/");
      strcat(PaqTmp,tPorts[nPort].NApp);
      // On empile le paquet.
      PushPData(nPort,NumCv,PaqTmp,strlen(PaqTmp),X25CNX);
      //
      // On remet … zro le buffer temporaire.
      //
      memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
      tPorts[nPort].wInBuf = 0;
      return(mtTRUE);
   }
   if((strncmp(tPorts[nPort].Buffer,"NO CARRIER",10) == 0) || (strncmp(tPorts[nPort].Buffer,"BUSY",4) == 0))
   {
      // Dconnexion.
      // On empile le paquet.
      PushPData(nPort,NumCv,tPorts[nPort].Buffer,tPorts[nPort].wInBuf,X25LIB);
      //
      // On remet … zro le buffer temporaire.
      //
      memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
      tPorts[nPort].wInBuf = 0;
      return(mtTRUE);
   }
   // Paquet de donnes ('xxx'+SEP+Code ou SEP+Code seul).
   // (ou xxx+CR)
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
void RTCSendPaquet(int nPort, mtBYTE pType, mtUWORD nCv, mtPTRBYTE pBufData)
{
   DATA4 *DbTmp;
   TAG4 *Idx;
   FIELD4 *Champ;
   char OBuff[80],*pBuf;
   mtPTRBYTE pRAC;
   int DbError,i;

   if(pType == X25LIB)
   {
      // Raccrochage du modem.
      // Ouverture de la base des configurations.
      DbTmp = d4open(&NeoServ,"MODEMS");
      if(! DbTmp)
      {
         WinInfo("Raccrochage: Impossible d'ouvrir la base MODEMS.DBF");
         WriteLogPaq("Raccrochage: Impossible d'ouvrir la base MODEMS.DBF");
      }
      else
      {
         // Slection de l'index maitre.
         Idx = d4tag_next(DbTmp,NULL);
         if(Idx)
            d4tag_select(DbTmp,Idx);
         // Recherche du modem.
         sprintf(OBuff,"%s",tPorts[nPort].Nom);
         DbError = d4seek(DbTmp,OBuff);
         if(DbError != r4success)
         {
            d4close(DbTmp);
            WinInfo("Raccrochage: Modem %s non trouv",OBuff);
            WriteLogPaq("Raccrochage: Modem %s non trouv",OBuff);
         }
         else
         {
            Champ = d4field(DbTmp,"HANGUP");
            pRAC  = f4str(Champ);
            if(pRAC)
            {
               memset(OBuff,0,80);
               pBuf = OBuff;
               strcpy(pBuf,pRAC);
               LTrim(pBuf);
               if(strlen(pBuf))
               {
                  // On enlve les espaces de fin.
                  i = strlen(pBuf)-1;
                  while(*pBuf && *(pBuf+i) == ' ')
                  {
                     *(pBuf+i) = 0;
                     i--;
                  }
                  if(strlen(pBuf))
                  {
                     SWCTransmitBuffer(nPort,pBuf);
                     SWCTransmitBuffer(nPort,"\x0D");
                     if(! RTCWait(nPort,"OK",500))
                     {
                        d4close(DbTmp);
                        WinInfo("Raccrochage: Erreur sur chaine HANGUP");
                        WriteLogPaq("Raccrochage: Erreur sur chaine HANGUP");
                     }
                  }
               }
            }
         }
      }
   }
   else
   {
      SWCTransmitBuffer(nPort,pBufData);
   }
   tPorts[nPort].NbPaqOut++;
   WriteLogPaq("Ecriture Paquet, Type=%c,Cv=%d,%s",pType,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un paquet de donnes.
//
void RTCSendDATA(int nPort, mtUWORD nCv, mtPTRBYTE pBufData)
{
   RTCSendPaquet(nPort,X25DATA,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre la connexion de nCv sur le port nPort.
//
void RTCCommLibere(int nPort, mtUWORD nCv)
{
   char OBuff[50];
   mtWORD wPos;

   SWCFlushTransmitQueue(nPort);
   sprintf(OBuff,"\x1F%c%c\x18",0x40,0x41);
   strcat(OBuff,"FIN DE COMMUNICATION");
   RTCSendPaquet(nPort,X25LIB,nCv,OBuff);
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
void RTCLibereCV(mtUWORD nCv)
{
   RTCCommLibere(TabCv[nCv].NumPort,nCv);
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
void RTCCommLibereAll(int nPort)
{
   mtUWORD i;

   for(i=MAXCV;i<TOTALCV;i++)
   {
      if(TabCv[i].Actif)
         RTCLibereCV(i);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisations diverse du port.
// Renvoie VRAI si tout c'est bien pass, FAUX sinon.
//
mtBOOL RTCInitialisePort(int nPort)
{
   DATA4 *DbTmp;
   TAG4 *Idx;
   FIELD4 *Champ;
   char OBuff[80],*pBuf;
   mtPTRBYTE pRAZ,pInit;
   int DbError,i;

   // Ouverture de la base des configurations.
   DbTmp = d4open(&NeoServ,"MODEMS");
   if(! DbTmp)
   {
      WinMsgErr(": Impossible d'ouvrir la base","RTCInitialisePort MODEMS.DBF");
      return(mtFALSE);
   }
   // Slection de l'index maitre.
   Idx = d4tag_next(DbTmp,NULL);
   if(Idx)
      d4tag_select(DbTmp,Idx);
   // Recherche du modem.
   sprintf(OBuff,"%s",tPorts[nPort].Nom);
   DbError = d4seek(DbTmp,OBuff);
   if(DbError != r4success)
   {
      d4close(DbTmp);
      WinMsgErr(": Modem non trouv","RTCInitialisePort %s",OBuff);
      return(mtFALSE);
   }
   // Chaine de RAZ.
   Champ = d4field(DbTmp,"RAZ");
   pRAZ  = f4str(Champ);
   if(pRAZ)
   {
      memset(OBuff,0,80);
      pBuf = OBuff;
      strcpy(pBuf,pRAZ);
      LTrim(pBuf);
      if(strlen(pBuf))
      {
         // On enlve les espaces de fin.
         i = strlen(pBuf)-1;
         while(*pBuf && *(pBuf+i) == ' ')
         {
            *(pBuf+i) = 0;
            i--;
         }
         if(strlen(pBuf))
         {
            SWCTransmitBuffer(nPort,pBuf);
            SWCTransmitBuffer(nPort,"\x0D");
            if(! RTCWait(nPort,"OK",500))
            {
               d4close(DbTmp);
               WinMsgErr(": Erreur sur chaine RAZ","RTCInitialisePort");
               return(mtFALSE);
            }
         }
      }
   }
   // Chaine d'init de la vitesse donne.
   sprintf(OBuff,"INIT%lu",tPorts[nPort].SpeedCom);
   Champ = d4field(DbTmp,OBuff);
   if(! Champ)
   {
      d4close(DbTmp);
      WinMsgErr(": Vitesse non trouve","RTCInitialisePort %s",OBuff);
      return(mtFALSE);
   }
   pInit  = f4str(Champ);
   if(pInit)
   {
      memset(OBuff,0,80);
      pBuf = OBuff;
      strcpy(pBuf,pInit);
      LTrim(pBuf);
      if(strlen(pBuf))
      {
         // On enlve les espaces de fin.
         i = strlen(pBuf)-1;
         while(*pBuf && *(pBuf+i) == ' ')
         {
            *(pBuf+i) = 0;
            i--;
         }
         if(strlen(pBuf))
         {
            SWCTransmitBuffer(nPort,pBuf);
            SWCTransmitBuffer(nPort,"\x0D");
            if(! RTCWait(nPort,"OK",500))
            {
               d4close(DbTmp);
               sprintf(OBuff,"INIT%lu",tPorts[nPort].SpeedCom);
               WinMsgErr(": Erreur sur chaine","RTCInitialisePort %s",OBuff);
               return(mtFALSE);
            }
         }
         else
         {
            d4close(DbTmp);
            sprintf(OBuff,"INIT%lu",tPorts[nPort].SpeedCom);
            WinMsgErr(": Chaine non prsente","RTCInitialisePort %s",OBuff);
            return(mtFALSE);
         }
      }
   }
   d4close(DbTmp);
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Attend la chaine donne sur le port donn, avec le timeout donn (en ms).
//
mtBOOL RTCWait(int nPort,mtPTRBYTE pCh,unsigned uMs)
{
   char CTmp[LEN_PBUFF+1];
   int c,i;

   i = 0;
   memset(CTmp,0,LEN_PBUFF+1);
   mDelay(uMs);
   while(1)
   {
      c = SWCReceiveCharacter(nPort);
      if(c == SWCQUEUEISEMPTY)
         break;
      CTmp[i++] = c;
   }
   if(strstr(CTmp,pCh))
      return(mtTRUE);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on peut emmettre pour le cv donn.
//
mtBOOL RTCTestTransmission(int nPort, mtUWORD nCv, mtPTRWORD cLen)
{
   *cLen = 128;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Confirme la connexion de nCv sur le port nPort.
//
void RTCCommEtabli(int nPort, mtUWORD nCv)
{
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

