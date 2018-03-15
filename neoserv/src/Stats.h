// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³STATS.H      ³                                                             ³
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
// ³DerniŠre modification : 31/07/1995                                         ³
// ³                                                                           ³
// ³D‚finitions des donn‚es relatives aux statistiques                         ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__STATS_H)
#define _STATS_H

#define  MAXPALIER 12

//
// Statistiques g‚n‚rales des divers paliers.
//
typedef struct _StPalier
{
   char            Nom[9];             // Nom ("3615 T34", ...).
   mtBYTE          Palier;             // Palier (83, ...).
   float           Tarif;              // Montant du reversement … la mn.
   mtUDWORD        NbAppel;            // Nombre d'appels pour ce palier.
   mtUDWORD        TpsCnx;             // Temps de connexion pour ce palier en secondes.
} StPalier;

//
// Statistiques relatives … une application.
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

