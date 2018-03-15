\ -----------------------------------------------------------------------------
\                             Le serpent bidulovore !
\
\ Le jeu du serpent en mode texte.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

EXIST? HFILE NOT ?\ INCLUDE DFILES
EXIST? RND NOT ?\ INCLUDE RND
EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES
EXIST? (EKEY) NOT ?\ INCLUDE EKEY

DECIMAL

\ Cr‚ation de variables chaine se terminant par 0.
\ (Utile pour stocker les noms de fichiers DOS)
: STRING0  ( long_max nom_chaine --- )
  CREATE
    DUP C, 0 C,
    1+ ALLOT
  DOES>
    1+ COUNT 2DUP +
    0 SWAP C!
;

\ Constantes couleurs.
0  CONSTANT NOIR
1  CONSTANT BLEU
2  CONSTANT VERT
3  CONSTANT CYAN
4  CONSTANT ROUGE
5  CONSTANT MAGENTA
6  CONSTANT BRUN
7  CONSTANT GRIS-CLAIR
8  CONSTANT GRIS-FONCE
9  CONSTANT BLEU-CLAIR
10 CONSTANT VERT-CLAIR
11 CONSTANT CYAN-CLAIR
12 CONSTANT ROUGE-CLAIR
13 CONSTANT MAGENTA-CLAIR
14 CONSTANT JAUNE
15 CONSTANT BLANC

\ Emission d'un caractŠre et d'une couleur.
CODE CEMIT  ( char col --- )
  BX POP
  BH BH XOR
  AX POP
  9 # AH MOV
  1 # CX MOV
  16 INT
  NEXT
END-CODE

\ Fait disparaitre le curseur.
: CURSOR-OFF  ( --- )
  -1 -1 (FRAME)
;

\ Fait apparaitre le curseur.
: CURSOR-ON  ( --- )
  7 8 (FRAME)
;

\ Le fichier du meilleur.
80 STRING0 FICTOP
" SNAKE.TOP" FICTOP $!

0 30 FIELD SNOM              \ Nom du meilleur.
30 2 FIELD SRECORD           \ Score du meilleur.

\ Cr‚ation d'un ‚cran virtuel de travail de 25 lignes et 80 colonnes.
\
\ AccŠs : X compris entre 0 et 79, Y compris entre 0 et 24, la position
\         0,0 correspondant au coin sup‚rieur gauche de l'‚cran.
\
\ Chaque "case" de l'‚cran contient un num‚ro de lutin.
\
80 CONSTANT DIMX
25 CONSTANT DIMY
DIMX DIMY CREER-CMATRICE ECRAN-VIRTUEL

\ Cr‚ation du serpent.
\ Chaque mot contient la position (X,Y) du corps.
2000 2 CREER-CMATRICE SERPENT

\ Les variables du jeu.
219 CONSTANT CMUR            \ CaractŠre des murs.
1   CONSTANT CBIDULE         \ CaractŠre des bidules.
79  CONSTANT CCORP           \ CaractŠre du corp du serpent.
2   CONSTANT CTETE           \ CaractŠre de la tˆte du serpent.
VARIABLE NMURS               \ Nombre de murs … faire apparaitre.
VARIABLE L                   \ Longueur du serpent.
VARIABLE A                   \ Facteur d'agrandissement du serpent.
VARIABLE T                   \ Num‚ro de la tˆte.
VARIABLE XT                  \ Position de la tˆte.
VARIABLE YT
VARIABLE DX                  \ D‚placements du serpent.
VARIABLE DY
VARIABLE XB                  \ Position du bidule.
VARIABLE YB
VARIABLE SENS                \ Sens courant du serpent.
VARIABLE SSENS               \ Sauvegarde de SENS.
VARIABLE ARGLL               \ VRAI si le serpent est mort !
VARIABLE SCORE               \ Score.
500 CONSTANT TEMPO           \ Temporisation.
VARIABLE NBIDULE             \ Nombre de bidules mang‚s.

\ Ouverture ou cr‚ation du fichier du meilleur.
: OPEN-TOP  ( --- )
  ?OPEN
  32 REC-SIZE !
  BUF-DIR REC-SIZE @ ERASE
  FICTOP DROP DUP 0 (SEARCH0)
  IF
    DUP READ-WRITE (OPEN)
    ?DOS-ERR
    HFILE !
    0 FREAD                  \ Lecture du meilleur.
    FCLOSE
  THEN
  0 (CREATE)
  ?DOS-ERR HFILE !
;

\ Enregistrement et fermeture du fichier du meilleur.
: CLOSE-TOP  ( --- )
  FAPPEND
  FCLOSE
;

\ Saisie du meilleur.
: TOP!  ( --- )
  DARK
  20 11 AT ." Votre Nom : " SNOM EXPECT
  SCORE @ SRECORD DROP !
;

\ Affichage du meilleur.
: .TOP  ( --- )
  SRECORD DROP @
  IF
    25 0 AT ." Recordman : " SNOM TYPE SPACE
    ." Score : " SRECORD DROP @ U.
  THEN
;

\ Remise … z‚ro de l'‚cran virtuel.
: RAZ-ECRAN-VIRTUEL  ( --- )
  DIMY 0
  DO
    DIMX 0
    DO
      0 I J ECRAN-VIRTUEL C!
    LOOP
  LOOP
  DIMX 0                     \ Mise en place des bords.
  DO
    1 I 1  ECRAN-VIRTUEL C!
    1 I 23 ECRAN-VIRTUEL C!
  LOOP
  DIMY 1- 1
  DO
    1 0  I ECRAN-VIRTUEL C!
    1 79 I ECRAN-VIRTUEL C!
  LOOP
;

\ Raz Serpent.
: RAZ-SERPENT  ( --- )
  2 0
  DO
    2000 0
    DO
      0 I J SERPENT C!
    LOOP
  LOOP
;

\ Affichage des bords de l'‚cran.
: .BORD  ( --- )
  DIMX 0
  DO
    I 1  AT CMUR CYAN CEMIT
    I 23 AT CMUR CYAN CEMIT
  LOOP
  DIMY 1- 1
  DO
    0  I AT CMUR CYAN CEMIT
    79 I AT CMUR CYAN CEMIT
  LOOP
;

\ Renvoi VRAI si le serpent rencontre un obstacle.
: COLLISION?  ( --- fl )
  XT @ YT @ ECRAN-VIRTUEL C@
  1 =
;

\ Renvoi VRAI si le serpent rencontre un bidule … manger.
: MIAM?  ( --- fl )
  XT @ YT @ ECRAN-VIRTUEL C@
  2 =
;

\ G‚nŠre une position dans l'‚cran.
: RND-POS  ( --- X Y )
  0 0
  BEGIN
    2DROP
    78 RND 1+
    22 RND 1+ 2DUP
    ECRAN-VIRTUEL C@ 0=
  UNTIL
;

\ Mise en place d'un bidule … l'‚cran.
: BIDULE!  ( --- )
  RND-POS 2DUP YB ! XB !
  (AT) CBIDULE VERT-CLAIR CEMIT
  ECRAN-VIRTUEL 2 SWAP C!
  NBIDULE 1+!
;

\ Mise en place d'un obstacle … l'‚cran.
: MUR!  ( --- )
  NBIDULE @ 0 >
  IF
    NBIDULE @ 10 /MOD DROP 0=
    IF                       \ Tout les 10 Bidules, nmurs obstacles apparaissent.
      NMURS @ 0
      DO
        RND-POS
        (AT) CMUR CYAN CEMIT
        ECRAN-VIRTUEL 1 SWAP C!
      LOOP
      TEMPO 50 - IS TEMPO    \ Ca va aussi un peu plus vite...
      NMURS 1+!              \ Un mur de plus au prochain passage.
    THEN
  THEN
;

\ Le serpent vient de manger un bidule, agrandissement de celui-ci.
: +SERPENT  ( --- )
  A @ L +!
  T @ A @ +
  L @
  DO
    2 0
    DO
      J A @ - I SERPENT C@
      J I SERPENT C!
    LOOP
    -1
  +LOOP
  T @ A @ +
  T @
  DO
    2 0
    DO
      0 J I SERPENT C!
    LOOP
  LOOP
  A @ T +!
  A 1+!
  BIDULE!
  MUR!
;

\ Effacement.
: EFFACE  ( ntete --- )
  DUP 0 SERPENT C@ 0=
  IF
    DROP
  ELSE
    DUP 0 SERPENT C@
    SWAP 1 SERPENT C@ (AT)
    BL NOIR CEMIT
    ECRAN-VIRTUEL 0 SWAP C!
  THEN
;

\ Argll !
: ARGL  ( --- )
  35 11 AT ." ARGLL !"
;

\ Incr‚mentation du score.
: +SCORE  ( --- )
  L @ A @ - SCORE !
;

\ Affichage du Score.
: .SCORE  ( --- )
  0 24 AT ." Score : "
  SCORE @ U.
;

\ Affichage du nombre de bidules mang‚s.
: .BIDULES  ( --- )
  65 24 AT ." Bidules : "
  NBIDULE @ 0 >=
  IF
    NBIDULE @ U.
  THEN
;

\ Affichage du serpent et gestion des collisions.
: .SERPENT  ( --- )
  T @ 0 SERPENT C@ DX @ + XT !
  T @ 1 SERPENT C@ DY @ + YT !
  COLLISION?
  IF
    TRUE ARGLL !
    ARGL
  THEN
  MIAM?
  IF
    +SERPENT
    +SCORE
  THEN
  L @ T @ + 1- L @ MOD
  DUP EFFACE
  T @ 0 SERPENT C@
  T @ 1 SERPENT C@ AT
  CCORP BRUN CEMIT
  T !
  XT @ T @ 0 SERPENT C!
  YT @ T @ 1 SERPENT C!
  1 XT @ YT @ ECRAN-VIRTUEL C!
  XT @ YT @ AT
  CTETE JAUNE CEMIT
;

\ Boucle g‚n‚rale de gestion des mouvements du serpent.
\    
: MVT  ( --- )
  0 SENS !
  0 SSENS !
  BEGIN
    KEY?
    IF
      KEY SSENS !
    THEN
    SSENS @
    CASE #FLG OF             \ Gauche
                -1 DX !
                 0 DY !
              ENDOF
         #FLD OF             \ Droite
                1 DX !
                0 DY !
              ENDOF
         #FLB OF             \ Bas
                0 DX !
                1 DY !
              ENDOF
         #FLH OF             \ Haut
                 0 DX !
                -1 DY !
              ENDOF
         SENS @ SSENS !
    ENDCASE
    SSENS @ SENS !
    SENS @
    IF
      .SERPENT
      .SCORE
      .BIDULES
      TEMPO MS
    THEN
    RANDOMIZE
    ARGLL @
  UNTIL
;

\ Initialisations.
: INIT-JEU  ( --- )
  RAZ-ECRAN-VIRTUEL
  RAZ-SERPENT
  -1 NBIDULE !
  FALSE ARGLL !
  4 A !
  1 L !
  0 T !
  0 SCORE !
  1 NMURS !
  500 IS TEMPO
  RND-POS YT ! XT !
  1 XT @ YT @ ECRAN-VIRTUEL C!
  XT @ T @ 0 SERPENT C!
  YT @ T @ 1 SERPENT C!
  XT @ YT @ AT
  CTETE JAUNE CEMIT
;

\ Pr‚sentation.
: .BANNIERE  ( --- )
  0 0 AT ." Le Serpent bidulovore !"
;

\ Fin du jeu.
: FIN-JEU  ( --- )
  XT @ YT @ AT
  CTETE JAUNE CEMIT
  31 24 AT ." Appuyez sur Escape"
  BEGIN
    KEY #ESC =
  UNTIL
;

\ Le programme principal.
: SNAKE  ( --- )
  3 MODE
  OPEN-TOP
  CURSOR-OFF
  INIT-JEU
  .BANNIERE
  .TOP
  .BORD
  .SCORE
  .BIDULES
  +SERPENT
  MVT
  FIN-JEU
  SCORE @ SRECORD DROP @ U>
  IF
    TOP!
  THEN
  DARK
  CURSOR-ON
  CLOSE-TOP
;

CR .( Tapez SNAKE pour lancer le programme. )
EOF

