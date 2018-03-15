\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE

: FGDEMO49 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      3 2 FGTESTMODE
      IF
         3 FGSETMODE
      ELSE
         7 2 FGTESTMODE
         IF
            7 FGSETMODE
         ELSE
            ABORT" Cette d‚monstration n‚c‚ssite 80 colonnes"
         THEN
      THEN
      0 FGCURSOR
      9 7 0 FGSETATTR
      25 0
      DO
         I 0 FGLOCATE
         " Ligne Nø " FGTEXT I (.) FGTEXT
         " Ligne Nø " FGTEXT I (.) FGTEXT
         " Ligne Nø " FGTEXT I (.) FGTEXT
         " Ligne Nø " FGTEXT I (.) FGTEXT
         " Ligne Nø " FGTEXT I (.) FGTEXT
      LOOP
      18 FGWAITFOR
      1 FGALLOCATE DROP
      32 47 11 13 0 24 0 1 FGTRANSFER
      1 FGSETCOLOR
      32 47 11 13 FGRECT
      15 1 0 FGSETATTR
      12 33 FGLOCATE
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      0 15 22 24 32 13 1 0 FGTRANSFER
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      1 FGFREEPAGE DROP
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

