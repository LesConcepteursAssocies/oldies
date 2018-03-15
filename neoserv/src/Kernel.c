// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณKERNEL.C     ณ                                                             ณ
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
// ณDfinition des primitives du noyau                                         ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <mem.h>
#include <direct.h>
#include <io.h>
#include <math.h>
#include <fltpnt.h>
#include <float.h>
#include <dir.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "toolkit.h"
#include "filespec.h"
#include "d4all.h"
#pragma SC align
#include "minitel.h"
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "ports.h"

// Table de translation des accents et caractres spciaux du minitel.
#define  NBACCENTS      16
typedef struct _TrAcc
{
   mtBYTE Accent;
   char SeqEsc[4];
} TrAcc;
TrAcc tAccents[NBACCENTS] =
{
   { '…', "Aa" },
   { '', "Ca" },
   { '', "Kc" },
   { '', "Be" },
   { '', "Ae" },
   { '', "Ce" },
   { '', "He" },
   { '', "Ci" },
   { '', "Hi" },
   { '–', "Cu" },
   { '—', "Au" },
   { '', "Hu" },
   { '”', "Ho" },
   { '“', "Co" },
   { 'แ', "{"  },
   { '', "#"  }
};

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition des constantes.
//
mtPTRDICO     pCONSTTRUE;
mtPTRDICO     pCONSTFALSE;
mtPTRDICO     pCONSTLINE0;             // Utilises par
mtPTRDICO     pCONSTCOL1;              // LINE0.
mtPTRDICO     pCONSTFKENVOI;
mtPTRDICO     pCONSTFKRETOUR;
mtPTRDICO     pCONSTFKREPETITION;
mtPTRDICO     pCONSTFKGUIDE;
mtPTRDICO     pCONSTFKANNULATION;
mtPTRDICO     pCONSTFKSOMMAIRE;
mtPTRDICO     pCONSTFKCORRECTION;
mtPTRDICO     pCONSTFKSUITE;
mtPTRDICO     pCONSTBLACK;
mtPTRDICO     pCONSTRED;
mtPTRDICO     pCONSTGREEN;
mtPTRDICO     pCONSTYELLOW;
mtPTRDICO     pCONSTBLUE;
mtPTRDICO     pCONSTMAGENTA;
mtPTRDICO     pCONSTCYAN;
mtPTRDICO     pCONSTWHITE;
mtPTRDICO     pCONSTFSIMPLE;
mtPTRDICO     pCONSTDHAUTEUR;
mtPTRDICO     pCONSTDLARGEUR;
mtPTRDICO     pCONSTDTAILLE;
mtPTRDICO     pCONSTTMVDT;
mtPTRDICO     pCONSTTMMXT;
mtPTRDICO     pCONSTTMTEL;
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   int        nErrCmd;           // Dernire erreur clavier.
extern   UnCv       TabCv[TOTALCV];    // Dfini Dans le prog. principal.
extern   UnCv       LTask[TOTALCV];    // Dfini Dans le prog. principal.
extern   Ports      tPorts[MAXPORTS];
extern   jmp_buf    EXECERROR;
extern   struct     dos_date_t DLanc;
extern   struct     dos_time_t HLanc;
extern   struct     dos_time_t HFinServ;
extern   mtUDWORD   RAMDispo;
extern   int        NbCnx;
extern   int        PointeCnx;
extern   float      ChargeA;
extern   float      ChargeM;
extern   mtBOOL     bInfoGen;
extern   mtBYTE     Heures;
extern   mtBYTE     Minutes;
extern   mtBYTE     Secondes;
extern   mtBYTE     Jour;
extern   mtBYTE     Mois;
extern   mtWORD     Annee;
extern   mtUDWORD   NbAppTot;
extern   struct     dos_time_t HPreCnx;
extern   struct     dos_time_t HDerCnx;
extern   mtPTRCV    pCurrCV;           // Pointe sur le cv courant.
extern   mtPTRCV    pCurrLT;           // Pointe sur la tche locale du cv courant.
extern   void       AppendCvOBuff(mtPTRCV,mtPTRBYTE,mtBOOL);
extern   mtPOINTEUR MemAlloc(mtUDWORD);
extern   mtPOINTEUR MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   void       MemFree(mtPOINTEUR);
extern   mtPTRTFKEY NewFKey(mtPTRTFKEY,mtBOOL,mtBOOL,mtBYTE,mtPTRDICO);
extern   mtPTRTFKEY SearchTFKEY(mtPTRTFKEY,mtBYTE,mtBOOL,mtBOOL);
extern   void       DelTFKEY(mtPTRTFKEY);
extern   mtPTRTKEYW NewKeyW(mtPTRTKEYW,mtPTRBYTE,mtPTRDICO);
extern   mtPTRTKEYW SearchTKEYW(mtPTRTKEYW,mtPTRBYTE);
extern   void       DelTKEYW(mtPTRTKEYW);
extern   void       DelSymValue(mtPTRDICO);
extern   void       WinInfo(mtPTRBYTE,...);
extern   void       SWVRingBell(void);
extern   void       CmCLS(void);
extern   float      TimeToCent(struct dos_time_t *);
extern   mtPTRAPP   SearchApp(mtPTRBYTE,mtBYTE);
extern   mtPTRDICO  SearchDico(pPDICO,mtUWORD,mtPTRBYTE,mtPTRUWORD);
extern   mtPTRREFAPP NewRefApp(mtPTRREFAPP,mtPTRAPP);
extern   mtPTRREFAPP SearchRefApp(mtPTRREFAPP,mtPTRAPP);
extern   void       _CopyLDic(void);
extern   mtPTRAPP   TSearchApp(ppFUNC);
extern   mtPTRBYTE  TSearchNomProc(pPDICO pDic,mtUWORD IDic,mtPOINTEUR pSeg,mtPOINTEUR pOff);
extern   void       InitCnxCv(mtPTRCV);
extern   void       DeconnecteCv(mtPTRCV);
extern   void       DeconnecteGTCv(mtPTRAPP);
extern   mtBOOL     _FileOpen(mtPTRBYTE,mtWORD,mtPTRWORD,mtBOOL);
extern   void       _FileClose(mtWORD);
extern   mtDWORD    _FileGetPos(mtWORD);
extern   mtBOOL     _IsEOF(mtWORD);
extern   void       _FileRead(mtWORD,mtPTRBYTE,size_t);
extern   void       _FileReadLine(mtWORD,mtPTRBYTE,size_t);
extern   void       _FileWrite(mtWORD,mtPTRBYTE,size_t);
extern   void       _FileWriteLn(mtWORD,mtPTRBYTE,size_t);
extern   void       ProcessKbdCmd(mtPTRBYTE);
extern   mtUSHORT   UpdateCRC16(mtBYTE,mtUSHORT);
extern   mtUWORD    UpdateCRC32(mtBYTE,mtUWORD);
extern   void       tr8to7(mtBYTE **,mtBYTE **);
extern   void       tr7to8(mtBYTE **,mtBYTE **);
extern   void       FreeCvOutB(mtPTRCV);
extern   void       _XXEncode(mtPTRBYTE,mtWORD,mtPTRBYTE);
extern   void       _XXDecode(mtPTRBYTE,mtWORD,mtPTRBYTE);
extern   void       _UUEncode(mtPTRBYTE,mtWORD,mtPTRBYTE);
extern   void       _UUDecode(mtPTRBYTE,mtWORD,mtPTRBYTE);
extern   mtPTRREGEXP regcomp(mtPTRBYTE);
extern   mtWORD      regexec(mtPTRREGEXP,mtPTRBYTE);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                       GESTION DES ERREURS D'EXECUTION
//                       อออออออออออออออออออออออออออออออ
//
// Cherche le symbole donn dans les dico. du cv donn.
// Si trouv, renvoie le pointeur sur le symbole dans l'application
// propritaire et l'adresse de l'appli., NULL sinon.
//
mtPTRDICO SearchAdrSym(mtPTRCV pCv,mtPTRDICO pSym,pPAPP pAp)
{
   mtPTRLISTDICO ld;
   mtPTRDICO pS,SSym=(mtPTRDICO) 0;
   mtPTRAPP pPRO=(mtPTRAPP) 0;
   mtUWORD uLD=0,uDI;

   // Balayage du tableau des dico. locaux.
   while(uLD<pCv->uLDico)
   {
      ld  = pCv->pLDico[uLD];
      uDI = 0;
      // Balayage du dico. local.
      while(uDI<ld->uLDico)
      {
         pS = ld->pLDico[uDI];
         if(pS == pSym)
         {
            pPRO = (mtPTRAPP) ld->pApp;
            break;
         }
         uDI++;
      }
      if(pPRO)
         break;
      uLD++;
   }
   if(pPRO)
   {
      // uDI pointe sur le symbole dans le dico. local de l'appli.
      SSym = pPRO->pLDico[uDI];
      *pAp = pPRO;
   }
   else
   {
      // On regarde dans le dico. publique de l'application du cv.
      uLD = 0;
      while(uLD<pCv->pApp->uPDico)
      {
         pS = pCv->pApp->pPDico[uLD];
         if(pS == pSym)
         {
            SSym = pS;
            *pAp = pCv->pApp;
            break;
         }
         uLD++;
      }
   }
   return(SSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//
// Stockage de l'erreur donne dans le fichier d'erreur de l'application
//
void ErrorLog(mtPTRCV LeCv,mtPTRBYTE MsgErr,mtPTRDICO pSym)
{
   char FLog[MAXPATH];
   char OBuff[256];
   mtPTRAPP pApp,pAPr;
   mtPOINTEUR pSeg,pOff;
   mtUWORD uIP,cSP;
   mtPTRBYTE pProc,pNBase;
   mtWORD nbSP;
   mtPTRDICO pSS,pSD;

   if(LeCv->pApp->NFicErr)
   {
      if(LeCv->pApp->Path)
      {
         // On ajoute le chemin principal.
         strcpy(FLog,LeCv->pApp->Path);
         strcat(FLog,LeCv->pApp->NFicErr);
      }
      else
         strcpy(FLog,LeCv->pApp->NFicErr);
      if(_FileOpen(FLog,O_BINARY | O_APPEND,&LeCv->pApp->pFicErr,mtTRUE))
      {
         memset(OBuff,0,81);
         memset(OBuff,45,80);
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         sprintf(OBuff,"ERREUR D'EXECUTION LE %02d/%02d/%04d A %02d:%02d:%02d",Jour,Mois,Annee,Heures,Minutes,Secondes);
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         _FileWriteLn(LeCv->pApp->pFicErr,MsgErr,strlen(MsgErr));
         if(LeCv->IsLTask)
            sprintf(OBuff,"Tche locale CV=%d IP=%05d SP=%05d RP=%05d",LeCv->NumCv,LeCv->IP,LeCv->SP,LeCv->RP);
         else
         {
            if(LeCv->IsOnLib)
               sprintf(OBuff,"Tche ONLIB CV=%d IP=%05d SP=%05d RP=%05d",LeCv->NumCv,LeCv->IP,LeCv->SP,LeCv->RP);
            else
            {
               if(LeCv->IsGTask)
                  sprintf(OBuff,"Tche GLOBALE IP=%05d SP=%05d RP=%05d",LeCv->IP,LeCv->SP,LeCv->RP);
               else
                  sprintf(OBuff,"CV=%d IP=%05d SP=%05d RP=%05d",LeCv->NumCv,LeCv->IP,LeCv->SP,LeCv->RP);
            }
         }
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         sprintf(OBuff,"Nombre de connects : %3d",NbCnx);
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         sprintf(OBuff,"DBERROR = %d (%d), ERRORCODE = %d",pCurrCV->DbError,pCurrCV->CBase.error_code,pCurrCV->CodeError);
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         if(LeCv->pNProc)
            sprintf(OBuff,"Primitive : '%s'",LeCv->pNProc);
         else
            sprintf(OBuff,"");
         // On cherche le nom de la procdure courante.
         pApp = TSearchApp(LeCv->pHere);
         if(pApp)
         {
            pSeg = (mtPOINTEUR) LeCv->pHere;
            uIP  = (mtUWORD) LeCv->IP;
            while(1)
            {
               pOff  = (mtPOINTEUR) uIP;
               pProc = TSearchNomProc(pApp->pPDico,pApp->uPDico,pSeg,pOff);
               if(pProc)
               {
                  strcat(OBuff," Appele dans ");
                  strcat(OBuff,pApp->Nom);
                  strcat(OBuff,"::");
                  strcat(OBuff,pProc);
                  break;
               }
               if(uIP == 0)
                  break;
               uIP--;
            }
         }
         _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         if(pSym)
         {
            // On affiche les informations sur le symbole ayant
            // provoqu l'erreur.
            // On le cherche d'abord dans les dico. du cv.
            pSD = SearchAdrSym(LeCv,pSym,&pAPr);
            if(pSD)
               sprintf(OBuff,"Symbole : '%s.%s' Type : ",pAPr->Nom,pSD->Nom);
            else
               sprintf(OBuff,"Symbole : INTERNE Type : ");
            switch(pSym->Type)
            {
               case mtTCHAR    : { strcat(OBuff,"CHAR")     ; break ; }
               case mtTNUMBER  : { strcat(OBuff,"NUMBER")   ; break ; }
               case mtTABCHAR  : { strcat(OBuff,"CHAR[]")   ; break ; }
               case mtTABNUMBER: { strcat(OBuff,"NUMBER[]") ; break ; }
            }
            switch(pSym->Attr)
            {
               case SYMLITERAL : { strcat(OBuff," (Litral)")            ; break ; }
               case SYMVARCON  : { strcat(OBuff," (Variable/Constante)") ; break ; }
            }
            _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
            _FileWrite(LeCv->pApp->pFicErr,"Contenu : ",10);
            switch(pSym->Type)
            {
               case mtTCHAR    :
               {
                  if(pSym->V.cVal)
                     _FileWriteLn(LeCv->pApp->pFicErr,pSym->V.cVal,strlen(pSym->V.cVal));
                  break;
               }
               case mtTNUMBER  :
               {
                  sprintf(OBuff,"%f",pSym->V.fVal);
                  _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
                  break;
               }
               case mtTABCHAR  :
               {
                  // _FileWriteLn(LeCv->pApp->pFicErr,(mtPTRBYTE) pValue,strlen((mtPTRBYTE) pValue));
                  break;
               }
               case mtTABNUMBER:
               {
                  // sprintf(OBuff,"%f",(float) pValue);
                  // _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
                  break;
               }
            }
         }
         if(LeCv->pApp->Path)
         {
            sprintf(OBuff,"PATH = %s",LeCv->pApp->Path);
            _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         }
         if(LeCv->pApp->PathVTX)
         {
            sprintf(OBuff,"PATHVTX = %s",LeCv->pApp->PathVTX);
            _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         }
         if(LeCv->pApp->PathDBF)
         {
            sprintf(OBuff,"PATHDBF = %s",LeCv->pApp->PathDBF);
            _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
         }
         if(LeCv->CurrBase)
         {
            pNBase = d4alias(LeCv->CurrBase);
            if(pNBase)
            {
               sprintf(OBuff,"Base courante : %s",pNBase);
               _FileWriteLn(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
            }
         }
         if(LeCv->SP)
         {
            // On imprime le contenu de la pile (les 10 1ers symboles maxi)
            _FileWriteLn(LeCv->pApp->pFicErr,"Contenu de la pile",18);
            nbSP = 0;
            cSP  = LeCv->SP;
            while((cSP) && (nbSP<=10))
            {
               cSP--;
               pSS = (mtPTRDICO) LeCv->PileD[cSP];
               if(pSS)
               {
                  if(pSS->Nom)
                     sprintf(OBuff,"Symbole : '%s' Type : ",pSS->Nom);
                  else
                     sprintf(OBuff,"Symbole : INTERNE Type : ");
                  switch(pSS->Type)
                  {
                     case mtTCHAR    : { strcat(OBuff,"CHAR")     ; break ; }
                     case mtTNUMBER  : { strcat(OBuff,"NUMBER")   ; break ; }
                     case mtTABCHAR  : { strcat(OBuff,"CHAR[]")   ; break ; }
                     case mtTABNUMBER: { strcat(OBuff,"NUMBER[]") ; break ; }
                  }
                  switch(pSS->Attr)
                  {
                     case SYMLITERAL : { strcat(OBuff," (Litral)")            ; break ; }
                     case SYMVARCON  : { strcat(OBuff," (Variable/Constante)") ; break ; }
                  }
                  _FileWrite(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
                  switch(pSS->Type)
                  {
                     case mtTCHAR    :
                     {
                        _FileWrite(LeCv->pApp->pFicErr," Contenu : ",11);
                        if(pSS->V.cVal)
                           _FileWrite(LeCv->pApp->pFicErr,pSS->V.cVal,strlen(pSS->V.cVal));
                        break;
                     }
                     case mtTNUMBER  :
                     {
                        _FileWrite(LeCv->pApp->pFicErr," Contenu : ",11);
                        sprintf(OBuff,"%f",pSS->V.fVal);
                        _FileWrite(LeCv->pApp->pFicErr,OBuff,strlen(OBuff));
                        break;
                     }
                  }
                  _FileWriteLn(LeCv->pApp->pFicErr,"",0);
               }
               else
                  _FileWriteLn(LeCv->pApp->pFicErr,"<<NULL>>",8);
               nbSP++;
            }
         }
         _FileClose(LeCv->pApp->pFicErr);
      }
      else
      {
         if(! bInfoGen)
            CmCLS();
         SWVRingBell();
         WinInfo("Impossible d'ouvrir le fichier %s",FLog);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Traitement des erreurs d'excution du cv courant.
//
void ExecError(mtPTRBYTE NomProc,int NoErr,mtPTRDICO pSym)
{
   char ErrBuff[128];

   static mtPTRBYTE ErrorMsg[] =
   {
      "Pile de donnes vide",
      "Pile de retour vide",
      "Pile de donnes pleine",
      "Pile de retour pleine",
      "Type incompatible",
      "Indice de tableau hors limite",
      "Donne non initialise",
      "Impossible d'ouvrir le fichier",
      "Base dj… ouverte",
      "Impossible d'ouvrir la base",
      "Impossible de fermer la base",
      "Impossible de fermer les bases",
      "Pas de base courante",
      "Impossible d'ajouter un enregistrement vide",
      "La base n'est pas ouverte",
      "Nom de champ incorrect",
      "Date incorrecte",
      "L'oprande destination ne peut tre un litral",
      "Division par zro",
      "Format d'affichage non valide",
      "Application inexistante",
      "Procdure inexistante ou non valide",
      "Type CHAR attendu",
      "Type CHAR[] attendu",
      "Type NUMBER attendu",
      "Type NUMBER[] attendu",
      "Type TABLEAU attendu",
      "Plus de mmoire disponible",
      "Application non valide",
      "Numro de champ incorrect",
      "Pas de fichier texte ouvert",
      "Impossible de slectionner l'index"
   };

   if(! bInfoGen)
      CmCLS();
   SWVRingBell();
   if(pCurrCV->IsLTask)
      sprintf(ErrBuff,"App='%s' Tche locale=%d Proc='%s': %s",pCurrCV->pApp->Nom,pCurrCV->NumCv,NomProc,ErrorMsg[NoErr]);
   else
   {
      if(pCurrCV->IsOnLib)
         sprintf(ErrBuff,"App='%s' Tche ONLIB=%d Proc='%s': %s",pCurrCV->pApp->Nom,pCurrCV->NumCv,NomProc,ErrorMsg[NoErr]);
      else
      {
         if(pCurrCV->IsGTask)
            sprintf(ErrBuff,"App='%s' Tche GLOBALE Proc='%s': %s",pCurrCV->pApp->Nom,NomProc,ErrorMsg[NoErr]);
         else
            sprintf(ErrBuff,"App='%s' Voie=%d Proc='%s': %s",pCurrCV->pApp->Nom,pCurrCV->NumCv,NomProc,ErrorMsg[NoErr]);
      }
   }
   WinInfo(ErrBuff);
   pCurrCV->ExecError = mtTRUE;
   pCurrCV->CodeError = NoErr;
   ErrorLog(pCurrCV,ErrBuff,pSym);
   if((pCurrCV->IsLTask) || (pCurrCV->IsOnLib) || (pCurrCV->IsGTask))
   {
      // Tche locale, globale ou ONLIB.
      pCurrCV->TimeExec = 0;
      // On remonte dans le programme principal.
      longjmp(EXECERROR,-1);
   }
   else
   {
      if((pCurrCV->pApp->bLIBErr) || (NoErr == ERRNULLPOINTER) || (NoErr == ERRDPILEVIDE) || (NoErr == ERRRPILEVIDE) || (NoErr == ERRBOUNDARRAY) || (NoErr == ERRZERODIVISE) || (NoErr == ERROUTOFMEMORY))
      {
         // #LIBONERROR = ON
         // ou erreur critique
         // On provoque la sortie de la boucle d'excution.
         pCurrCV->TimeExec = 0;
         // On remonte dans le programme principal.
         longjmp(EXECERROR,-1);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnre une erreur d'excution "valeur non initialise" si le pointeur
// donn est nul.
//
void CheckNull(mtPOINTEUR ptr,mtPTRBYTE NomProc,mtPTRDICO pSym)
{
   if(((pSym->Type == mtTCHAR) && (! ptr)) || ((pSym->Type == mtTABCHAR) && (! ptr)))
      ExecError(NomProc,ERRNULLPOINTER,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnre une erreur de "division par zro" si la valeur donne est nulle.
//
void CheckZeroDiv(float dVal,mtPTRBYTE NomProc,mtPTRDICO pSym)
{
   if(dVal == (float) 0)
      ExecError(NomProc,ERRZERODIVISE,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnre une erreur d'excution "...litral" si le symbole donn est un litral
// (chaine ou nombre)
//
void CheckLiteral(mtPTRDICO pSym,mtPTRBYTE NomProc)
{
   if(pSym->Attr == SYMLITERAL)
      ExecError(NomProc,ERROPLITERAL,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnre une erreur d'excution "type incompatible" si le symbole donn
// n'est pas du type numrique (simple ou tableau).
//
void CheckNumber(mtPTRDICO pSym,mtPTRBYTE NomProc)
{
   if((pSym->Type != mtTNUMBER) && (pSym->Type != mtTABNUMBER))
      ExecError(NomProc,ERRNUMBEREXPECT,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnre une erreur d'excution "type incompatible " si le symbole donn
// n'est pas du type caractre (simple ou tableau).
//
void CheckString(mtPTRDICO pSym,mtPTRBYTE NomProc)
{
   if((pSym->Type != mtTCHAR) && (pSym->Type != mtTABCHAR))
      ExecError(NomProc,ERRCHAREXPECT,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie vrai si la position donne est dans les limites du tableau
// donn.
//
mtBOOL IndiceTab(mtPTRITAB pITab,mtUWORD Lig,mtUWORD Col)
{
   if((Lig<pITab->MaxLig) && (Col<pITab->MaxCol))
      return(mtTRUE);
   return(mtFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                    GESTION DES PILES ET DES SYMBOLES
//                    อออออออออออออออออออออออออออออออออ
//
// Renvoie le pointeur du tableau de caractre donn … la position donne.
//
mtPTRBYTE ArrayCVal(mtPTRDICO pTab,mtPTRDICO pLig,mtPTRDICO pCol,mtPTRBYTE NomProc)
{
   mtPTRITAB pITmp;
   pPOINTEUR pTmp;
   mtPTRBYTE pTVal = NULL;
   mtUWORD Li,Co;

   Li    = (mtUWORD) pLig->V.fVal;
   Co    = (mtUWORD) pCol->V.fVal;
   pITmp = (mtPTRITAB) pTab->XVal;
   if(IndiceTab(pITmp,Li,Co))
   {
      CheckNull(pTab->V.tVal,NomProc,pTab);
      pTmp  = (pPOINTEUR) pTab->V.tVal;
      pTVal = *(pTmp+((Li*pITmp->MaxCol)+Co));
   }
   else
      ExecError(NomProc,ERRBOUNDARRAY,pTab);
   return(pTVal);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie le flottant du tableau donn … la position donne.
//
float ArrayNVal(mtPTRDICO pTab,mtPTRDICO pLig,mtPTRDICO pCol,mtPTRBYTE NomProc)
{
   mtPTRITAB pITmp;
   mtPTRFLOAT pTmp;
   float pTVal;
   mtUWORD Li,Co;

   Li    = (mtUWORD) pLig->V.fVal;
   Co    = (mtUWORD) pCol->V.fVal;
   pITmp = (mtPTRITAB) pTab->XVal;
   if(IndiceTab(pITmp,Li,Co))
   {
      CheckNull(pTab->V.tfVal,NomProc,pTab);
      pTmp  = (mtPTRFLOAT) pTab->V.tfVal;
      pTVal = *(pTmp+((Li*pITmp->MaxCol)+Co));
   }
   else
      ExecError(NomProc,ERRBOUNDARRAY,pTab);
   return(pTVal);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Met le pointeur caractre donn dans le tableau donn, … la position donne.
//
void ArrayCStore(mtPTRDICO pTab,mtPTRDICO pLig,mtPTRDICO pCol,mtPTRBYTE ptr)
{
   mtPTRITAB pITmp;
   mtUWORD Li,Co;

   Li    = (mtUWORD) pLig->V.fVal;
   Co    = (mtUWORD) pCol->V.fVal;
   pITmp = (mtPTRITAB) pTab->XVal;
   if(IndiceTab(pITmp,Li,Co))
      *((pPOINTEUR) pTab->V.tVal+((Li*pITmp->MaxCol)+Co)) = ptr;
   else
      ExecError(pCurrCV->pNProc,ERRBOUNDARRAY,pTab);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Met le flottant donn dans le tableau donn, … la position donne.
//
void ArrayNStore(mtPTRDICO pTab,mtPTRDICO pLig,mtPTRDICO pCol,float Value)
{
   mtPTRITAB pITmp;
   mtUWORD Li,Co;

   Li    = (mtUWORD) pLig->V.fVal;
   Co    = (mtUWORD) pCol->V.fVal;
   pITmp = (mtPTRITAB) pTab->XVal;
   if(IndiceTab(pITmp,Li,Co))
      *(pTab->V.tfVal+((Li*pITmp->MaxCol)+Co)) = Value;
   else
      ExecError(pCurrCV->pNProc,ERRBOUNDARRAY,pTab);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute si ncssaire une page … la pile de donnes du connect
// courant.
//
void _IncDStackSize(void)
{
   if((pCurrCV->SP % STACKPAGE) == 0)
   {
      pCurrCV->nPageDS++;
      pCurrCV->PileD = (pPOINTEUR) MemReAlloc(pCurrCV->PileD,(pCurrCV->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve si ncssaire une page (sauf la 1re) … la pile de donnes du
// connect courant.
//
void _DecDStackSize(void)
{
   if((pCurrCV->nPageDS>1) && ((pCurrCV->SP % STACKPAGE) == 0))
   {
      pCurrCV->nPageDS--;
      pCurrCV->PileD = (pPOINTEUR) MemReAlloc(pCurrCV->PileD,(pCurrCV->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute si ncssaire une page … la pile de retour du connect
// courant.
//
void _IncRStackSize(void)
{
   if((pCurrCV->RP % STACKPAGE) == 0)
   {
      pCurrCV->nPageRS++;
      pCurrCV->PileR = (pPOINTEUR) MemReAlloc(pCurrCV->PileR,(pCurrCV->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve si ncssaire une page (sauf la 1re) … la pile de retour du connect
// courant.
//
void _DecRStackSize(void)
{
   if((pCurrCV->nPageRS>1) && ((pCurrCV->RP % STACKPAGE) == 0))
   {
      pCurrCV->nPageRS--;
      pCurrCV->PileR = (pPOINTEUR) MemReAlloc(pCurrCV->PileR,(pCurrCV->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le pointeur donn au sommet de la pile de donnes du connect.
//
void _DPush(mtPOINTEUR ptr)
{
   if(pCurrCV->pApp->StackSize)
   {
      if(pCurrCV->SP >= pCurrCV->pApp->StackSize)
         ExecError(pCurrCV->pNProc,ERRDPILEPLEINE,NULL);
   }
   pCurrCV->PileD[pCurrCV->SP++] = ptr;
   _IncDStackSize();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile le pointeur prsent au sommet de la pile de donnes du connect.
//
mtPOINTEUR _DPop(void)
{
   mtPOINTEUR ptr = NULL;

   if(pCurrCV->SP)
   {
      _DecDStackSize();
      pCurrCV->SP--;
      ptr = pCurrCV->PileD[pCurrCV->SP];
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
   return(ptr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le pointeur donn au sommet de la pile de retour du connect.
//
void _RPush(mtPOINTEUR ptr)
{
   if(pCurrCV->pApp->StackSize)
   {
      if(pCurrCV->RP >= pCurrCV->pApp->StackSize)
         ExecError(pCurrCV->pNProc,ERRRPILEPLEINE,NULL);
   }
   pCurrCV->PileR[pCurrCV->RP++] = ptr;
   _IncRStackSize();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dpile le pointeur prsent au sommet de la pile de retour du connect.
//
mtPOINTEUR _RPop(void)
{
   mtPOINTEUR ptr = NULL;

   if(pCurrCV->RP)
   {
      _DecRStackSize();
      pCurrCV->RP--;
      ptr = pCurrCV->PileR[pCurrCV->RP];
   }
   else
      ExecError(pCurrCV->pNProc,ERRRPILEVIDE,NULL);
   return(ptr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Vide la pile des symboles internes du connect donn.
//
void DelIDico(mtPTRCV LeCv)
{
   mtPTRDICO sSym;

   if(! LeCv->IsLTask)
   {
      if(LeCv->pIDico)
      {
         while(LeCv->ISP)
         {
            LeCv->ISP--;
            sSym = (mtPTRDICO) LeCv->pIDico[LeCv->ISP];
            if(sSym)
            {
               DelSymValue(sSym);
               MemFree(sSym);
            }
         }
         MemFree(LeCv->pIDico);
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Prpare l'appel d'une procdure du type "2 points"
// ( : xxx ... ;)
//
void _2Points (void)
{
   mtPOINTEUR ptrSeg,ptrOff;
   static char cProcName[] = ":";

   pCurrCV->pNProc = cProcName;
   // On rcupre le Segment et l'IP de la procdure.
   ptrSeg = (mtPOINTEUR) pCurrCV->pHere[pCurrCV->IP++];
   ptrOff = (mtPOINTEUR) pCurrCV->pHere[pCurrCV->IP++];
   // Ici, IP pointe sur l'instruction suivant l'appel.
   _RPush((mtPOINTEUR) pCurrCV->IP);
   _RPush((mtPOINTEUR) pCurrCV->pHere);
   pCurrCV->pHere = (ppFUNC) ptrSeg;
   pCurrCV->IP    = (mtUWORD) ptrOff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Prpare le retour … la procdure appelante.
//
void _Next(void)
{
   static char cProcName[] = ";";

   pCurrCV->pNProc = cProcName;
   pCurrCV->pHere  = (ppFUNC) _RPop();
   pCurrCV->IP     = (mtUWORD) _RPop();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Prpare le retour … la procdure principale (MAIN).
//
void _MNext(void)
{
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;
   static char cProcName[] = ";";

   d4init_undo(&pCurrCV->CBase);
   pCurrCV->CurrBase = (DATA4 *) NULL;
   pCurrCV->pNProc = cProcName;
   pCurrCV->pHere  = pCurrCV->pApp->pHere;
   pCurrCV->IP     = pCurrCV->pApp->ENTRY;
   MemFree(pCurrCV->pData);
   MemFree(pCurrCV->pOutBuff);
   MemFree(pCurrCV->pUsing);
   MemFree(pCurrCV->pLInput);
   if(pCurrCV->MLITabLine)
   {
      pDst = pCurrCV->MLITabLine;
      for(i=0;i<pCurrCV->MLIMaxLine;i++)
      {
         pLig = (mtPTRBYTE) *(pDst+i);
         MemFree(pLig);
      }
      MemFree(pDst);
   }
   pCurrCV->pData      = NULL;
   pCurrCV->pOutBuff   = NULL;
   pCurrCV->pUsing     = NULL;
   pCurrCV->pLInput    = NULL;
   pCurrCV->MLITabLine = NULL;
   // Libration des symboles internes.
   DelIDico(pCurrCV);
   // Allocation de la 1re page de la pile des symboles
   // internes.
   pCurrCV->nPageI   = 1;
   pCurrCV->pIDico   = (pPOINTEUR) MemAlloc((pCurrCV->nPageI*ISYMPAGE)*sizeof(mtPOINTEUR));
   pCurrCV->NbISym   = 0;
   pCurrCV->ISP      = 0;
   pCurrCV->IGC      = 0;
   // On vide les piles.
   MemFree(pCurrCV->PileD);
   MemFree(pCurrCV->PileR);
   // Allocation des 1res pages des piles.
   pCurrCV->nPageDS  = 1;
   pCurrCV->PileD    = (pPOINTEUR) MemAlloc((pCurrCV->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   pCurrCV->nPageRS  = 1;
   pCurrCV->PileR    = (pPOINTEUR) MemAlloc((pCurrCV->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de la procdure ONLIB.
//
void _OLNext(void)
{
   DeconnecteCv(pCurrCV);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin de la procdure GTASK.
//
void _GTNext(void)
{
   DeconnecteGTCv(pCurrCV->pApp);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute si ncssaire une page … la pile des symboles internes du connect
// courant.
//
void _IncStackISym(void)
{
   if((pCurrCV->ISP % ISYMPAGE) == 0)
   {
      pCurrCV->nPageI++;
      pCurrCV->pIDico = (pPOINTEUR) MemReAlloc(pCurrCV->pIDico,(pCurrCV->nPageI*ISYMPAGE)*sizeof(mtPOINTEUR));
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnration d'un symbole (initialis) pour le cv courant.
// Le symbole est empil dans sa pile de symboles internes.
// Renvoie un pointeur sur le nouveau symbole.
//
mtPTRDICO GenCvISym(mtBYTE pTyp,mtPOINTEUR pInit,mtBYTE cAttr)
{
   mtPTRDICO NewD;
   mtUWORD uISP=0;

   NewD       = (mtPTRDICO) MemAlloc(sizeof(Dico));
   NewD->Type = pTyp;
   if(pInit)
      NewD->V.pVal = pInit;
   NewD->Attr = cAttr;
   if(pCurrCV->IsLTask)
   {
      // Tche locale, on prend les infos du cv propritaire.
      // (Le dictionnaire des symboles internes est commun)
      pCurrCV->nPageI = pCurrLT->nPageI;
      pCurrCV->pIDico = pCurrLT->pIDico;
      pCurrCV->ISP    = pCurrLT->ISP;
      pCurrCV->NbISym = pCurrLT->NbISym;
   }
   //
   // On empile le nouveau symbole.
   // On cherche un emplacement rcupr par le ramasse-miettes.
   //
   while(uISP<pCurrCV->ISP)
   {
      if((mtPOINTEUR) pCurrCV->pIDico[uISP] == (mtPOINTEUR) 0)
      {
         pCurrCV->pIDico[uISP] = (mtPOINTEUR) NewD;
         break;
      }
      uISP++;
   }
   if(uISP == pCurrCV->ISP)
   {
      pCurrCV->pIDico[pCurrCV->ISP++] = (mtPOINTEUR) NewD;
      _IncStackISym();
   }
   pCurrCV->NbISym++;
   if(pCurrCV->IsLTask)
   {
      // Tche locale, on met … jour les infos du dico interne.
      pCurrLT->nPageI = pCurrCV->nPageI;
      pCurrLT->pIDico = pCurrCV->pIDico;
      pCurrLT->ISP    = pCurrCV->ISP;
      pCurrLT->NbISym = pCurrCV->NbISym;
   }
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Duplique le symbole donn.
// Renvoie un pointeur sur le nouveau symbole
//
mtPTRDICO DupliqueSym(mtPTRDICO pDSym)
{
   mtPTRDICO NewD;
   mtUWORD Li,Co;
   mtPTRITAB pITmp,pINew;
   pPOINTEUR pTmp;
   mtPTRFLOAT pfTmp;
   mtPTRBYTE pCh,pNewCh;
   float pDb,pNewDb;

   NewD = GenCvISym(pDSym->Type,NULL,SYMVARCON);
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         //if(pDSym->V.cVal)
         //{
         NewD->V.cVal = (mtPTRBYTE) MemAlloc(strlen(pDSym->V.cVal)+1);
         strcpy(NewD->V.cVal,pDSym->V.cVal);
         //}
         break;
      }
      case mtTABCHAR:
      {
         pITmp         = (mtPTRITAB) pDSym->XVal;
         NewD->V.tVal  = (pPOINTEUR) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(mtPOINTEUR));
         pINew         = (mtPTRITAB) MemAlloc(sizeof(InfTab));
         pINew->MaxLig = pITmp->MaxLig;
         pINew->MaxCol = pITmp->MaxCol;
         NewD->XVal    = (mtPOINTEUR) pINew;
         //
         // On duplique le contenu du tableau.
         pTmp  = pDSym->V.tVal;
         for(Li=0;Li<pITmp->MaxLig;Li++)
         {
            for(Co=0;Co<pITmp->MaxCol;Co++)
            {
               pCh = (mtPTRBYTE) *(pTmp+((Li*pITmp->MaxCol)+Co));
               //if(pCh)
               //{
                  pNewCh = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
                  strcpy(pNewCh,pCh);
                  *((pPOINTEUR) NewD->V.tVal+((Li*pINew->MaxCol)+Co)) = pNewCh;
               //}
            }
         }
         break;
      }
      case mtTNUMBER:
      {
         NewD->V.fVal = pDSym->V.fVal;
         break;
      }
      case mtTABNUMBER:
      {
         pITmp         = (mtPTRITAB) pDSym->XVal;
         NewD->V.tfVal = (mtPTRFLOAT) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(float));
         pINew         = (mtPTRITAB) MemAlloc(sizeof(InfTab));
         pINew->MaxLig = pITmp->MaxLig;
         pINew->MaxCol = pITmp->MaxCol;
         NewD->XVal    = (mtPOINTEUR) pINew;
         //
         // On duplique le contenu du tableau.
         pfTmp = pDSym->V.tfVal;
         for(Li=0;Li<pITmp->MaxLig;Li++)
         {
            for(Co=0;Co<pITmp->MaxCol;Co++)
            {
               pDb = *(pfTmp+((Li*pITmp->MaxCol)+Co));
               *(NewD->V.tfVal+((Li*pINew->MaxCol)+Co)) = pDb;
            }
         }
         break;
      }
   }
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnration d'un symbole interne de type numrique simple (pas tableau)
// Renvoie un pointeur sur le symbole cr.
//
mtPTRDICO GenCvINUMBER(void)
{
   mtPTRDICO NewD;

   NewD = GenCvISym(mtTNUMBER,NULL,SYMVARCON);
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Gnration d'un symbole interne de type caractre simple (pas tableau)
// Renvoie un pointeur sur le symbole cr.
//
mtPTRDICO GenCvICHAR(mtWORD Lng)
{
   mtPTRDICO NewD;

   NewD         = GenCvISym(mtTCHAR,NULL,SYMVARCON);
   NewD->V.cVal = (mtPTRBYTE) MemAlloc(Lng);
   return(NewD);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration d'un symbole (non typ) sur la pile du connect.
//
void RecupSym(pPDICO pSym,pPDICO pLig,pPDICO pCol,mtPTRBYTE NomProc,mtBOOL bLit)
{
   mtPTRDICO pSRec,pSLi,pSCo;

   pSRec = (mtPTRDICO) _DPop();
   if(bLit)
      CheckLiteral(pSRec,NomProc);
   *pSym = pSRec;
   if((pSRec->Type == mtTABNUMBER) || (pSRec->Type == mtTABCHAR))
   {
      pSCo  = _DPop();
      CheckNumber(pSCo,NomProc);
      pSLi  = _DPop();
      CheckNumber(pSLi,NomProc);
      *pLig = pSLi;
      *pCol = pSCo;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration d'un symbole de type numrique sur la pile du connect.
//
float RecupNumber(pPDICO pSym,pPDICO pLig,pPDICO pCol,mtPTRBYTE NomProc,mtBOOL bLit)
{
   mtPTRDICO pSRec,pSLi,pSCo;
   float pDRet;

   pSRec = (mtPTRDICO) _DPop();
   CheckNumber(pSRec,NomProc);
   if(bLit)
      CheckLiteral(pSRec,NomProc);
   *pSym = pSRec;
   pDRet = pSRec->V.fVal;
   if(pSRec->Type == mtTABNUMBER)
   {
      pSCo  = _DPop();
      CheckNumber(pSCo,NomProc);
      pSLi  = _DPop();
      CheckNumber(pSLi,NomProc);
      pDRet = ArrayNVal(pSRec,pSLi,pSCo,NomProc);
      *pLig = pSLi;
      *pCol = pSCo;
   }
   return(pDRet);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration d'un symbole de type caractre sur la pile du connect.
//
mtPTRBYTE RecupString(pPDICO pSym,pPDICO pLig,pPDICO pCol,mtPTRBYTE NomProc,mtBOOL bLit)
{
   mtPTRDICO pSRec,pSLi,pSCo;
   mtPTRBYTE pCRet;

   pSRec = (mtPTRDICO) _DPop();
   CheckString(pSRec,NomProc);
   if(bLit)
      CheckLiteral(pSRec,NomProc);
   *pSym = pSRec;
   pCRet = pSRec->V.cVal;
   if(pSRec->Type == mtTABCHAR)
   {
      pSCo  = _DPop();
      CheckNumber(pSCo,NomProc);
      pSLi  = _DPop();
      CheckNumber(pSLi,NomProc);
      pCRet = ArrayCVal(pSRec,pSLi,pSCo,NomProc);
      *pLig = pSLi;
      *pCol = pSCo;
   }
   return(pCRet);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre d'lments prsents sur la pile de donnes du connect
// courant.
//
void _NStack(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "STACK#";

   pCurrCV->pNProc = cProcName;
   pSym            = GenCvINUMBER();
   pSym->V.fVal    = pCurrCV->SP;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                           GESTION DU MINITEL
//                           ออออออออออออออออออ
// Translation des accents d'une chaine.
// Sens = 0 pour translation accent -> squence escape
//      = 1 pour translation squence escape -> accent
// Renvoie un pointeur sur la nouvelle chaine.
mtPTRBYTE TrAccents(mtPTRBYTE Chaine,mtBYTE Sens)
{
   int i,pNew=0,Depl;
   mtPTRBYTE NewChaine;
   mtUDWORD tailC;

   tailC = strlen(Chaine)+1;
   NewChaine = (mtPTRBYTE) MemAlloc(tailC);
   if(Sens)
   {
      // translation squence escape -> accent
      while(*Chaine)
      {
         Depl = 1;
         if(*Chaine == '')
         {
            for(i=0;i<NBACCENTS;i++)
            {
               if(strncmp(Chaine,tAccents[i].SeqEsc,strlen(tAccents[i].SeqEsc)) == 0)
               {
                  // Remplacement de la squence escape trouve
                  tailC -= strlen(tAccents[i].SeqEsc)-1;
                  NewChaine = (mtPTRBYTE) MemReAlloc(NewChaine,tailC);
                  *(NewChaine+pNew) = tAccents[i].Accent;
                  Depl = strlen(tAccents[i].SeqEsc);
                  break;
               }
            }
         }
         else
            *(NewChaine+pNew) = *Chaine;
         Chaine += Depl;
         pNew++;
      }
   }
   else
   {
      // translation accent -> squence escape
      while(*Chaine)
      {
         Depl = 1;
         if(*Chaine > 127)
         {
            for(i=0;i<NBACCENTS;i++)
            {
               if(*Chaine == tAccents[i].Accent)
               {
                  // Remplacement de l'accent trouv
                  tailC += strlen(tAccents[i].SeqEsc)-1;
                  NewChaine = (mtPTRBYTE) MemReAlloc(NewChaine,tailC);
                  strncpy(NewChaine+pNew,tAccents[i].SeqEsc,strlen(tAccents[i].SeqEsc));
                  Depl = strlen(tAccents[i].SeqEsc);
                  break;
               }
            }
         }
         else
            *(NewChaine+pNew) = *Chaine;
         Chaine++;
         pNew += Depl;
      }
   }
   *(NewChaine+pNew) = '\0';
   return(NewChaine);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Efface l'cran du minitel.
// Syntaxe : CLS
//
void _Cls(void)
{
   static char cProcName[] = "CLS";
   mtBYTE OBuff[10];

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   if(pCurrCV->cMode == TMODE_VDT)
      sprintf(OBuff,"\x0C");
   else
      // CLS+LOCATE 1,1
      sprintf(OBuff,"%s2J",CSI);
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->cMode == TMODE_VDT)
   {
      // Mise … jour de la position du curseur.
      pCurrCV->Row      = 1;
      pCurrCV->Col      = 0;
   }
   pCurrCV->bGCursor = mtTRUE;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Demande d'identification du minitel.
// Syntaxe : ENQROM
//
void _EnqRom(void)
{
   mtBYTE OBuff[4];
   static char cProcName[] = "ENQROM";

   pCurrCV->pNProc = cProcName;
   sprintf(OBuff,"%s\x7B",PRO1);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Demande de la position du curseur.
//
void _GetCursor(void)
{
   mtBYTE OBuff[3];
   static char cProcName[] = "GETCURSORPOS";

   pCurrCV->pNProc = cProcName;
   sprintf(OBuff,"\x1B\x61");
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   pCurrCV->bGCursor = mtFALSE;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition de la couleur des caractres.
// Syntaxe : Couleur SETCOLOR
//           Couleur = 0 pour NOIR
//                     1  "   ROUGE
//                     2  "   VERT
//                     3  "   JAUNE
//                     4  "   BLEU
//                     5  "   MAGENTA
//                     6  "   CYAN
//                     7  "   BLANC
//
void _SetColor(void)
{
   float coul;
   mtBYTE OBuff[3],cCoul;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "SETCOLOR";

   pCurrCV->pNProc = cProcName;
   coul  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   cCoul = coul;
   sprintf(OBuff,"\x1B%c",0x40+cCoul);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition de la couleur de fond.
// Syntaxe : Couleur SETBACKGROUND
//           Couleur = 0 pour NOIR
//                     1  "   ROUGE
//                     2  "   VERT
//                     3  "   JAUNE
//                     4  "   BLEU
//                     5  "   MAGENTA
//                     6  "   CYAN
//                     7  "   BLANC
//
void _SetBackGround(void)
{
   float coul;
   mtBYTE OBuff[3],cCoul;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "SETBACKGROUND";

   pCurrCV->pNProc = cProcName;
   coul  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   cCoul = coul;
   sprintf(OBuff,"\x1B%c ",0x50+cCoul);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Demande la position du curseur.
// Syntaxe : GETCURSORPOS Lig Col
//
void _GetCursorPos(void)
{
   mtPTRDICO pSLig,pSCol;

   if(! pCurrCV->bGCursor)
   {
      // On rappelle GETCURSORPOS, au prochain paquet, on aura
      // la position du curseur.
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
   else
   {
      pSLig         = GenCvINUMBER();
      pSLig->V.fVal = pCurrCV->Row;
      pSCol         = GenCvINUMBER();
      pSCol->V.fVal = pCurrCV->Col;
      _DPush(pSLig);
      _DPush(pSCol);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution de locate.
//
void __Locate(void)
{
   float lig,col;
   mtBYTE OBuff[10],cLig,cCol;
   mtPTRDICO pDSym,pLi,pCo;

   col  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   lig  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   cCol = col;
   cLig = lig;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   if(pCurrCV->cMode == TMODE_VDT)
      sprintf(OBuff,"\x1F%c%c",0x40+cLig,0x40+cCol);
   else
      sprintf(OBuff,"%s%c%c\x3B%c%c\x48",CSI,0x30+((mtBYTE)(cLig/10)),0x30+((mtBYTE)(cLig%10)),0x30+((mtBYTE)(cCol/10)),0x30+((mtBYTE)(cCol%10)));
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   // Mise … jour de la position du curseur.
   pCurrCV->Row      = cLig;
   pCurrCV->Col      = cCol;
   pCurrCV->bGCursor = mtTRUE;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Positionnement du curseur.
// Syntaxe : Ligne Colonne LOCATE
//
void _Locate(void)
{
   static char cProcName[] = "LOCATE";

   pCurrCV->pNProc = cProcName;
   __Locate();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un message en ligne 0.
// Syntaxe : msg LIGNE0
//
void _Ligne0(void)
{
   mtPTRBYTE str;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "LINE0";

   pCurrCV->pNProc = cProcName;
   str = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(str,"Line0",pDSym);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   if(pCurrCV->cMode == TMODE_VDT)
   {
      _DPush(pCONSTLINE0);                  // Positionnement.
      _DPush(pCONSTCOL1);
      __Locate();
   }
   else
      AppendCvOBuff(pCurrCV,"\x1F\x40\x41",mtFALSE);
   AppendCvOBuff(pCurrCV,"\x18",mtFALSE);   // Effacement de la ligne.
   AppendCvOBuff(pCurrCV,str,mtTRUE);
   AppendCvOBuff(pCurrCV,"\x0A",mtFALSE);   // Envoi d'un LF.
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effacement de la ligne 0
//
void _Clear0(void)
{
   static char cProcName[] = "CLEAR0";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTLINE0);                     // Positionnement.
   _DPush(pCONSTCOL1);
   __Locate();
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x18",mtFALSE);   // Effacement de la ligne.
   AppendCvOBuff(pCurrCV,"\x0A",mtFALSE);   // Envoi d'un LF.
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un message en ligne 0 … un connect donn.
// Utilise par certaines commandes systme.
//
void Send0(mtPTRBYTE Msg,mtPTRCV pCv)
{
   mtPTRCV OldCv=pCurrCV;
   mtPTRDICO pSmsg;

   pCurrCV = pCv;
   pSmsg   = GenCvICHAR(strlen(Msg)+1);
   strcpy(pSmsg->V.cVal,Msg);
   _DPush(pSmsg);
   _Ligne0();
   pCurrCV = OldCv;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dfinition de la taille des caractres.
// Syntaxe : Taille SETFONT
//           Taille = 0 pour SIMPLE TAILLE
//                    1  "   DOUBLE HAUTEUR
//                    2  "   DOUBLE LARGEUR
//                    3  "   DOUBLE TAILLE
//
void _SetFont(void)
{
   float tail;
   mtBYTE OBuff[3],cTail;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "SETFONT";

   pCurrCV->pNProc = cProcName;
   tail  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   cTail = tail;
   sprintf(OBuff,"\x1B%c",0x4C+cTail);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rptition d'un caractre.
// Syntaxe : c n REPLICATE
//
void _Replicate(void)
{
   float nb;
   mtPTRBYTE str;
   mtBYTE OBuff[3],cNb;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "REPLICATE";

   pCurrCV->pNProc = cProcName;
   nb  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   str = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(str,"Replicate",pDSym);
   cNb = nb;
   sprintf(OBuff,"\x12%c",0x3F+cNb);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,str,mtTRUE);
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise le mode du terminal du connect courant.
//
void _SetTMode(void)
{
   mtPTRDICO pS1,pLi,pCo;
   float d1;
   mtWORD w1;
   mtBYTE OBuff[10];
   static char cProcName[] = "SETTMODE";

   pCurrCV->pNProc = cProcName;
   d1 = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   w1 = d1;
   memset(OBuff,0,10);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   switch(w1)
   {
      case TMODE_VDT:
      {
         switch(pCurrCV->cMode)
         {
            case TMODE_MIXTE:
            {
               sprintf(OBuff,"%s\x32\x7E",PRO2);
               pCurrCV->cMode = TMODE_VDT;
               break;
            }
            case TMODE_TELINFO:
            {
               sprintf(OBuff,"%s\x3F\x7B",CSI);
               pCurrCV->cMode = TMODE_VDT;
               break;
            }
         }
         break;
      }
      case TMODE_MIXTE:
      {
         switch(pCurrCV->cMode)
         {
            case TMODE_VDT:
            {
               sprintf(OBuff,"%s\x32\x7D",PRO2);
               pCurrCV->cMode = TMODE_MIXTE;
               break;
            }
         }
         break;
      }
      case TMODE_TELINFO:
      {
         switch(pCurrCV->cMode)
         {
            case TMODE_VDT:
            {
               sprintf(OBuff,"%s\x31\x7D",PRO2);
               pCurrCV->cMode = TMODE_TELINFO;
               break;
            }
            case TMODE_MIXTE:
            {
               sprintf(OBuff,"%s\x31\x7D",PRO2);
               pCurrCV->cMode = TMODE_TELINFO;
               break;
            }
         }
         break;
      }
   }
   AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un fichier vers le minitel
// Syntaxe : adr SENDPAGE
//
void _SendPage(void)
{
   mtWORD fPag;
   mtPTRBYTE NPag,LaPage;
   char FPage[MAXPATH],*NomFic;
   mtUDWORD ftail;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "SENDPAGE";

   pCurrCV->pNProc = cProcName;
   NPag = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NPag,"SendPage",pDSym);
   NomFic = filespecname(NPag);
   if(strlen(NomFic) == strlen(NPag))
   {
      // Pas de chemin spcifi.
      if(pCurrCV->pApp->PathVTX)
      {
         // On ajoute le chemin des pages.
         strcpy(FPage,pCurrCV->pApp->PathVTX);
         strcat(FPage,NPag);
      }
      else
      {
         if(pCurrCV->pApp->Path)
         {
            // On ajoute le chemin principal.
            strcpy(FPage,pCurrCV->pApp->Path);
            strcat(FPage,NPag);
         }
         else
            strcpy(FPage,NPag);
      }
   }
   else
      strcpy(FPage,NPag);
   if(_FileOpen(FPage,O_BINARY,&fPag,mtFALSE))
   {
      ftail  = filelength(fPag);
      LaPage = (mtPTRBYTE) MemAlloc(ftail+1);
      _FileRead(fPag,LaPage,ftail+1);
      _FileClose(fPag);
      if(*(LaPage+(ftail-1)) == 26)
         *(LaPage+(ftail-1)) = '\0';
      if(pCurrCV->IsLTask)
         // Tche locale, on prend le pointeur du cv propritaire.
         // (le buffer de sortie est commun)
         pCurrCV->pOutBuff = pCurrLT->pOutBuff;
      AppendCvOBuff(pCurrCV,LaPage,mtFALSE);
      if(pCurrCV->IsLTask)
         // Tche locale, on met … jour le pointeur du cv propritaire.
         // (le buffer de sortie est commun)
         pCurrLT->pOutBuff = pCurrCV->pOutBuff;
      MemFree(LaPage);
   }
   else
      ExecError(pCurrCV->pNProc,ERRFILEOPEN,pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un caractre quelconque vers le minitel.
// Syntaxe : car EMIT
//
void _Emit(void)
{
   float pCar;
   mtBYTE OBuff[2],cCar;
   mtPTRDICO pDSym,pLi,pCo;
   static char cProcName[] = "EMIT";

   pCurrCV->pNProc = cProcName;
   pCar = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   cCar = pCar;
   sprintf(OBuff,"%c",cCar);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,OBuff,mtTRUE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise le format d'affichage des nombres.
// Syntaxe : ChaineFormat USING
//
void _Using(void)
{
   mtPTRDICO pFmt,pLi,pCo;
   mtPTRBYTE pFormat;
   mtBYTE cFmt[30];
   static char cProcName[] = "USING";

   pCurrCV->pNProc = cProcName;
   pFormat = RecupString(&pFmt,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pFormat,"Using",pFmt);
   strcpy(cFmt,"%+-0123456789.eEfgGbdilouxX ");
   if(strspn(pFormat,cFmt) == strlen(pFormat))
   {
      if(pCurrCV->pUsing)
      {
         pCurrCV->pUsing = (mtPTRBYTE) MemReAlloc(pCurrCV->pUsing,strlen(pFormat)+1);
         memset(pCurrCV->pUsing,0,strlen(pFormat)+1);
      }
      else
         pCurrCV->pUsing = (mtPTRBYTE) MemAlloc(strlen(pFormat)+1);
      strcpy(pCurrCV->pUsing,pFormat);
   }
   else
      ExecError(pCurrCV->pNProc,ERRUSINGFAIL,pFmt);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive le formatages des nombres dfinit avec USING.
//
void _NoUsing(void)
{
   static char cProcName[] = "USINGOFF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->pUsing)
   {
      MemFree(pCurrCV->pUsing);
      pCurrCV->pUsing = NULL;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la chaine de format courante.
//
void _GetUsing(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "GETUSING";

   pCurrCV->pNProc = cProcName;
   pSym            = GenCvICHAR(1);
   if(pCurrCV->pUsing)
   {
      pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(pCurrCV->pUsing)+1);
      memset(pSym->V.cVal,0,strlen(pCurrCV->pUsing)+1);
      strcpy(pSym->V.cVal,pCurrCV->pUsing);
   }
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage d'un symbole … l'cran.
// Syntaxe : symb PRINT
//
void _Print(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   mtBYTE OBuff[128];
   float pDTmp;
   mtPTRBYTE pCTmp;
   mtWORD Value;
   static char cProcName[] = "PRINT";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         // CheckNull(pDSym->V.cVal,"Print",pDSym);
         AppendCvOBuff(pCurrCV,pDSym->V.cVal,mtTRUE);
         break;
      }
      case mtTABCHAR:
      {
         pCTmp = ArrayCVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         // CheckNull(pCTmp,"Print",pDSym);
         AppendCvOBuff(pCurrCV,pCTmp,mtTRUE);
         break;
      }
      case mtTNUMBER:
      {
         if(pCurrCV->pUsing)
         {
            if(strpbrk(pCurrCV->pUsing,"eEfgG"))
               sprintf(OBuff,pCurrCV->pUsing,pDSym->V.fVal);
            else
            {
               Value = pDSym->V.fVal;
               sprintf(OBuff,pCurrCV->pUsing,Value);
            }
         }
         else
            sprintf(OBuff,"%f",pDSym->V.fVal);
         AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
         break;
      }
      case mtTABNUMBER:
      {
         pDTmp = ArrayNVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         if(pCurrCV->pUsing)
         {
            if(strpbrk(pCurrCV->pUsing,"eEfgG"))
               sprintf(OBuff,pCurrCV->pUsing,pDTmp);
            else
            {
               Value = pDTmp;
               sprintf(OBuff,pCurrCV->pUsing,Value);
            }
         }
         else
            sprintf(OBuff,"%f",pDTmp);
         AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
         break;
      }
   }
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage format d'un symbole … l'cran.
// Syntaxe : symb format PRINT
//
void _PrintUsing(void)
{
   mtPTRDICO pDSym,pFmt,pLi,pCo;
   mtBYTE OBuff[128];
   float pDTmp;
   mtPTRBYTE pCTmp,pFormat;
   mtWORD Value;
   mtBYTE cFmt[30];
   static char cProcName[] = "PRINTUSING";

   pCurrCV->pNProc = cProcName;
   pFormat = RecupString(&pFmt,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   RecupSym(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   strcpy(cFmt,"%+-0123456789.eEfgGbdilouxX ");
   if(strspn(pFormat,cFmt) != strlen(pFormat))
      ExecError(pCurrCV->pNProc,ERRUSINGFAIL,pFmt);
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         // CheckNull(pDSym->V.cVal,"Print",pDSym);
         AppendCvOBuff(pCurrCV,pDSym->V.cVal,mtTRUE);
         break;
      }
      case mtTABCHAR:
      {
         pCTmp = ArrayCVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         // CheckNull(pCTmp,"Print",pDSym);
         AppendCvOBuff(pCurrCV,pCTmp,mtTRUE);
         break;
      }
      case mtTNUMBER:
      {
         if(strpbrk(pFormat,"eEfgG"))
            sprintf(OBuff,pFormat,pDSym->V.fVal);
         else
         {
            Value = pDSym->V.fVal;
            sprintf(OBuff,pFormat,Value);
         }
         AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
         break;
      }
      case mtTABNUMBER:
      {
         pDTmp = ArrayNVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         if(strpbrk(pFormat,"eEfgG"))
            sprintf(OBuff,pFormat,pDTmp);
         else
         {
            Value = pDTmp;
            sprintf(OBuff,pFormat,Value);
         }
         AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
         break;
      }
   }
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi un retour chariot (CRLF) au minitel.
// Syntaxe : CR
//
void _Cr(void)
{
   static char cProcName[] = "CR";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x0D\x0A",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
   // Mise … jour de la position du curseur.
   _GetCursor();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rend le curseur visible.
// Syntaxe : CURON
//
void _CursorOn(void)
{
   static char cProcName[] = "CURSORON";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x11",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rend le curseur invisible.
// Syntaxe : CUROFF
//
void _CursorOff(void)
{
   static char cProcName[] = "CURSOROFF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x14",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi un beep.
// Syntaxe : BEEP
//
void _MBeep(void)
{
   static char cProcName[] = "BEEP";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x07",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Active le clignotement
// Syntaxe : BLINKON
//
void _CligOn(void)
{
   static char cProcName[] = "BLINKON";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x48",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive le clignotement
// Syntaxe : BLINKOFF
//
void _CligOff(void)
{
   static char cProcName[] = "BLINKOFF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x49",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Active le soulignement
// Syntaxe : UNDERLON
//
void _SouligOn(void)
{
   static char cProcName[] = "UNDERLON";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x5A",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive le soulignement
// Syntaxe : UNDERLOFF
//
void _SouligOff(void)
{
   static char cProcName[] = "UNDERLOFF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x59",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Active l'inversion vido
// Syntaxe : INVERTON
//
void _InverseOn(void)
{
   static char cProcName[] = "INVERTON";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x5D",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive l'inversion vido
// Syntaxe : INVERTOFF
//
void _InverseOff(void)
{
   static char cProcName[] = "INVERTOFF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B\x5C",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Passage en majuscules.
// Syntaxe : UPPER
//
void _MUpper(void)
{
   static char cProcName[] = "UPPER";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B:jE",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Passage en minuscules.
// Syntaxe : LOWER
//
void _MLower(void)
{
   static char cProcName[] = "LOWER";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,"\x1B:iE",mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive de saisie.
// Syntaxe : INPUT
//
// Touche gres en dition : ANNULATION, CORRECTION
//
void _MInput(void)
{
   mtBOOL bSortie = mtFALSE;
   mtPTRDICO pSym;
   int i;
   static char cProcName[] = "INPUT";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->bPaqLu)
   {
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on prend les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrCV->pData    = pCurrLT->pData;
         pCurrCV->pOutBuff = pCurrLT->pOutBuff;
      }
      // On rcupre les donnes.
      if(pCurrCV->pData)
      {
         if(! pCurrCV->pLInput)
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemAlloc(strlen(pCurrCV->pData)+1);
            strcpy(pCurrCV->pLInput,pCurrCV->pData);
         }
         else
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemReAlloc(pCurrCV->pLInput,(strlen(pCurrCV->pLInput)+strlen(pCurrCV->pData))+1);
            strcat(pCurrCV->pLInput,pCurrCV->pData);
         }
      }
      switch(pCurrCV->TFonction)
      {
         case MCorrection:
         {
            if(pCurrCV->pLInput)
            {
               // On enlve le dernier caractre.
               *((mtPTRBYTE) pCurrCV->pLInput+(strlen(pCurrCV->pLInput)-1)) = '\0';
               // On envoi BACKSPACE+ESPACE+BACKSPACE.
               AppendCvOBuff(pCurrCV,"\x08 \x08",mtFALSE);
               if(!strlen(pCurrCV->pLInput))
               {
                  // Il n'y a plus rien
                  MemFree(pCurrCV->pLInput);
                  pCurrCV->pLInput = NULL;
               }
            }
            break;
         }
         case MAnnulation:
         {
            if(pCurrCV->pLInput)
            {
               // On recule de n caractres.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               // On envoi n espaces.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV," ",mtFALSE);
               // On recule de n caractres.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               MemFree(pCurrCV->pLInput);
               pCurrCV->pLInput = NULL;
            }
            break;
         }
         default:
         {
            // Toute autre touche provoque la fin de la saisie.
            pSym = GenCvICHAR(1);
            if(pCurrCV->pLInput)
            {
               // WinInfo("INPUT=%s",pCurrCV->pLInput);
               pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(pCurrCV->pLInput)+1);
               memset(pSym->V.cVal,0,strlen(pCurrCV->pLInput)+1);
               strcpy(pSym->V.cVal,pCurrCV->pLInput);
               MemFree(pCurrCV->pLInput);
               pCurrCV->pLInput = NULL;
            }
            _DPush(pSym);
            bSortie = mtTRUE;
         }
      }
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on met … jour les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrLT->pData    = pCurrCV->pData;
         pCurrLT->pOutBuff = pCurrCV->pOutBuff;
      }
   }
   if(! bSortie)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise INPUTVAR.
//
void _InitInputVar(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE pCh;
   static char cProcName[] = "INPUTVAR";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   CheckString(pSym,pCurrCV->pNProc);
   pCurrCV->pSInput = pSym;
   pCurrCV->pSILig  = pLi;
   pCurrCV->pSICol  = pCo;
   if(pSym->Type == mtTABCHAR)
      pCh = ArrayCVal(pSym,pLi,pCo,pCurrCV->pNProc);
   else
      pCh = pSym->V.cVal;
//   if(pCh)
//   {
      pCurrCV->pLInput = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
      strcpy(pCurrCV->pLInput,pCh);
//   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Saisie dans une variable.
// Syntaxe : NomVar INPUTVAR
//
// Touche gres en dition : ANNULATION, CORRECTION
//
void _MInputVar(void)
{
   mtBOOL bSortie = mtFALSE;
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE pCDst;
   int i;
   static char cProcName[] = "INPUTVAR";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->bPaqLu)
   {
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on prend les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrCV->pData    = pCurrLT->pData;
         pCurrCV->pOutBuff = pCurrLT->pOutBuff;
      }
      // On rcupre les donnes.
      if(pCurrCV->pData)
      {
         if(! pCurrCV->pLInput)
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemAlloc(strlen(pCurrCV->pData)+1);
            strcpy(pCurrCV->pLInput,pCurrCV->pData);
         }
         else
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemReAlloc(pCurrCV->pLInput,(strlen(pCurrCV->pLInput)+strlen(pCurrCV->pData))+1);
            strcat(pCurrCV->pLInput,pCurrCV->pData);
         }
      }
      switch(pCurrCV->TFonction)
      {
         case MCorrection:
         {
            if(pCurrCV->pLInput)
            {
               // On enlve le dernier caractre.
               *((mtPTRBYTE) pCurrCV->pLInput+(strlen(pCurrCV->pLInput)-1)) = '\0';
               // On envoi BACKSPACE+ESPACE+BACKSPACE.
               AppendCvOBuff(pCurrCV,"\x08 \x08",mtFALSE);
               if(!strlen(pCurrCV->pLInput))
               {
                  // Il n'y a plus rien
                  MemFree(pCurrCV->pLInput);
                  pCurrCV->pLInput = NULL;
               }
            }
            break;
         }
         case MAnnulation:
         {
            if(pCurrCV->pLInput)
            {
               // On recule de n caractres.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               // On envoi n espaces.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV," ",mtFALSE);
               // On recule de n caractres.
               for(i=0;i<strlen(pCurrCV->pLInput);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               MemFree(pCurrCV->pLInput);
               pCurrCV->pLInput = NULL;
            }
            break;
         }
         default:
         {
            // Toute autre touche provoque la fin de la saisie.
            pSym = pCurrCV->pSInput;
            pLi  = pCurrCV->pSILig;
            pCo  = pCurrCV->pSICol;
            if(pCurrCV->pLInput)
            {
               // WinInfo("INPUT=%s",pCurrCV->pLInput);
               if(pSym->Type == mtTABCHAR)
               {
                  pCDst = ArrayCVal(pSym,pLi,pCo,pCurrCV->pNProc);
                  pCDst = (mtPTRBYTE) MemReAlloc(pCDst,strlen(pCurrCV->pLInput)+1);
                  memset(pCDst,0,strlen(pCurrCV->pLInput)+1);
                  strcpy(pCDst,pCurrCV->pLInput);
                  ArrayCStore(pSym,pLi,pCo,pCDst);
               }
               else
               {
                  pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(pCurrCV->pLInput)+1);
                  memset(pSym->V.cVal,0,strlen(pCurrCV->pLInput)+1);
                  strcpy(pSym->V.cVal,pCurrCV->pLInput);
               }
               MemFree(pCurrCV->pLInput);
               pCurrCV->pLInput = NULL;
            }
            else
            {
               if(pSym->Type == mtTABCHAR)
               {
                  pCDst  = ArrayCVal(pSym,pLi,pCo,pCurrCV->pNProc);
                  pCDst  = (mtPTRBYTE) MemReAlloc(pCDst,1);
                  *pCDst = '\0';
                  ArrayCStore(pSym,pLi,pCo,pCDst);
               }
               else
               {
                  pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,1);
                  *((mtPTRBYTE) pSym->V.cVal) = '\0';
               }
            }
            bSortie = mtTRUE;
         }
      }
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on met … jour les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrLT->pData    = pCurrCV->pData;
         pCurrLT->pOutBuff = pCurrCV->pOutBuff;
      }
   }
   if(! bSortie)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise MLINPUT.
//
void _InitMLInput(void)
{
   mtPTRDICO pSym,pLi,pCo;
   float fnb;
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;
   static char cProcName[] = "MLINPUT";

   pCurrCV->pNProc = cProcName;
   fnb = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pCurrCV->MLILSize = fnb;
   fnb = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pCurrCV->MLIMaxLine = fnb;
   if(pCurrCV->MLILSize <= 0)
      pCurrCV->MLILSize = 40;
   if(pCurrCV->MLIMaxLine <= 0)
      pCurrCV->MLIMaxLine = 1;
   pCurrCV->MLILine = 1;
   // Allocation du tableau de lignes.
   pDst = (pPOINTEUR) MemAlloc(pCurrCV->MLIMaxLine*sizeof(mtPOINTEUR));
   for(i=0;i<pCurrCV->MLIMaxLine;i++)
   {
      pLig = (mtPTRBYTE) MemAlloc(pCurrCV->MLILSize+1);
      *(pDst+i) = (mtPOINTEUR) pLig;
   }
   pCurrCV->MLITabLine = pDst;
   pCurrCV->MLIRow     = pCurrCV->Row;
   pCurrCV->MLICol     = pCurrCV->Col;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Saisie multilignes.
// Syntaxe :  MaxLine Largeur MLINPUT
//
// Touche gres en dition : ANNULATION, CORRECTION, SUITE, RETOUR
//
void _MLInput(void)
{
   mtBOOL bSortie = mtFALSE;
   mtPTRDICO pSym;
   mtBYTE OBuff[4];
   int currlig,lenlig,nblig,i,iLen;
   mtPTRBYTE pLig,pLIn;
   pPOINTEUR pDst;
   static char cProcName[] = "MLINPUT";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->bPaqLu)
   {
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on prend les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrCV->pData    = pCurrLT->pData;
         pCurrCV->pOutBuff = pCurrLT->pOutBuff;
      }
      // On rcupre les donnes.
      if(pCurrCV->pData)
      {
         if(! pCurrCV->pLInput)
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemAlloc(strlen(pCurrCV->pData)+1);
            strcpy(pCurrCV->pLInput,pCurrCV->pData);
         }
         else
         {
            pCurrCV->pLInput = (mtPTRBYTE) MemReAlloc(pCurrCV->pLInput,(strlen(pCurrCV->pLInput)+strlen(pCurrCV->pData))+1);
            strcat(pCurrCV->pLInput,pCurrCV->pData);
         }
      }
      pDst    = pCurrCV->MLITabLine;
      currlig = pCurrCV->MLILine;
      // Stockage des info dans le tableau.
      if(pCurrCV->pLInput)
      {
         pLIn   = pCurrCV->pLInput;
         lenlig = strlen(pLIn);
         nblig  = (int) ((lenlig/pCurrCV->MLILSize)+1);
         while(lenlig > 0)
         {
            pLig = (mtPTRBYTE) *(pDst+(currlig-1));
            if(strlen(pLig))
            {
               // Ajout des infos.
               iLen = (pCurrCV->MLILSize-strlen(pLig));
               strncat(pLig,pLIn,iLen);
               lenlig -= iLen;
               pLIn   += iLen;
            }
            else
            {
               memset(pLig,0,pCurrCV->MLILSize);
               strncpy(pLig,pLIn,pCurrCV->MLILSize);
               lenlig -= pCurrCV->MLILSize;
               pLIn   += pCurrCV->MLILSize;
            }
            *(pDst+(currlig-1)) = (mtPOINTEUR) pLig;
            if(lenlig <= 0)
               break;
            currlig++;
            pCurrCV->MLIRow++;
            pCurrCV->MLICol = strlen(pLig)+1;
            if(currlig > pCurrCV->MLIMaxLine)
            {
               currlig--;
               pCurrCV->MLIRow--;
               break;
            }
         }
         if(lenlig > 0)
         {
            // Il y a plus de caractres que le nombre de lignes.
            // On positionne le curseur … la fin de la dernire ligne.
            sprintf(OBuff,"\x1F%c%c",0x40+pCurrCV->MLIRow,0x40+pCurrCV->MLICol);
            AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
         }
      }
      switch(pCurrCV->TFonction)
      {
         case MCorrection:
         {
            pLig = (mtPTRBYTE) *(pDst+(currlig-1));
            if(strlen(pLig))
            {
               // On enlve le dernier caractre.
               *(pLig+(strlen(pLig)-1)) = '\0';
               // On envoi BACKSPACE+ESPACE+BACKSPACE.
               AppendCvOBuff(pCurrCV,"\x08 \x08",mtFALSE);
               *(pDst+(currlig-1)) = (mtPOINTEUR) pLig;
            }
            else
            {
               // On remontre sur la ligne prcedente
               if(currlig > 1)
               {
                  currlig--;
                  pCurrCV->MLIRow--;
                  pLig            = (mtPTRBYTE) *(pDst+(currlig-1));
                  pCurrCV->MLICol = strlen(pLig)+1;
                  if(strlen(pLig))
                     // On enlve le dernier caractre.
                     *(pLig+(strlen(pLig)-1)) = '\0';
                  if(pCurrCV->MLICol <= 0)
                     pCurrCV->MLICol = 1;
                  // Pour le minitel 40 colonnes
                  if(pCurrCV->MLICol > 40)
                     pCurrCV->MLICol--;
                  // On positionne le curseur … la fin de la ligne.
                  sprintf(OBuff,"\x1F%c%c \x08",0x40+pCurrCV->MLIRow,0x40+pCurrCV->MLICol);
                  AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
               }
            }
            pCurrCV->MLITabLine = pDst;
            pCurrCV->MLILine    = currlig;
            break;
         }
         case MAnnulation:
         {
            pLig = (mtPTRBYTE) *(pDst+(currlig-1));
            if(strlen(pLig))
            {
               // On recule de n caractres.
               for(i=0;i<strlen(pLig);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               // On envoi n espaces.
               for(i=0;i<strlen(pLig);i++)
                  AppendCvOBuff(pCurrCV," ",mtFALSE);
               // On recule de n caractres.
               for(i=0;i<strlen(pLig);i++)
                  AppendCvOBuff(pCurrCV,"\x08",mtFALSE);
               memset(pLig,0,pCurrCV->MLILSize);
            }
            *(pDst+(currlig-1)) = (mtPOINTEUR) pLig;
            pCurrCV->MLITabLine = pDst;
            pCurrCV->MLILine    = currlig;
            break;
         }
         case MSuite:
         {
            // On avance d'une ligne.
            // if((! pCurrCV->pLInput) && (currlig < pCurrCV->MLIMaxLine))
            if(currlig < pCurrCV->MLIMaxLine)
            {
               currlig++;
               pCurrCV->MLIRow++;
            }
            pLig            = (mtPTRBYTE) *(pDst+(currlig-1));
            pCurrCV->MLICol = strlen(pLig)+1;
            if(pCurrCV->MLICol <= 0)
               pCurrCV->MLICol = 1;
            // Pour le minitel 40 colonnes
            if(pCurrCV->MLICol > 40)
               pCurrCV->MLICol--;
            // On positionne le curseur … la fin de la ligne.
            sprintf(OBuff,"\x1F%c%c",0x40+pCurrCV->MLIRow,0x40+pCurrCV->MLICol);
            AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
            pCurrCV->MLITabLine = pDst;
            pCurrCV->MLILine    = currlig;
            break;
         }
         case MRetour:
         {
            // On remonte d'une ligne.
            // if((! pCurrCV->pLInput) && (currlig > 1))
            if(currlig > 1)
            {
               currlig--;
               pCurrCV->MLIRow--;
            }
            pLig            = (mtPTRBYTE) *(pDst+(currlig-1));
            pCurrCV->MLICol = strlen(pLig)+1;
            if(pCurrCV->MLICol <= 0)
               pCurrCV->MLICol = 1;
            // Pour le minitel 40 colonnes
            if(pCurrCV->MLICol > 40)
               pCurrCV->MLICol--;
            // On positionne le curseur … la fin de la ligne.
            sprintf(OBuff,"\x1F%c%c",0x40+pCurrCV->MLIRow,0x40+pCurrCV->MLICol);
            AppendCvOBuff(pCurrCV,OBuff,mtFALSE);
            pCurrCV->MLITabLine = pDst;
            pCurrCV->MLILine    = currlig;
            break;
         }
         default:
         {
            // Toute autre touche provoque la fin de la saisie.
            // On empile les lignes saisies.
            for(i=0;i<pCurrCV->MLIMaxLine;i++)
            {
               pLig = (mtPTRBYTE) *(pDst+i);
               pSym = GenCvICHAR(strlen(pLig)+1);
               strcpy(pSym->V.cVal,pLig);
               _DPush(pSym);
               MemFree(pLig);
            }
            MemFree(pDst);
            pCurrCV->MLITabLine = NULL;
            bSortie             = mtTRUE;
         }
      }
      if(pCurrCV->pLInput)
      {
         MemFree(pCurrCV->pLInput);
         pCurrCV->pLInput = NULL;
      }
      if(pCurrCV->IsLTask)
      {
         // Tche locale, on met … jour les pointeurs du cv propritaire.
         // (les buffer d'entre/sortie sont communs)
         pCurrLT->pData    = pCurrCV->pData;
         pCurrLT->pOutBuff = pCurrCV->pOutBuff;
      }
   }
   if(! bSortie)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                             MANIPULATION DES PILES
//                             ออออออออออออออออออออออ
//
// Duplique le symbole situ au sommet de la pile.
// Syntaxe : symb DUP
//
void _PDup(void)
{
//   mtPTRDICO pDSym,pSDup;
   static char cProcName[] = "DUP";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->SP)
   {
//      pDSym = (mtPTRDICO) pCurrCV->PileD[pCurrCV->SP-1];
//      pSDup = DupliqueSym(pDSym);
//      _DPush(pSDup);
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileD[pCurrCV->SP-1]));
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Inverse les deux symboles situs au sommet de la pile.
// Syntaxe : symb1 symb2 SWAP
//
void _PSwap(void)
{
   mtPOINTEUR ptr1;
   static char cProcName[] = "SWAP";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->SP >= 2)
   {
      ptr1 = pCurrCV->PileD[pCurrCV->SP-2];
      pCurrCV->PileD[pCurrCV->SP-2] = pCurrCV->PileD[pCurrCV->SP-1];
      pCurrCV->PileD[pCurrCV->SP-1] = ptr1;
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution de OVER
//
void __Over(void)
{
//   mtPTRDICO pDSym1,pDSym2,pDSym3;

   if(pCurrCV->SP >= 2)
   {
//      pDSym2 = (mtPTRDICO) _DPop();
//      pDSym1 = (mtPTRDICO) _DPop();
//      pDSym3 = DupliqueSym(pDSym1);
//      _DPush(pDSym1);
//      _DPush(pDSym2);
//      _DPush(pDSym3);
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileD[pCurrCV->SP-2]));
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Duplique le symbole situ sous le premier.
// Syntaxe : symb1 symb2 OVER
//
void _POver(void)
{
   static char cProcName[] = "OVER";

   pCurrCV->pNProc = cProcName;
   __Over();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue une permutation circulaire sur les 3 symboles situe au sommet
// de la pile.
// Syntaxe : symb1 symb2 symb3 ROT
//
void _PRot(void)
{
//   mtPTRDICO pDSym1,pDSym2,pDSym3;
   mtPOINTEUR ptr1;
   static char cProcName[] = "ROT";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->SP >= 3)
   {
//      pDSym3 = (mtPTRDICO) _DPop();
//      pDSym2 = (mtPTRDICO) _DPop();
//      pDSym1 = (mtPTRDICO) _DPop();
//      _DPush(pDSym2);
//      _DPush(pDSym3);
//      _DPush(pDSym1);
      ptr1 = pCurrCV->PileD[pCurrCV->SP-3];
      pCurrCV->PileD[pCurrCV->SP-3] = pCurrCV->PileD[pCurrCV->SP-2];
      pCurrCV->PileD[pCurrCV->SP-2] = pCurrCV->PileD[pCurrCV->SP-1];
      pCurrCV->PileD[pCurrCV->SP-1] = ptr1;
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime le symbole situ sous le premier.
// Syntaxe : symb1 symb2 NIP
//
void _PNip(void)
{
   mtPTRDICO pDSym;
   static char cProcName[] = "NIP";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->SP >= 2)
   {
      pCurrCV->PileD[pCurrCV->SP-2] = pCurrCV->PileD[pCurrCV->SP-1];
      pDSym = (mtPTRDICO) _DPop();
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime le symbole situ au sommet de la pile.
// Syntaxe : symb DROP
//
void _PDrop(void)
{
   mtPTRDICO pDSym1;
   static char cProcName[] = "DROP";

   pCurrCV->pNProc = cProcName;
   pDSym1 = (mtPTRDICO) _DPop();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Duplique le nime symbole de la pile de donnes.
//
void _PPick(void)
{
   mtPTRDICO pS1,pLi,pCo;
   mtWORD w1;
   float d1;
   static char cProcName[] = "PICK";

   pCurrCV->pNProc = cProcName;
   d1 = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   w1 = d1;
   if(pCurrCV->PileD && (w1 <= pCurrCV->SP))
   {
//      pTmp = (mtPTRDICO) pCurrCV->PileD[pCurrCV->SP-w1];
//      pS2  = DupliqueSym(pTmp);
//      _DPush(pS2);
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileD[pCurrCV->SP-w1]));
   }
   else
      ExecError(pCurrCV->pNProc,ERRDPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'indice de la boucle la plus interne.
// Syntaxe : I
//
void _LoopI(void)
{
//   mtPTRDICO pSym;
   static char cProcName[] = "I";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->PileR)
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-1]));
   else
      ExecError(pCurrCV->pNProc,ERRRPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'indice de la boucle de second niveau.
// Syntaxe : J
//
void _LoopJ(void)
{
//   mtPTRDICO pSym;
   static char cProcName[] = "J";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->PileR && (pCurrCV->RP > 3))
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-3]));
   else
      ExecError(pCurrCV->pNProc,ERRRPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'indice de la boucle de niveau le plus externe.
// Syntaxe : K
//
void _LoopK(void)
{
//   mtPTRDICO pSym;
   static char cProcName[] = "K";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->PileR && (pCurrCV->RP > 5))
      _DPush(DupliqueSym((mtPTRDICO) pCurrCV->PileR[pCurrCV->RP-5]));
   else
      ExecError(pCurrCV->pNProc,ERRRPILEVIDE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//
//                            PRIMITIVES CONDITIONNELLES
//                            ออออออออออออออออออออออออออ
//
// Excution de TRUE.
//
void __TRUE(void)
{
   _DPush(pCONSTTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution de FALSE.
//
void __FALSE(void)
{
   _DPush(pCONSTFALSE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le boolen TRUE.
//
void _PushTRUE(void)
{
   static char cProcName[] = "TRUE";

   pCurrCV->pNProc = cProcName;
   __TRUE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le boolen FALSE.
//
void _PushFALSE(void)
{
   static char cProcName[] = "FALSE";

   pCurrCV->pNProc = cProcName;
   __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupre un symbole pour les comparaisons.
//
void RecupSymComp(pPDICO pSym,pPDICO pLig,pPDICO pCol,mtPTRBYTE NomProc,mtBOOL bLit,mtPTRBYTE pComp,pPOINTEUR pPtr,mtPTRFLOAT pFlt)
{
   mtPTRDICO pSRec,pSLi,pSCo;

   RecupSym(&pSRec,&pSLi,&pSCo,NomProc,bLit);
   *pSym  = pSRec;
   *pLig  = pSLi;
   *pCol  = pSCo;
   *pComp = 0;
   switch(pSRec->Type)
   {
      case mtTNUMBER:
      {
         *pComp = mtTNUMBER;
         *pFlt  = pSRec->V.fVal;
         break;
      }
      case mtTABNUMBER:
      {
         *pComp = mtTNUMBER;
         *pFlt  = ArrayNVal(pSRec,pSLi,pSCo,NomProc);
         break;
      }
      case mtTCHAR:
      {
         *pComp = mtTCHAR;
         *pPtr  = pSRec->V.cVal;
         break;
      }
      case mtTABCHAR:
      {
         *pComp = mtTCHAR;
         *pPtr  = ArrayCVal(pSRec,pSLi,pSCo,NomProc);
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du EGAL.
//
void __Egal(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;

   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) == 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 == flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si les 2 symboles situs au sommet de la pile sont gaux.
// Syntaxe : symb1 symb2 ==
//
void _SEgal(void)
{
   static char cProcName[] = "==";

   pCurrCV->pNProc = cProcName;
   __Egal();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si symb1 est infrieur … symb2.
// Syntaxe : symb1 symb2 <
//
void _SInf(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;
   static char cProcName[] = "<";

   pCurrCV->pNProc = cProcName;
   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) < 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 < flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si symb1 est suprieur … symb2.
// Syntaxe : symb1 symb2 >
//
void _SSup(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;
   static char cProcName[] = ">";

   pCurrCV->pNProc = cProcName;
   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) > 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 > flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si les 2 symboles situs au sommet de la pile sont diffrents.
// Syntaxe : symb1 symb2 !=
//
void _SDiff(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;
   static char cProcName[] = "!=";

   pCurrCV->pNProc = cProcName;
   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) != 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 != flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si symb1 est infrieur ou gal … symb2.
// Syntaxe : symb1 symb2 <=
//
void _SInfEgal(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;
   static char cProcName[] = "<=";

   pCurrCV->pNProc = cProcName;
   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) <= 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 <= flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si symb1 est suprieur ou gal … symb2.
// Syntaxe : symb1 symb2 >=
//
void _SSupEgal(void)
{
   mtPTRDICO pDSym1,pDSym2,pLi,pCo;
   mtPOINTEUR ptr1,ptr2;
   float flt1,flt2;
   mtBYTE Tcomp1,Tcomp2;
   static char cProcName[] = ">=";

   pCurrCV->pNProc = cProcName;
   RecupSymComp(&pDSym2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp2,&ptr2,&flt2);
   RecupSymComp(&pDSym1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE,&Tcomp1,&ptr1,&flt1);
   if(Tcomp1 != Tcomp2)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   if((Tcomp1 == 0) && (Tcomp2 == 0))
   {
      __FALSE();
   }
   else
   {
      if((Tcomp1 == mtTCHAR) && (Tcomp2 == mtTCHAR))
      {
         if(ptr1 && ptr2)
         {
            if(strcmp((mtPTRBYTE) ptr1,(mtPTRBYTE) ptr2) >= 0)
               __TRUE();
            else
               __FALSE();
         }
         else
            __FALSE();
      }
      if((Tcomp1 == mtTNUMBER) && (Tcomp2 == mtTNUMBER))
      {
         if(flt1 >= flt2)
            __TRUE();
         else
            __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//
//                               PRIMITIVES DIVERSES
//                               อออออออออออออออออออ
//
// Dconnecte le connect donn.
//
void _LibCnx(void)
{
   mtPTRDICO pSym,pLi,pCo;
   float pNCv;
   mtWORD NCv;
   static char cProcName[] = "LIBCV";

   pCurrCV->pNProc = cProcName;
   pNCv = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   NCv  = pNCv;
   if(NCv >= 0 && NCv < TOTALCV)
   {
      if(TabCv[NCv].Actif)
         tPorts[TabCv[NCv].NumPort].LibereCV(NCv);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la valeur du symbole donn.
//
void _VFetch(void)
{
   mtPTRDICO pDSym,pLi,pCo,pNew;
   mtPTRBYTE pcVal;
   float pnVal;
   static char cProcName[] = "@";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         pNew = GenCvICHAR(strlen(pDSym->V.cVal)+1);
         strcpy(pNew->V.cVal,pDSym->V.cVal);
         break;
      }
      case mtTABCHAR:
      {
         pcVal = ArrayCVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         pNew  = GenCvICHAR(strlen(pcVal)+1);
         strcpy(pNew->V.cVal,pcVal);
         break;
      }
      case mtTABNUMBER:
      {
         pNew         = GenCvINUMBER();
         pnVal        = ArrayNVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         pNew->V.fVal = pnVal;
         break;
      }
      case mtTNUMBER:
      {
         pNew         = GenCvINUMBER();
         pNew->V.fVal = pDSym->V.fVal;
         break;
      }
   }
   _DPush(pNew);
}
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile un espace.
//
void _Space(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SPACE";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(2);
   *(pSym->V.cVal) = ' ';
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile une chaine contenant n espaces.
//
void _SpaceS(void)
{
   mtPTRDICO pSym,pLi,pCo;
   float pNbSp;
   mtWORD Nbs;
   static char cProcName[] = "SPACES";

   pCurrCV->pNProc = cProcName;
   pNbSp = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   Nbs   = pNbSp;
   pSym  = GenCvICHAR(Nbs+1);
   memset(pSym->V.cVal,' ',Nbs);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile VRAI si le symbole donn est vide.
//
void _IsEmpty(void)
{
   mtPTRDICO pSym,pLi,pCo;
   static char cProcName[] = "EMPTY?";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   switch(pSym->Type)
   {
      case mtTCHAR:
      {
         if((pSym->V.cVal) && (strlen(pSym->V.cVal)))
         {
            if(strspn(pSym->V.cVal," ") == strlen(pSym->V.cVal))
               __TRUE();
            else
               __FALSE();
         }
         else
            __TRUE();
         break;
      }
      case mtTNUMBER:
      {
         if(pSym->V.fVal == (float) 0)
            __TRUE();
         else
            __FALSE();
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affiche sur laconsole du serveur le contenu du symbole donn.
//
void _DisplaySrv(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   mtBYTE OBuff[128];
   float pDTmp;
   mtPTRBYTE pCTmp;
   mtWORD Value;
   static char cProcName[] = "DISPLAY";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   switch(pDSym->Type)
   {
      case mtTCHAR:
      {
         // CheckNull(pDSym->V.cVal,"Display",pDSym);
         WinInfo("Voie=%d Display=%s",pCurrCV->NumCv,pDSym->V.cVal);
         break;
      }
      case mtTABCHAR:
      {
         pCTmp = ArrayCVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         // CheckNull(pCTmp,"Display",pDSym);
         WinInfo("Voie=%d Display=%s",pCurrCV->NumCv,pCTmp);
         break;
      }
      case mtTNUMBER:
      {
         if(pCurrCV->pUsing)
         {
            if(strpbrk(pCurrCV->pUsing,"eEfgG"))
               sprintf(OBuff,pCurrCV->pUsing,pDSym->V.fVal);
            else
            {
               Value = pDSym->V.fVal;
               sprintf(OBuff,pCurrCV->pUsing,Value);
            }
         }
         else
            sprintf(OBuff,"%f",pDSym->V.fVal);
         WinInfo("Voie=%d Display=%s",pCurrCV->NumCv,OBuff);
         break;
      }
      case mtTABNUMBER:
      {
         pDTmp = ArrayNVal(pDSym,pLi,pCo,pCurrCV->pNProc);
         if(pCurrCV->pUsing)
         {
            if(strpbrk(pCurrCV->pUsing,"eEfgG"))
               sprintf(OBuff,pCurrCV->pUsing,pDTmp);
            else
            {
               Value = pDTmp;
               sprintf(OBuff,pCurrCV->pUsing,Value);
            }
         }
         else
            sprintf(OBuff,"%f",pDTmp);
         WinInfo("Voie=%d Display=%s",pCurrCV->NumCv,OBuff);
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Appelle la procdure dont le nom est dans la chaine donne.
//
void _ExecuteProc(void)
{
   mtPTRBYTE NomLib,NomProc,pSep,pNProc;
   mtPTRDICO pDSym,pLi,pCo;
   mtPTRAPP pLib;
   mtPTRDICO pProc;
   mtWORD Len;
   mtUWORD wPos;
   mtPTRREFAPP NewR;
   static char cProcName[] = "EXECUTE";

   pCurrCV->pNProc = cProcName;
   pNProc = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pNProc,"Execute",pDSym);
   strupr(pNProc);
   pSep = strstr(pNProc,"::");
   if(pSep)
   {
      // Appel d'une procdure externe.
      // On rcupre le nom de la librairie.
      Len     = (pSep-pNProc);
      NomLib  = (mtPTRBYTE) MemAlloc(Len+1);
      strncpy(NomLib,pNProc,Len);
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
            //
            // On ajoute la rfrence … l'appli courante dans la liste
            // des rf. croises de pLib, si ce n'est pas dj… fait.
            //
            if(! SearchRefApp(pLib->pLRefApp,pCurrCV->pApp))
            {
               NewR = NewRefApp(pLib->pLRefApp,pCurrCV->pApp);
               if(! pLib->pLRefApp)
                  // 1re entre.
                  pLib->pLRefApp = NewR;
            }
            MemFree(NomProc);
            MemFree(NomLib);
            // Prparation de l'appel.
            // On recopie le dictionnaire local de l'appli.
            pCurrCV->pCApp = pLib;
            _CopyLDic();
            // Sauvegarde du point de retour.
            _RPush((mtPOINTEUR) pCurrCV->IP);
            _RPush((mtPOINTEUR) pCurrCV->pHere);
            pCurrCV->pHere = (ppFUNC) pProc->V.pVal;
            pCurrCV->IP    = (mtUWORD) pProc->XVal;
         }
         else
         {
            MemFree(NomProc);
            MemFree(NomLib);
            ExecError(pCurrCV->pNProc,ERRPROCNOTFOUND,pDSym);
         }
      }
      else
      {
         MemFree(NomProc);
         MemFree(NomLib);
         ExecError(pCurrCV->pNProc,ERRAPPNOTFOUND,pDSym);
      }
   }
   else
   {
      // Appel d'une procdure interne … l'appli.
      pProc = SearchDico(pCurrCV->pApp->pPDico,pCurrCV->pApp->uPDico,pNProc,&wPos);
      if(pProc && pProc->Type == mt2POINTS)
      {
         // Prparation de l'appel.
         // Sauvegarde du point de retour.
         _RPush((mtPOINTEUR) pCurrCV->IP);
         _RPush((mtPOINTEUR) pCurrCV->pHere);
         pCurrCV->pHere = (ppFUNC) pProc->V.pVal;
         pCurrCV->IP    = (mtUWORD) pProc->XVal;
      }
      else
         ExecError(pCurrCV->pNProc,ERRPROCNOTFOUND,pDSym);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Appelle l'application dont le nom est dans la chaine donne.
//
void _ExecuteApp(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   mtPTRBYTE NomApp;
   mtPTRAPP pApp;
   mtUWORD NumCv;
   mtBYTE NumPort,Dept,Palier,TypeMinitel;
   char TrameCNX[31],Pavi[7];
   struct dos_time_t HPaq;
   static char cProcName[] = "EXECUTEAPP";

   pCurrCV->pNProc = cProcName;
   NomApp = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(! pCurrCV->IsLTask)
   {
      // CheckNull(NomApp,"ExecuteApp",pDSym);
      strupr(NomApp);
      pApp = SearchApp(NomApp,0);
      if(pApp)
      {
         if(pApp->Active)
         {
            // On sauvegarde quelques infos.
            NumCv       = pCurrCV->NumCv;
            NumPort     = pCurrCV->NumPort;
            Dept        = pCurrCV->Dept;
            Palier      = pCurrCV->Palier;
            TypeMinitel = pCurrCV->TypeMinitel;
            strcpy(TrameCNX,pCurrCV->TrameCNX);
            strcpy(Pavi,pCurrCV->Pavi);
            // On simule la dconnexion de l'appli courante.
            DeconnecteCv(pCurrCV);
            // On simule la connexion … l'appli donne.
            pCurrCV->pApp        = pApp;
            pCurrCV->Actif       = mtTRUE;
            pCurrCV->NumCv       = NumCv;
            pCurrCV->NumPort     = NumPort;
            pCurrCV->Dept        = Dept;
            pCurrCV->Palier      = Palier;
            pCurrCV->TypeMinitel = TypeMinitel;
            strcpy(pCurrCV->TrameCNX,TrameCNX);
            strcpy(pCurrCV->Pavi,Pavi);
            strcpy(pCurrCV->Service,pApp->Nom);
            pCurrCV->NumComp = pApp->NumComp;
            _dos_getdate(&pCurrCV->DateCnx);
            _dos_gettime(&pCurrCV->HeureCnx);
            if(! NbAppTot)
               _dos_gettime(&HPreCnx);
            _dos_gettime(&HDerCnx);
            InitCnxCv(pCurrCV);
            _dos_gettime(&HPaq);
            HPaq.hsecond   = 0;
            pCurrCV->HRPaq = TimeToCent(&HPaq)/100;
            NbAppTot++;
         }
         else
            ExecError(pCurrCV->pNProc,ERRAPPNOTMAIN,pDSym);
      }
      else
         ExecError(pCurrCV->pNProc,ERRAPPNOTFOUND,pDSym);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile TRUE si l'application donne existe et est valide.
//
void _ChercheApp(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   mtPTRBYTE NomApp;
   mtPTRAPP pApp;
   static char cProcName[] = "SEARCHAPP";

   pCurrCV->pNProc = cProcName;
   NomApp = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(NomApp)
   {
      strupr(NomApp);
      pApp = SearchApp(NomApp,0);
      if(pApp)
      {
         if(pApp->Active)
            __TRUE();
         else
            __FALSE();
      }
      else
         __FALSE();
   }
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Execute la commande clavier donne.
//
void _ExecKbdCmd(void)
{
   mtPTRBYTE pCmd;
   mtPTRDICO pDSym,pRet,pLi,pCo;
   static char cProcName[] = "EXECKBDCMD";

   pCurrCV->pNProc = cProcName;
   pCmd = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   strupr(pCmd);
   pRet = GenCvINUMBER();
   ProcessKbdCmd(pCmd);
   pRet->V.fVal = (float) nErrCmd;
   _DPush(pRet);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Attend une action (rception d'un paquet) de l'utilisateur courant
// avant de continuer l'excution du code.
//
void _Wait(void)
{
   static char cProcName[] = "WAIT";

   pCurrCV->pNProc = cProcName;
   if(! pCurrCV->bPaqLu)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Attend que le buffer de sortie soit vide.
//
void _WaitOutBuffer(void)
{
   static char cProcName[] = "WAITEMPTYBUFFER";

   pCurrCV->pNProc = cProcName;
   if(! pCurrCV->pOutBuff)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la fonction WAITCHAR, en rcuprant le caractre … attendre.
//
void _InitWaitChar(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "WAITCHAR";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pCurrCV->WaitChar  = *pc1;
   pCurrCV->bWaitChar = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Attend un caractre sur le flux.
//
void _WaitChar(void)
{
   mtBOOL bSortie = mtFALSE;
   static char cProcName[] = "WAITCHAR";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->bPaqLu)
   {
      if(pCurrCV->pData)
      {
         if(strchr(pCurrCV->pData,pCurrCV->WaitChar))
         {
            pCurrCV->bWaitChar = mtFALSE;
            bSortie = mtTRUE;
         }
      }
   }
   if(! bSortie)
   {
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la fonction DELAY, en rcuprant l'heure courante,
// ainsi que le nombre de millisecondes donn.
//
void _InitDelay(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   float pDelai;
   struct dos_time_t HDeb;
   static char cProcName[] = "DELAY";

   pCurrCV->pNProc = cProcName;
   pDelai = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   _dos_gettime(&HDeb);
   pCurrCV->HDelay = TimeToCent(&HDeb)*10;
   pCurrCV->dDelay = pDelai;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Attend pendant n millisecondes
// Syntaxe : n DELAY
//
void _Delay(void)
{
   struct dos_time_t HFin;
   float TFin;
   static char cProcName[] = "DELAY";

   pCurrCV->pNProc = cProcName;
   _dos_gettime(&HFin);
   TFin = TimeToCent(&HFin)*10;
   if(TFin<pCurrCV->HDelay)
      // A cheval sur 2 jours.
      TFin += (float) ((float) 86400 * (float) 1000);
   if(((float) (TFin - pCurrCV->HDelay)) < pCurrCV->dDelay)
   {
      // Le dlai n'est pas atteint.
      pCurrCV->IP--;
      // On provoque la sortie de la boucle d'excution.
      pCurrCV->TimeExec = 0;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation d'une variable.
// Syntaxe : src dst :=
//
void _VarStore(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pDSrc,pDDst;
   mtPTRBYTE pCSrc,pCDst;
   static char cProcName[] = ":=";

   pCurrCV->pNProc = cProcName;
   RecupSym(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   switch(pDst->Type)
   {
      case mtTCHAR:
      {
         pCSrc = RecupString(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
         // CheckNull(pCSrc,":=",pSrc);
         //if(! pDst->V.cVal)
            // Premire initialisation.
         //   pDst->V.cVal = (mtPTRBYTE) MemAlloc(strlen(pCSrc)+1);
         //else
         //{
            pDst->V.cVal = (mtPTRBYTE) MemReAlloc(pDst->V.cVal,strlen(pCSrc)+1);
            memset(pDst->V.cVal,0,strlen(pCSrc)+1);
         //}
         strcpy(pDst->V.cVal,pCSrc);
         break;
      }
      case mtTABCHAR:
      {
         pCSrc = RecupString(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
         // CheckNull(pCSrc,":=",pSrc);
         pCDst = ArrayCVal(pDst,pLiDst,pCoDst,pCurrCV->pNProc);
         //if(! pCDst)
            // Premire initialisation.
         //   pCDst = (mtPTRBYTE) MemAlloc(strlen(pCSrc)+1);
         //else
         //{
            pCDst = (mtPTRBYTE) MemReAlloc(pCDst,strlen(pCSrc)+1);
            memset(pCDst,0,strlen(pCSrc)+1);
         //}
         strcpy(pCDst,pCSrc);
         ArrayCStore(pDst,pLiDst,pCoDst,pCDst);
         break;
      }
      case mtTNUMBER:
      {
         pDSrc = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
         pDst->V.fVal = pDSrc;
         break;
      }
      case mtTABNUMBER:
      {
         pDSrc = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
         ArrayNStore(pDst,pLiDst,pCoDst,pDSrc);
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'une touche de fonction dans la table globale.
//
void _SetGFKey(void)
{
   mtPTRDICO pSym,pProc;
   mtPTRBYTE pc1;
   mtBYTE bTch;
   float pd1;
   mtBOOL bEt=mtFALSE,bDi=mtFALSE,bRe=mtFALSE;
   mtPTRTFKEY NewK;
   static char cProcName[] = "SETGLOBALFKEY";

   pCurrCV->pNProc = cProcName;
   pProc = (mtPTRDICO) _DPop();
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bRe = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bEt = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bDi = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   bTch  = *pc1;
   NewK  = SearchTFKEY(pCurrCV->pApp->pGTabFKey,bTch,bDi,bEt);
   if(NewK)
   {
      // Remplacement d'une touche existante.
      NewK->bDiese    = bDi;
      NewK->bEtoile   = bEt;
      NewK->TFonction = bTch;
      NewK->HProc     = (ppFUNC) pProc->V.pVal;
      NewK->IPProc    = (mtUWORD) pProc->XVal;
   }
   else
   {
      NewK = NewFKey(pCurrCV->pApp->pGTabFKey,bDi,bEt,bTch,pProc);
      if(! pCurrCV->pApp->pGTabFKey)
         // 1re entre.
         pCurrCV->pApp->pGTabFKey = NewK;
   }
   NewK->bReturn = bRe;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'un mot-clef … la table globale.
//
void _SetGKeyW(void)
{
   mtPTRBYTE pc1;
   mtPTRDICO pSym,pProc;
   mtPTRTKEYW NewK;
   float pd1;
   mtBOOL bRe=mtFALSE;
   static char cProcName[] = "SETGLOBALKEYWORD";

   pCurrCV->pNProc = cProcName;
   pProc = (mtPTRDICO) _DPop();
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bRe = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   NewK  = SearchTKEYW(pCurrCV->pApp->pGTabKeyW,pc1);
   if(NewK)
   {
      // Remplacement d'une touche existante.
      strcpy(NewK->KeyWord,pc1);
      NewK->HProc  = (ppFUNC) pProc->V.pVal;
      NewK->IPProc = (mtUWORD) pProc->XVal;
   }
   else
   {
      NewK = NewKeyW(pCurrCV->pApp->pGTabKeyW,pc1,pProc);
      if(! pCurrCV->pApp->pGTabKeyW)
         // 1re entre.
         pCurrCV->pApp->pGTabKeyW = NewK;
   }
   NewK->bReturn = bRe;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'une touche de fonction dans la table locale du Cv.
//
void _SetLFKey(void)
{
   mtPTRDICO pSym,pProc;
   mtPTRBYTE pc1;
   mtBYTE bTch;
   float pd1;
   mtBOOL bEt=mtFALSE,bDi=mtFALSE,bRe=mtFALSE;
   mtPTRTFKEY NewK;
   mtPTRCV OldCv;
   static char cProcName[] = "SETLOCALFKEY";

   pCurrCV->pNProc = cProcName;
   pProc = (mtPTRDICO) _DPop();
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bRe = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bEt = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bDi = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   bTch  = *pc1;
   OldCv = pCurrCV;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le cv propritaire de celle-ci.
      // (Les tables de touches/mot-clef locales sont communes)
      pCurrCV = pCurrLT;
   NewK  = SearchTFKEY(pCurrCV->pLTabFKey,bTch,bDi,bEt);
   if(NewK)
   {
      // Remplacement d'une touche existante.
      NewK->bDiese    = bDi;
      NewK->bEtoile   = bEt;
      NewK->TFonction = bTch;
      NewK->HProc     = (ppFUNC) pProc->V.pVal;
      NewK->IPProc    = (mtUWORD) pProc->XVal;
   }
   else
   {
      NewK = NewFKey(pCurrCV->pLTabFKey,bDi,bEt,bTch,pProc);
      if(! pCurrCV->pLTabFKey)
         // 1re entre.
         pCurrCV->pLTabFKey = NewK;
   }
   pCurrCV = OldCv;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur conern.
      pCurrCV->pLTabFKey = pCurrLT->pLTabFKey;
   NewK->bReturn   = bRe;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Assignation d'un mot-clef … la table locale du Cv.
//
void _SetLKeyW(void)
{
   mtPTRBYTE pc1;
   mtPTRDICO pSym,pProc;
   mtPTRTKEYW NewK;
   mtPTRCV OldCv;
   float pd1;
   mtBOOL bRe=mtFALSE;
   static char cProcName[] = "SETLOCALKEYWORD";

   pCurrCV->pNProc = cProcName;
   pProc = (mtPTRDICO) _DPop();
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bRe = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   OldCv = pCurrCV;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le cv propritaire de celle-ci.
      // (Les tables de touches/mot-clef locales sont communes)
      pCurrCV = pCurrLT;
   NewK  = SearchTKEYW(pCurrCV->pLTabKeyW,pc1);
   if(NewK)
   {
      // Remplacement d'une touche existante.
      strcpy(NewK->KeyWord,pc1);
      NewK->HProc  = (ppFUNC) pProc->V.pVal;
      NewK->IPProc = (mtUWORD) pProc->XVal;
   }
   else
   {
      NewK = NewKeyW(pCurrCV->pLTabKeyW,pc1,pProc);
      if(! pCurrCV->pLTabKeyW)
         // 1re entre.
         pCurrCV->pLTabKeyW = NewK;
   }
   pCurrCV = OldCv;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur conern.
      pCurrCV->pLTabKeyW = pCurrLT->pLTabKeyW;
   NewK->bReturn = bRe;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une table de touche locale.
//
void _DelLFKey(void)
{
   static char cProcName[] = "EMPTYLOCALFKEYTABLE";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on dtruit la table du cv propritaire.
      DelTFKEY(pCurrLT->pLTabFKey);
   else
      DelTFKEY(pCurrCV->pLTabFKey);
   pCurrCV->pLTabFKey = NULL;
   pCurrLT->pLTabFKey = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une table de touche globale.
//
void _DelGFKey(void)
{
   static char cProcName[] = "EMPTYGLOBALFKEYTABLE";

   pCurrCV->pNProc = cProcName;
   DelTFKEY(pCurrCV->pApp->pGTabFKey);
   pCurrCV->pApp->pGTabFKey = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une table de mots-clefs locale.
//
void _DelLKeyW(void)
{
   static char cProcName[] = "EMPTYLOCALKEYWORDTABLE";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->IsLTask)
      // Tche locale, on dtruit la table du cv propritaire.
      DelTKEYW(pCurrLT->pLTabKeyW);
   else
      DelTKEYW(pCurrCV->pLTabKeyW);
   pCurrCV->pLTabKeyW = NULL;
   pCurrLT->pLTabKeyW = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une table de mots-clefs globale.
//
void _DelGKeyW(void)
{
   static char cProcName[] = "EMPTYGLOBALKEYWORDTABLE";

   pCurrCV->pNProc = cProcName;
   DelTKEYW(pCurrCV->pApp->pGTabKeyW);
   pCurrCV->pApp->pGTabKeyW = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation globale de touche.
//
void _DelUneGFKey(void)
{
   mtPTRDICO pSym;
   mtPTRBYTE pc1;
   mtBYTE bTch;
   float pd1;
   mtBOOL bEt=mtFALSE,bDi=mtFALSE;
   mtPTRTFKEY k,kprev;
   static char cProcName[] = "DELETEGLOBALFKEY";

   pCurrCV->pNProc = cProcName;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bEt = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bDi = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   bTch  = *pc1;
   k     = pCurrCV->pApp->pGTabFKey;
   kprev = pCurrCV->pApp->pGTabFKey;
   while(k)
   {
      if((k->TFonction == bTch) && (k->bDiese == bDi) && (k->bEtoile == bEt))
      {
         // La touche existe.
         if(kprev == k)
            // c'est la 1re touche assigne.
            pCurrCV->pApp->pGTabFKey = k->pNextTFKEY;
         else
            kprev->pNextTFKEY = k->pNextTFKEY;
         MemFree(k);
         break;
      }
      kprev = k;
      k     = k->pNextTFKEY;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation locale de touche.
//
void _DelUneLFKey(void)
{
   mtPTRDICO pSym;
   mtPTRBYTE pc1;
   mtBYTE bTch;
   float pd1;
   mtBOOL bEt=mtFALSE,bDi=mtFALSE;
   mtPTRTFKEY k,kprev;
   mtPTRCV OldCv;
   static char cProcName[] = "DELETELOCALFKEY";

   pCurrCV->pNProc = cProcName;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bEt = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pd1   = pSym->V.fVal;
   if(pd1 == (float) 1)
      bDi = mtTRUE;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   bTch  = *pc1;
   OldCv = pCurrCV;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le cv propritaire de celle-ci.
      // (Les tables de touches/mot-clef locales sont communes)
      pCurrCV = pCurrLT;
   k     = pCurrCV->pLTabFKey;
   kprev = pCurrCV->pLTabFKey;
   while(k)
   {
      if((k->TFonction == bTch) && (k->bDiese == bDi) && (k->bEtoile == bEt))
      {
         // La touche existe.
         if(kprev == k)
            // c'est la 1re touche assigne.
            pCurrCV->pLTabFKey = k->pNextTFKEY;
         else
            kprev->pNextTFKEY = k->pNextTFKEY;
         MemFree(k);
         break;
      }
      kprev = k;
      k     = k->pNextTFKEY;
   }
   pCurrCV = OldCv;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur conern.
      pCurrCV->pLTabFKey = pCurrLT->pLTabFKey;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation globale de mot-clef.
//
void _DelUnGKeyW(void)
{
   mtPTRBYTE pc1;
   mtPTRDICO pSym;
   mtPTRTKEYW k,kprev;
   static char cProcName[] = "DELETEGLOBALKEYWORD";

   pCurrCV->pNProc = cProcName;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   k     = pCurrCV->pApp->pGTabKeyW;
   kprev = pCurrCV->pApp->pGTabKeyW;
   while(k)
   {
      if(strcmp(k->KeyWord,pc1) == 0)
      {
         // Le mot-clef existe.
         if(kprev == k)
            // c'est la 1re assignation.
            pCurrCV->pApp->pGTabKeyW = k->pNextTKEYW;
         else
            kprev->pNextTKEYW = k->pNextTKEYW;
         MemFree(k->KeyWord);
         MemFree(k);
         break;
      }
      kprev = k;
      k     = k->pNextTKEYW;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une assignation locale de mot-clef.
//
void _DelUnLKeyW(void)
{
   mtPTRBYTE pc1;
   mtPTRDICO pSym;
   mtPTRTKEYW k,kprev;
   mtPTRCV OldCv;
   static char cProcName[] = "DELETELOCALKEYWORD";

   pCurrCV->pNProc = cProcName;
   pSym  = (mtPTRDICO) _DPop();
   pc1   = pSym->V.cVal;
   OldCv = pCurrCV;
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le cv propritaire de celle-ci.
      // (Les tables de touches/mot-clef locales sont communes)
      pCurrCV = pCurrLT;
   k     = pCurrCV->pLTabKeyW;
   kprev = pCurrCV->pLTabKeyW;
   while(k)
   {
      if(strcmp(k->KeyWord,pc1) == 0)
      {
         // Le mot-clef existe.
         if(kprev == k)
            // c'est la 1re assignation.
            pCurrCV->pLTabKeyW = k->pNextTKEYW;
         else
            kprev->pNextTKEYW = k->pNextTKEYW;
         MemFree(k->KeyWord);
         MemFree(k);
         break;
      }
      kprev = k;
      k     = k->pNextTKEYW;
   }
   pCurrCV = OldCv;
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur conern.
      pCurrCV->pLTabKeyW = pCurrLT->pLTabKeyW;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation de la gestion des touches de fonction locales.
//
void _DisLFKey(void)
{
   static char cProcName[] = "DISABLELOCALFKEY";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bLFKAct = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation de la gestion des touches de fonction globales.
//
void _DisGFKey(void)
{
   static char cProcName[] = "DISABLEGLOBALFKEY";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bGFKAct = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation de la gestion des mots-clefs locaux.
//
void _DisLKeyW(void)
{
   static char cProcName[] = "DISABLELOCALKEYWORD";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bLKWAct = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation de la gestion des mots-clefs globaux.
//
void _DisGKeyW(void)
{
   static char cProcName[] = "DISABLEGLOBALKEYWORD";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bGKWAct = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation de la gestion des touches de fonction locales.
//
void _EnaLFKey(void)
{
   static char cProcName[] = "ENABLELOCALFKEY";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bLFKAct = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation de la gestion des touches de fonction globales.
//
void _EnaGFKey(void)
{
   static char cProcName[] = "ENABLEGLOBALFKEY";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bGFKAct = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation de la gestion des mots-clefs locaux.
//
void _EnaLKeyW(void)
{
   static char cProcName[] = "ENABLELOCALKEYWORD";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bLKWAct = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation de la gestion des mots-clefs globaux.
//
void _EnaGKeyW(void)
{
   static char cProcName[] = "ENABLEGLOBALKEYWORD";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->bGKWAct = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation de la translation des accents du cv courant.
//
void _DisTrAcc(void)
{
   static char cProcName[] = "TRANSLATEOFF";

   pCurrCV->pNProc   = cProcName;
   pCurrCV->bNoTrAcc = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation de la translation des accents du cv courant.
//
void _EnaTrAcc(void)
{
   static char cProcName[] = "TRANSLATEON";

   pCurrCV->pNProc   = cProcName;
   pCurrCV->bNoTrAcc = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactivation du filtrage des squences escape du cv courant.
//
void _DisSeqEsc(void)
{
   static char cProcName[] = "FILTERINGOFF";

   pCurrCV->pNProc    = cProcName;
   pCurrCV->bNoSeqEsc = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation du filtrage des squences escape du cv courant.
//
void _EnaSeqEsc(void)
{
   static char cProcName[] = "FILTERINGON";

   pCurrCV->pNProc    = cProcName;
   pCurrCV->bNoSeqEsc = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation du mode binaire.
//
void _EnaBinMode(void)
{
   static char cProcName[] = "SETBINARYMODE";

   pCurrCV->pNProc   = cProcName;
   pCurrCV->bBinMode = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Activation du mode normal.
//
void _DisBinMode(void)
{
   static char cProcName[] = "SETNORMALMODE";

   pCurrCV->pNProc   = cProcName;
   pCurrCV->bBinMode = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//           
//                            LES OPERATEURS
//                            ออออออออออออออ
//
// Effectue un ET binaire entre les deux symboles.
// Syntaxe : symb1 symb2 &
//
void _BAnd(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "&";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi1 & wi2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un OU binaire entre les deux symboles.
// Syntaxe : symb1 symb2 |
//
void _BOr(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "|";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi1 | wi2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un OU EXCLUSIF binaire entre les deux symboles.
// Syntaxe : symb1 symb2 ^
//
void _BXor(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "^";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi1 ^ wi2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un ET logique entre les deux symboles.
// Syntaxe : symb1 symb2 &&
//
void _LAnd(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "&&";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi1 && wi2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un OU logique entre les deux symboles.
// Syntaxe : symb1 symb2 ||
//
void _LOr(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "||";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi1 || wi2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue une ngation logique du symbole au sommet de la pile.
// Syntaxe : symb !
//
void _LNot(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   mtWORD wi1;
   static char cProcName[] = "!";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   wi1 = d1;
   pS2->V.fVal = (float) (! wi1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue le complment … 1 du symbole au sommet de la pile.
// Syntaxe : symb ~
//
void _BComp1(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   mtWORD wi1;
   static char cProcName[] = "~";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   wi1 = d1;
   pS2->V.fVal = (float) (~ wi1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Incrmentation du symbole au sommet de la pile.
//
void _Inc1(void)
{
   mtPTRDICO pS1,pLi,pCo;
   float pd1;
   static char cProcName[] = "++";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtTRUE);
   pS1->V.fVal += (float) 1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcrmentation du symbole au sommet de la pile.
//
void _Dec1(void)
{
   mtPTRDICO pS1,pLi,pCo;
   float pd1;
   static char cProcName[] = "--";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtTRUE);
   pS1->V.fVal -= (float) 1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute le premier symbole … la valeur du second.
// Syntaxe : src dst +=
//
void _PlusEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   static char cProcName[] = "+=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   pd1 += pd2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,pd1);
   else
      pDst->V.fVal += pSrc->V.fVal;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Soustrait le premier symbole … la valeur du second.
// Syntaxe : src dst -=
//
void _MoinsEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   static char cProcName[] = "-=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   pd1 -= pd2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,pd1);
   else
      pDst->V.fVal -= pSrc->V.fVal;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Multiplie le premier symbole … la valeur du second.
// Syntaxe : src dst *=
//
void _MultEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   static char cProcName[] = "*=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   pd1 *= pd2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,pd1);
   else
      pDst->V.fVal *= pSrc->V.fVal;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Divise le premier symbole … la valeur du second.
// Syntaxe : src dst /=
//
void _DivEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   static char cProcName[] = "/=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   CheckZeroDiv(pd2,pCurrCV->pNProc,pSrc);
   pd1 /= pd2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,pd1);
   else
      pDst->V.fVal /= pSrc->V.fVal;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Calcule le modulo de src et dst et affecte dst avec le rsultat.
// Syntaxe : src dst %=
//
void _ModEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   static char cProcName[] = "%=";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2 = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   CheckZeroDiv(pd2,pCurrCV->pNProc,pSrc);
   pd1 = fmodf(pd1,pd2);
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,pd1);
   else
      pDst->V.fVal = fmodf(pDst->V.fVal,pSrc->V.fVal);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un ET binaire entre le premier et la valeur du second,
// et affecte le second avec le rsultat .
// Syntaxe : symb1 symb2 &=
//
void _BAndEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   mtWORD wi1,wi2;
   static char cProcName[] = "&=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   wi1  = pd1;
   wi2  = pd2;
   wi1 &= wi2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,wi1);
   else
      pDst->V.fVal = (float) wi1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un OU binaire entre le premier et la valeur du second,
// et affecte le second avec le rsultat .
// Syntaxe : symb1 symb2 |=
//
void _BOrEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   mtWORD wi1,wi2;
   static char cProcName[] = "|=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   wi1  = pd1;
   wi2  = pd2;
   wi1 |= wi2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,wi1);
   else
      pDst->V.fVal = (float) wi1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un OU exclusif binaire entre le premier et la valeur du second,
// et affecte le second avec le rsultat .
// Syntaxe : symb1 symb2 ^=
//
void _BXorEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   mtWORD wi1,wi2;
   static char cProcName[] = "^=";

   pCurrCV->pNProc = cProcName;
   pd1  = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2  = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   wi1  = pd1;
   wi2  = pd2;
   wi1 ^= wi2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,wi1);
   else
      pDst->V.fVal = (float) wi1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcale Symb2 de Symb1 bits vers la gauche et empile le rusltat.
// Syntaxe : symb1 symb2 <<
//
void _LeftShift(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = "<<";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi2 << wi1);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcale Symb1 de Symb2 bits vers la droite et empile le rsultat.
// Syntaxe : symb1 symb2 >>
//
void _RightShift(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   mtWORD wi1,wi2;
   static char cProcName[] = ">>";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   wi1 = d1;
   wi2 = d2;
   pS3->V.fVal = (float) (wi2 >> wi1);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcale dst de src bits vers la gauche, et affecte le rsultat …
// dst.
// Syntaxe : src dst <<=
//
void _LShiftEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   mtWORD wi1,wi2;
   static char cProcName[] = "<<=";

   pCurrCV->pNProc = cProcName;
   pd1   = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2   = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   wi1   = pd1;
   wi2   = pd2;
   wi1 <<= wi2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,wi1);
   else
      pDst->V.fVal = (float) wi1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcale Symb2 de Symb1 bits vers la droite, et affecte le rsultat …
// Symb2.
// Syntaxe : symb1 symb2 >>=
//
void _RShiftEgal(void)
{
   mtPTRDICO pSrc,pDst,pLiSrc,pCoSrc,pLiDst,pCoDst;
   float pd1,pd2;
   mtWORD wi1,wi2;
   static char cProcName[] = ">>=";

   pCurrCV->pNProc = cProcName;
   pd1   = RecupNumber(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pd2   = RecupNumber(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
   wi1   = pd1;
   wi2   = pd2;
   wi1 >>= wi2;
   if(pDst->Type == mtTABNUMBER)
      ArrayNStore(pDst,pLiDst,pCoDst,wi1);
   else
      pDst->V.fVal = (float) wi1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                               LES MATHEMATIQUES
//                               อออออออออออออออออ
//
// Additionne les deux symboles et empile le rsultat.
// Syntaxe : Symb1 Symb2 +
//
void _Plus(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "+";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = d1 + d2;
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Soustrait les deux symboles et empile le rsultat.
// Syntaxe : Symb1 Symb2 -
//
void _Moins(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "-";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = d1 - d2;
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Multiplie les deux symboles et empile le rsultat.
// Syntaxe : Symb1 Symb2 *
//
void _Mult(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "*";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = d1 * d2;
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Divise les deux symboles et empile le rsultat.
// Syntaxe : Symb1 Symb2 /
//
void _Div(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "/";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   CheckZeroDiv(d2,pCurrCV->pNProc,pS2);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = d1 / d2;
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Calcule le reste de la division (modulo) des deux symboles
// et empile le rsultat.
// Syntaxe : Symb1 Symb2 %
//
void _Mod(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "%";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   CheckZeroDiv(d2,pCurrCV->pNProc,pS2);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = fmodf(d1,d2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'arc cosinus du symbole donn.
//
void _ACos(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "ACOS";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = acosf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'arc sinus du symbole donn.
//
void _ASin(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "ASIN";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = asinf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'arc tangante du symbole donn.
//
void _ATan(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "ATAN";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = atanf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'arc tangante de symb1/symb2.
//
void _ATan2(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "ATAN2";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   CheckZeroDiv(d2,pCurrCV->pNProc,pS2);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = atan2f(d1,d2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Arrondi le symbole donn au chiffre immdiatement suprieur.
//
void _Ceil(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "CEIL";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = ceilf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le cosinus du symbole donn.
//
void _Cos(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "COS";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = cosf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le cosinus hyperbolique du symbole donn.
//
void _Cosh(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "COSH";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = coshf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile e puissance Symb.
//
void _Exp(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "EXP";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = expf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile e puissance Symb-1.
//
void _Expm1(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "EXP-1";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = expm1f(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la valeur absolue du symbole donn.
//
void _FAbs(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "ABS";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = fabsf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rend le nombre donn ngatif.
//
void _Negate(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "NEGATE";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = (float) (d1 * (float) -1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Arrondi le symbole donn au chiffre immdiatement infrieur.
//
void _Floor(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "FLOOR";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = floorf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Calcule l'hypothnuse du triangle dont l'angle droit est de
// longueur symb1 sur symb2
//
void _Hypot(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "HYPOT";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = (float) hypot((double) d1,(double) d2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le logarithme naturel du symbole donn.
//
void _Log(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "LOG";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = logf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile log. de 1+symb.
//
void _Log1p(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "LOG1+";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = log1pf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le logarithme en base 10 du symbole donn.
//
void _Log10(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "LOG10";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = log10f(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile symb1 puissance symb2.
//
void _Pow(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   float d1,d2;
   static char cProcName[] = "POW";

   pCurrCV->pNProc = cProcName;
   d2  = RecupNumber(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = powf(d1,d2);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile 10 puissance symb.
//
void _Pow10(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "POW10";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = powf((float) 10,d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile un nombre alatoire compris entre 0 et symb-1.
//
void _Rnd(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   mtWORD wi1;
   static char cProcName[] = "RND";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   wi1 = d1;
   pS2->V.fVal = (float) random(wi1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise le gnrateur de nombre alatoires.
//
void _Randomize(void)
{
   static char cProcName[] = "RANDOMIZE";

   pCurrCV->pNProc = cProcName;
   randomize();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le sinus du symbole donn.
//
void _Sin(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "SIN";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = sinf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le sinus hyperbolique du symbole donn.
//
void _Sinh(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "SINH";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = sinhf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la racine carre du symbole donn.
//
void _Sqrt(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "SQRT";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = sqrtf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la tangante du symbole donn.
//
void _Tan(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "TAN";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = tanf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la tangante hyperbolique du symbole donn.
//
void _Tanh(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "TANH";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = tanhf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la partie entire du symbole donn.
//
void _FInt(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   float d1;
   static char cProcName[] = "INT";

   pCurrCV->pNProc = cProcName;
   d1  = RecupNumber(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = truncf(d1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                            LES CHAINES DE CARACTERES
//                            อออออออออออออออออออออออออ
//
// Concatne la chaine src … la chaine dst.
// Syntaxe : src dst STRCAT
//
void _StrCat(void)
{
   mtPTRDICO pDst,pSrc,pLiSrc,pCoSrc,pLiDst,pCoDst;
   mtPTRBYTE pCSrc,pCDst;
   static char cProcName[] = "STRCAT";

   pCurrCV->pNProc = cProcName;
   pCDst = RecupString(&pDst,&pLiDst,&pCoDst,pCurrCV->pNProc,mtTRUE);
   pCSrc = RecupString(&pSrc,&pLiSrc,&pCoSrc,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pCSrc,"StrCat",pSrc);
   //if(pCDst)
      pCDst = (mtPTRBYTE) MemReAlloc(pCDst,(strlen(pCDst)+strlen(pCSrc))+1);
   //else
      // Premire initialisation.
   //   pCDst = (mtPTRBYTE) MemAlloc(strlen(pCSrc)+1);
   strcat(pCDst,pCSrc);
   if(pDst->Type == mtTABCHAR)
      ArrayCStore(pDst,pLiDst,pCoDst,pCDst);
   else
      pDst->V.cVal = pCDst;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la longueur de la chaine donne.
//
void _StrLen(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "STRLEN";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"StrLen",pS1);
   pS2 = GenCvINUMBER();
   pS2->V.fVal = (float) strlen(pc1);
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti la chaine donne en minuscules.
//
void _StrLower(void)
{
   mtPTRDICO pS1,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "STRLWR";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtTRUE);
//   CheckNull(pc1,"StrLwr",pS1);
   strlwr(pc1);
   if(pS1->Type == mtTABCHAR)
      ArrayCStore(pS1,pLi,pCo,pc1);
   else
      pS1->V.cVal = pc1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti la chaine donne en majuscules.
//
void _StrUpper(void)
{
   mtPTRDICO pS1,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "STRUPR";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtTRUE);
//   CheckNull(pc1,"StrUpr",pS1);
   strupr(pc1);
   if(pS1->Type == mtTABCHAR)
      ArrayCStore(pS1,pLi,pCo,pc1);
   else
      pS1->V.cVal = pc1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Inverse la chaine donne.
//
void _StrRev(void)
{
   mtPTRDICO pS1,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "STRREV";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtTRUE);
//   CheckNull(pc1,"StrRev",pS1);
   strrev(pc1);
   if(pS1->Type == mtTABCHAR)
      ArrayCStore(pS1,pLi,pCo,pc1);
   else
      pS1->V.cVal = pc1;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la position de la sous-chaine Symb2 dans la chaine Symb1.
// Syntaxe : Symb1 Symb2 AT
//
void _StrAt(void)
{
   mtPTRDICO pS1,pS2,pS3,pLi,pCo;
   mtPTRBYTE pc1,pc2,pc3;
   static char cProcName[] = "AT";

   pCurrCV->pNProc = cProcName;
   pc2 = RecupString(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"At",pS2);
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"At",pS1);
   pS3 = GenCvINUMBER();
   pS3->V.fVal = (float) -1;
   pc3 = strstr(pc1,pc2);
   if(pc3)
      pS3->V.fVal = (float) (pc3-pc1);
   _DPush(pS3);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Extrait une sous-chaine de Str … partir de Pos, sur une longueur Lon.
// Syntaxe : Str Pos Lon SUBSTR
//
void _StrSubstr(void)
{
   mtPTRDICO pCh,Pos,Lon,NewCh,pLi,pCo;
   float pd1,pd2;
   mtPTRBYTE pc1;
   mtWORD wLon,wPos;
   static char cProcName[] = "SUBSTR";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pd2 = RecupNumber(&Pos,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"SubStr",pCh);
   NewCh = GenCvICHAR(1);
   wLon = pd1;
   wPos = pd2;
   if(wPos < strlen(pc1))
   {
      if((wPos+wLon) > strlen(pc1))
         wLon = (strlen(pc1)-wPos);
      NewCh->V.cVal = (mtPTRBYTE) MemReAlloc(NewCh->V.cVal,wLon+1);
      memset(NewCh->V.cVal,0,wLon+1);
      strncpy(NewCh->V.cVal,pc1+wPos,wLon);
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti la chaine donne en nombre.
//
void _StrVal(void)
{
   mtPTRDICO pCh,pNum,pLi,pCo;
   mtPTRBYTE pc1,pEnd;
   static char cProcName[] = "VAL";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"Val",pCh);
   pNum = GenCvINUMBER();
   pNum->V.fVal = strtof(pc1,&pEnd);
   _DPush(pNum);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti le nombre donn en une chaine de caractre.
// L'ventuel format dfinit avec USING est utilis.
//
void _StrNum(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   float pNum;
   mtBYTE OBuff[128];
   mtWORD Value;
   static char cProcName[] = "STR";

   pCurrCV->pNProc = cProcName;
   pNum = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(pCurrCV->pUsing)
   {
      if(strpbrk(pCurrCV->pUsing,"eEfgG"))
         sprintf(OBuff,pCurrCV->pUsing,pNum);
      else
      {
         Value = pNum;
         sprintf(OBuff,pCurrCV->pUsing,Value);
      }
   }
   else
      sprintf(OBuff,"%f",pNum);
   pDSym = GenCvICHAR(strlen(OBuff)+1);
   strcpy(pDSym->V.cVal,OBuff);
   _DPush(pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution de RTRIM.
//
void __RTrim(void)
{
   mtPTRDICO pCh,pLi,pCo,NewCh;
   mtPTRBYTE pc1,pc2;
   mtWORD Nbc=0,sLen;

   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,pCurrCV->pNProc,pCh);
   sLen = strlen(pc1);
   pc2  = pc1+(sLen-1);
   while((pc2 >= pc1) && (*pc2 == ' '))
   {
      pc2--;
      Nbc++;
   }
   if(Nbc < sLen)
   {
      NewCh = GenCvICHAR((sLen-Nbc)+1);
      strncpy(NewCh->V.cVal,pc1,(sLen-Nbc));
   }
   else
      // Il n'y avait que des espaces.
      NewCh = GenCvICHAR(1);
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Excution du LTRIM
//
void __LTrim(void)
{
   mtPTRDICO pCh,pLi,pCo,NewCh;
   mtPTRBYTE pc1,pc2;
   mtWORD Nbc=0,sLen;

   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,pCurrCV->pNProc,pCh);
   sLen = strlen(pc1);
   pc2  = pc1;
   while(*pc2 && (*pc2 == ' '))
   {
      pc2++;
      Nbc++;
   }
   if(Nbc < sLen)
   {
      NewCh = GenCvICHAR((sLen-Nbc)+1);
      strcpy(NewCh->V.cVal,pc2);
   }
   else
      // Il n'y avait que des espaces.
      NewCh = GenCvICHAR(1);
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve les espaces … droite de la chaine donne.
//
void _StrRTrim(void)
{
   static char cProcName[] = "RTRIM";

   pCurrCV->pNProc = cProcName;
   __RTrim();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve les espaces … gauche de la chaine donne.
//
void _StrLTrim(void)
{
   static char cProcName[] = "LTRIM";

   pCurrCV->pNProc = cProcName;
   __LTrim();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve les espace … droite et … gauche de la chaine.
//
void _StrAllTrim(void)
{
   static char cProcName[] = "ALLTRIM";

   pCurrCV->pNProc = cProcName;
   __LTrim();
   __RTrim();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti le caractre donn en son code ascii.
//
void _StrAscii(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   mtPTRBYTE pc1;
   static char cProcName[] = "ASC";

   pCurrCV->pNProc = cProcName;
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"Ascii",pS1);
   pS2  = GenCvINUMBER();
   pS2->V.fVal = (float) *pc1;
   _DPush(pS2);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti le nombre donn en un caractre ascii.
//
void _StrChr(void)
{
   mtPTRDICO pDSym,pLi,pCo;
   float pNum;
   mtBYTE OBuff[2];
   mtWORD Value;
   static char cProcName[] = "CHR";

   pCurrCV->pNProc = cProcName;
   pNum  = RecupNumber(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   Value = pNum;
   sprintf(OBuff,"%c",(mtBYTE) pNum);
   pDSym = GenCvICHAR(strlen(OBuff)+1);
   strcpy(pDSym->V.cVal,OBuff);
   _DPush(pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile VRAI si tous les caractres de s2 sont dans s1
// Syntaxe : s1 s2 STRSPN
//
void _StrSpn(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   mtPTRBYTE pc1,pc2;
   static char cProcName[] = "STRSPN";

   pCurrCV->pNProc = cProcName;
   pc2 = RecupString(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"StrSpn",pS2);
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"StrSpn",pS1);
   if(strspn(pc1,pc2) == strlen(pc1))
      __TRUE();
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile VRAI si s1 contient un des caractres de s2
// Syntaxe : s1 s2 STRIN
//
void _StrPbrk(void)
{
   mtPTRDICO pS1,pS2,pLi,pCo;
   mtPTRBYTE pc1,pc2;
   static char cProcName[] = "STRIN";

   pCurrCV->pNProc = cProcName;
   pc2 = RecupString(&pS2,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"StrIn",pS2);
   pc1 = RecupString(&pS1,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"StrIn",pS1);
   if(strpbrk(pc1,pc2))
      __TRUE();
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcoupe la chaine donne en items dlimits par le dlimiteur donn,
// et stocke ceux-ci dans le tableau donn.
// Syntaxe : Chaine Delim Tab STRTOK
//
void _StrTok(void)
{
   mtPTRDICO pSCh,pSDl,pTDst,pLi,pCo;
   mtPTRBYTE pCh,pDl;
   mtPTRITAB pIDst;
   static char cProcName[] = "STRTOK";

   pCurrCV->pNProc = cProcName;
   pTDst = (mtPTRDICO) _DPop();
   if(pTDst->Type != mtTABCHAR)
      ExecError(pCurrCV->pNProc,ERRTCHAREXPECT,pTDst);
   pDl = RecupString(&pSDl,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pDl,"StrTok",pSDl);
   pCh = RecupString(&pSCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pCh,"StrTok",pSCh);
   // Prpare la suppression de l'ancien tableau.
   pIDst            = (mtPTRITAB) pTDst->XVal;
   pCurrCV->OTType  = pTDst->Type;
   pCurrCV->OldTab  = pTDst->V.tVal;
   pCurrCV->ALigDel = 0;
   pCurrCV->AColDel = 0;
   pCurrCV->MaxLi   = pIDst->MaxLig;
   pCurrCV->MaxCo   = pIDst->MaxCol;
   // Prpare le dcoupage.
   pIDst->MaxLig    = 1;                    // 1 ligne
   pIDst->MaxCol    = 1;                    // et 1 colonne par dfaut.
   pTDst->V.tVal    = (pPOINTEUR) MemAlloc((pIDst->MaxLig*pIDst->MaxCol)*sizeof(mtPOINTEUR));
   pTDst->XVal      = (mtPOINTEUR) pIDst;
   pCurrCV->ATabDst = pTDst;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
   pCurrCV->pChaine = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
   strcpy(pCurrCV->pChaine,pCh);
   pCurrCV->pDelim = (mtPTRBYTE) MemAlloc(strlen(pDl)+1);
   strcpy(pCurrCV->pDelim,pDl);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Encodage d'une chaine
// Syntaxe : FmtSrc FmtDst Src ENCODE
//
void _StrEncode(void)
{
   mtPTRDICO pSym,pLi,pCo,NewCh;
   mtPTRBYTE pdst,psrc,pfmtdst,pfmtsrc;
   static char cProcName[] = "ENCODE";

   pCurrCV->pNProc = cProcName;
   psrc = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(psrc,"Encode",pSym);
   pfmtdst = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pfmtdst,"Encode",pSym);
   pfmtsrc = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pfmtsrc,"Encode",pSym);
   NewCh = GenCvICHAR(strlen(pfmtdst)+1);
   c4encode(NewCh->V.cVal,psrc,pfmtdst,pfmtsrc);
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupre la liste des fichiers correspondants au masque donn
// et stocke ceux-ci dans le tableau donn.
// Syntaxe : Masque Tab ADIR
//
void _ADir(void)
{
   mtPTRDICO pSMsk,pTDst,pLi,pCo;
   mtPTRBYTE pMask;
   mtPTRITAB pIDst;
   static char cProcName[] = "ADIR";

   pCurrCV->pNProc = cProcName;
   pTDst = (mtPTRDICO) _DPop();
   if(pTDst->Type != mtTABCHAR)
      ExecError("Adir",ERRTCHAREXPECT,pTDst);
   pMask = RecupString(&pSMsk,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pMask,"Adir",pSMsk);
   // Prpare la suppression de l'ancien tableau.
   pIDst            = (mtPTRITAB) pTDst->XVal;
   pCurrCV->OTType  = pTDst->Type;
   pCurrCV->OldTab  = pTDst->V.tVal;
   pCurrCV->ALigDel = 0;
   pCurrCV->AColDel = 0;
   pCurrCV->MaxLi   = pIDst->MaxLig;
   pCurrCV->MaxCo   = pIDst->MaxCol;
   // Prpare la rcupration.
   pIDst->MaxLig    = 0;                    // MaxLig et V.tVal vont tre initialiss
   pIDst->MaxCol    = 4;                    // par __CountADirFiles.
   pTDst->XVal      = (mtPOINTEUR) pIDst;
   pCurrCV->ATabDst = pTDst;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
   pCurrCV->pMask   = (mtPTRBYTE) MemAlloc(strlen(pMask)+1);
   strcpy(pCurrCV->pMask,pMask);
   pCurrCV->uFind = _dos_findfirst(pCurrCV->pMask,_A_NORMAL,&pCurrCV->sFIND);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Remplace les occurences d'une chaine par une autre, dans la chaine donne.
// Syntaxe : OccSrc OccDst Chaine STRTRAN
//
void _StrTran(void)
{
   mtPTRDICO pSCOSrc,pSCODst,pChai,pChLi,pChCo,pLi,pCo;
   mtPTRBYTE OldCh,pcOSrc,pcODst,pCh,pOcc,NewCh=NULL;
   mtUDWORD tCh;
   static char cProcName[] = "STRTRAN";

   pCurrCV->pNProc = cProcName;
   pCh    = RecupString(&pChai,&pChLi,&pChCo,pCurrCV->pNProc,mtTRUE);
//   CheckNull(pCh,"StrTran",pChai);
   pcODst = RecupString(&pSCODst,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pcODst,"StrTran",pSCODst);
   pcOSrc = RecupString(&pSCOSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pcOSrc,"StrTran",pSCOSrc);
   OldCh  = pCh;
   pOcc   = strstr(pCh,pcOSrc);
   while(pOcc)
   {
      tCh = (pOcc-pCh)+strlen(pcODst);
      if(NewCh)
         NewCh = (mtPTRBYTE) MemReAlloc(NewCh,(strlen(NewCh)+tCh)+1);
      else
         NewCh = (mtPTRBYTE) MemAlloc(tCh+1);
      strncat(NewCh,pCh,(pOcc-pCh));
      strcat(NewCh,pcODst);
      pCh  = pOcc+strlen(pcOSrc);
      pOcc = strstr(pCh,pcOSrc);
   }
   if(strlen(pCh))
   {
      if(NewCh)
         NewCh = (mtPTRBYTE) MemReAlloc(NewCh,(strlen(NewCh)+strlen(pCh))+1);
      else
         NewCh = (mtPTRBYTE) MemAlloc(strlen(pCh)+1);
      strcat(NewCh,pCh);
   }
   MemFree(OldCh);
   if(pChai->Type == mtTABCHAR)
      ArrayCStore(pChai,pChLi,pChCo,NewCh);
   else
      pChai->V.cVal = NewCh;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile les n caractres … gauche de la chaine
// Syntaxe : Str Len LEFT
//
void _StrLeft(void)
{
   mtPTRDICO pCh,Lon,NewCh,pLi,pCo;
   float pd1;
   mtPTRBYTE pc1;
   mtWORD wLon;
   static char cProcName[] = "LEFT";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"Left",pCh);
   NewCh = GenCvICHAR(1);
   wLon  = pd1;
   if(wLon > strlen(pc1))
      wLon = strlen(pc1);
   NewCh->V.cVal = (mtPTRBYTE) MemReAlloc(NewCh->V.cVal,wLon+1);
   memset(NewCh->V.cVal,0,wLon+1);
   strncpy(NewCh->V.cVal,pc1,wLon);
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile les n caractres … droite de la chaine
// Syntaxe : Str Len RIGHT
//
void _StrRight(void)
{
   mtPTRDICO pCh,Lon,NewCh,pLi,pCo;
   float pd1;
   mtPTRBYTE pc1;
   mtWORD wLon;
   static char cProcName[] = "RIGHT";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc1,"Right",pCh);
   NewCh = GenCvICHAR(1);
   wLon  = pd1;
   if(wLon > strlen(pc1))
      wLon = strlen(pc1);
   NewCh->V.cVal = (mtPTRBYTE) MemReAlloc(NewCh->V.cVal,wLon+1);
   memset(NewCh->V.cVal,0,wLon+1);
   strncpy(NewCh->V.cVal,pc1+(strlen(pc1)-wLon),wLon);
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Complte la chaine donne, jusqu'… ce qu'elle atteigne la longueur donne,
// avec le caractre donn. Le caractre est mis … gauche de la chaine.
// Empile la nouvelle chaine.
// Syntaxe : Chaine Car Len PADL
//
void _StrPadL(void)
{
   mtPTRDICO pCh,pCar,Lon,NewCh,pLi,pCo;
   float pd1;
   mtPTRBYTE pc1,pc2;
   mtWORD wLon,cPos;
   mtBYTE cCar;
   static char cProcName[] = "PADL";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc2 = RecupString(&pCar,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadL",pCar);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadL",pCh);
   wLon = pd1;
   cCar = *pc2;
   NewCh = GenCvICHAR(wLon+1);
   if(wLon > strlen(pc1))
   {
      wLon -= strlen(pc1);
      cPos  = 0;
      while(wLon)
      {
         *(NewCh->V.cVal+cPos) = cCar;
         cPos++;
         wLon--;
      }
      strcpy((NewCh->V.cVal+cPos),pc1);
   }
   else
   {
      // La chaine est plus longue que wLon.
      strncpy(NewCh->V.cVal,pc1,wLon);
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Complte la chaine donne, jusqu'… ce qu'elle atteigne la longueur donne,
// avec le caractre donn. Le caractre est mis … droite de la chaine.
// Empile la nouvelle chaine.
// Syntaxe : Chaine Car Len PADR
//
void _StrPadR(void)
{
   mtPTRDICO pCh,pCar,Lon,NewCh,pLi,pCo;
   float pd1;
   mtPTRBYTE pc1,pc2;
   mtWORD wLon,cPos;
   mtBYTE cCar;
   static char cProcName[] = "PADR";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc2 = RecupString(&pCar,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadR",pCar);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadR",pCh);
   wLon = pd1;
   cCar = *pc2;
   NewCh = GenCvICHAR(wLon+1);
   if(wLon > strlen(pc1))
   {
      strcpy(NewCh->V.cVal,pc1);
      wLon -= strlen(pc1);
      cPos  = strlen(pc1);
      while(wLon)
      {
         *(NewCh->V.cVal+cPos) = cCar;
         cPos++;
         wLon--;
      }
   }
   else
   {
      // La chaine est plus longue que wLon.
      strncpy(NewCh->V.cVal,pc1,wLon);
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Centre la chaine donne, jusqu'… ce qu'elle atteigne la longueur donne,
// avec le caractre donn. Le caractre est mis de part et d'autre
// de la chaine.
// Empile la nouvelle chaine.
// Syntaxe : Chaine Car Len PADC
//
void _StrPadC(void)
{
   mtPTRDICO pCh,pCar,Lon,NewCh,pLi,pCo;
   float pd1;
   mtPTRBYTE pc1,pc2;
   mtWORD wLon,cPos,cLon;
   mtBYTE cCar;
   static char cProcName[] = "PADC";

   pCurrCV->pNProc = cProcName;
   pd1 = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pc2 = RecupString(&pCar,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadC",pCar);
   pc1 = RecupString(&pCh,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pc2,"PadC",pCh);
   wLon = pd1;
   cCar = *pc2;
   NewCh = GenCvICHAR(wLon+1);
   if(wLon > strlen(pc1))
   {
      cLon  = (wLon-strlen(pc1)) >> 1;
      cPos  = 0;
      while(cLon)
      {
         *(NewCh->V.cVal+cPos) = cCar;
         cPos++;
         cLon--;
      }
      strcpy(NewCh->V.cVal+cPos,pc1);
      cLon  = strlen(NewCh->V.cVal);
      cPos  = strlen(NewCh->V.cVal);
      while(cLon<wLon)
      {
         *(NewCh->V.cVal+cPos) = cCar;
         cPos++;
         cLon++;
      }
   }
   else
   {
      // La chaine est plus longue que wLon.
      strncpy(NewCh->V.cVal,pc1,wLon);
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Copie les len caractres de la chaine src dans la chaine dst … partir
// de la position pos.
// Empile la nouvelle chaine.
// Syntaxe : Src Dst Pos Lon STRINS
//
void _StrIns(void)
{
   mtPTRDICO pCSrc,pCDst,Pos,Lon,NewCh,pLi,pCo;
   mtPTRBYTE pcS,pcD;
   float fLen,fPos;
   mtWORD wLon,wPos;
   static char cProcName[] = "STRINS";

   pCurrCV->pNProc = cProcName;
   fLen = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   fPos = RecupNumber(&Pos,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pcD  = RecupString(&pCDst,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pcS  = RecupString(&pCSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon = fLen;
   wPos = fPos;
   if((wPos+1) > strlen(pcD))
      // Position de copie hors limite.
      wPos = strlen(pcD);
   if(wLon > strlen(pcS))
      wLon = strlen(pcS);
   NewCh = GenCvICHAR(strlen(pcD)+wLon+1);
   // On prend les n premiers car.
   strncpy(NewCh->V.cVal,pcD,((pcD+wPos)-pcD));
   strncat(NewCh->V.cVal,pcS,wLon);
   if(((pcD+wPos)-pcD) < strlen(pcD))
      strcat(NewCh->V.cVal,pcD+((pcD+wPos)-pcD));
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime les len caractres de la chaine donne, … partir de la position
// donne.
// Empile la nouvelle chaine.
// Syntaxe : Src Pos Lon STRDEL
//
void _StrDel(void)
{
   mtPTRDICO pCSrc,Pos,Lon,NewCh,pLi,pCo;
   mtPTRBYTE pcS;
   float fLen,fPos;
   mtWORD wLon,wPos;
   static char cProcName[] = "STRDEL";

   pCurrCV->pNProc = cProcName;
   fLen = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   fPos = RecupNumber(&Pos,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pcS  = RecupString(&pCSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon = fLen;
   wPos = fPos;
   if((wPos+1) > strlen(pcS))
   {
      // Position hors limite, pas de suppression.
      NewCh = GenCvICHAR(strlen(pcS)+1);
      strcpy(NewCh->V.cVal,pcS);
   }
   else
   {
      if(wLon > strlen(pcS))
         wLon = strlen(pcS);
      NewCh = GenCvICHAR((strlen(pcS)-wLon)+1);
      // On prend les n premiers car.
      strncpy(NewCh->V.cVal,pcS,((pcS+wPos)-pcS));
      if((wPos+wLon) < strlen(pcS))
         strcat(NewCh->V.cVal,pcS+(wPos+wLon));
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Remplace les len caractres de la chaine donne, par le caractre donn,
// … partir de la position doone.
// Empile la nouvelle chaine.
// Syntaxe : Src Pos Len Car STRSET
//
void _StrSet(void)
{
   mtPTRDICO pCSrc,Pos,Lon,pCar,NewCh,pLi,pCo;
   mtPTRBYTE pcS,pcC;
   float fLen,fPos;
   mtBYTE cCar;
   mtWORD wLon,wPos;
   static char cProcName[] = "STRSET";

   pCurrCV->pNProc = cProcName;
   pcC   = RecupString(&pCar,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   fLen  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   fPos  = RecupNumber(&Pos,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pcS   = RecupString(&pCSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wPos  = fPos;
   NewCh = GenCvICHAR(strlen(pcS)+1);
   strcpy(NewCh->V.cVal,pcS);
   if((wPos+1) < strlen(pcS))
   {
      wLon = fLen;
      cCar = *pcC;
      if(wLon > strlen(pcS))
         wLon = strlen(pcS);
      strnset(NewCh->V.cVal+wPos,cCar,wLon);
   }
   _DPush(NewCh);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Cherche dans la chaine donne, un motif dfini par l'expression rgulire
// donne.
// Empile TRUE et la chaine trouve, FALSE et chaine vide sinon.
//
void _StrMatch(void)
{
   mtPTRDICO pCSrc,pRexp,NewCh,pLi,pCo;
   mtPTRBYTE pCh,pEx;
   mtPTRREGEXP rgx;
   static char cProcName[] = "STRMATCH";

   pCurrCV->pNProc = cProcName;
   pEx   = RecupString(&pRexp,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pCh   = RecupString(&pCSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   NewCh = GenCvICHAR(1);
   rgx   = regcomp(pEx);
   if(! rgx)
   {
      MemFree(rgx);
      _DPush(NewCh);
      __FALSE();
   }
   else
   {
      if(regexec(rgx,pCh))
      {
         NewCh->V.cVal = (mtPTRBYTE) MemReAlloc(NewCh->V.cVal,(rgx->endp[0]-rgx->startp[0])+1);
         memset(NewCh->V.cVal,0,(rgx->endp[0]-rgx->startp[0])+1);
         strncpy(NewCh->V.cVal,rgx->startp[0],(rgx->endp[0]-rgx->startp[0]));
         MemFree(rgx);
         _DPush(NewCh);
         __TRUE();
      }
      else
      {
         MemFree(rgx);
         _DPush(NewCh);
         __FALSE();
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                       LES VARIABLES ET CONSTANTES SYSTEMES
//                       ออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction ENVOI.
//
void _SysFKEnvoi(void)
{
   static char cProcName[] = "FKENVOI";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKENVOI);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction RETOUR.
//
void _SysFKRetour(void)
{
   static char cProcName[] = "FKRETOUR";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKRETOUR);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction REPETITION.
//
void _SysFKRepetition(void)
{
   static char cProcName[] = "FKREPETITION";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKREPETITION);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction GUIDE.
//
void _SysFKGuide(void)
{
   static char cProcName[] = "FKGUIDE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKGUIDE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction ANNULATION.
//
void _SysFKAnnulation(void)
{
   static char cProcName[] = "FKANNULATION";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKANNULATION);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction SOMMAIRE.
//
void _SysFKSommaire(void)
{
   static char cProcName[] = "FKSOMMAIRE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKSOMMAIRE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction CORRECTION.
//
void _SysFKCorrection(void)
{
   static char cProcName[] = "FKCORRECTION";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKCORRECTION);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type de la touche de fonction SUITE.
//
void _SysFKSuite(void)
{
   static char cProcName[] = "FKSUITE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFKSUITE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur NOIR.
//
void _SysBlack(void)
{
   static char cProcName[] = "BLACK";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTBLACK);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur ROUGE.
//
void _SysRed(void)
{
   static char cProcName[] = "RED";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTRED);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur VERT.
//
void _SysGreen(void)
{
   static char cProcName[] = "GREEN";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTGREEN);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur JAUNE.
//
void _SysYellow(void)
{
   static char cProcName[] = "YELLOW";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTYELLOW);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur BLEU.
//
void _SysBlue(void)
{
   static char cProcName[] = "BLUE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTBLUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur MAGENTA.
//
void _SysMagenta(void)
{
   static char cProcName[] = "MAGENTA";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTMAGENTA);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur CYAN.
//
void _SysCyan(void)
{
   static char cProcName[] = "CYAN";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTCYAN);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la couleur BLANC.
//
void _SysWhite(void)
{
   static char cProcName[] = "WHITE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTWHITE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la simple taille.
//
void _SysFSimple(void)
{
   static char cProcName[] = "FSIMPLE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTFSIMPLE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la double hauteur.
//
void _SysDHauteur(void)
{
   static char cProcName[] = "FDHEIGHT";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTDHAUTEUR);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la double largeur.
//
void _SysDLargeur(void)
{
   static char cProcName[] = "FDWEIGHT";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTDLARGEUR);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la double taille.
//
void _SysDTaille(void)
{
   static char cProcName[] = "FDSIZE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTDTAILLE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la constante du mode VidoTex.
//
void _SysTModeVDT(void)
{
   static char cProcName[] = "TMODEVDT";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTTMVDT);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la constante du mode Mixte.
//
void _SysTModeMXT(void)
{
   static char cProcName[] = "TMODEMIXTE";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTTMMXT);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la constante du mode TlInformatique.
//
void _SysTModeTEL(void)
{
   static char cProcName[] = "TMODETELEINFO";

   pCurrCV->pNProc = cProcName;
   _DPush(pCONSTTMTEL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le mode courant du connect.
//
void _SysGetTMode(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "GETTMODE";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->cMode;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de la touche de fonction appuye.
//
void _SysMTouche(void)
{
   mtPTRDICO pCode;
   static char cProcName[] = "FKEY";

   pCurrCV->pNProc = cProcName;
   pCode = GenCvICHAR(2);
   *(pCode->V.cVal) = pCurrCV->TFonction;
   _DPush(pCode);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile les donnes saisies par le minitliste.
//
void _SysMData(void)
{
   mtPTRDICO pDat;
   static char cProcName[] = "DATA";

   pCurrCV->pNProc = cProcName;
   pDat = GenCvICHAR(1);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire
      // (Les donnes reues sont communes)
      pCurrCV->pData = pCurrLT->pData;
   if(pCurrCV->pData)
   {
      pDat->V.cVal = (mtPTRBYTE) MemReAlloc(pDat->V.cVal,strlen(pCurrCV->pData)+1);
      memset(pDat->V.cVal,0,strlen(pCurrCV->pData)+1);
      strcpy(pDat->V.cVal,pCurrCV->pData);
   }
   _DPush(pDat);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'heure courante sous forme de chaine "HH:MM:SS".
//
void _SysTime(void)
{
   mtPTRDICO pHeure;
   static char cProcName[] = "TIME";

   pCurrCV->pNProc = cProcName;
   pHeure = GenCvICHAR(9);
   sprintf(pHeure->V.cVal,"%02d:%02d:%02d",Heures,Minutes,Secondes);
   _DPush(pHeure);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'heure courante sous forme de centimes de secondes.
//
void _SysTimer(void)
{
   mtPTRDICO pSym;
   struct dos_time_t HTime;
   static char cProcName[] = "TIMER";

   pCurrCV->pNProc = cProcName;
   _dos_gettime(&HTime);
   pSym = GenCvINUMBER();
   pSym->V.fVal = TimeToCent(&HTime);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la date courante sous forme de chaine "JJ/MM/AAAA".
//
void _SysDate(void)
{
   mtPTRDICO pDate;
   static char cProcName[] = "DATE";

   pCurrCV->pNProc = cProcName;
   pDate = GenCvICHAR(11);
   sprintf(pDate->V.cVal,"%02d/%02d/%04d",Jour,Mois,Annee);
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la trame de connexion du connect courant.
//
void _SysMTrameCNX(void)
{
   mtPTRDICO pDat;
   static char cProcName[] = "CNXDATA";

   pCurrCV->pNProc = cProcName;
   pDat = GenCvICHAR(1);
   if(strlen(pCurrCV->TrameCNX))
   {
      pDat->V.cVal = (mtPTRBYTE) MemReAlloc(pDat->V.cVal,strlen(pCurrCV->TrameCNX)+1);
      memset(pDat->V.cVal,0,strlen(pCurrCV->TrameCNX)+1);
      strcpy(pDat->V.cVal,pCurrCV->TrameCNX);
   }
   _DPush(pDat);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le dpartement du connect.
//
void _SysDept(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "DEPT";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->Dept;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le n๘ de PAVI du connect.
//
void _SysPavi(void)
{
   mtPTRDICO pPav;
   static char cProcName[] = "PAVI";

   pCurrCV->pNProc = cProcName;
   pPav = GenCvICHAR(7);
   strcpy(pPav->V.cVal,pCurrCV->Pavi);
   _DPush(pPav);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le palier du connect.
//
void _SysPalier(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "PALIER";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->Palier;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le N๘ complmentaire du connect.
//
void _SysNComp(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "NCOMP";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->NumComp;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom du service appel par le connect.
//
void _SysNApp(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "APP";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(11);
   strcpy(pSym->V.cVal,pCurrCV->Service);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la date de connexion du connect sous la forme "JJ/MM/AAAA".
//
void _SysCnxDate(void)
{
   mtPTRDICO pDate;
   static char cProcName[] = "CNXDATE";

   pCurrCV->pNProc = cProcName;
   pDate = GenCvICHAR(11);
   sprintf(pDate->V.cVal,"%02d/%02d/%04d",pCurrCV->DateCnx.day,pCurrCV->DateCnx.month,pCurrCV->DateCnx.year);
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'heure de connexion du connect sous la forme "HH:MM:SS".
//
void _SysCnxTime(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "CNXTIME";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(9);
   sprintf(pSym->V.cVal,"%02d:%02d:%02d",pCurrCV->HeureCnx.hour,pCurrCV->HeureCnx.minute,pCurrCV->HeureCnx.second);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type du minitel du connect.
//
void _SysMType(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "MTYPE";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->TypeMinitel;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le n๘ de CV du connect courant.
//
void _SysNCV(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "NUMCV";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) pCurrCV->NumCv;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la date de lancement du serveur sous la forme "JJ/MM/AAAA".
//
void _SysSrvDLanc(void)
{
   mtPTRDICO pDate;
   static char cProcName[] = "SRVLAUNCHDATE";

   pCurrCV->pNProc = cProcName;
   pDate = GenCvICHAR(11);
   sprintf(pDate->V.cVal,"%02d/%02d/%04d",DLanc.day,DLanc.month,DLanc.year);
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'heure de lancement du serveur sous la forme "HH:MM:SS".
//
void _SysSrvHLanc(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVLAUNCHTIME";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(9);
   sprintf(pSym->V.cVal,"%02d:%02d:%02d",HLanc.hour,HLanc.minute,HLanc.second);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la mmoire disponible, en octets
//
void _SysSrvMemAvail(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVMEMAVAIL";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) RAMDispo;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la charge actuelle du serveur, en %
//
void _SysSrvChargeA(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVCHARGE";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = ChargeA;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la charge maxi du serveur, en %
//
void _SysSrvChargeM(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVCHARGEMAX";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = ChargeM;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre de connect du serveur
//
void _SysSrvNbCnx(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVNBCNX";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) NbCnx;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre de connect maxi du serveur
//
void _SysSrvPointeCnx(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVMAXCNX";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) PointeCnx;
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'heure d'arret du serveur sous la forme "HH:MM:SS".
//
void _SysSrvHFinServ(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "SRVSTOPTIME";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(9);
   sprintf(pSym->V.cVal,"%02d:%02d:00",HFinServ.hour,HFinServ.minute);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                       MANIPULATION DES BASES DE DONNEES
//                       อออออออออออออออออออออออออออออออออ
//
// Ouverture d'une base de donnes.
// Slectionne le 1er index ventuel par dfaut.
// Syntaxe : NomBase DBOPEN
//
void _DbOpen(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE NomBase;
   char FBase[MAXPATH],*NomFic;
   DATA4 *DbTmp;
   TAG4 *Idx;
   static char cProcName[] = "DBOPEN";

   pCurrCV->pNProc = cProcName;
   NomBase = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomBase,"DbOpen",pSym);
   NomFic = filespecname(NomBase);
   if(strlen(NomFic) == strlen(NomBase))
   {
      // Pas de chemin spcifi.
      if(pCurrCV->pApp->PathDBF)
      {
         // On ajoute le chemin des bases.
         strcpy(FBase,pCurrCV->pApp->PathDBF);
         strcat(FBase,NomBase);
      }
      else
      {
         if(pCurrCV->pApp->Path)
         {
            // On ajoute le chemin principal.
            strcpy(FBase,pCurrCV->pApp->Path);
            strcat(FBase,NomBase);
         }
         else
            strcpy(FBase,NomBase);
      }
   }
   else
      strcpy(FBase,NomBase);
   // Base dj… ouverte ?
   DbTmp = d4data(&pCurrCV->CBase,FBase);
   if(! DbTmp)
   {
      DbTmp = d4open(&pCurrCV->CBase,FBase);
      if(! DbTmp)
      {
         ExecError(pCurrCV->pNProc,ERRDBFAILOPEN,pSym);
         __FALSE();
      }
      else
      {
         pCurrCV->CurrBase = DbTmp;
         // Y a t'il un index ?
         Idx = d4tag_next(pCurrCV->CurrBase,NULL);
         if(Idx)
            d4tag_select(pCurrCV->CurrBase,Idx);
         // On se place au dbut de la base.
         pCurrCV->DbError = d4top(pCurrCV->CurrBase);
         __TRUE();
      }
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBOPEN,pSym);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ferme une base de donnes.
// Syntaxe : NomBase DBCLOSE
void _DbClose(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE NomBase;
   DATA4 *DbTmp;
   static char cProcName[] = "DBCLOSE";

   pCurrCV->pNProc = cProcName;
   NomBase = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomBase,"DbClose",pSym);
   // Base ouverte ?
   DbTmp = d4data(&pCurrCV->CBase,NomBase);
   if(DbTmp)
   {
      // Base courante ?
      if((DATA4 *) pCurrCV->CurrBase == (DATA4 *) DbTmp)
      {
         pCurrCV->DbError = d4close(pCurrCV->CurrBase);
         if(pCurrCV->DbError == 0)
            pCurrCV->CurrBase = (DATA4 *) NULL;
      }
      else
         pCurrCV->DbError = d4close(DbTmp);
      if(pCurrCV->DbError)
         ExecError(pCurrCV->pNProc,ERRDBFAILCLOSE,pSym);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ferme toutes les bases du cv courant.
// Syntaxe : DBCLOSEALL
//
void _DbCloseAll(void)
{
   static char cProcName[] = "DBCLOSEALL";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->DbError = d4close_all(&pCurrCV->CBase);
   if(pCurrCV->DbError == 0)
      pCurrCV->CurrBase = (DATA4 *) NULL;
   else
      ExecError(pCurrCV->pNProc,ERRDBALLCLOSE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute un enregistrement vide … la base courante.
// Syntaxe : DBAPPENDBLANK
//
void _DbAppendBlank(void)
{
   static char cProcName[] = "DBAPPENDBLANK";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      pCurrCV->DbError = d4append_blank(pCurrCV->CurrBase);
      if(pCurrCV->DbError)
         ExecError("DbAppendBlank",ERRDBAPPENDBLANK,NULL);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on est au dbut de la base courante.
// Syntaxe : DBBOF
void _DbBof(void)
{
   static char cProcName[] = "DBBOF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4bof(pCurrCV->CurrBase))
         __TRUE();
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Se place … la fin de la base courante.
// Syntaxe : DBBOTTOM
void _DbBottom(void)
{
   static char cProcName[] = "DBBOTTOM";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4bottom(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Slectionne une base de donnes.
// Syntaxe : NomBase DBSELECT
//
void _DbSelect(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE NomBase;
   char *DbAlias;
   DATA4 *DbTmp;
   static char cProcName[] = "DBSELECT";

   pCurrCV->pNProc = cProcName;
   NomBase = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomBase,"DbSelect",pSym);
   // On enlve l'eventuelle extension.
   DbAlias = filespecgetroot(NomBase);
   // Base ouverte ?
   DbTmp = d4data(&pCurrCV->CBase,DbAlias);
   MemFree(DbAlias);
   if(DbTmp)
      pCurrCV->CurrBase = DbTmp;
   else
      ExecError(pCurrCV->pNProc,ERRDBNOOPEN,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom de la base de donnes courante.
//
void _DbSelected(void)
{
   mtPTRDICO pSym;
   mtPTRBYTE pNomBase;
   static char cProcName[] = "DBSELECTED";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvICHAR(1);
   if(pCurrCV->CurrBase)
   {
      pNomBase = d4alias(pCurrCV->CurrBase);
      if(pNomBase)
      {
         pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(pNomBase)+1);
         memset(pSym->V.cVal,0,strlen(pNomBase)+1);
         strcpy(pSym->V.cVal,pNomBase);
      }
   }
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// change le nom d'une base de donnes.
//
void _DbSetAlias(void)
{
   mtPTRDICO pBase,pSym,pLi,pCo;
   mtPTRBYTE NomBase,Alias;
   DATA4 *DbTmp;
   static char cProcName[] = "DBSETALIAS";

   pCurrCV->pNProc = cProcName;
   Alias = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(Alias,"DbSetAlias",pSym);
   NomBase = RecupString(&pBase,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomBase,"DbSetAlias",pBase);
   // Base ouverte ?
   DbTmp = d4data(&pCurrCV->CBase,NomBase);
   if(DbTmp)
      d4alias_set(DbTmp,Alias);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOOPEN,pBase);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Marque l'enregistrement courant comme effac.
// Syntaxe : DBDELETE
//
void _DbDelete(void)
{
   static char cProcName[] = "DBDELETE";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      d4delete(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si l'enreg. courant est effac.
// Syntaxe : DBDELETED
//
void _DbDeleted(void)
{
   static char cProcName[] = "DBDELETED";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4deleted(pCurrCV->CurrBase))
         __TRUE();
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on est … la fin de la base courante.
// Syntaxe : DBEOF
void _DbEof(void)
{
   static char cProcName[] = "DBEOF";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4eof(pCurrCV->CurrBase))
         __TRUE();
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
      __TRUE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Met … jour (physiquement) la base de donnes courante.
// Syntaxe : DBCOMMIT
//
void _DbCommit(void)
{
   static char cProcName[] = "DBCOMMIT";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4flush(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Met … jour (physiquement) toutes les base de donnes du cv courant.
// Syntaxe : DBCOMMITALL
//
void _DbCommitAll(void)
{
   static char cProcName[] = "DBCOMMITALL";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->DbError = d4flush_files(&pCurrCV->CBase);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Se place sur l'enreg. donne de la base courante.
// Syntaxe : Num DBGO
void _DbGo(void)
{
   mtPTRDICO pSym,pLi,pCo;
   float pRec;
   mtDWORD RecNo;
   static char cProcName[] = "DBGO";

   pCurrCV->pNProc = cProcName;
   pRec  = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   RecNo = pRec;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4go(pCurrCV->CurrBase,RecNo);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// verouille l'enreg. courant.
// Syntaxe : DBLOCK
//
void _DbLock(void)
{
   static char cProcName[] = "DBLOCK";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4recno(pCurrCV->CurrBase) >= (mtDWORD) 1)
         pCurrCV->DbError = d4lock(pCurrCV->CurrBase,d4recno(pCurrCV->CurrBase));
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si l'enreg. courant est verouill.
// Syntaxe : DBLOCKED
//
void _DbLocked(void)
{
   static char cProcName[] = "DBLOCKED";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4recno(pCurrCV->CurrBase) >= (mtDWORD) 1)
      {
         if(d4lock_test(pCurrCV->CurrBase,d4recno(pCurrCV->CurrBase)))
            __TRUE();
         else
            __FALSE();
      }
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
      __TRUE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// verouille la base courante, ainsi que tous les fichiers associs.
// Syntaxe : DBFLOCK
//
void _DbFLock(void)
{
   static char cProcName[] = "DBFLOCK";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4lock_file(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si la base courante est verouille.
// Syntaxe : DBFLOCKED
//
void _DbFLocked(void)
{
   static char cProcName[] = "DBFLOCKED";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
   {
      if(d4lock_test_file(pCurrCV->CurrBase))
         __TRUE();
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
      __TRUE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom de l'index courant de la base courante.
//
void _DbIdxName(void)
{
   mtPTRDICO pName;
   TAG4 *Tag;
   static char cProcName[] = "DBINDEX";

   pCurrCV->pNProc = cProcName;
   pName = GenCvICHAR(1);
   if(pCurrCV->CurrBase)
   {
      // On prend l'index courant.
      Tag = d4tag_selected(pCurrCV->CurrBase);
      if(Tag)
      {
         pName->V.cVal = (mtPTRBYTE) MemReAlloc(pName->V.cVal,strlen(Tag->alias)+1);
         memset(pName->V.cVal,0,strlen(Tag->alias)+1);
         strcpy(pName->V.cVal,Tag->alias);
      }
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
   _DPush(pName);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la clef de l'index courant de la base courante.
//
void _DbIdxKey(void)
{
   mtPTRDICO pKey;
   TAG4 *Tag;
   static char cProcName[] = "DBKEY";

   pCurrCV->pNProc = cProcName;
   pKey = GenCvICHAR(1);
   if(pCurrCV->CurrBase)
   {
      // On prend l'index courant.
      Tag = d4tag_selected(pCurrCV->CurrBase);
      if(Tag)
      {
         pKey->V.cVal = (mtPTRBYTE) MemReAlloc(pKey->V.cVal,strlen(expr4source(Tag->expr))+1);
         memset(pKey->V.cVal,0,strlen(expr4source(Tag->expr))+1);
         strcpy(pKey->V.cVal,expr4source(Tag->expr));
      }
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
   _DPush(pKey);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve la marque d'effacement de l'enreg. courant.
// Syntaxe : DBRECALL
//
void _DbRecall(void)
{
   static char cProcName[] = "DBRECALL";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      d4recall(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre d'enreg. de la base courante.
// Syntaxe : DBRECCOUNT
//
void _DbReccount(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "DBRECCOUNT";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) 0;
   if(pCurrCV->CurrBase)
      pSym->V.fVal = (float) d4reccount(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le n๘ de l'enreg. courant de la base courante.
// Syntaxe : DBRECNO
//
void _DbRecno(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "DBRECNO";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   pSym->V.fVal = (float) 0;
   if(pCurrCV->CurrBase)
      pSym->V.fVal = (float) d4recno(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une clef (type caractre) avec l'index courant.
// Syntaxe : Clef DBSEEK
// Si l'index est de type DATE, Clef doit tre du genre "AAAAMMJJ"
// Si l'index est de type NUMERIC, Clef doit tre du genre "99.99"
//
void _DbSeek(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE pClef;
   int RetVal;
   static char cProcName[] = "DBSEEK";

   pCurrCV->pNProc = cProcName;
   pClef = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(pClef,"DbSeek",pSym);
   if(pCurrCV->CurrBase)
   {
      RetVal = d4seek(pCurrCV->CurrBase,pClef);
      if(RetVal == r4success)
         __TRUE();
      else
         __FALSE();
   }
   else
   {
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pSym);
      __FALSE();
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dplace le pointeur d'enreg. de Nb enreg.
// Syntaxe : Nb DBSKIP
//
void _DbSkip(void)
{
   mtPTRDICO pSym,pLi,pCo;
   float pRec;
   mtDWORD NbRec;
   static char cProcName[] = "DBSKIP";

   pCurrCV->pNProc = cProcName;
   pRec  = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   NbRec = pRec;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4skip(pCurrCV->CurrBase,NbRec);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Slectionne l'index donn.
// Syntaxe : NomIndex DBSETINDEX
//
void _DbSetIndex(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE NomIndex;
   TAG4 *Idx;
   static char cProcName[] = "DBSETINDEX";

   pCurrCV->pNProc = cProcName;
   NomIndex = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomIndex,"DbSetIndex",pSym);
   if(pCurrCV->CurrBase)
   {
      if((strlen(NomIndex) < 1) || (strspn(NomIndex," ") == strlen(NomIndex)))
      {
         // Nom d'index vide, slection de l'ordre naturel (n๘ enreg.)
         d4tag_select(pCurrCV->CurrBase,NULL);
      }
      else
      {
         Idx = d4tag(pCurrCV->CurrBase,NomIndex);
         if(Idx)
            d4tag_select(pCurrCV->CurrBase,Idx);
         else
            ExecError(pCurrCV->pNProc,ERRSETINDEXFAIL,pSym);
      }
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Se place au dbut de la base courante.
// Syntaxe : DBTOP
void _DbTop(void)
{
   static char cProcName[] = "DBTOP";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4top(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dverouille la base courante.
// Syntaxe : DBUNLOCK
//
void _DbUnLock(void)
{
   static char cProcName[] = "DBUNLOCK";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->CurrBase)
      pCurrCV->DbError = d4unlock(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dverouille toutes les bases du cv courant.
// Syntaxe : DBUNLOCKALL
//
void _DbUnLockAll(void)
{
   static char cProcName[] = "DBUNLOCKALL";

   pCurrCV->pNProc  = cProcName;
   pCurrCV->DbError = d4unlock_files(&pCurrCV->CBase);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Stocke la valeur donne dans le champ donn de la base courante.
// Syntaxe : Valeur NomChamp DBREPLACE
//
void _DbReplace(void)
{
   mtPTRDICO pSrc,pCha,pLi,pCo;
   mtPTRBYTE NomChamp,pc1;
   mtPTRDWORD plNum;
   mtDWORD lNum;
   float pdNum;
   float dNum;
   FIELD4 *Champ;
   int ChTyp;
   static char cProcName[] = "DBREPLACE";

   pCurrCV->pNProc = cProcName;
   NomChamp = RecupString(&pCha,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomChamp,"DbReplace",pCha);
   RecupSym(&pSrc,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(pCurrCV->CurrBase)
   {
      Champ = d4field(pCurrCV->CurrBase,NomChamp);
      if(Champ)
      {
         ChTyp = f4type(Champ);
         switch(ChTyp)
         {
            case r4str:
            {
               CheckString(pSrc,pCurrCV->pNProc);
               if(pSrc->Type == mtTABCHAR)
                  pc1 = ArrayCVal(pSrc,pLi,pCo,pCurrCV->pNProc);
               else
                  pc1 = pSrc->V.cVal;
               // CheckNull(pc1,"DbReplace",pSrc);
               f4assign(Champ,pc1);
               break;
            }
            case r4date:
            {
               switch(pSrc->Type)
               {
                  case mtTCHAR:
                  {
                     pc1 = pSrc->V.cVal;
                     // CheckNull(pc1,"DbReplace",pSrc);
                     // forme "AAAAMMJJ".
                     if(date4long(pc1) >= (mtDWORD) 0)
                        f4assign(Champ,pc1);
                     else
                        ExecError(pCurrCV->pNProc,ERRDBDATEFAIL,pSrc);
                     break;
                  }
                  case mtTABCHAR:
                  {
                     pc1 = ArrayCVal(pSrc,pLi,pCo,pCurrCV->pNProc);
                     // CheckNull(pc1,"DbReplace",pSrc);
                     // forme "AAAAMMJJ".
                     if(date4long(pc1) >= (mtDWORD) 0)
                        f4assign(Champ,pc1);
                     else
                        ExecError(pCurrCV->pNProc,ERRDBDATEFAIL,pSrc);
                     break;
                  }
                  case mtTNUMBER:
                  {
                     // Date en format numrique (jour julian).
                     pdNum = pSrc->V.fVal;
                     lNum  = pdNum;
                     f4assign_long(Champ,lNum);
                     break;
                  }
                  case mtTABNUMBER:
                  {
                     pdNum = ArrayNVal(pSrc,pLi,pCo,pCurrCV->pNProc);
                     lNum  = pdNum;
                     f4assign_long(Champ,lNum);
                     break;
                  }
               }
               break;
            }
            case r4log:
            {
               switch(pSrc->Type)
               {
                  case mtTCHAR:
                  {
                     pc1 = pSrc->V.cVal;
                     // CheckNull(pc1,"DbReplace",pSrc);
                     // forme "Y","y","N","n","T","t","f","F".
                     f4assign(Champ,pc1);
                     break;
                  }
                  case mtTABCHAR:
                  {
                     pc1 = ArrayCVal(pSrc,pLi,pCo,pCurrCV->pNProc);
                     // CheckNull(pc1,"DbReplace",pSrc);
                     // forme "Y","y","N","n","T","t","f","F".
                     f4assign(Champ,pc1);
                     break;
                  }
                  case mtTNUMBER:
                  {
                     // forme 0, 1.
                     pdNum = pSrc->V.fVal;
                     dNum  = pdNum;
                     if(dNum)
                        f4assign(Champ,"T");
                     else
                        f4assign(Champ,"F");
                     break;
                  }
                  case mtTABNUMBER:
                  {
                     pdNum = ArrayNVal(pSrc,pLi,pCo,pCurrCV->pNProc);
                     dNum  = pdNum;
                     if(dNum)
                        f4assign(Champ,"T");
                     else
                        f4assign(Champ,"F");
                     break;
                  }
               }
               break;
            }
            case r4memo:
            {
               CheckString(pSrc,pCurrCV->pNProc);
               if(pSrc->Type == mtTABCHAR)
                  pc1 = ArrayCVal(pSrc,pLi,pCo,"DbReplace");
               else
                  pc1 = pSrc->V.cVal;
               // CheckNull(pc1,"DbReplace",pSrc);
               pCurrCV->DbError = f4memo_assign(Champ,pc1);
               break;
            }
            case r4num:
            {
               CheckNumber(pSrc,pCurrCV->pNProc);
               if(pSrc->Type == mtTABNUMBER)
                  pdNum = ArrayNVal(pSrc,pLi,pCo,pCurrCV->pNProc);
               else
                  pdNum = pSrc->V.fVal;
               // CheckNull(pdNum,"DbReplace",pSrc);
               dNum  = pdNum;
               f4assign_double(Champ,(double) dNum);
               break;
            }
         }
      }
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pCha);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pCha);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le contenu du champ donn.
// Syntaxe : NomChamp DBFIELD
//
void _DbField(void)
{
   mtPTRDICO pSym,pCha,pLi,pCo;
   mtPTRBYTE NomChamp,cVal;
   FIELD4 *Champ;
   int ChTyp;
   static char cProcName[] = "DBFIELD";

   pCurrCV->pNProc = cProcName;
   NomChamp = RecupString(&pCha,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomChamp,"DbField",pCha);
   pSym = GenCvICHAR(1);
   if(pCurrCV->CurrBase)
   {
      Champ = d4field(pCurrCV->CurrBase,NomChamp);
      if(Champ)
      {
         ChTyp = f4type(Champ);
         switch(ChTyp)
         {
            case r4str:
            {
               cVal = f4str(Champ);
               if(cVal)
               {
                  pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(cVal)+1);
                  memset(pSym->V.cVal,0,strlen(cVal)+1);
                  strcpy(pSym->V.cVal,cVal);
               }
               break;
            }
            case r4date:
            {
               cVal = f4str(Champ);
               if(cVal)
               {
                  pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(cVal)+1);
                  memset(pSym->V.cVal,0,strlen(cVal)+1);
                  strcpy(pSym->V.cVal,cVal);
               }
               break;
            }
            case r4log:
            {
               MemFree(pSym->V.cVal);
               pSym->Type = mtTNUMBER;
               if(f4true(Champ))
                  pSym->V.fVal = (float) 1;
               else
                  pSym->V.fVal = (float) 0;
               break;
            }
            case r4memo:
            {
               cVal = f4memo_str(Champ);
               if(cVal)
               {
                  pSym->V.cVal = (mtPTRBYTE) MemReAlloc(pSym->V.cVal,strlen(cVal)+1);
                  memset(pSym->V.cVal,0,strlen(cVal)+1);
                  strcpy(pSym->V.cVal,cVal);
               }
               break;
            }
            case r4num:
            {
               MemFree(pSym->V.cVal);
               pSym->Type = mtTNUMBER;
               pSym->V.fVal = (float) f4double(Champ);
               break;
            }
         }
      }
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pCha);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pCha);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le type du champ donn.
//
void _DbFieldType(void)
{
   mtPTRDICO pSym,pCha,pLi,pCo;
   mtPTRBYTE NomChamp;
   FIELD4 *Champ;
   int ChTyp;
   static char cProcName[] = "DBFIELDTYPE";

   pCurrCV->pNProc = cProcName;
   NomChamp = RecupString(&pCha,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomChamp,"DbFieldType",pCha);
   pSym = GenCvICHAR(2);
   if(pCurrCV->CurrBase)
   {
      Champ = d4field(pCurrCV->CurrBase,NomChamp);
      if(Champ)
      {
         ChTyp = f4type(Champ);
         switch(ChTyp)
         {
            case r4str:
            {
               *(pSym->V.cVal) = 'C';
               break;
            }
            case r4date:
            {
               *(pSym->V.cVal) = 'D';
               break;
            }
            case r4log:
            {
               *(pSym->V.cVal) = 'L';
               break;
            }
            case r4memo:
            {
               *(pSym->V.cVal) = 'M';
               break;
            }
            case r4num:
            {
               *(pSym->V.cVal) = 'N';
               break;
            }
         }
      }
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pCha);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pCha);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la longueur du champ donn.
//
void _DbFieldLen(void)
{
   mtPTRDICO pSym,pCha,pLi,pCo;
   mtPTRBYTE NomChamp;
   FIELD4 *Champ;
   static char cProcName[] = "DBFIELDLEN";

   pCurrCV->pNProc = cProcName;
   NomChamp = RecupString(&pCha,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomChamp,"DbFieldLen",pCha);
   pSym = GenCvINUMBER();
   if(pCurrCV->CurrBase)
   {
      Champ = d4field(pCurrCV->CurrBase,NomChamp);
      if(Champ)
         pSym->V.fVal = (float) f4len(Champ);
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pCha);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pCha);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre de champs de la base courante.
//
void _DbFieldCount(void)
{
   mtPTRDICO pSym;
   static char cProcName[] = "DBFIELDCOUNT";

   pCurrCV->pNProc = cProcName;
   pSym = GenCvINUMBER();
   if(pCurrCV->CurrBase)
      pSym->V.fVal = (float) d4num_fields(pCurrCV->CurrBase);
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,NULL);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nombre de dcimales du champ donn.
//
void _DbFieldLenDec(void)
{
   mtPTRDICO pSym,pCha,pLi,pCo;
   mtPTRBYTE NomChamp;
   FIELD4 *Champ;
   static char cProcName[] = "DBFIELDLENDEC";

   pCurrCV->pNProc = cProcName;
   NomChamp = RecupString(&pCha,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
//   CheckNull(NomChamp,"DbFieldLenDec",pCha);
   pSym = GenCvINUMBER();
   if(pCurrCV->CurrBase)
   {
      Champ = d4field(pCurrCV->CurrBase,NomChamp);
      if(Champ)
         pSym->V.fVal = (float) f4decimals(Champ);
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pCha);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pCha);
   _DPush(pSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom du champ correspondant au numro donn.
//
void _DbFieldNumToName(void)
{
   mtPTRDICO pNum,pLi,pCo,pCha;
   float nch;
   mtPTRBYTE NomChamp;
   int numch;
   FIELD4 *Champ;
   static char cProcName[] = "DBNUMFIELD->NAME";

   pCurrCV->pNProc = cProcName;
   nch   = RecupNumber(&pNum,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   numch = nch;
   pCha  = GenCvICHAR(1);
   if(pCurrCV->CurrBase)
   {
      if((numch>=1) && (numch<=d4num_fields(pCurrCV->CurrBase)))
      {
         Champ = d4field_j(pCurrCV->CurrBase,numch);
         if(Champ)
         {
            NomChamp   = f4name(Champ);
            if(NomChamp)
            {
               pCha->V.cVal = (mtPTRBYTE) MemReAlloc(pCha->V.cVal,strlen(NomChamp)+1);
               memset(pCha->V.cVal,0,strlen(NomChamp)+1);
               strcpy(pCha->V.cVal,NomChamp);
            }
            else
               ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pNum);
         }
         else
            ExecError(pCurrCV->pNProc,ERRDBNOFIELD,pNum);
      }
      else
         ExecError(pCurrCV->pNProc,ERRDBNOFIELDNUM,pNum);
   }
   else
      ExecError(pCurrCV->pNProc,ERRDBNOCURRBASE,pNum);
   _DPush(pCha);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                           TRAITEMENT DES ERREURS
//                           ออออออออออออออออออออออ
//
// Empile le code d'erreur relatif aux manipulations des bases de donnes.
//
void _DbError(void)
{
   mtPTRDICO pDSym;
   static char cProcName[] = "DBERROR";

   pCurrCV->pNProc = cProcName;
   pDSym = GenCvINUMBER();
   pDSym->V.fVal = (float) pCurrCV->DbError;
   _DPush(pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile VRAI si une erreur d'excution s'est produite.
//
void _IntError(void)
{
   static char cProcName[] = "ERROR?";

   pCurrCV->pNProc = cProcName;
   if(pCurrCV->ExecError)
      __TRUE();
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le code de l'erreur d'excution.
//
void _ErrorCode(void)
{
   mtPTRDICO pDSym;
   static char cProcName[] = "ERRORCODE";

   pCurrCV->pNProc = cProcName;
   pDSym = GenCvINUMBER();
   pDSym->V.fVal = (float) pCurrCV->CodeError;
   _DPush(pDSym);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                        MANIPULATION DES TABLEAUX
//                        อออออออออออออออออออออออออ
//
// Renvoie la taille (NbLig, NbCol) du tableau donn
//
void _ALen(void)
{
   mtPTRDICO pTab,pLi,pCo;
   mtPTRITAB pITmp;
   static char cProcName[] = "ALEN";

   pCurrCV->pNProc = cProcName;
   pTab  = (mtPTRDICO) _DPop();
   if((pTab->Type != mtTABCHAR) && (pTab->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTab);
   pITmp = (mtPTRITAB) pTab->XVal;
   pLi   = GenCvINUMBER();
   pLi->V.fVal = (float) pITmp->MaxLig;
   pCo   = GenCvINUMBER();
   pCo->V.fVal = (float) pITmp->MaxCol;
   _DPush(pLi);
   _DPush(pCo);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la procdure AFILL.
//
void _InitAfill(void)
{
   mtPTRDICO pTab,pLi,pCo,pSym;
   mtPTRBYTE pCh;
   float pDb;
   static char cProcName[] = "AFILL";

   pCurrCV->pNProc = cProcName;
   pTab = (mtPTRDICO) _DPop();
   if((pTab->Type != mtTABCHAR) && (pTab->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTab);
   switch(pTab->Type)
   {
      case mtTABCHAR:
      {
         pCh = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
         // CheckNull(pCh,"AFill",pSym);
         pCurrCV->AValDst = (mtPOINTEUR) pCh;
         break;
      }
      case mtTABNUMBER:
      {
         pDb = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
         pCurrCV->AfValDst = pDb;
         break;
      }
   }
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
   pCurrCV->ATabDst = pTab;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Remplissage d'un tableau avec une valeur.
//
void _AFill(void)
{
   mtPTRITAB pITmp;
   mtPTRBYTE pCh;
   float pDb;
   pPOINTEUR pTab;
   mtPTRFLOAT pfTab;
   static char cProcName[] = "AFILL";

   pCurrCV->pNProc = cProcName;
   pITmp = (mtPTRITAB) pCurrCV->ATabDst->XVal;
   if((pCurrCV->ALigDst<pITmp->MaxLig) && (pCurrCV->AColDst<pITmp->MaxCol))
   {
      switch(pCurrCV->ATabDst->Type)
      {
         case mtTABCHAR:
         {
            pTab = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
            pCh  = (mtPTRBYTE) *(pTab+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst));
            //if(pCh)
               pCh = (mtPTRBYTE) MemReAlloc(pCh,strlen((mtPTRBYTE) pCurrCV->AValDst)+1);
            //else
            //   pCh = (mtPTRBYTE) MemAlloc(strlen((mtPTRBYTE) pCurrCV->AValDst)+1);
            strcpy(pCh,(mtPTRBYTE) pCurrCV->AValDst);
            *(pTab+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst)) = (mtPOINTEUR) pCh;
            break;
         }
         case mtTABNUMBER:
         {
            pfTab = (mtPTRFLOAT) pCurrCV->ATabDst->V.tfVal;
            pDb   = pCurrCV->AfValDst;
            *(pfTab+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst)) = pDb;
            break;
         }
      }
      pCurrCV->AColDst++;
      if(pCurrCV->AColDst>=pITmp->MaxCol)
      {
         pCurrCV->ALigDst++;
         pCurrCV->AColDst = 0;
      }
      // On boucle jusqu'… la fin du tableau.
      pCurrCV->IP--;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la procdure ASEARCH.
//
void _InitAsearch(void)
{
   mtPTRDICO pTab,pLi,pCo,pSym;
   mtPTRBYTE pCh;
   float pDb;
   static char cProcName[] = "ASEARCH";

   pCurrCV->pNProc = cProcName;
   pTab = (mtPTRDICO) _DPop();
   if((pTab->Type != mtTABCHAR) && (pTab->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTab);
   switch(pTab->Type)
   {
      case mtTABCHAR:
      {
         pCh = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
         // CheckNull(pCh,"ASearch",pSym);
         pCurrCV->AValDst = (mtPOINTEUR) pCh;
         break;
      }
      case mtTABNUMBER:
      {
         pDb = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
         pCurrCV->AfValDst = pDb;
         break;
      }
   }
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
   pCurrCV->ATabDst = pTab;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une valeur dans un tableau.
//
void _ASearch(void)
{
   mtPTRITAB pITmp;
   mtPOINTEUR pTVal;
   float pfTVal;
   mtPTRDICO pLig,pCol;
   mtBOOL bOk=mtFALSE;
   pPOINTEUR pTab;
   mtPTRFLOAT pfTab;
   static char cProcName[] = "ASEARCH";

   pCurrCV->pNProc = cProcName;
   pITmp = (mtPTRITAB) pCurrCV->ATabDst->XVal;
   if((pCurrCV->ALigDst<pITmp->MaxLig) && (pCurrCV->AColDst<pITmp->MaxCol))
   {
      switch(pCurrCV->ATabDst->Type)
      {
         case mtTABCHAR:
         {
            pTab  = pCurrCV->ATabDst->V.tVal;
            pTVal = *(pTab+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst));
            if(pTVal)
            {
               if(strcmp((mtPTRBYTE) pCurrCV->AValDst,(mtPTRBYTE) pTVal) == 0)
               {
                  // Chaine trouve.
                  pLig = GenCvINUMBER();
                  pLig->V.fVal = pCurrCV->ALigDst;
                  pCol = GenCvINUMBER();
                  pCol->V.fVal = pCurrCV->AColDst;
                  _DPush(pLig);
                  _DPush(pCol);
                  __TRUE();
                  bOk = mtTRUE;
               }
            }
            break;
         }
         case mtTABNUMBER:
         {
            pfTab  = pCurrCV->ATabDst->V.tfVal;
            pfTVal = *(pfTab+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst));
            if(pCurrCV->AfValDst == pfTVal)
            {
               // Nombre trouv.
               pLig = GenCvINUMBER();
               pLig->V.fVal = pCurrCV->ALigDst;
               pCol = GenCvINUMBER();
               pCol->V.fVal = pCurrCV->AColDst;
               _DPush(pLig);
               _DPush(pCol);
               __TRUE();
               bOk = mtTRUE;
            }
            break;
         }
      }
      if(! bOk)
      {
         pCurrCV->AColDst++;
         if(pCurrCV->AColDst>=pITmp->MaxCol)
         {
            pCurrCV->ALigDst++;
            pCurrCV->AColDst = 0;
         }
         if(pCurrCV->ALigDst>=pITmp->MaxLig)
         {
            // On a balay tout le tableau, on a rien trouv.
            pLig = GenCvINUMBER();
            pLig->V.fVal = (float) -1;
            pCol = GenCvINUMBER();
            pCol->V.fVal = (float) -1;
            _DPush(pLig);
            _DPush(pCol);
            __FALSE();
         }
         else
            // On boucle jusqu'… la fin du tableau.
            pCurrCV->IP--;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une ligne … la fin du tableau donn.
//
void _AIns(void)
{
   mtPTRDICO pTab;
   mtPTRITAB pITmp;
   static char cProcName[] = "AINS";

   pCurrCV->pNProc = cProcName;
   pTab = (mtPTRDICO) _DPop();
   if((pTab->Type != mtTABCHAR) && (pTab->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTab);
   pITmp = (mtPTRITAB) pTab->XVal;
   if(pTab->Type == mtTABCHAR)
      pCurrCV->OldTab = pTab->V.tVal;
   else
      pCurrCV->OldTab = (pPOINTEUR) pTab->V.tfVal;
   pCurrCV->MaxLi = pITmp->MaxLig;
   pITmp->MaxLig++;
   pTab->XVal = (mtPOINTEUR) pITmp;
   if(pTab->Type == mtTABCHAR)
      pTab->V.tVal = (pPOINTEUR) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(mtPOINTEUR));
   else
      pTab->V.tfVal = (mtPTRFLOAT) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(float));
   pCurrCV->ATabDst = pTab;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Enlve la dernire ligne du tableau donn.
//
void _ADel(void)
{
   mtPTRDICO pTab;
   mtPTRITAB pITmp;
   mtUWORD Co;
   mtPOINTEUR ptr;
   pPOINTEUR OldTab;
   static char cProcName[] = "ADEL";

   pCurrCV->pNProc = cProcName;
   pTab = (mtPTRDICO) _DPop();
   if((pTab->Type != mtTABCHAR) && (pTab->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTab);
   pITmp = (mtPTRITAB) pTab->XVal;
   if(pTab->Type == mtTABCHAR)
   {
      OldTab          = pTab->V.tVal;
      pCurrCV->OldTab = pTab->V.tVal;
   }
   else
   {
      OldTab          = (pPOINTEUR) pTab->V.tfVal;
      pCurrCV->OldTab = (pPOINTEUR) pTab->V.tfVal;
   }
   pITmp->MaxLig--;
   pCurrCV->MaxLi = pITmp->MaxLig;
   pTab->XVal     = (mtPOINTEUR) pITmp;
   if(pTab->Type == mtTABCHAR)
      pTab->V.tVal = (pPOINTEUR) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(mtPOINTEUR));
   else
      pTab->V.tfVal = (mtPTRFLOAT) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(float));
   if(pTab->Type == mtTABCHAR)
   {
      // On supprime les lments de la ligne conerne.
      for(Co=0;Co<pITmp->MaxCol;Co++)
      {
         ptr = *(OldTab+((pCurrCV->MaxLi*pITmp->MaxCol)+Co));
         MemFree(ptr);
      }
   }
   pCurrCV->ATabDst = pTab;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Transfert le tableau OldTab dans ATabDst.
// Utilise par AINS, ADEL.
//
void _TransTab(void)
{
   mtPTRITAB pITmp;
   pPOINTEUR pSrc,pDst;
   mtPOINTEUR ptr;
   mtPTRFLOAT pfSrc,pfDst;
   float flt;

   pITmp = (mtPTRITAB) pCurrCV->ATabDst->XVal;
   if((pCurrCV->ALigDst<pCurrCV->MaxLi) && (pCurrCV->AColDst<pITmp->MaxCol))
   {
      if(pCurrCV->ATabDst->Type == mtTABCHAR)
      {
         pSrc = (pPOINTEUR) pCurrCV->OldTab;
         pDst = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
         ptr  = *(pSrc+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst));
         *(pDst+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst)) = ptr;
      }
      else
      {
         pfSrc = (mtPTRFLOAT) pCurrCV->OldTab;
         pfDst = (mtPTRFLOAT) pCurrCV->ATabDst->V.tfVal;
         flt   = *(pfSrc+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst));
         *(pfDst+((pCurrCV->ALigDst*pITmp->MaxCol)+pCurrCV->AColDst)) = flt;
      }
      pCurrCV->AColDst++;
      if(pCurrCV->AColDst>=pITmp->MaxCol)
      {
         pCurrCV->ALigDst++;
         pCurrCV->AColDst = 0;
      }
      // On boucle jusqu'… la fin du tableau.
      pCurrCV->IP--;
   }
   else
      // On a fini le transfert.
      MemFree(pCurrCV->OldTab);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Copie le tableau ASrc dans le tableau ADst.
//
void _ACopy(void)
{
   mtPTRDICO pTSrc,pTDst;
   static char cProcName[] = "ACOPY";

   pCurrCV->pNProc = cProcName;
   pTDst = (mtPTRDICO) _DPop();
   if((pTDst->Type != mtTABCHAR) && (pTDst->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTDst);
   pTSrc = (mtPTRDICO) _DPop();
   if((pTSrc->Type != mtTABCHAR) && (pTSrc->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTSrc);
   if(pTDst->Type != pTSrc->Type)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   // Prpare la copie.
   pCurrCV->ATabSrc = pTSrc;
   pCurrCV->ALigSrc = 0;
   pCurrCV->AColSrc = 0;
   pCurrCV->ATabDst = pTDst;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Transfert les lments du tableau ATabSrc vers ATabDst.
// Utilise par ACOPY/ACLONE.
//
void _CopyTab(void)
{
   mtPTRITAB pISrc,pIDst;
   pPOINTEUR pSrc,pDst;
   mtPTRFLOAT pfSrc,pfDst;
   mtPTRBYTE pcSrc,pcDst;
   float pdSrc,pdDst;

   pISrc = (mtPTRITAB) pCurrCV->ATabSrc->XVal;
   pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
   if((pCurrCV->ALigDst<pIDst->MaxLig) && (pCurrCV->AColDst<pIDst->MaxCol))
   {
      // On n'est pas … la fin du tableau destination.
      if((pCurrCV->ALigSrc<pISrc->MaxLig) && (pCurrCV->AColSrc<pISrc->MaxCol))
      {
         // On n'est pas … la fin du tableau source.
         if((pCurrCV->ALigDst<pISrc->MaxLig) && (pCurrCV->AColDst<pISrc->MaxCol))
         {
            // On peut encore copier.
            switch(pCurrCV->ATabDst->Type)
            {
               case mtTABCHAR:
               {
                  pSrc  = (pPOINTEUR) pCurrCV->ATabSrc->V.tVal;
                  pDst  = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
                  pcSrc = (mtPTRBYTE) *(pSrc+((pCurrCV->ALigSrc*pISrc->MaxCol)+pCurrCV->AColSrc));
                  pcDst = (mtPTRBYTE) *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+pCurrCV->AColDst));
                  if(pcSrc)
                  {
                     pcDst = (mtPTRBYTE) MemReAlloc(pcDst,strlen(pcSrc)+1);
                     strcpy(pcDst,pcSrc);
                  }
                  else
                  {
                     // Valeur source non initialise.
                     MemFree(pcDst);
                     pcDst = NULL;
                  }
                  *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+pCurrCV->AColDst)) = (mtPOINTEUR) pcDst;
                  break;
               }
               case mtTABNUMBER:
               {
                  pfSrc = (mtPTRFLOAT) pCurrCV->ATabSrc->V.tfVal;
                  pfDst = (mtPTRFLOAT) pCurrCV->ATabDst->V.tfVal;
                  pdSrc = (float) *(pfSrc+((pCurrCV->ALigSrc*pISrc->MaxCol)+pCurrCV->AColSrc));
                  *(pfDst+((pCurrCV->ALigDst*pIDst->MaxCol)+pCurrCV->AColDst)) = pdSrc;
                  break;
               }
            }
            pCurrCV->AColSrc++;
            if(pCurrCV->AColSrc>=pISrc->MaxCol)
            {
               pCurrCV->ALigSrc++;
               pCurrCV->AColSrc = 0;
            }
            pCurrCV->AColDst++;
            if(pCurrCV->AColDst>=pIDst->MaxCol)
            {
               pCurrCV->ALigDst++;
               pCurrCV->AColDst = 0;
            }
            // On boucle jusqu'… la fin du tableau.
            pCurrCV->IP--;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Effectue un clone du tableau source dans le tableau destination.
//
void _AClone(void)
{
   mtPTRDICO pTSrc,pTDst;
   mtPTRITAB pISrc,pIDst;
   static char cProcName[] = "ACLONE";

   pCurrCV->pNProc = cProcName;
   pTDst = (mtPTRDICO) _DPop();
   if((pTDst->Type != mtTABCHAR) && (pTDst->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTDst);
   pTSrc = (mtPTRDICO) _DPop();
   if((pTSrc->Type != mtTABCHAR) && (pTSrc->Type != mtTABNUMBER))
      ExecError(pCurrCV->pNProc,ERRTABEXPECT,pTSrc);
   if(pTDst->Type != pTSrc->Type)
      ExecError(pCurrCV->pNProc,ERRTYPINCOMP,NULL);
   // Prpare la suppression de l'ancien tableau.
   pIDst           = (mtPTRITAB) pTDst->XVal;
   pCurrCV->OTType = pTDst->Type;
   if(pTDst->Type == mtTABCHAR)
      pCurrCV->OldTab = pTDst->V.tVal;
   else
      pCurrCV->OldTab = (pPOINTEUR) pTDst->V.tfVal;
   pCurrCV->ALigDel = 0;
   pCurrCV->AColDel = 0;
   pCurrCV->MaxLi   = pIDst->MaxLig;
   pCurrCV->MaxCo   = pIDst->MaxCol;
   // Prpare la copie.
   pISrc            = (mtPTRITAB) pTSrc->XVal;
   if(pTDst->Type == mtTABCHAR)
      pTDst->V.tVal = (pPOINTEUR) MemAlloc((pISrc->MaxLig*pISrc->MaxCol)*sizeof(mtPOINTEUR));
   else
      pTDst->V.tfVal = (mtPTRFLOAT) MemAlloc((pISrc->MaxLig*pISrc->MaxCol)*sizeof(float));
   pIDst->MaxLig    = pISrc->MaxLig;
   pIDst->MaxCol    = pISrc->MaxCol;
   pTDst->XVal      = (mtPOINTEUR) pIDst;
   pCurrCV->ATabSrc = pTSrc;
   pCurrCV->ALigSrc = 0;
   pCurrCV->AColSrc = 0;
   pCurrCV->ATabDst = pTDst;
   pCurrCV->ALigDst = 0;
   pCurrCV->AColDst = 0;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Supprime les lments du tableau OldTab, ainsi que le tableau lui-mme.
// Utilise par ACLONE.
//
void _DeleteTab(void)
{
   pPOINTEUR pDst;
   mtPOINTEUR ptr;

   if(pCurrCV->OTType == mtTABNUMBER)
      // On a fini la suppression
      MemFree(pCurrCV->OldTab);
   else
   {
      if((pCurrCV->ALigDel<pCurrCV->MaxLi) && (pCurrCV->AColDel<pCurrCV->MaxCo))
      {
         // On n'est pas … la fin du tableau.
         pDst = (pPOINTEUR) pCurrCV->OldTab;
         ptr  = *(pDst+((pCurrCV->ALigDel*pCurrCV->MaxCo)+pCurrCV->AColDel));
         MemFree(ptr);
         pCurrCV->AColDel++;
         if(pCurrCV->AColDel>=pCurrCV->MaxCo)
         {
            pCurrCV->ALigDel++;
            pCurrCV->AColDel = 0;
         }
         // On boucle jusqu'… la fin du tableau.
         pCurrCV->IP--;
      }
      else
         // On a fini la suppression
         MemFree(pCurrCV->OldTab);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dcoupage d'une chaine par dlimiteurs et stockage dans ATabDst.
//
void _DecoupeChaine(void)
{
   mtPTRITAB pIDst;
   pPOINTEUR pDst;
   mtPTRBYTE pCh,pcDst;
   int cDelim;

   cDelim = *(pCurrCV->pDelim);
   // On limine d'abord les ventuels dlimiteurs au dbut de la chaine.
   pCh    = pCurrCV->pChaine;
   while(*pCh == cDelim)
      pCh++;
   if(pCh>pCurrCV->pChaine)
   {
      // On en a trouv, on les limine.
      memmove(pCurrCV->pChaine,pCh,(strlen(pCurrCV->pChaine)-((pCh-pCurrCV->pChaine)+1))+2);
   }
   pCh = strchr(pCurrCV->pChaine,cDelim);
   if(pCh)
   {
      pcDst = (mtPTRBYTE) MemAlloc((pCh-pCurrCV->pChaine)+1);
      strncpy(pcDst,pCurrCV->pChaine,(pCh-pCurrCV->pChaine));
      // On limine l'item de la chaine.
      memmove(pCurrCV->pChaine,pCh+1,(strlen(pCurrCV->pChaine)-((pCh-pCurrCV->pChaine)+1))+1);
      pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
      pDst  = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
      if(pCurrCV->AColDst)
      {
         // Une colonne de plus.
         pIDst->MaxCol++;
         pDst = (pPOINTEUR) MemReAlloc(pDst,(pIDst->MaxLig*pIDst->MaxCol)*sizeof(mtPOINTEUR));
      }
      *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+pCurrCV->AColDst)) = (mtPOINTEUR) pcDst;
      pCurrCV->AColDst++;
      pCurrCV->ATabDst->XVal   = (mtPOINTEUR) pIDst;
      pCurrCV->ATabDst->V.tVal = (mtPOINTEUR) pDst;
      // On boucle jusqu'… la fin de la chaine.
      pCurrCV->IP--;
   }
   else
   {
      // Dcoupage fini ?.
      if(strlen(pCurrCV->pChaine))
      {
         // Il reste quelque chose dans la chaine.
         pcDst = (mtPTRBYTE) MemAlloc(strlen(pCurrCV->pChaine)+1);
         strcpy(pcDst,pCurrCV->pChaine);
         pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
         pDst  = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
         if(pCurrCV->AColDst)
         {
            // Une colonne de plus.
            pIDst->MaxCol++;
            pDst = (pPOINTEUR) MemReAlloc(pDst,(pIDst->MaxLig*pIDst->MaxCol)*sizeof(mtPOINTEUR));
         }
         *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+pCurrCV->AColDst)) = (mtPOINTEUR) pcDst;
         pCurrCV->ATabDst->XVal   = (mtPOINTEUR) pIDst;
         pCurrCV->ATabDst->V.tVal = (mtPOINTEUR) pDst;
      }
      MemFree(pCurrCV->pChaine);
      MemFree(pCurrCV->pDelim);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Compte les fichiers correspondants au masque donn.
// (utilise par ADIR)
//
void __CountADirFiles(void)
{
   mtPTRITAB pIDst;
   pPOINTEUR pDst;

   if(pCurrCV->uFind == 0)
   {
      // Une ligne de plus.
      pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
      pIDst->MaxLig++;
      pCurrCV->ATabDst->XVal = (mtPOINTEUR) pIDst;
      // On boucle jusqu'… la fin de la recherche.
      pCurrCV->uFind = _dos_findnext(&pCurrCV->sFIND);
      pCurrCV->IP--;
   }
   else
   {
      // Fini, on dimensionne le tableau.
      pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
      if(pIDst->MaxLig == 0)
         // Aucun fichier trouv, on met au moins une ligne.
         pIDst->MaxLig = 1;
      pDst  = (pPOINTEUR) MemAlloc((pIDst->MaxLig*pIDst->MaxCol)*sizeof(mtPOINTEUR));
      pCurrCV->ATabDst->V.tVal = (mtPOINTEUR) pDst;
      // On relance la recherche pour ADirTab.
      pCurrCV->uFind = _dos_findfirst(pCurrCV->pMask,_A_NORMAL,&pCurrCV->sFIND);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Rcupration des fichiers du masque donn et stockage dans ATabDst.
//
void _ADirTab(void)
{
   mtPTRITAB pIDst;
   pPOINTEUR pDst;
   mtPTRBYTE pCh;
   mtBYTE OBuff[32];
   unsigned short i;

   if(pCurrCV->uFind == 0)
   {
      // Stockage des infos du fichier trouv.
      pIDst = (mtPTRITAB) pCurrCV->ATabDst->XVal;
      pDst  = (pPOINTEUR) pCurrCV->ATabDst->V.tVal;
      // Stockage du nom.
      pCh   = (mtPTRBYTE) MemAlloc(strlen(pCurrCV->sFIND.name)+1);
      strcpy(pCh,pCurrCV->sFIND.name);
      *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+0)) = (mtPOINTEUR) pCh;
      // Stockage de la taille.
      sprintf(OBuff,"%lu",pCurrCV->sFIND.size);
      pCh   = (mtPTRBYTE) MemAlloc(strlen(OBuff)+1);
      strcpy(pCh,OBuff);
      *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+1)) = (mtPOINTEUR) pCh;
      // Stockage de la date.
      i = pCurrCV->sFIND.wr_date;
      sprintf(OBuff,"%02d/%02d/%04d",(i&31),((i>>5)&15),((i>>9)&127)+1980);
      pCh   = (mtPTRBYTE) MemAlloc(strlen(OBuff)+1);
      strcpy(pCh,OBuff);
      *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+2)) = (mtPOINTEUR) pCh;
      // Stockage de l'heure.
      i = pCurrCV->sFIND.wr_time;
      sprintf(OBuff,"%02d:%02d:%02d",((i>>11)&31),((i>>5)&63),(i&31));
      pCh   = (mtPTRBYTE) MemAlloc(strlen(OBuff)+1);
      strcpy(pCh,OBuff);
      *(pDst+((pCurrCV->ALigDst*pIDst->MaxCol)+3)) = (mtPOINTEUR) pCh;
      // Une ligne de plus.
      pCurrCV->ALigDst++;
      pCurrCV->ATabDst->XVal   = (mtPOINTEUR) pIDst;
      pCurrCV->ATabDst->V.tVal = (mtPOINTEUR) pDst;
      // On boucle jusqu'… la fin de la recherche.
      pCurrCV->uFind = _dos_findnext(&pCurrCV->sFIND);
      // On vrifie qu'on est toujours dans les limites du tableau.
      if(pCurrCV->ALigDst<pIDst->MaxLig)
         pCurrCV->IP--;
      else
         // Plus de place.
         MemFree(pCurrCV->pMask);
   }
   else
   {
      // Fini.
      MemFree(pCurrCV->pMask);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                        MANIPULATION DES TACHES
//                        อออออออออออออออออออออออ
//
// Initialise une tche locale au connect courant.
//
void _SetLTask(void)
{
   mtPTRDICO pProc;
   mtPTRCV pTaskCV;
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;
   static char cProcName[] = "SETLOCALTASK";

   pCurrCV->pNProc = cProcName;
   pProc   = (mtPTRDICO) _DPop();
   pTaskCV = &LTask[pCurrCV->NumCv];
   if(pTaskCV->Actif)
   {
      // Tche dj… active.
      // On vide les donnes locales … la tche (!).
      d4init_undo(&pTaskCV->CBase);
      MemFree(pTaskCV->PileD);
      MemFree(pTaskCV->PileR);
      MemFree(pTaskCV->pUsing);
      MemFree(pTaskCV->pLInput);
      if(pTaskCV->MLITabLine)
      {
         pDst = pTaskCV->MLITabLine;
         for(i=0;i<pTaskCV->MLIMaxLine;i++)
         {
            pLig = (mtPTRBYTE) *(pDst+i);
            MemFree(pLig);
         }
         MemFree(pDst);
      }
   }
   memset(pTaskCV,0,sizeof(UnCv));
   // Allocation des 1res pages des piles.
   pTaskCV->nPageDS = 1;
   pTaskCV->PileD   = (pPOINTEUR) MemAlloc((pTaskCV->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   pTaskCV->SP      = 0;
   pTaskCV->nPageRS = 1;
   pTaskCV->PileR   = (pPOINTEUR) MemAlloc((pTaskCV->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
   pTaskCV->RP      = 0;
   // Initialisation des donnes globales … la tche et au connect.
   pTaskCV->NumPort = pCurrCV->NumPort;
   pTaskCV->NumCv   = pCurrCV->NumCv;
   strcpy(pTaskCV->TrameCNX,pCurrCV->TrameCNX);
   pTaskCV->Dept    = pCurrCV->Dept;
   strcpy(pTaskCV->Pavi,pCurrCV->Pavi);
   pTaskCV->Palier  = pCurrCV->Palier;
   pTaskCV->NumComp = pCurrCV->NumComp;
   strcpy(pTaskCV->Service,pCurrCV->Service);
   _dos_getdate(&pTaskCV->DateCnx);
   _dos_gettime(&pTaskCV->HeureCnx);
   pTaskCV->TypeMinitel = pCurrCV->TypeMinitel;
   pTaskCV->pData       = pCurrCV->pData;
   pTaskCV->pOutBuff    = pCurrCV->pOutBuff;
   pTaskCV->TFonction   = pCurrCV->TFonction;
   pTaskCV->bEtoile     = pCurrCV->bEtoile;
   pTaskCV->bDiese      = pCurrCV->bDiese;
   pTaskCV->Attente     = pCurrCV->Attente;
   pTaskCV->pApp        = pCurrCV->pApp;
   pTaskCV->pLDico      = pCurrCV->pLDico;
   pTaskCV->uLDico      = pCurrCV->uLDico;
   pTaskCV->pIDico      = pCurrCV->pIDico;
   pTaskCV->ISP         = pCurrCV->ISP;
   pTaskCV->IGC         = pCurrCV->IGC;
   pTaskCV->ISP         = pCurrCV->ISP;
   pTaskCV->NbISym      = pCurrCV->NbISym;
   pTaskCV->nPageI      = pCurrCV->nPageI;
   pTaskCV->Row         = pCurrCV->Row;
   pTaskCV->Col         = pCurrCV->Col;
   pTaskCV->bGCursor    = pCurrCV->bGCursor;
   pTaskCV->pLTabFKey   = pCurrCV->pLTabFKey;
   pTaskCV->pLTabKeyW   = pCurrCV->pLTabKeyW;
   d4init(&pTaskCV->CBase);
   pTaskCV->CBase.exclusive     = 0;
   pTaskCV->CBase.read_only     = 0;
   pTaskCV->CBase.read_lock     = 0;
   pTaskCV->CBase.lock_attempts = 0;
   pTaskCV->CBase.open_error    = 0;
   pTaskCV->CBase.off_error     = 1;
   // Initialisation et activation de la tche.
   pTaskCV->pHere  = (ppFUNC) pProc->V.pVal;
   pTaskCV->IP     = (mtUWORD) pProc->XVal;
   pTaskCV->Actif  = mtTRUE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive une tche locale.
//
void _DeleteLTask(void)
{
   mtPTRCV pTaskCV;
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;
   static char cProcName[] = "DELETELOCALTASK";

   pCurrCV->pNProc = cProcName;
   pTaskCV = &LTask[pCurrCV->NumCv];
   if(pTaskCV->Actif)
   {
      // On vide les donnes locales … la tche (!).
      d4init_undo(&pTaskCV->CBase);
      MemFree(pTaskCV->PileD);
      MemFree(pTaskCV->PileR);
      MemFree(pTaskCV->pUsing);
      MemFree(pTaskCV->pLInput);
      if(pTaskCV->MLITabLine)
      {
         pDst = pTaskCV->MLITabLine;
         for(i=0;i<pTaskCV->MLIMaxLine;i++)
         {
            pLig = (mtPTRBYTE) *(pDst+i);
            MemFree(pLig);
         }
         MemFree(pDst);
      }
   }
   memset(pTaskCV,0,sizeof(UnCv));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile VRAI si la tche locale du connect courant est active, FAUX sinon.
//
void _IsLTaskActive(void)
{
   mtPTRCV pTaskCV;
   static char cProcName[] = "LOCALTASKACTIVE?";

   pCurrCV->pNProc = cProcName;
   pTaskCV = &LTask[pCurrCV->NumCv];
   if(pTaskCV->Actif)
      __TRUE();
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                          MANIPULATION DES DATES
//                          ออออออออออออออออออออออ
//
// Converti une date de la forme "JJ/MM/AAAA" vers la forme "AAAAMMJJ"
//
void _DateDtoS(void)
{
   mtPTRDICO pSym,pLi,pCo,pDate;
   mtPTRBYTE pcD;
   static char cProcName[] = "DTOS";

   pCurrCV->pNProc = cProcName;
   pcD   = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pDate = GenCvICHAR(9);
   date4init(pDate->V.cVal,pcD,"DD/MM/CCYY");
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti une date de la forme "AAAAMMJJ" vers la forme "JJ/MM/AAAA"
//
void _DateStoD(void)
{
   mtPTRDICO pSym,pLi,pCo,pDate;
   mtPTRBYTE pcD;
   static char cProcName[] = "STOD";

   pCurrCV->pNProc = cProcName;
   pcD   = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pDate = GenCvICHAR(11);
   date4format(pcD,pDate->V.cVal,"DD/MM/CCYY");
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti une date de la forme "AAAAMMJJ" en jours julien.
//
void _DateDtoJ(void)
{
   mtPTRDICO pSym,pLi,pCo,pJul;
   mtPTRBYTE pcD;
   static char cProcName[] = "DTOJ";

   pCurrCV->pNProc = cProcName;
   pcD  = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pJul = GenCvINUMBER();
   pJul->V.fVal = (float) date4long(pcD);
   _DPush(pJul);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Converti une date en jours julien en une date de la forme "AAAAMMJJ".
//
void _DateJtoD(void)
{
   mtPTRDICO pSym,pLi,pCo,pDate;
   float Jul;
   mtDWORD lJ;
   static char cProcName[] = "JTOD";

   pCurrCV->pNProc = cProcName;
   Jul   = RecupNumber(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   lJ    = Jul;
   pDate = GenCvICHAR(9);
   date4assign(pDate->V.cVal,lJ);
   _DPush(pDate);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le jours du mois (1 … 31) de la date donne.
//
void _DateDay(void)
{
   mtPTRDICO pSym,pLi,pCo,pDay;
   mtPTRBYTE pcD;
   static char cProcName[] = "DAY";

   pCurrCV->pNProc = cProcName;
   pcD  = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pDay = GenCvINUMBER();
   pDay->V.fVal = (float) date4day(pcD);
   _DPush(pDay);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le jour de la semaine (1 … 7) de la date donne.
//
void _DateDow(void)
{
   mtPTRDICO pSym,pLi,pCo,pDow;
   mtPTRBYTE pcD;
   static char cProcName[] = "DOW";

   pCurrCV->pNProc = cProcName;
   pcD  = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pDow = GenCvINUMBER();
   pDow->V.fVal = (float) date4dow(pcD);
   _DPush(pDow);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le mois (1 … 12) de la date donne.
//
void _DateMonth(void)
{
   mtPTRDICO pSym,pLi,pCo,pMonth;
   mtPTRBYTE pcD;
   static char cProcName[] = "MONTH";

   pCurrCV->pNProc = cProcName;
   pcD    = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pMonth = GenCvINUMBER();
   pMonth->V.fVal = (float) date4month(pcD);
   _DPush(pMonth);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile l'anne de la date donne.
//
void _DateYear(void)
{
   mtPTRDICO pSym,pLi,pCo,pYear;
   mtPTRBYTE pcD;
   static char cProcName[] = "YEAR";

   pCurrCV->pNProc = cProcName;
   pcD   = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pYear = GenCvINUMBER();
   pYear->V.fVal = (float) date4year(pcD);
   _DPush(pYear);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom du jours de la date donne.
//
void _DateCDow(void)
{
   mtPTRDICO pSym,pLi,pCo,pcDow;
   mtPTRBYTE pcD;
   int nj;
   static char *tJours[8] = {"","Dimanche","Lundi","Mardi","Mercredi","Jeudi","Vendredi","Samedi"};
   static char cProcName[] = "CDOW";

   pCurrCV->pNProc = cProcName;
   pcD   = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   nj    = date4dow(pcD);
   if(nj>0)
   {
      pcDow = GenCvICHAR(strlen(tJours[nj])+1);
      strcpy(pcDow->V.cVal,tJours[nj]);
   }
   else
      pcDow = GenCvICHAR(1);
   _DPush(pcDow);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le nom du mois de la date donne.
//
void _DateCMonth(void)
{
   mtPTRDICO pSym,pLi,pCo,pcMonth;
   mtPTRBYTE pcD;
   int nm;
   static char *tMois[13] = {"","Janvier","Fvrier","Mars","Avril","Mai","Juin","Juillet","Ao—t","Septembre","Octobre","Novembre","Dcembre"};
   static char cProcName[] = "CMONTH";

   pCurrCV->pNProc = cProcName;
   pcD   = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   nm    = date4month(pcD);
   if(nm>0)
   {
      pcMonth = GenCvICHAR(strlen(tMois[nm])+1);
      strcpy(pcMonth->V.cVal,tMois[nm]);
   }
   else
      pcMonth = GenCvICHAR(1);
   _DPush(pcMonth);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                      MANIPULATION DES FICHIERS TEXTES
//                      ออออออออออออออออออออออออออออออออ
//
// Ouverture du fichier donn.
//
void _FTOpen(void)
{
   mtPTRBYTE pName;
   mtPTRDICO pDSym,pLi,pCo;
   mtWORD Handle;
   static char cProcName[] = "FOPEN";

   pCurrCV->pNProc = cProcName;
   pName  = RecupString(&pDSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   Handle = sopen(pName,O_BINARY | O_RDONLY,SH_DENYNO);
   if(Handle != -1)
   {
      if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
         close(pCurrCV->fHandle);
      pCurrCV->fHandle  = Handle;
      pCurrCV->fCurrPos = (mtDWORD) 0;
      pCurrCV->fEof     = mtFALSE;
      __TRUE();
   }
   else
      __FALSE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ferme le fichier du cv courant.
//
void _FTClose(void)
{
   static char cProcName[] = "FCLOSE";
   pCurrCV->pNProc = cProcName;

   if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
      close(pCurrCV->fHandle);
   pCurrCV->fHandle  = -1;
   pCurrCV->fCurrPos = (mtDWORD) 0;
   pCurrCV->fEof     = mtFALSE;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si la fin du fichier du cv courant est atteinte.
//
void _FTEof(void)
{
   static char cProcName[] = "FEOF";

   pCurrCV->pNProc = cProcName;
   if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
   {
      if(pCurrCV->fEof)
         __TRUE();
      else
         __FALSE();
   }
   else
      __TRUE();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'une ligne de texte du fichier du cv courant.
//
void _FTReadLn(void)
{
   mtPTRDICO pLigne;
   char Buff[513];
   mtDWORD lPos;
   static char cProcName[] = "FREADLN";

   pCurrCV->pNProc = cProcName;
   if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
   {
      memset(Buff,0,513);
      // Sauvegarde de la position actuelle.
      lPos = _FileGetPos(pCurrCV->fHandle);
      lseek(pCurrCV->fHandle,pCurrCV->fCurrPos,SEEK_SET);
      _FileReadLine(pCurrCV->fHandle,Buff,512);
      if(_eof(pCurrCV->fHandle))
         pCurrCV->fEof = mtTRUE;
      else
         pCurrCV->fCurrPos = _FileGetPos(pCurrCV->fHandle);
      // Restauration de l'ancienne position.
      lseek(pCurrCV->fHandle,lPos,SEEK_SET);
      pLigne = GenCvICHAR(strlen(Buff)+1);
      strcpy(pLigne->V.cVal,Buff);
      _DPush(pLigne);
   }
   else
      ExecError(pCurrCV->pNProc,ERRFTNOOPEN,NULL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture de n octets du fichier courant.
//
void _FTRead(void)
{
   mtPTRDICO Lon,pLi,pCo,pBuff,pLu;
   float fLon;
   mtWORD wLon;
   mtDWORD lPos;
   int nRead;
   static char cProcName[] = "FREAD";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pLu   = GenCvINUMBER();
   pBuff = GenCvICHAR(1);
   if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
   {
      if(wLon > 0)
      {
         pBuff->V.cVal = (mtPTRBYTE) MemReAlloc(pBuff->V.cVal,wLon+1);
         memset(pBuff->V.cVal,0,wLon+1);
         // Sauvegarde de la position actuelle.
         lPos = _FileGetPos(pCurrCV->fHandle);
         lseek(pCurrCV->fHandle,pCurrCV->fCurrPos,SEEK_SET);
         nRead       = read(pCurrCV->fHandle,pBuff->V.cVal,wLon);
         pLu->V.fVal = nRead;
         if(_eof(pCurrCV->fHandle))
            pCurrCV->fEof = mtTRUE;
         else
            pCurrCV->fCurrPos = _FileGetPos(pCurrCV->fHandle);
         // Restauration de l'ancienne position.
         lseek(pCurrCV->fHandle,lPos,SEEK_SET);
      }
   }
   else
      ExecError(pCurrCV->pNProc,ERRFTNOOPEN,NULL);
   _DPush(pBuff);
   _DPush(pLu);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile la taille du fichier ouvert.
//
void _FTSize(void)
{
   mtPTRDICO pSize;
   static char cProcName[] = "FSIZE";

   pCurrCV->pNProc = cProcName;
   pSize = GenCvINUMBER();
   if((pCurrCV->fHandle) && (pCurrCV->fHandle != -1))
      pSize->V.fVal = (float) filelength(pCurrCV->fHandle);
   _DPush(pSize);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                     CALCULS DE CRC ET CONVERSIONS 8/7 BITS
//                     ออออออออออออออออออออออออออออออออออออออ
//
// Empile le CRC 16 bits du buffer donn.
//
void _CRC16Buff(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pCRC;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD i,wLon;
   mtUSHORT uCRC;
   mtBYTE b;
   static char cProcName[] = "CRC16";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pCRC  = GenCvINUMBER();
   uCRC  = 0x0000;
   for(i=0;i<wLon;i++)
   {
      b    = pBuff[i];
      uCRC = UpdateCRC16(b,uCRC);
   }
   pCRC->V.fVal = uCRC;
   _DPush(pCRC);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Empile le CRC 32 bits du buffer donn.
//
void _CRC32Buff(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pCRC;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD i,wLon;
   mtUWORD uCRC;
   mtBYTE b;
   static char cProcName[] = "CRC32";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pCRC  = GenCvINUMBER();
   uCRC  = 0xFFFFFFFF;
   for(i=0;i<wLon;i++)
   {
      b    = pBuff[i];
      uCRC = UpdateCRC32(b,uCRC);
   }
   pCRC->V.fVal = uCRC;
   _DPush(pCRC);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Conversion du buffer donne de 8 bits vers 7 bits.
//
void _Conv8to7(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
   mtPTRBYTE pDst;
   static char cProcName[] = "B8->7";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon*2+1);
   pDst  = pNew->V.cVal;
   while(wLon)
   {
      tr8to7(&pBuff,&pDst);
      wLon--;
   }
   wLon         = (pDst-pNew->V.cVal);
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,wLon+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = wLon;
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Conversion du buffer donne de 7 bits vers 8 bits.
//
void _Conv7to8(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
   mtPTRBYTE pDst;
   static char cProcName[] = "B7->8";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon+1);
   pDst  = pNew->V.cVal;
   while(wLon)
   {
      tr7to8(&pBuff,&pDst);
      wLon--;
   }
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,strlen(pNew->V.cVal)+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = (float) strlen(pNew->V.cVal);
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// XXEncodage d'un buffer
//
void __XXEncode(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
//   mtPTRBYTE pDst;
   static char cProcName[] = "XXENCODE";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon*2+1);
//   pDst  = pNew->V.cVal;
   _XXEncode(pBuff,wLon,pNew->V.cVal);
   wLon         = strlen(pNew->V.cVal);
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,wLon+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = wLon;
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// XXDecodage d'un buffer
//
void __XXDecode(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
//   mtPTRBYTE pDst;
   static char cProcName[] = "XXDECODE";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon+1);
//   pDst  = pNew->V.cVal;
   _XXDecode(pBuff,wLon,pNew->V.cVal);
   wLon         = strlen(pNew->V.cVal);
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,wLon+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = wLon;
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// UUEncodage d'un buffer
//
void __UUEncode(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
//   mtPTRBYTE pDst;
   static char cProcName[] = "UUENCODE";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon*2+1);
//   pDst  = pNew->V.cVal;
   _UUEncode(pBuff,wLon,pNew->V.cVal);
   wLon         = strlen(pNew->V.cVal);
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,wLon+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = wLon;
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// UUDecodage d'un buffer
//
void __UUDecode(void)
{
   mtPTRDICO pDBuff,Lon,pLi,pCo,pNew;
   mtPTRBYTE pBuff;
   float fLon;
   mtWORD wLon;
//   mtPTRBYTE pDst;
   static char cProcName[] = "UUDECODE";

   pCurrCV->pNProc = cProcName;
   fLon  = RecupNumber(&Lon,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   pBuff = RecupString(&pDBuff,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   wLon  = fLon;
   pNew  = GenCvICHAR(wLon+1);
//   pDst  = pNew->V.cVal;
   _UUDecode(pBuff,wLon,pNew->V.cVal);
   wLon         = strlen(pNew->V.cVal);
   pNew->V.cVal = (mtPTRBYTE) MemReAlloc(pNew->V.cVal,wLon+1);
   Lon          = GenCvINUMBER();
   Lon->V.fVal  = wLon;
   _DPush(pNew);
   _DPush(Lon);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//                EMISSION/RECEPTION DE PAQUETS EN MODE BINAIRE
//                อออออออออออออออออออออออออออออออออออออออออออออ
//
// Vide le buffer de sortie du connect courant.
//
void _FlushOutBuff(void)
{
   static char cProcName[] = "FLUSHOUTBUFFER";

   pCurrCV->pNProc  = cProcName;
   FreeCvOutB(pCurrCV);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi le buffer donn
//
void _TransmitBuff(void)
{
   mtPTRDICO pSym,pLi,pCo;
   mtPTRBYTE pBuff;
   static char cProcName[] = "EMITBUFFER";

   pCurrCV->pNProc = cProcName;
   pBuff = RecupString(&pSym,&pLi,&pCo,pCurrCV->pNProc,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on prend le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrCV->pOutBuff = pCurrLT->pOutBuff;
   AppendCvOBuff(pCurrCV,pBuff,mtFALSE);
   if(pCurrCV->IsLTask)
      // Tche locale, on met … jour le pointeur du cv propritaire.
      // (le buffer de sortie est commun)
      pCurrLT->pOutBuff = pCurrCV->pOutBuff;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

