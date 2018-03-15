\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE

: FGDEMO52 ( --- )
   FASTGRAPH?
   IF
      13 1 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite une carte EGA"
      THEN
      FGGETMODE OLDMODE !
      13 FGSETMODE
      3 FGSETFUNC
      15 FGSETCOLOR
      0 319 0 199 FGRECT
      10 15 OR FGSETCOLOR
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

