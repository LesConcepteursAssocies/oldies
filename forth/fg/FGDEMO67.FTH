\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

: FGDEMO67 ( --- )
   FASTGRAPH?
   IF
      9 0 FGTESTMODE
      0=
      IF
         ABORT" Cette d‚monstration n‚c‚ssite un Tandy 100 ou un PCJr"
      THEN
      CR ." Son p‚riodique 2048 Hz"
      4 2 15 3 FGVOICE
      18 FGWAITFOR
      CR ." Bruit blanc 2048 Hz"
      5 2 15 3 FGVOICE
      18 FGWAITFOR
      CR ." Incr‚mentation du volume"
      15 1
      DO
         1 500 I 0 FGVOICE
         4 FGWAITFOR
      LOOP
      FGQUIET
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

