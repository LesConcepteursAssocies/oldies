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

\ Un nouveau type de donn‚e : le type POINTEUR.
\ C'est lui-mˆme une sous-classe de la classe DATA.
DATA SUBCLASS POINTEUR
4 FOR DATASIZE               \ Un pointeur occupe 4 octets (segment:offset)
PROCEDURE P@                 \ Pointer Fetch.
PROCEDURE P!                 \ Pointer Store.
PROCEDURE P1+!               \ Incr‚mentation d'un pointeur.
PROCEDURE P1-!               \ D‚cr‚mentation d'un pointeur.

\ On peut avoir deux types de pointeurs :
\  1) Un pointeur sur un octet.
POINTEUR SUBCLASS CPOINTEUR

METHODS DEFINITIONS
\ Incr‚mente le pointeur.
: (CP1+!)  ( padr --- )
  DUP 2@ 1+ ROT 2!
;
THAT METHOD P1+!

\ D‚cr‚mente le pointeur.
: (CP1-!)  ( padr --- )
  DUP 2@ 1- ROT 2!
;
THAT METHOD P1-!

\ Empile le contenu point‚.
: (CP@)  ( padr --- o )
  2@ LC@
;
THAT METHOD P@

\ Initialise le contenu point‚.
: (CP!)  ( o padr --- )
  2@ LC!
;
THAT METHOD P!
FORTH DEFINITIONS

\  2) Un pointeur sur un mot.
POINTEUR SUBCLASS WPOINTEUR

METHODS DEFINITIONS
\ Incr‚mente le pointeur.
: (WP1+!)  ( padr --- )
  DUP 2@ 2+ ROT 2!
;
THAT METHOD P1+!

\ D‚cr‚mente le pointeur.
: (WP1-!)  ( padr --- )
  DUP 2@ 2- ROT 2!
;
THAT METHOD P1-!

\ Empile le contenu point‚.
: (WP@)  ( padr --- n )
  2@ L@
;
THAT METHOD P@

\ Initialise le contenu point‚.
: (WP!)  ( n padr --- )
  2@ L!
;
THAT METHOD P!
FORTH DEFINITIONS

EOF

\ -----------------------------------------------------------------------------
\ Exemple d'utilisation :
\ Pointeur sur la RAM ‚cran.
WPOINTEUR OBJECT VIDEORAM
HEX B800 0 VIDEORAM 2! DECIMAL

\ Effacement de l'‚cran.
: CLRSCR  ( --- )
  VIDEORAM 2@
  2000 0                     \ 25 lignes de 80 colonnes
  DO
    0 VIDEORAM P!            \ Remplies avec des 0.
    VIDEORAM P1+!
  LOOP
  VIDEORAM 2!
;

