\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

VARIABLE OLDMODE
VARIABLE READSTATUS
VARIABLE WRITESTATUS
13 STRING FICHIER1$
FICHIER1$ DROP 13 ERASE
" CORAL.PCX" FICHIER1$ $!
13 STRING FICHIER2$
FICHIER2$ DROP 13 ERASE
" TEST.PCX" FICHIER2$ $!

: FGDEMO44 ( --- )
   FASTGRAPH?
   IF
      19 1 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite le mode MCGA 320x200"
      THEN
      FGGETMODE OLDMODE !
      19 FGSETMODE
      FICHIER1$ DROP 0 FGDISPPCX READSTATUS !
      " Appuyez sur une touche" FGTEXT
      FGWAITKEY
      READSTATUS @
      0=
      IF
         0 319 80 99 FICHIER2$ DROP FGMAKEPCX WRITESTATUS !
      THEN
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
      READSTATUS @ 1 =
      IF
         CR ." Fichier CORAL.PCX non trouv‚"
      THEN
      READSTATUS @ 2 =
      IF
         CR ." Le fichier CORAL.PCX n'est pas un fichier PCX"
      THEN
      WRITESTATUS @ 1 =
      IF
         CR ." Fichier TEST.PCX non cr‚‚"
      THEN
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

