\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE

: FGDEMO35 ( --- )
   FASTGRAPH?
   IF
      13 1 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode EGA 320x200"
      THEN
      FGGETMODE OLDMODE !
      13 FGSETMODE
      9 FGSETCOLOR
      0 38 FGLOCATE
      " Redimensionnement (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      FGERASE
      640 200 FGRESIZE
      10 FGSETCOLOR
      0 38 FGLOCATE
      " Redimensionnement (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

