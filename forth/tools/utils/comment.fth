\ -----------------------------------------------------------------------------
\                      Insertion de commentaires multilignes.
\ 
\ 
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

\ Ignore toutes les lignes suivantes jusqu'au caractŠre c.
\ A n'utiliser que pendant l'inclusion d'un fichier source.
: COMMENT  ( c --- )
  [COMPILE] ASCII            \ c ---
  BEGIN
    GETLINE BUFFER COUNT     \ c addeb l ---
    OVER SWAP 3 PICK         \ c addeb addeb l c ---
    SCAN                     \ c addeb adfin l ---
    IF
      SWAP - 1+ >IN ! TRUE   \ c fl ---
    ELSE
      2DROP FALSE
    THEN
    EOF? @ OR
  UNTIL                      \ c ---
  DROP
;
IMMEDIATE

\ Tests imm‚diats !
COMMENT *
  ceci n'est pas compil‚:
  : MACHIN  ( --- )
    CR ." je ne doit pas ˆtre compil‚" CR
  ;
ceci non plus *

COMMENT /
  autre exemple de commentaires multilignes
  non inclus
  Plus pratique … utiliser qu'une suite
  de caractŠres "\" ou "( ... )"
/

EOF

