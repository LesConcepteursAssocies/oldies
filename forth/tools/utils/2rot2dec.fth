\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ D‚calages et rotations 32 bits.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ Rotation … gauche d'un nombre 32 bits.
\ Effectue la rotation de n bits … gauche du nombre d.
CODE 2RCL  ( d n --- d )
  CX POP
  AX POP
  DX POP
  HERE
    AX SHL
    DX RCL
    0 # AX ADC
  LOOP
  2PUSH
END-CODE

\ Rotation … droite d'un nombre 32 bits.
\ Effectue la rotation de n bits … droite du nombre d.
CODE 2RCR  ( d n --- d )
  CX POP
  AX POP
  DX POP
  BX PUSH
  HERE
    DX BX MOV
    BX SHR
    AX RCR
    DX RCR
  LOOP
  BX POP
  2PUSH
END-CODE

\ D‚calage … gauche d'un nombre 32 bits.
\ D‚cale le nombre d de n bits vers la gauche.
CODE 2SHL  ( d n --- d )
  CX POP
  AX POP
  DX POP
  HERE
    AX SHL
    DX RCL
  LOOP
  2PUSH
END-CODE

\ D‚calage … droite d'un nombre 32 bits.
\ D‚cale le nombre d de n bits vers la droite.
CODE 2SHR  ( d n --- d )
  CX POP
  AX POP
  DX POP
  HERE
    DX SHR
    AX RCR
  LOOP
  2PUSH
END-CODE

EOF

