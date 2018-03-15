\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ Outils de cr‚ation de matrices.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
DECIMAL

\ Tests de d‚passement.
: (OVF-MATRICE)  ( X Y [PFA] --- X Y [PFA] )
  >R
  OVER R@ @      \ X Y --- X dimX
  U< NOT ABORT" MATRICE : Indice X hors limites"
  DUP R@ 2+ @    \ X Y --- Y dimY
  U< NOT ABORT" MATRICE : Indice Y hors limites"
  R>
;

DEFER OVF-MATRICE
' (OVF-MATRICE) IS OVF-MATRICE

\ Cr‚ation de matrices d'octets … deux dimensions.
: CREER-CMATRICE  ( dimX dimY --- )
  CREATE
    2DUP SWAP
    ,              \ dimX dans PFA
    ,              \ dimY dans PFA+2
    * ALLOT        \ reserve dimX * dimY octets
  DOES>  ( X Y [PFA] --- adr_‚lem. )
    OVF-MATRICE
    DUP @          \ X Y [PFA] --- dimX
    ROT *          \ X [PFA] --- dimX*Y
    + + 4 +        \ --- [PFA]+4+dimX*Y+X
;

\ Cr‚ation de matrices de mots … deux dimensions.
: CREER-MATRICE  ( dimX dimY --- )
  CREATE
    2DUP SWAP
    ,                        \ dimX dans PFA
    ,                        \ dimY dans PFA+2
    * 2* ALLOT               \ reserve dimX * dimY mots
  DOES>                      \ X Y [PFA] --- adr_‚lem.
    OVF-MATRICE
    DUP @                    \ X Y [PFA] --- dimX
    ROT * 2*                 \ X [PFA] --- dimX*Y
    + SWAP 2* +              \ --- [PFA]+dimX*2Y+2X
    4 +                      \ --- [PFA]+dimX*2Y+2X+4
;

\ Cr‚ation de matrices de dmots … deux dimensions.
: CREER-2MATRICE  ( dimX dimY --- )
  CREATE
    2DUP SWAP
    ,                        \ dimX dans PFA
    ,                        \ dimY dans PFA+2
    * 4 * ALLOT              \ reserve dimX * dimY dmots
  DOES>                      \ X Y [PFA] --- adr_‚lem.
    OVF-MATRICE
    DUP @                    \ X Y [PFA] --- dimX
    ROT * 4 *                \ X [PFA] --- dimX*Y
    + SWAP 4 * +             \ --- [PFA]+dimX*4Y+4X
    4 +                      \ --- [PFA]+dimX*4Y+4X+4
;

EOF

