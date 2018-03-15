\ -----------------------------------------------------------------------------
\                                Utilitaire.
\
\ Routines de gestion de la m‚moire de type XMS.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
HEX

2VARIABLE XMS-ADR            \ Contient l'adresse du point d'entr‚e.

\ Renvoi VRAI si un driver XMS est pr‚sent.
CODE XMS?  ( --- fl )
  4300 # AX MOV
  2F INT
  80 # AL CMP
  0=
  IF
    TRUE # AX MOV
  ELSE
    FALSE # AX MOV
  THEN
  1PUSH
END-CODE

\ R‚cup‚ration de l'adresse du point d'entr‚e dans le driver.
CODE (XMS-ADR!)  ( --- seg off )
  4310 # AX MOV
  2F INT
  ES PUSH
  BX PUSH
  NEXT
END-CODE

\ Initialise la variable XMS-ADR.
: XMS-ADR!  ( --- )
  (XMS-ADR!) XMS-ADR 2!
;

\ Gestion des erreurs
: ?XMS-ERR  ( nøerr fl --- )
  0=
  IF
    80 OVER = ABORT" XMS: Fonction non impl‚ment‚e"
    81 OVER = ABORT" XMS: Vdisk d‚tect‚"
    82 OVER = ABORT" XMS: Erreur A20 rencontr‚e"
    8E OVER = ABORT" XMS: Erreur g‚n‚rale du driver"
    8F OVER = ABORT" XMS: Erreur fatale du driver"
    90 OVER = ABORT" XMS: Zone HMA inexistante"
    91 OVER = ABORT" XMS: Zone HMA d‚j… utilis‚e"
    92 OVER = ABORT" XMS: DX est inf‚rieur au paramŠtre HMAMIN"
    93 OVER = ABORT" XMS: Zone HMA non allou‚e"
    94 OVER = ABORT" XMS: La ligne A20 doit ˆtre valid‚e"
    A0 OVER = ABORT" XMS: Toute la m‚moire ‚tendue est allou‚e"
    A1 OVER = ABORT" XMS: Tous les handles disponibles sont allou‚s"
    A2 OVER = ABORT" XMS: Handle invalide"
    A3 OVER = ABORT" XMS: Handle source invalide"
    A4 OVER = ABORT" XMS: Offset source invalide"
    A5 OVER = ABORT" XMS: Handle destination invalide"
    A6 OVER = ABORT" XMS: Offset destination invalide"
    A7 OVER = ABORT" XMS: Longueur invalide"
    A8 OVER = ABORT" XMS: Recouvrement du d‚placement invalide"
    A9 OVER = ABORT" XMS: Erreur de parit‚ rencontr‚e"
    AA OVER = ABORT" XMS: Bloc non verrouill‚"
    AB OVER = ABORT" XMS: Bloc verrouill‚"
    AC OVER = ABORT" XMS: D‚passement du compteur de verrouillage"
    AD OVER = ABORT" XMS: Erreur de verrouillage"
    B0 OVER = ABORT" XMS: Seul un petit UMB est disponible"
    B1 OVER = ABORT" XMS: Pas d'UMB disponible"
    B2 OVER = ABORT" XMS: Segment d'UMB invalide"
  THEN
  DROP
;

\ Compile un appel au point d'entr‚e du driver XMS.
ASSEMBLER ALSO
: >XMS  ( --- )
  CS: FAR XMS-ADR S#) CALL
;
ONLY FORTH ALSO

\ Empile le nø de version de l'XMS.
\ bcdver  : nø de version (BCD)
\ bcdrint : nø interne de r‚vision (BCD)
\ flhma   : = 0 : H.M.A inexistant, = 1 : H.M.A existe
CODE (XMS-@VERSION)  ( --- flhma bcdrint bcdver )
  0 # AH MOV
  >XMS
  DX PUSH
  BX PUSH
  1PUSH
END-CODE

\ Convertion d'un nombre BCD 16 Bits en d‚cimal.
\ forme du nombre BCD :
\
\    H     L
\ +--+--+--+--+
\ |n1|n2|n3|n4|
\ +--+--+--+--+
\
CODE (BCD->N)  ( nbcd --- n3 n4 n1 n2 )
  AX POP
  AL DL MOV
  DH DH XOR
  4 # CL MOV
  DL CL SHR
  DX PUSH
  AL DL MOV
  0F # DL AND
  DX PUSH
  AH AL XCHG
  AH AH XOR
  AL DL MOV
  4 # CL MOV
  DL CL SHR
  0F # AL AND
  2PUSH
END-CODE

: BCD->N  ( nbcd --- nl nh )
  (BCD->N) SWAP 0A * +
  -ROT SWAP 0A * + SWAP
;

: XMS-@VERSION  ( --- flhma rintmin rintmaj vermin vermaj )
  (XMS-@VERSION)
  BCD->N ROT
  BCD->N 2SWAP
;

\ Allocation du segment H.M.A.
\ noct : nombre d'octets … allouer.
CODE (XMS-ALLOC-HMA)  ( noct --- nøerr fl )
  DX POP
  1 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-ALLOC-HMA  ( noct --- )
  (XMS-ALLOC-HMA) ?XMS-ERR
;

\ Lib‚ration du segment H.M.A.
CODE (XMS-FREE-HMA)  ( --- nøerr fl )
  2 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-FREE-HMA  ( --- )
  (XMS-FREE-HMA) ?XMS-ERR
;

\ Validation globale de la ligne A20.
\ N‚c‚ssaire pour utiliser le H.M.A.
CODE (XMS-GALLOC-A20)  ( --- nøerr fl )
  3 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-GALLOC-A20  ( --- )
  (XMS-GALLOC-A20) ?XMS-ERR
;

\ Lib‚ration globale de la ligne A20.
CODE (XMS-GFREE-A20)  ( --- nøerr fl )
  4 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-GFREE-A20  ( --- )
  (XMS-GFREE-A20) ?XMS-ERR
;

\ Validation locale de la ligne A20.
\ Utilis‚ pour l'accŠs direct … la m‚moire ‚tendue.
CODE (XMS-LALLOC-A20)  ( --- nøerr fl )
  5 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-LALLOC-A20  ( --- )
  (XMS-LALLOC-A20) ?XMS-ERR
;

\ Lib‚ration locale de la ligne A20.
CODE (XMS-LFREE-A20)  ( --- nøerr fl )
  6 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-LFREE-A20  ( --- )
  (XMS-LFREE-A20) ?XMS-ERR
;

\ Etat de la ligne A20.
\ status = 1 : Ligne valid‚e
\ status = 0 : Ligne invalid‚e.
CODE (XMS-STATUS-A20)  ( --- status nøerr fl )
  7 # AH MOV
  >XMS
  BH BH XOR
  AX PUSH
  BX PUSH
  BX PUSH
  NEXT
END-CODE

: XMS-STATUS-A20  ( --- status )
  (XMS-STATUS-A20) DUP 0<>
  IF
    DROP 0 ?XMS-ERR
  ELSE
    2DROP
  THEN
;

\ Renvoi la taille du plus grand E.M.B disponible, ainsi que le total
\ de la m‚moire ‚tendue, en Ko. Le H.M.A n'est pas compt‚.
CODE  (XMS-#EMB)  ( --- #total #emb nøerr )
  8 # AH MOV
  >XMS
  BH BH XOR
  DX PUSH
  AX PUSH
  BX PUSH
  NEXT
END-CODE

: XMS-#EMB  ( --- #total #emb )
  (XMS-#EMB) 0 ?XMS-ERR
;

\ Allocation d'un E.M.B.
\ taille est en Ko.
CODE (XMS-ALLOC-EMB)  ( taille --- Hemb nøerr fl )
  DX POP
  9 # AH MOV
  >XMS
  BH BH XOR
  DX PUSH
  BX PUSH
  1PUSH
END-CODE

\ Renvoi un Handler sur le bloc allou‚.
: XMS-ALLOC-EMB  ( taille --- Hemb )
  (XMS-ALLOC-EMB) ?XMS-ERR
;

\ Lib‚ration d'un E.M.B allou‚.
CODE (XMS-FREE-EMB)  ( Hemb --- nøerr fl )
  DX POP
  0A # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-FREE-EMB  ( Hemb --- )
  (XMS-FREE-EMB) ?XMS-ERR
;

\ Recopie d'un E.M.B.
\ adrEMMstr : segment et offset de la structure E.M.M du bloc.
\ Structure de l'EMM.:
\
\ taille     Offset  description
\  4 Octets    0     Nombre d'octets … copier (doit ˆtre pair).
\  2 Octets    4     Handler du bloc source.
\  4 Octets    6     Offset dans le bloc source.
\  2 Octets   10     Handler du bloc destination.
\  4 Octets   12     Offset dans le bloc destination.
CODE (XMS-EMB-MOVE)  ( adrEMMstr --- nøerr fl )
  BX POP
  AX POP
  SI PUSH
  DS PUSH
  AX DS MOV
  BX SI MOV
  0B # AH MOV
  >XMS
  DS POP
  SI POP
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-EMB-MOVE  ( adrEMMstr --- )
  (XMS-EMB-MOVE) ?XMS-ERR
;

\ Verrouillage d'un E.M.B.
\ Utilis‚ pour les accŠs directs.
CODE (XMS-EMB-LOCK)  ( Hemb --- adr32 nøerr fl )
  DX POP
  0C # AH MOV
  >XMS
  DX PUSH
  BX PUSH
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

\ Renvoi l'adresse lin‚aire (32 bits) du bloc verrouill‚.
: XMS-EMB-LOCK  ( Hemb --- adr32 )
  (XMS-EMB-LOCK) ?XMS-ERR
;

\ D‚verrouillage d'un E.M.B.
CODE (XMS-EMB-UNLOCK)  ( Hemb --- nøerr fl )
  DX POP
  0D # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-EMB-UNLOCK  ( Hemb --- )
  (XMS-EMB-UNLOCK) ?XMS-ERR
;

\ Renvoi des informations sur un E.M.B.
\ #taille : taille du bloc en Ko.
\ #info   : partie haute = compteur de verrouillage.
\           partie basse = nombre de hanlders disponibles.
CODE (XMS-EMB-INFO)  ( Hemb --- #taille #info nøerr fl )
  DX POP
  0E # AH MOV
  >XMS
  DX PUSH
  BX PUSH
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-EMB-INFO  ( Hemb --- #taille #info )
  (XMS-EMB-INFO) ?XMS-ERR
;

\ Change la taille d'un E.M.B.
\ #ntaille = nouvelle taille en Ko.
CODE (XMS-REALLOC-EMB)  ( #ntaille Hemb --- nøerr fl )
  DX POP
  BX POP
  0F # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-REALLOC-EMB  ( #ntaille Hemb --- )
  (XMS-REALLOC-EMB) ?XMS-ERR
;

\ Allocation d'un U.M.B.
\ #ptaille = taille en paragraphes.
CODE (XMS-ALLOC-UMB)  ( #ptaille --- segUMB #acttail nøerr fl )
  DX POP
  10 # AH MOV
  >XMS
  BX PUSH
  DX PUSH
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

\ Renvoi l'adresse de segment de l'U.M.B, ainsi que la taille actuelle
\ de celui-ci. Si une erreur survient, #acttail est la taille du plus
\ grand U.M.B disponible.
: XMS-ALLOC-UMB  ( #ptaille --- segUMB #acttail )
  (XMS-ALLOC-UMB) ?XMS-ERR
;

\ Lib‚ration d'un U.M.B.
CODE (XMS-FREE-UMB)  ( segUMB --- nøerr fl )
  DX POP
  11 # AH MOV
  >XMS
  BH BH XOR
  BX PUSH
  1PUSH
END-CODE

: XMS-FREE-UMB  ( segUMB --- )
  (XMS-FREE-UMB) ?XMS-ERR
;

DECIMAL

EOF

