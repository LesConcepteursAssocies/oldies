// ��������������������������������͵NEOSERV����������������������������������͸
// �MEMOIRE.C    �                                                             �
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
// �Gestion des ressources m�moire                                             �
// ���������������������������������������������������������������������������;

#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <mem.h>
#include "generic.h"

//
// ����������������������������������������������������������������������������
//
extern   jmp_buf   COMPERROR;
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      ExecError(mtPTRBYTE,int,mtPTRDICO,mtPOINTEUR);
extern   mtBOOL    bInCompile;
extern   void      WriteLogNeo(mtPTRBYTE,...);
//
// ����������������������������������������������������������������������������
//
// Allocation d'un bloc m�moire et mise � 0 de celui-ci.
//
mtPOINTEUR MemAlloc(mtUDWORD Len)
{
   mtPOINTEUR NewP=NULL;

   if((size_t) _x386_coreleft() < (size_t) Len)
   {
      if(! bInCompile)
         ExecError("MemAlloc",ERROUTOFMEMORY,NULL,NULL);
      else
      {
         WinInfo("MemAlloc %lu octets: M�moire insuffisante",Len);
         WriteLogNeo("MemAlloc %lu octets: M�moire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   if((NewP = mem_malloc((size_t) Len)) == NULL)
   {
      if(! bInCompile)
         ExecError("MemAlloc",ERROUTOFMEMORY,NULL,NULL);
      else
      {
         WinInfo("MemAlloc %lu octets: M�moire insuffisante",Len);
         WriteLogNeo("MemAlloc %lu octets: M�moire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   else
      memset(NewP,0,Len);
   return(NewP);
}
//
// ����������������������������������������������������������������������������
//
// R�allocation d'un bloc m�moire.
//
mtPOINTEUR MemReAlloc(mtPOINTEUR MemPtr,mtUDWORD Len)
{
   if((size_t) _x386_coreleft() < (size_t) Len)
   {
      if(! bInCompile)
         ExecError("MemReAlloc",ERROUTOFMEMORY,NULL,NULL);
      else
      {
         WinInfo("MemReAlloc %lu octets: M�moire insuffisante",Len);
         WriteLogNeo("MemReAlloc %lu octets: M�moire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   return(mem_realloc(MemPtr,Len));
}
//
// ����������������������������������������������������������������������������
//
// Lib�ration d'un bloc m�moire.
//
void MemFree(mtPOINTEUR MemPtr)
{
   mem_free(MemPtr);
}
//
// ����������������������������������������������������������������������������
//

