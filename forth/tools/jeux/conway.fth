\ -----------------------------------------------------------------------------
\                  Le Jeu de la vie, ou l'Univers de CONWAY.
\
\ Par Guillaumaud philippe
\ -----------------------------------------------------------------------------

EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES

\ Positionne le curseur en haut … gauche sans effacer l'‚cran
: HOME  ( --- )
  27 (CONS) 91 (CONS) 102 (CONS)
;

20 CONSTANT DIMX
20 CONSTANT DIMY
DIMX DIMY CREER-CMATRICE UNIVERS

\ Initialisation de l'univers
: SET  ( Y X --- )
  UNIVERS 1 SWAP C!
;

\ Remise … z‚ro de l'univers
: RESET  ( Y X --- )
  UNIVERS 0 SWAP C!
;

: CLEAR  ( --- )
  DIMX 0
  DO
    DIMY 0
    DO
      I J RESET
    LOOP
  LOOP
;
CLEAR

\ Affichage de l'univers
: AFFICHER  ( --- )
  HOME
  DIMY 0
  DO
    38 DIMY - SPACES
    DIMX 0
    DO
      I J UNIVERS C@
      IF
        ." ±±"
      ELSE
        ."   "
      THEN
    LOOP
    CR
  LOOP
;

\ Normalisation de l'univers
: NORMALISER  ( --- )
  DIMY 0
  DO
    DIMX 0
    DO
      I J UNIVERS DUP C@
      DUP 3 >
      IF
        DROP 0
      ELSE
        DUP 1 >
        IF
          DROP 1
        THEN
      THEN
      SWAP C!
    LOOP
  LOOP
;

: VERIFIER  ( n --- n' )
  DUP 3 =
  IF
    DROP 2+
  ELSE
    2 = 0=
    IF
      4 +
    THEN
  THEN
;

\ G‚n‚ration de la population suivante
: GENERER  ( --- )
  DIMY 0
  DO
    DIMX 0
    DO
      0
      J 2+ DIMY MIN
      J 1- 0    MAX
      DO
        J 2+ DIMX MIN
        J 1- 0    MAX
        DO
          I J UNIVERS C@ 1 AND +
        LOOP
      LOOP
      I J UNIVERS C@ 1 AND
      SWAP OVER -
      VERIFIER
      I J UNIVERS C!
    LOOP
  LOOP
;

\ Le programme principal
: JEU  ( --- )
  DARK
  0
  DO
    AFFICHER
    GENERER
    NORMALISER
    STOP? ?LEAVE
  LOOP
  AFFICHER
;

\ Exemple de configuration initiale
: WAY-02  ( Y X --- )
  2DUP SET
  OVER 1+ OVER SET
  2DUP 1+ SET
  SWAP 1+ SWAP 1+ SET
;

: CONWAY  ( --- )
  CLEAR
  01 01 WAY-02 01 05 WAY-02 01 09 WAY-02 01 13 WAY-02 01 17 WAY-02
  05 01 WAY-02                           05 13 WAY-02 05 17 WAY-02
  09 01 WAY-02              09 09 WAY-02              09 17 WAY-02
  13 01 WAY-02 13 05 WAY-02                           13 17 WAY-02
  17 01 WAY-02 17 05 WAY-02 17 09 WAY-02 17 13 WAY-02 17 17 WAY-02
  18 01 DO
    I I SET
  LOOP
  6 10 SET 7 11 SET 8 12 SET 9 13 SET
  10 6 SET 11 7 SET 12 8 SET 13 9 SET
  84 JEU
;


EOF

