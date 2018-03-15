\ -----------------------------------------------------------------------------
\
\ Tests Divers.
\
\ -----------------------------------------------------------------------------

IFNDEF TIME INCLUDE PROGS\TIME.FTH

: TEST1
  CR ." 100.000 Boucles vides en "
  TIME
  100 0
  DO
    1000 0
    DO
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST2
  CR ." 100.000 Additions d'entiers simples en "
  TIME
  100 0
  DO
    1000 0
    DO
      2 2 + DROP
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST3
  CR ." 100.000 Multiplications d'entiers simples en "
  TIME
  100 0
  DO
    1000 0
    DO
      2 2 * DROP
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST4
  CR ." 100.000 Divisions d'entiers simples en "
  TIME
  100 0
  DO
    1000 0
    DO
      2 2 / DROP
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST5
  TIME
  1000 0
  DO
    I .
  LOOP
  TIME
  CR ." Affichage de 1000 nombres simples en "
  .INTERVAL CR
;

: TEST6
  CR ." 100.000 Additions d'entiers doubles en "
  TIME
  100 0
  DO
    1000 0
    DO
      2. 2. D+ 2DROP
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST7
  CR ." 100.000 Soustractions d'entiers doubles en "
  TIME
  100 0
  DO
    1000 0
    DO
      2. 2. D- 2DROP
    LOOP
  LOOP
  TIME
  .INTERVAL CR
;

: TEST8
  TIME 
  1000 0 
  DO 
    I S>D D. 
  LOOP 
  TIME
  CR ." Affichage de 1000 nombres doubles en "
  .INTERVAL CR
;

