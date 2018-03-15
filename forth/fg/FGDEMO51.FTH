\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE
VARIABLE NEWMODE

: FGDEMO51 ( --- )
   FASTGRAPH?
   IF
      320 200 1 FGBESTMODE NEWMODE !
      NEWMODE @
      DUP 0< SWAP 12 = OR
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode 320x200"
      THEN
      FGGETMODE OLDMODE !
      NEWMODE @ FGSETMODE
      15 FGSETCOLOR
      0 319 0 199 FGRECT
      320 -20
      DO
         10 FGSETCOLOR
         I I 19 + 95 104 FGCLPRECT
         1 FGWAITFOR
         0 FGSETCOLOR
         I I 19 + 95 104 FGCLPRECT
      5 +LOOP
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

