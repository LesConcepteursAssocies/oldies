\ -----------------------------------------------------------------------------
\                                Utilitaire divers.
\
\ Cr‚ation de tableaux de chaines.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ Cr‚‚ un tableau de nbstr chaines de longueur longmax.
: []STRING  ( nbstr longmax --- )
  CREATE
    SWAP 2DUP C, C, 0
    DO
      DUP C, 0 C,
      DUP ALLOT
    LOOP
    DROP
  DOES>  ( n adr --- adr lutil )
    OVER OVER C@ 1- > ABORT" []STRING: Indice hors limites"
    DUP 1+ C@ 2+ ROT * + 2+
    1+ COUNT
;

\ Cr‚‚ un tableau de nbstr chaines se terminant par 0, de longueur longmax.
: []STRING0  ( nbstr longmax --- )
  CREATE
    SWAP 2DUP C, C, 0
    DO
      DUP C, 0 C,
      DUP 1+ ALLOT
    LOOP
    DROP
  DOES>  ( n adr --- adr lutil )
    OVER OVER C@ 1- > ABORT" []STRING0: Indice hors limites"
    DUP 1+ C@ 3 + ROT * + 2+
    1+ COUNT 2DUP + 0 SWAP C!
;

EOF

