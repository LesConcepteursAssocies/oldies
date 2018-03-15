\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL

: FGDEMO66 ( --- )
   FASTGRAPH?
   IF
      CR ." Son 20 Hz"
      20 3 FGSOUND
      CR ." Son 100 Hz"
      100 3 FGSOUND
      18 FGWAITFOR
      CR ." Son 1000 Hz"
      1000 3 FGSOUND
      18 FGWAITFOR
      CR ." WARBLE..."
      400 1 FGSOUND
      410 1 FGSOUND
      400 1 FGSOUND
      410 1 FGSOUND
      18 FGWAITFOR
      CR ." de 100 … 500 Hz"
      500 100
      DO
         I 2 FGSOUND
      LOOP
   ELSE
      CR ." Le driver FastGraph n'est pas charg‚ !!" CR
   THEN
;

EOF

