\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ Etude de l'interruption 2E.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
HEX

\ Lib‚ration de la m‚moire allou‚e … Turbo-FORTH.
CODE LIB-COM  ( --- )
  CS AX MOV
  AX ES MOV
  1000 # BX MOV
  4A # AH MOV
  21 INT
  NEXT
END-CODE

LIB-COM

FORGET LIB-COM

\ D‚finition de chaine avec strockage du CR au bout.
: STRINGCR  ( lmax nom --- )
  CREATE
    DUP C, 0 C,
    1+ ALLOT
  DOES>
    1+ COUNT 2DUP +
    0D SWAP C!
;

\ Passage d'une commande … l'interpr‚teur interne (en g‚n‚ral COMMAND.COM),
\ et ‚x‚cution de celle-ci.
\ seg:off : adresse de la ligne de commande.
CODE (XSHELL)  ( seg off --- )
  IP CX MOV
  SI POP
  DS AX MOV
  DS POP
  ES PUSH                    \ Sauvegarde du contexte.
  SS PUSH
  AX PUSH
  CX PUSH
  RP PUSH
  W  PUSH
  SP PUSH
  2E INT
  SP POP
  W  POP
  RP POP
  IP POP
  DS POP
  SS POP
  ES POP
  NEXT
END-CODE

DECIMAL

\ Le programme principal.
: XSHELL  ( $cmdline --- )
  DROP 1- DSEGMENT SWAP (XSHELL)
;

EOF
L'interruption 2E permet d'‚x‚cuter une commande D.O.S, sans pour cela
recharger une copie de l'interpr‚teur de commande.
Seule la partie transitoire de COMMAND.COM est susceptible d'ˆtre recharg‚e,
pr‚voir dans ce cas la place n‚c‚ssaire.

