\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE

: FGDEMO55 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      FGAUTOMODE FGSETMODE
      2 FGSETCOLOR
      0 FGGETMAXX 0 FGGETMAXY FGRECT
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      15 FGSETCOLOR
      0 FGFADEOUT
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg� !!" CR
   THEN
;

EOF

