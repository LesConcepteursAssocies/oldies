// ��������������������������������͵NEOSERV����������������������������������͸
// �DIVERS.C     �                                                             �
// ���������������                                                             �
// �Auteur                : Guillaumaud Philippe                               �
// �                        teknik@yaka2.com                                   �
// �                        http://www.yaka2.com/teknik                        �
// �                                                                           �
// �Vous �tes autoris� � diffuser et � utiliser ce logiciel sans restrictions. �
// �                                                                           �
// �Ce logiciel est fourni tel quel, sans aucune garantie.                     �
// �L'auteur ne saurait voir sa responsabilit� engag�e en cas de               �
// �dommages de quelque nature que ce soit subis par l'utilisateur             �
// �ou des tiers et r�sultant directement ou                                   �
// �indirectement de son utilisation, notamment la perte de donn�es,           �
// �ou toute perte financi�re r�sultant de son                                 �
// �utilisation ou de l'impossibilit� de l'utiliser, et ceci m�me si           �
// �l'auteur a �t� pr�venu de la possibilit� de tels dommages.                 �
// �                                                                           �
// �Commenc� le           : 09/06/1993                                         �
// �Derni�re modification : 31/07/1995                                         �
// �                                                                           �
// �Proc�dure diverses.                                                        �
// ���������������������������������������������������������������������������;

#include <stdio.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <stdarg.h>
#include <fcntl.h>
#include "generic.h"

mtWORD   FicLogNeo=-1;
mtWORD   FicLogPaq=-1;
mtBYTE   LogBuff[2048];
//
// ����������������������������������������������������������������������������
//
extern   mtBOOL     bTracePaq;
extern   char       *pFicPAQ;
extern   mtBOOL     bTraceNeo;
extern   char       *pFicNEO;
extern   mtBYTE     Heures;
extern   mtBYTE     Minutes;
extern   mtBYTE     Secondes;
extern   mtBYTE     Jour;
extern   mtBYTE     Mois;
extern   mtWORD     Annee;
extern   mtBOOL    _FileOpen(mtPTRBYTE,mtWORD,mtPTRWORD,mtBOOL);
extern   void      _FileClose(mtWORD);
extern   mtBOOL    _IsEOF(mtWORD);
extern   void      _FileRead(mtWORD,mtPTRBYTE,size_t);
extern   void      _FileReadLine(mtWORD,mtPTRBYTE,size_t);
extern   void      _FileWrite(mtWORD,mtPTRBYTE,size_t);
extern   void      _FileWriteLn(mtWORD,mtPTRBYTE,size_t);
extern   float     TimeToCent(struct dos_time_t *);
//
// ����������������������������������������������������������������������������
//
// Ouvre le fichier de trace des paquets
// Renvoie VRAI si tout c'est bien pass�.
//
mtBOOL OpenLogPaq(void)
{
   if(FicLogPaq != -1)
      _FileClose(FicLogPaq);
   return(_FileOpen(pFicPAQ,O_BINARY | O_APPEND,&FicLogPaq,mtTRUE));
}
//
// ����������������������������������������������������������������������������
//
// Ouvre le fichier de trace de N�oServ
// Renvoie VRAI si tout c'est bien pass�.
//
mtBOOL OpenLogNeo(void)
{
   if(FicLogNeo != -1)
      _FileClose(FicLogNeo);
   return(_FileOpen(pFicNEO,O_BINARY | O_APPEND,&FicLogNeo,mtTRUE));
}
//
// ����������������������������������������������������������������������������
//
// Ferme le fichier de trace des paquets.
//
void CloseLogPaq(void)
{
   if(bTracePaq)
   {
      _FileClose(FicLogPaq);
      FicLogPaq = -1;
   }
}
//
// ����������������������������������������������������������������������������
//
// Ferme le fichier de trace de N�oServ.
//
void CloseLogNeo(void)
{
   if(bTraceNeo)
   {
      _FileClose(FicLogNeo);
      FicLogNeo = -1;
   }
}
//
// ����������������������������������������������������������������������������
//
// Trace les informations donn�es dans le fichier des paquets.
//
void WriteLogPaq(mtPTRBYTE MsgLog, ...)
{
   va_list fParam;

   if(bTracePaq)
   {
      sprintf(LogBuff,"%02d/%02d/%04d � %02d:%02d:%02d, ",Jour,Mois,Annee,Heures,Minutes,Secondes);
      _FileWrite(FicLogPaq,LogBuff,strlen(LogBuff));
      va_start(fParam,MsgLog);
      vsprintf(LogBuff,MsgLog,fParam);
      _FileWriteLn(FicLogPaq,LogBuff,strlen(LogBuff));
   }
}
//
// ����������������������������������������������������������������������������
//
// Trace les informations donn�es dans le fichier de n�oserv.
//
void WriteLogNeo(mtPTRBYTE MsgLog, ...)
{
   va_list fParam;

   if(bTraceNeo)
   {
      va_start(fParam,MsgLog);
      vsprintf(LogBuff,MsgLog,fParam);
      _FileWriteLn(FicLogNeo,LogBuff,strlen(LogBuff));
   }
}
//
// ����������������������������������������������������������������������������
//
// Attend pendant uMs millisecondes.
//
void mDelay(unsigned uMs)
{
   struct dos_time_t HDeb,HFin;
   float dHDeb,dHFin;

   _dos_gettime(&HDeb);
   dHDeb  = TimeToCent(&HDeb)*10;
   while(1)
   {
      _dos_gettime(&HFin);
      dHFin = TimeToCent(&HFin)*10;
      if(dHFin<dHDeb)
         // A cheval sur 2 jours.
         dHFin += (float) 86400000;
      if((dHFin - dHDeb) >= uMs)
         break;
   }
}
//
// ����������������������������������������������������������������������������
//
// D�tection de SHARE.EXE
// Renvoie TRUE si SHARE est install�.
//
mtBOOL IsShareInstalled(void)
{
   union REGS regs;

   regs.x.ax = 0x1000;
   int86(0x2F,&regs,&regs);
   if(regs.h.al == 0xFF)
      return(mtTRUE);
   return(mtFALSE);
}
//
// ����������������������������������������������������������������������������
//

