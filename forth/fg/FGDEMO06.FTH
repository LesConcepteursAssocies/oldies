\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE

: FGDEMO06 ( --- )
   FASTGRAPH?
   IF
      FGEGACHECK 2 <
      IF
         ABORT" Cette d‚monstration  n‚c‚ssite une carte EGA"
      THEN
      FGGETMODE OLDMODE !
      16 FGSETMODE
      15 FGSETCOLOR
      " D‚monstration de FastGraph en mode EGA (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

