// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³COMPILER.H   ³                                                             ³
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
// ³D‚finitions des donn‚es relatives … la compilation du code                 ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__COMPILER_H)
#define _COMPILER_H

//
// Bloc de d‚finition d'une application
//
typedef struct _DefApp
{
   struct _DefApp     *pNextAPP;       // Pointe sur la prochaine appli.
   char               Nom[11];         // Nom de l'application (10 car. maxi).
                                       // Correspond en g‚n‚ral au nom du
                                       // code t‚l‚matique, ou du module (BAL, ...).
   char               FicSrc[MAXPATH]; // Nom du fichier source principal.
   mtPTRBYTE          Path;            // R‚pertoire principal de l'application.
   mtPTRBYTE          PathDBF;         // R‚pertoire des bases de donn‚es.
   mtPTRBYTE          PathVTX;         // R‚pertoire des pages vid‚otex.
   mtBYTE             NumComp;         // Num‚ro compl‚mentaire de l'application.
   mtBOOL             Active;          // Vrai si l'appli. est utilisable.
   mtBOOL             bOnLib;          // Vrai si l'appli. … une proc. ONLIB.
   mtBOOL             bGTask;          // Vrai si l'appli. … une tƒche globale.
   mtBOOL             bAnticipe;       // Vrai si l'anticipation est active.
   pPDICO             pPDico;          // Dictionnaire public de l'appli.
                                       // Contient les symboles publics … l'appli.
   mtUWORD            uPDico;          // Nombre de symboles dans pPDico.
   pPDICO             pLDico;          // Dictionnaire local de l'appli.
   mtUWORD            uLDico;          // Nombre de symboles dans pLDico.
   pPLISTDICO         pEDico;          // Chaine des dictionnaires locaux des r‚f‚rences externes.
                                       // (utilis‚ dans les librairies communes).
   mtUWORD            uEDico;          // Nombre de dictionnaires dans pEDico.
   pPDICO             pIDico;          // Dictionnaire des symboles internes … l'appli.
                                       // (cr‚‚s … la compilation, comme les chaines
                                       //  ou nombre litt‚raux, ...).
   mtUWORD            uIDico;          // Nombre de symboles dans pIDico.
   mtPTRREFAPP        pLRefApp;        // R‚f‚rence crois‚e des modules utilisant l'appli.
                                       // courante.
   mtPTRLSTCV         pLstCnx;         // Liste chain‚e des cv actifs de
                                       // l'application courante.
   ppFUNC             pHere;           // Bloc de code de l'appli.
   mtUWORD            ENTRY;           // Pointe sur la proc. principale (MAIN).
   mtUWORD            ONLIB;           // Pointe sur la proc. de fin (ONLIB).
   mtUWORD            GTASK;           // Pointe sur la tƒche globale (GTASK).
   mtPOINTEUR         pGTCv;           // Pointe sur le CV de la tƒche globale.
   mtPTRSTAPP         pStat;           // Pointe sur les stats. pour les divers paliers permis.
   mtPTRTFKEY         pGTabFKey;       // Pointe sur la table globale des touches de fonction … intercepter.
   mtPTRTKEYW         pGTabKeyW;       // Pointe sur la table globale des mot-clefs … intercepter.
   mtUWORD            NbBlock;         // Nombre de blocs utilis‚s.
   mtUWORD            SizeCode;        // Taille du code.
   mtUWORD            SizePData;       // Taille des donn‚es publiques.
   mtUWORD            SizeLData;       // Taille des donn‚es locales.
   mtUWORD            StackSize;       // Taille des piles (directive STACKSIZE ...)
   struct  dos_time_t HPreCnx;         // Heure de la premiŠre connexion.
   struct  dos_time_t HDerCnx;         // Heure de la derniŠre connexion.
   mtUDWORD           NbAppTot;        // Nombre total d'appels.
   int                NbCnx;           // Nombre de connect‚s actifs.
   int                PointeCnx;       // Pointe des connect‚s.
   mtBOOL             bLIBErr;         // VRAI si on d‚connecte sur erreur.
   char               NFicErr[13];     // Nom du fichier d'erreurs.
   mtWORD             pFicErr;         // Handler du fichier d'erreur.
   mtBOOL             ConfCNX;         // VRAI si on confirme les appels entrants (sur X25).
   mtBOOL             bAutoCommit;     // VRAI si on raffraichit les base de l'appli.
   mtWORD             wCMIT;           // P‚riode de raffraichissement (en mn).
} DefApp;

typedef DefApp * mtPTRAPP;
typedef DefApp ** pPAPP;

#endif

