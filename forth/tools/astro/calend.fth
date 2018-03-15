\ -----------------------------------------------------------------------------
\                            Etude des calendriers.
\
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

\ La pr‚cision ‚tant un facteur important, on travaille avec
\ les flottants...
EXIST? FLOAT NOT ?\ INCLUDE \L\TBF\PRGTBF\F-PACK.FTH

ONLY FORTH ALSO F-PACK ALSO
FORTH DEFINITIONS

DECIMAL

FVARIABLE JJ
FVARIABLE MM
FVARIABLE AA
FVARIABLE TU

\ Conversion d'une date en date julienne.
\ jour  : jour
\ mois  : mois
\ annee : ann‚e
\ tu    : temps universel
\ jd est un flottant
: DATE->JD  ( jour mois annee tu --- fjd )
  S>F TU F! S>F AA F! S>F MM F! S>F JJ F!
  AA F@ MM F@ FDUP F' 3 F<
  IF
    F' 12 F+ FSWAP %1 F- FSWAP
  THEN
  FOVER F' 365.25 F* FINT    \ Y M int(Y * 365.25) ---
  FSWAP %1 F+ F' 30.6001 F*
  FINT F+ JJ F@ F+           \ Y int(Y * 365.25)+int(M+1 * 30.6001)+jour ---
  F' 1720994.5 F+ FSWAP      \ int(Y * 365.25)+int(M+1 * 30.6001)+jour+1720994.5 Y ---
  AA F@ MM F@ F' 100 F/ F+
  JJ F@ F' 10000 F/ F+
  F' 1582.10145 F>
  IF
    F' 100 F/ FINT
    FDUP F' 4 F/ FINT
    FROT F' 2 F+ FROT F- F+
  ELSE
    FDROP
  THEN
  TU F@ F' 24 F/ F+
;

\ Calcul du rang (dans l'ann‚e) du jour d'une date.
: DATE->RG  ( jour mois annee --- rang )
  S>F AA F! S>F MM F! S>F JJ F!
  F' 2
  AA F@ F' 4 F/ FDUP FINT    \ 2 A/4 int(A/4) ---
  F=
  IF
    FDROP %1
  THEN
  MM F@ F' 9 F/ F' 275 F* FINT
  FSWAP JJ F@ F' 30 F- FSWAP
  MM F@ F' 9 F+ F' 12 F/ FINT
  F* F- F+
;

\ D‚termination du jour de la semaine d'une date.
\ njour = 0 : dimanche
\ njour = 1 : lundi
\ njour = 2 : mardi
\ ...
: JOUR  ( jour mois annee --- njour )
  0 DATE->JD F' 1.5 F+ F' 7 FMOD
;

\ Calcul de la date de pƒques.
\ Calculs interm‚diaires.
: FA  ( --- A )
  AA F@ F' 19 AA F@ F' 19 F/
  FINT F* F-
;

: FB  ( --- B )
  AA F@ F' 100 F/ FINT
;

: FC  ( --- C )
  AA F@ F' 100 FB F* F-
;

: FD  ( --- D )
  FB F' 4 F/ FINT
;

: FE  ( --- E )
  FB F' 4 FD F* F-
;

: FF  ( --- F )
  FB F' 8 F+ F' 25 F/ FINT
;

: FG  ( --- G )
  FB FF F- %1 F+ F' 3 F/ FINT
;

: FH  ( --- H )
  F' 19 FA F*
  FB FD F- FG F- F+ F' 15 F+
  FDUP F' 30 FSWAP F' 30 F/ FINT F* F-
;

: FI  ( --- I )
  FC F' 4 F/ FINT
;

: FK  ( --- K )
  FC F' 4 FI F* F-
;

: FL  ( --- L )
  F' 32 FE F' 2 F* F+
  FI F' 2 F* F+ FH F- FK F-
  FDUP F' 7 FSWAP F' 7 F/ FINT F* F-
;

: FM  ( --- M )
  F' 11 FH F* F' 22 FL F* F+ FA F+
  F' 451 F/ FINT
;

\ Calendrier gr‚gorien.
: GREGORIEN  ( --- mois jour )
  FH FL F+ F' 7 FM F* F- F' 114
  F+ F' 31 F/ FINT           \ mois ---
  FH FL F+ F' 7 FM F* F- F' 114
  F+ FOVER F' 31 F* F-       \ mois jour ---
;

: JFA  ( --- A )
  AA F@ F' 4 AA F@ F' 4 F/
  FINT F* F-
;

: JFB  ( --- A )
  AA F@ F' 7 AA F@ F' 7 F/
  FINT F* F-
;

: JFC  ( --- A )
  AA F@ F' 19 AA F@ F' 19 F/
  FINT F* F-
;

: JFD  ( --- D )
  F' 19 JFC F* F' 15 F+
  F' 30 FOVER F' 30 F/ FINT
  F* F-
;

: JFE  ( --- E )
  F' 2 JFA F* F' 4 JFB F* F+
  JFD F- F' 34 F+
  F' 7 FOVER F' 7 F/ FINT
  F* F-
;

\ Calendrier julien.
: JULIEN  ( --- mois jour )
  JFD JFE F+ F' 114 F+
  F' 31 F/ FINT              \ mois ---
  JFD JFE F+ F' 114 F+
  FOVER F' 31 F* F-          \ mois jour ---
;

\ Le programme principal.
\ annee : ann‚e … calculer.
\ Renvoi le mois et le jour de pƒques de l'ann‚e donn‚e.
: PAQUES  ( annee --- mois jour )
  DUP S>F FINT AA F!
  1583 <
  IF
    JULIEN
  ELSE
    GREGORIEN
  THEN
  %1 F+
;

EOF

