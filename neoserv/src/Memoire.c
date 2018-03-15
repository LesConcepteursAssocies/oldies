// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณMEMOIRE.C    ณ                                                             ณ
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
// ณGestion des ressources mmoire                                             ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <setjmp.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <dos.h>
#include <mem.h>
#include "generic.h"

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   jmp_buf   COMPERROR;
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      ExecError(mtPTRBYTE,int,mtPTRDICO,mtPOINTEUR);
extern   mtBOOL    bInCompile;
extern   void      WriteLogNeo(mtPTRBYTE,...);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Allocation d'un bloc mmoire et mise … 0 de celui-ci.
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
         WinInfo("MemAlloc %lu octets: Mmoire insuffisante",Len);
         WriteLogNeo("MemAlloc %lu octets: Mmoire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   if((NewP = mem_malloc((size_t) Len)) == NULL)
   {
      if(! bInCompile)
         ExecError("MemAlloc",ERROUTOFMEMORY,NULL,NULL);
      else
      {
         WinInfo("MemAlloc %lu octets: Mmoire insuffisante",Len);
         WriteLogNeo("MemAlloc %lu octets: Mmoire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   else
      memset(NewP,0,Len);
   return(NewP);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rallocation d'un bloc mmoire.
//
mtPOINTEUR MemReAlloc(mtPOINTEUR MemPtr,mtUDWORD Len)
{
   if((size_t) _x386_coreleft() < (size_t) Len)
   {
      if(! bInCompile)
         ExecError("MemReAlloc",ERROUTOFMEMORY,NULL,NULL);
      else
      {
         WinInfo("MemReAlloc %lu octets: Mmoire insuffisante",Len);
         WriteLogNeo("MemReAlloc %lu octets: Mmoire insuffisante",Len);
         longjmp(COMPERROR,-1);
      }
   }
   return(mem_realloc(MemPtr,Len));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libration d'un bloc mmoire.
//
void MemFree(mtPOINTEUR MemPtr)
{
   mem_free(MemPtr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

