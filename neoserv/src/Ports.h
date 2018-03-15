// ��������������������������������͵NEOSERV����������������������������������͸
// �PORTS.H      �                                                             �
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
// �Derni�re modification : 21/11/1995                                         �
// �                                                                           �
// �D�finitions des donn�es relatives aux Ports de communication               �
// ���������������������������������������������������������������������������;

#if !defined(__PORTS_H)
#define _PORTS_H

#define  MAXPORTS                 4    // Nbre maxi de ports g�r�s.
#define  LEN_PBUFF                4096 // Taille du buffer de r�ception temporaire.

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
   mtWORD   MaxCV;                     // Nombre de cv maxi g�r�s sur le port (MITEC).
   mtBYTE   Nom[26];                   // Nom du modem (RTC).
   mtBYTE   NApp[11];                  // Nom de l'application � ex�cuter (RTC).
   mtWORD   EtatM;                     // Etat du modem.
   mtWORD   EtatL;                     // Etat de la ligne.
   mtBYTE   Buffer[LEN_PBUFF+1];       // Buffer temporaire.
   mtWORD   wInBuf;
   mtUDWORD SpeedCom;
   mtUSHORT Parite;
   mtUSHORT WordLen;
   mtUSHORT StopBit;
   mtUDWORD NbPaqLu;                   // Nombre de paquets lus.
   mtUDWORD NbPaqErr;                  // Nombre de paquets rat�s.
   mtUDWORD NbPaqOut;                  // Nombre de paquets envoy�s.
   mtBOOL   (*InitialisePort) (int);             // Pointe sur la fonction d'initialisation du port.
   mtBOOL   (*ReadPaquet) (int);                 // Pointe sur la fonction de lecture de paquet.
   mtBOOL   (*TestTX) (int,mtUWORD,mtPTRWORD);   // Pointe sur la fonction de test de transmission.
   void     (*SendDATA) (int,mtUWORD,mtPTRBYTE); //   "     "   "    "     d'envoi de paquet de donn�es.
   void     (*LibereCV) (mtUWORD);               //   "     "   "    "     de lib�ration d'un cv.
   void     (*CommLibereAll) (int);              //   "     "   "    "     de lib�ration globale.
   void     (*CommEtabli) (int,mtUWORD);         //   "     "   "    "     de confirmation d'appel.
} Ports;

#endif

