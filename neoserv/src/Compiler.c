// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณCOMPILER.C   ณ                                                             ณ
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
// ณDernire modification : 21/03/1996                                         ณ
// ณ                                                                           ณ
// ณCompilation/Excution d'une application                                    ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <setjmp.h>
#include <stdio.h>
#include "toolkit.h"
#include "filespec.h"
#include <string.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <fltpnt.h>
#include <float.h>
#include <direct.h>
#include <ctype.h>
#include <limits.h>
#include <fcntl.h>
#include "d4all.h"
#pragma SC align
#include "minitel.h"
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"

mtPTRAPP      pAppli;                  // Pointe sur la 1re application.
pPDICO        pGlobalDic;              // Pointe sur le dictionnaire global du systme.
                                       // (celui qui contient les instructions
                                       //  du langage, ainsi que les symboles et
                                       //  procdures publiques).
mtUWORD       uGDico;                  // Nombre de symboles dans pGlobalDic.
//
// Le contexte d'un fichier programme (application et/ou librairie).
//
typedef struct _CtxFic
{
   mtWORD     hFic;
   mtUDWORD   NoLig;
   char       Ligne[MAXLLIG+1];        // Ligne en cours de compilation.
   char       *pLig;                   //             " "
   char       LigAff[MAXLLIG+1];       // Copie de la ligne pour affichage sur erreur.
   char      *pLAff;
   mtBYTE     NApp[11];                // Nom de l'application/librairie compile.
   mtPTRAPP   pCApp;                   // Pointe sur l'appli. en cours de compilation.
   mtUWORD    wHERE;                   // Indice du 1er emplacement libre dans le bloc
                                       // de code de l'appli. courante.
   mtBOOL     bIn2Points;              // VRAI si on est en cours de dfinition
                                       // de procdure.
   mtBOOL     bInMain;                 // VRAI si on est en train de dfinir la
                                       // procdure principale.
   mtBOOL     bOnLib;                  // VRAI si on est en train de dfinir la
                                       // procdure de fin.
   mtBOOL     bGTask;                  // VRAI si on est en train de dfinir la
                                       // tche globale.
   mtBOOL     bMainDef;                // VRAI si l'appli en cours de compilation
                                       // contient une procdure MAIN.
   mtBOOL     bPUBLICDEF;              // Drapeau dfinition publique.
   mtBOOL     bLOCALDEF;               // Drapeau dfinition locale.
   mtBOOL     bEXTERNDEF;              // Drapeau dfinition externe.
   mtBOOL     bFinLigne;               // VRAI si la compilation de la ligne doit s'arreter.
   mtBOOL     bWarning;                // VRAI si on affiche les rf. non utilises.
   mtBOOL     bEcho;                   // VRAI si on affiche le source en cours de compilation.
   mtPTRWORD  TabMark;                 // Contient les IP des rfrences des
                                       // sauts (utilis par les structures
                                       // conditionnelles).
   mtUWORD    pTMark;                  // Indice dans TabMark.
   //
   // Quelques contr“les de compilation.
   //
   mtWORD     NbDO;                    // Nombre de DO.
   mtWORD     NbIF;                    // Nombre de IF.
   mtWORD     NbBEGIN;                 // Nombre de BEGIN.
   mtWORD     NbWHILE;                 // Nombre de WHILE.
   mtWORD     NbCASE;                  // Nombre de CASE.
   mtWORD     NbOF;                    // Nombre de OF.
} CtxFic;
typedef CtxFic *mtPTRCFIC;

pPOINTEUR     PileFIC;                 // Pile des fichiers … compiler.
mtUWORD       SPFIC=0;                 // Pointeur de la pile des fichiers.
mtPTRCFIC     pCFIC;                   // Pointe sur le fichier en cours de compilation.

mtUWORD       HMark = UINT_MAX;        // Marqueur de fin dans TabMark.
char          cDelim[] = " \t";        // Les sparateurs d'instructions.
jmp_buf       COMPERROR;               // Retour sur erreur de compilation.
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   mtBOOL     _FileOpen(mtPTRBYTE,mtWORD,mtPTRWORD,mtBOOL);
extern   void       _FileClose(mtWORD);
extern   mtBOOL     _IsEOF(mtWORD);
extern   void       _FileRead(mtWORD,mtPTRBYTE,size_t);
extern   void       _FileReadLine(mtWORD,mtPTRBYTE,size_t);
extern   void       _FileWrite(mtWORD,mtPTRBYTE,size_t);
extern   char       *PathAPPS;
extern   char       *ExtPrg;
extern   mtBOOL     bAutoCNX;
extern   char       *pAppAutoCNX;
extern   char       *NomAppAutoCNX;
extern   mtPTRCV    pCurrCV;           // Pointe sur le cv courant.
extern   mtPTRCV    pCurrLT;           // Pointe sur la tche locale du cv courant.
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   mtBOOL     bNextStep;
extern   mtBOOL     bAffTrace;
extern   mtWORD     wPasTrace;
extern   mtWORD     wNbInst;
extern   void       WinAffTraceCv(void);
extern   void       MemFree(mtPOINTEUR);
extern   void       DelStApp(mtPTRAPP);
extern   void       WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       CopyValue(mtPTRDICO,mtPTRDICO);
extern   void       DelLstCv(mtPTRLSTCV);
extern   float      TimeToCent(struct dos_time_t *);
extern   char       TLoadFic[MAXFICLOAD][80];
extern   mtUWORD    iLoad;
extern   void       SauveAppStat(mtPTRAPP);
extern   void       RecupAppStat(mtPTRAPP);
extern   void       SauveAppSysInfo(mtPTRAPP);
extern   void       RecupAppSysInfo(mtPTRAPP);
extern   void       WriteLogNeo(mtPTRBYTE,...);
extern   mtBOOL     bInCompile;
//
// Les primitives du KERNEL.
//
#include "declkrnl.h"
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
mtPTRDICO     NewDicEntry(pPOINTEUR,mtPTRUWORD,mtPTRBYTE,mtBYTE,mtBOOL);
void          DelDico(pPDICO,mtUWORD);
mtPTRDICO     SearchDico(pPDICO,mtUWORD,mtPTRBYTE,mtPTRUWORD);
mtPTRTFKEY    NewFKey(mtPTRTFKEY,mtBOOL,mtBOOL,mtBYTE,mtPTRDICO);
mtPTRTFKEY    SearchTFKEY(mtPTRTFKEY,mtBYTE,mtBOOL,mtBOOL);
void          DelTFKEY(mtPTRTFKEY);
mtPTRTKEYW    NewKeyW(mtPTRTKEYW,mtPTRBYTE,mtPTRDICO);
mtPTRTKEYW    SearchTKEYW(mtPTRTKEYW,mtPTRBYTE);
void          DelTKEYW(mtPTRTKEYW);
mtPTRDICO     SearchSymb(mtPTRBYTE,mtPTRWORD,mtPTRUWORD);
mtPTRLISTDICO NewListDico(pPOINTEUR,mtPTRUWORD,mtPTRAPP);
void          DelListDico(pPLISTDICO,mtUWORD);
mtPTRLISTDICO SearchListDico(pPLISTDICO,mtUWORD,mtPTRAPP);
mtPTRAPP      NewApp(mtPTRBYTE,mtBYTE);
void          DelApp(mtPTRBYTE);
mtPTRAPP      SearchApp(mtPTRBYTE,mtBYTE);
mtPTRAPP      SearchAppFic(mtPTRBYTE);
mtBOOL        LoadApp(mtPTRBYTE);
mtBOOL        CompBEGDEF(void);
mtBOOL        CompENDDEF(void);
mtBOOL        CompSTRING(void);
mtBOOL        CompPUBDEF(void);
mtBOOL        CompLOCDEF(void);
mtBOOL        CompUSE(void);
mtBOOL        CompCHAR(void);
mtBOOL        CompNUMBER(void);
mtBOOL        CompTABCHAR(void);
mtBOOL        CompTABNUMBER(void);
mtBOOL        CompIF(void);
mtBOOL        CompELSE(void);
mtBOOL        CompENDIF(void);
mtBOOL        CompDO(void);
mtBOOL        CompLOOP(void);
mtBOOL        CompPLOOP(void);
mtBOOL        CompLOOPI(void);
mtBOOL        CompLOOPJ(void);
mtBOOL        CompLOOPK(void);
mtBOOL        CompBEGIN(void);
mtBOOL        CompAGAIN(void);
mtBOOL        CompUNTIL(void);
mtBOOL        CompWHILE(void);
mtBOOL        CompREPEAT(void);
mtBOOL        CompCASE(void);
mtBOOL        CompOF(void);
mtBOOL        CompENDOF(void);
mtBOOL        CompENDCASE(void);
mtBOOL        CompGETCPOS(void);
mtBOOL        CompASGFKEY(void);
mtBOOL        CompASGKEYW(void);
mtBOOL        CompASLFKEY(void);
mtBOOL        CompASLKEYW(void);
mtBOOL        CompDEGFKEY(void);
mtBOOL        CompDELFKEY(void);
mtBOOL        CompDEGKEYW(void);
mtBOOL        CompDELKEYW(void);
mtBOOL        CompPATH(void);
mtBOOL        CompPATHDBF(void);
mtBOOL        CompPATHVTX(void);
mtBOOL        CompSTKSIZE(void);
mtBOOL        CompAPPNAME(void);
mtBOOL        CompNUMCOMP(void);
mtBOOL        CompDELAY(void);
mtBOOL        CompAFILL(void);
mtBOOL        CompASEARCH(void);
mtBOOL        CompAINS(void);
mtBOOL        CompADEL(void);
mtBOOL        CompACOPY(void);
mtBOOL        CompACLONE(void);
mtBOOL        CompSTRTOK(void);
mtBOOL        CompADIR(void);
mtBOOL        CompLIBERR(void);
mtBOOL        CompCONFCNX(void);
mtBOOL        CompINPUTVAR(void);
mtBOOL        CompMLINPUT(void);
mtBOOL        CompSETLTASK(void);
mtBOOL        CompWARNING(void);
mtBOOL        CompSETECHO(void);
mtBOOL        CompSETACMIT(void);
mtBOOL        CompANTICIPE(void);
mtBOOL        CompWAITCHAR(void);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                         Les primitives de compilation.
//                         ออออออออออออออออออออออออออออออ
//
static mtPTRBYTE CompileDef[] =
{
   ":",                                // Dbut de procdure.
   ";",                                // Fin de procdure.
   "\x22",                             // Chaine littrale " ...".
   "PUBLIC",                           // Symbole public.
   "LOCAL",                            // Symbole local.
   "#USE",                             // Utilisation de librairies externes.
   "CHAR",                             // Type caractre.
   "NUMBER",                           // Type numrique.
   "CHAR[]",                           // Type tableau de caractre.
   "NUMBER[]",                         // Type tableau numrique.
   "IF",                               // Dbut de conditionnelle IF ... ENDIF.
   "ELSE",                             // Alternative du IF ... ELSE ... ENDIF.
   "ENDIF",                            // Fin de conditionnelle IF ... ENDIF.
   "DO",                               // Dbut de boucle DO ... LOOP.
   "LOOP",                             // Fin de boucle DO ... LOOP.
   "+LOOP",                            // Fin de boucle DO ... +LOOP.
   "I",                                // Indice de boucle niveau 1.
   "J",                                // Indice de boucle niveau 2.
   "K",                                // Indice de boucle niveau 3.
   "BEGIN",                            // Dbut de boucle conditionnelle.
   "AGAIN",                            // Fin de boucle infinie BEGIN ... AGAIN.
   "UNTIL",                            // Fin de boucle conditionnelle BEGIN ... UNTIL.
   "WHILE",                            // Alternative de boucle BEGIN ... WHILE ... REPEAT.
   "REPEAT",                           // Fin de boucle BEGIN ... WHILE ... REPEAT.
   "CASE",                             // Dbut de slection multiple CASE ... OF ... ENDOF ... ENDCASE.
   "OF",                               // Dbut de slection OF ... ENDOF.
   "ENDOF",                            // Fin de slection multiple OF ... ENDOF.
   "ENDCASE",                          // Fin de slection multiple CASE ... ENDCASE.
   "GETCURSORPOS",                     // Demande de position du curseur.
   "SETGLOBALFKEY",                    // Assignation d'une touche de fonction globale.
   "SETGLOBALKEYWORD",                 // Assignation d'un mot-clef global.
   "SETLOCALFKEY",                     // Assignation d'une touche de fonction locale.
   "SETLOCALKEYWORD",                  // Assignation d'un mot-clef locale.
   "#PATH",                            // Assignation du rpertoire principal de l'appli.
   "#PATHDBF",                         // Assignation du rpertoire des bases de donnes de l'appli.
   "#PATHVTX",                         // Assignation du rpertoire des pages de l'appli.
   "#STACKSIZE",                       // Paramtrage de la taille des piles.
   "#NAME",                            // Assignation d'un nom … l'appli.
   "#NUMCOMP",                         // Dfinition du numro complmentaire.
   "DELAY",                            // Fonction DELAY.
   "AFILL",                            // Fonction AFILL.
   "ASEARCH",                          // Fonction ASEARCH.
   "AINS",                             // Fonction AINS.
   "ADEL",                             // Fonction ADEL.
   "ACOPY",                            // Fonction ACOPY.
   "ACLONE",                           // Fonction ACLONE.
   "STRTOK",                           // Fonction STRTOK.
   "#LIBONERROR",                      // Paramtrage de la dconnexion sur erreur.
   "#CONFCNX",                         // Paramtrage de la confirmation d'appels X25.
   "INPUTVAR",                         // Saisie dans une variable.
   "DELETEGLOBALFKEY",                 // Suppression d'une assignation globale de touche.
   "DELETELOCALFKEY",                  // Suppression d'une assignation locale de touche.
   "DELETEGLOBALKEYWORD",              // Suppression d'une assignation globale de mot-clef.
   "DELETELOCALKEYWORD",               // Suppression d'une assignation locale de mot-clef.
   "SETLOCALTASK",                     // Dfinition d'une tche locale.
   "ADIR",                             // Fonction ADIR.
   "#WARNING",                         // Affichage ou non des rf. croises.
   "#ECHO",                            // Affichage ou non des lignes du source courant.
   "#DBAUTOCOMMIT",                    // Paramtrage du raffraichissement automatique des bases.
   "MLINPUT",                          // Saisie multilignes.
   "#ANTICIPATION",                    // Paramtrage de l'anticipation.
   "WAITCHAR",                         // Attente d'un caractre sur le flux.
   NULL
};
//
// Les directives associes.
//
static mtBOOL (*FctCompile[]) (void) =
{
   CompBEGDEF,
   CompENDDEF,
   CompSTRING,
   CompPUBDEF,
   CompLOCDEF,
   CompUSE,
   CompCHAR,
   CompNUMBER,
   CompTABCHAR,
   CompTABNUMBER,
   CompIF,
   CompELSE,
   CompENDIF,
   CompDO,
   CompLOOP,
   CompPLOOP,
   CompLOOPI,
   CompLOOPJ,
   CompLOOPK,
   CompBEGIN,
   CompAGAIN,
   CompUNTIL,
   CompWHILE,
   CompREPEAT,
   CompCASE,
   CompOF,
   CompENDOF,
   CompENDCASE,
   CompGETCPOS,
   CompASGFKEY,
   CompASGKEYW,
   CompASLFKEY,
   CompASLKEYW,
   CompPATH,
   CompPATHDBF,
   CompPATHVTX,
   CompSTKSIZE,
   CompAPPNAME,
   CompNUMCOMP,
   CompDELAY,
   CompAFILL,
   CompASEARCH,
   CompAINS,
   CompADEL,
   CompACOPY,
   CompACLONE,
   CompSTRTOK,
   CompLIBERR,
   CompCONFCNX,
   CompINPUTVAR,
   CompDEGFKEY,
   CompDELFKEY,
   CompDEGKEYW,
   CompDELKEYW,
   CompSETLTASK,
   CompADIR,
   CompWARNING,
   CompSETECHO,
   CompSETACMIT,
   CompMLINPUT,
   CompANTICIPE,
   CompWAITCHAR
};
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recalcul des adresses des procdures de l'application courante.
//
void ReCalcAdr(ppFUNC pOld,ppFUNC pNew)
{
   pPDICO pTDic=pCFIC->pCApp->pPDico;
   mtPTRDICO pSDic;
   mtUWORD wHTmp=0,wIDic=pCFIC->pCApp->uPDico;
   pFUNC pAdr;

   //
   // On scanne d'abord les procdures de l'application.
   //
   while(wIDic)
   {
      wIDic--;
      pSDic = pTDic[wIDic];
      if(pSDic->Type == mt2POINTS)
         pSDic->V.pVal = pNew;
   }
   //
   // Ensuite, on scanne le bloc de code, et on remplace
   // pOld par pNew.
   //
   while(wHTmp < pCFIC->wHERE)
   {
      pAdr = (pFUNC) *(pNew+wHTmp);
      if(pAdr == (pFUNC) pOld)
         *(pNew+wHTmp) = (pFUNC) pNew;
      wHTmp++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Vrifie que le bloc de code de l'application courante est assez grand.
// Ajoute ventuellement un bloc supplmentaire si ncssaire.
//
void _CheckBlockSize(void)
{
   ppFUNC OldHere;

   if((pCFIC->wHERE % BLOCKSIZE) == 0)
   {
      OldHere = pCFIC->pCApp->pHere;
      // On est … la fin d'un bloc de code,
      // il faut en ajouter un … la suite.
      pCFIC->pCApp->NbBlock++;
      pCFIC->pCApp->pHere = (ppFUNC) MemReAlloc(pCFIC->pCApp->pHere,(pCFIC->pCApp->NbBlock*BLOCKSIZE)*sizeof(pFUNC));
      if(pCFIC->pCApp->pHere != OldHere)
      {
         // L'adresse du bloc a change.
         // Il faut recalculer les rfrences aux fonctions du programme.
         ReCalcAdr(OldHere,pCFIC->pCApp->pHere);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le pointeur sur la structure CtxFic dans la pile des fichiers.
//
void PushCFic(mtPOINTEUR pFic)
{
   if(PileFIC)
      PileFIC = (pPOINTEUR) MemReAlloc(PileFIC,(SPFIC+1)*sizeof(mtPOINTEUR));
   else
      PileFIC = (pPOINTEUR) MemAlloc((SPFIC+1)*sizeof(mtPOINTEUR));
   PileFIC[SPFIC] = pFic;
   SPFIC++;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile un fichier.
//
mtPOINTEUR PopCFic(void)
{
   static mtPOINTEUR ptr = NULL;

   if(PileFIC)
   {
      SPFIC--;
      ptr = PileFIC[SPFIC];
      if(SPFIC)
         PileFIC = (pPOINTEUR) MemReAlloc(PileFIC,(SPFIC+1)*sizeof(mtPOINTEUR));
      else
      {
         MemFree(PileFIC);
         PileFIC = NULL;
      }
   }
   return(ptr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une nouvelle entre au dictionnaire donn.
// Renvoie un pointeur sur la nouvelle entre cre.
// Si bH est FAUX, NomSym n'est pas copi dans l'entre.
//
mtPTRDICO NewDicEntry(pPOINTEUR LeDico,mtPTRUWORD pIDic,mtPTRBYTE NomSym,mtBYTE cType,mtBOOL bH)
{
   pPDICO pDic;
   mtPTRDICO NewD;
   mtUWORD wDic;

   NewD = (mtPTRDICO) MemAlloc(sizeof(Dico));
   if(bH)
   {
      NewD->Nom = (mtPTRBYTE) MemAlloc(strlen(NomSym)+1);
      strcpy(NewD->Nom,NomSym);
   }
   NewD->Type = cType;
   // Ajout dans le tableau des symboles donn.
   pDic = (pPDICO) *LeDico;
   wDic = *pIDic;
   pDic       = (pPDICO) MemReAlloc(pDic,((wDic+1)*sizeof(mtPTRDICO)));
   pDic[wDic] = NewD;
   wDic++;
   *LeDico = (mtPOINTEUR) pDic;
   *pIDic  = wDic;
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression du contenu du symbole donn.
//
void DelSymValue(mtPTRDICO pSym)
{
   mtUWORD Li,Co;
   mtPTRITAB pITmp;
   mtPOINTEUR ptr;
   pPOINTEUR pTmp;

   if(pSym)
   {
      MemFree(pSym->Nom);
      switch(pSym->Type)
      {
         case mtTCHAR:
         {
            MemFree(pSym->V.cVal);
            break;
         }
         case mtTABCHAR:
         {
            if((pSym->XVal) && (pSym->V.tVal))
            {
               pITmp = (mtPTRITAB) pSym->XVal;
               pTmp  = pSym->V.tVal;
               for(Li=0;Li<pITmp->MaxLig;Li++)
               {
                  for(Co=0;Co<pITmp->MaxCol;Co++)
                  {
                     ptr = *(pTmp+((Li*pITmp->MaxCol)+Co));
                     MemFree(ptr);
                  }
               }
            }
            MemFree(pSym->V.tVal);
            MemFree(pSym->XVal);
            break;
         }
         case mtTABNUMBER:
         {
            MemFree(pSym->V.tfVal);
            MemFree(pSym->XVal);
            break;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'un dictionnaire.
//
void DelDico(pPDICO LeDico,mtUWORD IDic)
{
   mtPTRDICO p;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      p = LeDico[wIDic];
      DelSymValue(p);
      MemFree(p);
      wIDic++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une entre de dictionnaire.
// Renvoie un pointeur sur l'entre trouve et sa position dans pPos,ou NULL.
//
mtPTRDICO SearchDico(pPDICO LeDico,mtUWORD IDic,mtPTRBYTE NSym,mtPTRUWORD pPos)
{
   mtPTRDICO d,SDic=(mtPTRDICO) 0;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      d = LeDico[wIDic];
      if(strcmp(d->Nom,NSym) == 0)
      {
         *pPos = wIDic;
         return(d);
      }
      wIDic++;
   }
   return(SDic);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une nouvelle dfinition de touche … la table donne.
// Renvoie un pointeur sur la nouvelle entre.
//
mtPTRTFKEY NewFKey(mtPTRTFKEY LaTable,mtBOOL bD,mtBOOL bE,mtBYTE TFonc,mtPTRDICO Proc)
{
   mtPTRTFKEY NewK,k;

   NewK = (mtPTRTFKEY) MemAlloc(sizeof(TabFKey));
   NewK->bDiese    = bD;
   NewK->bEtoile   = bE;
   NewK->TFonction = TFonc;
   NewK->HProc     = (ppFUNC) Proc->V.pVal;
   NewK->IPProc    = (mtUWORD) Proc->XVal;
   if(! LaTable)
      // 1re entre
      LaTable = NewK;
   else
   {
      // Insertion.
      k = LaTable;
      while(k->pNextTFKEY)
         k = k->pNextTFKEY;            // On se place … la fin.
      k->pNextTFKEY = NewK;
   }
   return(NewK);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un touche de fonction dans la table donne.
// Renvoie un pointeur sur la touche ou NULL sinon.
//
mtPTRTFKEY SearchTFKEY(mtPTRTFKEY LaTable,mtBYTE TFonc,mtBOOL bDi,mtBOOL bEt)
{
   mtPTRTFKEY k,SKey=(mtPTRTFKEY) 0;

   k = LaTable;
   while(k)
   {
      if((k->TFonction == TFonc) && (k->bDiese == bDi) && (k->bEtoile == bEt))
         return(k);
      k = k->pNextTFKEY;
   }
   return(SKey);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime une table d'interception de touches de fonctions.
//
void DelTFKEY(mtPTRTFKEY LaTable)
{
   mtPTRTFKEY k,kNext;

   k = LaTable;
   while(k)
   {
      kNext = k->pNextTFKEY;
      MemFree(k);
      k = kNext;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute un nouveau mot-clef … la table donne.
// Renvoie un pointeur sur la nouvelle entre.
//
mtPTRTKEYW NewKeyW(mtPTRTKEYW LaTable,mtPTRBYTE cKeyW,mtPTRDICO Proc)
{
   mtPTRTKEYW NewK,k;

   NewK            = (mtPTRTKEYW) MemAlloc(sizeof(TabKeyW));
   NewK->KeyWord   = (mtPTRBYTE) MemAlloc(strlen(cKeyW)+1);
   strcpy(NewK->KeyWord,cKeyW);
   NewK->HProc     = (ppFUNC) Proc->V.pVal;
   NewK->IPProc    = (mtUWORD) Proc->XVal;
   if(! LaTable)
      // 1re entre
      LaTable = NewK;
   else
   {
      // Insertion.
      k = LaTable;
      while(k->pNextTKEYW)
         k = k->pNextTKEYW;            // On se place … la fin.
      k->pNextTKEYW = NewK;
   }
   return(NewK);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un mot-clef dans la table donne.
// Renvoie un pointeur sur l'entre ou NULL sinon.
//
mtPTRTKEYW SearchTKEYW(mtPTRTKEYW LaTable,mtPTRBYTE cKeyW)
{
   mtPTRTKEYW k,SKey=(mtPTRTKEYW) 0;

   k = LaTable;
   while(k)
   {
      if(strcmp(k->KeyWord,cKeyW) == 0)
         return(k);
      k = k->pNextTKEYW;
   }
   return(SKey);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime une table d'interception de mot-clef.
//
void DelTKEYW(mtPTRTKEYW LaTable)
{
   mtPTRTKEYW k,kNext;

   k = LaTable;
   while(k)
   {
      kNext = k->pNextTKEYW;
      MemFree(k->KeyWord);
      MemFree(k);
      k = kNext;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un symbole.
// Balaye les dictionnaires conerns.
// Renvoie un pointeur sur l'entre trouve et la position du symbole, ou NULL.
//
mtPTRDICO SearchSymb(mtPTRBYTE NSym,mtPTRWORD pPortee,mtPTRUWORD pPos)
{
   mtPTRDICO SSym=(mtPTRDICO) 0;

   *pPortee = 0;
   SSym     = SearchDico(pCFIC->pCApp->pLDico,pCFIC->pCApp->uLDico,NSym,pPos);
   if(! SSym)
   {
      SSym = SearchDico(pCFIC->pCApp->pPDico,pCFIC->pCApp->uPDico,NSym,pPos);
      if(! SSym)
      {
         SSym = SearchDico(pGlobalDic,uGDico,NSym,pPos);
         if(SSym)
            *pPortee = PGLOBAL;
      }
      else
         *pPortee = PPUBLIC;
   }
   else
      *pPortee = PLOCAL;
   return(SSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnration d'un symbole dans le dico interne de l'application courante.
// Renvoie un pointeur sur le nouveau symbole.
//
mtPTRDICO GenAppISym(mtBYTE pTyp,mtBYTE cAttr)
{
   mtPTRDICO NewD;

   NewD       = NewDicEntry(&pCFIC->pCApp->pIDico,&pCFIC->pCApp->uIDico,NULL,pTyp,mtFALSE);
   NewD->Attr = cAttr;
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la valeur donne dans pCFIC->TabMark.
//
void PushMark(mtUWORD wMark)
{
   if(pCFIC->TabMark)
      pCFIC->TabMark = (mtPTRWORD) MemReAlloc(pCFIC->TabMark,(pCFIC->pTMark+1)*sizeof(mtPTRWORD));
   else
      pCFIC->TabMark = (mtPTRWORD) MemAlloc((pCFIC->pTMark+1)*sizeof(mtPTRWORD));
   pCFIC->TabMark[pCFIC->pTMark] = wMark;
   pCFIC->pTMark++;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile la valeur situe au sommet de pCFIC->TabMark
//
mtUWORD PopMark(void)
{
   mtUWORD wTmp = UINT_MAX;

   if(pCFIC->TabMark)
   {
      pCFIC->pTMark--;
      wTmp = pCFIC->TabMark[pCFIC->pTMark];
      if(pCFIC->pTMark)
         pCFIC->TabMark = (mtPTRWORD) MemReAlloc(pCFIC->TabMark,(pCFIC->pTMark+1)*sizeof(mtPTRWORD));
      else
      {
         MemFree(pCFIC->TabMark);
         pCFIC->TabMark = NULL;
      }
   }
   return(wTmp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une nouvelle entre … la liste des dictionnaires donnes.
// Renvoie un pointeur sur la nouvelle entre cre.
//
mtPTRLISTDICO NewListDico(pPOINTEUR LeEDico,mtPTRUWORD pIDic,mtPTRAPP pApp)
{
   pPLISTDICO pDic;
   mtPTRLISTDICO NewD;
   mtUWORD wDic;

   NewD         = (mtPTRLISTDICO) MemAlloc(sizeof(ListDico));
   NewD->pApp   = (mtPOINTEUR) pApp;
   // Ajout dans le tableau des dictionnaires donn.
   pDic = (pPLISTDICO) *LeEDico;
   wDic = *pIDic;
   pDic = (pPLISTDICO) MemReAlloc(pDic,((wDic+1)*sizeof(mtPTRLISTDICO)));
   pDic[wDic] = NewD;
   wDic++;
   *LeEDico   = (mtPOINTEUR) pDic;
   *pIDic     = wDic;
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une liste de dictionnaires.
//
void DelListDico(pPLISTDICO LeEDico,mtUWORD IDic)
{
   mtPTRLISTDICO p;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      p = LeEDico[wIDic];
      DelDico(p->pLDico,p->uLDico);
      MemFree(p->pLDico);
      MemFree(p);
      wIDic++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une entre dans la liste des dictionnaires donnes.
// Renvoie un pointeur sur le dictionnaire de l'application donne,
// ou NULL si non trouv.
//
mtPTRLISTDICO SearchListDico(pPLISTDICO LeEDico,mtUWORD IDic,mtPTRAPP pApp)
{
   mtPTRLISTDICO d,SDic=(mtPTRLISTDICO) 0;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      d = LeEDico[wIDic];
      if((mtPTRAPP) d->pApp == pApp)
         return(d);
      wIDic++;
   }
   return(SDic);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une nouvelle entre … la liste des rfrences croises d'une appli.
// Renvoie un pointeur sur la nouvelle entre cre.
//
mtPTRREFAPP NewRefApp(mtPTRREFAPP LstRef,mtPTRAPP pApp)
{
   mtPTRREFAPP NewR,r;

   NewR       = (mtPTRREFAPP) MemAlloc(sizeof(RefApp));
   NewR->pApp = (mtPOINTEUR) pApp;
   if(! LstRef)
      // 1re entre
      LstRef = NewR;
   else
   {
      // Insertion.
      r = LstRef;
      while(r->pNext)
         r = r->pNext;                 // On se place … la fin de la liste.
      r->pNext = NewR;
   }
   return(NewR);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une liste de rfrence croise d'application.
//
void DelRefApp(mtPTRREFAPP LstRef)
{
   mtPTRREFAPP r,pRNext;

   r = LstRef;
   while(r)
   {
      pRNext = r->pNext;
      MemFree(r);
      r = pRNext;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une entre de la liste de rfrence croise d'application.
// Supprime de la liste donne la rfrence … l'application donne.
//
void DelUneRefApp(pREFAPP LstRef,mtPTRAPP pApp)
{
   mtPTRREFAPP r,pRNext;

   r = *LstRef;
   if((mtPTRAPP) r->pApp == pApp)
   {
      // On veut supprimer la 1re entre.
      pRNext = r->pNext;
      MemFree(r);
      *LstRef = pRNext;
   }
   else
   {
      while(r->pNext)
      {
         if((mtPTRAPP) r->pNext->pApp == pApp)
         {
            pRNext   = r->pNext->pNext;
            MemFree(r->pNext);
            r->pNext = pRNext;
            break;
         }
         r = r->pNext;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une entre dans la liste des rfrences croises d'une
// application.
// Renvoie un pointeur sur l'entre trouve, ou NULL sinon.
//
mtPTRREFAPP SearchRefApp(mtPTRREFAPP LstRef,mtPTRAPP pApp)
{
   mtPTRREFAPP r,SRef=(mtPTRREFAPP) 0;

   r = LstRef;
   while(r)
   {
      if((mtPTRAPP) r->pApp == pApp)
         return(r);
      r = r->pNext;
   }
   return(SRef);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime toutes les rfrences … l'appli donne dans
// les applications prsentes.
//
void DelAllRefApp(mtPTRAPP pApp)
{
   mtPTRAPP p;

   p = pAppli;
   while(p)
   {
      if(SearchRefApp(p->pLRefApp,pApp))
         DelUneRefApp(&p->pLRefApp,pApp);
      p = p->pNextAPP;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cration d'une nouvelle application, et
// insertion de celle-ci dans la liste.
// Renvoie un pointeur sur le bloc de dfinition cr.
//
mtPTRAPP NewApp(mtPTRBYTE NomApp,mtBYTE NComp)
{
   mtPTRAPP NewP,p;

   NewP = (mtPTRAPP) MemAlloc(sizeof(DefApp));
   strncpy(NewP->Nom,NomApp,10);
   if(NComp)
      NewP->NumComp = NComp;
   NewP->Active = mtFALSE;
   // Allocattion du cv de la tche globale.
   NewP->pGTCv  = MemAlloc(sizeof(UnCv));
   if(! pAppli)
      // C'est la 1re
      pAppli = NewP;
   else
   {
      // Insertion.
      p = pAppli;
      while(p->pNextAPP)
         p = p->pNextAPP;              // On se place … la fin de la liste.
      p->pNextAPP = NewP;
   }
   return(NewP);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une application.
// La liste est mise … jour.
//
void DelApp(mtPTRBYTE NomApp)
{
   mtPTRAPP p,pNext,pSup;

   if(pAppli)
   {
      if(strcmp(pAppli->Nom,NomApp) == 0)
      {
         // On veut supprimer la 1re
         pNext  = pAppli->pNextAPP;
         pSup   = pAppli;
         MemFree(pAppli->Path);
         MemFree(pAppli->PathDBF);
         MemFree(pAppli->PathVTX);
         DelDico(pAppli->pPDico,pAppli->uPDico);
         MemFree(pAppli->pPDico);
         DelDico(pAppli->pLDico,pAppli->uLDico);
         MemFree(pAppli->pLDico);
         DelListDico(pAppli->pEDico,pAppli->uEDico);
         MemFree(pAppli->pEDico);
         DelDico(pAppli->pIDico,pAppli->uIDico);
         MemFree(pAppli->pIDico);
         DelLstCv(pAppli->pLstCnx);
         MemFree(pAppli->pHere);
         MemFree(pAppli->pGTCv);
         DelStApp(pAppli);
         DelTFKEY(pAppli->pGTabFKey);
         DelTKEYW(pAppli->pGTabKeyW);
         MemFree(pAppli);
         pAppli = pNext;
         DelAllRefApp(pSup);
      }
      else
      {
         p = pAppli;
         while(p->pNextAPP)
         {
            if(strcmp(p->pNextAPP->Nom,NomApp) == 0)
            {
               pNext = p->pNextAPP->pNextAPP;
               pSup  = p->pNextAPP;
               MemFree(p->pNextAPP->Path);
               MemFree(p->pNextAPP->PathDBF);
               MemFree(p->pNextAPP->PathVTX);
               DelDico(p->pNextAPP->pPDico,p->pNextAPP->uPDico);
               MemFree(p->pNextAPP->pPDico);
               DelDico(p->pNextAPP->pLDico,p->pNextAPP->uLDico);
               MemFree(p->pNextAPP->pLDico);
               DelListDico(p->pNextAPP->pEDico,p->pNextAPP->uEDico);
               MemFree(p->pNextAPP->pEDico);
               DelDico(p->pNextAPP->pIDico,p->pNextAPP->uIDico);
               MemFree(p->pNextAPP->pIDico);
               DelLstCv(p->pNextAPP->pLstCnx);
               MemFree(p->pNextAPP->pHere);
               MemFree(p->pNextAPP->pGTCv);
               DelStApp(p->pNextAPP);
               DelTFKEY(p->pNextAPP->pGTabFKey);
               DelTKEYW(p->pNextAPP->pGTabKeyW);
               MemFree(p->pNextAPP);
               p->pNextAPP = pNext;
               DelAllRefApp(pSup);
               break;
            }
            p = p->pNextAPP;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime toutes les applications prsentes.
//
void DelAllApp(void)
{
   mtPTRAPP p,pNext;

   if(pAppli)
   {
      p = pAppli;
      while(p)
      {
         pNext = p->pNextAPP;
         MemFree(p->Path);
         MemFree(p->PathDBF);
         MemFree(p->PathVTX);
         DelDico(p->pPDico,p->uPDico);
         MemFree(p->pPDico);
         DelDico(p->pLDico,p->uLDico);
         MemFree(p->pLDico);
         DelListDico(p->pEDico,p->uEDico);
         MemFree(p->pEDico);
         DelDico(p->pIDico,p->uIDico);
         MemFree(p->pIDico);
         DelRefApp(p->pLRefApp);
         DelLstCv(p->pLstCnx);
         MemFree(p->pHere);
         MemFree(p->pGTCv);
         DelStApp(p);
         DelTFKEY(p->pGTabFKey);
         DelTKEYW(p->pGTabKeyW);
         MemFree(p);
         p = pNext;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une application par un nom ou un numro complmentaire.
// Renvoie un pointeur sur l'application trouve, ou NULL.
//
mtPTRAPP SearchApp(mtPTRBYTE NomApp,mtBYTE NComp)
{
   mtPTRAPP p,SApp=(mtPTRAPP) 0;

   p = pAppli;
   while(p)
   {
      if((NomApp) && (strcmp(p->Nom,NomApp) == 0))
         return(p);
      if((p->NumComp && NComp) && (p->NumComp == NComp))
         return(p);
      p = p->pNextAPP;
   }
   return(SApp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une application par un nom de fichier source.
// Renvoie un pointeur sur l'application trouve, ou NULL.
//
mtPTRAPP SearchAppFic(mtPTRBYTE NomFic)
{
   mtPTRAPP p,SApp=(mtPTRAPP) 0;

   p = pAppli;
   while(p)
   {
      if(filespeccmp(p->FicSrc,NomFic) == 0)
         return(p);
      p = p->pNextAPP;
   }
   return(SApp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                     PRIMITIVES DE COMPILATION/EXECUTION
//                     อออออออออออออออออออออออออออออออออออ
//
// Copie d'un dico local d'une appli dans la chaine des dico du cv courant,
// si ce n'est pas dj… fait.
//
void _CopyLDic(void)
{
   pPDICO TDic;
   mtPTRDICO d,NewD;
   mtPTRLISTDICO l,NewL;
   mtUWORD wID;
   static char cProcName[] = "COPYLDIC";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
   {
      // Tche locale, on prend les infos du cv propritaire.
      // (Les symboles locaux sont communs)
      pCurrCV->pLDico = pCurrLT->pLDico;
      pCurrCV->uLDico = pCurrLT->uLDico;
   }
   if(pCurrCV->pCApp->uLDico)
   {
      l = SearchListDico(pCurrCV->pLDico,pCurrCV->uLDico,pCurrCV->pCApp);
      if(! l)
      {
         // le dico local de l'appli n'est pas encore install dans le cv
         NewL = NewListDico(&pCurrCV->pLDico,&pCurrCV->uLDico,pCurrCV->pCApp);
         TDic = pCurrCV->pCApp->pLDico;
         wID  = 0;
         while(wID<pCurrCV->pCApp->uLDico)
         {
            d          = TDic[wID];
            NewD       = NewDicEntry(&NewL->pLDico,&NewL->uLDico,d->Nom,d->Type,mtFALSE);
            NewD->Attr = d->Attr;
            CopyValue(d,NewD);
            wID++;
         }
         if(pCurrCV->IsLTask)
         {
            // Tche locale, on met … jour les infos du cv propritaire.
            // (Les symboles locaux sont communs)
            pCurrLT->pLDico = pCurrCV->pLDico;
            pCurrLT->uLDico = pCurrCV->uLDico;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//
// La primitive de compilation de bas niveau.
//
void _Compile(pFUNC cSym)
{
   pCFIC->pCApp->pHere[pCFIC->wHERE++] = cSym;
   _CheckBlockSize();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//
// Rcupre et empile le pointeur suivant dans le bloc de code.
//
void _Literal()
{
   static char cProcName[] = "LITERAL";

   pCurrCV->pNProc = cProcName;
   _DPush((mtPOINTEUR) pCurrCV->pHere[pCurrCV->IP++]);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupre un pointeur sur une application
// (Utilise pour les copie de dictionnaires locaux)
//
void _RecupPTRAPP()
{
   static char cProcName[] = "RECUPPTRAPP";

   pCurrCV->pNProc = cProcName;
   pCurrCV->pCApp  = (mtPTRAPP) pCurrCV->pHere[pCurrCV->IP++];
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un branchement inconditionnel … l'IP suivant dans le bloc de code.
//
void _Branch(void)
{
   pCurrCV->IP = (mtUWORD) pCurrCV->pHere[pCurrCV->IP];
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un branchement conditionnel … l'IP suivant dans le bloc de code.
// Le branchement est effectu si le boolen prsent sur la pile est FAUX
// (== 0).
//
void _0Branch(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   mtPTRBYTE pCh;
   float Flt;

   RecupSym(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         if((pDSym->V.cVal) && (strlen(pDSym->V.cVal)))
            pCurrCV->IP++;
         else
            pCurrCV->IP = (mtUWORD) pCurrCV->pHere[pCurrCV->IP];
         break;
      }
      case mtTNUMBER:
      {
         if(pDSym->V.fVal != (float) 0)
            pCurrCV->IP++;
         else
            pCurrCV->IP = (mtUWORD) pCurrCV->pHere[pCurrCV->IP];
         break;
      }
      case mtTABCHAR:
      {
         pCh = ArrayCVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         if(pCh && (strlen(pCh)))
            pCurrCV->IP++;
         else
            pCurrCV->IP = (mtUWORD) pCurrCV->pHere[pCurrCV->IP];
         break;
      }
      case mtTABNUMBER:
      {
         Flt = ArrayNVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         if(Flt != (float) 0)
            pCurrCV->IP++;
         else
            pCurrCV->IP = (mtUWORD) pCurrCV->pHere[pCurrCV->IP];
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive de dbut de boucle DO ... LOOP | +LOOP
//
void _Do(void)
{
   float pFlt;
   mtPTRDICO pFin,pTmp,pDeb,pLi,pCo;
   static char cProcName[] = "DO";

   pCurrCV->pNProc = cProcName;
   //
   // On met les indices sur la pile de retour.
   //
   pFlt = RecupNumber(&pTmp,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pDeb = DupliqueSym(pTmp);
   pFlt = RecupNumber(&pFin,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   _RPush(pFin);
   _RPush(pDeb);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive de fin de boucle DO ... LOOP
//
void _Loop(void)
{
   mtPTRDICO pFin,pDeb;
   static char cProcName[] = "LOOP";

   pCurrCV->pNProc = cProcName;
//   pDeb = (mtPTRDICO) _RPop();
//   pFin = (mtPTRDICO) _RPop();
   pDeb = (mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-1];
   pFin = (mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-2];
   //
   // 1 tour de plus.
   //
//   pDeb->V.fVal += (float) 1;
   pDeb->V.fVal++;
   //
   // Test de la fin de boucle.
   //
   if(pDeb->V.fVal == pFin->V.fVal)
   {
      // Fin de la boucle.
      // On nettoie la pile de retour.
      pDeb = (mtPTRDICO) _RPop();
      pFin = (mtPTRDICO) _RPop();
      __TRUE();
   }
   else
   {
//      _RPush(pFin);
//      _RPush(pDeb);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive de fin de boucle DO ... +LOOP
//
void _PLoop(void)
{
   float pFlt;
   mtPTRDICO pFin,pDeb,pInc,pLi,pCo;
   static char cProcName[] = "+LOOP";

   pCurrCV->pNProc = cProcName;
   pFlt = RecupNumber(&pInc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   pDeb = (mtPTRDICO) _RPop();
//   pFin = (mtPTRDICO) _RPop();
   pDeb = (mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-1];
   pFin = (mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-2];
   //
   // 1 tour de plus.
   //
   pDeb->V.fVal += pFlt;
   //
   // Test de la fin de boucle.
   //
   if(pDeb->V.fVal >= pFin->V.fVal)
   {
      // Fin de la boucle.
      // On nettoie la pile de retour.
      pDeb = (mtPTRDICO) _RPop();
      pFin = (mtPTRDICO) _RPop();
      __TRUE();
   }
   else
   {
//      _RPush(pFin);
//      _RPush(pDeb);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive de dbut de slection OF ... ENDOF
//
void _Of(void)
{
   static char cProcName[] = "OF";

   pCurrCV->pNProc = cProcName;
   __Over();
   __Egal();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du IF
//
void _ExecIF(void)
{
   static char cProcName[] = "IF";

   pCurrCV->pNProc = cProcName;
   _0Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du ELSE
//
void _ExecELSE(void)
{
   static char cProcName[] = "ELSE";

   pCurrCV->pNProc = cProcName;
   _Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du AGAIN
//
void _ExecAGAIN(void)
{
   static char cProcName[] = "AGAIN";

   pCurrCV->pNProc = cProcName;
   _Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du UNTIL
//
void _ExecUNTIL(void)
{
   static char cProcName[] = "UNTIL";

   pCurrCV->pNProc = cProcName;
   _0Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du WHILE
//
void _ExecWHILE(void)
{
   static char cProcName[] = "WHILE";

   pCurrCV->pNProc = cProcName;
   _0Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du REPEAT
//
void _ExecREPEAT(void)
{
   static char cProcName[] = "REPEAT";

   pCurrCV->pNProc = cProcName;
   _Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du ENDOF
//
void _ExecENDOF(void)
{
   static char cProcName[] = "ENDOF";

   pCurrCV->pNProc = cProcName;
   _Branch();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'un symbole local au cv courant dans la chaine des dico.
//
void _SearchLSym()
{
   mtUWORD uPos;
   mtPTRDICO pDSym;
   mtPTRLISTDICO pLstDic;
   static char cProcName[] = "SEARCHLSYM";

   pCurrCV->pNProc = cProcName;
   uPos            = (mtUWORD) pCurrCV->pHere[pCurrCV->IP++];
   pCurrCV->pCApp  = (mtPTRAPP) pCurrCV->pHere[pCurrCV->IP++];
   if(pCurrCV->IsLTask)
   {
      // Tche locale, on prend les infos du cv propritaire.
      // (Les symboles locaux sont communs)
      pCurrCV->pLDico = pCurrLT->pLDico;
      pCurrCV->uLDico = pCurrLT->uLDico;
   }
   pLstDic = SearchListDico(pCurrCV->pLDico,pCurrCV->uLDico,pCurrCV->pCApp);
   pDSym   = pLstDic->pLDico[uPos];
   _DPush(pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                          FONCTIONS DIVERSES
//                          ออออออออออออออออออ
//
// Renvoie vrai si la chaine donne est un nombre,
// et initialise dNum avec celui-ci.
//
mtBOOL IsNumber(mtPTRBYTE pCNum,mtPTRFLOAT dNum)
{
   mtBYTE CarBase,cBase[20];
   mtWORD wBase=10;
   mtPTRBYTE Chaine;

   Chaine  = (mtPTRBYTE) MemAlloc(strlen(pCNum)+1);
   strcpy(Chaine,pCNum);
   CarBase = *(Chaine+(strlen(Chaine)-1));
   switch(CarBase)
   {
      case 'D':
      {
         // Dcimal.
         strcpy(cBase,"+-0123456789.,E");
         wBase = 10;
         *(Chaine+(strlen(Chaine)-1)) = '\0';
         break;
      }
      case 'H':
      {
         // Hxa.
         strcpy(cBase,"0123456789ABCDEF");
         wBase = 16;
         *(Chaine+(strlen(Chaine)-1)) = '\0';
         break;
      }
      case 'O':
      {
         // Octal.
         strcpy(cBase,"01234567");
         wBase = 8;
         *(Chaine+(strlen(Chaine)-1)) = '\0';
         break;
      }
      case 'B':
      {
         // Binaire.
         strcpy(cBase,"01");
         wBase = 2;
         *(Chaine+(strlen(Chaine)-1)) = '\0';
         break;
      }
      default:
      {
         // Base dcimale par dfaut.
         memset(cBase,0,20);
         strcpy(cBase,"+-0123456789.,E");
      }
   }
   if(*Chaine)
   {
      if(strspn(Chaine,cBase) == strlen(Chaine))
      {
         if(wBase == 10)
            *dNum = (float) strtof(Chaine,NULL);
         else
            *dNum = (float) strtol(Chaine,NULL,wBase);
         MemFree(Chaine);
         return(mtTRUE);
      }
   }
   MemFree(Chaine);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoi VRAI si Chaine reprsente un appel de procdure externe.
// (Syntaxe : NomLib::NomProc)
//
mtBOOL IsExternProc(mtPTRBYTE Chaine)
{
   mtBOOL bOk = mtFALSE;
   mtPTRBYTE NomLib,NomProc,pSep;
   mtWORD Len;
   mtPTRAPP pLib;
   mtPTRDICO pProc;
   mtPTRLISTDICO pLstDic;
   mtUWORD wPos;

   pSep = strstr(Chaine,"::");
   if(pSep)
   {
      // On rcupre le nom de la librairie.
      Len     = (pSep-Chaine);
      NomLib  = (mtPTRBYTE) MemAlloc(Len+1);
      strncpy(NomLib,Chaine,Len);
      // On rcupre le nom de la procdure.
      pSep   += 2;
      Len     = strlen(pSep)+1;
      NomProc = (mtPTRBYTE) MemAlloc(Len);
      strcpy(NomProc,pSep);
      pLib    = SearchApp(NomLib,0);
      if(pLib)
      {
         // On vrifie le rfrencement de l'appli.
         pLstDic = SearchListDico(pCFIC->pCApp->pEDico,pCFIC->pCApp->uEDico,pLib);
         if(pLstDic)
         {
            pProc = SearchDico(pLib->pPDico,pLib->uPDico,NomProc,&wPos);
            if(pProc && pProc->Type == mt2POINTS)
               bOk = mtTRUE;
            else
            {
               WinInfo("%s INEXISTANT OU NON VALIDE DANS %s",NomProc,NomLib);
               WriteLogNeo("%s INEXISTANT OU NON VALIDE DANS %s",NomProc,NomLib);
            }
         }
         else
         {
            WinInfo("%s INEXISTANT (USE ...)",NomLib);
            WriteLogNeo("%s INEXISTANT (USE ...)",NomLib);
         }
      }
      MemFree(NomProc);
      MemFree(NomLib);
   }
   return(bOk);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoi VRAI si Chaine reprsente une rfrence de variable externe
// (Syntaxe : NomLib.NomVar)
//
mtBOOL IsExternVar(mtPTRBYTE Chaine)
{
   mtBOOL bOk = mtFALSE;
   mtPTRBYTE NomLib,NomVar,pSep;
   mtWORD Len;
   mtPTRAPP pLib;
   mtPTRDICO pVar;
   mtPTRLISTDICO pLstDic;
   mtUWORD wPos;

   pSep = strchr(Chaine,'.');
   if(pSep)
   {
      // On rcupre le nom de la librairie.
      Len     = (pSep-Chaine);
      NomLib  = (mtPTRBYTE) MemAlloc(Len+1);
      strncpy(NomLib,Chaine,Len);
      // On rcupre le nom de la variable.
      pSep++;
      Len    = strlen(pSep)+1;
      NomVar = (mtPTRBYTE) MemAlloc(Len);
      strcpy(NomVar,pSep);
      pLib   = SearchApp(NomLib,0);
      if(pLib)
      {
         // On vrifie le rfrencement de l'appli.
         pLstDic = SearchListDico(pCFIC->pCApp->pEDico,pCFIC->pCApp->uEDico,pLib);
         if(pLstDic)
         {
            // On regarde d'abord si ce n'est pas une LOCALE
            pVar = SearchDico(pLib->pLDico,pLib->uLDico,NomVar,&wPos);
            if(pVar && pVar->Type != mt2POINTS)
               bOk = mtTRUE;
            else
            {
               // On regarde ensuite si ce n'est pas une PUBLIQUE
               pVar = SearchDico(pLib->pPDico,pLib->uPDico,NomVar,&wPos);
               if(pVar && pVar->Type != mt2POINTS)
                  bOk = mtTRUE;
               else
               {
                  WinInfo("%s INEXISTANTE OU NON VALIDE DANS %s",NomVar,NomLib);
                  WriteLogNeo("%s INEXISTANTE OU NON VALIDE DANS %s",NomVar,NomLib);
               }
            }
         }
         else
         {
            WinInfo("%s INEXISTANT (USE ...)",NomLib);
            WriteLogNeo("%s INEXISTANT (USE ...)",NomLib);
         }
      }
      else
      {
         WinInfo("APPLICATION %s INEXISTANTE",NomLib);
         WriteLogNeo("APPLICATION %s INEXISTANTE",NomLib);
      }
      MemFree(NomVar);
      MemFree(NomLib);
   }
   return(bOk);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation d'un symbole dans le bloc de code de l'application courante.
//
void CompileSym(mtPTRDICO pCSym,mtWORD cPortee,mtPTRAPP pCApp,mtUWORD wPos)
{
   switch(pCSym->Type)
   {
      case mtTCHAR:
      case mtTABCHAR:
      case mtTNUMBER:
      case mtTABNUMBER:
      {
         // Type Caractre, Chaine ou Nombre
         // simple ou en tableau
         switch(cPortee)
         {
            case PGLOBAL:
            {
               // Variable globale.
               _Compile(_Literal);
               _Compile((pFUNC) pCSym);
               break;
            }
            case PPUBLIC:
            {
               // Variable publique.
               _Compile(_Literal);
               _Compile((pFUNC) pCSym);
               if(pCApp == pCFIC->pCApp)
                  // Marque le symbole local comme tant utilis.
                  pCSym->bAccessed = mtTRUE;
               break;
            }
            case PLOCAL:
            {
               // Variable locale, on compile le processus de recherche.
               if(pCApp != pCFIC->pCApp)
               {
                  // C'est une variable externe … l'appli en cours.
                  _Compile(_RecupPTRAPP);           // processus de recopie
                  _Compile((pFUNC) pCApp);          // du dico local de l'appli
                  _Compile((pFUNC) _CopyLDic);      // en train de s'excuter.
               }
               else
                  // Marque le symbole local comme tant utilis.
                  pCSym->bAccessed = mtTRUE;
               _Compile((pFUNC) _SearchLSym);
               _Compile((pFUNC) wPos);
               _Compile((pFUNC) pCApp);
               break;
            }
            case PINTERNE:
            {
               // Variable interne.
               _Compile(_Literal);
               _Compile((pFUNC) pCSym);
               break;
            }
         }
         break;
      }
      case mtKERNEL:
      {
         // Type Primitive.
         _Compile((pFUNC) pCSym->V.pVal);
         break;
      }
      case mt2POINTS:
      {
         // Type Procdure/Fonction.
         _Compile(_2Points);
         _Compile(pCSym->V.pVal);      // Segment.
         _Compile(pCSym->XVal);        // Offset.
         if(pCApp == pCFIC->pCApp)
            // Marque le symbole local comme tant utilis.
            pCSym->bAccessed = mtTRUE;
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation de la fin de la dfinition principale .
//
void CompileMNext(void)
{
   _Compile(_MNext);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation de la fin d'une dfinition.
//
void CompileNext(void)
{
   _Compile(_Next);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation de la fin d'une dfinition ONLIB.
//
void CompileOLNext(void)
{
   _Compile(_OLNext);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation de la fin d'une tche globale.
//
void CompileGTNext(void)
{
   _Compile(_GTNext);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cherche et Converti les codes de contr“les ("\XXX")
// prsents dans une chaine.
//
mtPTRBYTE ConvCTRL(mtPTRBYTE Chaine)
{
   int pNew=0,iVal,nbc;
   mtPTRBYTE NewChaine,OldCh;
   mtBYTE cVal[4];

   NewChaine = (mtPTRBYTE) MemAlloc(strlen(Chaine)+1);
   while(*Chaine)
   {
      if(*Chaine == 0x5C)
      {
         memset(cVal,0,4);
         OldCh = Chaine;
         nbc   = 0;
         while(*Chaine)
         {
            Chaine++;
            if(isdigit(*Chaine))
            {
               cVal[nbc] = *Chaine;
               nbc++;
               if(nbc>=3)
               {
                  Chaine++;
                  break;
               }
            }
            else
               break;
         }
         if(nbc)
         {
            iVal = atoi(cVal);
            sprintf(cVal,"%c",iVal);
            *(NewChaine+pNew) = cVal[0];
         }
         else
         {
            Chaine = OldCh;
            *(NewChaine+pNew) = *Chaine;
            Chaine++;
         }
      }
      else
      {
         *(NewChaine+pNew) = *Chaine;
         Chaine++;
      }
      pNew++;
   }
   *(NewChaine+pNew) = '\0';
   NewChaine         = (mtPTRBYTE) MemReAlloc(NewChaine,strlen(NewChaine)+1);
   *(NewChaine+strlen(NewChaine)) = '\0';
   return(NewChaine);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation d'une chaine.
//
void CompileChaine(mtPTRBYTE pCh)
{
   mtPTRDICO NewD;

   NewD         = GenAppISym(mtTCHAR,SYMLITERAL);
   NewD->V.cVal = ConvCTRL(pCh);
   CompileSym(NewD,PINTERNE,pCFIC->pCApp,0);
   if(pCFIC->bPUBLICDEF)
      pCFIC->pCApp->SizePData += (mtUWORD) (sizeof(Dico)+strlen(pCh)+1);
   else
      pCFIC->pCApp->SizeLData += (mtUWORD) (sizeof(Dico)+strlen(pCh)+1);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation d'un nombre.
//
void CompileNombre(float Valeur)
{
   mtPTRDICO NewD;

   NewD         = GenAppISym(mtTNUMBER,SYMLITERAL);
   NewD->V.fVal = Valeur;
   CompileSym(NewD,PINTERNE,pCFIC->pCApp,0);
   if(pCFIC->bPUBLICDEF)
      pCFIC->pCApp->SizePData += (mtUWORD) sizeof(Dico);
   else
      pCFIC->pCApp->SizeLData += (mtUWORD) sizeof(Dico);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation d'un appel de procdure externe.
// La syntaxe est valide (vrifie par IsExternProc()).
//
void CompileExternProc(mtPTRBYTE Chaine)
{
   mtPTRBYTE NomLib,NomProc,pSep;
   mtWORD Len;
   mtPTRAPP pLib;
   mtPTRDICO pProc;
   mtPTRREFAPP NewR;
   mtUWORD wPos;

   pSep = strstr(Chaine,"::");
   if(pSep)
   {
      // On rcupre le nom de la librairie.
      Len     = (pSep-Chaine);
      NomLib  = (mtPTRBYTE) MemAlloc(Len+1);
      strncpy(NomLib,Chaine,Len);
      // On rcupre le nom de la procdure.
      pSep   += 2;
      Len     = strlen(pSep)+1;
      NomProc = (mtPTRBYTE) MemAlloc(Len);
      strcpy(NomProc,pSep);
      pLib    = SearchApp(NomLib,0);
      if(pLib)
      {
         pProc = SearchDico(pLib->pPDico,pLib->uPDico,NomProc,&wPos);
         if(pProc && pProc->Type == mt2POINTS)
         {
            _Compile(_RecupPTRAPP);           // processus de recopie
            _Compile((pFUNC) pLib);           // du dico local de l'appli
            _Compile((pFUNC) _CopyLDic);      // appele.
            // On compile l'appel … la procdure.
            CompileSym(pProc,NULL,pLib,wPos);
            //
            // On ajoute la rfrence … l'appli courante dans la liste
            // des rf. croises de pLib.
            //
            NewR = NewRefApp(pLib->pLRefApp,pCFIC->pCApp);
            if(! pLib->pLRefApp)
               // 1re entre.
               pLib->pLRefApp = NewR;
         }
      }
      MemFree(NomProc);
      MemFree(NomLib);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation d'une rfrence de variable externe
// La syntaxe est valide (vrifie par IsExternVar()).
//
void CompileExternVar(mtPTRBYTE Chaine)
{
   mtPTRBYTE NomLib,NomVar,pSep;
   mtWORD Len;
   mtPTRAPP pLib;
   mtPTRDICO pVar;
   mtUWORD wPos;

   pSep = strchr(Chaine,'.');
   if(pSep)
   {
      // On rcupre le nom de la librairie.
      Len     = (pSep-Chaine);
      NomLib  = (mtPTRBYTE) MemAlloc(Len+1);
      strncpy(NomLib,Chaine,Len);
      // On rcupre le nom de la procdure.
      pSep++;
      Len    = strlen(pSep)+1;
      NomVar = (mtPTRBYTE) MemAlloc(Len);
      strcpy(NomVar,pSep);
      pLib   = SearchApp(NomLib,0);
      if(pLib)
      {
         pVar = SearchDico(pLib->pLDico,pLib->uLDico,NomVar,&wPos);
         if(pVar && pVar->Type != mt2POINTS)
         {
            // Variable externe LOCALE
            _Compile(_RecupPTRAPP);           // processus de recopie
            _Compile((pFUNC) pLib);           // du dico local de l'appli
            _Compile((pFUNC) _CopyLDic);      // appele.
            // On compile la rfrence … la variable
            CompileSym(pVar,PLOCAL,pLib,wPos);
         }
         else
         {
            pVar = SearchDico(pLib->pPDico,pLib->uPDico,NomVar,&wPos);
            if(pVar && pVar->Type != mt2POINTS)
            {
               // Variable externe PUBLIQUE
               // On compile la rfrence … la variable
               CompileSym(pVar,PPUBLIC,pLib,wPos);
            }
         }
      }
      MemFree(NomVar);
      MemFree(NomLib);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement du dbut d'une dfinition.
//
mtBOOL CompBEGDEF (void)
{
   mtPTRBYTE cItem;
   mtWORD wPortee;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   if(! pCFIC->bIn2Points)
   {
      cItem = strtok(NULL,cDelim);     // Rcupre le nom du symbole.
      if(cItem)
      {
         strupr(cItem);
         pDSym = SearchSymb(cItem,&wPortee,&wPos);
         if(! pDSym)
         {
            //
            // Cration de la nouvelle dfinition.
            //
            pCFIC->bIn2Points = mtTRUE;
            pDSym             = NewDicEntry(&pCFIC->pCApp->pPDico,&pCFIC->pCApp->uPDico,cItem,mt2POINTS,mtTRUE);
            pDSym->V.pVal     = pCFIC->pCApp->pHere;
            pDSym->XVal       = pCFIC->wHERE;
            //
            // On regarde si ce n'est pas la procdure principale
            //
            if(strcmp(cItem,"MAIN") == 0)
            {
               // C'est le point d'entre du programme
               pCFIC->pCApp->ENTRY = pCFIC->wHERE;
               pCFIC->bInMain      = mtTRUE;
            }
            //
            // On regarde si ce n'est pas la procdure de fin
            //
            if(strcmp(cItem,"ONLIB") == 0)
            {
               pCFIC->pCApp->ONLIB = pCFIC->wHERE;
               pCFIC->bOnLib       = mtTRUE;
            }
            //
            // On regarde si ce n'est pas la tche globale
            //
            if(strcmp(cItem,"GTASK") == 0)
            {
               pCFIC->pCApp->GTASK = pCFIC->wHERE;
               pCFIC->bGTask       = mtTRUE;
            }
         }
         else
         {
            WinInfo("%s Existe dj…",cItem);
            WriteLogNeo("%s Existe dj…",cItem);
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nom de procdure manquant");
         WriteLogNeo("Nom de procdure manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Dj… en cours de dfinition");
      WriteLogNeo("Dj… en cours de dfinition");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la fin d'une dfinition.
//
mtBOOL CompENDDEF (void)
{
   if(pCFIC->bIn2Points)
   {
      if(pCFIC->bInMain)
      {
         CompileMNext();
         pCFIC->bMainDef = mtTRUE;
      }
      else
      {
         if(pCFIC->bOnLib)
         {
            CompileOLNext();
            pCFIC->pCApp->bOnLib = mtTRUE;
         }
         else
         {
            if(pCFIC->bGTask)
            {
               CompileGTNext();
               pCFIC->pCApp->bGTask = mtTRUE;
            }
            else
               CompileNext();
         }
      }
      pCFIC->bIn2Points = mtFALSE;
      pCFIC->bInMain    = mtFALSE;
      pCFIC->bOnLib     = mtFALSE;
      // On vrifie les structures de contr“le.
      if((pCFIC->NbDO) || (pCFIC->NbIF) || (pCFIC->NbBEGIN) || (pCFIC->NbWHILE) || (pCFIC->NbCASE) || (pCFIC->NbOF))
      {
         // Erreurs d'imbrications
         if(pCFIC->NbDO)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (DO...LOOP|+LOOP)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (DO...LOOP|+LOOP)");
         }
         if(pCFIC->NbIF)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (IF...[ELSE...] ENDIF)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (IF...[ELSE...] ENDIF)");
         }
         if(pCFIC->NbBEGIN)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (BEGIN...AGAIN|UNTIL)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (BEGIN...AGAIN|UNTIL)");
         }
         if(pCFIC->NbWHILE)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (BEGIN...WHILE...REPEAT)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (BEGIN...WHILE...REPEAT)");
         }
         if(pCFIC->NbCASE)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (CASE OF...ENDOF ENDCASE)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (CASE OF...ENDOF ENDCASE)");
         }
         if(pCFIC->NbOF)
         {
            WinInfo("STRUCTURE(S) MAL IMBRIQUEE(S) (CASE OF...ENDOF ENDCASE)");
            WriteLogNeo("STRUCTURE(S) MAL IMBRIQUEE(S) (CASE OF...ENDOF ENDCASE)");
         }
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("': <nom>' manquant");
      WriteLogNeo("': <nom>' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la compilation d'une chaine.
//
mtBOOL CompSTRING(void)
{
   mtPTRBYTE cItem;

   cItem = strtok(NULL,"\x22");      // Cherche le prochain guillemet.
   if(cItem)
   {
      CompileChaine(cItem);
   }
   else
   {
      WinInfo("Guillemet de fin de chaine manquant");
      WriteLogNeo("Guillemet de fin de chaine manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la compilation du type CHAR.
//
mtBOOL CompCHAR(void)
{
   mtPTRBYTE cItem,pCh;
   mtWORD wPortee;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cItem = strtok(NULL,cDelim);        // Rcupre le nom du symbole.
   if(cItem)
   {
      while(cItem)
      {
         if(strncmp(cItem,"//",2) == 0)
         {
            pCFIC->bFinLigne = mtTRUE;
            break;
         }
         strupr(cItem);
         pDSym = SearchSymb(cItem,&wPortee,&wPos);
         if(! pDSym)
         {
            //
            // Cration du nouveau symbole.
            //
            if(pCFIC->bPUBLICDEF)
            {
               pDSym       = NewDicEntry(&pCFIC->pCApp->pPDico,&pCFIC->pCApp->uPDico,cItem,mtTCHAR,mtTRUE);
               pDSym->Attr = SYMVARCON;
               pCFIC->pCApp->SizePData += (mtUWORD) sizeof(Dico);
            }
            else
            {
               if(pCFIC->bLOCALDEF)
               {
                  pDSym       = NewDicEntry(&pCFIC->pCApp->pLDico,&pCFIC->pCApp->uLDico,cItem,mtTCHAR,mtTRUE);
                  pDSym->Attr = SYMVARCON;
                  pCFIC->pCApp->SizeLData += (mtUWORD) sizeof(Dico);
               }
               else
               {
                  // dfinition externe.
               }
            }
            //
            // Recherche d'une ventuelle assignation.
            // (exemple: PUBLIC CHAR CHAINE := "...")
            //
            if(! pCFIC->bEXTERNDEF)
            {
               cItem = strtok(NULL,cDelim);
               if(cItem)
               {
                  if(strcmp(cItem,":=") == 0)
                  {
                     cItem = strtok(NULL,"\x22");      // Cherche le guillemet.
                     if(cItem)
                     {
                        pCh = ConvCTRL(cItem);
                        pDSym->V.cVal = pCh;
                        if(pCFIC->bPUBLICDEF)
                           pCFIC->pCApp->SizePData += (mtUWORD) (strlen(pCh)+1);
                        else
                           pCFIC->pCApp->SizeLData += (mtUWORD) (strlen(pCh)+1);
                     }
                     else
                     {
                        WinInfo("Guillemet de fin de chaine manquant");
                        WriteLogNeo("Guillemet de fin de chaine manquant");
                        return(mtTRUE);
                     }
                  }
                  else
                  {
                     if(pCFIC->bPUBLICDEF)
                        // On initialise avec une chaine vide.
                        pDSym->V.cVal = (mtPTRBYTE) MemAlloc(1);
                     continue;
                  }
               }
               else
                  continue;
            }
            else
            {
               WinInfo("Assignation impossible avec EXTERN <Symbole>");
               WriteLogNeo("Assignation impossible avec EXTERN <Symbole>");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("%s Existe dj…",cItem);
            WriteLogNeo("%s Existe dj…",cItem);
            return(mtTRUE);
         }
         cItem = strtok(NULL,cDelim);
      }
   }
   else
   {
      WinInfo("Nom de la donne manquant");
      WriteLogNeo("Nom de la donne manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la compilation du type NUMBER.
//
mtBOOL CompNUMBER(void)
{
   mtPTRBYTE cItem;
   mtWORD wPortee;
   float dVal;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cItem = strtok(NULL,cDelim);        // Rcupre le nom du symbole.
   if(cItem)
   {
      while(cItem)
      {
         if(strncmp(cItem,"//",2) == 0)
         {
            pCFIC->bFinLigne = mtTRUE;
            break;
         }
         strupr(cItem);
         pDSym = SearchSymb(cItem,&wPortee,&wPos);
         if(! pDSym)
         {
            //
            // Cration du nouveau symbole.
            //
            if(pCFIC->bPUBLICDEF)
            {
               pDSym       = NewDicEntry(&pCFIC->pCApp->pPDico,&pCFIC->pCApp->uPDico,cItem,mtTNUMBER,mtTRUE);
               pDSym->Attr = SYMVARCON;
               pCFIC->pCApp->SizePData += (mtUWORD) sizeof(Dico);
            }
            else
            {
               if(pCFIC->bLOCALDEF)
               {
                  pDSym       = NewDicEntry(&pCFIC->pCApp->pLDico,&pCFIC->pCApp->uLDico,cItem,mtTNUMBER,mtTRUE);
                  pDSym->Attr = SYMVARCON;
                  pCFIC->pCApp->SizeLData += (mtUWORD) sizeof(Dico);
               }
               else
               {
                  // dfinition externe.
               }
            }
            //
            // Recherche d'une ventuelle assignation.
            // (exemple: PUBLIC NUMBER OPTION := 5)
            //
            if(! pCFIC->bEXTERNDEF)
            {
               cItem = strtok(NULL,cDelim);
               if(cItem)
               {
                  if(strcmp(cItem,":=") == 0)
                  {
                     cItem = strtok(NULL,cDelim);
                     if(cItem)
                     {
                        if(IsNumber(cItem,&dVal))
                           pDSym->V.fVal = dVal;
                        else
                        {
                           WinInfo("Type incompatible");
                           WriteLogNeo("Type incompatible");
                           return(mtTRUE);
                        }
                     }
                     else
                     {
                        WinInfo("valeur d'assignation manquante");
                        WriteLogNeo("valeur d'assignation manquante");
                        return(mtTRUE);
                     }
                  }
                  else
                     continue;
               }
               else
                  continue;
            }
            else
            {
               WinInfo("Assignation impossible avec EXTERN <Symbole>");
               WriteLogNeo("Assignation impossible avec EXTERN <Symbole>");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("%s Existe dj…",cItem);
            WriteLogNeo("%s Existe dj…",cItem);
            return(mtTRUE);
         }
         cItem = strtok(NULL,cDelim);
      }
   }
   else
   {
      WinInfo("Nom de la donne manquant");
      WriteLogNeo("Nom de la donne manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la compilation du type TABLEAU DE CHAR.
//
mtBOOL CompTABCHAR(void)
{
   mtPTRBYTE cItem;
   mtWORD wPortee;
   mtUWORD wPos;
   mtUWORD Li,Co;
   mtUWORD NbLig,NbCol;
   mtPTRDICO pDSym;
   float dTmp;
   mtPTRITAB pITmp;

   cItem = strtok(NULL,cDelim);        // Rcupre le nombre de lignes.
   if(cItem)
   {
      if(IsNumber(cItem,&dTmp))
      {
         NbLig = (mtUWORD) dTmp;
         cItem = strtok(NULL,cDelim);  // Rcupre le nombre de colonnes.
         if(cItem)
         {
            if(IsNumber(cItem,&dTmp))
            {
               NbCol = (mtUWORD) dTmp;
               cItem = strtok(NULL,cDelim);      // Rcupre le nom du symbole.
               if(cItem)
               {
                  strupr(cItem);
                  pDSym = SearchSymb(cItem,&wPortee,&wPos);
                  if(! pDSym)
                  {
                     //
                     // Cration du nouveau symbole.
                     //
                     if(pCFIC->bPUBLICDEF)
                     {
                        pDSym       = NewDicEntry(&pCFIC->pCApp->pPDico,&pCFIC->pCApp->uPDico,cItem,mtTABCHAR,mtTRUE);
                        pDSym->Attr = SYMVARCON;
                        pCFIC->pCApp->SizePData += (mtUWORD) sizeof(Dico);
                     }
                     else
                     {
                        if(pCFIC->bLOCALDEF)
                        {
                           pDSym       = NewDicEntry(&pCFIC->pCApp->pLDico,&pCFIC->pCApp->uLDico,cItem,mtTABCHAR,mtTRUE);
                           pDSym->Attr = SYMVARCON;
                           pCFIC->pCApp->SizeLData += (mtUWORD) sizeof(Dico);
                        }
                        else
                        {
                           // dfinition externe.
                        }
                     }
                     //
                     // Rservation de la zone du tableau.
                     // (NbLig*NbCol)*sizeof(POINTEUR)
                     //
                     if(! pCFIC->bEXTERNDEF)
                     {
                        if(NbLig && NbCol)
                        {
                           if(pCFIC->bPUBLICDEF)
                           {
                              // On initialise un tableau de chaines vides.
                              pDSym->V.tVal = (pPOINTEUR) MemAlloc((NbLig*NbCol)*sizeof(mtPOINTEUR));
                              for(Li=0;Li<NbLig;Li++)
                              {
                                 for(Co=0;Co<NbCol;Co++)
                                 {
                                    *((pPOINTEUR) pDSym->V.tVal+((Li*NbCol)+Co)) = (mtPTRBYTE) MemAlloc(1);
                                 }
                              }
                           }
                           if(pCFIC->bPUBLICDEF)
                              pCFIC->pCApp->SizePData += (mtUWORD) ((NbLig*NbCol)*sizeof(mtPOINTEUR));
                           else
                              pCFIC->pCApp->SizeLData += (mtUWORD) ((NbLig*NbCol)*sizeof(mtPOINTEUR));
                           pITmp = (mtPTRITAB) MemAlloc(sizeof(InfTab));
                           if(pCFIC->bPUBLICDEF)
                              pCFIC->pCApp->SizePData += (mtUWORD) (sizeof(InfTab));
                           else
                              pCFIC->pCApp->SizeLData += (mtUWORD) (sizeof(InfTab));
                           pITmp->MaxLig = NbLig;
                           pITmp->MaxCol = NbCol;
                           pDSym->XVal   = (mtPOINTEUR) pITmp;
                        }
                        else
                        {
                           WinInfo("Ligne ou Colonne nulle");
                           WriteLogNeo("Ligne ou Colonne nulle");
                           return(mtTRUE);
                        }
                     }
                  }
                  else
                  {
                     WinInfo("%s Existe dj…",cItem);
                     WriteLogNeo("%s Existe dj…",cItem);
                     return(mtTRUE);
                  }
               }
               else
               {
                  WinInfo("Nom du symbole manquant");
                  WriteLogNeo("Nom du symbole manquant");
                  return(mtTRUE);
               }
            }
            else
            {
               WinInfo("Nombre de colonnes manquant");
               WriteLogNeo("Nombre de colonnes manquant");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Nombre de colonnes manquant");
            WriteLogNeo("Nombre de colonnes manquant");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nombre de lignes manquant");
         WriteLogNeo("Nombre de lignes manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Nombre de lignes manquant");
      WriteLogNeo("Nombre de lignes manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la compilation du type TABLEAU DE NUMBER.
//
mtBOOL CompTABNUMBER(void)
{
   mtPTRBYTE cItem;
   mtWORD wPortee;
   mtUWORD wPos;
   mtUWORD NbLig,NbCol;
   mtPTRDICO pDSym;
   float dTmp;
   mtPTRITAB pITmp;

   cItem = strtok(NULL,cDelim);        // Rcupre le nombre de lignes.
   if(cItem)
   {
      if(IsNumber(cItem,&dTmp))
      {
         NbLig = (mtUWORD) dTmp;
         cItem = strtok(NULL,cDelim);  // Rcupre le nombre de colonnes.
         if(cItem)
         {
            if(IsNumber(cItem,&dTmp))
            {
               NbCol = (mtUWORD) dTmp;
               cItem = strtok(NULL,cDelim);      // Rcupre le nom du symbole.
               if(cItem)
               {
                  strupr(cItem);
                  pDSym = SearchSymb(cItem,&wPortee,&wPos);
                  if(! pDSym)
                  {
                     //
                     // Cration du nouveau symbole.
                     //
                     if(pCFIC->bPUBLICDEF)
                     {
                        pDSym       = NewDicEntry(&pCFIC->pCApp->pPDico,&pCFIC->pCApp->uPDico,cItem,mtTABNUMBER,mtTRUE);
                        pDSym->Attr = SYMVARCON;
                        pCFIC->pCApp->SizePData += (mtUWORD) sizeof(Dico);
                     }
                     else
                     {
                        if(pCFIC->bLOCALDEF)
                        {
                           pDSym       = NewDicEntry(&pCFIC->pCApp->pLDico,&pCFIC->pCApp->uLDico,cItem,mtTABNUMBER,mtTRUE);
                           pDSym->Attr = SYMVARCON;
                           pCFIC->pCApp->SizeLData += (mtUWORD) sizeof(Dico);
                        }
                        else
                        {
                           // dfinition externe.
                        }
                     }
                     //
                     // Rservation de la zone du tableau.
                     // (NbLig*NbCol)*sizeof(float)
                     //
                     if(! pCFIC->bEXTERNDEF)
                     {
                        if(NbLig && NbCol)
                        {
                           // pDSym->V.tfVal = (mtPTRFLOAT) MemAlloc((NbLig*NbCol)*sizeof(float));
                           if(pCFIC->bPUBLICDEF)
                              pCFIC->pCApp->SizePData += (mtUWORD) ((NbLig*NbCol)*sizeof(float));
                           else
                              pCFIC->pCApp->SizeLData += (mtUWORD) ((NbLig*NbCol)*sizeof(float));
                           pITmp = (mtPTRITAB) MemAlloc(sizeof(InfTab));
                           if(pCFIC->bPUBLICDEF)
                              pCFIC->pCApp->SizePData += (mtUWORD) (sizeof(InfTab));
                           else
                              pCFIC->pCApp->SizeLData += (mtUWORD) (sizeof(InfTab));
                           pITmp->MaxLig = NbLig;
                           pITmp->MaxCol = NbCol;
                           pDSym->XVal   = (mtPOINTEUR) pITmp;
                        }
                        else
                        {
                           WinInfo("Ligne ou Colonne nulle");
                           WriteLogNeo("Ligne ou Colonne nulle");
                           return(mtTRUE);
                        }
                     }
                  }
                  else
                  {
                     WinInfo("%s Existe dj…",cItem);
                     WriteLogNeo("%s Existe dj…",cItem);
                     return(mtTRUE);
                  }
               }
               else
               {
                  WinInfo("Nom du symbole manquant");
                  WriteLogNeo("Nom du symbole manquant");
                  return(mtTRUE);
               }
            }
            else
            {
               WinInfo("Nombre de colonnes manquant");
               WriteLogNeo("Nombre de colonnes manquant");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Nombre de colonnes manquant");
            WriteLogNeo("Nombre de colonnes manquant");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nombre de lignes manquant");
         WriteLogNeo("Nombre de lignes manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Nombre de lignes manquant");
      WriteLogNeo("Nombre de lignes manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la directive PUBLIC.
//
mtBOOL CompPUBDEF(void)
{
   pCFIC->bPUBLICDEF = mtTRUE;
   pCFIC->bLOCALDEF  = mtFALSE;
   pCFIC->bEXTERNDEF = mtFALSE;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la directive LOCAL.
//
mtBOOL CompLOCDEF(void)
{
   pCFIC->bPUBLICDEF = mtFALSE;
   pCFIC->bLOCALDEF  = mtTRUE;
   pCFIC->bEXTERNDEF = mtFALSE;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la directive USE.
//
mtBOOL CompUSE(void)
{
   mtPTRBYTE cItem;
   mtPTRAPP pLib;
   mtPTRLISTDICO pLstDic;
   mtPTRCFIC pTFic;
   char file[MAXPATH];
   char *PathFic,*NomFic;
   mtBOOL bErr;
   char tUSE[20][MAXPATH];
   mtUWORD iUse=0,iMax=0;

   // Rcupration des noms des fichiers.
   cItem = strtok(NULL,cDelim);
   while(cItem)
   {
      strupr(cItem);
      if(strncmp(cItem,"//",2) == 0)
         break;
      strcpy(tUSE[iMax],cItem);
      iMax++;
      if(iMax>=20)
         break;
      cItem = strtok(NULL,cDelim);
   }
   if(iMax)
   {
      while(iUse<iMax)
      {
         bErr = mtFALSE;
         NomFic = filespecgetroot(filespecname(tUSE[iUse]));
         if(strlen(filespecname(tUSE[iUse])) == strlen(tUSE[iUse]))
         {
            // Pas de chemin spcifi, on ajoute PathAPPS.
            PathFic = filespecaddpath(PathAPPS,NomFic);
            strcpy(file,PathFic);
            MemFree(PathFic);
         }
         else
            // Un chemin est donn
            strcpy(file,tUSE[iUse]);
         pLib = SearchApp(NomFic,0);
         if(! pLib)
         {
            // L'application n'existe pas encore, on
            // va la compiler.
            PushCFic(pCFIC);
            bErr  = LoadApp(file);
            pCFIC = (mtPTRCFIC) PopCFic();
         }
         if(! bErr)
         {
            pLib = SearchApp(NomFic,0);
            MemFree(NomFic);
            if(pLib)
            {
               // On ajoute le dictionnaire local du module utilis
               // … la liste des dictionnaires locaux de l'appli courante.
               pLstDic = SearchListDico(pCFIC->pCApp->pEDico,pCFIC->pCApp->uEDico,pLib);
               if(! pLstDic)
               {
                  pLstDic = NewListDico(&pCFIC->pCApp->pEDico,&pCFIC->pCApp->uEDico,pLib);
               }
            }
            else
            {
               WinInfo("%s NON TROUVEE !!!",cItem);
               WriteLogNeo("%s NON TROUVEE !!!",cItem);
               return(mtTRUE);
            }
         }
         else
         {
            MemFree(NomFic);
         //   return(mtTRUE);
         }
         iUse++;
      }
      pCFIC->bFinLigne = mtTRUE;
   }
   else
   {
      WinInfo("USE: Nom d'application manquant");
      WriteLogNeo("USE: Nom d'application manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement du dbut de la conditionnelle IF ... ENDIF
//
mtBOOL CompIF(void)
{
   _Compile((pFUNC) _ExecIF);
   PushMark(pCFIC->wHERE);
   pCFIC->wHERE++;
   _CheckBlockSize();
   pCFIC->NbIF++;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de l'alternative de la conditionnelle IF ... ELSE ... ENDIF
//
mtBOOL CompELSE(void)
{
   mtUWORD wIF;

   if(pCFIC->NbIF)
   {
      // On rcupre le branchement du IF ...
      wIF = PopMark();
      if(wIF != UINT_MAX)
      {
         _Compile((pFUNC) _ExecELSE);
         PushMark(pCFIC->wHERE);
         pCFIC->wHERE++;
         _CheckBlockSize();
         // On rsout le branchement du IF ...
         pCFIC->pCApp->pHere[wIF] = (pFUNC) pCFIC->wHERE;
      }
      else
      {
         WinInfo("IF MANQUANT (IF...ELSE...ENDIF)");
         WriteLogNeo("IF MANQUANT (IF...ELSE...ENDIF)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("IF MANQUANT (IF...ELSE...ENDIF)");
      WriteLogNeo("IF MANQUANT (IF...ELSE...ENDIF)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement de la fin de la conditionnelle IF ... ENDIF
//
mtBOOL CompENDIF(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbIF)
   {
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         // On rsout le branchement.
         pCFIC->pCApp->pHere[wSaut] = (pFUNC) pCFIC->wHERE;
         pCFIC->NbIF--;
      }
      else
      {
         WinInfo("IF MANQUANT (IF...[ELSE...] ENDIF)");
         WriteLogNeo("IF MANQUANT (IF...[ELSE...] ENDIF)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("IF MANQUANT (IF...[ELSE...] ENDIF)");
      WriteLogNeo("IF MANQUANT (IF...[ELSE...] ENDIF)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dbut de boucle DO ... LOOP
//
mtBOOL CompDO(void)
{
   _Compile((pFUNC) _Do);
   PushMark(pCFIC->wHERE);
   pCFIC->NbDO++;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de boucle DO ... LOOP
//
mtBOOL CompLOOP(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbDO)
   {
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         _Compile((pFUNC) _Loop);
         _Compile((pFUNC) _0Branch);
         _Compile((pFUNC) wSaut);
         pCFIC->NbDO--;
      }
      else
      {
         WinInfo("DO MANQUANT (DO...LOOP)");
         WriteLogNeo("DO MANQUANT (DO...LOOP)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("DO MANQUANT (DO...LOOP)");
      WriteLogNeo("DO MANQUANT (DO...LOOP)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de boucle DO ... +LOOP
//
mtBOOL CompPLOOP(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbDO)
   {
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         _Compile((pFUNC) _PLoop);
         _Compile((pFUNC) _0Branch);
         _Compile((pFUNC) wSaut);
         pCFIC->NbDO--;
      }
      else
      {
         WinInfo("DO MANQUANT (DO...+LOOP)");
         WriteLogNeo("DO MANQUANT (DO...+LOOP)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("DO MANQUANT (DO...+LOOP)");
      WriteLogNeo("DO MANQUANT (DO...+LOOP)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile l'accs … l'indice de boucle de niveau 1.
//
mtBOOL CompLOOPI(void)
{
   if(pCFIC->NbDO)
      _Compile((pFUNC) _LoopI);
   else
   {
      WinInfo("I S'UTILISE DANS UNE STRUCTURE DO...LOOP");
      WriteLogNeo("I S'UTILISE DANS UNE STRUCTURE DO...LOOP");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile l'accs … l'indice de boucle de niveau 2.
//
mtBOOL CompLOOPJ(void)
{
   if(pCFIC->NbDO >= 2)
      _Compile((pFUNC) _LoopJ);
   else
   {
      WinInfo("J S'UTILISE DANS UNE STRUCTURE DO DO...LOOP LOOP");
      WriteLogNeo("J S'UTILISE DANS UNE STRUCTURE DO DO...LOOP LOOP");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile l'accs … l'indice de boucle de niveau 3.
//
mtBOOL CompLOOPK(void)
{
   if(pCFIC->NbDO >= 3)
      _Compile((pFUNC) _LoopK);
   else
   {
      WinInfo("K S'UTILISE DANS UNE STRUCTURE DO DO DO...LOOP LOOP LOOP");
      WriteLogNeo("K S'UTILISE DANS UNE STRUCTURE DO DO DO...LOOP LOOP LOOP");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dbut de boucle conditionnelle
//
mtBOOL CompBEGIN(void)
{
   PushMark(pCFIC->wHERE);
   pCFIC->NbBEGIN++;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de boucle infinie BEGIN...AGAIN.
//
mtBOOL CompAGAIN(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbBEGIN)
   {
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         _Compile((pFUNC) _ExecAGAIN);
         _Compile((pFUNC) wSaut);
         pCFIC->NbBEGIN--;
      }
      else
      {
         WinInfo("BEGIN MANQUANT (BEGIN...AGAIN)");
         WriteLogNeo("BEGIN MANQUANT (BEGIN...AGAIN)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("BEGIN MANQUANT (BEGIN...AGAIN)");
      WriteLogNeo("BEGIN MANQUANT (BEGIN...AGAIN)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de la boucle BEGIN ... UNTIL
//
mtBOOL CompUNTIL(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbBEGIN)
   {
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         _Compile((pFUNC) _ExecUNTIL);
         _Compile((pFUNC) wSaut);
         pCFIC->NbBEGIN--;
      }
      else
      {
         WinInfo("BEGIN MANQUANT (BEGIN...UNTIL)");
         WriteLogNeo("BEGIN MANQUANT (BEGIN...UNTIL)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("BEGIN MANQUANT (BEGIN...UNTIL)");
      WriteLogNeo("BEGIN MANQUANT (BEGIN...UNTIL)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Alternative de la boucle BEGIN ... WHILE ... REPEAT
//
mtBOOL CompWHILE(void)
{
   _Compile((pFUNC) _ExecWHILE);
   PushMark(pCFIC->wHERE);
   pCFIC->wHERE++;
   _CheckBlockSize();
   pCFIC->NbWHILE++;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de la boucle BEGIN ... WHILE ... REPEAT
//
mtBOOL CompREPEAT(void)
{
   mtUWORD wSWhile,wSBegin;

   if(pCFIC->NbBEGIN && pCFIC->NbWHILE)
   {
      wSWhile = PopMark();
      wSBegin = PopMark();
      if((wSWhile && wSBegin) && (wSWhile != UINT_MAX) && (wSBegin != UINT_MAX))
      {
         _Compile((pFUNC) _ExecREPEAT);
         _Compile((pFUNC) wSBegin);
         // On rsout le branchement du WHILE
         pCFIC->pCApp->pHere[wSWhile] = (pFUNC) pCFIC->wHERE;
         pCFIC->NbWHILE--;
         pCFIC->NbBEGIN--;
      }
      else
      {
         WinInfo("BEGIN ET/OU WHILE MANQUANT (BEGIN...WHILE...REPEAT)");
         WriteLogNeo("BEGIN ET/OU WHILE MANQUANT (BEGIN...WHILE...REPEAT)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("BEGIN ET/OU WHILE MANQUANT (BEGIN...WHILE...REPEAT)");
      WriteLogNeo("BEGIN ET/OU WHILE MANQUANT (BEGIN...WHILE...REPEAT)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dbut de slection multiple CASE ... OF ... ENDOF ... ENDCASE
//
mtBOOL CompCASE(void)
{
//   WinInfo("COMPCASE wHERE=%d",pCFIC->wHERE);
   PushMark(HMark);
   pCFIC->NbCASE++;
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dbut de slection CASE ... OF ... ENDOF ... ENDCASE
//
mtBOOL CompOF(void)
{
   if(pCFIC->NbCASE)
   {
      _Compile((pFUNC) _Of);
      _Compile((pFUNC) _0Branch);
      PushMark(pCFIC->wHERE);
      pCFIC->wHERE++;
      _CheckBlockSize();
      // Pour liminer la valeur … tester.
      _Compile((pFUNC) _PDrop);
      pCFIC->NbOF++;
   }
   else
   {
      WinInfo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
      WriteLogNeo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de slection CASE ... OF ... ENDOF ... ENDCASE
//
mtBOOL CompENDOF(void)
{
   mtUWORD wOF;

   if(pCFIC->NbCASE && pCFIC->NbOF)
   {
      // On rcupre le branchement du OF ...
      wOF = PopMark();
      if(wOF != UINT_MAX)
      {
         _Compile((pFUNC) _ExecENDOF);
         PushMark(pCFIC->wHERE);
         pCFIC->wHERE++;
         _CheckBlockSize();
         // On rsout le branchement du OF ...
         pCFIC->pCApp->pHere[wOF] = (pFUNC) pCFIC->wHERE;
         pCFIC->NbOF--;
      }
      else
      {
         WinInfo("CASE ET/OU OF MANQUANT (CASE OF...ENDOF ENDCASE)");
         WriteLogNeo("CASE ET/OU OF MANQUANT (CASE OF...ENDOF ENDCASE)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("CASE ET/OU OF MANQUANT (CASE OF...ENDOF ENDCASE)");
      WriteLogNeo("CASE ET/OU OF MANQUANT (CASE OF...ENDOF ENDCASE)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de slection multiple CASE ... OF ... ENDOF ... ENDCASE
//
mtBOOL CompENDCASE(void)
{
   mtUWORD wSaut;

   if(pCFIC->NbCASE)
   {
      // On rcupre le branchement du 1er ENDOF ...
      wSaut = PopMark();
      if(wSaut != UINT_MAX)
      {
         while(wSaut != HMark)
         {
            // On rsout le branchement du ENDOF courant
            pCFIC->pCApp->pHere[wSaut] = (pFUNC) pCFIC->wHERE;
            wSaut = PopMark();
         }
         pCFIC->NbCASE--;
      }
      else
      {
         WinInfo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
         WriteLogNeo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
      WriteLogNeo("CASE MANQUANT (CASE OF...ENDOF ENDCASE)");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile la demande de position du curseur.
//
mtBOOL CompGETCPOS(void)
{
   _Compile((pFUNC) _GetCursor);
   _Compile((pFUNC) _GetCursorPos);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'une touche de fonction … la table globale.
//
mtBOOL CompASGFKEY(void)
{
   mtPTRBYTE cTouche,cProc,cRet;
   mtBOOL bDi=mtFALSE,bEt=mtFALSE,bRe=mtTRUE;
   mtBYTE bTch=0,cTmp[2];
   mtWORD wPortee;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cTouche = strtok(NULL,cDelim);      // Rcupre le code de la touche.
   if(cTouche)
   {
      strupr(cTouche);
      cProc = strtok(NULL,cDelim);
      if(cProc)
      {
         strupr(cProc);
         cRet = strtok(NULL,cDelim);
         if(cRet)
         {
            strupr(cRet);
            if(strstr(cRet,"NORETURN"))
               bRe = mtFALSE;
         }
         if(*cTouche == '*')
         {
            bEt = mtTRUE;
            bDi = mtFALSE;
            cTouche++;
         }
         if(*cTouche == '#')
         {
            bEt = mtFALSE;
            bDi = mtTRUE;
            cTouche++;
         }
         if((strcmp(cTouche,"FKENVOI")) == 0)
            bTch = MEnvoi;
         if((strcmp(cTouche,"FKRETOUR")) == 0)
            bTch = MRetour;
         if((strcmp(cTouche,"FKREPETITION")) == 0)
            bTch = MRepetition;
         if((strcmp(cTouche,"FKGUIDE")) == 0)
            bTch = MGuide;
         if((strcmp(cTouche,"FKANNULATION")) == 0)
            bTch = MAnnulation;
         if((strcmp(cTouche,"FKSOMMAIRE")) == 0)
            bTch = MSommaire;
         if((strcmp(cTouche,"FKCORRECTION")) == 0)
            bTch = MCorrection;
         if((strcmp(cTouche,"FKSUITE")) == 0)
            bTch = MSuite;
         if(bTch)
         {
            pDSym = SearchSymb(cProc,&wPortee,&wPos);
            if(pDSym)
            {
               if(pDSym->Type == mt2POINTS)
               {
                  cTmp[0] = bTch;
                  CompileChaine(cTmp);
                  if(bDi)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  if(bEt)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  if(bRe)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  _Compile((pFUNC) _Literal);
                  _Compile((pFUNC) pDSym);
                  _Compile((pFUNC) _SetGFKey);
               }
               else
               {
                  WinInfo("Type de symbole incompatible");
                  WriteLogNeo("Type de symbole incompatible");
                  return(mtTRUE);
               }
            }
            else
            {
               WinInfo("Symbole inconnu");
               WriteLogNeo("Symbole inconnu");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Code de touche inconnu");
            WriteLogNeo("Code de touche inconnu");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nom de la procdure manquant");
         WriteLogNeo("Nom de la procdure manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Code de la touche manquant");
      WriteLogNeo("Code de la touche manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'un mot-clef la table globale.
//
mtBOOL CompASGKEYW(void)
{
   mtPTRBYTE cKeyW,cProc,cRet;
   mtWORD wPortee;
   mtBOOL bRe=mtTRUE;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cKeyW = strtok(NULL,cDelim);        // Rcupre le mot-clef.
   if(cKeyW)
   {
      cProc = strtok(NULL,cDelim);
      if(cProc)
      {
         strupr(cProc);
         cRet = strtok(NULL,cDelim);
         if(cRet)
         {
            strupr(cRet);
            if(strstr(cRet,"NORETURN"))
               bRe = mtFALSE;
         }
         pDSym = SearchSymb(cProc,&wPortee,&wPos);
         if(pDSym)
         {
            if(pDSym->Type == mt2POINTS)
            {
               CompileChaine(cKeyW);
               if(bRe)
                  CompileNombre(1);
               else
                  CompileNombre(0);
               _Compile((pFUNC) _Literal);
               _Compile((pFUNC) pDSym);
               _Compile((pFUNC) _SetGKeyW);
            }
            else
            {
               WinInfo("Type de symbole incompatible");
               WriteLogNeo("Type de symbole incompatible");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Symbole inconnu");
            WriteLogNeo("Symbole inconnu");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nom de la procdure manquant");
         WriteLogNeo("Nom de la procdure manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("mot-clef manquant");
      WriteLogNeo("mot-clef manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'une touche de fonction … la table locale.
//
mtBOOL CompASLFKEY(void)
{
   mtPTRBYTE cTouche,cProc,cRet;
   mtBOOL bDi=mtFALSE,bEt=mtFALSE,bRe=mtTRUE;
   mtBYTE bTch=0,cTmp[2];
   mtWORD wPortee;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cTouche = strtok(NULL,cDelim);      // Rcupre le code de la touche.
   if(cTouche)
   {
      strupr(cTouche);
      cProc = strtok(NULL,cDelim);
      if(cProc)
      {
         strupr(cProc);
         cRet = strtok(NULL,cDelim);
         if(cRet)
         {
            strupr(cRet);
            if(strstr(cRet,"NORETURN"))
               bRe = mtFALSE;
         }
         if(*cTouche == '*')
         {
            bEt = mtTRUE;
            bDi = mtFALSE;
            cTouche++;
         }
         if(*cTouche == '#')
         {
            bEt = mtFALSE;
            bDi = mtTRUE;
            cTouche++;
         }
         if((strcmp(cTouche,"FKENVOI")) == 0)
            bTch = MEnvoi;
         if((strcmp(cTouche,"FKRETOUR")) == 0)
            bTch = MRetour;
         if((strcmp(cTouche,"FKREPETITION")) == 0)
            bTch = MRepetition;
         if((strcmp(cTouche,"FKGUIDE")) == 0)
            bTch = MGuide;
         if((strcmp(cTouche,"FKANNULATION")) == 0)
            bTch = MAnnulation;
         if((strcmp(cTouche,"FKSOMMAIRE")) == 0)
            bTch = MSommaire;
         if((strcmp(cTouche,"FKCORRECTION")) == 0)
            bTch = MCorrection;
         if((strcmp(cTouche,"FKSUITE")) == 0)
            bTch = MSuite;
         if(bTch)
         {
            pDSym = SearchSymb(cProc,&wPortee,&wPos);
            if(pDSym)
            {
               if(pDSym->Type == mt2POINTS)
               {
                  cTmp[0] = bTch;
                  CompileChaine(cTmp);
                  if(bDi)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  if(bEt)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  if(bRe)
                     CompileNombre(1);
                  else
                     CompileNombre(0);
                  _Compile((pFUNC) _Literal);
                  _Compile((pFUNC) pDSym);
                  _Compile((pFUNC) _SetLFKey);
               }
               else
               {
                  WinInfo("Type de symbole incompatible");
                  WriteLogNeo("Type de symbole incompatible");
                  return(mtTRUE);
               }
            }
            else
            {
               WinInfo("Symbole inconnu");
               WriteLogNeo("Symbole inconnu");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Code de touche inconnu");
            WriteLogNeo("Code de touche inconnu");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nom de la procdure manquant");
         WriteLogNeo("Nom de la procdure manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Code de la touche manquant");
      WriteLogNeo("Code de la touche manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'un mot-clef la table locale.
//
mtBOOL CompASLKEYW(void)
{
   mtPTRBYTE cKeyW,cProc,cRet;
   mtWORD wPortee;
   mtBOOL bRe=mtTRUE;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cKeyW = strtok(NULL,cDelim);        // Rcupre le mot-clef.
   if(cKeyW)
   {
      cProc = strtok(NULL,cDelim);
      if(cProc)
      {
         strupr(cProc);
         cRet = strtok(NULL,cDelim);
         if(cRet)
         {
            strupr(cRet);
            if(strstr(cRet,"NORETURN"))
               bRe = mtFALSE;
         }
         pDSym = SearchSymb(cProc,&wPortee,&wPos);
         if(pDSym)
         {
            if(pDSym->Type == mt2POINTS)
            {
               CompileChaine(cKeyW);
               if(bRe)
                  CompileNombre(1);
               else
                  CompileNombre(0);
               _Compile((pFUNC) _Literal);
               _Compile((pFUNC) pDSym);
               _Compile((pFUNC) _SetLKeyW);
            }
            else
            {
               WinInfo("Type de symbole incompatible");
               WriteLogNeo("Type de symbole incompatible");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("Symbole inconnu");
            WriteLogNeo("Symbole inconnu");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Nom de la procdure manquant");
         WriteLogNeo("Nom de la procdure manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("mot-clef manquant");
      WriteLogNeo("mot-clef manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation globale de touche.
//
mtBOOL CompDEGFKEY(void)
{
   mtPTRBYTE cTouche,cProc;
   mtBOOL bDi=mtFALSE,bEt=mtFALSE;
   mtBYTE bTch=0,cTmp[2];

   cTouche = strtok(NULL,cDelim);      // Rcupre le code de la touche.
   if(cTouche)
   {
      strupr(cTouche);
      if(*cTouche == '*')
      {
         bEt = mtTRUE;
         bDi = mtFALSE;
         cTouche++;
      }
      if(*cTouche == '#')
      {
         bEt = mtFALSE;
         bDi = mtTRUE;
         cTouche++;
      }
      if((strcmp(cTouche,"FKENVOI")) == 0)
         bTch = MEnvoi;
      if((strcmp(cTouche,"FKRETOUR")) == 0)
         bTch = MRetour;
      if((strcmp(cTouche,"FKREPETITION")) == 0)
         bTch = MRepetition;
      if((strcmp(cTouche,"FKGUIDE")) == 0)
         bTch = MGuide;
      if((strcmp(cTouche,"FKANNULATION")) == 0)
         bTch = MAnnulation;
      if((strcmp(cTouche,"FKSOMMAIRE")) == 0)
         bTch = MSommaire;
      if((strcmp(cTouche,"FKCORRECTION")) == 0)
         bTch = MCorrection;
      if((strcmp(cTouche,"FKSUITE")) == 0)
         bTch = MSuite;
      if(bTch)
      {
         cTmp[0] = bTch;
         CompileChaine(cTmp);
         if(bDi)
            CompileNombre(1);
         else
            CompileNombre(0);
         if(bEt)
            CompileNombre(1);
         else
            CompileNombre(0);
         _Compile((pFUNC) _DelUneGFKey);
      }
      else
      {
         WinInfo("Code de touche inconnu");
         WriteLogNeo("Code de touche inconnu");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Code de la touche manquant");
      WriteLogNeo("Code de la touche manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation locale de touche.
//
mtBOOL CompDELFKEY(void)
{
   mtPTRBYTE cTouche,cProc;
   mtBOOL bDi=mtFALSE,bEt=mtFALSE;
   mtBYTE bTch=0,cTmp[2];

   cTouche = strtok(NULL,cDelim);      // Rcupre le code de la touche.
   if(cTouche)
   {
      strupr(cTouche);
      if(*cTouche == '*')
      {
         bEt = mtTRUE;
         bDi = mtFALSE;
         cTouche++;
      }
      if(*cTouche == '#')
      {
         bEt = mtFALSE;
         bDi = mtTRUE;
         cTouche++;
      }
      if((strcmp(cTouche,"FKENVOI")) == 0)
         bTch = MEnvoi;
      if((strcmp(cTouche,"FKRETOUR")) == 0)
         bTch = MRetour;
      if((strcmp(cTouche,"FKREPETITION")) == 0)
         bTch = MRepetition;
      if((strcmp(cTouche,"FKGUIDE")) == 0)
         bTch = MGuide;
      if((strcmp(cTouche,"FKANNULATION")) == 0)
         bTch = MAnnulation;
      if((strcmp(cTouche,"FKSOMMAIRE")) == 0)
         bTch = MSommaire;
      if((strcmp(cTouche,"FKCORRECTION")) == 0)
         bTch = MCorrection;
      if((strcmp(cTouche,"FKSUITE")) == 0)
         bTch = MSuite;
      if(bTch)
      {
         cTmp[0] = bTch;
         CompileChaine(cTmp);
         if(bDi)
            CompileNombre(1);
         else
            CompileNombre(0);
         if(bEt)
            CompileNombre(1);
         else
            CompileNombre(0);
         _Compile((pFUNC) _DelUneLFKey);
      }
      else
      {
         WinInfo("Code de touche inconnu");
         WriteLogNeo("Code de touche inconnu");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Code de la touche manquant");
      WriteLogNeo("Code de la touche manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation globale de mot-clef.
//
mtBOOL CompDEGKEYW(void)
{
   mtPTRBYTE cKeyW;

   cKeyW = strtok(NULL,cDelim);        // Rcupre le mot-clef.
   if(cKeyW)
   {
      CompileChaine(cKeyW);
      _Compile((pFUNC) _DelUnGKeyW);
   }
   else
   {
      WinInfo("mot-clef manquant");
      WriteLogNeo("mot-clef manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation locale de mot-clef.
//
mtBOOL CompDELKEYW(void)
{
   mtPTRBYTE cKeyW;

   cKeyW = strtok(NULL,cDelim);        // Rcupre le mot-clef.
   if(cKeyW)
   {
      CompileChaine(cKeyW);
      _Compile((pFUNC) _DelUnLKeyW);
   }
   else
   {
      WinInfo("mot-clef manquant");
      WriteLogNeo("mot-clef manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation du rpertoire principal.
//
mtBOOL CompPATH(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le chemin.
      if(pCh)
      {
         // On ajoute l'antislash … la fin.
         if(*(pCh+(strlen(pCh)-1)) != 0x5C)
            strcat(pCh,"\x5C");
         if(pCFIC->pCApp->Path)
            pCFIC->pCApp->Path = (mtPTRBYTE) MemReAlloc(pCFIC->pCApp->Path,strlen(pCh)+1);
         else
            pCFIC->pCApp->Path = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
         strcpy(pCFIC->pCApp->Path,pCh);
      }
      else
      {
         WinInfo("chemin manquant");
         WriteLogNeo("chemin manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation du rpertoire des bases de donnes.
//
mtBOOL CompPATHDBF(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le chemin.
      if(pCh)
      {
         // On ajoute l'antislash … la fin.
         if(*(pCh+(strlen(pCh)-1)) != 0x5C)
            strcat(pCh,"\x5C");
         if(pCFIC->pCApp->PathDBF)
            pCFIC->pCApp->PathDBF = (mtPTRBYTE) MemReAlloc(pCFIC->pCApp->PathDBF,strlen(pCh)+1);
         else
            pCFIC->pCApp->PathDBF = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
         strcpy(pCFIC->pCApp->PathDBF,pCh);
      }
      else
      {
         WinInfo("chemin manquant");
         WriteLogNeo("chemin manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation du rpertoire des pages vidotex.
//
mtBOOL CompPATHVTX(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le chemin.
      if(pCh)
      {
         // On ajoute l'antislash … la fin.
         if(*(pCh+(strlen(pCh)-1)) != 0x5C)
            strcat(pCh,"\x5C");
         if(pCFIC->pCApp->PathVTX)
            pCFIC->pCApp->PathVTX = (mtPTRBYTE) MemReAlloc(pCFIC->pCApp->PathVTX,strlen(pCh)+1);
         else
            pCFIC->pCApp->PathVTX = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
         strcpy(pCFIC->pCApp->PathVTX,pCh);
      }
      else
      {
         WinInfo("chemin manquant");
         WriteLogNeo("chemin manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de la taille des piles d'une application.
//
mtBOOL CompSTKSIZE(void)
{
   mtPTRBYTE pCh;
   float dTaille;
   mtUWORD wTaille;

   pCh = strtok(NULL,cDelim);          // Rcupre la taille.
   if(pCh)
   {
      if(IsNumber(pCh,&dTaille))
      {
         wTaille = dTaille;
         if(wTaille)
            pCFIC->pCApp->StackSize = wTaille;
         else
         {
            WinInfo("STACKSIZE: La taille donne ne peut tre nulle");
            WriteLogNeo("STACKSIZE: La taille donne ne peut tre nulle");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("STACKSIZE: Taille invalide");
         WriteLogNeo("STACKSIZE: Taille invalide");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("STACKSIZE: Taille manquante");
      WriteLogNeo("STACKSIZE: Taille manquante");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Changement du nom de l'application courante.
//
mtBOOL CompAPPNAME(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le nom.
      if(pCh)
      {
         strupr(pCh);
         if(! SearchApp(pCh,0))
         {
            memset(pCFIC->pCApp->Nom,0,11);
            strncpy(pCFIC->pCApp->Nom,pCh,10);
         }
         else
         {
            WinInfo("%s existe dj…",pCh);
            WriteLogNeo("%s existe dj…",pCh);
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("nom manquant");
         WriteLogNeo("nom manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition du numro complmentaire.
//
mtBOOL CompNUMCOMP(void)
{
   mtPTRBYTE pCh;
   mtWORD iVal;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le numro.
      if(pCh)
      {
         iVal = atoi(pCh);
         if(iVal>=1 && iVal<=99)
            pCFIC->pCApp->NumComp = (mtBYTE) iVal;
         else
         {
            WinInfo("%d: numro complmentaire invalide",iVal);
            WriteLogNeo("%d: numro complmentaire invalide",iVal);
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("nom manquant");
         WriteLogNeo("nom manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction DELAY.
//
mtBOOL CompDELAY(void)
{
   _Compile((pFUNC) _InitDelay);
   _Compile((pFUNC) _Delay);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction AFILL.
//
mtBOOL CompAFILL(void)
{
   _Compile((pFUNC) _InitAfill);
   _Compile((pFUNC) _AFill);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction ASEARCH.
//
mtBOOL CompASEARCH(void)
{
   _Compile((pFUNC) _InitAsearch);
   _Compile((pFUNC) _ASearch);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction AINS.
//
mtBOOL CompAINS(void)
{
   _Compile((pFUNC) _AIns);
   _Compile((pFUNC) _TransTab);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction ADEL.
//
mtBOOL CompADEL(void)
{
   _Compile((pFUNC) _ADel);
   _Compile((pFUNC) _TransTab);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction ACOPY.
//
mtBOOL CompACOPY(void)
{
   _Compile((pFUNC) _ACopy);
   _Compile((pFUNC) _CopyTab);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction ACLONE.
//
mtBOOL CompACLONE(void)
{
   _Compile((pFUNC) _AClone);
   _Compile((pFUNC) _CopyTab);
   _Compile((pFUNC) _DeleteTab);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction STRTOK.
//
mtBOOL CompSTRTOK(void)
{
   _Compile((pFUNC) _StrTok);
   _Compile((pFUNC) _DeleteTab);
   _Compile((pFUNC) _DecoupeChaine);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction ADIR.
//
mtBOOL CompADIR(void)
{
   _Compile((pFUNC) _ADir);
   _Compile((pFUNC) _DeleteTab);
   _Compile((pFUNC) __CountADirFiles);
   _Compile((pFUNC) _ADirTab);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de la deconnexion sur erreur.
//
mtBOOL CompLIBERR(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le flag ('ON|OFF')
      if(pCh)
      {
         strupr(pCh);
         if(strcmp(pCh,"ON") == 0)
            pCFIC->pCApp->bLIBErr = mtTRUE;
         else
            pCFIC->pCApp->bLIBErr = mtFALSE;
      }
      else
      {
         WinInfo("ON | OFF manquant");
         WriteLogNeo("ON | OFF manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de la confirmation des appels entrants (X25).
//
mtBOOL CompCONFCNX(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le flag ('ON|OFF')
      if(pCh)
      {
         strupr(pCh);
         if(strcmp(pCh,"ON") == 0)
            pCFIC->pCApp->bLIBErr = mtTRUE;
         else
            pCFIC->pCApp->bLIBErr = mtFALSE;
      }
      else
      {
         WinInfo("ON | OFF manquant");
         WriteLogNeo("ON | OFF manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de l'affichage des warnings … la compilation.
//
mtBOOL CompWARNING(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le flag ('ON|OFF')
      if(pCh)
      {
         strupr(pCh);
         if(strcmp(pCh,"ON") == 0)
            pCFIC->bWarning = mtTRUE;
         else
            pCFIC->bWarning = mtFALSE;
      }
      else
      {
         WinInfo("ON | OFF manquant");
         WriteLogNeo("ON | OFF manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de l'affichage des lignes du source courant.
//
mtBOOL CompSETECHO(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le flag ('ON|OFF')
      if(pCh)
      {
         strupr(pCh);
         if(strcmp(pCh,"ON") == 0)
            pCFIC->bEcho = mtTRUE;
         else
            pCFIC->bEcho = mtFALSE;
      }
      else
      {
         WinInfo("ON | OFF manquant");
         WriteLogNeo("ON | OFF manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage du raffraichissement automatique des bases de l'application.
//
mtBOOL CompSETACMIT(void)
{
   mtPTRBYTE pCh;
   mtUWORD wDelai;
   float dDelai;

   pCh = strtok(NULL,cDelim);          // Rcupre la taille.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le dlai.
      if(pCh)
      {
         if(IsNumber(pCh,&dDelai))
         {
            wDelai = dDelai;
            if(wDelai)
            {
               pCFIC->pCApp->bAutoCommit = mtTRUE;
               pCFIC->pCApp->wCMIT       = wDelai*60;
            }
            else
            {
               WinInfo("DBAUTOCOMMIT: Le dlai donn ne peut tre nul");
               WriteLogNeo("DBAUTOCOMMIT: Le dlai donn ne peut tre nul");
               return(mtTRUE);
            }
         }
         else
         {
            WinInfo("DBAUTOCOMMIT: dlai invalide");
            WriteLogNeo("DBAUTOCOMMIT: dlai invalide");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("dlai manquant");
         WriteLogNeo("dlai manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Paramtrage de l'anticipation.
//
mtBOOL CompANTICIPE(void)
{
   mtPTRBYTE pCh;

   pCh = strtok(NULL,cDelim);          // Rcupre le '='.
   if(pCh)
   {
      // On regarde si on a '=...'
      if(strlen(pCh) > 1)
      {
         // On passe le '='.
         if(*pCh == '=')
            pCh++;
      }
      else
         // On a '= ...'
         pCh = strtok(NULL,cDelim);    // On rcupre le flag ('ON|OFF')
      if(pCh)
      {
         strupr(pCh);
         if(strcmp(pCh,"ON") == 0)
            pCFIC->pCApp->bAnticipe = mtTRUE;
         else
            pCFIC->pCApp->bAnticipe = mtFALSE;
      }
      else
      {
         WinInfo("ON | OFF manquant");
         WriteLogNeo("ON | OFF manquant");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("'=' manquant");
      WriteLogNeo("'=' manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fonction WAITCHAR.
//
mtBOOL CompWAITCHAR(void)
{
   _Compile((pFUNC) _InitWaitChar);
   _Compile((pFUNC) _WaitChar);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile INPUTVAR.
//
mtBOOL CompINPUTVAR(void)
{
   _Compile((pFUNC) _InitInputVar);
   _Compile((pFUNC) _MInputVar);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compile MLINPUT.
//
mtBOOL CompMLINPUT(void)
{
   _Compile((pFUNC) _InitMLInput);
   _Compile((pFUNC) _MLInput);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition d'une tche locale.
//
mtBOOL CompSETLTASK(void)
{
   mtPTRBYTE cProc;
   mtWORD wPortee;
   mtPTRDICO pDSym;
   mtUWORD wPos;

   cProc = strtok(NULL,cDelim);
   if(cProc)
   {
      strupr(cProc);
      pDSym = SearchSymb(cProc,&wPortee,&wPos);
      if(pDSym)
      {
         if(pDSym->Type == mt2POINTS)
         {
            _Compile((pFUNC) _Literal);
            _Compile((pFUNC) pDSym);
            _Compile((pFUNC) _SetLTask);
         }
         else
         {
            WinInfo("Type de symbole incompatible");
            WriteLogNeo("Type de symbole incompatible");
            return(mtTRUE);
         }
      }
      else
      {
         WinInfo("Symbole inconnu");
         WriteLogNeo("Symbole inconnu");
         return(mtTRUE);
      }
   }
   else
   {
      WinInfo("Nom de la procdure manquant");
      WriteLogNeo("Nom de la procdure manquant");
      return(mtTRUE);
   }
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute un mot-clef au dictionnaire global.
//
void AddKeyWord(mtPTRBYTE KeyWord,pFUNC pExec,mtBYTE fType)
{
   mtPTRDICO pDic;

   pDic         = NewDicEntry(&pGlobalDic,&uGDico,KeyWord,fType,mtTRUE);
   pDic->V.pVal = pExec;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation du dictionnaire global.
//
void InitGlobalDic(void)
{
   //
   // Cration des constantes du Kernel.
   //
   pCONSTTRUE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTTRUE->Type   = mtTNUMBER;
   pCONSTTRUE->V.fVal = mtTRUE;
   pCONSTTRUE->Attr   = SYMLITERAL;

   pCONSTFALSE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFALSE->Type   = mtTNUMBER;
   pCONSTFALSE->V.fVal = mtFALSE;
   pCONSTFALSE->Attr   = SYMLITERAL;

   pCONSTLINE0         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTLINE0->Type   = mtTNUMBER;
   pCONSTLINE0->V.fVal = 0;
   pCONSTLINE0->Attr   = SYMLITERAL;

   pCONSTCOL1         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTCOL1->Type   = mtTNUMBER;
   pCONSTCOL1->V.fVal = 1;
   pCONSTCOL1->Attr   = SYMLITERAL;

   pCONSTFKENVOI            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKENVOI->Type      = mtTCHAR;
   pCONSTFKENVOI->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKENVOI->V.cVal) = MEnvoi;
   pCONSTFKENVOI->Attr      = SYMLITERAL;

   pCONSTFKRETOUR            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKRETOUR->Type      = mtTCHAR;
   pCONSTFKRETOUR->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKRETOUR->V.cVal) = MRetour;
   pCONSTFKRETOUR->Attr      = SYMLITERAL;

   pCONSTFKREPETITION            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKREPETITION->Type      = mtTCHAR;
   pCONSTFKREPETITION->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKREPETITION->V.cVal) = MRepetition;
   pCONSTFKREPETITION->Attr      = SYMLITERAL;

   pCONSTFKGUIDE            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKGUIDE->Type      = mtTCHAR;
   pCONSTFKGUIDE->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKGUIDE->V.cVal) = MGuide;
   pCONSTFKGUIDE->Attr      = SYMLITERAL;

   pCONSTFKANNULATION            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKANNULATION->Type      = mtTCHAR;
   pCONSTFKANNULATION->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKANNULATION->V.cVal) = MAnnulation;
   pCONSTFKANNULATION->Attr      = SYMLITERAL;

   pCONSTFKSOMMAIRE            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKSOMMAIRE->Type      = mtTCHAR;
   pCONSTFKSOMMAIRE->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKSOMMAIRE->V.cVal) = MSommaire;
   pCONSTFKSOMMAIRE->Attr      = SYMLITERAL;

   pCONSTFKCORRECTION            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKCORRECTION->Type      = mtTCHAR;
   pCONSTFKCORRECTION->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKCORRECTION->V.cVal) = MCorrection;
   pCONSTFKCORRECTION->Attr      = SYMLITERAL;

   pCONSTFKSUITE            = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFKSUITE->Type      = mtTCHAR;
   pCONSTFKSUITE->V.cVal    = (mtPTRBYTE) MemAlloc(2);
   *(pCONSTFKSUITE->V.cVal) = MSuite;
   pCONSTFKSUITE->Attr      = SYMLITERAL;

   pCONSTBLACK         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTBLACK->Type   = mtTNUMBER;
   pCONSTBLACK->V.fVal = 0;
   pCONSTBLACK->Attr   = SYMLITERAL;

   pCONSTRED         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTRED->Type   = mtTNUMBER;
   pCONSTRED->V.fVal = 1;
   pCONSTRED->Attr   = SYMLITERAL;

   pCONSTGREEN         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTGREEN->Type   = mtTNUMBER;
   pCONSTGREEN->V.fVal = 2;
   pCONSTGREEN->Attr   = SYMLITERAL;

   pCONSTYELLOW         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTYELLOW->Type   = mtTNUMBER;
   pCONSTYELLOW->V.fVal = 3;
   pCONSTYELLOW->Attr   = SYMLITERAL;

   pCONSTBLUE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTBLUE->Type   = mtTNUMBER;
   pCONSTBLUE->V.fVal = 4;
   pCONSTBLUE->Attr   = SYMLITERAL;

   pCONSTMAGENTA         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTMAGENTA->Type   = mtTNUMBER;
   pCONSTMAGENTA->V.fVal = 5;
   pCONSTMAGENTA->Attr   = SYMLITERAL;

   pCONSTCYAN         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTCYAN->Type   = mtTNUMBER;
   pCONSTCYAN->V.fVal = 6;
   pCONSTCYAN->Attr   = SYMLITERAL;

   pCONSTWHITE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTWHITE->Type   = mtTNUMBER;
   pCONSTWHITE->V.fVal = 7;
   pCONSTWHITE->Attr   = SYMLITERAL;

   pCONSTFSIMPLE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTFSIMPLE->Type   = mtTNUMBER;
   pCONSTFSIMPLE->V.fVal = 0;
   pCONSTFSIMPLE->Attr   = SYMLITERAL;

   pCONSTDHAUTEUR         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTDHAUTEUR->Type   = mtTNUMBER;
   pCONSTDHAUTEUR->V.fVal = 1;
   pCONSTDHAUTEUR->Attr   = SYMLITERAL;

   pCONSTDLARGEUR         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTDLARGEUR->Type   = mtTNUMBER;
   pCONSTDLARGEUR->V.fVal = 2;
   pCONSTDLARGEUR->Attr   = SYMLITERAL;

   pCONSTDTAILLE         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTDTAILLE->Type   = mtTNUMBER;
   pCONSTDTAILLE->V.fVal = 3;
   pCONSTDTAILLE->Attr   = SYMLITERAL;

   pCONSTTMVDT         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTTMVDT->Type   = mtTNUMBER;
   pCONSTTMVDT->V.fVal = TMODE_VDT;
   pCONSTTMVDT->Attr   = SYMLITERAL;

   pCONSTTMMXT         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTTMMXT->Type   = mtTNUMBER;
   pCONSTTMMXT->V.fVal = TMODE_MIXTE;
   pCONSTTMMXT->Attr   = SYMLITERAL;

   pCONSTTMTEL         = (mtPTRDICO) MemAlloc(sizeof(Dico));
   pCONSTTMTEL->Type   = mtTNUMBER;
   pCONSTTMTEL->V.fVal = TMODE_TELINFO;
   pCONSTTMTEL->Attr   = SYMLITERAL;
   //
   // Le reste.
   //
   AddKeyWord("CLS",_Cls,mtKERNEL);
   AddKeyWord("STACK#",_NStack,mtKERNEL);
   AddKeyWord("ENQROM",_EnqRom,mtKERNEL);
   AddKeyWord("SETCOLOR",_SetColor,mtKERNEL);
   AddKeyWord("SETBACKGROUND",_SetBackGround,mtKERNEL);
   AddKeyWord("LOCATE",_Locate,mtKERNEL);
   AddKeyWord("LINE0",_Ligne0,mtKERNEL);
   AddKeyWord("CLEAR0",_Clear0,mtKERNEL);
   AddKeyWord("SETFONT",_SetFont,mtKERNEL);
   AddKeyWord("REPLICATE",_Replicate,mtKERNEL);
   AddKeyWord("SETTMODE",_SetTMode,mtKERNEL);
   AddKeyWord("SENDPAGE",_SendPage,mtKERNEL);
   AddKeyWord("EMIT",_Emit,mtKERNEL);
   AddKeyWord("USING",_Using,mtKERNEL);
   AddKeyWord("USINGOFF",_NoUsing,mtKERNEL);
   AddKeyWord("GETUSING",_GetUsing,mtKERNEL);
   AddKeyWord("PRINT",_Print,mtKERNEL);
   AddKeyWord("PRINTUSING",_PrintUsing,mtKERNEL);
   AddKeyWord("CR",_Cr,mtKERNEL);
   AddKeyWord("CURSORON",_CursorOn,mtKERNEL);
   AddKeyWord("CURSOROFF",_CursorOff,mtKERNEL);
   AddKeyWord("BEEP",_MBeep,mtKERNEL);
   AddKeyWord("BLINKON",_CligOn,mtKERNEL);
   AddKeyWord("BLINKOFF",_CligOff,mtKERNEL);
   AddKeyWord("UNDERLON",_SouligOn,mtKERNEL);
   AddKeyWord("UNDERLOFF",_SouligOff,mtKERNEL);
   AddKeyWord("INVERTON",_InverseOn,mtKERNEL);
   AddKeyWord("INVERTOFF",_InverseOff,mtKERNEL);
   AddKeyWord("UPPER",_MUpper,mtKERNEL);
   AddKeyWord("LOWER",_MLower,mtKERNEL);
   AddKeyWord("INPUT",_MInput,mtKERNEL);
   AddKeyWord("DUP",_PDup,mtKERNEL);
   AddKeyWord("SWAP",_PSwap,mtKERNEL);
   AddKeyWord("OVER",_POver,mtKERNEL);
   AddKeyWord("ROT",_PRot,mtKERNEL);
   AddKeyWord("NIP",_PNip,mtKERNEL);
   AddKeyWord("DROP",_PDrop,mtKERNEL);
   AddKeyWord("PICK",_PPick,mtKERNEL);
   AddKeyWord("==",_SEgal,mtKERNEL);
   AddKeyWord("<",_SInf,mtKERNEL);
   AddKeyWord(">",_SSup,mtKERNEL);
   AddKeyWord("!=",_SDiff,mtKERNEL);
   AddKeyWord("<=",_SInfEgal,mtKERNEL);
   AddKeyWord(">=",_SSupEgal,mtKERNEL);
   AddKeyWord("LIBCV",_LibCnx,mtKERNEL);
   AddKeyWord("@",_VFetch,mtKERNEL);
   AddKeyWord("SPACE",_Space,mtKERNEL);
   AddKeyWord("SPACES",_SpaceS,mtKERNEL);
   AddKeyWord("EMPTY?",_IsEmpty,mtKERNEL);
   AddKeyWord("DISPLAY",_DisplaySrv,mtKERNEL);
   AddKeyWord("EXECUTE",_ExecuteProc,mtKERNEL);
   AddKeyWord("EXECUTEAPP",_ExecuteApp,mtKERNEL);
   AddKeyWord("SEARCHAPP",_ChercheApp,mtKERNEL);
   AddKeyWord("EXECKBDCMD",_ExecKbdCmd,mtKERNEL);
   AddKeyWord("WAIT",_Wait,mtKERNEL);
   AddKeyWord("WAITEMPTYBUFFER",_WaitOutBuffer,mtKERNEL);
   AddKeyWord(":=",_VarStore,mtKERNEL);
   AddKeyWord("EMPTYLOCALFKEYTABLE",_DelLFKey,mtKERNEL);
   AddKeyWord("EMPTYGLOBALFKEYTABLE",_DelGFKey,mtKERNEL);
   AddKeyWord("EMPTYLOCALKEYWORDTABLE",_DelLKeyW,mtKERNEL);
   AddKeyWord("EMPTYGLOBALKEYWORDTABLE",_DelGKeyW,mtKERNEL);
   AddKeyWord("DISABLELOCALFKEY",_DisLFKey,mtKERNEL);
   AddKeyWord("DISABLEGLOBALFKEY",_DisGFKey,mtKERNEL);
   AddKeyWord("DISABLELOCALKEYWORD",_DisLKeyW,mtKERNEL);
   AddKeyWord("DISABLEGLOBALKEYWORD",_DisGKeyW,mtKERNEL);
   AddKeyWord("ENABLELOCALFKEY",_EnaLFKey,mtKERNEL);
   AddKeyWord("ENABLEGLOBALFKEY",_EnaGFKey,mtKERNEL);
   AddKeyWord("ENABLELOCALKEYWORD",_EnaLKeyW,mtKERNEL);
   AddKeyWord("ENABLEGLOBALKEYWORD",_EnaGKeyW,mtKERNEL);
   AddKeyWord("TRANSLATEOFF",_DisTrAcc,mtKERNEL);
   AddKeyWord("TRANSLATEON",_EnaTrAcc,mtKERNEL);
   AddKeyWord("FILTERINGOFF",_DisSeqEsc,mtKERNEL);
   AddKeyWord("FILTERINGON",_EnaSeqEsc,mtKERNEL);
   AddKeyWord("SETBINARYMODE",_EnaBinMode,mtKERNEL);
   AddKeyWord("SETNORMALMODE",_DisBinMode,mtKERNEL);
   AddKeyWord("&",_BAnd,mtKERNEL);
   AddKeyWord("|",_BOr,mtKERNEL);
   AddKeyWord("^",_BXor,mtKERNEL);
   AddKeyWord("&&",_LAnd,mtKERNEL);
   AddKeyWord("||",_LOr,mtKERNEL);
   AddKeyWord("!",_LNot,mtKERNEL);
   AddKeyWord("~",_BComp1,mtKERNEL);
   AddKeyWord("++",_Inc1,mtKERNEL);
   AddKeyWord("--",_Dec1,mtKERNEL);
   AddKeyWord("+=",_PlusEgal,mtKERNEL);
   AddKeyWord("-=",_MoinsEgal,mtKERNEL);
   AddKeyWord("*=",_MultEgal,mtKERNEL);
   AddKeyWord("/=",_DivEgal,mtKERNEL);
   AddKeyWord("%=",_ModEgal,mtKERNEL);
   AddKeyWord("&=",_BAndEgal,mtKERNEL);
   AddKeyWord("|=",_BOrEgal,mtKERNEL);
   AddKeyWord("^=",_BXorEgal,mtKERNEL);
   AddKeyWord("<<",_LeftShift,mtKERNEL);
   AddKeyWord(">>",_RightShift,mtKERNEL);
   AddKeyWord("<<=",_LShiftEgal,mtKERNEL);
   AddKeyWord(">>=",_RShiftEgal,mtKERNEL);
   AddKeyWord("+",_Plus,mtKERNEL);
   AddKeyWord("-",_Moins,mtKERNEL);
   AddKeyWord("*",_Mult,mtKERNEL);
   AddKeyWord("/",_Div,mtKERNEL);
   AddKeyWord("%",_Mod,mtKERNEL);
   AddKeyWord("ACOS",_ACos,mtKERNEL);
   AddKeyWord("ASIN",_ASin,mtKERNEL);
   AddKeyWord("ATAN",_ATan,mtKERNEL);
   AddKeyWord("ATAN2",_ATan2,mtKERNEL);
   AddKeyWord("CEIL",_Ceil,mtKERNEL);
   AddKeyWord("COS",_Cos,mtKERNEL);
   AddKeyWord("COSH",_Cosh,mtKERNEL);
   AddKeyWord("EXP",_Exp,mtKERNEL);
   AddKeyWord("EXP-1",_Expm1,mtKERNEL);
   AddKeyWord("ABS",_FAbs,mtKERNEL);
   AddKeyWord("NEGATE",_Negate,mtKERNEL);
   AddKeyWord("FLOOR",_Floor,mtKERNEL);
   AddKeyWord("HYPOT",_Hypot,mtKERNEL);
   AddKeyWord("LOG",_Log,mtKERNEL);
   AddKeyWord("LOG1+",_Log1p,mtKERNEL);
   AddKeyWord("LOG10",_Log10,mtKERNEL);
   AddKeyWord("POW",_Pow,mtKERNEL);
   AddKeyWord("POW10",_Pow10,mtKERNEL);
   AddKeyWord("RND",_Rnd,mtKERNEL);
   AddKeyWord("RANDOMIZE",_Randomize,mtKERNEL);
   AddKeyWord("SIN",_Sin,mtKERNEL);
   AddKeyWord("SINH",_Sinh,mtKERNEL);
   AddKeyWord("SQRT",_Sqrt,mtKERNEL);
   AddKeyWord("TAN",_Tan,mtKERNEL);
   AddKeyWord("TANH",_Tanh,mtKERNEL);
   AddKeyWord("INT",_FInt,mtKERNEL);
   AddKeyWord("STRCAT",_StrCat,mtKERNEL);
   AddKeyWord("STRLEN",_StrLen,mtKERNEL);
   AddKeyWord("STRLWR",_StrLower,mtKERNEL);
   AddKeyWord("STRUPR",_StrUpper,mtKERNEL);
   AddKeyWord("STRREV",_StrRev,mtKERNEL);
   AddKeyWord("AT",_StrAt,mtKERNEL);
   AddKeyWord("SUBSTR",_StrSubstr,mtKERNEL);
   AddKeyWord("VAL",_StrVal,mtKERNEL);
   AddKeyWord("STR",_StrNum,mtKERNEL);
   AddKeyWord("RTRIM",_StrRTrim,mtKERNEL);
   AddKeyWord("LTRIM",_StrLTrim,mtKERNEL);
   AddKeyWord("ALLTRIM",_StrAllTrim,mtKERNEL);
   AddKeyWord("ASC",_StrAscii,mtKERNEL);
   AddKeyWord("CHR",_StrChr,mtKERNEL);
   AddKeyWord("STRSPN",_StrSpn,mtKERNEL);
   AddKeyWord("STRIN",_StrPbrk,mtKERNEL);
   AddKeyWord("ENCODE",_StrEncode,mtKERNEL);
   AddKeyWord("STRTRAN",_StrTran,mtKERNEL);
   AddKeyWord("LEFT",_StrLeft,mtKERNEL);
   AddKeyWord("RIGHT",_StrRight,mtKERNEL);
   AddKeyWord("PADL",_StrPadL,mtKERNEL);
   AddKeyWord("PADR",_StrPadR,mtKERNEL);
   AddKeyWord("PADC",_StrPadC,mtKERNEL);
   AddKeyWord("STRINS",_StrIns,mtKERNEL);
   AddKeyWord("STRDEL",_StrDel,mtKERNEL);
   AddKeyWord("STRSET",_StrSet,mtKERNEL);
   AddKeyWord("STRMATCH",_StrMatch,mtKERNEL);
   AddKeyWord("TRUE",_PushTRUE,mtKERNEL);
   AddKeyWord("FALSE",_PushFALSE,mtKERNEL);
   AddKeyWord("FKENVOI",_SysFKEnvoi,mtKERNEL);
   AddKeyWord("FKRETOUR",_SysFKRetour,mtKERNEL);
   AddKeyWord("FKREPETITION",_SysFKRepetition,mtKERNEL);
   AddKeyWord("FKGUIDE",_SysFKGuide,mtKERNEL);
   AddKeyWord("FKANNULATION",_SysFKAnnulation,mtKERNEL);
   AddKeyWord("FKSOMMAIRE",_SysFKSommaire,mtKERNEL);
   AddKeyWord("FKCORRECTION",_SysFKCorrection,mtKERNEL);
   AddKeyWord("FKSUITE",_SysFKSuite,mtKERNEL);
   AddKeyWord("BLACK",_SysBlack,mtKERNEL);
   AddKeyWord("RED",_SysRed,mtKERNEL);
   AddKeyWord("GREEN",_SysGreen,mtKERNEL);
   AddKeyWord("YELLOW",_SysYellow,mtKERNEL);
   AddKeyWord("BLUE",_SysBlue,mtKERNEL);
   AddKeyWord("MAGENTA",_SysMagenta,mtKERNEL);
   AddKeyWord("CYAN",_SysCyan,mtKERNEL);
   AddKeyWord("WHITE",_SysWhite,mtKERNEL);
   AddKeyWord("FSIMPLE",_SysFSimple,mtKERNEL);
   AddKeyWord("FDHEIGHT",_SysDHauteur,mtKERNEL);
   AddKeyWord("FDWEIGHT",_SysDLargeur,mtKERNEL);
   AddKeyWord("FDSIZE",_SysDTaille,mtKERNEL);
   AddKeyWord("TMODEVDT",_SysTModeVDT,mtKERNEL);
   AddKeyWord("TMODEMIXTE",_SysTModeMXT,mtKERNEL);
   AddKeyWord("TMODETELEINFO",_SysTModeTEL,mtKERNEL);
   AddKeyWord("GETTMODE",_SysGetTMode,mtKERNEL);
   AddKeyWord("FKEY",_SysMTouche,mtKERNEL);
   AddKeyWord("DATA",_SysMData,mtKERNEL);
   AddKeyWord("TIME",_SysTime,mtKERNEL);
   AddKeyWord("TIMER",_SysTimer,mtKERNEL);
   AddKeyWord("DATE",_SysDate,mtKERNEL);
   AddKeyWord("CNXDATA",_SysMTrameCNX,mtKERNEL);
   AddKeyWord("DEPT",_SysDept,mtKERNEL);
   AddKeyWord("PAVI",_SysPavi,mtKERNEL);
   AddKeyWord("PALIER",_SysPalier,mtKERNEL);
   AddKeyWord("NCOMP",_SysNComp,mtKERNEL);
   AddKeyWord("APP",_SysNApp,mtKERNEL);
   AddKeyWord("CNXDATE",_SysCnxDate,mtKERNEL);
   AddKeyWord("CNXTIME",_SysCnxTime,mtKERNEL);
   AddKeyWord("MTYPE",_SysMType,mtKERNEL);
   AddKeyWord("NUMCV",_SysNCV,mtKERNEL);
   AddKeyWord("SRVLAUNCHDATE",_SysSrvDLanc,mtKERNEL);
   AddKeyWord("SRVLAUNCHTIME",_SysSrvHLanc,mtKERNEL);
   AddKeyWord("SRVMEMAVAIL",_SysSrvMemAvail,mtKERNEL);
   AddKeyWord("SRVCHARGE",_SysSrvChargeA,mtKERNEL);
   AddKeyWord("SRVCHARGEMAX",_SysSrvChargeM,mtKERNEL);
   AddKeyWord("SRVNBCNX",_SysSrvNbCnx,mtKERNEL);
   AddKeyWord("SRVMAXCNX",_SysSrvPointeCnx,mtKERNEL);
   AddKeyWord("SRVSTOPTIME",_SysSrvHFinServ,mtKERNEL);
   AddKeyWord("DBOPEN",_DbOpen,mtKERNEL);
   AddKeyWord("DBCLOSE",_DbClose,mtKERNEL);
   AddKeyWord("DBCLOSEALL",_DbCloseAll,mtKERNEL);
   AddKeyWord("DBAPPENDBLANK",_DbAppendBlank,mtKERNEL);
   AddKeyWord("DBBOF",_DbBof,mtKERNEL);
   AddKeyWord("DBBOTTOM",_DbBottom,mtKERNEL);
   AddKeyWord("DBSELECT",_DbSelect,mtKERNEL);
   AddKeyWord("DBSELECTED",_DbSelected,mtKERNEL);
   AddKeyWord("DBSETALIAS",_DbSetAlias,mtKERNEL);
   AddKeyWord("DBDELETE",_DbDelete,mtKERNEL);
   AddKeyWord("DBDELETED",_DbDeleted,mtKERNEL);
   AddKeyWord("DBEOF",_DbEof,mtKERNEL);
   AddKeyWord("DBCOMMIT",_DbCommit,mtKERNEL);
   AddKeyWord("DBCOMMITALL",_DbCommitAll,mtKERNEL);
   AddKeyWord("DBGO",_DbGo,mtKERNEL);
   AddKeyWord("DBLOCK",_DbLock,mtKERNEL);
   AddKeyWord("DBLOCKED",_DbLocked,mtKERNEL);
   AddKeyWord("DBFLOCK",_DbFLock,mtKERNEL);
   AddKeyWord("DBFLOCKED",_DbFLocked,mtKERNEL);
   AddKeyWord("DBINDEX",_DbIdxName,mtKERNEL);
   AddKeyWord("DBKEY",_DbIdxKey,mtKERNEL);
   AddKeyWord("DBRECALL",_DbRecall,mtKERNEL);
   AddKeyWord("DBRECCOUNT",_DbReccount,mtKERNEL);
   AddKeyWord("DBRECNO",_DbRecno,mtKERNEL);
   AddKeyWord("DBSEEK",_DbSeek,mtKERNEL);
   AddKeyWord("DBSKIP",_DbSkip,mtKERNEL);
   AddKeyWord("DBSETINDEX",_DbSetIndex,mtKERNEL);
   AddKeyWord("DBTOP",_DbTop,mtKERNEL);
   AddKeyWord("DBUNLOCK",_DbUnLock,mtKERNEL);
   AddKeyWord("DBUNLOCKALL",_DbUnLockAll,mtKERNEL);
   AddKeyWord("DBREPLACE",_DbReplace,mtKERNEL);
   AddKeyWord("DBFIELD",_DbField,mtKERNEL);
   AddKeyWord("DBFIELDTYPE",_DbFieldType,mtKERNEL);
   AddKeyWord("DBFIELDLEN",_DbFieldLen,mtKERNEL);
   AddKeyWord("DBFIELDCOUNT",_DbFieldCount,mtKERNEL);
   AddKeyWord("DBFIELDLENDEC",_DbFieldLenDec,mtKERNEL);
   AddKeyWord("DBNUMFIELD->NAME",_DbFieldNumToName,mtKERNEL);
   AddKeyWord("DBERROR",_DbError,mtKERNEL);
   AddKeyWord("ERROR?",_IntError,mtKERNEL);
   AddKeyWord("ERRORCODE",_ErrorCode,mtKERNEL);
   AddKeyWord("ALEN",_ALen,mtKERNEL);
   AddKeyWord("DELETELOCALTASK",_DeleteLTask,mtKERNEL);
   AddKeyWord("LOCALTASKACTIVE?",_IsLTaskActive,mtKERNEL);
   AddKeyWord("DTOS",_DateDtoS,mtKERNEL);
   AddKeyWord("STOD",_DateStoD,mtKERNEL);
   AddKeyWord("DTOJ",_DateDtoJ,mtKERNEL);
   AddKeyWord("JTOD",_DateJtoD,mtKERNEL);
   AddKeyWord("DAY",_DateDay,mtKERNEL);
   AddKeyWord("DOW",_DateDow,mtKERNEL);
   AddKeyWord("MONTH",_DateMonth,mtKERNEL);
   AddKeyWord("YEAR",_DateYear,mtKERNEL);
   AddKeyWord("CDOW",_DateCDow,mtKERNEL);
   AddKeyWord("CMONTH",_DateCMonth,mtKERNEL);
   AddKeyWord("FOPEN",_FTOpen,mtKERNEL);
   AddKeyWord("FCLOSE",_FTClose,mtKERNEL);
   AddKeyWord("FEOF",_FTEof,mtKERNEL);
   AddKeyWord("FREADLN",_FTReadLn,mtKERNEL);
   AddKeyWord("FREAD",_FTRead,mtKERNEL);
   AddKeyWord("FSIZE",_FTSize,mtKERNEL);
   AddKeyWord("CRC16",_CRC16Buff,mtKERNEL);
   AddKeyWord("CRC32",_CRC32Buff,mtKERNEL);
   AddKeyWord("B8->7",_Conv8to7,mtKERNEL);
   AddKeyWord("B7->8",_Conv7to8,mtKERNEL);
   AddKeyWord("XXENCODE",__XXEncode,mtKERNEL);
   AddKeyWord("XXDECODE",__XXDecode,mtKERNEL);
   AddKeyWord("UUENCODE",__UUEncode,mtKERNEL);
   AddKeyWord("UUDECODE",__UUDecode,mtKERNEL);
   AddKeyWord("FLUSHOUTBUFFER",_FlushOutBuff,mtKERNEL);
   AddKeyWord("EMITBUFFER",_TransmitBuff,mtKERNEL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression du dictionnaire global.
//
void DelGlobalDic(void)
{
   DelDico(pGlobalDic,uGDico);
   MemFree(pGlobalDic);
   // Suppression des constantes du kernel.
   DelSymValue(pCONSTTRUE);
   DelSymValue(pCONSTFALSE);
   DelSymValue(pCONSTLINE0);
   DelSymValue(pCONSTCOL1);
   DelSymValue(pCONSTFKENVOI);
   DelSymValue(pCONSTFKRETOUR);
   DelSymValue(pCONSTFKREPETITION);
   DelSymValue(pCONSTFKGUIDE);
   DelSymValue(pCONSTFKANNULATION);
   DelSymValue(pCONSTFKSOMMAIRE);
   DelSymValue(pCONSTFKCORRECTION);
   DelSymValue(pCONSTFKSUITE);
   DelSymValue(pCONSTBLACK);
   DelSymValue(pCONSTRED);
   DelSymValue(pCONSTGREEN);
   DelSymValue(pCONSTYELLOW);
   DelSymValue(pCONSTBLUE);
   DelSymValue(pCONSTMAGENTA);
   DelSymValue(pCONSTCYAN);
   DelSymValue(pCONSTWHITE);
   DelSymValue(pCONSTFSIMPLE);
   DelSymValue(pCONSTDHAUTEUR);
   DelSymValue(pCONSTDLARGEUR);
   DelSymValue(pCONSTDTAILLE);
   DelSymValue(pCONSTTMVDT);
   DelSymValue(pCONSTTMMXT);
   DelSymValue(pCONSTTMTEL);
   MemFree(pCONSTTRUE);
   MemFree(pCONSTFALSE);
   MemFree(pCONSTLINE0);
   MemFree(pCONSTCOL1);
   MemFree(pCONSTFKENVOI);
   MemFree(pCONSTFKRETOUR);
   MemFree(pCONSTFKREPETITION);
   MemFree(pCONSTFKGUIDE);
   MemFree(pCONSTFKANNULATION);
   MemFree(pCONSTFKSOMMAIRE);
   MemFree(pCONSTFKCORRECTION);
   MemFree(pCONSTFKSUITE);
   MemFree(pCONSTBLACK);
   MemFree(pCONSTRED);
   MemFree(pCONSTGREEN);
   MemFree(pCONSTYELLOW);
   MemFree(pCONSTBLUE);
   MemFree(pCONSTMAGENTA);
   MemFree(pCONSTCYAN);
   MemFree(pCONSTWHITE);
   MemFree(pCONSTFSIMPLE);
   MemFree(pCONSTDHAUTEUR);
   MemFree(pCONSTDLARGEUR);
   MemFree(pCONSTDTAILLE);
   MemFree(pCONSTTMVDT);
   MemFree(pCONSTTMMXT);
   MemFree(pCONSTTMTEL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compilation de la ligne courante.
// Renvoie VRAI si une erreur se produit.
//
mtBOOL CompileLigne(mtPTRBYTE Lig)
{
   mtBOOL bErr=mtFALSE,bComp=mtFALSE;
   mtPTRBYTE cItem;
   float dVal;
   mtPTRDICO pDSym;
   mtWORD wPortee;
   mtBYTE pComp;
   mtUWORD wPos;

   cItem = strtok(Lig,cDelim);
   while(cItem)
   {
      if(strncmp(cItem,"//",2) == 0)
         break;
      strupr(cItem);
      //
      // Est-ce un symbole dfini ?.
      //
      pDSym = SearchSymb(cItem,&wPortee,&wPos);
      if(pDSym)
      {
         CompileSym(pDSym,wPortee,pCFIC->pCApp,wPos);
      }
      else
      {
         //
         // Est-ce une directive de compilation ?
         //
         pComp            = 0;
         bComp            = mtFALSE;
         pCFIC->bFinLigne = mtFALSE;
         while(CompileDef[pComp])
         {
            if(strcmp(CompileDef[pComp],cItem) == 0)
            {
               bErr  = (FctCompile[pComp])();
               bComp = mtTRUE;
               break;
            }
            else
               pComp++;
         }
         if(bErr)
            break;
         if(pCFIC->bFinLigne)
            break;
         if(! bComp)
         {
            //
            // Est-ce un nombre ?
            //
            if(IsNumber(cItem,&dVal))
            {
               CompileNombre(dVal);
            }
            else
            {
               //
               // Est-ce un appel de procdure externe ?
               //
               if(IsExternProc(cItem))
               {
                  CompileExternProc(cItem);
               }
               else
               {
                  //
                  // Est-ce une rfrence … une variable externe ?
                  //
                  if(IsExternVar(cItem))
                  {
                     CompileExternVar(cItem);
                  }
                  else
                  {
                     WinInfo("%s inconnu",cItem);
                     WriteLogNeo("%s inconnu",cItem);
                     bErr = mtTRUE;
                     break;
                  }
               }
            }
         }
      }
      cItem = strtok(NULL,cDelim);
   }
   return(bErr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement des redirections de touches de fonctions
//
void ProcessFKey(void)
{
   mtPTRTFKEY K;

   pCurrCV->bFKeyOk = mtFALSE;
   if(pCurrCV->bPaqLu)
   {
      if(pCurrCV->bLFKAct)
      {
         // Recherche dans la table locale d'abord.
         K = SearchTFKEY(pCurrCV->pLTabFKey,pCurrCV->TFonction,pCurrCV->bDiese,pCurrCV->bEtoile);
         if(K)
         {
            if(K->bReturn)
            {
               // Sauvegarde du point de retour.
               _RPush((mtPOINTEUR) pCurrCV->IP);
               _RPush((mtPOINTEUR) pCurrCV->pHere);
            }
            pCurrCV->pHere   = K->HProc;
            pCurrCV->IP      = K->IPProc;
            pCurrCV->bFKeyOk = mtTRUE;
         }
      }
      if((pCurrCV->bGFKAct) && (! pCurrCV->bFKeyOk))
      {
         // Recherche dans la table globale.
         K = SearchTFKEY(pCurrCV->pApp->pGTabFKey,pCurrCV->TFonction,pCurrCV->bDiese,pCurrCV->bEtoile);
         if(K)
         {
            if(K->bReturn)
            {
               // Sauvegarde du point de retour.
               _RPush((mtPOINTEUR) pCurrCV->IP);
               _RPush((mtPOINTEUR) pCurrCV->pHere);
            }
            pCurrCV->pHere   = K->HProc;
            pCurrCV->IP      = K->IPProc;
            pCurrCV->bFKeyOk = mtTRUE;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement des mot-clef.
//
void ProcessKeyW(void)
{
   mtPTRTKEYW K;

   pCurrCV->bKeyWOk = mtFALSE;
   if((pCurrCV->bPaqLu) && (pCurrCV->pData))
   {
      if(pCurrCV->bLKWAct)
      {
         // Recherche dans la table locale d'abord.
         K = SearchTKEYW(pCurrCV->pLTabKeyW,pCurrCV->pData);
         if(K)
         {
            if(K->bReturn)
            {
               // Sauvegarde du point de retour.
               _RPush((mtPOINTEUR) pCurrCV->IP);
               _RPush((mtPOINTEUR) pCurrCV->pHere);
            }
            pCurrCV->pHere   = K->HProc;
            pCurrCV->IP      = K->IPProc;
            pCurrCV->bKeyWOk = mtTRUE;
         }
      }
      if((pCurrCV->bGKWAct) && (! pCurrCV->bKeyWOk))
      {
         // Recherche dans la table globale.
         K = SearchTKEYW(pCurrCV->pApp->pGTabKeyW,pCurrCV->pData);
         if(K)
         {
            if(K->bReturn)
            {
               // Sauvegarde du point de retour.
               _RPush((mtPOINTEUR) pCurrCV->IP);
               _RPush((mtPOINTEUR) pCurrCV->pHere);
            }
            pCurrCV->pHere   = K->HProc;
            pCurrCV->IP      = K->IPProc;
            pCurrCV->bKeyWOk = mtTRUE;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du code de pCurrCV pendant le temps TimeExec.
//
void ExecCode(void)
{
   pFUNC fExec;
   struct dos_time_t HFin;
   float dHDeb,dHFin;

   _dos_gettime(&pCurrCV->HDebExec);
   dHDeb          = TimeToCent(&pCurrCV->HDebExec)*10;
   pCurrCV->NbIPS = 0;
   while(1)
   {
      if(pCurrCV->bInDebug)
      {
         if(bNextStep)
         {
            // Mode Trace
            fExec = pCurrCV->pHere[pCurrCV->IP++];
            (fExec)();
         }
         else
            // Prochain pas non activ.
            break;
      }
      else
      {
         fExec = pCurrCV->pHere[pCurrCV->IP++];
         (fExec)();
      }
      if(! pCurrCV->Actif)
         break;
      pCurrCV->NbIPS++;
      if(pCurrCV->bInDebug)
      {
         WinAffTraceCv();
         // Mode Trace, wPasTrace instruction par tranche.
         wNbInst++;
         if(wNbInst >= wPasTrace)
         {
            bNextStep = mtFALSE;
            bAffTrace = mtFALSE;
            wNbInst   = 0;
         }
         // On sort quand mme … chaque instruction.
         break;
      }
      if(pCurrCV->TimeExec)
      {
         _dos_gettime(&HFin);
         dHFin = TimeToCent(&HFin)*10;
         if(dHFin<dHDeb)
            // A cheval sur 2 jours.
            dHFin += (float) 86400000;
         if((dHFin - dHDeb) >= pCurrCV->TimeExec)
            // Tranche de temps termine.
            break;
      }
      else
      {
         // Une instruction du kernel a fore TimeExec … 0 pour provoquer une
         // sortie immdiate.
         break;
      }
      pCurrCV->bPaqLu = mtFALSE;
   }
   if(pCurrCV->NbIPS > pCurrCV->MaxIPS)
      pCurrCV->MaxIPS = pCurrCV->NbIPS;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des symboles cres mais non utiliss par une application.
void AffAccessed(pPDICO LeDico,mtUWORD IDic)
{
   mtBYTE OBuff[80];
   mtPTRDICO d;
   mtUWORD wIDic=0;

   while(wIDic<IDic)
   {
      d = LeDico[wIDic];
      if(! d->bAccessed)
      {
         if((strcmp(d->Nom,"MAIN") != 0) && (strcmp(d->Nom,"ONLIB") != 0) && (strcmp(d->Nom,"GTASK") != 0))
         {
            // Ce n'est ni MAIN, ni ONLIB, ni GTASK
            sprintf(OBuff,"%s (",d->Nom);
            switch(d->Type)
            {
               case mtTCHAR    : { strcat(OBuff,"CHAR")     ; break ; }
               case mtTNUMBER  : { strcat(OBuff,"NUMBER")   ; break ; }
               case mtTABCHAR  : { strcat(OBuff,"CHAR[]")   ; break ; }
               case mtTABNUMBER: { strcat(OBuff,"NUMBER[]") ; break ; }
               case mt2POINTS  : { strcat(OBuff,"PROC/FUNC"); break ; }
            }
            strcat(OBuff,") non utilis");
            WinInfo(OBuff);
            WriteLogNeo(OBuff);
         }
      }
      wIDic++;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Chargement et compilation d'un source.
// Renvoie FAUX si tout c'est bien pass.
//
mtBOOL LoadApp(mtPTRBYTE FicPrg)
{
   ppFUNC OldHere;
   int i,JCode;
   char *pTmp;
   char file[MAXPATH];
   char *PathFic,*NomFic;
   mtBOOL bErrComp=mtFALSE;
   mtPTRAPP pAp;

   bInCompile = mtTRUE;
   //
   // Initialisation des erreurs de compilation.
   //
   JCode = setjmp(COMPERROR);
   if(JCode != 0)
   {
      if(pCFIC)
      {
         // Affichage de la ligne incrimine.
         if(! pCFIC->bEcho)
            WinInfo("Ligne %5d '%s'",pCFIC->NoLig,pCFIC->pLAff);
         _FileClose(pCFIC->hFic);
         if(pCFIC->NApp)
            DelApp(pCFIC->NApp);
         if(pCFIC->TabMark)
         {
            MemFree(pCFIC->TabMark);
            pCFIC->TabMark = NULL;
         }
         MemFree(pCFIC);
         MemFree(PathFic);
      }
      bErrComp = mtTRUE;
   }
   if(! bErrComp)
   {
      PathFic    = filespecdefaultext(FicPrg,ExtPrg);
      if(PathFic)
      {
         NomFic = filespecgetroot(filespecname(FicPrg));
         if(strlen(NomFic) == (strlen(PathFic)-4))
         {
            // Pas de chemin spcifi, on ajoute PathAPPS.
            pTmp    = filespecaddpath(PathAPPS,PathFic);
            MemFree(PathFic);
            PathFic = pTmp;
         }
         strcpy(file,NomFic);
         MemFree(NomFic);
         pAp = SearchAppFic(PathFic);
         if(! pAp)
         {
            pCFIC = (mtPTRCFIC) MemAlloc(sizeof(CtxFic));
            if(_FileOpen(PathFic,O_BINARY,&pCFIC->hFic,mtFALSE))
            {
               WinInfo("Compilation de %s en cours...",PathFic);
               WriteLogNeo("Compilation de %s en cours...",PathFic);
               pCFIC->NoLig = 0L;
               //
               // Par dfaut, et si il n'existe pas dj…, le
               // nom du fichier est pris comme nom d'application.
               //
               strcpy(pCFIC->NApp,file);
               pCFIC->pCApp = SearchApp(pCFIC->NApp,0);
               if(! pCFIC->pCApp)
               {
                  pCFIC->pCApp = NewApp(pCFIC->NApp,0);
                  // On fabrique le nom du fichier d'erreur.
                  // Par dfaut, les 8 premiers caractres du nom de l'appli.
                  strncpy(pCFIC->pCApp->NFicErr,pCFIC->NApp,8);
                  strcat(pCFIC->pCApp->NFicErr,".ERR");
                  // On conserve le nom du fichier source.
                  strcpy(pCFIC->pCApp->FicSrc,PathFic);
                  // Allocation du premier bloc de code.
                  pCFIC->pCApp->NbBlock   = 1;
                  pCFIC->pCApp->pHere     = (ppFUNC) MemAlloc((pCFIC->pCApp->NbBlock*BLOCKSIZE)*sizeof(pFUNC));
                  pCFIC->pCApp->bLIBErr   = mtTRUE;
                  pCFIC->pCApp->ConfCNX   = mtTRUE;
                  pCFIC->pCApp->bAnticipe = mtFALSE;
                  pCFIC->wHERE  = 0;
                  pCFIC->pTMark = 0;
               }
               else
               {
                  WinInfo("LoadApp '%s': CE NOM EXISTE DEJA ",pCFIC->NApp);
                  WriteLogNeo("LoadApp '%s': CE NOM EXISTE DEJA ",pCFIC->NApp);
                  bErrComp = mtTRUE;
               }
               if(! bErrComp)
               {
                  pCFIC->bIn2Points = mtFALSE;
                  pCFIC->bInMain    = mtFALSE;
                  pCFIC->bMainDef   = mtFALSE;
                  pCFIC->bPUBLICDEF = mtTRUE;
                  pCFIC->bLOCALDEF  = mtFALSE;
                  pCFIC->bEXTERNDEF = mtFALSE;
                  pCFIC->bWarning   = mtFALSE;
                  pCFIC->bEcho      = mtFALSE;
                  pCFIC->NbDO       = 0;
                  pCFIC->NbIF       = 0;
                  pCFIC->NbBEGIN    = 0;
                  pCFIC->NbWHILE    = 0;
                  pCFIC->NbCASE     = 0;
                  pCFIC->NbOF       = 0;
                  while((! bErrComp) && (! _IsEOF(pCFIC->hFic)))
                  {
                     pCFIC->NoLig++;
                     pCFIC->pLig = pCFIC->Ligne;
                     memset(pCFIC->pLig,0,MAXLLIG+1);
                     pCFIC->pLAff = pCFIC->LigAff;
                     memset(pCFIC->pLAff,0,MAXLLIG+1);
                     _FileReadLine(pCFIC->hFic,pCFIC->pLig,MAXLLIG);
                     strcpy(pCFIC->pLAff,pCFIC->pLig);
                     if(pCFIC->bEcho)
                        WinInfo("%s",pCFIC->pLAff);
                     LTrim(pCFIC->pLig);
                     //
                     // On enlve le CRLF de fin.
                     //
                     //pTmp = strrchr(pCFIC->pLig,10);
                     //if(pTmp)
                     //   *(pTmp) = '\0';
                     //pTmp = strrchr(pCFIC->pLig,13);
                     //if(pTmp)
                     //   *(pTmp) = '\0';
                     if(strlen(pCFIC->pLig) < 1)
                        continue;
                     //
                     // Compilation.
                     //
                     bErrComp = CompileLigne(pCFIC->pLig);
                     if((bErrComp) && (! pCFIC->bEcho))
                     {
                        // Affichage de la ligne incrimine.
                        WinInfo("Ligne %5d '%s'",pCFIC->NoLig,pCFIC->pLAff);
                        WriteLogNeo("Ligne %5d '%s'",pCFIC->NoLig,pCFIC->pLAff);
                     }
                  }
                  _FileClose(pCFIC->hFic);
                  if(! bErrComp)
                  {
                     // Pas d'erreurs de compilation
                     // Vrification prsence de MAIN
                     if(pCFIC->bMainDef)
                        pCFIC->pCApp->Active = mtTRUE;
                     else
                        pCFIC->pCApp->Active = mtFALSE;
                     // On ajuste la taille du bloc de code.
                     OldHere = pCFIC->pCApp->pHere;
                     pCFIC->pCApp->pHere = (ppFUNC) MemReAlloc(pCFIC->pCApp->pHere,(pCFIC->wHERE+1)*sizeof(pFUNC));
                     if(pCFIC->pCApp->pHere != OldHere)
                     {
                        // L'adresse du bloc a change.
                        // Il faut recalculer les rfrences aux fonctions du programme.
                        ReCalcAdr(OldHere,pCFIC->pCApp->pHere);
                     }
                     pCFIC->pCApp->SizeCode = (mtUWORD) (pCFIC->wHERE*sizeof(pFUNC));
                     WinInfo("%s (%lu lignes), Code = %do., Publiques = %do., Locales = %do.",pCFIC->NApp,pCFIC->NoLig,pCFIC->pCApp->SizeCode,pCFIC->pCApp->SizePData,pCFIC->pCApp->SizeLData);
                     WriteLogNeo("%s (%lu lignes), Code = %do., Publiques = %do., Locales = %do.",pCFIC->NApp,pCFIC->NoLig,pCFIC->pCApp->SizeCode,pCFIC->pCApp->SizePData,pCFIC->pCApp->SizeLData);
                     if(pCFIC->TabMark)
                     {
                        MemFree(pCFIC->TabMark);
                        pCFIC->TabMark = NULL;
                     }
                     if(pCFIC->bWarning)
                     {
                        // Affichage des symboles non utiliss
                        AffAccessed(pCFIC->pCApp->pPDico,pCFIC->pCApp->uPDico);
                        AffAccessed(pCFIC->pCApp->pLDico,pCFIC->pCApp->uLDico);
                     }
                     // Rcupration des ventuelles infos paliers.
                     RecupAppStat(pCFIC->pCApp);
                     // Rcupration des ventuelles infos systmes.
                     RecupAppSysInfo(pCFIC->pCApp);
                  }
                  else
                  {
                     DelApp(pCFIC->NApp);
                     if(pCFIC->TabMark)
                     {
                        MemFree(pCFIC->TabMark);
                        pCFIC->TabMark = NULL;
                     }
                  }
               }
               else
                  _FileClose(pCFIC->hFic);
            }
            else
            {
               bErrComp = mtTRUE;
               WinInfo("LoadApp '%s': OUVERTURE DU FICHIER IMPOSSIBLE",PathFic);
               WriteLogNeo("LoadApp '%s': OUVERTURE DU FICHIER IMPOSSIBLE",PathFic);
            }
            MemFree(pCFIC);
         }
         else
         {
            bErrComp = mtTRUE;
            WinInfo("LoadApp '%s': APPLICATION DEJA CHARGEE (Nom=%s)",PathFic,pAp->Nom);
            WriteLogNeo("LoadApp '%s': APPLICATION DEJA CHARGEE (Nom=%s)",PathFic,pAp->Nom);
         }
         MemFree(PathFic);
      }
      else
      {
         bErrComp = mtTRUE;
         WinInfo("LoadApp '%s': OUVERTURE DU FICHIER IMPOSSIBLE",FicPrg);
         WriteLogNeo("LoadApp '%s': OUVERTURE DU FICHIER IMPOSSIBLE",FicPrg);
      }
   }
   bInCompile = mtFALSE;
   return(bErrComp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libration d'une application.
//
void UnLoadApp(mtPTRBYTE NomApp)
{
   mtPTRAPP pApp,pAp;

   pApp = SearchApp(NomApp,0);
   if(pApp)
   {
      if(pApp->pLRefApp)
      {
         WinMsgErr(": APPLICATION UTILISEE","UnLoadApp %s",NomApp);
         WriteLogNeo("UnLoadApp %s: APPLICATION UTILISEE",NomApp);
      }
      else
      {
         if(pApp->pLstCnx)
         {
            WinMsgErr(": CONNEXIONS ENCORES ACTIVES","UnLoadApp %s",NomApp);
            WriteLogNeo("UnLoadApp %s: CONNEXIONS ENCORES ACTIVES",NomApp);
         }
         else
         {
            SauveAppStat(pApp);
            SauveAppSysInfo(pApp);
            if(bAutoCNX)
            {
               pAp = SearchApp(NomAppAutoCNX,0);
               if(pApp == pAp)
                  bAutoCNX = mtFALSE;
            }
            DelApp(NomApp);
            WinInfo("Application %s supprime",NomApp);
            WriteLogNeo("Application %s supprime",NomApp);
         }
      }
   }
   else
   {
      WinMsgErr(": APPLICATION INEXISTANTE","UnLoadApp %s",NomApp);
      WriteLogNeo("UnLoadApp %s: APPLICATION INEXISTANTE",NomApp);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void LoadFichier(void)
{
   mtBOOL bErr;
   mtUWORD pLoad=0;

   if(iLoad)
   {
      while(pLoad<iLoad)
      {
         bErr  = LoadApp(TLoadFic[pLoad]);
         pLoad++;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Chargement de l'application d'auto-connexion.
//
void LoadAutoApp(void)
{
   char *PathFic,*NomFic;
   mtPTRAPP pAp;
   mtBOOL bRet;

   if(pAppAutoCNX)
   {
      NomFic = filespecgetroot(filespecname(pAppAutoCNX));
      if(strlen(pAppAutoCNX) == strlen(NomFic))
      {
         // Pas de chemin spcifi, on ajoute PathAPPS.
         PathFic     = filespecaddpath(PathAPPS,NomFic);
         pAppAutoCNX = (char *) MemReAlloc(pAppAutoCNX,strlen(PathFic)+1);
         strcpy(pAppAutoCNX,PathFic);
         MemFree(PathFic);
      }
      NomAppAutoCNX = (char *) MemReAlloc(NomAppAutoCNX,strlen(NomFic)+1);
      strcpy(NomAppAutoCNX,NomFic);
      MemFree(NomFic);
      pAp = SearchApp(NomAppAutoCNX,0);
      if(! pAp)
      {
         if(LoadApp(pAppAutoCNX))
            bAutoCNX = mtFALSE;
         pAp = SearchApp(NomAppAutoCNX,0);
         if(pAp)
         {
            if(! pAp->Active)
               bAutoCNX = mtFALSE;
         }
      }
      else
      {
         if(! pAp->Active)
            bAutoCNX = mtFALSE;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

