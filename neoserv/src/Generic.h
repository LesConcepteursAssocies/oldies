// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³GENERIC.H    ³                                                             ³
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
// ³DerniŠre modification : 12/03/1996                                         ³
// ³                                                                           ³
// ³D‚finitions des constantes & macros de base                                ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__GENERIC_H)
#define _GENERIC_H

#define  MAX_HISTO      20             // Taille de l'historique des commandes clavier.

#define  MAXCV          64             // Nombre de C.V g‚r‚s sur flux X25.
#define  MAXLOCCV       8              // Nombre de C.V g‚r‚s sur flux local s‚rie.
#define  TOTALCV        MAXCV+MAXLOCCV // Nombre de CV total.

//
// Constantes pr‚fix‚es par mt (pour Moniteur T‚l‚matique)
//
typedef unsigned short mtUSHORT;
typedef int            mtWORD;
typedef unsigned int   mtUWORD;
typedef long           mtDWORD;
typedef unsigned long  mtUDWORD;
typedef unsigned char  mtBYTE;
typedef mtWORD         mtBOOL;

#define mtTRUE        1
#define mtFALSE       0

typedef void *     mtPOINTEUR;
typedef mtWORD *   mtPTRWORD;
typedef mtUWORD *  mtPTRUWORD;
typedef mtDWORD *  mtPTRDWORD;
typedef mtBYTE *   mtPTRBYTE;
typedef double *   mtPTRDOUBLE;
typedef float *    mtPTRFLOAT;
typedef mtUSHORT * mtPTRUSHORT;

#define  MAXFICLOAD     100            // Nbre de fichiers chargeables dans la section Load du .INI.

//
// D‚finition des modes de fonctionnement des cv.
// (Minitel 40, 80, vocal, ...)
//
#define  TMODE_VDT      0              // Mode Vid‚oTex 40 colonnes.
#define  TMODE_MIXTE    1              // Mode Mixte (80 colonnes).
#define  TMODE_TELINFO  2              // Mode T‚l‚informatique (80 colonnes, s‚quences CSI...).

//
// Types relatifs … la compilation du code.
//
#define  ISYMPAGE        256           // Taille d'une page d'alloc. des symboles internes.
#define  STACKPAGE       128           // Taille d'une page de pile.
#define  BLOCKSIZE      1024           // Taille par d‚faut d'un bloc de code.
#define  MAXLLIG        1024           // Longueur maxi d'une ligne de programme.
#define  MAXEXEC           5           // Nombre de ms allou‚es … un cv pour l'ex‚cution de son code.

//
// D‚finition des types de donn‚es.
//
#define  mtTCHAR          1            // Type caractŠre.
#define  mtTNUMBER        2            // Type nombre (entier,long,double,flottant).
#define  mtTABCHAR       10            // Tableau de caractŠres.
#define  mtTABNUMBER     20            // Tableau de nombres.
#define  mtKERNEL       254            // Primitive du noyau.
#define  mt2POINTS      255            // Procedure/Fonction (type : xxx ... ;).

//
// D‚finition de la port‚e des symboles.
//
#define  PLOCAL    1
#define  PPUBLIC   2
#define  PGLOBAL   3
#define  PINTERNE  4
#define  PEXTERN   5
#define  PFORWARD  6

typedef void **pPOINTEUR;              // Type pointeur sur pointeur.
typedef void (**ppFUNC) ();            // Type pointeur sur pointeur de fonction.
typedef void (*pFUNC) ();              // Type pointeur sur fonction.

//
// D‚finition du type tableau de d‚finition de touche de fonction
// (fonction SET...FKEY)
//
typedef struct _TabFKey
{
   struct _TabFKey  *pNextTFKEY;       // Pointe sur la prochaine entr‚e.
   mtBOOL           bDiese;            // Vrai si le diŠse est … prendre en compte.
   mtBOOL           bEtoile;           // Vrai si l'‚toile est … prendre en compte.
   mtBYTE           TFonction;         // Code de la touche de fonction con‡ern‚e.
   mtBOOL           bReturn;           // VRAI si retour … l'appelant.
   ppFUNC           HProc;             // Bloc de code de la fonction appel‚e.
   mtUWORD          IPProc;            // IP de la fonction appel‚e.
   mtPOINTEUR       pApp;              // Pointeur sur l'application propri‚taire de la proc‚dure.
} TabFKey;
typedef TabFKey * mtPTRTFKEY;

//
// D‚finition du type tableau de d‚finition de mot-clef
// (fonction SET...KEYWORD)
//
typedef struct _TabKeyW
{
   struct _TabKeyW  *pNextTKEYW;       // Pointe sur la prochaine entr‚e.
   mtPTRBYTE        KeyWord;           // Mot-clef … intercepter.
   mtBOOL           bReturn;           // VRAI si retour … l'appelant.
   ppFUNC           HProc;             // Bloc de code de la fonction appel‚e.
   mtUWORD          IPProc;            // IP de la fonction appel‚e.
   mtPOINTEUR       pApp;              // Pointeur sur l'application propri‚taire de la proc‚dure.
} TabKeyW;
typedef TabKeyW * mtPTRTKEYW;

//
// Informations du type tableau.
//
typedef struct _InfTab
{
   mtUWORD MaxLig;
   mtUWORD MaxCol;
} InfTab;

typedef InfTab * mtPTRITAB;

//
// D‚finition d'une entr‚e de Dictionnaire.
//
typedef struct _Dico
{
  mtBOOL            bAccessed;         // VRAI si le symbole est acc‚d‚ par l'appli.
  mtPTRBYTE         Nom;               // Nom du symbole.
  mtBYTE            Type;              // Type du symbole.
  mtBYTE            Attr;              // Attribut (Lit‚ral, variable, ...)
  union _V
  {
     mtPOINTEUR     pVal;              // Pointeur g‚n‚rique.
     mtPTRBYTE      cVal;              // Pointe sur la chaine (si type=TCHAR).
     float          fVal;              // Contient la donn‚e (si type=TNUMBER).
     pPOINTEUR      tVal;              // Pointe sur les donn‚es (si type=TABCHAR).
     mtPTRFLOAT     tfVal;             // Pointe sur les donn‚es (si type=TABNUMER).
  } V;
  mtPOINTEUR        XVal;              // Pointe sur des infos. suppl‚mentaires.
} Dico;

typedef Dico * mtPTRDICO;
typedef Dico ** pPDICO;

//
// D‚finition du type chaine de dictionnaire
//
typedef struct _ListDico
{
  mtPOINTEUR        pApp;              // Contient le pointeur sur l'application
                                       // propri‚taire du dictionnaire.
  pPDICO            pLDico;            // Pointe sur le dictionnaire local con‡ern‚.
  mtUWORD           uLDico;            // Nombre de symboles du dictionnaire.
} ListDico;

typedef ListDico * mtPTRLISTDICO;
typedef ListDico ** pPLISTDICO;

//
// D‚finition du type chaine des r‚f‚rence crois‚e pour les
// applications (directive USE)
//
typedef struct _RefApp
{
  struct _RefApp *pNext;               // Pointe sur la prochaine entr‚e.
  mtPOINTEUR      pApp;
} RefApp;

typedef RefApp * mtPTRREFAPP;
typedef RefApp **pREFAPP;              // Pointeur sur un pointeur de structure.

//
// D‚finition du type liste de connect‚s.
//
typedef struct _LstCv
{
  struct _LstCv  *pNext;               // Pointe sur la prochaine entr‚e.
  mtPOINTEUR     pCv;
} LstCv;

typedef LstCv  * mtPTRLSTCV;
typedef LstCv  **pLSTCV;

//
// Utilis‚ par la compilation d'expression r‚guliŠres
//
#define  NSUBEXP        10
typedef struct _regexp
{
   mtPTRBYTE     startp[NSUBEXP];
   mtPTRBYTE     endp[NSUBEXP];
   mtBYTE        regstart;             /* Internal use only. */
   mtBYTE        reganch;              /* Internal use only. */
   mtPTRBYTE     regmust;              /* Internal use only. */
   mtWORD        regmlen;              /* Internal use only. */
   mtBYTE        program[1];           /* Unwarranted chumminess with compiler. */
} regexp;

typedef regexp  * mtPTRREGEXP;

//
// D‚finition des attributs des symboles
//
#define  SYMLITERAL     0              // Lit‚ral.
#define  SYMVARCON      1              // Variable/Constante.

//
// Constantes vid‚o
//
#define  SWBLACK        0x00
#define  SWBLUE         0x01
#define  SWGREEN        0x02
#define  SWCYAN         0x03
#define  SWRED          0x04
#define  SWMAGENTA      0x05
#define  SWBROWN        0x06
#define  SWWHITE        0x07
#define  SWBLINK        0x80
#define  SWINTENSE      0x08

#define  SWSCROLLUP     0
#define  SWSCROLLDOWN   1

//
// Codes d'erreur
//
#define  ERRDPILEVIDE        0
#define  ERRRPILEVIDE        1
#define  ERRDPILEPLEINE      2
#define  ERRRPILEPLEINE      3
#define  ERRTYPINCOMP        4
#define  ERRBOUNDARRAY       5
#define  ERRNULLPOINTER      6
#define  ERRFILEOPEN         7
#define  ERRDBOPEN           8
#define  ERRDBFAILOPEN       9
#define  ERRDBFAILCLOSE      10
#define  ERRDBALLCLOSE       11
#define  ERRDBNOCURRBASE     12
#define  ERRDBAPPENDBLANK    13
#define  ERRDBNOOPEN         14
#define  ERRDBNOFIELD        15
#define  ERRDBDATEFAIL       16
#define  ERROPLITERAL        17
#define  ERRZERODIVISE       18
#define  ERRUSINGFAIL        19
#define  ERRAPPNOTFOUND      20
#define  ERRPROCNOTFOUND     21
#define  ERRCHAREXPECT       22
#define  ERRTCHAREXPECT      23
#define  ERRNUMBEREXPECT     24
#define  ERRTNUMBEREXPECT    25
#define  ERRTABEXPECT        26
#define  ERROUTOFMEMORY      27
#define  ERRAPPNOTMAIN       28
#define  ERRDBNOFIELDNUM     29
#define  ERRFTNOOPEN         30
#define  ERRSETINDEXFAIL     31

//
// Macros
//

#define  LTrim(pc) while(*pc==' ' || *pc== '\t') pc++
#define  SWVBLDATR(usForeGround,usBackGround) ((usBackGround<<4)+usForeGround)

#endif

