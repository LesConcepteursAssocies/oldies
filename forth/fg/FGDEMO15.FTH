\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE ROUGE
VARIABLE VERT
VARIABLE BLEU

: FGDEMO15 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      19 FGSETMODE
      103 FGSETCOLOR
      " D‚monstration de FastGraph" FGTEXT
      18 FGWAITFOR
      103 ROUGE VERT BLEU FGGETRGB
      BEGIN
         ROUGE @ VERT @ BLEU @
         + + 0>
      WHILE
         ROUGE @ 0>
         IF
            ROUGE 1-!
         THEN
         VERT @ 0>
         IF
            VERT 1-!
         THEN
         BLEU @ 0>
         IF
            BLEU 1-!
         THEN
         103 ROUGE VERT BLEU FGSETRGB
         1 FGWAITFOR
      REPEAT
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

