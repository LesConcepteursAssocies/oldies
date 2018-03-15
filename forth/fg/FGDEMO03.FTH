\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE

: FGDEMO03 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      3 1 FGTESTMODE
      IF
         3 FGSETMODE
      ELSE
         7 1 FGTESTMODE
         IF
            7 FGSETMODE
         ELSE
            ABORT" Cette d‚monstration n‚c‚ssite un ‚cran 80 colonnes"
         THEN
      THEN
      0 FGCURSOR
      15 FGSETCOLOR
      " D‚monstration de FastGraph (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

