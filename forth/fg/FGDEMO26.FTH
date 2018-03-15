\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE

: FGDEMO26 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      3 FGSETMODE
      0 FGCURSOR

      14 0 0 FGSETATTR
      " Jaune " FGTEXT
      10 0 0 FGSETATTR
      " Vert " FGTEXT
      12 0 1 FGSETATTR
      " Clignotant" FGTEXT
      12 7 0 FGSETATTR
      24 0 FGLOCATE
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

