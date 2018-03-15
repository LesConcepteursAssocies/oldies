\ -----------------------------------------------------------------------------
\                    Affichage ‚tendu du contenu du r‚pertoire.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF

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

80 STRING0 *.*
" *.*" *.* $!

2VARIABLE DIR.SIZE           \ Taille totale des fichiers.
VARIABLE  DIR.FILES          \ Nombre de fichiers.
VARIABLE  DIR.REPS           \ Nombre de r‚pertoires.
VARIABLE  DIR.SEARCH         \ Attribut de recherche.

\ Attributs des fichiers
1   CONSTANT IS_RDONLY
2   CONSTANT IS_HIDDEN
4   CONSTANT IS_SYSTEM
8   CONSTANT IS_LABEL
16  CONSTANT IS_DIR
32  CONSTANT IS_ARCHIVE
255 CONSTANT ALL_FILES
ALL_FILES DIR.SEARCH !       \ Tous les fichiers par d‚faut.

CREATE DTA 512 ALLOT         \ On se fabrique une DTA.

\ Affichage d'un nombre justifi‚ … droite avec des 0.
: 0.R
  >R (.) R>
  OVER - ASCII 0 REPLICATE
  TYPE
;

\ Affichage du nom justifi‚ sur 12 caractŠres.
: .NAME
  12 >R
  0
  BEGIN
    DUP DTA 30 + + DUP C@
  WHILE
    DUP C@ ASCII . =
    IF
      DUP 1+
      C@ ASCII . =
      R@ 4 > OR
      IF
        OVER 8 SWAP - DUP R>
        SWAP - >R SPACES
      THEN
    THEN
    C@ EMIT 1+
  REPEAT
  DROP
  R> SWAP - SPACES
;

\ Affichage des attributs du fichier courant.
: .ATTRB
  DTA 21 + C@
  DUP IS_RDONLY AND
  IF
    ."  RDONLY"  \ Lecture seule (Read Only)
  THEN
  DUP IS_HIDDEN AND
  IF
    ."  HIDDEN"   \ Cach‚ (Hidden)
  THEN
  DUP IS_SYSTEM AND
  IF
    ."  SYSTEM"   \ SystŠme (System)
  THEN
  DUP IS_LABEL AND
  IF
    ."  LABEL"   \ Nom de volume (Label)
  THEN
  DUP IS_ARCHIVE AND
  IF
    ."  ARCHIVE"   \ Archiv‚ (Archive)
  THEN
  0=
  IF
    ."  NORMAL"   \ Normal
  THEN
;

\ Affichage de la taille du fichier
: .SIZE
  DTA 21 + C@ IS_DIR AND
  IF
    ."      <DIR>"
    DIR.REPS 1+!
  ELSE
    DTA 26 + 2@ SWAP 10 UD.R
    DIR.FILES 1+!
  THEN
;

\ Affichage de la date du fichier
: .DATE
  DTA 24 + @
  DUP 31 AND 2 0.R ASCII / EMIT
  DUP 32 / 15 AND 2 0.R ASCII / EMIT
  512 / 1980 + 4 0.R
;

\ Affichage de l'heure du fichier
: .TIME
  DTA 22 + @
  DUP 2048 / ABS 2 0.R ASCII : EMIT
  32 / 63 AND 2 0.R
;

\ Empile Vrai si la recherche a abouti.
: SEARCH?  ( --- fl )
  DTA 21 + C@
  DIR.SEARCH @ AND
;

\ Le programme principal.
: XDIR
  DTA SET-DMA
  0. DIR.SIZE 2!
  0  DIR.FILES !
  0  DIR.REPS  !
  PATHWAY DUP C@ 0=
  IF
    DROP *.* DROP
  THEN
  255 (SEARCH0)
  IF
    BASE @ DECIMAL
    BEGIN
      SEARCH?
      IF
        CR
        .NAME SPACE
        .SIZE 2 SPACES
        .DATE 2 SPACES
        .TIME 2 SPACES
        .ATTRB
        DIR.SIZE DUP 2@ SWAP
        DTA 26 + 2@ SWAP D+ SWAP ROT 2!
      THEN
      (SEARCH)
      IF
        STOP?
      ELSE
        TRUE
      THEN
    UNTIL
    CR CR 7 SPACES ." Total "
          DIR.SIZE 2@ SWAP 10 UD.R SPACE ." Octet(s)"
       CR 13 SPACES
          DIR.FILES @ 10        .R SPACE ." Fichier(s)"
       CR 13 SPACES
          DIR.REPS  @ 10        .R SPACE ." R‚pertoire(s)"
       CR
    BASE !
  ELSE
    CR ." Fichier(s) non trouv‚(s)" CR
  THEN
;

EOF
\ -----------------------------------------------------------------------------
\ Exemples d'utilisation :

  IS_DIR DIR.SEARCH ! XDIR  (tous les r‚pertoires)
  IS_HIDDEN DIR.SEARCH ! XDIR (tous les fichiers cach‚s)

