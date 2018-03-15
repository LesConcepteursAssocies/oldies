// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณAFFICHAG.C   ณ                                                             ณ
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
// ณGestion des affichages                                                     ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <dos.h>
#include <disp.h>
#include "generic.h"

mtBYTE   LigErr,ColErr;                // Position des messages d'erreurs.
mtBYTE   LigDH,ColDH;                  // Position de la date & heure.
mtUSHORT CoulDH;                       // Couleur         "
mtBYTE   LigInf,ColInf;                // Position des msg d'informations.
mtBYTE   LDebInf,CDebInf;              // Position de la fentre des
mtBYTE   LFinInf,CFinInf;              // messages d'informations.
mtBYTE   LigCmd,ColCmd;                // Position des commandes.
mtBYTE   LDebCmd,CDebCmd;              // Position de la fentre des
mtBYTE   LFinCmd,CFinCmd;              // commandes.
mtPTRUSHORT pSvgInfo;                  // Sauvegarde de la fentre d'informations.
mtBYTE   sLigI,sColI;                  // Sauvegarde de la position des msg d'informations.
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
extern   mtUSHORT    CoulErr;          // Couleur des msg. d'erreur
extern   mtUSHORT    CoulSys;          // Couleur des infos systme
extern   mtUSHORT    CoulInf;          // Couleur des msg. d'information
extern   mtUSHORT    CoulCmd;          // Couleur des commandes
extern   mtUSHORT    CoulCur;          // Couleur du curseur
extern   mtUSHORT    CoulBdx;          // Couleur des bandeaux d'affichage.
extern   mtBOOL      bInfoGen;
extern   struct dos_date_t DLanc;
extern   struct dos_time_t HLanc;
extern   struct dos_time_t HFinServ;
extern   mtBOOL      bArretAuto;
extern   mtUDWORD    RAMDispo;
extern   int         NbCnx;
extern   int         PointeCnx;
extern   float       ChargeA;
extern   float       ChargeM;
extern   void        SWVRingBell(void);
extern   void        SWVScroll(mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE);
extern   void        SWVPutChar(mtBYTE,mtBYTE,mtBYTE,mtUSHORT);
extern   void        SWVSetCursor(mtBOOL);
extern   mtPOINTEUR  MemAlloc(mtUDWORD);
extern   void        MemFree(mtPOINTEUR);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
void VPrint(mtBYTE,mtBYTE,mtPTRBYTE,mtUSHORT);
mtBYTE WinPrint(mtPTRBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtBYTE,mtUSHORT);
void ClsAllWin(void);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Ecriture directe en RAM vido
// Vb a au plus 80 car. de long
void VPrint(mtBYTE Li,mtBYTE Co,mtPTRBYTE Vb,mtUSHORT Cc)
{
   int iC=0;

   if(Vb)
   {
      disp_move(Li,Co);
      disp_flush();
      disp_setattr(Cc);
      // Filtrage des codes de contr“les.
      while(Vb[iC])
      {
         if(Vb[iC] < 32)
            Vb[iC] = 'þ';
         iC++;
      }
      disp_printf("%s",Vb);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage d'un message d'erreur.
//
void WinMsgErr(mtPTRBYTE Msg,mtPTRBYTE lErr, ...)
{
   va_list fParam;
   char VBuff[128];
   int lBuff;

   va_start(fParam,lErr);
   vsprintf(VBuff,lErr,fParam);
   lBuff = strlen(VBuff);
   if((strlen(Msg)+lBuff) > 79)
   {
      // On tronque la ligne
      // (on remplace la fin par "...")
      strncpy(&VBuff[76-strlen(Msg)],"...",3);
      VBuff[76-strlen(Msg)+3] = '\0';
   }
   strcat(VBuff,Msg);
   SWVRingBell();
   VPrint(LigErr,ColErr,VBuff,CoulErr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Primitive d'affichage dans une fentre avec gestion du scrolling
// Renvoie la nouvelle valeur de la ligne d'affichage.
//
mtBYTE WinPrint(mtPTRBYTE Chaine,mtBYTE Lig,mtBYTE Col,mtBYTE LDeb,mtBYTE CDeb,mtBYTE LFin,mtBYTE CFin,mtUSHORT Couleur)
{
   char VTmp[81];
   int lenlig,plig=0;
   mtBYTE Ligne;

   Ligne  = Lig;
   lenlig = strlen(Chaine);
   while(lenlig > 0)
   {
      memset(VTmp,0,81);
      strncpy(VTmp,Chaine+plig,80);
      if(Ligne == LFin+1)
      {
         SWVScroll(SWSCROLLUP,LDeb,CDeb,LFin,CFin,1,Couleur);
         Ligne = LFin;
      }
      VPrint(Ligne,Col,VTmp,Couleur);
      Ligne++;
      lenlig -= 80;
      plig   += 80;
   }
   return(Ligne);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage d'un message d'information gnrale.
//
void WinInfo(mtPTRBYTE MsgInf, ...)
{
   va_list fParam;
   char VBuff[1024];

   if(bInfoGen)
   {
      va_start(fParam,MsgInf);
      vsprintf(VBuff,MsgInf,fParam);
      LigInf = WinPrint(VBuff,LigInf,ColInf,LDebInf,CDebInf,LFinInf,CFinInf,CoulInf);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Affichage des infos systme.
void WinSysInfo(void)
{
   mtBYTE lTmp;
   char VBuff[128];

   sprintf(VBuff,"Lancement le %02d/%02d/%04d … %02d:%02d:%02d",DLanc.day,DLanc.month,DLanc.year,HLanc.hour,HLanc.minute,HLanc.second);
   lTmp = WinPrint(VBuff,1,0,1,0,3,79,CoulSys);
   sprintf(VBuff,"R.A.M = %5lu Ko",(mtUDWORD) (RAMDispo/1024L));
   lTmp = WinPrint(VBuff,1,64,1,0,3,79,CoulSys);
   sprintf(VBuff,"Charge = %5.1f/%5.1f %%",ChargeA,ChargeM);
   lTmp = WinPrint(VBuff,2,0,1,0,3,79,CoulSys);
   sprintf(VBuff,"Connexions = %3d/%3d",NbCnx,PointeCnx);
   lTmp = WinPrint(VBuff,2,60,1,0,3,79,CoulSys);
   if(bArretAuto)
      sprintf(VBuff,"Arret Auto … %02d:%02d",HFinServ.hour,HFinServ.minute);
   else
      sprintf(VBuff,"                  ");
   lTmp = WinPrint(VBuff,3,0,1,0,3,79,CoulSys);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Efface la fentre d'informations gnrales.
void ClsWinInf(mtBOOL bCLS)
{
   if(bCLS)
   {
      if(bInfoGen)
      {
         // Sauvegarde de la fentre d'informations avant effacement.
         disp_peekbox(pSvgInfo,LDebInf,CDebInf,LFinInf,CFinInf);
         sLigI = LigInf;
         sColI = ColInf;
      }
      SWVScroll(SWSCROLLUP,LDebInf,CDebInf,LFinInf,CFinInf,(LFinInf-LDebInf)+1,CoulInf);
      LigInf = 4;
      ColInf = 0;
   }
   else
   {
      if(bInfoGen)
      {
         SWVScroll(SWSCROLLUP,LDebInf,CDebInf,LFinInf,CFinInf,(LFinInf-LDebInf)+1,CoulInf);
         LigInf = 4;
         ColInf = 0;
      }
      else
      {
         // Restauration de la sauvegarde.
         disp_pokebox(pSvgInfo,LDebInf,CDebInf,LFinInf,CFinInf);
         LigInf = sLigI;
         ColInf = sColI;
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Efface toutes les fentre.
void ClsAllWin(void)
{
   SWVScroll(SWSCROLLUP,0,0,0,79,1,CoulDH);
   SWVScroll(SWSCROLLUP,1,0,3,79,3,CoulSys);
   ClsWinInf(mtTRUE);
   SWVScroll(SWSCROLLUP,LDebCmd,CDebCmd,LFinCmd,CFinCmd,(LFinCmd-LDebCmd)+1,CoulCmd);
   // Curseur … ON.
   SWVPutChar(LigCmd,ColCmd,' ',CoulCur);
   SWVScroll(SWSCROLLUP,LigErr,ColErr,LigErr,79,1,CoulErr);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisation des paramtres d'affichage.
void InitWin(void)
{
   LigErr  = 24;
   ColErr  = 0;
   LigDH   = 0;
   ColDH   = 61;
   CoulDH  = SWVBLDATR(SWBLACK,SWWHITE);
   LigInf  = 4;
   ColInf  = 0;
   LDebInf = LigInf;
   CDebInf = ColInf;
   LFinInf = 20;
   CFinInf = 79;
   LigCmd  = 21;
   ColCmd  = 0;
   LDebCmd = LigCmd;
   CDebCmd = ColCmd;
   LFinCmd = 23;
   CFinCmd = 79;
   //
   // Allocation de la zone de sauvegarde de la fentre d'informations.
   //
   pSvgInfo = (mtPTRUSHORT) MemAlloc((LFinInf-LDebInf+1)*(CFinInf-CDebInf+1)*sizeof(mtUSHORT));
   //
   // Initialisation des diverses fentres
   //
   ClsAllWin();
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fin du fentrage.
//
void EndWin(void)
{
   MemFree(pSvgInfo);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

