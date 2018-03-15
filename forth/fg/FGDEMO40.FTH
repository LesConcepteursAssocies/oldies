\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
CREATE TRIANGLE
   1 C, 9 C, 7 C, 1 C, 1 C, 1 C, 2 C, 5 C, 1 C, 1 C, 7 C, 3 C, 1 C, 1 C, 2 C, 3 C,
   1 C, 1 C, 7 C, 5 C, 1 C, 1 C, 2 C, 1 C, 1 C, 1 C, 7 C, 7 C, 1 C, 1 C, 7 C, 4 C,

VARIABLE OLDMODE
VARIABLE NEWMODE

: FGDEMO40 ( --- )
   FASTGRAPH?
   IF
      320 200 1 FGBESTMODE NEWMODE !
      NEWMODE @ DUP
      0< SWAP 12 = OR
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode 320x200"
      THEN
      FGGETMODE OLDMODE !
      NEWMODE @ FGSETMODE
      7 FGSETCOLOR
      0 319 0 199 FGRECT
      156 101 FGMOVE
      TRIANGLE 16 9 FGDISPLAY
      " (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

