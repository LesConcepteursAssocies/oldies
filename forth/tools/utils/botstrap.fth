\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Plusieurs m‚thodes pour relancer le systŠme.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

HEX

2VARIABLE BOOTSTRAP          \ Adresse de saut pour relancer le systŠme.
FFFF 0000 BOOTSTRAP 2!

\ Effectue un boot ‚quivalent au CTRL+ALT+DEL.
CODE CTRL-ALT-DEL  ( --- )
  0040 # AX MOV
  AX DS MOV
  0072 # BX MOV
  1234 # AX MOV
  AX 0 [BX] MOV
  FAR CS: BOOTSTRAP S#) JMP
END-CODE

\ Effectue un boot ‚quivalent … un reset.
\ R‚initialise la m‚moire.
CODE RESET  ( --- )
  0040 # AX MOV
  AX DS MOV
  0072 # BX MOV
  0 # AX MOV
  AX 0 [BX] MOV
  FAR CS: BOOTSTRAP S#) JMP
END-CODE

DECIMAL

EOF

