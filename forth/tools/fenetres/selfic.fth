\ -----------------------------------------------------------------------------
\                             Application de FSELECT
\
\ S‚lection interactive d'un fichier.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? WSELECT NOT ?\ INCLUDE FSELECT

\ Cr‚ation de variables chaine se terminant par 0.
\ (Utile pour stocker les noms de fichiers DOS)
: STRING0  ( long_max nom_chaine --- )
  CREATE
    DUP C, 0 C,
    1+ ALLOT
  DOES>
    1+ COUNT 2DUP +
    0 SWAP C!
;

\ Primitive de changement de r‚pertoire.
: (CHDIR)  ( adr0 --- )
  59 BDOS DROP
;

\ Obtention du r‚pertoire courant.
\ adr : adresse du buffer de r‚ception.
HEX
CODE (CWD)  ( adr --- )
  SI BX MOV
  0 # DL MOV                 \ 0 = unit‚ par d‚faut.
  SI POP
  47 # AH MOV
  21 INT
  BX SI MOV
  NEXT
END-CODE
DECIMAL

\ Calcul la longueur d'une chaine ascii0.
: STRLEN0  ( str0 l --- )
  DROP DUP 0
  LEN$ NIP 0 SCAN
  DROP OVER - SWAP 1- C!
;

\ R‚cupŠre et calcule la longueur du r‚pertoire courant.
: CWD  ( adr0 l --- )
  DROP DUP ASCII \ SWAP C!
  DUP 1+ (CWD) 0 STRLEN0
;

\ Contient une sauvegarde du r‚pertoire courant.
65 STRING0 CWD$

\ Contient le masque de recherche.
12 STRING0 MASQUE$
" *.*" MASQUE$ $!

\ Masque par d‚faut.
3 STRING0 *.*
" *.*" *.* $!

\ Contient le r‚pertoire actuel.
65 STRING0 CREP$

\ Contient le nom complet (r‚pertoire+nom) du fichier s‚lectionn‚.
80 STRING0 FICHIER$

\ Sauvegarde du r‚pertoire courant.
: >CWD  ( --- )
  CWD$ CWD
;

\ Cr‚ation de la DTA
CREATE DTA 512 ALLOT

\ Cr‚ation du buffer de stockage des noms de fichiers.
4096 CONSTANT #NOMS          \ Taille de la zone en octets.
                             \ (permet de stocker 315 noms de 13 car.)
CREATE BUFFNOMS
  #NOMS ALLOT

\ Remise … z‚ro du buffer.
: RAZ-BUFFNOMS
  BUFFNOMS #NOMS ERASE
;

\ Empile VRAI si le fichier courant est un r‚pertoire.
: DIR?  ( --- fl )
  DTA 21 + C@ 16 AND
  16 =
;

\ Met le nom pr‚sent dans la DTA dans le buffer.
\ Met … jour la longueur du buffer.
: NAME!  ( adr l --- adr l' )
  0
  BEGIN                      \ adr l n ---
    DUP DTA 30 + + DUP C@
  WHILE                      \ adr l n adrDTA ---
    C@ 2OVER + C!            \ adr l n ---
    1+
    SWAP 1+ SWAP             \ adr l n ---
  REPEAT                     \ adr l n adrDTA ---
  2DROP
  DIR?
  IF                         \ Si on a affaire … un r‚pertoire.
    2DUP ASCII \
    -ROT + C! 1+             \ InsŠre un backslash.
  THEN
  2DUP BL -ROT + C!
  1+                         \ adr l ---
;

\ Stockage des noms de r‚pertoire en premier.
\ Renvoi l'adresse et la longueur du buffer mise … jour.
: DIR!  ( --- adr l )
  BUFFNOMS 0 *.* DROP        \ adr l masque$ ---
  255 (SEARCH0)
  IF
    BEGIN
      DIR?
      IF
        DTA 30 + C@ ASCII . =
        IF
          DTA 31 + C@        \ On ne stocke pas le point "." unique.
          IF
            NAME!
          THEN
        ELSE
          NAME!
        THEN
      THEN
      (SEARCH) NOT
    UNTIL                    \ adr l ---
  THEN
;

\ Remplissage du buffer de stockage.
\ masque$ : adresse de la chaine contenant le masque.
\ Renvoie la longueur du buffer si quelque chose a ‚t‚ trouv‚, o— 0 sinon.
: BUFFNOMS!  ( masque$ --- l )
  RAZ-BUFFNOMS
  DIR! ROT                   \ adr l adr$ ---
  255 (SEARCH0)              \ adr l fl ---
  IF
    BEGIN                    \ adr l ---
      DIR? NOT
      IF
        NAME!
      THEN
      (SEARCH) NOT
    UNTIL                    \ adr l ---
  THEN
  NIP                        \ l ---
;

\ Le programme principal.
\ Permet de s‚lectionner un fichier correspondant au contenu de la variable
\ MASQUE$.
\ GŠre les d‚placements dans les diff‚rents r‚pertoires.
\ Initialise la variable chaine FICHIER$ avec le nom complet du fichier
\ (r‚pertoire+nom) si un nom a ‚t‚ s‚lectionn‚.
: SELFIC  ( --- )
  CWD$ LEN$ NIP ERASE
  CREP$ LEN$ NIP ERASE
  FICHIER$ LEN$ NIP ERASE
  DMA                        \ Sauvegarde de la DTA courante.
  >CWD                       \ Sauvegarde du r‚pertoire courant.
  WINIT
  EKEY-ON
  DTA SET-DMA
  MASQUE$ DROP BUFFNOMS! >R
  10 5 13 5 1 GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT MASQUE$ ROUGE CYAN F/C FALSE
  DSEGMENT BUFFNOMS R> NOIR BLANC F/C 1 WSELECT
  BEGIN
    WGSELECT
    IF
      $OPTIONS 3@ OPTION# @ LITEM
      3DUP 1- + LC@ ASCII \ =
      IF
        CREP$ LEN$ NIP ERASE
        1- DSEGMENT CREP$    \ D‚placement dans le r‚pertoire s‚lectionn‚.
        DROP >R R@ ROT LCMOVE
        R@ 0 STRLEN0 R> (CHDIR)
        MASQUE$ DROP BUFFNOMS!
        DSEGMENT BUFFNOMS ROT
        3DUP LITEMS #OPTIONS !
        $OPTIONS 3!
        1 DUP OPTX !
        DUP OPTY !
        DUP #F1ER !
        OPTION# !
        DARK
        (W.FMENU)
        FALSE
      ELSE
        DSEGMENT FICHIER$
        DROP >R R@ ROT LCMOVE
        R> 0 STRLEN0
        CREP$ CWD
        " \" FICHIER$ 0 INSERT$
        CREP$ FICHIER$ 0 INSERT$
        TRUE
      THEN
    ELSE
       " " FICHIER$ $! TRUE
    THEN
  UNTIL
  WFERME
  EKEY-OFF
  ROT SET-DMA                \ Restitution de la DTA.
  CWD$ DROP (CHDIR)          \ Restitution du r‚pertoire.
;

EOF
--------------------------------------------------------------------------------
SELFIC est un bon exemple d'utilisation des fenˆtres de type s‚lection.

Il permet de choisir un fichier, de fa‡on interactive, avec gestion automatique
des changements de r‚pertoire.

Le masque de recherche est stock‚ dans la variable MASQUE$.

L'‚ventuel fichier s‚lectionn‚ (nom + r‚pertoire) est stock‚ dans la variable
FICHIER$.

Le contexte (r‚pertoire courant, DTA) est sauvegard‚, puis restitu‚ … la fin
de la s‚lection.

Exemples d'utilisation :

" *.FTH" MASQUE$ $! SELFIC
" *.TXT" MASQUE$ $! SELFIC

