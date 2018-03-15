/*
** ----------------------------------------------------------------------------
** Gnu Xml Engine
** philippe Guillaumaud <pguillaumaud@april.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
**
** D�finitions des donn�es globales � l'application
*/
#ifndef S_GLOBALS_H
#define S_GLOBALS_H

#ifdef S_ALLOCATE
  #define S_PUBLIC
#else
  #define S_PUBLIC  extern
#endif

/*
** ----------------------------------------------------------------------------
*/
/*
** D�finition d'un canal de communication serveur<>client
*/
#define   _FxBUFF_SIZE   1024

typedef enum
{
  FXS_MONOCLIENT,
  FXS_FRONTAL,
  FXS_UNDEF
} _FxStreamType;

typedef enum
{
  OM_XML,
  OM_ASCII,
  OM_UNDEF
} _FxOutputMode;

typedef enum
{
  CHLD_READ_SOCKET,
  CHLD_PARSE_REQUEST,
  CHLD_RUN_CMD,
  CHLD_UNDEF
} _FxChildState;

/* Attribut des balises */
struct _FxAttr
{
  gchar *name;                                    /* Nom de l'atribut */
  gchar *value;                                   /* Valeur */
};

typedef struct _FxAttr *  _FxAttrPtr;

struct _FxStream
{
  _FxStreamType      type;                        /* Type du canal */
  _FxChildState      state;                       /* Etat courant */
  gint               sock;                        /* Socket attach�e au canal */
  struct sockaddr_in adresse;                     /* IP de la socket */
  FILE               *fp;                         /* Associ� au socket pour faciliter les I/O sur chaines */
  gchar              buff[_FxBUFF_SIZE+1];        /* Buffer de lecture */
  gint               nbrq;                        /* Nombre de requ�tes trait�es */
  GString            *Requete;                    /* Requ�te re�ue sur le canal */
  GSList             *LAttr;                      /* Liste des attributs de la balise racine (FxML) */
  _FxOutputMode      omode;                       /* Format de sortie du r�sultat (XML ou ASCII brut) */
  xmlDocPtr          Out;                         /* Buffer de sortie xml */
  GString            *strOut;                     /* Bufer de sortie ascii */
};

typedef struct _FxStream *  _FxStreamPtr;

/*
** ----------------------------------------------------------------------------
** Traitement des commandes
*/
#define FX_RETURN_SUCCESS 0
#define FX_RETURN_ERROR   -1

/* D�finition de la table des commandes */
struct _FxCmd
{
  gchar   *name;                                  /* Nom de la commande */
  GModule *pmodule;                               /* pointe sur le module qui contient la commande */
};

typedef struct _FxCmd *  _FxCmdPtr;

/* Param�tres d'une commande */
typedef enum
{
  T_SINGLE,                                       /* Type simple (type par d�faut) */
  T_LIST,                                         /* Type liste (contient des sous-balises) */
  T_UNDEF
} _FxParamType;

typedef _FxParamType FxParamType;                 /* Type utilis� par les API */

struct _FxParam
{
  gchar        *name;                             /* Nom du param�tre courant */
  GSList       *LAttr;                            /* Liste des �ventuels attributs du param�tre courant */
  _FxParamType type;                              /* Type du param�tre courant */
  union _v
  {
    GString    *value;                            /* Valeur si type=T_SINGLE */
    GSList     *LParam;                           /* Valeur si type=T_LIST */
  } v;
};

typedef struct _FxParam *  _FxParamPtr;
typedef struct _FxParam *  FxParameterPtr;        /* Type utilis� par les API */

/* D�finition du contexte d'appel d'une commande */
struct _CallCmd
{
  _FxCmdPtr  cmd;                                 /* Pointe sur les infos de la commande appel�e */
  GSList     *LAttr;                              /* Liste des �ventuels attributs de la commande */
  GSList     *LParam;                             /* Liste des �ventuels param�tres de la commande */
  xmlNodePtr FirstLevel;                          /* Premier niveau d'indentation xml */
  xmlNodePtr CurrentLevel;                        /* Niveau courrant d'indentation xml */
};

typedef struct _CallCmd * _CallCmdPtr;
typedef struct _CallCmd * FxCommandPtr;           /* Type utilis� par les API */


/*
** ----------------------------------------------------------------------------
** Codes d'erreurs de l'API
*/
#define   FX_ERR_NOPARAM      100
#define   FX_ERR_NOMEM        101
#define   FX_ERR_NOSESSION    102
#define   FX_ERR_SESSIONEXIST 103


/*
** ----------------------------------------------------------------------------
** Traitement des appels entrants
*/
#define _FxEND_RQ        "\n\n"                   /* Marqueur de fin de requ�te */
#define S_TIMEOUT_CALL   5                        /* Timeout de 5 secondes */
#define S_RETRY_CALL     6                        /* On essaie n fois de lire la socket */


/*
** ----------------------------------------------------------------------------
** Traitement des modules dynamiques
*/
/* Informations sur les modules charg�s */
struct _Module
{
  gchar   *name;                                  /* Nom du module (utilis� dans l'attribut 'module=...' des balises cmd */
  time_t  mtime;                                  /* Heure de derni�re modification au moment du chargement du module */
  GModule *pmodule;                               /* pointeur sur la structure utilis�e par les fonctions g_module... */
};

typedef struct _Module *_ModulePtr;

#define  S_MODULE_PATH   "/modules"               /* R�pertoire contenant les modules � charger */
#define  S_MODULE_EXT    ".so"                    /* Extension des modules dynamiques */

/*
** ----------------------------------------------------------------------------
** Outils de gestion des sockets
*/
#define   SERV_NAME      "gxe"
#define   SERV_PORT      40000

/*
** ----------------------------------------------------------------------------
** Parseur de requ�tes XML
*/
#define   S_FXML_START   "FxML"                   /* Balise de d�but des commandes */
#define   S_FXML_VERSION "1.0"

/* Erreurs */
typedef enum
{
  XML_SUCCESS = 0,
  XML_ERR_CMD_NOT_FOUND,
  XML_ERR_PARAM_FOUND,
  XML_ERR_EXEC_CMD,
  XML_ERR_INTERNAL
} _ParserError;

/* Etats du parseur */
typedef enum
{
  PARSER_START,
  PARSER_FINISH,
  PARSER_CMD,
  PARSER_CMD_ERROR,
  PARSER_CMD_PARAM,
  PARSER_CMD_PARAM_ERROR,
  PARSER_ERROR,
  PARSER_UNKNOWN
} _ParserState;

/* Contexte du parsing courant */
struct _swParserState
{
  _ParserState state;                             /* Etat courant */
  _ParserState prev_state;                        /* Etat pr�c�dent */
  gint         udepth;                            /* Profondeur de parsing sur tag inconnu */
  gint         errors;                            /* Nombre d'erreurs */
  gchar        *tagerror;                         /* Nom de la balise en erreur */
  gint         xmlerrno;                          /* Code d'erreur du parser */
  gchar        *xmlerror;                         /* Msg d'erreur du parser */
  _CallCmdPtr  currentcmd;                        /* Contexte de la commande courante */
  _FxParamPtr  currentparam;                      /* Param�tre en cours de parsing */
  GSList       *stackParam;                       /* Pile des param�tres */
  gpointer     Child;                             /* Pointe sur le fils propri�taire du parser (utilis� dans le parser xml) */
};
typedef struct _swParserState *_swParserStatePtr;


/*
** ----------------------------------------------------------------------------
** Traitement des erreurs et messages divers
*/
#define   S_LOG_IDENT      "gxe"
#define   S_LOG_OPTIONS    (LOG_PID|LOG_CONS|LOG_NOWAIT|LOG_PERROR)
#define   S_LOG_FACILITY   LOG_LOCAL4


/*
** ----------------------------------------------------------------------------
** Ex�cution des commandes
*/
/* Pointeur de fonction g�n�rique */
typedef gint (*_RunCmdPtr)(_CallCmdPtr);


/*
** ----------------------------------------------------------------------------
** Traitement des r�sultats des commandes
*/
#define FX_FIRSTLEVEL      0
#define FX_CURRENTLEVEL    1
#define FX_PREVIOUSLEVEL   2


/*
** ----------------------------------------------------------------------------
** Traitement des sessions
*/
struct _FxSession
{
  gchar        *frontalname;                      /* Nom du frontal associ� */
  gchar        *sessionID;                        /* Identificateur de la session */
};

typedef struct _FxSession *  _FxSessionPtr;


/*
** ----------------------------------------------------------------------------
** Traitement des process fils
*/
struct _FxChild
{
  pthread_t         thr_id;                       /* ID du fils */
  _FxStreamPtr      Stream;                       /* Canal associ� */
  _swParserStatePtr Parser;                       /* Parser xml associ� */
};

typedef struct _FxChild *  _FxChildPtr;

/*
** ============================================================================
** VARIABLE GLOBALES
*/

#define FX_FILE_CFG        "/etc/gxe.conf"        /* Fichier de configuration */
#define FX_MAX_REQUEST     100                    /* D�faut du Nombre maxi de requ�tes sur un canal avant fermeture */

S_PUBLIC  GSList                *_TFxCmds;        /* Table des commandes */
S_PUBLIC  pthread_mutex_t       *mutex_TCmd;      /* Verrou de la table  */

S_PUBLIC  GSList                *_TModules;       /* Table des modules dynamiques pr�sents */
S_PUBLIC  pthread_mutex_t       *mutex_TModules;  /* Verrou de la table  */

S_PUBLIC  GSList                *_TChilds;        /* Table des fils actifs */
S_PUBLIC  pthread_mutex_t       *mutex_TChilds;   /* Verrou de la table  */

S_PUBLIC  _FxSessionPtr         _swSession;       /* Session courante */

S_PUBLIC  gchar                 *_ModulesPath;    /* R�pertoire contenant les modules � charger */
S_PUBLIC  gchar                 *_CurrentDir;     /* R�pertoire de travail courant */
S_PUBLIC  pid_t                 _swPid;           /* PID de gxe  */
S_PUBLIC  gint                  _swPort;          /* Port d'�coute */
S_PUBLIC  gint                  _swMaxRQ;         /* Nombre maxi de requ�tes sur un canal avant fermeture */
S_PUBLIC  gint                  _swRetryMax;      /* Nombre maxi de tentatives de lecture d'un canal */
S_PUBLIC  gint                  _swTimeoutRQ;     /* Timeout sur lecture d'un canal (en secondes) */

/*
** ----------------------------------------------------------------------------
*/
#include "s_cmd.h"
#include "s_incomming.h"
#include "s_modules.h"
#include "s_sockets.h"
#include "s_xmlparse.h"
#include "s_syslog.h"
#include "s_signaux.h"
#include "s_misc.h"
#include "s_out.h"
#include "s_runcmd.h"
#include "s_api.h"
#include "s_session.h"
#include "s_process.h"
#include "s_xmlattr.h"

#endif
/*
** ----------------------------------------------------------------------------
*/
