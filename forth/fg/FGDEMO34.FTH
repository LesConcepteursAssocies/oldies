\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

4 CONSTANT FGPAGES

VARIABLE OLDMODE
VARIABLE NEWMODE
VARIABLE STATUS

: FGDEMO34 ( --- )
   FASTGRAPH?
   IF
      320 200 1 FGBESTMODE NEWMODE !
      NEWMODE @ DUP
      0< SWAP 12 = OR
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode 320x200"
      THEN
      FGGETMODE OLDMODE !
      NEWMODE @ FGSETMODE
      FGINITXMS STATUS !
      STATUS @
      0=
      IF
         8 FGALLOCXMS STATUS !
      THEN
      STATUS @
      0<
      IF
         FGINITEMS STATUS !
         STATUS @
         0=
         IF
            8 FGALLOCEMS STATUS !
         THEN
      THEN
      STATUS @
      0<
      IF
         8 FGALLOCCMS STATUS !
      THEN
      STATUS @
      0<
      IF
         OLDMODE @ FGSETMODE
         FGRESET
         1 FGCURSOR
         ABORT" Impossible de cr‚er une page logique"
      THEN
      7 FGSETCOLOR
      0 319 0 199 FGRECT
      9 FGSETCOLOR
      " D‚monstration de FastGraph (Appuyez sur une touche)" FGTEXT
      FGWAITKEY
      0 8 FGCOPYPAGE
      FGERASE
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      8 0 FGCOPYPAGE
      FGWAITKEY
      8 FGFREEPAGE DROP
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

