;
; VARIABLES
;

MOT@          TOS,101b,3,"TO","S",PAR_VARIABLE
              dw        0

MOT@          EOF_INT,101b,4,"EOF","?",PAR_VARIABLE
$EOF          dw        0

MOT@          HANDLE,101b,6,"HANDL","E",PAR_VARIABLE
              dw        0

MOT@          ECHO,101b,4,"ECH","O",PAR_VARIABLE
              dw        0

MOT@          STATE,101b,5,"STAT","E",PAR_VARIABLE
              dw        0

MOT@          CSP,101b,3,"CS","P",PAR_VARIABLE
              dw        0

MOT@          DP,101b,2,"D","P",PAR_VARIABLE
              dw        @HERE

MOT@          WARNING,101b,7,"WARNIN","G",PAR_VARIABLE
              dw        1

MOT@          IBASE,101b,5,"IBAS","E",PAR_VARIABLE
              dw        10

MOT@          OBASE,101b,5,"OBAS","E",PAR_VARIABLE
              dw        10

MOT@          DPL,101b,3,"DP","L",PAR_VARIABLE
              dw        0

MOT@          HLD,101b,3,"HL","D",PAR_VARIABLE
              dw        0

MOT@          CAPS,101b,4,"CAP","S",PAR_VARIABLE
$CAPS         dw        1

MOT@          CURRENT,101b,7,"CURREN","T",PAR_VARIABLE
              dw        _FORTH+2

MOT@          CONTEXT,101b,7,"CONTEX","T",PAR_VARIABLE
              dw        _FORTH+2,0,0,0,0,0,0,0

MOT@          VOC_LINK,101b,8,"VOC-LIN","K",PAR_VARIABLE
              dw        @FINVOC

MOT@          LAST,101b,4,"LAS","T",PAR_VARIABLE
              dw        @FINLFA+2

MOT@          PRIOR,101b,5,"PRIO","R",PAR_VARIABLE
              dw        ?

MOT@          END_INT,101b,4,"END","?",PAR_VARIABLE
              dw        ?

MOT@          WIDTH,101b,5,"WIDT","H",PAR_VARIABLE
              dw        31

MOT@          SPAN,101b,4,"SPA","N",PAR_VARIABLE
              dw        ?

MOT@          SHA_OUT,101b,4,"#OU","T",PAR_VARIABLE
$OUT          dw        0

MOT@          SHA_LINE,101b,5,"#LIN","E",PAR_VARIABLE
$LINE         dw        0

MOT@          PRINTING,101b,8,"PRINTIN","G",PAR_VARIABLE
$PRINTING     dw        0

MOT@          COLOR_INT,101b,6,"COLOR","?",PAR_VARIABLE
              dw        0

MOT@          SHA_COLOR,101b,6,"#COLO","R",PAR_VARIABLE
              dw        07

MOT@          TICK_TIB,101b,4,"'TI","B",PAR_VARIABLE
              dw        _BUFFERS+3

MOT@          SUP_IN,101b,3,">I","N",PAR_VARIABLE
              dw        0

MOT@          ATTRIBUTS,101b,9,"ATTRIBUT","S",PAR_VARIABLE
              dw        0

MOT@          STATUS_INT,101b,7,"STATUS","?",PAR_VARIABLE
              dw        0

MOT@          SP0,101b,3,"SP","0",PAR_VARIABLE
              dw        0FFFEh

MOT@          RP0,101b,3,"RP","0",PAR_VARIABLE
              dw        07FFEh

MOT@          C_C,101b,3,"C-","C",PAR_VARIABLE
              dw        _C_C_F+2

MOT@          FENCE,101b,5,"FENC","E",PAR_VARIABLE
              dw        @FINLFA

;
; CONSTANTES
;

MOT@          SHA_TIB,101b,4,"#TI","B",PAR_CONSTANT
              dw        _BUFFERS+2

MOT@          FILES,101b,5,"FILE","S",PAR_CONSTANT
$FILES        dw        20

MOT@          SHA_FILES,101b,6,"#FILE","S",PAR_CONSTANT
$SHA_FILES    dw        5

MOT@          LBUF,101b,4,"LBU","F",PAR_CONSTANT
              dw        256

MOT@          CPATH,101b,6,"C/PAT","H",PAR_CONSTANT
              dw        64

MOT@          READ,101b,4,"REA","D",PAR_CONSTANT
              dw        0

MOT@          WRITE,101b,5,"WRIT","E",PAR_CONSTANT
              dw        1

MOT@          READ_WRITE,101b,10,"READ-WRIT","E",PAR_CONSTANT
              dw        2

MOT@          SHA_VOCS,101b,5,"#VOC","S",PAR_CONSTANT
              dw        8

MOT@          SHA_THREADS,101b,8,"#THREAD","S",PAR_CONSTANT
              dw        4

MOT@          BL,101b,2,"B","L",PAR_CONSTANT
              dw        " "

MOT@          BS,101b,2,"B","S",PAR_CONSTANT
              dw        8

MOT@          BELL,101b,4,"BEL","L",PAR_CONSTANT
              dw        7

MOT@          CBYL,101b,3,"C/","L",PAR_CONSTANT
              dw        255

MOT@          PAD,101b,3,"PA","D",PAR_CONSTANT
              dw        _$PAD+257

MOT@          STDIN,101b,5,"STDI","N",PAR_CONSTANT
$STDIN        dw        0

MOT@          STDOUT,101b,6,"STDOU","T",PAR_CONSTANT
$STDOUT       dw        1

MOT@          AUXIN,101b,5,"AUXI","N",PAR_CONSTANT
$AUXIN        dw        3

MOT@          AUXOUT,101b,6,"AUXOU","T",PAR_CONSTANT
$AUXOUT       dw        3

MOT@          STDPRN,101b,6,"STDPR","N",PAR_CONSTANT
$STDPRN       dw        4

MOT@          TIB,101b,3,"TI","B",PAR_CONSTANT
              dw        _BUFFERS+3

;
; TABLEAUX
;

MOT@          C_C_F,101b,5,"C-C-","F",PAR_CREATE
dw            _NOOP    ,_NOOP ,_NOOP ,_NOOP ,_NOOP ,_NOOP    ,_NOOP ,_BEEP
dw            _DEL_IN  ,_NOOP ,_NOOP ,_NOOP ,_NOOP ,_CR_IN   ,_NOOP ,_NOOP
dw            _NOOP    ,_NOOP ,_NOOP ,_NOOP ,_NOOP ,_BACK_UP ,_NOOP ,_NOOP
dw            _BACK_UP ,_NOOP ,_NOOP ,_NOOP ,_NOOP ,_NOOP    ,_NOOP ,_NOOP

MOT@          $PAD,101b,4,"$PA","D",PAR_CREATE
              db        256 dup(0)

MOT@          ERRMSG,101b,6,"ERRMS","G",PAR_CREATE
STR@          49,<"num‚ro de fonction invalide                      ">
STR@          49,<"fichier introuvable                              ">
STR@          49,<"chemin introuvable                               ">
STR@          49,<"trop de fichiers ouverts                         ">
STR@          49,<"accŠs refus‚                                     ">
STR@          49,<"num‚ro de fichier invalide                       ">
STR@          49,<"blocs de contr“le de m‚moire d‚truits            ">
STR@          49,<"m‚moire insuffisante                             ">
STR@          49,<"adresse de bloc m‚moire invalide                 ">
STR@          49,<"environnement invalide                           ">
STR@          49,<"format invalide                                  ">
STR@          49,<"mode d'accŠs invalide                            ">
STR@          49,<"donn‚es invalides                                ">
STR@          49,<"unit‚ invalide                                   ">
STR@          49,<"tentative de suppression du r‚pertoire en cours  ">
STR@          49,<"pas la mˆme unit‚                                ">
STR@          49,<"plus de fichier                                  ">
STR@          49,<"disquette prot‚g‚e en ‚criture                   ">
STR@          49,<"unit‚ de disque invalide                         ">
STR@          49,<"unit‚ non prˆte                                  ">
STR@          49,<"commande de disque invalide                      ">
STR@          49,<"erreur de CRC                                    ">
STR@          49,<"longueur invalide (disque)                       ">
STR@          49,<"erreur de recherche (seek sur disque)            ">
STR@          49,<"disque non DOS                                   ">
STR@          49,<"secteur introuvable                              ">
STR@          49,<"plus de papier                                   ">
STR@          49,<"erreur d'‚criture                                ">
STR@          49,<"erreur de lecture                                ">
STR@          49,<"d‚faillance g‚n‚rale                             ">
STR@          49,<"problŠme de partage de fichier                   ">
STR@          49,<"problŠme de verrouillage                         ">
STR@          49,<"mauvais disque                                   ">
STR@          49,<"pas de FCB                                       ">
STR@          49,<"erreur DOS                                       ">


MOT@          DTA,101b,3,"DT","A",PAR_CREATE
              db        512 dup(0)

MOT@          BUFFERS,101b,7,"BUFFER","S",PAR_CREATE
              db        256 dup(0)
              db        4, "CON:", 59 dup(0)
              dw        0
              db        256 dup(0)
              db        4, "CON:", 59 dup(0)
              dw        0
              db        256 dup(0)
              db        4, "CON:", 59 dup(0)
              dw        0
              db        256 dup(0)
              db        5, "COM1:", 58 dup(0)
              dw        0
              db        256 dup(0)
              db        4, "PRN:", 59 dup(0)
              dw        0
              db        15 dup(322 dup(0))

MOT@          PROMPT,101b,6,"PROMP","T",PAR_CREATE
              db        20
              db        2
              db        "Ok"
              db        18 dup(0)

;
; FIN DES VARIABLES
;

