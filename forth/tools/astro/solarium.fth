\ -----------------------------------------------------------------------------
\                       Voyage dans le systŠme solaire.
\
\
\ Contrainte : Ce programme n‚c‚ssite l'inclusion du package flottant CORDIC,
\              ainsi qu'une carte CGA au minimum.
\
\ Pour profiter de l'affichage, une machine rapide est n‚c‚ssaire...
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF

EXIST? CORDIC NOT ?\ CR .( Ce programme n‚c‚ssite l'inclusion du package flottant CORDIC) EOF

DECIMAL

\ Quelques outils.
\ D‚finition de vecteurs flottants.
: FVECTEUR  ( n <nom> --- )
  CREATE
    DUP ,
    SW * ALLOT
  DOES>  ( n adr --- adr )
    OVER OVER @ 1-
    U> ABORT" FVECTEUR: Indice hors limite"
    2+ SWAP SW * +
;

\ Definition de vecteurs d'entiers.
: VECTEUR  ( n <nom> --- )
  CREATE
    DUP ,
    2* ALLOT
  DOES>  ( n adr --- adr )
    OVER OVER @ 1-
    U> ABORT" VECTEUR: Indice hors limite"
    2+ SWAP 2* +
;

FORTH DEFINITIONS
EXIST? RND NOT ?\ INCLUDE RND

\ Variables et constantes globales.
17 CONSTANT NPLA
21 CONSTANT NETO
VARIABLE TOUCHE
FVARIABLE DEG
FVARIABLE RMAX
FVARIABLE RMAY
360 FVECTEUR SINUS
360 FVECTEUR COSIN
NPLA FVECTEUR D
NPLA FVECTEUR V
NPLA FVECTEUR A
NPLA FVECTEUR DX
NPLA FVECTEUR DY
NPLA FVECTEUR X
NPLA FVECTEUR Y
VARIABLE OX
VARIABLE OY
VARIABLE PLAN
VARIABLE DEV
VARIABLE MUL
FVARIABLE VIT
FVARIABLE RAL
10 FVECTEUR T
NETO VECTEUR EX
NETO VECTEUR EY
FVARIABLE FPOLE

\ Initialisations.
: GRAPH  ( --- )
  6 MODE
  ." Initialisation..."
  360 0
  DO
    I S>F DEG F@ F* FSIN
    I SINUS F!
    I S>F DEG F@ F* FCOS
    I COSIN F!
  LOOP
  NETO 0
  DO
    RANDOMIZE
    640 RND I EX !
    200 RND I EY !
  LOOP
  \ Les 9 PlanŠtes.
  F# 0000.0 0 D F!  F# 001.000 0 V F!
  F# 0057.9 1 D F!  F# 000.241 1 V F! F# 512. 1 T F!
  F# 0108.2 2 D F!  F# 000.615 2 V F! F# 256. 2 T F!
  F# 0149.6 3 D F!  F# 001.000 3 V F! F# 128. 3 T F!
  F# 0228.0 4 D F!  F# 001.881 4 V F! F# 064. 4 T F!
  F# 0778.4 5 D F!  F# 011.860 5 V F! F# 032. 5 T F!
  F# 1427.0 6 D F!  F# 029.460 6 V F! F# 032. 6 T F!
  F# 2869.0 7 D F!  F# 084.010 7 V F! F# 032. 7 T F!
  F# 4496.6 8 D F!  F# 164.100 8 V F! F# 032. 8 T F!
  F# 5965.2 9 D F!  F# 247.000 9 V F! F# 032. 9 T F!
  \ La Lune.
  F# 0.384 10 D F!  F# 0.0794 10 V F!
  \ Les satellites de Mars.
  F# 0.009 11 D F!  F# 0.0008 11 V F!
  F# 0.023 12 D F!  F# 0.0034 12 V F!
  \ Les satellites de Jupiter.
  F# 0.422 13 D F!  F# -0.0048 13 V F!
  F# 1.070 14 D F!  F#  0.0196 14 V F!
  F# 0.671 15 D F!  F#  0.0097 15 V F!
  F# 1.885 16 D F!  F# -0.0244 16 V F!
  NPLA 0
  DO
    F# 0. I A F!
    F# 0. I X F!
    F# 0. I Y F!
  LOOP
  320 OX !
  100 OY !
  4 PLAN !
  F# 32. VIT F!
  F# 1.  RAL F!
  1 MUL  !
  99 DEV !
  DARK
;

\ Affichage du systŠme courant.
: INFO  ( --- )
  0 0 AT
  PLAN @
  CASE 1 OF ." Mercure" ENDOF
       2 OF ." Venus  " ENDOF
       3 OF ." Terre  " ENDOF
       4 OF ." Mars   " ENDOF
       5 OF ." Jupiter" ENDOF
       6 OF ." Saturne" ENDOF
       7 OF ." Uranus " ENDOF
       8 OF ." Neptune" ENDOF
       9 OF ." Pluton " ENDOF
  ENDCASE
;

\ Tracage des planŠtes.
DEFER PLOT

CODE (BIOS-PLOT)  ( X Y coul --- )
  AX POP
  DX POP
  CX POP
  12 # AH MOV
  16 INT
  NEXT
END-CODE

: (PLOT)  ( X Y coul --- )
  >R 2DUP
  0 200 WITHIN SWAP
  0 640 WITHIN AND
  IF
    R> (BIOS-PLOT)
  ELSE
    R> DROP 2DROP
  THEN
;

' (PLOT) IS PLOT

\ Initialisation.
: INIT  ( --- )
  F# 99. PLAN @ D F@ F/ RMAX F!
  DEV @ S>F PLAN @ D F@ F/ RMAY F!
  NPLA 0
  DO
    I D F@ RMAX F@ F* I DX F!
    I D F@ RMAY F@ F* I DY F!
  LOOP
;

\ Tracage et calcul de la nouvelle position.
: POLE  ( fOX fOY fA fDX fDY adrX adrY --- )
  2DUP >R F@ SINT R> F@ SINT 0 PLOT
  >R >R 4 FROLL F# 2.16 3 FROLL
  4 FPICK SINT SINUS F@ F* F* F+
  R@ F!
  FROT FSWAP FROT SINT COSIN F@
  F* F+ FPOLE F!
  R> FPOLE F@ R@ F!
  F@ SINT R> F@ SINT 1 PLOT
;

\ Tracage globale.
: CRAYON  ( --- )
  10 0
  DO
    OX @ S>F OY @ S>F I A F@
    I DX F@ I DY F@ I X I Y POLE
    3 X F@ 3 Y F@ 10 A F@
    10 DX F@ MUL @ S>F F*
    10 DY F@ MUL @ S>F F*
    10 X 10 Y POLE
  LOOP
  13 11
  DO
    4 X F@ 4 Y F@ I A F@
    I DX F@ MUL @ S>F F*
    I DY F@ MUL @ S>F F*
    I X I Y POLE
  LOOP
  17 13
  DO
    5 X F@ 5 Y F@ I A F@
    I DX F@ MUL @ S>F F*
    I DY F@ MUL @ S>F F*
    I X I Y POLE
  LOOP
;

\ Calculs.
: CALCUL  ( --- )
  NPLA 0
  DO
    I A F@ F# 360. I V F@ VIT F@ RAL F@
    F+ F* F/ F+ I A F!
    BEGIN
      I A F@ F# 360. F>
    WHILE
      I A F@ F# 360. F- I A F!
    REPEAT
    BEGIN
      I A F@ F# 0. F<
    WHILE
      I A F@ F# 360. F+ I A F!
    REPEAT
  LOOP
;

\ Gestion des touches.
: COMMANDES  ( --- )
  TOUCHE @ UPC
  CASE ASCII K OF
                 OX @ 10 - OX !
               ENDOF
       ASCII M OF
                 OX @ 10 + OX !
               ENDOF
       ASCII + OF
                 PLAN @ 9 <
                 IF
                   PLAN 1+!
                   PLAN @ T F@ VIT F!
                   F# 1. RAL F!
                   INIT
                   INFO
                 THEN
               ENDOF
       ASCII - OF
                 PLAN @ 1 >
                 IF
                   PLAN 1-!
                   PLAN @ T F@ VIT F!
                   F# 1. RAL F!
                   INIT
                   INFO
                 THEN
               ENDOF
       ASCII G OF
                 RAL F@ F# 1. F>
                 IF
                   RAL F@ F# 2. F/ RAL F!
                 THEN
               ENDOF
       ASCII O OF
                 RAL F@ F# 2. F* RAL F!
               ENDOF
       ASCII H OF
                 DEV @ 9 >
                 IF
                   DEV @ 10 - DEV !
                   INIT
                 THEN
               ENDOF
       ASCII P OF
                 DEV @ 99 <
                 IF
                   DEV @ 10 + DEV !
                   INIT
                 THEN
               ENDOF
       ASCII I OF
                 MUL @ 1024 <
                 IF
                   MUL @ 2* MUL !
                 THEN
               ENDOF
       ASCII Q OF
                 MUL @ 1 >
                 IF
                   MUL @ 2/ MUL !
                 THEN
               ENDOF
            27 OF
                 BEGIN
                   KEY
                 UNTIL
               ENDOF
  ENDCASE
;

\ Le programme principal.
: SOLARIUM  ( --- )
  PI F# 180. F/ DEG F!
  GRAPH
  INIT
  INFO
  BEGIN
    CRAYON
    CALCUL
    KEY?
    IF
      KEY TOUCHE !
      COMMANDES
    ELSE
      0 TOUCHE !
    THEN
    TOUCHE @ 27 =
  UNTIL
  3 MODE
;

EOF

