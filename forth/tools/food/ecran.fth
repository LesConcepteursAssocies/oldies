\ -----------------------------------------------------------------------------
\                             Turbo Forth Orient‚ Objet.
\
\ Exemple de d‚finition de type de donn‚es suppl‚mentaire.
\
\ Version 0.00.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

EXIST? CLASS NOT ?\ INCLUDE FOOD
WARNING OFF

\ Cr‚ation d'un nouveau type de donn‚e : la classe ECRAN.

CLASS ECRAN
CLASSVAR #LIGNES             \ Nombre de lignes.
CLASSVAR #COLONNES           \ Nombre de colonnes.

PROCEDURE OBJECT
PROCEDURE ?INDICE            \ V‚rification des indices.
PROCEDURE CELL               \ Adresse de l'‚l‚ment.
PROCEDURE E@                 \ Ecran Fetch.
PROCEDURE E!                 \ Ecran Store.
PROCEDURE .E                 \ Affichage.

METHODS DEFINITIONS

\ V‚rification des indices.
: (?INDICE)  ( c l adr --- )
  2DUP @ U> ABORT" Indice de ligne hors limite"
  2 PICK OVER 2+ @ U> ABORT" Indice de colonne hors limite"
;
THAT METHOD ?INDICE

\ Empile l'adresse de l'‚l‚ment.
: (CELL)  ( c l adr --- adr )
  DUP 2+ @ ROT * ROT + 4 + +
;
THAT METHOD CELL

\ Cr‚ation d'un nouvel objet ECRAN.
: (ECRANCREATE)
  CREATE
    ACTIVE @ , IMMEDIATE
    [COMPILE] #LIGNES DUP ,
    [COMPILE] #COLONNES DUP ,
    * HERE OVER ERASE ALLOT
    IMMEDIATE
  DOES>
    DUP @ ACTIVE ! 2+
    STATE @
    IF
      [COMPILE] LITERAL
    THEN
;
THAT METHOD OBJECT

\ Empile l'‚l‚ment point‚.
: (E@)  ( c l adr --- o )
  [COMPILE] ?INDICE
  [COMPILE] CELL
  C@
;
THAT METHOD E@

\ Initialise un ‚l‚ment.
: (E!)  ( o c l adr --- )
  [COMPILE] ?INDICE
  [COMPILE] CELL
  C!
;
THAT METHOD E!

\ Affichage.
\ (uniquement … titre indicatif !)
: (.E)  ( adr --- )
  DUP @ 0
  DO
    DUP 2+ @ 0
    DO
      I J 2DUP AT
      2 PICK [COMPILE] CELL
      C@ EMIT
    LOOP
  LOOP
  DROP
;
THAT METHOD .E

FORTH DEFINITIONS

\ Taille par d‚faut d'un objet de type ECRAN.
25 FOR #LIGNES
80 FOR #COLONNES

\ Variation : Sous-classe FENETRE.
ECRAN SUBCLASS FENETRE
10 FOR #LIGNES               \ Taille par d‚faut
20 FOR #COLONNES
CLASSVAR #CPOS               \ Position de la fenˆtre.
CLASSVAR #LPOS               \ (colonne, ligne)

METHODS DEFINITIONS

\ Affichage d'une fenˆtre.
\ (toujours uniquement … titre indicatif !)
: (.F)  ( adr --- )
  DUP @ 0
  DO
    DUP 2+ @ 0
    DO
      I J 2DUP
      [COMPILE] #LPOS +
      SWAP [COMPILE] #CPOS + SWAP AT
      2 PICK [COMPILE] CELL
      C@ EMIT
    LOOP
  LOOP
  DROP
;
THAT METHOD .E

FORTH DEFINITIONS

EOF

Exemples d'utilisation :
ECRAN OBJECT ECRAN-VIDEO     \ On se fabrique un ‚cran virtuel.

ECRAN-VIDEO .E

FENETRE OBJECT SOUS-ECRAN
5 FOR #CPOS
3 FOR #LPOS

SOUS-ECRAN .E

