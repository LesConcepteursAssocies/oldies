\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE MIDX
VARIABLE MIDY
VARIABLE X
VARIABLE Y

: FGDEMO21 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      FGAUTOMODE FGSETMODE
      15 FGSETCOLOR
      FGGETMAXX 2/ MIDX !
      FGGETMAXY 2/ MIDY !
      MIDX @ 8 / X !
      MIDY @ 8 / Y !
      MIDX @ MIDY @ FGMOVE
      X @ Y @ FGELLIPSE
      X @ FGCIRCLE
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

