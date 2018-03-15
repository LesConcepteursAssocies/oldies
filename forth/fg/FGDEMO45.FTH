\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
HEX
CREATE CGAPATTERN
   0000 , 00FF , 00FF , 00FF ,
   02BB , 0000 , 0222 , 0255 ,
   00FF , 00FF , 00FF , 0055 ,
   00AA , 00AA , 00FF , 0277 ,
DECIMAL

VARIABLE OLDMODE
VARIABLE NEWMODE
13 STRING FICHIER$
FICHIER$ DROP 13 ERASE
" CORAL.PPR" FICHIER$ $!

: FGDEMO45 ( --- )
   FASTGRAPH?
   IF
      320 200 1 FGBESTMODE NEWMODE !
      NEWMODE @
      DUP 0< SWAP 12 = OR
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode 320x200"
      THEN
      FGGETMODE OLDMODE !
      NEWMODE @ FGSETMODE
      NEWMODE @ 4 =
      IF
         0 0 FGPALETTE
         16 0
         DO
            I CGAPATTERN I + @ FGPATTERN
         LOOP
      ELSE
         NEWMODE @
         DUP 9 = SWAP 13 = OR
         IF
            15 [ HEX ] 04F7 [ DECIMAL ] FGPATTERN
         ELSE
            15 38 38 38 FGSETRGB
         THEN
      THEN
      0 199 FGMOVE
      FICHIER$ DROP 320 1 FGDISPFILE
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

