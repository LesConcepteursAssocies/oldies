\
\ Test de FGDRIVER
\
EXIST? FASTGRAF NOT ?\ INCLUDE FGDRIVER.FTH

ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
DECIMAL
VARIABLE OLDMODE
VARIABLE X
VARIABLE Y
VARIABLE FGKEY
VARIABLE FGAUX

: FGDEMO60 ( --- )
   FASTGRAPH?
   IF
      FGGETMODE OLDMODE !
      13 FGSETMODE
      640 400 FGRESIZE
      2 FGSETCOLOR
      0 FGGETMAXX 0 FGGETMAXY FGRECT
      15 FGSETCOLOR
      0 FGGETMAXX 0 FGGETMAXY FGBOX
      24 1 FGLOCATE
      " Appuyez sur les flŠches ou Esc" FGTEXT
      0 X !
      0 Y !
      BEGIN
         FGKEY FGAUX FGGETKEY
         FGAUX @ 72 = Y @ 200 < AND
         IF
            Y 1+!
         ELSE
            FGAUX @ 75 = X @ 320 < AND
            IF
               X 1+!
            ELSE
               FGAUX @ 77 = X @ 0 > AND
               IF
                  X 1-!
               ELSE
                  FGAUX @ 80 = Y @ 0 > AND
                  IF
                     Y 1-!
                  THEN
               THEN
            THEN
         THEN
         X @ Y @ FGPAN
         FGKEY @ 27 =
      UNTIL
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

