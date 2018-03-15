\ -----------------------------------------------------------------------------
\                              Utilitaire disques.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? HFILE NOT ?\ INCLUDE DFILES

DECIMAL

\ Dumping d'un fichier.
\ pos  : position du premier octet … lire
\ long : longueur … lire
: FDUMP  ( pos long adr_fic --- )
  ?OPEN
  PATHWAY READ (OPEN) ?DOS-ERR
  HFILE ! 
  REC-SIZE !
  BUF-DIR REC-SIZE @ BLANK
  FREAD
  BASE @ HEX
  CR 7 SPACES
  8 0
  DO
    I 2 .R SPACE
  LOOP SPACE
  16 8
  DO
    I 2 .R SPACE
  LOOP SPACE
  16 0
  DO
    I 1 .R
  LOOP CR 7 SPACES
  23 ASCII - REPLICATE 2 SPACES
  23 ASCII - REPLICATE 2 SPACES
  16 ASCII - REPLICATE
  REC-SIZE @ 0
  DO
    CR 7 SPACES
    I 
    8 2DUP   D.2 SPACE
    OVER + 8 D.2 SPACE
    16 BOUNDS
    ?DO
      BUF-DIR I + C@ 127 AND
      DUP BL 126 BETWEEN NOT
      IF
        DROP ASCII .
      THEN
      EMIT
    LOOP
    STOP? ?LEAVE
    16
  +LOOP
  BASE ! CR
  HFILE @ (CLOSE)
;

EOF

