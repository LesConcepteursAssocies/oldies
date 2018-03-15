\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

: FGDEMO02 ( --- )
   FASTGRAPH?
   IF
      3 FGSETMODE
      0 FGCURSOR
      15 FGSETCOLOR
      " D‚monstration de FASTGRAF (Appuyz sur une touche)" FGTEXT
      FGWAITKEY
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF


