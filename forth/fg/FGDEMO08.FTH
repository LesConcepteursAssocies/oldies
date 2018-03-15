\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE NEWMODE

: FGDEMO08 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      320 200 1 FGBESTMODE NEWMODE !
      NEWMODE @ 0<
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode graphique 320x200"
      THEN
      NEWMODE @ FGSETMODE
      15 FGSETCOLOR
      " D‚monstration de FastGraph en mode " FGTEXT
      NEWMODE @ (.) FGTEXT
      "  (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

