// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³PORTS.H      ³                                                             ³
// ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÙ                                                             ³
// ³Auteur                : Guillaumaud Philippe                               ³
// ³                        teknik@yaka2.com                                   ³
// ³                        http://www.yaka2.com/teknik                        ³
// ³                                                                           ³
// ³Vous ˆtes autoris‚ … diffuser et … utiliser ce logiciel sans restrictions. ³
// ³                                                                           ³
// ³Ce logiciel est fourni tel quel, sans aucune garantie.                     ³
// ³L'auteur ne saurait voir sa responsabilit‚ engag‚e en cas de               ³
// ³dommages de quelque nature que ce soit subis par l'utilisateur             ³
// ³ou des tiers et r‚sultant directement ou                                   ³
// ³indirectement de son utilisation, notamment la perte de donn‚es,           ³
// ³ou toute perte financiŠre r‚sultant de son                                 ³
// ³utilisation ou de l'impossibilit‚ de l'utiliser, et ceci mˆme si           ³
// ³l'auteur a ‚t‚ pr‚venu de la possibilit‚ de tels dommages.                 ³
// ³                                                                           ³
// ³Commenc‚ le           : 09/06/1993                                         ³
// ³DerniŠre modification : 21/11/1995                                         ³
// ³                                                                           ³
// ³D‚finitions des donn‚es relatives aux Ports de communication               ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__PORTS_H)
#define _PORTS_H

#define  MAXPORTS                 4    // Nbre maxi de ports g‚r‚s.
#define  LEN_PBUFF                4096 // Taille du buffer de r‚ception temporaire.

#define  SWCSUCCESSFUL            0
#define  SWCQUEUEISEMPTY          -498
#define  SWCTERMINATORREACHED     -478

#define  SWPARITYNONE             0
#define  SWPARITYODD              1
#define  SWPARITYEVEN             2

#define  TPORTX25ASM              0
#define  TPORTLOCAL               1
#define  TPORTRTC                 2
#define  TPORTX25MITEC            3

typedef struct
{
   mtBOOL   Actif;                     // Vrai si le port est actif.
   mtBOOL   ArretCarte;                // TRUE= Arret de la carte X25.
   mtWORD   Type;                      // Type du port (X25, LOCAL).
   mtWORD   MaxCV;                     // Nombre de cv maxi g‚r‚s sur le port (MITEC).
   mtBYTE   Nom[26];                   // Nom du modem (RTC).
   mtBYTE   NApp[11];                  // Nom de l'application … ex‚cuter (RTC).
   mtWORD   EtatM;                     // Etat du modem.
   mtWORD   EtatL;                     // Etat de la ligne.
   mtBYTE   Buffer[LEN_PBUFF+1];       // Buffer temporaire.
   mtWORD   wInBuf;
   mtUDWORD SpeedCom;
   mtUSHORT Parite;
   mtUSHORT WordLen;
   mtUSHORT StopBit;
   mtUDWORD NbPaqLu;                   // Nombre de paquets lus.
   mtUDWORD NbPaqErr;                  // Nombre de paquets rat‚s.
   mtUDWORD NbPaqOut;                  // Nombre de paquets envoy‚s.
   mtBOOL   (*InitialisePort) (int);             // Pointe sur la fonction d'initialisation du port.
   mtBOOL   (*ReadPaquet) (int);                 // Pointe sur la fonction de lecture de paquet.
   mtBOOL   (*TestTX) (int,mtUWORD,mtPTRWORD);   // Pointe sur la fonction de test de transmission.
   void     (*SendDATA) (int,mtUWORD,mtPTRBYTE); //   "     "   "    "     d'envoi de paquet de donn‚es.
   void     (*LibereCV) (mtUWORD);               //   "     "   "    "     de lib‚ration d'un cv.
   void     (*CommLibereAll) (int);              //   "     "   "    "     de lib‚ration globale.
   void     (*CommEtabli) (int,mtUWORD);         //   "     "   "    "     de confirmation d'appel.
} Ports;

#endif

