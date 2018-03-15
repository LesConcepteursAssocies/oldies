// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณLIBCOM.C     ณ                                                             ณ
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
// ณDernire modification : 17/10/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des ports de communication                                         ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <string.h>
#include <dos.h>
#include <direct.h>
#include "d4all.h"
#pragma SC align
#include "minitel.h"
#include "generic.h"
#include "ports.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"

Ports    tPorts[MAXPORTS];
int      CycPort;
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   int        FOSSILInitDriver(int);
extern   int        SWCPeekCharacter(int);
extern   void       SWCCloseComm(int);
extern   int        SWCSetUART(int,mtUDWORD,mtUSHORT,mtUSHORT,mtUSHORT);
extern   void       SWCFlushTransmitQueue(int);
extern   void       SWCFlushReceiveQueue(int);
extern   UnCv       TabCv[TOTALCV];    // Dfini Dans le prog. principal.
extern   UnCv       LTask[TOTALCV];    // Dfini Dans le prog. principal.
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       CCICloseCarte(int);
extern   void       WriteLogNeo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise les structures de donnes.
//
void RazPort(void)
{
   int i;

   CycPort = 0;
   for(i = 0;i<MAXPORTS;i++)
   {
      tPorts[i].Actif      = mtFALSE;
      tPorts[i].ArretCarte = mtFALSE;
      tPorts[i].Type       = TPORTX25ASM;
      memset(tPorts[i].Nom,0,26);
      memset(tPorts[i].NApp,0,11);
      tPorts[i].EtatM      = 0;
      tPorts[i].EtatL      = 0;
      memset(tPorts[i].Buffer,0,512);
      tPorts[i].wInBuf     = 0;
      tPorts[i].SpeedCom   = (mtUDWORD) 0;
      tPorts[i].Parite     = SWPARITYNONE;
      tPorts[i].WordLen    = (mtUSHORT) 0;
      tPorts[i].StopBit    = (mtUSHORT) 0;
      tPorts[i].NbPaqLu    = (mtUDWORD) 0;
      tPorts[i].NbPaqErr   = (mtUDWORD) 0;
      tPorts[i].NbPaqOut   = (mtUDWORD) 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ouverture des ports.
// Renvoie vrai si tout c'est bien pass.
//
mtBOOL OpenPorts(void)
{
   int i,ComStatus,PortActif = 0;

   for(i = 0;i<MAXPORTS;i++)
   {
      if((tPorts[i].Actif) && (tPorts[i].Type != TPORTX25MITEC))
      {
         if(FOSSILInitDriver(i))
         {
            tPorts[i].Actif = mtFALSE;
            WinInfo("","Erreur … l'ouverture du port %d",i);
            WriteLogNeo("","Erreur … l'ouverture du port %d",i);
            continue;
         }
         if((ComStatus = SWCSetUART(i,tPorts[i].SpeedCom,tPorts[i].Parite,tPorts[i].WordLen,tPorts[i].StopBit)) != SWCSUCCESSFUL)
         {
            SWCCloseComm(i);
            tPorts[i].Actif = mtFALSE;
            WinInfo("","Erreur %d … l'initialisation du port %d",ComStatus,i);
            WriteLogNeo("","Erreur %d … l'initialisation du port %d",ComStatus,i);
            continue;
         }
         PortActif++;
         SWCFlushReceiveQueue(i);
         SWCFlushTransmitQueue(i);
         tPorts[i].NbPaqLu  = (mtUDWORD) 0;
         tPorts[i].NbPaqErr = (mtUDWORD) 0;
         tPorts[i].NbPaqOut = (mtUDWORD) 0;
      }
   }
   if(PortActif)
      return(mtTRUE);
   else
      return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fermeture des ports.
//
void ClosePorts(void)
{
   int i;

   for(i=0;i<MAXPORTS;i++)
   {
      if((tPorts[i].Actif) && (tPorts[i].Type == TPORTX25MITEC))
      {
         CCICloseCarte(i);
         continue;
      }
      if((tPorts[i].Actif) && (tPorts[i].Type != TPORTX25MITEC))
      {
         SWCCloseComm(i);
         tPorts[i].Actif = mtFALSE;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture cyclique des ports.
//
mtBOOL ReadPort(void)
{
   mtBOOL bRead;

   if(CycPort>=MAXPORTS)
      CycPort = 0;
   if(tPorts[CycPort].Actif)
      bRead = tPorts[CycPort].ReadPaquet(CycPort);
   CycPort++;
   return(bRead);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libration des voies.
//
void LibPort(void)
{
   int i;

   for(i=0;i<MAXPORTS;i++)
   {
      if(tPorts[i].Actif)
      {
         tPorts[i].CommLibereAll(i);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise les ports actifs.
//
void InitialiseAllPort(void)
{
   int i;

   for(i=0;i<MAXPORTS;i++)
   {
      if(tPorts[i].Actif)
      {
         if(! tPorts[i].InitialisePort(i))
         {
            WinInfo("ERREUR: InitialisePort %d",i);
            WriteLogNeo("ERREUR: InitialisePort %d",i);
            tPorts[i].Actif = mtFALSE;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Vide le buffer du connect donn.
//
void VideCvBuffer(mtUWORD Cv)
{
   int lBuff,lbT,iLen;
   mtBYTE nPort,bTmp[513];

   if((TabCv[Cv].Actif) && (! TabCv[Cv].ArretVoie) && (TabCv[Cv].pOutBuff))
   {
      nPort = TabCv[Cv].NumPort;
      if(tPorts[nPort].TestTX(nPort,Cv,&iLen))
      {
         lBuff = strlen(TabCv[Cv].pOutBuff);
//         while(lBuff > 0)
         if(lBuff > 0)
         {
            // On prend iLen caractres.
            memset(bTmp,0,513);
            if(iLen>512)
               iLen = 512;
            strncpy(bTmp,TabCv[Cv].pOutBuff,iLen);
            lbT = strlen(bTmp);
            // On envoie ce paquet.
            tPorts[nPort].SendDATA(nPort,Cv,bTmp);
            if((lBuff-lbT) > 0)
               // On enlve les caractres envoys du buffer
               memmove(TabCv[Cv].pOutBuff,TabCv[Cv].pOutBuff+lbT,(lBuff-lbT)+1);
            lBuff -= lbT;
            if(lBuff)
            {
               // On rajuste la taille du buffer de sortie.
               TabCv[Cv].pOutBuff = (mtPTRBYTE) MemReAlloc(TabCv[Cv].pOutBuff,lBuff+1);
               *(TabCv[Cv].pOutBuff+lBuff) = '\0';
            }
            else
            {
               // Fini.
               MemFree(TabCv[Cv].pOutBuff);
               TabCv[Cv].pOutBuff = NULL;
               lBuff              = 0;
            }
         }
         // On met … jour le pointeur de la tche locale.
         if(LTask[Cv].Actif)
            LTask[Cv].pOutBuff = TabCv[Cv].pOutBuff;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Vide les buffers de sortie des voies.
//
void VideBuffer(void)
{
   mtUWORD i;

   for(i=0;i<TOTALCV;i++)
      VideCvBuffer(i);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Elimination des squences escape d'une chaine.
// Modifie la longueur de la chaine donne en consquences.
//
void SeqEsc(mtPTRBYTE Chaine,mtPTRCV pCv)
{
   mtPTRBYTE pTmp,pLig=Chaine;
   mtBYTE Car;

   if(((pCv->Actif) && (! pCv->bNoSeqEsc) && (pCv->cMode != TMODE_TELINFO)) && ((strchr(pLig,1)) || (strchr(pLig,mtUS)) || (strchr(pLig,27))))
   {
      while(*pLig)
      {
         if(*pLig == 1)
         {
            // Rponse
            // … l'EnqRom.
            pTmp = strchr(pLig+1,4);
            if(! pTmp)
               break;
            Car = *(pLig+2);
            switch(Car)
            {
               case 'b': { pCv->TypeMinitel = 0x62 ; break ; }   // Minitel M1.
               case 'c': { pCv->TypeMinitel = 0x63 ; break ; }   // Minitel M1.
               case 'r': { pCv->TypeMinitel = 0x72 ; break ; }   // Minitel M1.
               case 's': { pCv->TypeMinitel = 0x73 ; break ; }   // Minitel M1 couleur.
               case 'd': { pCv->TypeMinitel = 0x64 ; break ; }   // Minitel M10.
               case 'f': { pCv->TypeMinitel = 0x66 ; break ; }   // Minitel M10.
               case 'u': { pCv->TypeMinitel = 0x75 ; break ; }   // Minitel M1b.
               case 'w': { pCv->TypeMinitel = 0x77 ; break ; }   // Minitel M10b.
               case 'v':
               {
                  if(*(pLig+1) == 'B')
                     // Minitel M2.
                     pCv->TypeMinitel = 0x76;
                  else
                  {
                     if(*(pLig+1) == 'P')
                        // Minitel Rapide/Photo.
                        // Type 0x80 arbitraire.
                        pCv->TypeMinitel = 0x80;
                  }
                  break;
               }
               case 'z': { pCv->TypeMinitel = 0x7A ; break ; }   // Minitel M12.
               case 'y': { pCv->TypeMinitel = 0x79 ; break ; }   // Minitel M5.
               default : { pCv->TypeMinitel = 0x62 ;         }   // Minitel M1 par dfaut.
            }
            // On limine la squence de la chaine.
            memmove(pLig,pTmp+1,(strlen(Chaine)-((pTmp-pLig)+1))+1);
            if(! *pLig)
               break;
            continue;
         }
         if(*pLig == mtUS)
         {
            // Rponse
            // … la demande de position du curseur.
            Car           = *(pLig+1);
            pCv->Row      = Car-0x40;
            Car           = *(pLig+2);
            pCv->Col      = Car-0x40;
            pCv->bGCursor = mtTRUE;
            // On limine la squence de la chaine.
            memmove(pLig,pLig+3,(strlen(Chaine)-3)+1);
            if(! *pLig)
               break;
            continue;
         }
         if(*pLig == 27)
         {
            // Escape
            if(*(pLig+1) == 57)
            {
               // PRO1
               // On limine la squence de la chaine.
               memmove(pLig,pLig+3,(strlen(Chaine)-3)+2);
               if(! *pLig)
                  break;
               continue;
            }
            if(*(pLig+1) == 58)
            {
               // PRO2
               // On limine la squence de la chaine.
               memmove(pLig,pLig+4,(strlen(Chaine)-4)+2);
               if(! *pLig)
                  break;
               continue;
            }
            if(*(pLig+1) == 59)
            {
               // PRO3
               // On limine la squence de la chaine.
               memmove(pLig,pLig+5,(strlen(Chaine)-5)+2);
               if(! *pLig)
                  break;
               continue;
            }
            if((*(pLig+1) == 29) && (*(pLig+2) == 34))
            {
               // Shift+Connexion/fin en mode TELINFO.
               // On limine la squence de la chaine.
               memmove(pLig,pLig+3,(strlen(Chaine)-3)+2);
               if(! *pLig)
                  break;
               continue;
            }
            if((*(pLig+1) < 64) || (*(pLig+1) > 93))
            {
               // Esc + quelque chose...
               // On limine la squence de la chaine.
               memmove(pLig,pLig+1,(strlen(Chaine)-1)+2);
               if(! *pLig)
                  break;
               continue;
            }
         }
         pLig++;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

