\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

4 CONSTANT FGPAGES

VARIABLE OLDMODE
VARIABLE FGCOLOR

: FGDEMO31 ( --- )
   FASTGRAPH?
   IF
      1 FGPAGES FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite de la couleur"
      THEN
      FGGETMODE OLDMODE !
      1 FGSETMODE
      FGPAGES 0
      DO
         I FGSETPAGE
         I 1+ FGCOLOR !
         FGCOLOR @ FGSETCOLOR
         0 FGGETMAXX 0 FGGETMAXY FGRECT
         0 FGCOLOR @ 0 FGSETATTR
         12 17 FGLOCATE
         " PAGE Nø " FGTEXT
         I (.) FGTEXT
      LOOP
      FGPAGES 0
      DO
         I FGSETVPAGE
         18 FGWAITFOR
      LOOP
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

