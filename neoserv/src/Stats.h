// ��������������������������������͵NEOSERV����������������������������������͸
// �STATS.H      �                                                             �
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
// �D�finitions des donn�es relatives aux statistiques                         �
// ���������������������������������������������������������������������������;

#if !defined(__STATS_H)
#define _STATS_H

#define  MAXPALIER 12

//
// Statistiques g�n�rales des divers paliers.
//
typedef struct _StPalier
{
   char            Nom[9];             // Nom ("3615 T34", ...).
   mtBYTE          Palier;             // Palier (83, ...).
   float           Tarif;              // Montant du reversement � la mn.
   mtUDWORD        NbAppel;            // Nombre d'appels pour ce palier.
   mtUDWORD        TpsCnx;             // Temps de connexion pour ce palier en secondes.
} StPalier;

//
// Statistiques relatives � une application.
//
typedef struct _StApp
{
   struct _StApp     *pNextSt;         // Pointe sur la prochaine stat.
   mtBYTE            Palier;           // Palier (83, ...).
   mtUDWORD          NbAppel;          // Nombre d'appels pour ce palier.
   mtUDWORD          TpsCnx;           // Temps de connexion pour ce palier en secondes.
} StApp;

typedef StApp * mtPTRSTAPP;

#endif

