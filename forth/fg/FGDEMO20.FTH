\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

10 CONSTANT VERTICES
CREATE X
   200 , 300 , 400 , 400 , 300 , 240 , 160 , 160 , 200 , 200 ,
CREATE Y
   100 ,  80 , 100 , 220 , 320 , 320 , 240 , 200 , 160 , 160 ,

VARIABLE OLDMODE

: FGDEMO20 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      16 1 FGTESTMODE
      IF
         16 FGSETMODE
      ELSE
         15 1 FGTESTMODE
         IF
            15 FGSETMODE
         ELSE
            ABORT" Cette d‚monstration n‚c‚ssite le mode 640x350"
         THEN
      THEN
      1 FGSETCOLOR
      X Y VERTICES FGPOLYGON
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

