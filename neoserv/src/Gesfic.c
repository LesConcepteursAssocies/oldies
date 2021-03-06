// 嬪様様様様様様冤様様様様様様様様裕NEOSERV突様様様様様様様様様様様様様様様様邑
// �GESFIC.C     �                                                             �
// 団陳陳陳陳陳陳�                                                             �
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
// �Gestion des fichiers                                                       �
// 塒様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <conio.h>
#include <int.h>
#include <io.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "generic.h"

//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       WriteLogNeo(mtPTRBYTE,...);
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Ouvre un fichier.
// Renvoie VRAI si le fichier est ouvert, FAUX sinon.
// wType = Type du fichier (TEXT, BINARY)
// bCreat est VRAI si on autorise la cr�ation du fichier en cas
// de non-ouverture.
//
mtBOOL _FileOpen(mtPTRBYTE pName,mtWORD wType,mtPTRWORD pHandle,mtBOOL bCreat)
{
   int ih;

   *pHandle = sopen(pName,wType | O_RDWR,SH_DENYNO);
   if(*pHandle == -1)
   {
      if(bCreat)
      {
         ih = creat(pName,S_IREAD | S_IWRITE);
         if(ih == -1)
            return(mtFALSE);
         *pHandle = ih;
         return(mtTRUE);
      }
      return(mtFALSE);
   }
   return(mtTRUE);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Ferme le fichier donn�.
//
void _FileClose(mtWORD Handle)
{
   close(Handle);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Renvoie la position du pointeur de fichier
//
mtDWORD _FileGetPos(mtWORD Handle)
{
   return(lseek(Handle,0L,SEEK_CUR));
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Renvoie VRAI si on est � la fin du fichier donn�.
//
mtBOOL _IsEOF(mtWORD Handle)
{
   return((_eof(Handle) ? mtTRUE : mtFALSE));
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Lecture du fichier donn�
//
void _FileRead(mtWORD Handle,mtPTRBYTE pBuff,size_t Len)
{
   int nRead;

   nRead = read(Handle,pBuff,Len);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Lecture d'une ligne de texte dans le fichier donn�.
//
void _FileReadLine(mtWORD Handle,mtPTRBYTE pBuff,size_t Len)
{
   mtDWORD CurrPos;
   int nRead;
   char CRLF[3] = { 0x0D, 0x0A , 0x00};
   mtPTRBYTE pCRLF;

   CurrPos = _FileGetPos(Handle);
   nRead   = read(Handle,pBuff,Len);
   pCRLF   = strstr(pBuff,CRLF);
   if(pCRLF)
   {
      // On va se placer apr�s le CRLF.
      lseek(Handle,(mtDWORD) (CurrPos+(pCRLF-pBuff)+2),SEEK_SET);
      *pCRLF = '\0';
   }
   // Recherche du CTRL+Z.
   pCRLF = strchr(pBuff,0x1A);
   if(pCRLF)
      *pCRLF = '\0';
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Ecriture dans le fichier donn�.
//
void _FileWrite(mtWORD Handle,mtPTRBYTE pBuff,size_t Len)
{
   int nWrite;

   nWrite = write(Handle,pBuff,Len);
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// Ecriture dans le fichier donn�, avec un CRLF.
//
void _FileWriteLn(mtWORD Handle,mtPTRBYTE pBuff,size_t Len)
{
   int nWrite;
   char CRLF[3] = { 0x0D, 0x0A , 0x00};

   nWrite = write(Handle,pBuff,Len);
   nWrite = write(Handle,CRLF,strlen(CRLF));
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//
// R�cup�ration du FILES du config.sys
//
void InitNbFiles(void)
{
   int nbFic=0,err;
   mtWORD hConf;
   char BufTmp[129], *pTmp;
   char *pCfg,*pEG;

   pCfg = searchpath("C:\\CONFIG.SYS");
   if(pCfg)
   {
      if(_FileOpen(pCfg,O_BINARY,&hConf,mtFALSE))
      {
         while(! _IsEOF(hConf))
         {
            pTmp = BufTmp;
            memset(pTmp,0,129);
            _FileReadLine(hConf,pTmp,128);
            if(strlen(pTmp) <=1)
               continue;
            LTrim(pTmp);
            if(strlen(pTmp) <=1)
               continue;
            strupr(pTmp);
            if(strncmp(pTmp,"FILES",5) == 0)
            {
               // Ligne FILES=... trouv�e.
               pEG = strchr(pTmp,'=');
               if(pEG)
               {
                  // On passe le '='.
                  pEG++;
                  LTrim(pEG);
                  nbFic = atoi(pEG);
                  break;
               }
            }
         }
         _FileClose(hConf);
         if(nbFic)
         {
            err = _dos_sethandlecount(nbFic);
            if(err == 0)
               WinInfo("FILES=%d",nbFic);
            else
            {
               WinInfo("ERREUR DosSetHandleCount %d",err);
               WriteLogNeo("ERREUR DosSetHandleCount %d",err);
            }
         }
         else
         {
            WinInfo("FILES=... NON TROUVE DANS %s",pCfg);
            WriteLogNeo("FILES=... NON TROUVE DANS %s",pCfg);
         }
      }
      else
      {
         WinInfo("IMPOSSIBLE D'OUVRIR %s",pCfg);
         WriteLogNeo("IMPOSSIBLE D'OUVRIR %s",pCfg);
      }
   }
   else
   {
      WinInfo("C:\\CONFIG.SYS NON TROUVE");
      WriteLogNeo("C:\\CONFIG.SYS NON TROUVE");
   }
}
//
// 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様
//

