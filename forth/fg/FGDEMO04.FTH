\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE NEWMODE

: FGDEMO04 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      80 25 1 FGBESTMODE NEWMODE !
      NEWMODE @ 0<
      IF
         ABORT" Cette d‚monstration n‚c‚ssite un ‚cran 80 colonnes"
      THEN
      NEWMODE @ FGSETMODE
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

