\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE

: FGDEMO10 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      4 FGSETMODE
      0 1 FGPALETTE
      3 FGSETCOLOR
      " D‚monstration de FastGraph" FGTEXT
      "  (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

