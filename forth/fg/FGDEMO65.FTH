\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
HEX
CREATE SOLID
   E03F , E03F , E03F , 0007 , 0007 , 0007 , 0007 , 0007 ,
   0007 , 0007 , E03F , E03F , E03F , FFFF , FFFF , FFFF ,
   0000 , 0F80 , 0F80 , 0F80 , 7FF0 , 7FF0 , 7FF0 , 7FF0 ,
   7FF0 , 0F80 , 0F80 , 0F80 , 0000 , 0000 , 0000 , 0000 ,
CREATE HOLLOW
   E03F , EFBF , EFBF , 0F87 , 7FF7 , 7FF7 , 7FF7 , 7FF7 ,
   7FF7 , 0F87 , EFBF , EFBF , E03F , FFFF , FFFF , FFFF ,
   0000 , 0000 , 0000 , 0000 , 0000 , 0000 , 0200 , 0000 ,
   0000 , 0000 , 0000 , 0000 , 0000 , 0000 , 0000 , 0000 ,
DECIMAL
VARIABLE OLDMODE
VARIABLE COLUMN
VARIABLE LASTROW

: FGDEMO65 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      FGAUTOMODE FGSETMODE
      FGMOUSEINI
      0<
      IF
         OLDMODE @ FGSETMODE
         FGRESET
         1 FGCURSOR
         ABORT" Cette d�monstration n�c�ssite une souris"
      THEN
      15 FGSETCOLOR
      0 FGGETMAXX 0 FGGETMAXY FGRECT
      12 FGSETCOLOR
      FGGETMAXX 2/ FGXALPHA 6 - COLUMN !
      FGGETMAXY FGYALPHA 1+ LASTROW !
      LASTROW @ 0
      DO
         I COLUMN @ FGLOCATE
         " D�monstration FastGraph" FGTEXT
      LOOP
      1 FGMOUSEVIS
      FGWAITKEY
      SOLID 6 6 FGMOUSEPTR
      FGWAITKEY
      HOLLOW 6 6 FGMOUSEPTR
      FGWAITKEY
      OLDMODE @ FGSETMODE
      FGRESET
      1 FGCURSOR
   ELSE
      CR ." Le driver FastGraph n'est pas charg� !!" CR
   THEN
;

EOF

