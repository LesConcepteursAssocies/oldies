// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณCORINTE.C    ณ                                                             ณ
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
// ณDernire modification : 24/10/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des trames X25 sur carte CORINTE                                   ณ
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

//
// Dfinition des types de trames X25 en format ASM (CORINTE).
//
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
// Types supplmentaires en format ASMT.
//
#define  X25BDATA       0x3A           // Trame de donnes avec bit qualificateur.
#define  X25CNXE        0x3B           // Trame d'appel tendue.
#define  X25CONFE       0x3C           // Trame de confirmation d'appel tendu.
#define  X25LIBE        0x3D           // Trame de libration tendue.
#define  X25ERR         0x3F           // Trame d'erreur.

#define  STX            0x02           // Dbut de paquet.
#define  ETX            0x03           // Fin de paquet.
#define  LENPAQMIN      3              // Longueur mini d'un paquet.
#define  LENPAQMAX      132            // Longueur maxi d'un paquet.

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
extern   void      FreeCvData(mtPTRCV);               //      "
extern   void      InitCnxCv(mtPTRCV);
extern   void      DeconnecteCv(mtPTRCV);
extern   void      InitOnLibCv(mtPTRCV);
extern   void      WinInfo(mtPTRBYTE,...);            // Dans AFFICHAG.CPP
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
mtBOOL X25ReadPaquet(int);
mtBOOL X25ReadPaquetCV(int);
void   X25SendPaquet(int,mtBYTE,mtUWORD,mtPTRBYTE);
void   X25CommEtabli(int,mtUWORD);
void   X25CommLibere(int,mtUWORD);
void   X25CommLibereAll(int);
mtBOOL X25InitialisePort(int);
mtBOOL X25TestTransmission(int, mtUWORD, mtPTRWORD);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet de chaque cv actif.
//
mtBOOL X25ReadPaquet(int nPort)
{
   mtUWORD  uCv;
   mtBOOL bLu = mtFALSE;

   for(uCv=0;uCv<MAXCV;uCv++)
   {
      bLu |= X25ReadPaquetCV(nPort);
   }
   return(bLu);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet sur le port pPort.
// Renvoie VRAI si un paquet est lu.
//
mtBOOL X25ReadPaquetCV(int nPort)
{
   int  c;
   mtUWORD NumCv;
   mtBYTE Type;
   struct dos_time_t HDeb,HFin;
   float dHDeb,dHFin;

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
      WinInfo("X25: Buffer plein");
      WriteLogPaq("X25: Buffer plein");
      NumCv = (tPorts[nPort].Buffer[2] - 0x40);
      Type  = tPorts[nPort].Buffer[1];
      // On empile le paquet.
      PushPData(nPort,NumCv,&tPorts[nPort].Buffer[3],tPorts[nPort].wInBuf-3,Type);
      //
      // On remet … zro le buffer temporaire.
      //
      memset(tPorts[nPort].Buffer,0,LEN_PBUFF+1);
      tPorts[nPort].wInBuf = 0;
      return(mtTRUE);
   }

   if(c == STX)
   {
      // Un paquet arrive.
      _dos_gettime(&HDeb);
      dHDeb = TimeToCent(&HDeb)*10;
      while(c != ETX)
      {
         c = SWCReceiveCharacter(nPort);
         if(c == SWCQUEUEISEMPTY)
            break;
         tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = c;
         tPorts[nPort].wInBuf++;
         // On gre un TimeOut de 55 ms maxi
         // (… 19200 bps, un paquet arrive en environ 53,4 ms)
         _dos_gettime(&HFin);
         dHFin = TimeToCent(&HFin)*10;
         if(dHFin<dHDeb)
            // A cheval sur 2 jours.
            dHFin += (float) 86400000;
         if((dHFin - dHDeb) >= 55)
            break;
      }
   }
   //
   // On attend que ETX soit prsent dans le buffer de rception.
   //
   if(c != ETX)
      return(mtFALSE);
   //
   // On a reu ETX, Lecture du paquet
   //
   NumCv = (tPorts[nPort].Buffer[2] - 0x40);
   Type  = tPorts[nPort].Buffer[1];
   // On enlve ETX du buffer.
   tPorts[nPort].wInBuf--;
   tPorts[nPort].Buffer[tPorts[nPort].wInBuf] = '\0';
   PushPData(nPort,NumCv,&tPorts[nPort].Buffer[3],tPorts[nPort].wInBuf-3,Type);
   //
   // On remet … zro le buffer temporaire.
   //
   memset(tPorts[nPort].Buffer,0,LEN_PBUFF);
   tPorts[nPort].wInBuf = 0;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi un paquet sur le port nPort.
//
void X25SendPaquet(int nPort, mtBYTE pType, mtUWORD nCv, mtPTRBYTE pBufData)
{
   char PaqTmp[LEN_PBUFF+1], *ptmp=PaqTmp;

   if(! tPorts[nPort].ArretCarte)
   {
      memset(ptmp,0,LEN_PBUFF+1);
      *ptmp++ = STX;
      *ptmp++ = pType;
      *ptmp++ = (mtBYTE) (0x40+nCv);
      strcat(ptmp,pBufData);
      ptmp = PaqTmp;
      *(ptmp+strlen(pBufData)+3) = ETX;
      SWCTransmitBuffer(nPort,ptmp);
      tPorts[nPort].NbPaqOut++;
      WriteLogPaq("Ecriture Paquet, Type=%c,Cv=%d,%s",pType,nCv,pBufData);
//      WinInfo("X25SendPaquet %s",ptmp);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un paquet de donnes.
//
void X25SendDATA(int nPort, mtUWORD nCv, mtPTRBYTE pBufData)
{
   X25SendPaquet(nPort,X25DATA,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Confirme la connexion de nCv sur le port nPort.
//
void X25CommEtabli(int nPort, mtUWORD nCv)
{
   X25SendPaquet(nPort,X25CONF,nCv,"");
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre la connexion de nCv sur le port nPort.
//
void X25CommLibere(int nPort, mtUWORD nCv)
{
   mtWORD wPos;

   X25SendPaquet(nPort,X25LIB,nCv,"0000");
   // On limine de la pile les paquets de ce cv.
   while(SearchPData(nCv,&wPos))
      FreePData(PopPData(wPos));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre le cv donn.
//
void X25LibereCV(mtUWORD nCv)
{
   X25CommLibere(TabCv[nCv].NumPort,nCv);
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
void X25CommLibereAll(int nPort)
{
   mtUWORD i;

   for(i=0;i<MAXCV;i++)
   {
      if(TabCv[i].Actif)
         X25LibereCV(i);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisations diverse du port.
// Renvoie VRAI si tout c'est bien pass, FAUX sinon.
//
mtBOOL X25InitialisePort(int nPort)
{
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on peut emmettre pour le cv donn.
//
mtBOOL X25TestTransmission(int nPort, mtUWORD nCv, mtPTRWORD cLen)
{
   *cLen = 128;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

