\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ Op‚rateurs m‚moires 32 bits extra segment.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

CODE L2@  ( seg off --- d )
  DS CX MOV
  BX POP
  DS POP
  0 [BX] AX MOV
  BX INC
  BX INC
  0 [BX] DX MOV
  CX DS MOV
  2PUSH
END-CODE

CODE L2!  ( d seg off --- )
  DS CX MOV
  BX POP
  DS POP
  0 [BX] POP
  BX INC
  BX INC
  0 [BX] POP
  CX DS MOV
  NEXT
END-CODE

EOF

