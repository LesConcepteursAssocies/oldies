\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ R‚cup‚ration de COMSPEC et am‚lioration du mot SHELL.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

WARNING OFF
ECHO OFF

HEX

\ Empile l'adresse du segment de code.
CODE CSEGMENT  ( --- cseg )
  CS AX MOV
  1PUSH
END-CODE

\ Empile l'adresse de segment du PSP.
: PSP  ( --- adr )
  CSEGMENT
;

\ Empile l'adresse de segment du bloc d'environnement de Turbo-Forth.
: SEG-ENV  ( --- adr )
  PSP 2C L@
;

CREATE COMMAND.COM 50 ALLOT

\ R‚cup‚ration de COMSPEC
\ la chaine "COMSPEC=u:..." d‚bute tout bloc d'environnement.
: COMSPEC!  ( --- )
  COMMAND.COM 1+ >R
  SEG-ENV 8
  BEGIN
    2DUP LC@            \ seg off car ---
    SWAP 1+ SWAP        \ seg off+1 car ---
    DUP 0<>
  WHILE
    R@ C! R> 1+ >R
  REPEAT
  R> C!
  9 - COMMAND.COM C!    \ Calcule et stocke la longueur de COMSPEC
  DROP
;

DECIMAL

\ R‚‚criture du mot SHELL.
\ Ex‚cution d'une commande D.O.S sans que le processeur de commandes
\ (COMMAND.COM) soit obligatoirement dans le r‚pertoire courant.
: SHELL  ( string -- )
  COMSPEC!                   \ R‚cupŠre COMSPEC
  HEIGHT ALLOC DROP PASS     \ prend la commande
  " /C " COMMAND$ 0 INSERT$  \ la fait pr‚c‚der de /C
  PARAM!
  COMMAND.COM COUNT
  HERE PLACE
  0 HERE COUNT + C!
  HERE 1+ PARAM 0 (PROGRAM) ?DOS-ERR
;

EOF

