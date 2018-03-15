\ -----------------------------------------------------------------------------
\                                Utilitaire.
\
\ Routines de gestion de la m‚moire EMS.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? INT@ NOT ?\ INCLUDE MSDOS

HEX

\ Recherche d'une chaŒne inter-segments.
\ (extrait de ED_LIB.FTH, par M. ZUPAN)
\
ASSEMBLER LABEL >UPPER
  ASCII a # AL CMP  0>=
  IF
    ASCII z 1+ # AL CMP
    0<
    IF
      20 # AL SUB
    THEN
  THEN
RET

LABEL TEMPORARY NOP

\ Renvoi VRAI et l'offset si la chaŒne adr len est trouv‚e dans bseg badr,
\ sur blen.
CODE LSEARCH  ( adr len bseg badr blen --- badr flg )
  CLD
  CX POP
  BX POP
  ES POP
  DX POP
  DI POP
  DX CX SUB
  BX CX ADD
  SI PUSH
  BX PUSH
  BX SI XCHG
  0 [DI] AL MOV
  >UPPER #) CALL
  AL  TEMPORARY #) MOV
  BEGIN
    CX SI CMP <=
  WHILE
    TEMPORARY #) AH MOV
    ES: AL LODS
    >UPPER #) CALL
    AH AL CMP 0=
    IF
      SI PUSH
      BX BX XOR
      BEGIN
        BX INC
        0 [DI+BX] AL MOV
        >UPPER #) CALL
        AH AL XCHG
        ES: AL LODS
        >UPPER #) CALL
        AH AL CMP  0<>
      UNTIL
      SI POP
      DX BX CMP >=
      IF
        BX POP
        SI DEC
        DX POP
        DX SI XCHG
        -1 # AX MOV
        2PUSH
      THEN
    THEN
  REPEAT
  DX POP
  SI POP
  AX AX XOR
  2PUSH
END-CODE

VARIABLE ?EMS                \ Contient VRAI si de la m‚moire EMS est pr‚sente.
VARIABLE EMS-FRAME           \ Contient l'adresse de segment du cadre de page EMS.

: EMS?  ( --- fl )
  ?EMS @
;

: #EMS-FRAME  ( --- seg )
  EMS-FRAME @
;

\ Initialise la variable EMS? … VRAI si de la m‚moire EMS est pr‚sente.
: EMS-INST  ( --- )
  " EMMXXXX0"                \ L'entˆte de tout driver … la norme EMS
  67 INT@ DROP 0A            \ commence par la chaine "EMMXXXX0"
  2 PICK LSEARCH             \ off fl ---
  ?EMS ! DROP                \ ---
;

\ Gestion des erreurs.
: ?EMS-ERR  ( nøerr --- )
  ?DUP 
  IF
    80 OVER = ABORT" EMM: Erreur interne, EMM peut-ˆtre d‚truit"
    81 OVER = ABORT" EMM: D‚faut de fonctionnement de l'‚lectronique EMS"
    82 OVER = ABORT" EMM: Erreur non identifi‚e"
    83 OVER = ABORT" EMM: Handle EMS incorrect"
    84 OVER = ABORT" EMM: Fonction EMS inexistante"
    85 OVER = ABORT" EMM: Plus de handle EMS disponible"
    86 OVER = ABORT" EMM: Erreur de sauvegarde ou de reconstitution du mapping"
    87 OVER = ABORT" EMM: Pas assez de pages physiques disponibles"
    88 OVER = ABORT" EMM: Pas assez de pages libres disponibles"
    89 OVER = ABORT" EMM: 0 page r‚clam‚e"
    8A OVER = ABORT" EMM: Page logique incorrecte"
    8B OVER = ABORT" EMM: Page physique incorrecte"
    8C OVER = ABORT" EMM: Zone de m‚moire du mapping pleine"
    8D OVER = ABORT" EMM: Mapping non r‚tabli aprŠs avoir ‚t‚ sauvegard‚"
  THEN
;

\ Renvoi le status de l'EMM
CODE (EMM-STATUS)  ( --- nøerr )
  40 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

: EMM-STATUS  ( --- )
  (EMM-STATUS) ?EMS-ERR
;

\ Renvoi l'adresse de segment du cadre de page EMS.
CODE (EMM-GETFRAME)  ( --- seg nøerr )
  41 # AH MOV
  67 INT
  BX PUSH
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

\ Initialise la variable EMS-FRAME
: EMM-GETFRAME  ( --- )
  (EMM-GETFRAME) ?EMS-ERR
  EMS-FRAME !
;

\ Renvoi le nombre de pages (16 Ko chaques) libres et totales
CODE (EMM-GETPAGE)  ( --- pages_libres pages_totales nøerr)
  42 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  BX PUSH
  2PUSH
END-CODE

\ Renvoi le nombre de pages EMS totales
: EMM-NUMPAGES  ( --- pages_totales )
  (EMM-GETPAGE) ?EMS-ERR
  NIP
;

\ Renvoi le nombre de pages EMS libres
: EMM-FREEPAGES  ( --- pages_libres )
  (EMM-GETPAGE) ?EMS-ERR
  DROP
;

\ Alloue un nombre de pages (16 Ko chaques) EMS.
\ Renvoi le handler n‚c‚ssaire aux futurs accŠs.
CODE (EMM-ALLOC)  ( nbpages --- hEMS nøerr)
  BX POP
  43 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  2PUSH
END-CODE

: EMM-ALLOC  ( nbpages --- hEMS )
  (EMM-ALLOC) ?EMS-ERR
;

\ Initialise le mapping.
\ Permet d'incruster une page pr‚c‚dement allou‚e, dans l'une des 4
\ pages physique du cadre de page EMS.
\ nøpge_physique : num‚ro de page physique (0 … 3)
\ nøpge_logique  : num‚ro de page logique (0 … nb_page_allou‚es-1)
\ hEMS           : handle EMS
CODE (EMM-SETMAPPING)  ( nøpge_physique nøpage_logique hEMS --- nøerr )
  DX POP
  BX POP
  AX POP
  44 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

: EMM-SETMAPPING  ( nøpge_physique nøpage_logique hEMS --- )
  (EMM-SETMAPPING) ?EMS-ERR
;

\ Lib‚ration des pages pr‚c‚dement allou‚es.
CODE (EMM-FREE)  ( hEMS --- nøerr )
  DX POP
  45 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

: EMM-FREE  ( hEMS --- )
  (EMM-FREE) ?EMS-ERR
;

\ Empile le nø de version de l'EMM
CODE (EMM-@VERSION)  ( --- nmin nmaj nøerr )
  46 # AH MOV
  67 INT
  AL DL MOV
  DH DH XOR
  0F # DL AND
  DX PUSH
  AL DL MOV
  4 # CL MOV
  DL CL SHR
  AL AH XCHG
  AH AH XOR
  2PUSH
END-CODE

: EMM-@VERSION  ( --- nmin nmaj )
  (EMM-@VERSION) ?EMS-ERR
;

\ Sauvegarde le mapping actuel
CODE (EMM->MAPPING)  ( hEMS --- nøerr )
  DX POP
  47 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

: EMM->MAPPING  ( hEMS --- )
  (EMM->MAPPING) ?EMS-ERR
;

\ Restitue le mapping pr‚c‚dement sauvegard‚
CODE (EMM-MAPPING>)  ( hEMS --- nøerr )
  DX POP
  48 # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  1PUSH
END-CODE

: EMM-MAPPING>  ( hEMS --- )
  (EMM-MAPPING>) ?EMS-ERR
;

\ Empile le nombre de handlers attribu‚s avec la fonction 43h
CODE (EMM-@HANDLERS)  ( --- nb_hEMS nøerr )
  4B # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  BX PUSH
  1PUSH
END-CODE

: EMM-@HANDLERS  ( --- nb_hEMS )
  (EMM-@HANDLERS) ?EMS-ERR
;

\ Empile le nombre de pages allou‚es pour le handler sp‚cifi‚
CODE (EMM-@PAGES)  ( hEMS --- nb_pages nøerr )
  DX POP
  4C # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  BX PUSH
  1PUSH
END-CODE

: EMM-@PAGES  ( hEMS --- nb_pages )
  (EMM-@PAGES) ?EMS-ERR
;

\ Renvoi des informations sur tous les handlers activ‚s
\ seg off    : adresse d'un tableau qui contiendra les informations
\              sur les handlers.
\ nb_entr‚es : nombre de lignes remplies du tableau pr‚c‚dent
CODE (EMM-@INFO)  ( seg off --- seg off nb_entr‚es nøerr )
  DI POP
  AX POP
  ES PUSH
  AX ES MOV
  4D # AH MOV
  67 INT
  AL AH XCHG
  AH AH XOR
  CX POP
  ES PUSH
  DI PUSH
  BX PUSH
  CX ES MOV
  1PUSH
END-CODE

: EMM-@INFO  ( seg off --- seg off nb_entr‚es )
  (EMM-@INFO) ?EMS-ERR
;

DECIMAL
EOF

