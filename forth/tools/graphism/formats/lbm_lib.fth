\ ------------------------------------------------------------------------------
\                 Kernel de Manipulation du format graphique LBM.
\                         (utilis‚ par DELUXE PAINT)
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

WARNING OFF

\ Chargement des outils.
EXIST? (CGA-ECRAN) NOT ?\ INCLUDE CGA_LIB
EXIST? (EGA-ECRAN) NOT ?\ INCLUDE EGA_LIB
EXIST? (MCGA-ECRAN) NOT ?\ INCLUDE MCGA_LIB

\ D‚codage de fichiers d'images DELUXE PAINT CGA 4 couleurs.
\ Contient l'entˆte du fichier.
CREATE (CGA-ENTETE-DPII) 68 ALLOT

\ Lecture de l'entˆte du fichier.
: (CGA-LIT-ENTETE-DPII)  ( hdl --- )
  >R DSEGMENT (CGA-ENTETE-DPII) 68
  R> (GET) ?DOS-ERR DROP
;

\ Le programme principal.
\ Temps d'‚x‚cution : environ 00:00:26,91 sec. sur 80386 … 16 Mhz.
: (CGA-LIT-DPII)  ( hdl --- )
  DUP >R (CGA-LIT-ENTETE-DPII)
  16384 MALLOC DUP 0 16000   \ adr_seg adr_seg 0 16000 ---
  R> (GET) ?DOS-ERR DROP
  200 0                      \ adr_seg ---
  DO
    40 0
    DO
      8 0
      DO                               \ adr_seg ---
        2^ I 2* + @
        2DUP SWAP K 80 * J + LC@       \ adr_seg aa aa o ---
        SWAP IDIV 2MOD                 \ adr_seg aa o1/aaMOD2 ---
        2 PICK K 80 * J + 40 + LC@     \ adr_seg aa o1/aaMOD2 o2 ---
        ROT IDIV 2MOD                  \ adr_seg a b ---
        SWAP 1 << OR                   \ adr_seg c ---
        3 I 4 MOD - 2* <<              \ adr_seg oct ---
        K 2MOD 0=
        IF
          (*CGA-ECRAN) K 40 * J 2* +   \ ligne paire
          I 4 IDIV + 2DUP LC@
          3 ROLL OR -ROT LC!
        ELSE
          (*CGA-ECRAN) K 2 IDIV 80 *   \ ligne impaire
          J 2* + I 4 IDIV + 8192 +
          2DUP LC@ 3 ROLL OR -ROT LC!
        THEN
      LOOP
    LOOP
  LOOP
  RELEASE
;

\ D‚codage de fichiers d'images DELUXE PAINT EGA 16 couleurs.
\ Contient l'entˆte du fichier.
HEX
CREATE (EGA-ENTETE-DPII) 68 ALLOT

\ Lecture de l'entˆte du fichier.
: (EGA-LIT-ENTETE-DPII)  ( hdl --- )
  >R DSEGMENT (EGA-ENTETE-DPII) 68
  R> (GET) ?DOS-ERR DROP
;

\ Le programme principal.
\ Temps d'‚x‚cution : environ 00:00:00,11 sec. sur 80386 … 16 Mhz.
: (EGA-LIT-DPII)  ( hdl --- )
  DUP >R (EGA-LIT-ENTETE-DPII)
  8000 MALLOC DUP 0 7D00          \ adr_seg adr_seg 0 32000 ---
  R> (GET) ?DOS-ERR DROP
  4 0                             \ adr_seg ---
  DO
    2 03C4 PC!
    1 I << 03C5 PC!
    C8 0
    DO
      DUP I 4 * J + 28 *          \ adr_seg adr_seg 40*(I*4+J) ---
      (*EGA-ECRAN) I 28 *
      J 1F40 * + 28               \ adr_seg adr_seg 40*(I*4+J) adr_dst I*40+plan L ---
      LCMOVE
    LOOP                          \ adr_seg ---
  LOOP
  2 03C4 PC!
  F 03C5 PC!
  RELEASE
;
DECIMAL

\ D‚codage de fichiers d'images DELUXE PAINT MCGA 256 couleurs.
\ Contient l'entˆte du fichier.
CREATE (MCGA-ENTETE-DPII) 824 ALLOT

\ Lecture de l'entˆte du fichier.
: (MCGA-LIT-ENTETE-DPII)  ( hdl --- )
  >R DSEGMENT (MCGA-ENTETE-DPII) 824
  R> (GET) ?DOS-ERR DROP
;

\ Fabrique un octet … partir des 8 nombres pr‚sents sur la pile
CODE BITS->OCTET  ( b8 b7 b6 b5 b4 b3 b2 b1 --- o )
  AX POP
  BX POP
  BX SHL
  BX AX OR
  BX POP
  2 # CL MOV
  BX CL SHL
  BX AX OR
  BX POP
  3 # CL MOV
  BX CL SHL
  BX AX OR
  BX POP
  4 # CL MOV
  BX CL SHL
  BX AX OR
  BX POP
  5 # CL MOV
  BX CL SHL
  BX AX OR
  BX POP
  6 # CL MOV
  BX CL SHL
  BX AX OR
  BX POP
  7 # CL MOV
  BX CL SHL
  BX AX OR
  1PUSH
END-CODE

VARIABLE OCTET
0 , 0 , 0 , 0 , 0 , 0 , 0 ,

\ Le programme principal.
\ Temps d'‚x‚cution : environ 00:00:37,52 sec. sur 80386 16 Mhz.
: (MCGA-LIT-DPII)  ( hdl --- )
  DUP >R (MCGA-LIT-ENTETE-DPII)
  65535 MALLOC DUP 0 64000   \ adr_seg adr_seg 0 64000 ---
  R> (GET) ?DOS-ERR DROP
  200 0                      \ adr_seg ---
  DO
    I 320 *
    40 0
    DO                       \ adr_seg adr1 ---
      DUP I +
      2 PICK SWAP
      8 0
      DO                               \ adr_seg adr1 adr_seg adresse ---
        2^ I 2* + @                    \ adr_seg adr1 adr_seg adresse a ---
        >R 2DUP LC@ R>                 \ adr_seg adr1 adr_seg adresse octet a ---
        SWAP OVER IDIV 2MOD OCTET !    \ adr_seg adr1 adr_seg adresse a ---
        >R 2DUP 40 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 2+ !
        >R 2DUP 80 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 4 + !
        >R 2DUP 120 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 6 + !
        >R 2DUP 160 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 8 + !
        >R 2DUP 200 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 10 + !
        >R 2DUP 240 + LC@ R>
        SWAP OVER IDIV 2MOD OCTET 12 + !
        >R 2DUP 280 + LC@ R>
        IDIV 2MOD OCTET 14 + !
        (*MCGA-ECRAN) 3 PICK J 8* + I +
        OCTET 14 + @ OCTET 12 + @ OCTET 10 + @
        OCTET 8 + @ OCTET 6 + @ OCTET 4 + @
        OCTET 2+ @ OCTET @ BITS->OCTET
        -ROT LC!                       \ adr_seg adr1 adr_seg adresse ---
      LOOP
      2DROP
    LOOP
    DROP
  LOOP
  RELEASE
;

\ Le programme principal.
: LIT-DPII  ( adr_fic l --- )
  ?OPEN
  DROP 0 (OPEN) ?DOS-ERR >R
  GRMODE @
  CASE
     5 OF
         (*CGA-ECRAN) 0 16384 0 LFILL
         R@ (CGA-LIT-DPII)
       ENDOF
    13 OF
         (*EGA-ECRAN) 0 32768 0 LFILL
         R@ (EGA-LIT-DPII)
       ENDOF
    19 OF
         (*MCGA-ECRAN) 0 65535 0 LFILL
         R@ (MCGA-LIT-DPII)
       ENDOF
  ENDCASE
  R> (CLOSE)
;

\ A titre de test :
\ Lecture et affichage d'un fichier MCGA.
: .LBM-MCGA  ( adr l --- )
  19 MODE
  (MCGA-*INIT)
  LIT-DPII
  (MCGA-*ECR->ECR)
  BEGIN
    KEY 32 =
  UNTIL
  (*MCGA-ECRAN) RELEASE
  (*MCGA-SAUVE) RELEASE
  3 MODE
;

CR .( Librairie LBM charg‚e )
EOF

