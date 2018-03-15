\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ R‚cupŠre l'adresse du D.I.B. (Dos Information Block), aussi appel‚e
\ List Of List.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
HEX

\ Empile l'adresse du DIB.
\ Ne pointe pas sur le d‚but de la structure.
CODE (DIB)  ( --- seg off )
  ES PUSH
  AX AX XOR
  AX ES MOV
  AX BX MOV
  52 # AH MOV
  21 INT
  ES DX MOV
  BX AX MOV
  ES POP
  2PUSH
END-CODE

DECIMAL

\ Empile l'adresse du DIB.
\ Pointe sur le d‚but de la structure.
: DIB  ( --- seg off-12 )
  (DIB) 12 -
;

EOF

