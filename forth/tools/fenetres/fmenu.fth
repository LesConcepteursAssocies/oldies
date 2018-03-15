\ -----------------------------------------------------------------------------
\                               Extension … FEN.FTH
\
\ Gestion de menus d‚roulants.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? (EKEY) NOT ?\ INCLUDE EKEY
EXIST? FEN NOT ?\ INCLUDE FEN

FEN DEFINITIONS
DECIMAL

\ -----------------------------------------------------------------------------
\                        Manipulation des paramŠtres de menu
\ -----------------------------------------------------------------------------
0 CONSTANT MHORIZONTAL
1 CONSTANT MVERTICAL

\ Options du menu (segment, offset et longueur. de la chaine).
: $OPTIONS  ( --- adr )
  WPARAM 32 +
;

\ Contient le nombre d'options disponibles.
: #OPTIONS  ( --- adr )
  WPARAM 38 +
;

\ Contient l'option courante.
: OPTION#  ( --- adr )
  WPARAM 40 +
;

\ Couleur de l'option courante (fond et caractŠre).
: OCOL  ( --- adr )
  WPARAM 42 +
;

\ Type du menu (0=horizontal, 1=vertical).
: TMENU  ( --- adr )
  WPARAM 44 +
;

\ Position de la chaine de libell‚s en x.
: OPTX  ( --- adr )
  WPARAM 46 +
;

\ Position de la chaine de libell‚s en y.
: OPTY  ( --- adr )
  WPARAM 48 +
;

\ -----------------------------------------------------------------------------
\                             Fonctions de menu
\ -----------------------------------------------------------------------------
\ Versions extra-segment de SKIP, SCAN, ITEM.
LABEL LDONE
  CX PUSH
  NEXT

CODE LSKIP  ( seg off l c --- seg off' l' )
  AX POP
  CX POP
  LDONE JCXZ
  DI POP
  ES POP
  REPZ BYTE SCAS
  0<>
  IF
    CX INC
    DI DEC
  THEN
  ES PUSH
  DI PUSH
  CX PUSH
  NEXT
END-CODE

CODE LSCAN  ( seg off l c --- seg off' l' )
  AX POP
  CX POP
  LDONE JCXZ
  DI POP
  ES POP
  CX BX MOV
  REP BYTE SCAS
  0=
  IF
    CX INC
    DI DEC
  THEN
  ES PUSH
  DI PUSH
  CX PUSH
  NEXT
END-CODE

: LITEM  ( seg off l n --- seg off l )
  1 MAX >R -ROT 2 PICK R> 0
  DO                         \ l seg off l ---
    3 ROLL DROP BL LSKIP     \ seg off' l' ---
    2 PICK 2 PICK ROT
    BL LSCAN 4 ROLL DROP     \ off' seg off'' l'' ---
  LOOP
  DROP ROT SWAP              \ seg off' off'' ---
  OVER -                     \ seg off l ---
;

\ Empile le nombre total d'items pr‚sents dans une chaine extra-segment.
: LITEMS  ( seg off l --- n )
  0 3 ROLL 2SWAP 1           \ n seg off l n ---
  BEGIN
    >R
    3DUP R@ LITEM 2NIP       \ n seg off l l ---
    R> SWAP                  \ n seg off l n l ---
    0<>
  WHILE
    >R                       \ n seg off l ---
    2SWAP
    SWAP 1+ SWAP
    2SWAP
    R> 1+
  REPEAT                     \ n seg off l n ---
  2DROP 2DROP
;

\ Primitive d'affichage de l'option courante avec la couleur donn‚e.
: (.OPTION)  ( coul --- )
  OPTX @ OPTY @              \ coul X Y ---
  $OPTIONS 3@                \ coul X Y seg off l ---
  OPTION# @ LITEM 2NIP       \ coul X Y l ---
  WXCHGCOL
;

\ Affiche l'option courante avec la couleur de OCOL.
: OPTION-ON  ( --- )
  OCOL @ (.OPTION)
;

\ Affiche l'option courante avec la couleur de FCOL.
: OPTION-OFF  ( --- )
  FCOL @ (.OPTION)
;

\ Primitive d'affichage d'un menu, en fonction de son type.
: (W.MENU)  ( --- )
  CR-OFF
  OPTX @ OPTY @ (AT) 2DROP
  TMENU @
  CASE MHORIZONTAL OF
                     $OPTIONS 3@ LTYPE
                     OPTX @ OPTY @ AT
                   ENDOF
       MVERTICAL   OF
                     #OPTIONS @
                     HAUT @ MIN 1+ 1
                     DO
                       OPTX @ I AT
                       $OPTIONS 3@ I LITEM LTYPE
                     LOOP
                     OPTX @ OPTY @ AT
                   ENDOF
  ENDCASE
  CURX @ CURY @ AT
  CR-ON
;

\ Gestion du menu courant.
\ Touches g‚r‚es :
\                  - Les flŠches pour les d‚placements.
\                  - La touche Return pour valider l'option courante.
\                  - La touche Escape pour abandonner.
\ Empile le nø de l'option choisie, ou 0 si abandon.
: WGESMENU  ( --- n )
  CURSOR-OFF
  OPTX @ OPTY @ (AT) 2DROP
  BEGIN
    OPTION-ON
    FALSE
    KEY?
    IF
      KEY
      CASE #FLH OF           \ flŠche Haute
                  TMENU @ MVERTICAL =
                  IF
                    OPTION-OFF
                    OPTION# @ 1-
                    DUP 1 <
                    IF
                      DROP #OPTIONS @
                      HAUT @ MIN
                    THEN
                    DUP OPTION# !
                    OPTY !
                  THEN
                ENDOF
           #FLB OF           \ flŠche Basse
                  TMENU @ MVERTICAL =
                  IF
                    OPTION-OFF
                    OPTION# @ 1+
                    DUP #OPTIONS @ >
                    OVER HAUT @ > OR
                    IF
                      DROP 1
                    THEN
                    DUP OPTION# !
                    OPTY !
                  THEN
                ENDOF
           #FLG OF           \ flŠche Gauche
                  TMENU @ MHORIZONTAL =
                  IF
                    OPTION-OFF
                    OPTION# @ 1-
                    DUP 1 <
                    IF
                      DROP #OPTIONS @
                    THEN
                    OPTION# !
                    $OPTIONS 3@ OPTION# @ LITEM DROP NIP
                    $OPTIONS 3@ DROP NIP - 1+ OPTX !
                  THEN
                ENDOF
           #FLD OF           \ flŠche Droite
                  TMENU @ MHORIZONTAL =
                  IF
                    OPTION-OFF
                    OPTION# @ 1+
                    DUP #OPTIONS @ >
                    IF
                      DROP 1
                    THEN
                    OPTION# !
                    $OPTIONS 3@ OPTION# @ LITEM DROP NIP
                    $OPTIONS 3@ DROP NIP - 1+ OPTX !
                  THEN
                ENDOF
           #RET OF           \ Validation
                  DROP OPTION# @ TRUE
                ENDOF
           #ESC OF           \ Abandon
                  DROP 0 TRUE
                ENDOF
      ENDCASE
    THEN
  UNTIL
  CURX @ CURY @ AT
  CURSOR-ON
;

\ Ouvre une fenˆtre de type menu.
: WMENU  ( pos_x pos_y larg haut forme_bord coul_fen coul_bord $<titre> coul_titre shadow $<options> ocol tmenu --- )
  FNUM @ >WTABLE
  RAZ-WPARAM
  TMENU !
  OCOL !
  3DUP LITEMS #OPTIONS !
  $OPTIONS 3!
  1 OPTION# !
  1 OPTX !
  1 OPTY !
  (WOUVRE)
  (W.MENU)
;

ONLY FORTH FEN ALSO FORTH ALSO DEFINITIONS
CR .( Gestionnaire de MENUS DEROULANTS charg‚ )
EOF

