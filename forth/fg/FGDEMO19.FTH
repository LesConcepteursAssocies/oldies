\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE NEWMODE
VARIABLE MAXX
VARIABLE MAXY
VARIABLE MIDX
VARIABLE MIDY

: FGDEMO19 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      FGAUTOMODE NEWMODE !
      NEWMODE @ FGSETMODE
      FGGETMAXX MAXX !
      FGGETMAXY MAXY !
      MAXX @ 2/ MIDX !
      MAXY @ 2/ MIDY !
      15 FGSETCOLOR
      MIDX @ 0 FGMOVE
      MIDX @ MAXY @ 13107 FGDASH
      0 MIDY @ FGMOVE
      MAXX @ MIDY @ 13107 FGDASH
      " D�monstration de FastGraph (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg� !!" CR
   THEN
;

EOF

