\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE
VARIABLE HIDDEN
VARIABLE X

: FGDEMO54 ( --- )
   FASTGRAPH?
   IF
      13 3 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode EGA"
      THEN
      FGGETMODE OLDMODE !
      13 FGSETMODE
      2 FGSETPAGE
      1 FGSETCOLOR
      0 319 0 199 FGRECT
      15 FGSETCOLOR
      160 100 FGMOVE
      20 20 FGELLIPSE

      10 FGSETCOLOR
      1 HIDDEN !
      320 -10
      DO
         HIDDEN @ FGSETPAGE
         0 319 0 199 0 199 2 HIDDEN @ FGTRANSFER
         I I 19 + 96 105 FGCLPRECT
         HIDDEN @ FGSETVPAGE
         1 HIDDEN @ - HIDDEN !
         1 FGWAITFOR
      4 +LOOP
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

