\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE
2 CONSTANT DELAY
4 CONSTANT JUMP

: FGDEMO59 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      FGAUTOMODE FGSETMODE
      15 FGSETCOLOR
      0 JUMP FGMOVE
      FGGETMAXX JUMP FGDRAW
      FGGETMAXX FGGETMAXY JUMP - FGDRAW
      0 FGGETMAXY JUMP - FGDRAW
      0 JUMP FGDRAW
      18 FGWAITFOR
      6 0
      DO
         0 JUMP FGPAN
         DELAY FGWAITFOR
         0 0 FGPAN
         DELAY FGWAITFOR
      LOOP
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

