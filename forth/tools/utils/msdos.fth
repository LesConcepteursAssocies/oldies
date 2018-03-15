\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Outils divers D.O.S.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

HEX

\ empile le num‚ro de version du dos.
CODE OSVER  ( --- min maj )
  30 # AH MOV
  21 INT
  DX DX XOR
  AH DL MOV
  AH AH XOR
  2PUSH
END-CODE

: OSMAJOR  ( --- maj )
  OSVER NIP
;

: OSMINOR  ( --- min )
  OSVER DROP
;

\ Empile l'adresse du vecteur d'interruption n.
CODE INT@  ( n --- seg off )
  AX POP
  35 # AH MOV
  21 INT
  ES PUSH
  BX PUSH
  NEXT
END-CODE

\ Initialise le vecteur d'interruption n avec l'adresse donn‚e.
CODE INT!  ( seg off n --- )
  AX POP
  DX POP
  BX POP
  DS PUSH
  BX DS MOV
  25 # AH MOV
  21 INT
  DS POP
  NEXT
END-CODE

DECIMAL

EOF

