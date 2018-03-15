\ -----------------------------------------------------------------------------
\               G‚n‚rateur de puzzle graphiques en mode 320x200.
\
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ Chargement des outils.
EXIST? (EKEY) NOT ?\ INCLUDE EKEY
EXIST? (EGA-LIT-PCX) NOT ?\ INCLUDE PCX_LIB

DECIMAL

\ Quelques variables
VARIABLE PLARG                    \ Largeur d'une piŠce du puzzle.
VARIABLE PHAUT                    \ Hauteur d'une piŠce du puzzle.
VARIABLE NLP                      \ Nombre de lignes et de colonnes du puzzle
VARIABLE NCP                      \ (fonction de la largeur des piŠces).

\ Initialise les variables NLP et NCP.
: NLP!  ( --- )
  200 PHAUT @ / 1- NLP !
;

: NCP!  ( --- )
  200 PLARG @ / 1- NCP !
;

\ Affichage du menu.
: .MENU  ( --- )
  200 0
  DO
    200 I 15 (EGA-PSET)
  LOOP
  (EGA-*ECR->ECR)
  27 0 AT ." Menu g‚n‚ral"
;

80 STRING0 FICTEST
" \L\TF\PG\TOOLS\GRAPHISM\FORMATS\IMGEGA.PCX" FICTEST $!

: TEST  ( --- )
  (EGA-*INIT)
  NLP! NCP!
  13 GRMODE !
  DARK (EGA-ECR->*ECR)
  .MENU
  FICTEST LIT-PCX

  (*EGA-ECRAN) (EGA-ECRAN) 0 0 50 200 (EGA-!IMAGE)
  BEGIN KEY #SPC = UNTIL
  (*EGA-ECRAN) (EGA-ECRAN) 50 0 50 200 (EGA-!IMAGE)
  BEGIN KEY #SPC = UNTIL
  (*EGA-ECRAN) (EGA-ECRAN) 100 0 50 200 (EGA-!IMAGE)
  BEGIN KEY #SPC = UNTIL
  (*EGA-ECRAN) (EGA-ECRAN) 150 0 50 200 (EGA-!IMAGE)
  BEGIN
    KEY #ESC =
  UNTIL
  3 MODE
  (*EGA-ECRAN) RELEASE
  (*EGA-SAUVE) RELEASE
;

EOF

