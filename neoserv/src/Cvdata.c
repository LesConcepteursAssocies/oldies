// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณCVDATA.C     ณ                                                             ณ
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
// ณManipulation des structures relatives aux Connects                        ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <mem.h>
#include <direct.h>
#include <io.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"

//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   UnCv           LTask[TOTALCV];    // Dfini Dans le prog. principal.
extern   UnCv           OLCv[TOTALCV];     // Dfini Dans le prog. principal.
extern   int            NbCnx;
extern   mtPTRCV        pCurrCV;           // Pointe sur le cv courant.
extern   mtPOINTEUR     MemAlloc(mtUDWORD);
extern   mtPOINTEUR     MemReAlloc(mtPOINTEUR,mtUDWORD);
extern   void           MemFree(mtPOINTEUR);

extern   mtPTRDICO      NewDicEntry(pPOINTEUR,mtPTRUWORD,mtPTRBYTE,mtBYTE,mtBOOL);
extern   mtPTRLISTDICO  NewListDico(pPOINTEUR,mtPTRUWORD,mtPTRAPP);
extern   mtPTRLISTDICO  SearchListDico(pPLISTDICO,mtUWORD,mtPTRAPP);
extern   void           DelIDico(mtPTRCV);
extern   void           DelTFKEY(mtPTRTFKEY);
extern   void           DelTKEYW(mtPTRTKEYW);
extern   void           DelListDico(pPLISTDICO,mtUWORD);
extern   mtPTRBYTE      TrAccents(mtPTRBYTE,mtBYTE);
extern   void           WinInfo(mtPTRBYTE,...);
extern   void           MajStatPalier(mtPTRCV);
extern   void           ClsWinInf(mtBOOL);
extern   void           MajTPointe(void);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void          AppendCvData(mtPTRCV,mtPTRBYTE,mtWORD);
void          AppendCvOBuff(mtPTRCV,mtPTRBYTE,mtBOOL);
void          FreeCvData(mtPTRCV);
void          FreeCvOutB(mtPTRCV);
void          FreeCv(mtPTRCV);
void          InitGTaskCv(mtPTRAPP);
void          DeconnecteGTCv(mtPTRAPP);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute une nouvelle entre … la liste des connects d'une appli.
// Renvoie un pointeur sur la nouvelle entre cre.
//
mtPTRLSTCV NewLstCv(mtPTRLSTCV ListCv,mtPTRCV LeCv)
{
   mtPTRLSTCV NewL,l;

   NewL      = (mtPTRLSTCV) MemAlloc(sizeof(LstCv));
   NewL->pCv = (mtPOINTEUR) LeCv;
   if(! ListCv)
      // 1re entre
      ListCv = NewL;
   else
   {
      // Insertion
      l = ListCv;
      while(l->pNext)
         l = l->pNext;                 // On se place … la fin de la liste.
      l->pNext = NewL;
   }
   return(NewL);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une liste de connects.
//
void DelLstCv(mtPTRLSTCV ListCv)
{
   mtPTRLSTCV l,pLNext;

   l = ListCv;
   while(l)
   {
      pLNext = l->pNext;
      MemFree(l);
      l = pLNext;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Suppression d'une entre de la liste des connecte donne.
// Supprime de la liste la rfrence au cv donn.
//
void DelUneLstCv(pLSTCV ListCv,mtPTRCV LeCv)
{
   mtPTRLSTCV l,pLNext;

   l = *ListCv;
   if((mtPTRCV) l->pCv == LeCv)
   {
      // On veut supprimer la 1re entre.
      pLNext = l->pNext;
      MemFree(l);
      *ListCv = pLNext;
   }
   else
   {
      while(l->pNext)
      {
         if((mtPTRCV) l->pNext->pCv == LeCv)
         {
            pLNext   = l->pNext->pNext;
            MemFree(l->pNext);
            l->pNext = pLNext;
            break;
         }
         l = l->pNext;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Recherche d'une entre dans la liste des connects donne.
// Renvoie un pointeur sur l'entre trouve, ou NULL sinon.
//
mtPTRLSTCV SearchLstCv(mtPTRLSTCV ListCv,mtPTRCV LeCv)
{
   mtPTRLSTCV l=ListCv,SCv=(mtPTRLSTCV) 0;

   while(l)
   {
      if((mtPTRCV) l->pCv == LeCv)
      {
         SCv = l;
         break;
      }
      l = l->pNext;
   }
   return(SCv);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute les ventuelles donnes reues au buffer d'un Cv
//
void AppendCvData(mtPTRCV LeCv,mtPTRBYTE LesData,mtWORD uLen)
{
   mtUDWORD NewLen;
   mtPTRBYTE pDat;

   if(LeCv->bBinMode)
   {
      // Mode BINAIRE (tlchargement, ...)
      pDat = (mtPTRBYTE) MemAlloc(uLen+1);
      memcpy(pDat,LesData,uLen);
   }
   else
   {
      if((LeCv->cMode == TMODE_VDT) && (! LeCv->bNoTrAcc))
         pDat = TrAccents(LesData,1);
      else
      {
         pDat = (mtPTRBYTE) MemAlloc((strlen(LesData))+1);
         strcpy(pDat,LesData);
      }
   }
   if(strlen(pDat))
   {
      if(LeCv->pData)
      {
         NewLen = (strlen(LeCv->pData)+strlen(pDat))+1;
         LeCv->pData = (mtPTRBYTE) MemReAlloc(LeCv->pData,NewLen);
         strcat(LeCv->pData,pDat);
      }
      else
      {
         NewLen = (strlen(pDat))+1;
         LeCv->pData = (mtPTRBYTE) MemAlloc(NewLen);
         strcpy(LeCv->pData,pDat);
      }
   }
   MemFree(pDat);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ajoute les ventuelles donnes … envoyer au buffer d'un Cv
// bAcc est VRAI si on translate les accents avant.
//
void AppendCvOBuff(mtPTRCV LeCv,mtPTRBYTE LesData,mtBOOL bAcc)
{
   mtUDWORD NewLen;
   mtPTRBYTE pDat;

   if(bAcc)
   {
      if((LeCv->cMode != TMODE_TELINFO) && (! LeCv->bNoTrAcc))
         pDat = TrAccents(LesData,0);
      else
      {
         pDat = (mtPTRBYTE) MemAlloc((strlen(LesData))+1);
         strcpy(pDat,LesData);
      }
   }
   else
   {
      pDat = (mtPTRBYTE) MemAlloc((strlen(LesData))+1);
      strcpy(pDat,LesData);
   }
   if(strlen(pDat))
   {
      if(LeCv->pOutBuff)
      {
         NewLen = (strlen(LeCv->pOutBuff)+strlen(pDat))+1;
         LeCv->pOutBuff = (mtPTRBYTE) MemReAlloc(LeCv->pOutBuff,NewLen);
         strcat(LeCv->pOutBuff,pDat);
      }
      else
      {
         NewLen = (strlen(pDat))+1;
         LeCv->pOutBuff = (mtPTRBYTE) MemAlloc(NewLen);
         strcpy(LeCv->pOutBuff,pDat);
      }
   }
   MemFree(pDat);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre les donnes reues du cv.
void FreeCvData(mtPTRCV LeCv)
{
   MemFree(LeCv->pData);
   LeCv->pData = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre les donnes … envoyer du cv.
void FreeCvOutB(mtPTRCV LeCv)
{
   MemFree(LeCv->pOutBuff);
   LeCv->pOutBuff = NULL;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dsactive un Cv.
//
void FreeCv(mtPTRCV LeCv)
{
   mtPTRCV pTaskCV;
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;

   if((LeCv->fHandle) && (LeCv->fHandle != -1))
      close(LeCv->fHandle);
   if(! LeCv->IsLTask)
   {
      MemFree(LeCv->pData);
      MemFree(LeCv->pOutBuff);
   }
   MemFree(LeCv->PileD);
   MemFree(LeCv->PileR);
   if(! LeCv->IsLTask)
   {
      DelListDico(LeCv->pLDico,LeCv->uLDico);
      MemFree(LeCv->pLDico);
      DelIDico(LeCv);                        // Libration des symboles internes.
      MemFree(LeCv->pUsing);
   }
   MemFree(LeCv->pLInput);
   if(LeCv->MLITabLine)
   {
      pDst = LeCv->MLITabLine;
      for(i=0;i<LeCv->MLIMaxLine;i++)
      {
         pLig = (mtPTRBYTE) *(pDst+i);
         MemFree(pLig);
      }
      MemFree(pDst);
   }
   d4init_undo(&LeCv->CBase);
   if((! LeCv->IsLTask) && (! LeCv->IsOnLib))
   {
      DelTFKEY(LeCv->pLTabFKey);
      DelTKEYW(LeCv->pLTabKeyW);
   }
   if((! LeCv->IsGTask) && (! LeCv->IsLTask))
   {
      // Tche locale active ?
      pTaskCV = &LTask[LeCv->NumCv];
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
         memset(pTaskCV,0,sizeof(UnCv));
      }
   }
   memset(LeCv,0,sizeof(UnCv));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Copie de la valeur d'un symbole dans un autre.
//
void CopyValue(mtPTRDICO Src,mtPTRDICO Dst)
{
   mtPTRITAB pITmp,pINew;
   mtUWORD Li,Co;
   mtPTRBYTE pCh;

   switch(Src->Type)
   {
      case mtTCHAR:
      {
         Dst->V.cVal = (mtPTRBYTE) MemAlloc(1);
         if(Src->V.cVal)
         {
            Dst->V.cVal = (mtPTRBYTE) MemReAlloc(Dst->V.cVal,strlen(Src->V.cVal)+1);
            memset(Dst->V.cVal,0,strlen(Src->V.cVal)+1);
            strcpy(Dst->V.cVal,Src->V.cVal);
         }
         break;
      }
      case mtTABCHAR:
      {
         pITmp         = (mtPTRITAB) Src->XVal;
         Dst->V.tVal   = (pPOINTEUR) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(mtPOINTEUR));
         pINew         = (mtPTRITAB) MemAlloc(sizeof(InfTab));
         pINew->MaxLig = pITmp->MaxLig;
         pINew->MaxCol = pITmp->MaxCol;
         Dst->XVal     = (mtPOINTEUR) pINew;
         // On initialise le tableau avec des chaines vides.
         for(Li=0;Li<pINew->MaxLig;Li++)
         {
            for(Co=0;Co<pINew->MaxCol;Co++)
            {
               pCh = (mtPTRBYTE) MemAlloc(1);
               *((pPOINTEUR) Dst->V.tVal+((Li*pINew->MaxCol)+Co)) = pCh;
            }
         }
         break;
      }
      case mtTNUMBER:
      {
         Dst->V.fVal = Src->V.fVal;
         break;
      }
      case mtTABNUMBER:
      {
         pITmp         = (mtPTRITAB) Src->XVal;
         Dst->V.tfVal  = (mtPTRFLOAT) MemAlloc((pITmp->MaxLig*pITmp->MaxCol)*sizeof(float));
         pINew         = (mtPTRITAB) MemAlloc(sizeof(InfTab));
         pINew->MaxLig = pITmp->MaxLig;
         pINew->MaxCol = pITmp->MaxCol;
         Dst->XVal     = (mtPOINTEUR) pINew;
         break;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Copie du dictionnaire local de l'appli dans la chaine des dico du cv courant.
//
void CopyLDico(void)
{
   pPDICO TDic;
   mtPTRDICO d, NewD;
   mtPTRLISTDICO l = (mtPTRLISTDICO) 0,NewL;
   mtUWORD wID;

   if(pCurrCV->pApp->uLDico)
   {
      l = SearchListDico(pCurrCV->pLDico,pCurrCV->uLDico,pCurrCV->pApp);
      if(! l)
      {
         // le dico local de l'appli n'est pas encore install dans le cv
         NewL = NewListDico(&pCurrCV->pLDico,&pCurrCV->uLDico,pCurrCV->pApp);
         TDic = pCurrCV->pApp->pLDico;
         wID  = 0;
         while(wID<pCurrCV->pApp->uLDico)
         {
            d          = TDic[wID];
            NewD       = NewDicEntry(&NewL->pLDico,&NewL->uLDico,d->Nom,d->Type,mtFALSE);
            NewD->Attr = d->Attr;
            CopyValue(d,NewD);
            wID++;
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise les informations du cv Courant … la connexion.
//
void InitCnxCv(mtPTRCV LeCv)
{
   mtPTRLSTCV NewL;

   // On ajoute le cv … la liste des connects de l'appli
   // si ce n'est pas dj… fait.
   NewL = SearchLstCv(LeCv->pApp->pLstCnx,LeCv);
   if(! NewL)
   {
      NewL = NewLstCv(LeCv->pApp->pLstCnx,LeCv);
      if(! LeCv->pApp->pLstCnx)
         // 1re entre.
         LeCv->pApp->pLstCnx = NewL;
   }
   LeCv->pApp->NbCnx++;
   if((LeCv->pApp->NbCnx == 1) && (LeCv->pApp->bGTask))
      // Tche globale dfinie, initialisation au 1er appel de l'appli.
      InitGTaskCv(LeCv->pApp);
   if(LeCv->pApp->NbCnx > LeCv->pApp->PointeCnx)
      LeCv->pApp->PointeCnx = LeCv->pApp->NbCnx;
   if(! LeCv->pApp->NbAppTot)
      _dos_gettime(&LeCv->pApp->HPreCnx);
   _dos_gettime(&LeCv->pApp->HDerCnx);
   LeCv->pApp->NbAppTot++;
   LeCv->pHere = LeCv->pApp->pHere;
   LeCv->IP    = LeCv->pApp->ENTRY;              // Pointe sur la proc. principale.
   CopyLDico();                                  // Copie des var. locales.
   // Allocation des 1res pages des piles.
   LeCv->nPageDS = 1;
   LeCv->PileD   = (pPOINTEUR) MemAlloc((LeCv->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   LeCv->SP      = 0;
   LeCv->nPageRS = 1;
   LeCv->PileR   = (pPOINTEUR) MemAlloc((LeCv->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
   LeCv->RP      = 0;
   // Allocation de la 1re page de la pile des symboles
   // internes.
   LeCv->nPageI  = 1;
   LeCv->pIDico  = (pPOINTEUR) MemAlloc((LeCv->nPageI*ISYMPAGE)*sizeof(mtPOINTEUR));
   LeCv->ISP     = 0;
   LeCv->IGC     = 0;
   LeCv->NbISym  = 0;
   d4init(&LeCv->CBase);
   LeCv->CBase.exclusive     = 0;
   LeCv->CBase.read_only     = 0;
   LeCv->CBase.read_lock     = 0;
   LeCv->CBase.lock_attempts = 0;
   LeCv->CBase.open_error    = 0;
   LeCv->CBase.off_error     = 1;
   LeCv->TimeExec            = MAXEXEC;
   LeCv->bLFKAct             = mtTRUE;
   LeCv->bGFKAct             = mtTRUE;
   LeCv->bLKWAct             = mtTRUE;
   LeCv->bGKWAct             = mtTRUE;
   NbCnx++;
   MajTPointe();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Dconnexion d'un cv
//
void DeconnecteCv(mtPTRCV LeCv)
{
   mtPTRAPP sApp;

   if((! LeCv->IsLTask) && (! LeCv->IsOnLib))
   {
      if(LeCv->bInDebug)
         ClsWinInf(mtFALSE);
      sApp = LeCv->pApp;
      // Suppression du cv de la liste des connects de l'appli.
      if(SearchLstCv(sApp->pLstCnx,LeCv))
         DelUneLstCv(&sApp->pLstCnx,LeCv);
      if(sApp->NbCnx)
      {
         sApp->NbCnx--;
         if((sApp->NbCnx == 0) && (sApp->bGTask))
            // Tche globale dfinie, dconnexion si plus d'actifs.
            DeconnecteGTCv(sApp);
      }
      _dos_gettime(&LeCv->HeureLib);
      // Mise … jour des stats.
      MajStatPalier(LeCv);
   }
   if((! LeCv->IsLTask) &&(! LeCv->IsOnLib))
   {
      if(NbCnx > 0)
         NbCnx--;
   }
   FreeCv(LeCv);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la procdure ONLIB.
//
void InitOnLibCv(mtPTRCV LeCv)
{
   mtPTRAPP sApp;
   mtPTRCV pTaskCV,pOLCv;
   int i;
   mtPTRBYTE pLig;
   pPOINTEUR pDst;

   if(LeCv->bInDebug)
      ClsWinInf(mtFALSE);
   sApp = LeCv->pApp;
   // Suppression du cv de la liste des connects de l'appli.
   if(SearchLstCv(sApp->pLstCnx,LeCv))
      DelUneLstCv(&sApp->pLstCnx,LeCv);
   if(sApp->NbCnx)
      sApp->NbCnx--;
   _dos_gettime(&LeCv->HeureLib);
   // Mise … jour des stats.
   MajStatPalier(LeCv);
   // Libration des infos du connect principal
   // (Sauf les dicos)
   MemFree(LeCv->pData);
   MemFree(LeCv->pOutBuff);
   MemFree(LeCv->PileD);
   MemFree(LeCv->PileR);
   MemFree(LeCv->pLInput);
   if(LeCv->MLITabLine)
   {
      pDst = LeCv->MLITabLine;
      for(i=0;i<LeCv->MLIMaxLine;i++)
      {
         pLig = (mtPTRBYTE) *(pDst+i);
         MemFree(pLig);
      }
      MemFree(pDst);
   }
   MemFree(LeCv->pUsing);
   if((LeCv->fHandle) && (LeCv->fHandle != -1))
      close(LeCv->fHandle);
   d4init_undo(&LeCv->CBase);
   DelTFKEY(LeCv->pLTabFKey);
   DelTKEYW(LeCv->pLTabKeyW);
   // Tche locale active ?
   pTaskCV = &LTask[LeCv->NumCv];
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
      memset(pTaskCV,0,sizeof(UnCv));
   }
   // On initialise les donnes de la tche ONLIB.
   pOLCv = &OLCv[LeCv->NumCv];
   if(pOLCv->Actif)
      FreeCv(pOLCv);
   // Allocation des 1res pages des piles.
   pOLCv->nPageDS = 1;
   pOLCv->PileD   = (pPOINTEUR) MemAlloc((pOLCv->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
   pOLCv->SP      = 0;
   pOLCv->nPageRS = 1;
   pOLCv->PileR   = (pPOINTEUR) MemAlloc((pOLCv->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
   pOLCv->RP      = 0;
   // Initialisation des donnes globales … la tche et au connect.
   pOLCv->NumPort = LeCv->NumPort;
   pOLCv->NumCv   = LeCv->NumCv;
   strcpy(pOLCv->TrameCNX,LeCv->TrameCNX);
   pOLCv->Dept    = LeCv->Dept;
   strcpy(pOLCv->Pavi,LeCv->Pavi);
   pOLCv->Palier  = LeCv->Palier;
   pOLCv->NumComp = LeCv->NumComp;
   strcpy(pOLCv->Service,LeCv->Service);
   _dos_getdate(&pOLCv->DateCnx);
   _dos_gettime(&pOLCv->HeureCnx);
   pOLCv->TypeMinitel = LeCv->TypeMinitel;
   pOLCv->TFonction   = LeCv->TFonction;
   pOLCv->bEtoile     = LeCv->bEtoile;
   pOLCv->bDiese      = LeCv->bDiese;
   pOLCv->Attente     = LeCv->Attente;
   pOLCv->pApp        = LeCv->pApp;
   pOLCv->pLDico      = LeCv->pLDico;
   pOLCv->uLDico      = LeCv->uLDico;
   pOLCv->pIDico      = LeCv->pIDico;
   pOLCv->ISP         = LeCv->ISP;
   pOLCv->IGC         = LeCv->IGC;
   pOLCv->ISP         = LeCv->ISP;
   pOLCv->NbISym      = LeCv->NbISym;
   pOLCv->nPageI      = LeCv->nPageI;
   pOLCv->Row         = LeCv->Row;
   pOLCv->Col         = LeCv->Col;
   pOLCv->bGCursor    = LeCv->bGCursor;
   d4init(&pOLCv->CBase);
   pOLCv->CBase.exclusive     = 0;
   pOLCv->CBase.read_only     = 0;
   pOLCv->CBase.read_lock     = 0;
   pOLCv->CBase.lock_attempts = 0;
   pOLCv->CBase.open_error    = 0;
   pOLCv->CBase.off_error     = 1;
   // Initialisation et activation de la tche ONLIB.
   pOLCv->pHere   = LeCv->pApp->pHere;
   pOLCv->IP      = LeCv->pApp->ONLIB;
   pOLCv->Actif   = mtTRUE;
   pOLCv->IsOnLib = mtTRUE;
   memset((mtPTRCV) LeCv,0,sizeof(UnCv));
   if(NbCnx > 0)
      NbCnx--;
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialise la procdure GTASK.
//
void InitGTaskCv(mtPTRAPP pAp)
{
   mtPTRCV LeCv,OldCv;

   if(pAp->Active)
   {
      OldCv = pCurrCV;
      LeCv          = (mtPTRCV) pAp->pGTCv;
      LeCv->NumCv   = -1;
      LeCv->Actif   = mtTRUE;
      LeCv->NumPort = -1;
      LeCv->Dept    = 0xFF;
      strcpy(LeCv->Pavi,"888888");
      LeCv->Pavi[6] = '\0';
      LeCv->Palier  = 0xFF;
      LeCv->NumComp = 0;
      strncpy(LeCv->Service,pAp->Nom,10);
      LeCv->Service[10] = '\0';
      strupr(LeCv->Service);
      _dos_getdate(&LeCv->DateCnx);
      _dos_gettime(&LeCv->HeureCnx);
      LeCv->pApp   = pAp;
      LeCv->bPaqLu = mtTRUE;
      LeCv->pHere  = LeCv->pApp->pHere;
      LeCv->IP     = LeCv->pApp->GTASK;
      CopyLDico();                               // Copie des var. locales.
      // Allocation des 1res pages des piles.
      LeCv->nPageDS = 1;
      LeCv->PileD   = (pPOINTEUR) MemAlloc((LeCv->nPageDS*STACKPAGE)*sizeof(mtPOINTEUR));
      LeCv->SP      = 0;
      LeCv->nPageRS = 1;
      LeCv->PileR   = (pPOINTEUR) MemAlloc((LeCv->nPageRS*STACKPAGE)*sizeof(mtPOINTEUR));
      LeCv->RP      = 0;
      // Allocation de la 1re page de la pile des symboles
      // internes.
      LeCv->nPageI  = 1;
      LeCv->pIDico  = (pPOINTEUR) MemAlloc((LeCv->nPageI*ISYMPAGE)*sizeof(mtPOINTEUR));
      LeCv->ISP     = 0;
      LeCv->IGC     = 0;
      LeCv->NbISym  = 0;
      d4init(&LeCv->CBase);
      LeCv->CBase.exclusive     = 0;
      LeCv->CBase.read_only     = 0;
      LeCv->CBase.read_lock     = 0;
      LeCv->CBase.lock_attempts = 0;
      LeCv->CBase.open_error    = 0;
      LeCv->CBase.off_error     = 1;
      LeCv->TimeExec            = MAXEXEC;
      LeCv->bLFKAct             = mtFALSE;
      LeCv->bGFKAct             = mtFALSE;
      LeCv->bLKWAct             = mtFALSE;
      LeCv->bGKWAct             = mtFALSE;
      LeCv->IsGTask             = mtTRUE;
      pAp->pGTCv                = (mtPOINTEUR) LeCv;
      pCurrCV                   = OldCv;
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void DeconnecteGTCv(mtPTRAPP pAp)
{
   FreeCv((mtPTRCV) pAp->pGTCv);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

