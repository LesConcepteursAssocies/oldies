\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE
VARIABLE MSTATUS

: FGDEMO61 ( --- )
   FASTGRAPH?
   IF
      -1 FGSETMODE
      FGMOUSEINI MSTATUS !
      MSTATUS @
      0<
      IF
         CR ." Pas de souris disponible"
      ELSE
         CR ." Souris disponible avec " MSTATUS @ . ."  Boutons"
      THEN
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

