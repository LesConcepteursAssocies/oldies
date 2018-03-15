\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE

: FGDEMO64 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      3 FGSETMODE
      FGMOUSEINI
      0<
      IF
         OLDMODE @ FGSETMODE
         FGRESET
         1 FGCURSOR
         ABORT" Cette d‚monstration n‚c‚ssite une souris"
      THEN
      7 0 0 FGSETATTR
      0 FGGETMAXX 0 FGGETMAXY FGRECT
      12 7 0 FGSETATTR
      25 0
      DO
         I 34 FGLOCATE
         " D‚monstration FastGraph" FGTEXT
      LOOP
      1 FGMOUSEVIS
      " Une touche" FGTEXT
      FGWAITKEY
      [ HEX ] 7FFF 7F00 [ DECIMAL ] FGMOUSECUR
      " Une touche" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

