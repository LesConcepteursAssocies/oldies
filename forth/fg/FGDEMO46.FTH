\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
CREATE BITMAP 32 ALLOT
VARIABLE OLDMODE
VARIABLE NEWMODE

: FGDEMO46 ( --- )
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
      9 FGSETCOLOR
      " D‚monstration de FastGraph (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      0 7 FGMOVE
      BITMAP 4 8 FGGETMAP
      4 15 FGMOVE
      BITMAP 4 8 FGDRAWMAP
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

