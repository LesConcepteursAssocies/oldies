\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE LINES
VARIABLE OLDLINES

: FGDEMO05 ( --- )
   FASTGRAPH?
   IF
      FGGETLINES OLDLINES !
      FGGETMODE OLDMODE !
      3 FGSETMODE
      0 FGCURSOR
      15 FGSETCOLOR
      " D‚monstration de FastGraph: 1Šre ligne (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      16 0 FGTESTMODE
      IF
         43 FGSETLINES
         0 FGCURSOR
         " Mode 43 Lignes" FGTEXT
         FGWAITKEY
      THEN
      17 0 FGTESTMODE
      IF
         50 FGSETLINES
         0 FGCURSOR
         " Mode 50 Lignes" FGTEXT
         FGWAITKEY
      THEN
      OLDMODE @ FGSETMODE
      OLDLINES @ FGSETLINES
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

