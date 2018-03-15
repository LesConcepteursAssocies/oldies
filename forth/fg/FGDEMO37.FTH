\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
HEX
CREATE TRIANGLE
  55 C, 55 C, 40 C, 1A C, A9 C, 00 C, 06 C, A4 C, 00 C, 01 C,
  90 C, 00 C, 00 C, 40 C, 00 C,
DECIMAL

VARIABLE OLDMODE

: FGDEMO37 ( --- )
   FASTGRAPH?
   IF
      4 1 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode CGA 320x200"
      THEN
      FGGETMODE OLDMODE !
      4 FGSETMODE
      7 FGSETCOLOR
      0 319 0 199 FGRECT
      156 101 FGMOVE
      TRIANGLE 3 5 FGDRWIMAGE
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

