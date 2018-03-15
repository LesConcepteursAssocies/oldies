\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Lancement d'un programme … une heure donn‚e.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? COMMAND.COM NOT ?\ INCLUDE COMSPEC

HEX

\ Fonction DOS. Retourne l'heure courante du systŠme.
CODE ((TIME)) ( --- hhmm sscc )
  2C # AH MOV
  21 INT
  CX PUSH
  DX PUSH
  NEXT
END-CODE

\ Fonction DOS. Retourne la date courante du systŠme.
CODE ((DATE))  ( --- aaaa mmqq nj )
    2A # AH MOV
    21 INT
    CX PUSH                       \ cx: ann‚e
    DX PUSH                       \ dh: mois; dl: quantiŠme
    AH AH XOR
    1PUSH                         \ al : nø du jour (0=Dimanche,1=Lundi,...6=Samedi)
END-CODE

DECIMAL

\ Retourne l'heure et les minutes.
: (TIME)  ( --- mm hh )
  ((TIME)) DROP 256 /MOD
;

\ Retourne la date.
: (DATE)  ( --- ann‚e mois jour )
  ((DATE)) DROP 256 /MOD SWAP
;

\ Converti la date en chaine
10 STRING DATE$

: ##  ( n --- adr l )
  0 <# # # #>
;

: DATE!  ( --- )
  " " DATE$ $!
  (DATE)
  ## DATE$ 0 INSERT$              \ Jour.
  ## DATE$ 2 INSERT$              \ Mois.
  0 <# #S #>
  DATE$ 4 INSERT$                 \ Ann‚e.
;

2VARIABLE TIMESTART               \ Contient l'heure (HH:MM) de d‚clenchement
80 STRING PROGSTART               \ Contient le nom du programme … lancer.
VARIABLE  NBEXEC                  \ Nombre d'ex‚cutions
VARIABLE  #EXEC                   \ Nombre d'ex‚cutions autoris‚e.

\ Ex‚cute <str> … hh mm.
: ALARME  ( <str> hh mm nbj --- )
  DEPTH 4 <
  IF
    128 COUNT $EXECUTE            \ Ligne de commande
  THEN
  #EXEC !
  0 NBEXEC !
  SWAP TIMESTART 2!
  PROGSTART $!
  CR
  PROGSTART TYPE
  ."  sera ex‚cut‚ "
  #EXEC @ . ." fois … "
  TIMESTART 2@ . 1 BACKSPACES ." h" .
  BEGIN
    (TIME)                        \ mm hh ---
    TIMESTART 2@ D=
    IF
      CR
      DATE! ." Le " DATE$ TYPE SPACE
      ." ex‚cution de " PROGSTART TYPE SPACE
      PROGSTART $EXECUTE
      NBEXEC 1+!
    THEN
    NBEXEC @ #EXEC @ >=
  UNTIL
  BYE
;

\ Application: Lancement de DF.BAT
80 STRING COMMANDE$

: LANCEDF
  ((DATE)) NIP NIP 15 AND
  DUP 0<> SWAP 6 <> AND           \ Ni samedi ni dimanche
  IF
    " DF " COMMANDE$ $!
    DATE$ COMMANDE$ 3 INSERT$
    COMMANDE$ SHELL
  THEN
;

EOF
-------------------------------------------------------------------------------
Exemple :

: TDIR  ( --- )
  " DIR" SHELL
;

\ Ex‚cute TDIR … 10h20mn 3 jours de suite.
" TDIR" 10 20 3 ALARME

