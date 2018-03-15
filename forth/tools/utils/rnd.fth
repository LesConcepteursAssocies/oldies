\ ------------------------------------------------------------------------------
\                     G‚n‚ration de nombres pseudo-al‚atoires.
\
\ par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

ECHO OFF

DECIMAL

VARIABLE SEED

: SEEDON  ( --- n )
  SEED @ 75 UM* 75 0 D+
  2DUP U< - -
  1- DUP SEED !
;

\ G‚nŠre un nombre compris entre 0 et n-1.
: RND ( n --- [0..n-1] )
  SEEDON UM* SWAP DROP
;

\ G‚nŠre un nombre compris entre a et b inclus.
: []RND  ( a b --- n )
  OVER - 1+ RND +
;

\ Initialise le g‚n‚rateur.
: RANDOMIZE  ( --- )
  1000 RND DROP
;

EOF

