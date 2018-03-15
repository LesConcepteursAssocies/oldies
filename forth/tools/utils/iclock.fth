\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ D‚tournement d'une interruption.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? INT@ NOT ?\ INCLUDE MSDOS

HEX

\
\ Exemple : d‚tournement de l'interruption horloge 1Ch et affichage permanent
\           de l'heure.
\
CREATE $TEMPS 5 ALLOT        \ contient l'heure sous forme ASCII 'HH:MM'
VARIABLE ATTRB 70 ATTRB C!   \ attribut d'affichage (inversion vid‚o par d‚faut)
B800 CONSTANT SCREEN         \ adresse de l'‚cran

VARIABLE #SEC                \ compteur des secondes

2VARIABLE OLDINT1C           \ ancien vecteur 1Ch
1C INT@ OLDINT1C 2!

\ Fonction DOS. Retourne l'heure courante du systŠme.
CODE (TIME) ( --- hhmm sscc )
  2C # AH MOV
  21 INT
  CX PUSH
  DX PUSH
  NEXT
END-CODE

DECIMAL

\ R‚cupŠre l'heure courante, et calcule le nombre de secondes restant
\ avant la prochaine incr‚mentation des minutes.
\
: TIME  ( --- d )
  (TIME)
  256 /MOD NIP     \ hh mm ss
  60 SWAP - 182 *  \ nb_secondes * 182 (18,2 * 10)
  10 /MOD NIP
  #SEC !
  256 /MOD         \ mm hh
  60 * +           \ mm
  S>D
;

\ Passage en base 6.
: SEXTAL
  6 BASE !
;

: :00
  # SEXTAL #
  ASCII : HOLD
  DECIMAL
;

: HH:MM  ( d --- adr long )
  BASE @ >R
  <#
    :00
    # #
  #>
  R> BASE !
  $TEMPS SWAP CMOVE
;

: !$TEMPS
  TIME HH:MM
;

\ affichage
\ on ‚crit directement dans la R.A.M vid‚o
LABEL .TIME
  ES PUSH
  DI PUSH
  SI PUSH
  AX PUSH
  BX PUSH
  CX PUSH
  $TEMPS # BX MOV
  75 2 * # DI MOV                      \ Ligne 0, colonne 75
  SCREEN # AX MOV
  AX ES MOV
  ' ATTRB >BODY # SI MOV
  0 [SI] AH MOV
  5 # CX MOV
  BEGIN
    0 # CX CMP
    0<>
  WHILE
    0 [BX] AL MOV
    AX ES: 0 [DI] MOV
    DI INC
    DI INC
    BX INC
    CX DEC
  REPEAT
  CX POP
  BX POP
  AX POP
  SI POP
  DI POP
  ES POP
  RET

\
\ Nouvelle interruption 1Ch
\ (appel‚e 18,2 fois par seconde)
\
LABEL SORTIE
  AX POP
  BX POP
  DS POP
  FAR CS: OLDINT1C S#) JMP             \ saut … l'ancienne routine

LABEL NEWINT1C
  CLI
  DS PUSH
  BX PUSH
  AX PUSH
  DSEGMENT # AX MOV
  AX DS MOV
  .TIME #) CALL                        \ affichage
  ' #SEC >BODY # BX MOV                \ d‚crementation du nombre de
  0 [BX] AX MOV                        \ secondes restant.
  AX DEC
  AX 0 [BX] MOV
  0 # AX CMP
  0=
  IF
    1092 # AX MOV                      \ 1 minute = 60 * 18,2
    AX 0 [BX] MOV
    $TEMPS 4 + # BX MOV
    0 [BX] AL MOV
    AL INC                             \ 1 minute de plus
    AL 0 [BX] MOV
    ASCII 9 # AL CMP                   \ est-on … '9' ?
    >
    IF
      ASCII 0 # AL MOV                 \ oui, on repasse … '0'
      AL 0 [BX] MOV
      BX DEC
      0 [BX] AL MOV
      AL INC
      AL 0 [BX] MOV                    \ 1 dizaine de minute de plus
      ASCII 5 # AL CMP                 \ est-on … '59' ?
      >
      IF
        ASCII 0 # AL MOV               \ oui, on repasse … '0'
        AL 0 [BX] MOV
        BX DEC
        BX DEC                         \ on passe le ':'
        0 [BX] AL MOV                  \ 1 heure de plus
        AL INC
        AL 0 [BX] MOV
        ASCII 4 # AL CMP
        0=
        IF
          BX DEC
          0 [BX] AL MOV
          ASCII 2 # AL CMP             \ est-on … '24' heure ?
          0=
          IF
            ASCII 0 # AL MOV           \ oui, on repasse … '00'
            AL 0 [BX] MOV
            BX INC
            AL 0 [BX] MOV
            SORTIE #) JMP
          THEN
        THEN
        $TEMPS 1+ # BX MOV
        0 [BX] AL MOV
        ASCII 9 # AL CMP
        >
        IF
          ASCII 0 # AL MOV
          AL 0 [BX] MOV
          BX DEC
          0 [BX] AL MOV
          AL INC
          AL 0 [BX] MOV                \ 1 dizaine d'heure de plus
        THEN
      THEN
    THEN
  THEN
  STI
  SORTIE #) JMP

HEX

\ Activation de l'horloge permanente.
\
: CLOCK
  !$TEMPS
  1C INT@ OLDINT1C 2!
  DSEGMENT NEWINT1C 1C INT!
;

\ D‚sactivation de l'horloge
\
: NOCLOCK
  OLDINT1C 2@ 1C INT!
;

DECIMAL
EOF

