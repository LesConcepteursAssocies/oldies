\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ Un peu de musique...
\ (adapt‚ d'une id‚e de C. H. Ting)
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

64 CONSTANT CANAL            \ Adresse de base du timer 8253.

\ Initialise la g‚n‚ration de sons.
: INIT-TIMER  ( --- )
  97 PC@ 3 OR 97 PC!         \ Autorise l'‚mission de sons.
  182 CANAL 3 + PC!          \ Met le 8253 en mode 3.
;

\ Cr‚ation de la table des notes.
CREATE NOTES 256 ALLOT

NOTES 256 ERASE

\ Cr‚‚ et stocke une nouvelle note dans la table.
: NOTE  ( adr-note freq dur‚e --- adr-note' )
  11900 ROT */ OVER
  CONSTANT OVER ! 2+
;

\ D‚finition de 8 octaves.
\ Adresse de base de toutes les notes.
NOTES
\
\ Rappel pour les non musiciens :
\
\ C  C#  D  D#  E  F  F#  G   G#   A  A#  B      \ notation anglo-saxone.
\ Do Do# R‚ R‚# Mi Fa Fa# Sol Sol# La La# Si
\
\ Le symbole # est le diŠse.
\ 1er octave
4120  10000 NOTE E1           4365 10000 NOTE F1
4625  10000 NOTE F1#          4900 10000 NOTE G1
5191  10000 NOTE G1#          5500 10000 NOTE A1
5827  10000 NOTE A1#          6174 10000 NOTE B1
\ 2Šme octave
6541  10000 NOTE C2           6930 10000 NOTE C2#
7342  10000 NOTE D2           7778 10000 NOTE D2#
8241  10000 NOTE E2           8731 10000 NOTE F2
9250  10000 NOTE F2#          9800 10000 NOTE G2
10383 10000 NOTE G2#         11000 10000 NOTE A2
11654 10000 NOTE A2#         12347 10000 NOTE B2
\ 3Šme octave
13081 10000 NOTE C3          13859 10000 NOTE C3#
14683 10000 NOTE D3          15556 10000 NOTE D3#
16481 10000 NOTE E3          17461 10000 NOTE F3
18500 10000 NOTE F3#         19600 10000 NOTE G3
20765 10000 NOTE G3#         22000 10000 NOTE A3
23308 10000 NOTE A3#         24694 10000 NOTE B3
\ 4Šme octave
2616   1000 NOTE C4            2772 1000 NOTE C4#
2937   1000 NOTE D4            3111 1000 NOTE D4#
3296   1000 NOTE E4            3492 1000 NOTE F4
3700   1000 NOTE F4#           3920 1000 NOTE G4
4153   1000 NOTE G4#           4400 1000 NOTE A4
4662   1000 NOTE A4#           4939 1000 NOTE B4
\ 5Šme octave
5233   1000 NOTE C5            5544 1000 NOTE C5#
5873   1000 NOTE D5            6223 1000 NOTE D5#
6593   1000 NOTE E5            6985 1000 NOTE F5
7400   1000 NOTE F5#           7840 1000 NOTE G5
8306   1000 NOTE G5#           8800 1000 NOTE A5
9323   1000 NOTE A5#           9878 1000 NOTE B5
\ 6Šme octave
10465  1000 NOTE C6           11087 1000 NOTE C6#
11747  1000 NOTE D6           12445 1000 NOTE D6#
13185  1000 NOTE E6           13969 1000 NOTE F6
14800  1000 NOTE F6#          15680 1000 NOTE G6
16612  1000 NOTE G6#          17600 1000 NOTE A6
18647  1000 NOTE A6#          19755 1000 NOTE B6
\ 7Šme octave
2093    100 NOTE C7             2218 100 NOTE C7#
2349    100 NOTE D7             2489 100 NOTE D7#
2637    100 NOTE E7             2794 100 NOTE F7
2960    100 NOTE F7#            3136 100 NOTE G7
3322    100 NOTE G7#            3520 100 NOTE A7
3729    100 NOTE A7#            3951 100 NOTE B7
\ 8Šme octave
4186    100 NOTE C8             4435 100 NOTE C8#
4699    100 NOTE D8             4978 100 NOTE D8#
5274    100 NOTE E8             5588 100 NOTE F8
5920    100 NOTE F8#            6272 100 NOTE G8
6645    100 NOTE G8#            7040 100 NOTE A8
7459    100 NOTE A8#            7902 100 NOTE B8

2975 1 NOTE Z                 \ silence
DROP

\ Les b‚mols.
C2# CONSTANT D2b    D2# CONSTANT E2b   F2# CONSTANT G2b
C3# CONSTANT D3b    D3# CONSTANT E3b   F3# CONSTANT G3b
C4# CONSTANT D4b    D4# CONSTANT E4b   F4# CONSTANT G4b
C5# CONSTANT D5b    D5# CONSTANT E5b   F5# CONSTANT G5b
C6# CONSTANT D6b    D6# CONSTANT E6b   F6# CONSTANT G6b
C7# CONSTANT D7b    D7# CONSTANT E7b   F7# CONSTANT G7b
C8# CONSTANT D8b    D8# CONSTANT E8b   F8# CONSTANT G8b

G1# CONSTANT A1b   A1# CONSTANT B1b
G2# CONSTANT A2b   A2# CONSTANT B2b
G3# CONSTANT A3b   A3# CONSTANT B3b
G4# CONSTANT A4b   A4# CONSTANT B4b
G5# CONSTANT A5b   A5# CONSTANT B5b
G6# CONSTANT A6b   A6# CONSTANT B6b
G7# CONSTANT A7b   A7# CONSTANT B7b

\ Joue une note.
: PLAY  ( note-addr --- )
  @                          \ Prend la note.
  DUP CANAL 2+ PC!           \ Octet faible du d‚lai.
  FLIP CANAL 2+ PC!          \ Octet fort.
;

\ Joue un silence (!).
: |  ( --- )
  Z PLAY
;

\ Idem.
: HUSH
  |
;

INIT-TIMER
HUSH
DECIMAL

EOF

