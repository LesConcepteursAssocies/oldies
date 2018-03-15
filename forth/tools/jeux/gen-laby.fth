\ -----------------------------------------------------------------------------
\            G‚n‚rateur al‚atoire de labyrinthe et affichage graphique.
\
\ Par Guillaumaud Philippe
\
\ Contrainte :
\ ------------
\ Une carte graphique de type C.G.A minimum est n‚c‚ssaire pour ‚x‚cuter
\ ce programme.
\ -----------------------------------------------------------------------------

EXIST? RND NOT ?\ INCLUDE RND
EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES

DECIMAL

\ D‚finition de quelques outils n‚c‚ssaires au programme

\ Positionne le curseur de l'‚cran en haut … gauche, sans effacer celui-ci.
: HOME  ( --- )
  27 (CONS)
  91 (CONS)
  102 (CONS)
;

VARIABLE SAUVE     \ utilis‚e dans certaines d‚finitions (n‚c‚ssaire …
                   \ cause des appels r‚cursifs de CHEMIN, qui satureraient
                   \ rapidement la pile de retour).

\ le mot suivant peut ˆtre remplac‚ par >R dans les d‚finitions l'utilisant
\ si le contexte (r‚cursivit‚) le permet.
: SAUVE!  ( n --- )
  SAUVE !
;

\ le mot suivant peut ˆtre remplac‚ par R> (ou R@) dans les d‚finitions
\ l'utilisant si le contexte (r‚cursivit‚) le permet.
: SAUVE@  ( --- n )
  SAUVE @
;

\ Passage en base binaire
: BINARY  ( --- )
  2 BASE !
;

HEX

\ Adresse du segment de l'‚cran C.G.A.
\
\ Rappel des principaux modes graphiques valides sur P.C:
\
\ +------+-------+------------+----------+--------------------+
\ | Mode | Carte | R‚solution | Couleurs | Segment de l'‚cran |
\ +------+-------+------------+----------+--------------------+
\ |   3  |  CGA  | texte 25x80|    16    |       0B800h       |
\ |   4  |  CGA  |   320x200  |     4    |       0B800h       |
\ |   5  |  CGA  |   320x200  |     4    |       0B800h       |
\ |   6  |  CGA  |   640x200  |     2    |       0B800h       |
\ |  13  |  EGA  |   320x200  |    16    |       0A000h       |
\ |  14  |  EGA  |   640x200  |    16    |       0A000h       |
\ |  15  |  EGA  |   640x350  |   mono.  |       0A000h       |
\ |  16  |  EGA  |   640x350  |    16    |       0A000h       |
\ |  17  | MCGA  |   640x480  |     2    |       0A000h       |
\ |  18  |  VGA  |   640x480  |    16    |       0A000h       |
\ |  19  | MCGA  |   320x200  |   256    |       0A000h       |
\ +------+-------+------------+----------+--------------------+
\
0B800 CONSTANT ECRAN

\ Cr‚ation de lutins (sprites, en anglais) de dimensions 8x8 points en C.G.A.
\ Au moment de la cr‚ation de lutins, ce mot doit trouver
\ sur la pile de donn‚es le nombre n‚c‚ssaire de mots 
\ d‚finissants les lutins, ainsi que le nombre total de
\ ces lutins plus 1 (le premier ayant le num‚ro 0).
\
\ A l'‚x‚cution du mot cr‚‚, empiler le num‚ro du lutin auquel
\ on veut acc‚der.
\
: CREER-LUTIN-CGA8X8 ( [data.1/0..data.8/0]..[data.1/n..data.8/n] n+1 --- )
  CREATE
    DUP 1- ,       \ Stockage du nombre de lutins.
    0              \ Pour n+1 lutins
    DO
      8 0
      DO
        FLIP       \ Inversion de chaque mot (utile pour l'affichage)
        ,          \ et stockage.
      LOOP
    LOOP
  DOES>  ( Nø_lutin [PFA] --- adr_lutin )
    2DUP @         \ V‚rification de d‚passement de la zone.
    U> ABORT" CREER-LUTIN-CGA8X8: Indice hors limites"
    DUP @ ROT -    \ Calcul de l'adresse de d‚part de la zone contenant
    10 * 2+ +      \ la d‚finition du lutin choisi.
;

\ D‚finition des lutins du programme
\ (‚l‚ments du labyrinthe)
\
\ NE PAS MODIFIER L'ORDRE DANS LEQUEL CEUX-CI SONT DEFINIS.
\ En effet, le OU logique est utilis‚ pour fabriquer le lutin correspondant
\ … un ‚tat donn‚, pour l'affichage (voir les mots de d‚placements plus bas).
\
\ Exemple : l'impasse haute Ú¿ ayant le Nø 1, l'impasse gauche ÚÄ ayant le Nø 2,
\                                                              ÀÄ
\           1 OR 2 = 3, ce qui correspond … ÚÄÄ
\                                           ³ Ú

BINARY

\ Num‚ro 0
1010101010101010
1011111111111110
1011000000001110   \ R‚pr‚sentation graphique
1011000000001110   \         ÚÄÄÄ¿
1011000000001110   \         ³   ³
1011000000001110   \         ÀÄÄÄÙ
1011111111111110
1010101010101010
\ Num‚ro 1
1010101010101010
1011111111111110
1011000000001110   \ R‚pr‚sentation graphique
1011000000001110   \         ÚÄÄÄ¿
1011000000001110   \         ³   ³
1011000000001110
1011000000001110
1011000000001110
\ Num‚ro 2
1010101010101010
1011111111111111
1011000000000000   \ R‚pr‚sentation graphique
1011000000000000   \         ÚÄÄÄ
1011000000000000   \         ³
1011000000000000   \         ÀÄÄÄ
1011111111111111
1010101010101010
\ Num‚ro 3
1010101010101010
1011111111111111
1011000000000000   \ R‚pr‚sentation graphique
1011000000000000   \         ÚÄÄÄÄ
1011000000000000   \         ³
1011000000000000   \         ³   Ú
1011000000001111
1011000000001110
\ Num‚ro 4
1011000000001110
1011000000001110
1011000000001110   \ R‚pr‚sentation graphique
1011000000001110   \
1011000000001110   \         ³   ³
1011000000001110   \         ÀÄÄÄÙ
1011111111111110
1010101010101010
\ Num‚ro 5
1011000000001110
1011000000001110
1011000000001110   \ R‚pr‚sentation graphique
1011000000001110   \
1011000000001110   \         ³   ³
1011000000001110   \         ³   ³
1011000000001110
1011000000001110
\ Num‚ro 6
1011000000001110
1011000000001111
1011000000000000   \ R‚pr‚sentation graphique
1011000000000000   \         ³   À
1011000000000000   \         ³
1011000000000000   \         ÀÄÄÄÄ
1011111111111111
1010101010101010
\ Num‚ro 7
1011000000001110
1011000000001111
1011000000000000   \ R‚pr‚sentation graphique
1011000000000000   \         ³   À
1011000000000000   \         ³
1011000000000000   \         ³   Ú
1011000000001111
1011000000001110
\ Num‚ro 8
1010101010101010
1111111111111110
0000000000001110   \ R‚pr‚sentation graphique
0000000000001110   \          ÄÄÄ¿
0000000000001110   \             ³
0000000000001110   \          ÄÄÄÙ
1111111111111110
1010101010101010
\ Num‚ro 9
1010101010101010
1111111111111110
0000000000001110   \ R‚pr‚sentation graphique
0000000000001110   \         ÄÄÄÄ¿
0000000000001110   \             ³
0000000000001110   \         ¿   ³
1111000000001110
1011000000001110
\ Num‚ro 10
1010101010101010
1111111111111111
0000000000000000   \ R‚pr‚sentation graphique
0000000000000000   \         ÄÄÄÄÄ
0000000000000000   \
0000000000000000   \         ÄÄÄÄÄ
1111111111111111
1010101010101010
\ Num‚ro 11
1010101010101010
1111111111111111
0000000000000000   \ R‚pr‚sentation graphique
0000000000000000   \         ÄÄÄÄÄ
0000000000000000   \
0000000000000000   \         ¿   Ú
1111000000001111
1011000000001110
\ Num‚ro 12
1011000000001110
1111000000001110
0000000000001110   \ R‚pr‚sentation graphique
0000000000001110   \         Ù   ³
0000000000001110   \             ³
0000000000001110   \         ÄÄÄÄÙ
1111111111111110
1010101010101010
\ Num‚ro 13
1011000000001110
1111000000001110
0000000000001110   \ R‚pr‚sentation graphique
0000000000001110   \         Ù   ³
0000000000001110   \             ³
0000000000001110   \         ¿   ³
1111000000001110
1011000000001110
\ Num‚ro 14
1011000000001110
1111000000001111
0000000000000000   \ R‚pr‚sentation graphique
0000000000000000   \         Ù   À
0000000000000000   \
0000000000000000   \         ÄÄÄÄÄ
1111111111111111
1010101010101010
\ Num‚ro 15
1011000000001110
1111000000001111
0000000000000000   \ R‚pr‚sentation graphique
0000000000000000   \         Ù   À
0000000000000000   \
0000000000000000   \         ¿   Ú
1111000000001111
1011000000001110
\ Num‚ro 16
0000000000000000
0000000000000000
0000010101010000   \ R‚pr‚sentation graphique
0000010101010000   \
0000010101010000   \           Ú¿
0000010101010000   \           ÀÙ
0000000000000000
0000000000000000

HEX

11 CREER-LUTIN-CGA8X8 LUTINS-CGA8X8

\ Affichage de lutins de dimensions 8x8 points … la position X,Y en C.G.A.
\
\ L'affichage se fait en ‚crivant directement dans la R.A.M ‚cran,
\ pour des raisons de rapidit‚.
\
: .LUTINS-CGA8X8 ( X Y Nø_lutin --- )
  LUTINS-CGA8X8
  ROT 2*
  ROT 140 * +        \ Calcul de l'offset dans l'‚cran.
  SAUVE!
  8 0
  DO
    DUP @ SWAP 2+  \ R‚cup‚ration des mots d‚finissants le lutin choisi.
  LOOP
  DROP
  SAUVE@
  4 0              \ L'affichage se fait par mots (plus rapide).
  DO
    SAUVE!
    SAUVE@ ECRAN SWAP L!          \ Affichage de la ligne paire.
    SAUVE@ 2000 + ECRAN SWAP L!   \ Affichage de la ligne impaire.
    SAUVE@ 50 +                   \ Nouvel offset d'‚cran (ligne suivante).
  LOOP
  DROP
;

DECIMAL

\ Cr‚ation d'un ‚cran virtuel de travail de 25 lignes et 40 colonnes.
\
\ AccŠs : X compris entre 0 et 39, Y compris entre 0 et 24, la position
\         0,0 correspondant au coin sup‚rieur gauche de l'‚cran.
\
\ Chaque "case" de l'‚cran contient un num‚ro de lutin.
\
VARIABLE TEMPS-REEL     \ VRAI si l'affichage se fait en "temps r‚el"
                        \ (au fur et … mesure de la construction du
                        \ labyrinthe).
40 CONSTANT DIMX
25 CONSTANT DIMY
DIMX DIMY CREER-CMATRICE ECRAN-VIRTUEL

\ Remise … z‚ro de l'‚cran virtuel.
: RAZ-ECRAN-VIRTUEL  ( --- )
  HOME
  DIMY 0
  DO
    DIMX 0
    DO
      0 I J ECRAN-VIRTUEL C!
    LOOP
  LOOP
;

\ Affichage de l'‚cran virtuel.
: .ECRAN-VIRTUEL  ( --- )
  HOME
  DIMY 0
  DO
    DIMX 0
    DO
      I J 2DUP ECRAN-VIRTUEL C@ .LUTINS-CGA8X8
    LOOP
  LOOP
;

\ Calcule 2 puissance n.
\ empile 1 si n = 0.
\
: 2^  ( n --- 2^n )
  1 SWAP 0
  ?DO
    2*
  LOOP
;

VARIABLE MURS      \ Utilis‚e pour conserver les divers ‚l‚ments du
                   \ labyrinthe.

\ Tests de d‚passement.
\ Vrai si Y est correct.
: YMAX  ( X Y --- X Y flag )
  DUP DIMY 1- <
;

\ Vrai si X est correct.
: XMAX  ( X Y --- X Y flag )
  OVER DIMX 1- <
;

: YMIN  ( X Y --- X Y flag )
  DUP 0 >
;

: XMIN  ( X Y --- X Y flag )
  OVER 0 >
;

\ Empile un flag VRAI si on peut aller vers le bas de l'‚cran.
: BAS?  ( X Y --- X Y flag )
  YMAX             \ V‚rification de d‚passement vertical.
  IF
    2DUP 1+
    ECRAN-VIRTUEL  \ On regarde s'il n'y a pas d‚j… un mur … la nouvelle
    C@ 0=          \ position.
  ELSE
    FALSE
  THEN
;

\ Empile un flag VRAI si on peut aller vers la droite de l'‚cran.
: DROITE?  ( X Y --- X Y flag )
  XMAX             \ V‚rification de d‚passement horizontal.
  IF
    OVER 1+ OVER
    ECRAN-VIRTUEL  \ On regarde s'il n'y a pas d‚j… un mur … la nouvelle
    C@ 0=          \ position.
  ELSE
    FALSE
  THEN
;

\ Empile un flag VRAI si on peut aller vers le haut de l'‚cran.
: HAUT?  ( X Y --- X Y flag )
  YMIN             \ V‚rification de d‚passement vertical.
  IF
    2DUP 1-
    ECRAN-VIRTUEL  \ On regarde s'il n'y a pas d‚j… un mur … la nouvelle
    C@ 0=          \ position.
  ELSE
    FALSE
  THEN
;

\ Empile un flag VRAI si on peut aller vers la gauche de l'‚cran.
: GAUCHE?  ( X Y --- X Y flag )
  XMIN             \ V‚rification de d‚passement horizontal.
  IF
    OVER 1- OVER
    ECRAN-VIRTUEL  \ On regarde s'il n'y a pas d‚j… un mur … la nouvelle
    C@ 0=          \ position.
  ELSE
    FALSE
  THEN
;

\ D‚placement vers le bas et fabrication du lutin correspondant, pour
\ l'affichage.
: BAS  ( X Y --- X Y X Y+1 )
  2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 1 OR SWAP C!
  TEMPS-REEL @
  IF
    2DUP SAUVE@ C@ .LUTINS-CGA8X8
  THEN
  2DUP YMAX
  IF
    1+
    2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 4 OR SWAP C!
    TEMPS-REEL @
    IF
      2DUP SAUVE@ C@ .LUTINS-CGA8X8
    THEN
  THEN
;

\ D‚placement vers la droite et fabrication du lutin correspondant, pour
\ l'affichage.
: DROITE  ( X Y --- X Y X+1 Y )
  2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 2 OR SWAP C!
  TEMPS-REEL @
  IF
    2DUP SAUVE@ C@ .LUTINS-CGA8X8
  THEN
  2DUP XMAX
  IF
    SWAP 1+ SWAP
    2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 8 OR SWAP C!
    TEMPS-REEL @
    IF
      2DUP SAUVE@ C@ .LUTINS-CGA8X8
    THEN
  THEN
;

\ D‚placement vers le haut et fabrication du lutin correspondant, pour
\ l'affichage.
: HAUT  ( X Y --- X Y X Y-1)
  2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 4 OR SWAP C!
  TEMPS-REEL @
  IF
    2DUP SAUVE@ C@ .LUTINS-CGA8X8
  THEN
  2DUP YMIN
  IF
    1-
    2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 1 OR SWAP C!
    TEMPS-REEL @
    IF
      2DUP SAUVE@ C@ .LUTINS-CGA8X8
    THEN
  THEN
;

\ D‚placement vers la gauche et fabrication du lutin correspondant, pour
\ l'affichage.
: GAUCHE  ( X Y --- X Y  X-1 Y )
  2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 8 OR SWAP C!
  TEMPS-REEL @
  IF
    2DUP SAUVE@ C@ .LUTINS-CGA8X8
  THEN
  2DUP XMIN
  IF
    SWAP 1- SWAP
    2DUP ECRAN-VIRTUEL DUP SAUVE! DUP C@ 2 OR SWAP C!
    TEMPS-REEL @
    IF
      2DUP SAUVE@ C@ .LUTINS-CGA8X8
    THEN
  THEN
;

\ Le coeur du programme.
\ Principe : construit un chemin valide, … partir d'un position initiale.
: CHEMIN  ( X Y --- )
  RECURSIVE
  MURS OFF
  BEGIN
    4 RND 2^                 \ G‚nŠre une direction al‚atoire.
    DUP MURS @ OR DUP MURS !
    15 <
  WHILE
    DUP 1 =
    IF
      DROP BAS?
      IF
        BAS CHEMIN
      THEN
    ELSE
      DUP 2 =
      IF
        DROP DROITE?
        IF
          DROITE CHEMIN
        THEN
      ELSE
        DUP 4 =
        IF
          DROP HAUT?
          IF
            HAUT CHEMIN
          THEN
        ELSE
          DUP 8 =
          IF
            DROP GAUCHE?
            IF
              GAUCHE CHEMIN
            THEN
          ELSE
            DROP
          THEN
        THEN
      THEN
    THEN
  REPEAT
  DROP 2DROP
  MURS OFF
;

\ Programme principal.
: GEN-LABY
  ['] NOOP IS OVF-MATRICE    \ On n'effectue pas les tests de d‚passements (… cause de >R et R>).
  FALSE TEMPS-REEL !
  5 MODE
  ."   G‚n‚ration al‚atoire de labyrinthes."
  CR CR ." Appuyez sur une touche aprŠs l'affichage"
  CR ." Affichage temps r‚el O/N ?"
  KEY UPC ASCII O = TEMPS-REEL !
  CR CR ." Appuyez sur une touche..."
  BEGIN
    -1 RND DROP
    KEY?
  UNTIL
  KEY DROP
  5 MODE
  TEMPS-REEL @ NOT
  IF
    ." Construction du labyrinthe en cours..."
  THEN
  RAZ-ECRAN-VIRTUEL
  DIMX 2/ DIMY 2/ CHEMIN
  TEMPS-REEL @ NOT
  IF
    .ECRAN-VIRTUEL
  THEN
  KEY DROP
  3 MODE
  ['] (OVF-MATRICE) IS OVF-MATRICE
;

CR .( Tapez GEN-LABY pour lancer le programme. )
EOF

