// ีอออออออออออออัออออออออออออออออออตNEOSERVฦออออออออออออออออออออออออออออออออออธ
// ณMITEC.C      ณ                                                             ณ
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
// ณDernire modification : 25/10/1995                                         ณ
// ณ                                                                           ณ
// ณGestion des trames X25 sur carte MITEC                                     ณ
// ิอออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออพ

#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <conio.h>
#include <int.h>
#include <direct.h>
#include <share.h>
#include <fcntl.h>
#include <sys\stat.h>
#include "d4all.h"
#pragma SC align
#include "generic.h"
#include "stats.h"
#include "compiler.h"
#include "cvdata.h"
#include "minitel.h"
#include "ports.h"
#include "symcci.h"
#include "pdata.h"

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

// Canal courant.
mtUWORD  CCICv;

// Dfinition des chaines de commandes.
unsigned char CCI_LinkON[]  = "LINK=ON";    // Autoriser la connexion.
unsigned char CCI_LinkOFF[] = "LINK=OFF";   // Dconnecter un canal.
unsigned char CCI_CaaON[]   = "CAA=ON";     // Autoriser la confirmation automatique des appels.
unsigned char CCI_CaaOFF[]  = "CAA=OFF";    // Dsactiver la confirmation automatique des appels.
unsigned char CCI_ApASC[]   = "AP?";        // Demande du paquet d'appel (ASCII).
unsigned char CCI_ApBIN[]   = "APB?";       // Demande du paquet d'appel (BINAIRE).
unsigned char CCI_Lib[]     = "LIB?";       // Demande du paquet de libration.
unsigned char CCI_LCX[]     = "LIB=0;0";    // Libration.

static mtPTRBYTE CCIMsgErr[] =
{
   "",
   "Offset de requte incorrect",
   "Longueur de requte incorrect",
   "Code de fonction inconnu",
   "Argument d'IOCTL inconnu",
   "Numro de canal inconnu",
   "Conflit, requte en cours de traitement",
   "Pile d'vnements sature",
   "Donne dtruites",
   "Nom inconnu",
   "Erreur d'excution",
   "Erreur de resource mmoire",
   "Erreur de resource systme",
   "Erreur d'accs au driver",
   "Erreur de syntaxe",
   "Erreur de paramtre",
   "Erreur de valeur",
   "Erreur d'environnement",
   "Fin de chaine"
};

// Structures utilises pour la conversion du paquet d'appel.
struct service_def
{
   char   pcv;               /* <> 0   : PCV demande                        */
   char   sel_rapide;         /* <> 0   : Selection rapide demandee         */
   char   debit_appele;      /* Classe de debit transmission appele         */
   char   debit_appelant;   /* Classe de debit transmission appelant      */
   char   gfa;               /* Numero de groupe                           */
   char   modif_appele;
   int   lpaq_appele;      /* Longueur de paquet appele -> appelant      */
   int   lpaq_appelant;
   char   fen_appele;         /* Taille de fenetre appele -> appelant      */
   char   fen_appelant;
   int   dly_transit;      /* Delai de transit (en milli-secondes)      */
};

struct ap_def
{
   char   appele[16];         /* Adresse appele (16 chiffres)               */
   char   appelant[16];      /* Adresse appelant (idem)                     */
   char   motif_transfert;   /* Motif du transfert d'appel                  */
   char   transfert[16];      /* Adresse appele d'origine                  */
   struct service_def   services;
   char   protocole[4];
   char   donnees[12];      /* Donnees d'appel utilisateur (12 car)      */
};
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
extern   void      SWCPurgeTransmitQueue(int);
extern   void      SWCFlushTransmitQueue(int);
extern   void      FreeCvData(mtPTRCV);
extern   void      InitCnxCv(mtPTRCV);
extern   void      DeconnecteCv(mtPTRCV);
extern   void      InitOnLibCv(mtPTRCV);
extern   void      WinInfo(mtPTRBYTE,...);
extern   void      WinMsgErr(mtPTRBYTE,mtPTRBYTE,...);
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
mtBOOL CCIReadPaquet(int);
mtBOOL CCIReadPaquetCV(int);
void CCISendPaquet(int, mtBYTE, mtUWORD, mtPTRBYTE);
void CCISendDATA(int, mtUWORD, mtPTRBYTE);
void CCICommEtabli(int, mtUWORD);
void CCICommLibere(int, mtUWORD);
void CCILibereCV(mtUWORD);
void CCICommLibereAll(int);
mtBOOL CCIInitialisePort(int);
mtBOOL CCITestTransmission(int, mtUWORD, mtPTRWORD);
void CCICloseCarte(int);
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
//*----------------------------------------------------------------------*
//:       CONVERTION PAQUET D'APPEL VERS STRUCTURE DE DEFINITION         :
//*----------------------------------------------------------------------*
//
static void _pap_to_sap(mtPTRBYTE pap, struct ap_def *sap)
{
   int   len_pap;
   int   len_appele;
   int   len_appelant;
   int   len_service;
   int   i;
   char   *p;

   /* Recupere la longueur du paquet d'appel
   */
   len_pap = *pap;
   pap++;

   /* Recupere la longueur des champs d'adresses
   */
   len_appele = *pap & 0x0f;
   len_appelant = *pap >> 4;
   pap++;
   len_pap--;

   /* Recupere l'adresse appele
   */
   i = 0;
   p = sap->appele;
   while(len_appele)
   {
      if(i & 1)
      {
         *p = (*pap & 0x0f) | '0';
         pap++;
      }
      else
         *p = (*pap >> 4) | '0';

      p++;
      i++;
      len_appele--;
   }

   /* Recupere l'adresse appelant
   */
   p = sap->appelant;
   while(len_appelant)
   {
      if(i & 1)
      {
         *p = (*pap & 0x0f) | '0';
         pap++;
      }
      else
         *p = (*pap >> 4) | '0';

      p++;
      i++;
      len_appelant--;
   }

   /* Mises a jour
   */
   len_pap -= i / 2;
   if(i & 1)
   {
      pap++;
      len_pap --;
   }

   /* Recupere la longueur du champ 'service complementaire'
   */
   len_service = *pap;
   pap++;
   len_pap -= len_service + 1;

   /* Analyse des donnees de service complementaire
   */
   while(len_service)
   {
      p = pap;
      pap++;

      switch(*p >> 6)
      {
         case   0   :   /* 1 octet   */
            i = 1;
            switch(*p & 0x3f)
            {
               case   1   :   /* Facturation au demande et selection rapide */
                  if(*pap & 1)
                     sap->services.pcv++;

                  if(*pap & 0x80)
                     sap->services.sel_rapide = *pap >> 6;
                  break;

               case   2   :   /* Negociation des classes de debit   */
                  sap->services.debit_appele = *pap >> 4;
                  sap->services.debit_appelant = *pap & 0x0f;
                  break;

               case   3   :   /* GFA   */
                  sap->services.gfa = (*pap >> 4) * 10;
                  sap->services.gfa += *pap & 0x0f;
                  break;

               case   8   :   /* Adresse d'appele modifiee   */
                  sap->services.modif_appele = *pap;
                  break;

               default   :   /* Service inconnu   */
                  pap++;
                  break;
            }
            break;

         case   1   :   /* 2 octets   */
            i = 2;
            switch(*p & 0x3f)
            {
               case   2   :   /* Negociation longueur de paquet   */
                  sap->services.lpaq_appele = *pap;
                  pap++;
                  sap->services.lpaq_appelant = *pap;
                  break;

               case   3   :   /* Negociation taille de fenetre   */
                  sap->services.fen_appele = *pap;
                  pap++;
                  sap->services.fen_appelant = *pap;
                  break;

               case   9   :   /* Choix et indication delai de transit   */
                  sap->services.dly_transit = *pap * 256;
                  pap++;
                  sap->services.dly_transit += *pap;
                  break;

               default   :   /* Service inconnu   */
                  pap++;
                  break;
            }
            break;

         case   2   :   /* 3 octets   */
            i = 3;
            pap += 2;
            break;

         case   3   :   /* Longueur variable   */
            i = 1;
            switch(*p & 0x3f)
            {
               case   3   :   /* Avis de transfert d'appel   */
                  i += *pap;
                  pap += *pap;
                  break;

               default   :
                  i += *pap;
                  pap += *pap;
                  break;
            }
            break;
      }

      pap++;
      len_service -= i + 1;
   }

   /* Recupere l'identification de protocole
   */
   if(len_pap >= 4)
   {
      memcpy(&sap->protocole, pap, 4);
      pap += 4;
      len_pap -= 4;
   }
   else
   {
      memcpy(&sap->protocole, pap, len_pap);
      pap += len_pap;
      len_pap -= len_pap;
   }

   /* Recupere les donnees d'appel utilisateur
   */
   memcpy(&sap->donnees, pap, len_pap);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet de chaque cv actif.
//
mtBOOL CCIReadPaquet(int nPort)
{
   mtBOOL bLu = mtFALSE;

   for(CCICv=2;CCICv<=(tPorts[nPort].MaxCV+1);CCICv++)
   {
      bLu |= CCIReadPaquetCV(nPort);
   }
   return(bLu);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Lecture d'un paquet du canal CCICv.
//
mtBOOL CCIReadPaquetCV(int nPort)
{
   char PaqTmp[512];
   char  *ptmp;
   char PBuff[80];
   mtBYTE Arg;
   mtUSHORT uLen;
   int i,Ret, IoSta, Cnt;
   mtWORD iLen;
   struct ap_def sap;
   mtPTRCV p;

   p        = &TabCv[CCICv-2];

   // Lecture du status du canal.
   Ret = CCIiosta(CCICv,&IoSta);
   if(Ret != 0)
   {
      // Erreur, canal suivant.
      WinInfo("CCIReadPaquet: Erreur IOSTA, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
      WriteLogPaq("CCIReadPaquet: Erreur IOSTA, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
      return(mtFALSE);
   }
   if(! p->Actif)
   {
      if(IoSta & S_WDCNX)
         // Dconnexion en cours.
         return(mtFALSE);
      // On regarde si il y a une connexion.
      if((IoSta & S_CNX) || (IoSta & S_WCNX))
      {
         // Trame d'appel.
         // Init. de la demande du paquet.
         uLen = (mtUSHORT) strlen(CCI_ApBIN);
         Ret  = CCIioctl(CCICv,IOC_SDAT,&uLen,CCI_ApBIN);
         if(Ret != 0)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur IOC_SDAT APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur IOC_SDAT APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         // Lecture du paquet.
         memset(PaqTmp,0,512);
         uLen = 512;
         Ret  = CCIioctl(CCICv,IOC_GDAT,&uLen,PaqTmp);
         if(Ret != 0)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur IOC_GDAT APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur IOC_GDAT APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         memset(&sap,0,sizeof(struct ap_def));
         ptmp = PaqTmp;
         i    = uLen;
         while(i && (*ptmp != '='))
         {
            ptmp++;
            i--;
         }
         i--;
         *ptmp = i;
         if(! i)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur Lecture APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur Lecture APB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         // Conversion.
         _pap_to_sap(ptmp,&sap);
         WinInfo("X25: CNX Voie=%d, %s;%s;%s",CCICv-2,sap.appelant,sap.appele,sap.donnees);
         WriteLogPaq("Lecture Paquet, Type=CNX,Cv=%d,%s;%s;%s",CCICv-2,sap.appelant,sap.appele,sap.donnees);
         // Fabrication du paquet d'appel.
         memset(PBuff,0,80);
         PBuff[0] = 'M';
         // Dpartement
         strncpy(&PBuff[1],&sap.appelant[1],2);
         // Pavi
         strncpy(&PBuff[3],&sap.appelant[3],6);
         // Palier
         strncpy(&PBuff[9],&sap.appelant[9],2);
         PBuff[11] = '/';
         // Numro Complmentaire
         strncpy(&PBuff[12],&sap.appele[0],2);
         strcpy(&PBuff[14],"/0/00/");
         //
         // On prend le nom du service.
         //
         strncpy(&PBuff[20],sap.donnees,10);
         // On empile le paquet.
         PushPData(nPort,CCICv-2,PBuff,strlen(PBuff),X25CNX);
         // Canal suivant.
         return(mtTRUE);
      }
   }
   if(p->Actif)
   {
      // Canal actif.
      // On regarde si il y a dconnexion.
      if((IoSta & S_DLTCNX) && ((IoSta & S_CNX) == 0))
      {
         // Lecture des codes de libration.
         uLen = (mtUSHORT) strlen(CCI_Lib);
         Ret  = CCIioctl(CCICv,IOC_SDAT,&uLen,CCI_Lib);
         if(Ret != 0)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur IOC_SDAT LIB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur IOC_SDAT LIB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         // Lecture des codes.
         memset(PaqTmp,0,512);
         uLen = 512;
         Ret  = CCIioctl(CCICv,IOC_GDAT,&uLen,PaqTmp);
         if(Ret != 0)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur IOC_GDAT LIB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur IOC_GDAT LIB?, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         WinInfo("X25: LIB Voie=%d, %s",CCICv-2,PaqTmp);
         WriteLogPaq("Lecture Paquet, Type=LIB,Cv=%d,%s",CCICv-2,PaqTmp);
         // On empile le paquet.
         PushPData(nPort,CCICv-2,PaqTmp,strlen(PaqTmp),X25LIB);

         // Canal suivant.
         return(mtTRUE);
      }
      // Canal actif et non dconnect.
      // On regarde si il y a des donnes … lire.
      Ret = CCIinput_sta(CCICv,&IoSta,&Cnt);
      if(Ret != 0)
      {
         // Erreur, canal suivant.
         WinInfo("CCIReadPaquet: Erreur INPUT_STA, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
         WriteLogPaq("CCIReadPaquet: Erreur INPUT_STA, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
         return(mtFALSE);
      }
      if(Cnt > 0)
      {
         // Lecture des donnes.
         memset(PaqTmp,0,512);
         Cnt = 512;
         Ret = CCIinput(CCICv,&Cnt,PaqTmp,&Arg);
         if(Ret != 0)
         {
            // Erreur, canal suivant.
            WinInfo("CCIReadPaquet: Erreur INPUT, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            WriteLogPaq("CCIReadPaquet: Erreur INPUT, Voie=%d:%s",CCICv-2,CCIMsgErr[Ret]);
            return(mtFALSE);
         }
         WinInfo("X25: DATA Voie=%d, %s",CCICv-2,PaqTmp);
         WriteLogPaq("Lecture Paquet, Type=DAT,Cv=%d,%s",CCICv-2,PaqTmp);
         // On empile le paquet.
         PushPData(nPort,CCICv-2,PaqTmp,strlen(PaqTmp),X25DATA);
      }
   }
   // Canal suivant.
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi un paquet sur le port nPort.
//
void CCISendPaquet(int nPort, mtBYTE pType, mtUWORD nCv, mtPTRBYTE pBufData)
{
   int Ret,Cnt;
   mtUSHORT IoSta,uLen;

   Cnt = strlen(pBufData);
   switch(pType)
   {
      case X25LIB:
      {
         uLen = (mtUSHORT) strlen(CCI_LCX);
         Ret  = CCIioctl(nCv+2,IOC_SDAT,&uLen,CCI_LCX);
         if(Ret == 0)
         {
            Ret = CCIinput_flh(nCv+2);
         }
         break;
      }
      case X25CONF:
      {
         uLen = (mtUSHORT) strlen(CCI_LinkON);
         Ret  = CCIioctl(nCv+2,IOC_SDAT,&uLen,CCI_LinkON);
         break;
      }
      default:
         Ret = CCIoutput(nCv+2,Cnt,pBufData,0);
   }
   tPorts[nPort].NbPaqOut++;
   WriteLogPaq("Ecriture Paquet, Type=%c,Cv=%d,%s",pType,nCv,pBufData);
   if(Ret != 0)
   {
      // Erreur.
      WinInfo("CCISendPaquet: Erreur OUTPUT, Voie=%d:%s",nCv,CCIMsgErr[Ret]);
      WriteLogPaq("CCISendPaquet: Erreur OUTPUT, Voie=%d:%s",nCv,CCIMsgErr[Ret]);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Envoi d'un paquet de donnes.
//
void CCISendDATA(int nPort, mtUWORD nCv, mtPTRBYTE pBufData)
{
   CCISendPaquet(nPort,X25DATA,nCv,pBufData);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Confirme la connexion de nCv sur le port nPort.
//
void CCICommEtabli(int nPort, mtUWORD nCv)
{
   mtWORD iLen;

   if(CCITestTransmission(nPort,nCv,&iLen))
      CCISendPaquet(nPort,X25CONF,nCv,"");
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre la connexion de nCv sur le port nPort.
//
void CCICommLibere(int nPort, mtUWORD nCv)
{
   mtWORD wPos;

   CCISendPaquet(nPort,X25LIB,nCv,"0000");
   // On limine de la pile les paquets de ce cv.
   while(SearchPData(nCv,&wPos))
      FreePData(PopPData(wPos));
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Libre le cv donn.
//
void CCILibereCV(mtUWORD nCv)
{
   CCICommLibere(TabCv[nCv].NumPort,nCv);
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
void CCICommLibereAll(int nPort)
{
   mtUWORD i;

   for(i=0;i<tPorts[nPort].MaxCV;i++)
   {
      if(TabCv[i].Actif)
         CCILibereCV(i);
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Initialisations diverse du port.
// Renvoie VRAI si tout c'est bien pass, FAUX sinon.
//
mtBOOL CCIInitialisePort(int nPort)
{
   mtUWORD i;
   mtWORD Ret;
   mtUSHORT uLen;

   if(! CCIInitCartes())
   {
      WinMsgErr(": Erreur … l'initialisation des cartes MITEC","CCIInitialisePort");
      return(mtFALSE);
   }
   // Initialisation des canaux.
   // 0 = canal systme.
   // 1 = canal srie.
   // 2 = 1er cv x25.
   for(i=2;i<=(tPorts[nPort].MaxCV+1);i++)
   {
      // Ouverture.
      Ret = CCIopen(i);
      if(Ret)
      {
         WinInfo("Ouverture du canal %d:%s",i-2,CCIMsgErr[Ret]);
         WriteLogPaq("Ouverture du canal %d:%s",i-2,CCIMsgErr[Ret]);
         continue;
      }
      // Autorise la connexion.
      uLen = (mtUSHORT) strlen(CCI_LinkON);
      Ret  = CCIioctl(i,IOC_SDAT,&uLen,CCI_LinkON);
      if(Ret)
      {
         WinInfo("IOCTL (LINK=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
         WriteLogPaq("IOCTL (LINK=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
         continue;
      }
      // Pas de confirmation automatique d'appel.
      uLen = (mtUSHORT) strlen(CCI_CaaOFF);
      Ret  = CCIioctl(i,IOC_SDAT,&uLen,CCI_CaaOFF);
      if(Ret)
      {
         WinInfo("IOCTL (CAA=OFF) canal %d:%s",i-2,CCIMsgErr[Ret]);
         WriteLogPaq("IOCTL (CAA=OFF) canal %d:%s",i-2,CCIMsgErr[Ret]);
         continue;
      }
   }
   CCICv = 2;
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Renvoie VRAI si on peut emmettre pour le cv donn.
//
mtBOOL CCITestTransmission(int nPort, mtUWORD nCv, mtPTRWORD cLen)
{
   int Ret,Cnt;
   mtUSHORT IoSta;

   // Lecture du status de transmission.
   Ret   = CCIoutput_sta(nCv+2,&IoSta,&Cnt);
   *cLen = Cnt;
   if(Ret != 0)
   {
      // Erreur.
      WinInfo("CCITestTransmission: Erreur OUTPUT_STA, Voie=%d:%s",nCv,CCIMsgErr[Ret]);
      WriteLogPaq("CCITestTransmission: Erreur OUTPUT_STA, Voie=%d:%s",nCv,CCIMsgErr[Ret]);
      return(mtFALSE);
   }
   if(IoSta & S_STOP)
   {
      if(! TabCv[nCv].ArretVoie)
      {
         WinInfo("CCITestTransmission: Suspension canal %d",nCv);
         WriteLogPaq("CCITestTransmission: Suspension canal %d",nCv);
         TabCv[nCv].ArretVoie = mtTRUE;
      }
      return(mtFALSE);
   }
   else
   {
      if(TabCv[nCv].ArretVoie)
      {
         WinInfo("CCITestTransmission: Fin Suspension canal %d",nCv);
         WriteLogPaq("CCITestTransmission: Fin Suspension canal %d",nCv);
         TabCv[nCv].ArretVoie = mtFALSE;
      }
   }
   if((IoSta & S_EMPTY) == 0)
   {
      WinInfo("CCITestTransmission: Voie=%d, Buffer d'emission non-vide",nCv);
      WriteLogPaq("CCITestTransmission: Voie=%d, Buffer d'emission non-vide",nCv);
      return(mtFALSE);
   }
   return(mtTRUE);
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//
// Fermeture des canaux et envoi de la page d'indisponibilit.
//
void CCICloseCarte(int nPort)
{
   int flen,hFic;
   mtUWORD i;
   mtWORD Ret;
   mtUSHORT uLen;
   char *pFic;
   char Buff[768];

   pFic = searchpath("MODULES\\PAGES\\PACCUEIL.VTX");
   if(pFic)
   {
      hFic = sopen(pFic,O_BINARY,SH_DENYNO);
      if(hFic != -1)
      {
         memset(Buff,0,768);
         flen = read(hFic,Buff,768);
         close(hFic);
         for(i=2;i<=(tPorts[nPort].MaxCV+1);i++)
         {
            // Autorise la connexion.
            uLen = (mtUSHORT) strlen(CCI_LinkON);
            Ret  = CCIioctl(i,IOC_SDAT,&uLen,CCI_LinkON);
            if(Ret)
            {
               WinInfo("IOCTL (LINK=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
               WriteLogPaq("IOCTL (LINK=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
               continue;
            }
            // Confirmation automatique d'appel.
            uLen = (mtUSHORT) strlen(CCI_CaaON);
            Ret  = CCIioctl(i,IOC_SDAT,&uLen,CCI_CaaON);
            if(Ret)
            {
               WinInfo("IOCTL (CAA=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
               WriteLogPaq("IOCTL (CAA=ON) canal %d:%s",i-2,CCIMsgErr[Ret]);
               continue;
            }
            // Fermeture et envoi de la page d'accueil.
            Ret = CCIclose(i,flen,Buff);
            if(Ret)
            {
               WinInfo("IOCTL (CLOSE) canal %d:%s",i-2,CCIMsgErr[Ret]);
               WriteLogPaq("IOCTL (CLOSE) canal %d:%s",i-2,CCIMsgErr[Ret]);
               continue;
            }
         }
      }
   }
}
//
// ออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออออ
//

