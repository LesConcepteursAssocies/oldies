// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³CVDATA.H     ³                                                             ³
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
// ³DerniŠre modification : 13/03/1996                                         ³
// ³                                                                           ³
// ³D‚finitions des donn‚es relatives aux Connect‚s                            ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__CVDATA_H)
#define _CVDATA_H

//
// Structure principale d'un connect‚
//
typedef struct _UnCv
{
   mtWORD           cMode;             // Type du cv (Minitel 40,80,vocal,...).
   mtBOOL           Actif;             // TRUE=Connect‚ actif.
   mtBOOL           ArretVoie;         // TRUE=Arret de la voie X25.
   mtBOOL           IsLTask;           // TRUE=Cv d'une tƒche locale.
   mtBOOL           IsOnLib;           // TRUE=Cv d'une proc ONLIB.
   mtBOOL           IsGTask;           // TRUE=Cv d'une proc GTASK.
   mtUDWORD         HRPaq;             // Heure de r‚ception du dernier paquet.
   mtBOOL           bInDebug;          // TRUE=Ex‚cution en mode debuggage.
   mtBOOL           bPaqLu;            // TRUE=Un paquet est arriv‚ pour ce cv.
   mtBYTE           NumPort;           // Nø de port sur lequel est arriv‚ ce CV.
   mtUWORD          NumCv;             // Nø de CV physique.
   char             TrameCNX[31];      // On conserve la trame d'appel (30 car. maxi).
   mtBYTE           Dept;              // D‚partement.
   char             Pavi[7];           // Nø de pavi.
   mtBYTE           Palier;            // 82=3614,70=3615bas,86=3615haut,...
   mtBYTE           NumComp;           // Num‚ro compl‚mentaire.
   char             Service[11];       // Nom du service.
   struct dos_date_t DateCnx;          // Date de la connexion.
   struct dos_time_t HeureCnx;         // Heure de la connexion.
   struct dos_time_t HeureLib;         // Heure de la d‚connexion.
   mtBYTE           TypeMinitel;       // Type de minitel connect‚.
   mtPTRBYTE        pData;             // Pointe sur les donn‚es re‡ues.
   mtPTRBYTE        pOutBuff;          // Pointe sur les donn‚es … envoyer.
   mtBYTE           TFonction;         // Eventuelle touche de fonction re‡ue.
   mtBOOL           bEtoile;           // Vrai si on a re‡u une s‚quence de type "*+Tfonction"
   mtBOOL           bDiese;            // Vrai si on a re‡u une s‚quence de type "#+Tfonction"
   mtBYTE           Attente;           // 0=n'attend rien,1=attend une touche,2=SEP d‚j… re‡u.
   mtPTRBYTE        pNProc;            // Pointe sur le nom de la proc‚dure en cours d'ex‚cution.
   mtPTRAPP         pApp;              // Pointeur sur l'application appel‚e.
   pPOINTEUR        PileD;             // Pile de donn‚es.
   pPOINTEUR        PileR;             // Pile de retour.
   mtUWORD          SP;                // Stack Pointer.
   mtUWORD          RP;                // Return Pointer.
   mtUWORD          nPageDS;           // Nombre de pages de la pile de donn‚es.
   mtUWORD          nPageRS;           // Nombre de pages de la pile de retour.
   ppFUNC           pHere;             // Bloc de code de l'appli courante.
   mtPTRAPP         pCApp;             // Pointeur sur l'appli. du dictionnaire local … copier.
   mtUWORD          IP;                // Instruction Pointer.
   pPLISTDICO       pLDico;            // Pointe sur la chaine des dictionnaires
                                       // des variables locales.
   mtUWORD          uLDico;            // Nombre de dictionnaire dans pLDico.
   pPOINTEUR        pIDico;            // Pile des symboles internes.
   mtUWORD          ISP;               // SP de pIDico.
   mtUWORD          IGC;               // Utilis‚ par le ramasse-miettes.
   mtUWORD          NbISym;            // Nombre de symboles internes.
   mtUWORD          nPageI;            // Nombre de pages de la pile des symboles internes.
   mtBOOL           ExecError;         // VRAI si une erreur d'ex‚cution survient.
   mtWORD           CodeError;         // Code de l'erreur d'ex‚cution.
   mtBYTE           Row;               // Position courante
   mtBYTE           Col;               // du curseur.
   mtBOOL           bGCursor;          // Vrai si on a re‡u la position du curseur.
   mtPTRBYTE        pLInput;           // Ligne en cours de saisie.
   mtPTRBYTE        pUsing;            // Format d'affichage des nombres.
   CODE4            CBase;             // Structure CODEBASE.
   DATA4            *CurrBase;         // Base courante.
   int              DbError;           // DerniŠre CODEBASE erreur survenue.
   mtBOOL           bLFKAct;           // VRAI si la gestion des touches locales est active.
   mtBOOL           bGFKAct;           // VRAI si la gestion des touches glocales est active.
   mtBOOL           bLKWAct;           // VRAI si la gestion des mot-clef locaux est active.
   mtBOOL           bGKWAct;           // VRAI si la gestion des mot-clef globaux est active.
   mtPTRTFKEY       pLTabFKey;         // Pointe sur la table locale des touches de fonction … intercepter.
   mtPTRTKEYW       pLTabKeyW;         // Pointe sur la table locale des mot-clefs … intercepter.
   mtBOOL           bKeyWOk;           // VRAI si un mot-clef est trouv‚ dans une des tables.
   mtBOOL           bFKeyOk;           // VRAI si une touche est trouv‚e dans une des tables.
   struct dos_time_t HDebExec;         // Heure de d‚but d'ex‚cution du code.
   int              TimeExec;          // Nombre de ms allou‚es au cv courant pour l'ex‚cution de son code.
   mtWORD           NbIPS;             // Nombre d'instructions ex‚cut‚e pendant TimeExec.
   mtWORD           MaxIPS;            // Pointe d'IPS.
   float            HDelay;            // Utilis‚e par la fonction DELAY.
   float            dDelay;            //     "     "   "     "      "
   mtUWORD          ALigSrc;           // Indice courant dans
   mtUWORD          AColSrc;           // ACOPY.
   mtPTRDICO        ATabSrc;           // Tableau source dans ACOPY.
   mtPOINTEUR       AValSrc;           // Valeur source.
   mtUWORD          ALigDst;           // Indice courant dans
   mtUWORD          AColDst;           // AFILL/ASEARCH/ACOPY/ACLONE/STRTOK/ADIR.
   mtPTRDICO        ATabDst;           // Tableau donn‚ dans AFILL/ASEARCH/ACOPY/ACLONE/STRTOK/ADIR.
   mtPOINTEUR       AValDst;           // Valeur donn‚e dans AFILL/ASEARCH.
   float            AfValDst;          // Valeur donn‚e dans AFILL/ASEARCH.
   pPOINTEUR        OldTab;            // Utilis‚es dans AINS.
   mtBYTE           OTType;            // Type de OldTab.
   mtUWORD          MaxLi;             // Utilis‚ dans ADEL/ACLONE.
   mtUWORD          MaxCo;             // Utilis‚ dans ACLONE.
   mtUWORD          ALigDel;           // Indice courant utilis‚ dans
   mtUWORD          AColDel;           // DeleteTab (ACLONE/STRTOK/ADIR).
   mtPTRBYTE        pChaine;           // Utilis‚s
   mtPTRBYTE        pDelim;            // dans STRTOK.
   mtPTRDICO        pSInput;           // Symbole
   mtPTRDICO        pSILig;            // utilis‚ par
   mtPTRDICO        pSICol;            // INPUTVAR
   struct find_t    sFIND;             // Utilis‚e par ADIR.
   mtPTRBYTE        pMask;             // Utilis‚ par ADIR.
   mtUWORD          uFind;             // Utilis‚ par ADIR.
   mtBOOL           bACMIT;            // Utilis‚es pour le raffraichissement
   mtUDWORD         TpACMIT;           // p‚riodique des bases de donn‚es.
   mtWORD           MLIMaxLine;        // Utilis‚ par MLInput (nbr. de lignes max).
   mtWORD           MLILine;           //     "    "     "    (ligne courante).
   mtWORD           MLILSize;          //     "    "     "    (largeur d'une ligne).
   pPOINTEUR        MLITabLine;        //     "    "     "    (tableau des lignes).
   mtBYTE           MLIRow;            //     "    "     "    (Position courante
   mtBYTE           MLICol;            //     "    "     "     du curseur).
   mtWORD           fHandle;           // Handle du fichier texte ouvert.
   mtDWORD          fCurrPos;          // Position courante dans le fichier texte.
   mtBOOL           fEof;              // VRAI si on est … la fin du fichier texte.
   mtBOOL           bNoTrAcc;          // VRAI si on d‚sactive la translation des accents.
   mtBOOL           bNoSeqEsc;         // VRAI si on d‚sactive le filtrage des s‚quences escape.
   mtBOOL           bBinMode;          // VRAI si on est en mode binaire (utilis‚ pour le t‚l‚chargement, entre autre).
   mtBOOL           bWaitChar;         // VRAI si on attend le caractŠre WaitChar.
   mtBYTE           WaitChar;          // CaractŠre en attente.
} UnCv;

typedef UnCv * mtPTRCV;
#define mtNULLCV (mtPTRCV) 0

#endif

