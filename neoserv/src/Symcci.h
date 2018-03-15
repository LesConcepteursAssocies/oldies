// ÕÍÍÍÍÍÍÍÍÍÍÍÍÍÑÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµNEOSERVÆÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¸
// ³SYMCCI.H     ³                                                             ³
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
// ³D‚finitions des donn‚es relatives … la gestion des cartes MITEC.           ³
// ÔÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¾

#if !defined(__SYMCCI_H)
#define _SYMCCI_H

//
// DEFINITION DES ARGUMENTS STANDARD D'IOCTL
//
#define   IOC_SDAT         0         // Ecriture des donn‚es de controle
#define   IOC_GDAT         1         // Lecture des donn‚es de controle
#define   IOC_GSTC         2         // Lecture des donn‚es de statistique

#define   IOC_WALT         4         // Programmation des alertes

/*
** DEFINITION DU STATUS CANAL
*/
                                 /* Sur IOSTA, INPUT_STA, et OUTPUT_STA      */
                                 /*----------------------------------------*/
#define   S_CNX         0x4000      /* Canal connect‚                           */
#define   S_WCNX      0x2000      /* Connexion sortante en cours            */
#define   S_WDCNX      0x1000      /* Deconnexion sortante en cours            */
#define   S_APE         0x0800      /* Connexion entrante en cours            */

                                 /* Sur IOSTA, raz aprŠs lecture            */
                                 /*----------------------------------------*/
#define   S_DLTCNX      0x0080      /* Modification de S_CNX                  */
#define   S_BREAK      0x0008      /* Break ou interruption                  */
#define   S_REINI      0x0004      /* R‚initialisation                        */

/*
** DEFINITION DU STATUS D'ENTREES-SORTIES
*/
#define   S_EMPTY      0x0001      /* Vide                                    */
#define   S_FULL      0x0002      /* Plein                                    */

#define   S_STOP      0x0010      /* ArrŠter la transmission (OUTPUT_STA)   */

/*
** DEFINITION DU MASQUE DE PROGRAMMATION DES ALERTES
*/
#define   A_TXEMP      1            /* Masque alerte transmission vide         */
#define   A_RXRDY      2            /* Masque alerte reception non vide         */
#define   A_STACH      4            /* Masque alerte changement de status      */
#define   A_TXSTA      0x10         /* Masque alerte changement S_STOP         */

/*
** DEFINITON DES CODES D'ALERTE
*/
#define   ALT_TXE      0            /* Code alerte transmission vide            */
#define   ALT_RXR      1            /* Code alerte reception non vide         */
#define   ALT_SCH      2            /* Code alerte changement de status         */

/*
** DEFINITON DE L'ARGUMENT DE LECTURE-ECRITURE
*/
#define   ARG_DAS      0x10         /* Donn‚es … suivre                        */
#define   ARG_BEB      0x20         /* Bout-en-bout                           */
#define   ARG_QDD      0x80         /* Qualificateur de donn‚es               */

/*-------------------------------------------------------------------------*
 :                 STRUCTURE RETOURNEE PAR CCIinfo()                        :
 *-------------------------------------------------------------------------*/

typedef struct cci_info
{
   mtBYTE      inf_typ;            /* Type de la carte                           */
   mtBYTE      inf_upc;            /* Vitesse processeur                        */
   mtBYTE      inf_mem;            /* M‚moire total en multiple de 64 Ko         */
   mtBYTE      inf_ver;            /* Version logiciel                           */
   mtBYTE      inf_rev;            /* R‚vision logiciel                           */
   mtBYTE      inf_hst;            /* Nombre total de canaux                     */
   mtBYTE      inf_sci;            /* Nombre total de liaison s‚rie               */
   mtBYTE      inf_rsv[16-7-2];
} info_t;

/*
** DEFINITION DE INF_TYP
*/
#define   _CX25         1      /* Carte CX25                                 */
#define   _C8LSI      2      /* Carte C8LSI                                 */

/*
** DEFINITION DE INF_UPC
*/
#define   _UPC_8         1      /* Carte … 8 MHz                              */
#define   _UPC_10         2      /* Carte … 10 MHz                              */
#define   _UPC_12         3      /* Carte … 12 MHz                              */
#define   _UPC_16         4      /* Carte … 16 MHz                              */
#define   _UPC_20         4      /* Carte … 20 MHz                              */

/*-------------------------------------------------------------------------*
 :                 STRUCTURE RETOURNEE PAR CCIdrvstc()                        :
 *-------------------------------------------------------------------------*/

typedef struct cci_stat
{
   mtUDWORD      cnt_open;         /* Compte d'appel … CCIopen()                  */
   mtUDWORD      cnt_close;         /* Compte d'appel … CCIclose()               */
   mtUDWORD      cnt_iosta;         /* Compte d'appel … CCIiosta()               */
   mtUDWORD      cnt_ioctl;         /* Compte d'appel … CCIioctl()               */
   mtUDWORD      cnt_output;         /* Compte d'appel … CCIoutput()               */
   mtUDWORD      cnt_output_sta;   /* Compte d'appel … CCIoutput_sta()            */
   mtUDWORD      cnt_output_flh;   /* Compte d'appel … CCIoutput_flh()            */
   mtUDWORD      cnt_input;         /* Compte d'appel … CCIinput()               */
   mtUDWORD      cnt_input_sta;      /* Compte d'appel … CCIinput_sta()            */
   mtUDWORD      cnt_input_flh;      /* Compte d'appel … CCIinput_flh()            */
   mtUDWORD      cnt_req;            /* Compte d'appel … la carte                  */
   mtUDWORD      cnt_dif;            /* Compte d'appel trait‚s par une tƒche      */
} stat_t;

/*-------------------------------------------------------------------------*
 :                              DEFINITION DE ERRNO                           :
 *-------------------------------------------------------------------------*/

                              /* Erreurs retourn‚es par la carte            */
                              /*-------------------------------------------*/
#define   CCI_NOERROR      0      /* Ex‚cution sans erreur de la fonction      */
#define   CCIER_OFF      1      /* Offset requˆte incorrect                  */
#define   CCIER_SIZ      2      /* Longueur requˆte incorrecte               */
#define   CCIER_FCT      3      /* Code fonction inconnu                     */
#define   CCIER_ARG      4      /* Argument d'IOCTL inconnu                  */   
#define   CCIER_CNL      5      /* Num‚ro de canal inconnu                     */
#define   CCIER_CFL      6      /* Conflit, requˆte en cours de traitement   */
#define   CCIER_EVT      7      /* Pile d'evenements satur‚e                  */
#define   CCIER_OUT      8      /* Les donn‚es ont ‚t‚ d‚truites               */
#define   CCIER_NAM      9      /* Nom inconnu                                 */
#define   CCIER_EXE      10      /* Erreur g‚n‚rale d'execution               */
#define   CCIER_MEM      11      /* Resource m‚moire                           */
#define   CCIER_RSC      12      /* Resource systŠme                           */
#define   CCIER_DRV      13      /* AccŠs driver                              */
#define   CCIER_SYN      14      /* Syntaxe                                    */
#define   CCIER_PRM      15      /* ParamŠtre                                 */
#define   CCIER_VAL      16      /* Valeur                                    */
#define   CCIER_ENV      17      /* Environnement                              */
#define   CCIER_CHN      18      /* Fin de chaine                              */

                              /* Erreurs retourn‚es par le driver            */
                              /*-------------------------------------------*/
#define   DRVER_IRQ      -1      /* Pas de r‚ponse du driver de la carte      */
#define   DRVER_TSK      -2      /* Pas de r‚ponse de la tƒche                  */
#define   DRVER_SIZ      CCIER_SIZ + 128   /* Longueur message trop grande   */
#define   DRVER_CNL      CCIER_CNL + 128   /* Nø de canal > inf_hst         */
#define   DRVER_FCT      CCIER_FCT + 128   /* Code fonction inconnu         */

                              /* Erreurs retourn‚es par :                  */
                              /*      CCI_init(), CCI_info(), CCI_stat()      */
                              /*-------------------------------------------*/
#define   INIER_IRQ      -1      /* Pas de r‚ponse du driver de la carte      */
#define   INIER_TSK      -2      /* Pas de r‚ponse de la tƒche systŠme         */
#define   INIER_MEM      -3      /* Pas de m‚moire dans le segment sp‚cifi‚   */
#define   INIER_FIC      -4      /* Erreur accŠs fichier de config            */
#define   INIER_CFG      -5      /* Erreur format fichier de config            */
#define   INIER_SEL      -6      /* Plus de selecteur disponible               */

/*
** STRUCTURE GENERALE D'UNE REQUETE
*/
typedef struct cci_req
{
   short   r_siz;      /* Longueur totale      */
   mtBYTE   r_dev;      /* Numero de canal      */
   mtBYTE   r_fct;      /* Fonction … executer   */
   mtBYTE   r_arg;      /* Argument               */
   mtBYTE   r_sta;      /* Status requete         */
   mtUSHORT   r_ios;      /* Status canal         */
} req_t;

/*
** STRUCTURE D'UNE REQUETE DE STATUS D'ENTREES-SORTIES
*/
typedef struct cci_rio
{
   short   r_siz;      /* Longueur totale      */
   mtBYTE   r_dev;      /* Numero de canal      */
   mtBYTE   r_fct;      /* Fonction … executer   */
   mtBYTE   r_arg;      /* Argument               */
   mtBYTE   r_sta;      /* Status requete         */
   mtUSHORT   r_ios;      /* Status canal         */
   short   r_cnt;      /* Compte de caractere   */
} rio_t;

/*
** STRUCTURE D'UNE ALERTE
*/
typedef struct cci_alt
{
   short   r_siz;      /* Longueur totale      */
   mtBYTE   r_dev;      /* Numero de canal      */
   mtBYTE   r_fct;      /* Fonction … executer   */
   mtBYTE   r_arg;      /* Argument               */
   mtBYTE   r_sta;      /* Status requete         */
   mtUSHORT   r_ios;      /* Status canal         */
   char   r_alt;      /* Code de l'alerte      */
} alt_t;

/*
** CODAGE DE 'R_FCT'
*/
#define   ROPEN            0
#define   RCLOSE         1
#define   RIOSTA         2
#define   RIOCTL         3
#define   ROUTPUT         4
#define   ROUTPUT_STA      6
#define   ROUTPUT_FLH      7
#define   RINPUT         8
#define   RINPUT_STA      10
#define   RINPUT_FLH      11

//
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ CCICFG.H ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
//

typedef struct
{
   /*
   ** PARAMETRES CARTE
   **
   */
   unsigned short   X25_Adr;
   unsigned char   X25_Irq;
   unsigned char   X25_Int;
   unsigned char   X25_Ini;
   unsigned char   X25_Rsv[8-5];

   /*
   ** PAD ASYNCHRONE
   **
   */
   unsigned short   PAD_Baud;
   unsigned char   PAD_Par;
   unsigned char   PAD_Bit;
   unsigned char   PAD_Stp;
   unsigned char   PAD_Mode;
   unsigned char   PAD_Term;
   unsigned char   PAD_Rsv;

   /*
   ** X25 NIVEAU 1
   **
   */
   unsigned short   XN1_Baud;
   unsigned char   XN1_Clock;
   unsigned char   XN1_Rsv[8-3];

   /*
   ** X25 NIVEAU 2
   **
   */
   unsigned char   XN2_Mode;
   unsigned char   XN2_FenRec;
   unsigned char   XN2_FenEmi;
   unsigned char   XN2_Repetition;
   unsigned short   XN2_TimeOut;
   unsigned char   XN2_Rsv[8-6];

   /*
   ** X25 NIVEAU 3
   **
   */
   unsigned char   XN3_NbCV;
   unsigned char   XN3_PremCV;
   unsigned char   XN3_LenPaq;
   unsigned char   XN3_FenRec;
   unsigned char   XN3_FenEmi;
   unsigned char   XN3_RepAuto;
   unsigned short   XN3_TimeOut1;
   unsigned short   XN3_TimeOut2;
   unsigned char   XN3_Proto;
   unsigned char   XN3_Rsv1;
   unsigned char   XN3_Rsv2[16-12];

} cfg_t;

#define   CCI_EXT      'E'
#define   CCI_INT      'I'

#define   CCI_ETTD      0
#define   CCI_ETCD      1

#define   CCI_DIRECT   0
#define   CCI_ASM      1

#define   CCI_VDTX      0
#define   CCI_ASCII   1
#define   CCI_TRANSP   2

#define   CCI_LOCAL   0
#define   CCI_MODEM   1

#define   CCI_NON      'N'
#define   CCI_OUI      'O'

typedef  unsigned char __far *LPXPTR;
typedef  info_t              *LPINFO;
typedef  req_t               *LPREQ;
typedef  rio_t               *LPRIO;
typedef  stat_t              *LPSTAT;

//
// D‚finition des prototypes de SYMCCI.C.
//
mtBOOL CCIInitCartes(void);
void CCIEndCartes(void);
int CCIopen(int);
int CCIclose(int, int, mtPTRBYTE);
int CCIiosta(int, mtPTRWORD);
int CCIioctl(int, int, mtPTRUSHORT, mtPTRBYTE);
int CCIoutput(int, int, mtPTRBYTE, mtBYTE);
int CCIoutput_sta(int, mtPTRUSHORT, mtPTRWORD);
int CCIoutput_flh(int);
int CCIinput(int, mtPTRWORD, mtPTRBYTE, mtPTRBYTE);
int CCIinput_sta(int, mtPTRWORD, mtPTRWORD);
int CCIinput_flh(int);
int CCIinfo(int, LPINFO);
int CCIstat(int, LPSTAT, int);

#endif

