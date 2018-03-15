// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณSWASYNC.C    ณ                                                             ณ
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
// ณDfinition des routines de communication compatible Silvercom(tm)          ณ
// ณ(Les communications sries sont gres par le driver FOSSIL).              ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <int.h>
#include "generic.h"
#include "ports.h"

#define mtLOWORD(l)           ((mtUSHORT)(mtDWORD)(l))
#define mtHIWORD(l)           ((mtUSHORT)((((mtDWORD)(l)) >> 16) & 0xFFFF))

#define  COM1      0
#define  COM2      1
#define  COM3      2
#define  COM4      3

#define  ERRPARITE 1
#define  ERRTRAME  2
#define  ERRBREAK  3

#pragma pack(1)
typedef struct _X00TagInfo
{
   mtWORD   InfoSize;
   mtBYTE   CurrFossil;
   mtBYTE   CurrRev;
   char far *IDString;
   mtWORD   IBSize;
   mtWORD   IFree;
   mtWORD   OBSize;
   mtWORD   OFree;
   mtBYTE   SWidth;
   mtBYTE   SHeight;
   mtBYTE   Baud;
} X00TagInfo;
#pragma pack()
typedef X00TagInfo * mtPTRX00;

mtUSHORT usStdUARTBaseIOAddress[MAXPORTS] = {0x3F8, 0x2F8, 0x3E8, 0x2E8};
mtUSHORT usStdIRQNumber[MAXPORTS]         = {4, 3, 4, 3};

X00TagInfo X00Info;
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   Ports      tPorts[MAXPORTS];
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   void       WinInfo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupre les informations du port donn.
//
void FOSSILGetInfo(int nPort)
{
   union REGS regs;
   struct SREGS sregs;
   X00TagInfo far *fp;
   unsigned long sAbsAdr;
   mtUSHORT sSeg,sOff;

   fp        = &X00Info;
   regs.h.ah = 0x1B;
   regs.x.dx = nPort;
   segread(&sregs);
   regs.x.cx = sizeof(X00TagInfo);
//   sregs.es  = FP_SEG(fp);
//   regs.x.di = FP_OFF(fp);
   sAbsAdr   = _x386_get_abs_address(fp);
   sOff      = mtLOWORD(sAbsAdr);
   sSeg      = mtHIWORD(sAbsAdr);
   sregs.es  = sSeg/16;
   regs.x.di = sOff;
   int86x_real(0x14,&regs,&regs,&sregs);
   WinInfo("DS=%04X Struct=%p fp=%p",getDS(),&X00Info,fp);
   WinInfo("Abs=%08lX sSeg=%04X %04X sOff=%04X",sAbsAdr,sSeg,(mtUSHORT) (sSeg/16),sOff);
   WinInfo("GETINFO (%d) AX=%d CH=%c CL=%c DX=%c",sizeof(X00TagInfo),regs.x.ax,regs.h.ch,regs.h.cl,regs.x.dx);
   WinInfo("InfoSize  =%d",fp->InfoSize);
   WinInfo("CurrFossil=%c",fp->CurrFossil);
   WinInfo("CurrRev   =%c",fp->CurrRev);
   WinInfo("IDString  =%Fs",fp->IDString);
   WinInfo("IBSize    =%d",fp->IBSize);
   WinInfo("IFree     =%d",fp->IFree);
   WinInfo("OBSize    =%d",fp->OBSize);
   WinInfo("OFree     =%d",fp->OFree);
   WinInfo("SWidth    =%d",fp->SWidth);
   WinInfo("SHeight   =%d",fp->SHeight);
   WinInfo("Baud      =%d",fp->Baud);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise le port donn.
//
int FOSSILInitDriver(int nPort)
{
   union REGS regs;
//   struct SREGS sregs;

   regs.h.ah = 0x04;
   regs.x.dx = nPort;
//   segread(&sregs);
//   int86x(0x14,&regs,&regs,&sregs);
   int86(0x14,&regs,&regs);
   if(regs.x.ax != 0x1954)
      return(1);
   return(0);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ferme le port donn.
//
void FOSSILCloseDriver(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x05;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un caractre et attente.
// Renvoi 1 si le car. n'a pu tre transmis.
//
int FOSSILSendCharWait(int nPort,mtBYTE sCar)
{
   union REGS regs;

   regs.h.ah = 0x01;
   regs.h.al = sCar;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   return(regs.x.ax & 128);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un caractre sans attente.
// Renvoi 0 si le car. n'a pu tre transmis.
//
int FOSSILSendChar(int nPort,mtBYTE sCar)
{
   union REGS regs;

   regs.h.ah = 0x0B;
   regs.h.al = sCar;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   return(regs.x.ax);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
int FOSSILCharReady(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x0C;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   if(regs.x.ax == 0xFFFF)
      return(0);
   return(1);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Extrait un caractre du tampon de rception,sans modifier celui-ci.
// (utilis par SearchForCharacter).
//
int SWCPeekCharacter(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x0C;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   if(regs.x.ax == 0xFFFF)
      return(SWCQUEUEISEMPTY);
   return((int) regs.h.al);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un caractre dans le tampon de rception du port donn.
//
int SWCSearchForCharacter(int nPort,mtBYTE bCharacter)
{
   int RetVal=SWCSUCCESSFUL;

   while(RetVal==SWCSUCCESSFUL)
   {
      RetVal = SWCPeekCharacter(nPort);
      if(RetVal < 0)
         break;
      if(bCharacter == (mtBYTE) RetVal)
         return(SWCSUCCESSFUL);
   }
   return(SWCQUEUEISEMPTY);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'une chaine.
//
mtUDWORD SWCTransmitBuffer(int nPort,mtPTRBYTE pbBuffer)
{
   int Status;
   mtUDWORD nbSend=(mtUDWORD) 0;

   while(*pbBuffer)
   {
      Status = FOSSILSendChar(nPort,*pbBuffer++);
      if(! Status)
         break;
      nbSend++;
   }
   return(nbSend);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Flushe le tampon de rception.
//
void SWCFlushReceiveQueue(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x0A;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Flushe le tampon d'mission.
//
void SWCFlushTransmitQueue(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x09;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi ce qui reste dans le tampon d'mission.
//
void SWCPurgeTransmitQueue(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x08;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un caractre dans le tampon de rception.
//
mtBYTE FOSSILGetChar(int nPort)
{
   union REGS regs;

   regs.h.ah = 0x02;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   return(regs.h.al);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
int SWCReceiveCharacter(int nPort)
{
   if(FOSSILCharReady(nPort))
      return(FOSSILGetChar(nPort));
   else
      return(SWCQUEUEISEMPTY);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'une chaine dans le tampon de rception.
//
int SWCReceiveBuffer(int nPort,mtPTRBYTE pszBuffer,mtDWORD ulMaxLength,int usTerminator,mtUDWORD *ulLength)
{
   int RetVal;
   mtBYTE InChar;
   mtBOOL bFini = mtFALSE;
   mtUDWORD NbLu = (mtUDWORD) 0;

//   if(SWCPeekCharacter(nPort) == SWCQUEUEISEMPTY)
//   {
//      RetVal = SWCQUEUEISEMPTY;
//      bFini  = mtTRUE;
//   }

   WinInfo("RECEIVEBUFFER");
   while(! bFini)
   {
      if(NbLu >= ulMaxLength)
      {
         bFini = mtTRUE;
         continue;
      }
      if(SWCPeekCharacter(nPort) != SWCQUEUEISEMPTY)
      {
         InChar            = (mtBYTE) SWCReceiveCharacter(nPort);
         pszBuffer[NbLu++] = (mtBYTE) InChar;
         if(usTerminator != -1 && InChar == (mtBYTE) usTerminator)
         {
            RetVal = SWCTERMINATORREACHED;
            bFini  = mtTRUE;
            continue;
         }
      }
      else
      {
         RetVal = SWCQUEUEISEMPTY;
         bFini  = mtTRUE;
         continue;
      }
   }
   pszBuffer[NbLu]='\0';
   if(ulLength)
      *ulLength=NbLu;
   return(RetVal);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ferme un port.
//
void SWCCloseComm(int nPort)
{
   FOSSILCloseDriver(nPort);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise l'uart d'un port.
//
int SWCSetUART(int nPort,mtUDWORD ulBaudRate,mtUSHORT usParity,mtUSHORT usDataBits,mtUSHORT usStopBits)
{
   union REGS regs;
   int Set = 0;

   switch(ulBaudRate)
   {
      case   300: { Set = 0x40; break; }
      case   600: { Set = 0x60; break; }
      case  1200: { Set = 0x80; break; }
      case  2400: { Set = 0xA0; break; }
      case  4800: { Set = 0xC0; break; }
      case  9600: { Set = 0xE0; break; }
      case 19200: { Set = 0x00; break; }
      case 38400: { Set = 0x02; break; }
         default: return(1);
   }
   switch(usParity)
   {
      case  SWPARITYODD: { Set |= 0x08; break; }
      case SWPARITYEVEN: { Set |= 0x18; break; }
                default: { Set |= 0x00; break; }
   }
   switch(usStopBits)
   {
      case  1: { Set |= 0x00; break; }
      default: { Set |= 0x04; break; }
   }
   switch(usDataBits)
   {
      case  5: { Set |= 0x00; break; }
      case  6: { Set |= 0x01; break; }
      case  7: { Set |= 0x02; break; }
      default: { Set |= 0x03; break; }
   }
   regs.h.ah = 0x00;
   regs.h.al = Set;
   regs.x.dx = nPort;
   int86(0x14,&regs,&regs);
   return(SWCSUCCESSFUL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

