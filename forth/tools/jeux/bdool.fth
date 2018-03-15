\ -----------------------------------------------------------------------------
\                                Boby et Dooly !
\
\ Par Guillaumaud Philippe
\
\ Contrainte :
\ ------------
\ Une carte graphique de type C.G.A minimum est n‚c‚ssaire pour ‚x‚cuter
\ ce programme.
\ -----------------------------------------------------------------------------

\ D‚finition de quelques outils n‚c‚ssaires au programme
EXIST? RND NOT ?\ INCLUDE RND
EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES
EXIST? (EKEY) NOT ?\ INCLUDE EKEY

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

\ Cr‚ation de lutins (sprites, en anglais) de dimensions 16x16 points en
\ C.G.A.
\ Au moment de la cr‚ation de lutins, ce mot doit trouver
\ sur la pile de donn‚es le nombre n‚c‚ssaire de mots
\ d‚finissants les lutins, ainsi que le nombre total de
\ ces lutins plus 1 (le premier ayant le num‚ro 0).
\
\ A l'‚x‚cution du mot cr‚‚, empiler le num‚ro du lutin auquel
\ on veut acc‚der.
\
: CREER-LUTIN-CGA16X16  ( [data.1/0..data.32/0]..[data.1/n..data.32/n] n+1 --- )
  CREATE
    DUP 1- ,       \ Stockage du nombre de lutins.
    0              \ Pour n+1 lutins
    DO
      20 0
      DO
        ,
      LOOP
    LOOP
  DOES>  ( n [PFA] --- adr_lutin )
    2DUP @         \ V‚rification de d‚passement de la zone.
    U> ABORT" CREER-LUTIN-CGA16X16: Indice hors limites"
    DUP @ ROT -    \ Calcul de l'adresse de d‚part de la zone contenant
    40 * 2+ +      \ la d‚finition du lutin choisi.
;

\ D‚finition des lutins du jeu.
\ Num‚ro 0 mur
05555 05555 01550 00155 0156A 0A156 01566 06156
0556A 0A556 04055 05505 04F55 055FD 04D55 0555C
04D55 0555C 05F55 055FC 05455 05500 01550 00155
0156A 0A156 01566 06156 0556A 0A556 05555 05555
\ Num‚ro 1 Boby
00F00 000F0 0FF00 000FF 0FF03 0C0FF 0FF0F 0F0FF
0EB3F 0FCFA 0823F 0BCE0 0EBFF 0FFFA 0FFFF 0FFFF
055D5 05755 0A5D5 0575A 08A35 05CA2 06035 05C09
05A0D 070A5 05503 0C055 0F500 0005F 00F00 000F0
\ Num‚ro 2 Dooly
00F00 000F0 0FF00 000FF 0FF03 0C0FF 0FF0F 0F0FF
0D73F 0FCF5 0413F 07CD0 0D7FF 0FFF5 0FFFF 0FFFF
0AAEA 0ABAA 05AEA 0ABA5 0453A 0AC51 0903A 0AC06
0A50E 0B05A 0AA03 0C0AA 0FA00 000AF 00F00 000F0
\ Num‚ro 3 personnage position 1
05001 04005 09001 04006 09001 04006 0A456 0951A
0A96A 0A96A 0AB56 095EA 0AE01 040BA 07B00 000ED
07B00 000ED 0AE01 040BA 0AB56 095EA 0A96A 0A96A
0A456 0951A 09001 04006 09001 04006 05001 04005
\ Num‚ro 4 personnage position 2
01500 00054 01900 00064 01900 00064 06A01 040A9
0A901 0406A 0AB56 095EA 0AE6A 0A9BA 07B56 095ED
07B56 095ED 0AE6A 0A9BA 0AB56 095EA 0A901 0406A
06A01 040A9 01900 00064 01900 00064 01500 00054
\ Num‚ro 5 point … manger
00000 00000 00000 00000 00000 00000 00000 00000
00000 00000 00000 00000 00200 00080 00900 000E0
00B00 00060 00200 00080 00000 00000 00000 00000
00000 00000 00000 00000 00000 00000 00000 00000
\ Num‚ro 6 espace
00000 00000 00000 00000 00000 00000 00000 00000
00000 00000 00000 00000 00000 00000 00000 00000
00000 00000 00000 00000 00000 00000 00000 00000
00000 00000 00000 00000 00000 00000 00000 00000

7 CREER-LUTIN-CGA16X16 LUTINS-CGA16X16

\ Affichage de lutins de dimensions 16x16 points … la position X,Y en C.G.A.
\
\ L'affichage se fait en ‚crivant directement dans la R.A.M ‚cran,
\ pour des raisons de rapidit‚.
\
: .LUTINS-CGA16X16 ( X Y Nø_lutin --- )
  LUTINS-CGA16X16 
  ROT 14 OVER      \ V‚rification de d‚passement horizontal.
  U> NOT ABORT" .LUTINS-CGA16X16: Indice X hors limites"
  4 *
  ROT 0C OVER      \ V‚rification de d‚passement vertical.
  U> NOT ABORT" .LUTINS-CGA16X16: Indice Y hors limites"
  280 * +          \ Calcul de l'offset dans l'‚cran.
  >R
  20 0
  DO
    DUP @ SWAP 2+  \ R‚cup‚ration des mots d‚finissants le lutin choisi.
  LOOP DROP
  R>
  8 0
  DO
    >R
    R@ ECRAN SWAP L!         \ Affichage des deux mots
    R@ 2+ ECRAN SWAP L!      \ de la ligne paire.
    R@ 2000 + ECRAN SWAP L!  \ Affichage des deux mots 
    R@ 2002 + ECRAN SWAP L!  \ de la ligne impaire.
    R> 50 +                  \ Nouvel offset d'‚cran (ligne suivante).
  LOOP
  DROP
;

DECIMAL

\ D‚finitions des variables du jeu.
2VARIABLE SCORE    \ Score du joueur
2VARIABLE VALPT    \ Valeur d'un point dans le labyrinthe courant
50 CONSTANT DELAI  \ Temporisation utilis‚e dans la gestion des monstres
20 CONSTANT DIMX
12 CONSTANT DIMY
VARIABLE VIE       \ Nombre de vies du joueur
VARIABLE BOX       \ Coordonn‚es de Boby
VARIABLE BOY 
VARIABLE DOX       \ Coordonn‚es de Dooly
VARIABLE DOY 
VARIABLE PERSO     \ Code du sprite du joueur
VARIABLE PERX      \ Coordonn‚es du joueur
VARIABLE PERY 
VARIABLE NPOINT    \ Nombre de points … manger dans le labyrinthe courant
VARIABLE NBLAB     \ Nombre de labyrinthes
VARIABLE NIVEAU    \ Niveau actuel du joueur

DIMX DIMY CREER-CMATRICE LAB

\ Initialisation du labyrinthe courant.
: !LAB  ( [data.1/0...data.20/0] ... [data.1/11..data.20/11] --- )
  DIMY 0
  DO
    DIMX 0
    DO
      I J LAB C!
    LOOP
  LOOP
;

\ Initialisatoin du premier labyrinthe.
: LABY1  ( --- )
  112 NPOINT !
  0 0 0 0 0 0 0 0 0 6 0 0 0 0 0 0 0 0 0 0
  0 5 5 5 5 5 5 5 5 5 0 5 5 5 5 5 5 5 5 0
  0 5 0 5 5 0 5 0 5 5 0 5 0 5 0 0 5 0 5 0
  0 5 5 5 5 0 5 0 5 5 0 5 0 5 0 0 5 0 5 0
  0 0 0 0 0 0 5 5 5 5 0 5 0 5 0 0 5 0 5 0
  0 5 5 5 5 5 5 0 0 5 0 5 5 5 5 5 5 5 5 0
  6 5 5 5 0 5 0 5 5 5 5 5 0 0 0 0 0 0 5 6
  0 0 0 5 0 5 0 5 5 5 5 5 5 5 5 5 5 5 5 0
  0 5 5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0
  0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 5 0
  0 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 0
  0 0 0 0 0 0 0 0 0 6 0 0 0 0 0 0 0 0 0 0
  !LAB
;

\ Initialisatoin du second labyrinthe.
: LABY2  ( --- )
  121 NPOINT !
  0 0 0 0 0 0 0 0 0 6 0 0 0 0 0 0 0 0 0 0
  0 5 5 5 5 5 5 5 5 5 0 5 5 5 5 5 5 5 5 0
  0 0 0 5 5 0 5 0 5 5 0 5 0 5 0 0 5 0 5 0
  0 5 5 5 5 0 5 0 5 5 0 5 0 5 5 5 5 0 5 0
  0 0 0 0 0 0 5 5 5 5 0 5 0 5 0 0 5 0 5 0
  0 5 5 5 5 5 5 0 0 5 0 5 5 5 5 5 5 5 5 0
  6 5 5 5 0 5 0 5 5 5 5 5 0 0 0 0 0 0 5 6
  0 0 0 5 0 5 0 5 5 5 5 5 5 5 5 5 5 5 5 0
  0 5 5 5 5 5 5 5 5 0 5 0 5 0 0 5 0 0 5 0
  0 0 0 0 0 0 5 0 0 0 5 0 5 0 0 5 0 0 5 0
  0 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 0
  0 0 0 0 0 0 0 0 0 6 0 0 0 0 0 0 0 0 0 0
  !LAB
;

CREATE LABY ' LABY1 , ' LABY2 ,

2 NBLAB !

\ S‚lection du labyrinthe correspondant au niveau courant.
: SELECT-LAB  ( n --- )
  DUP NBLAB @
  > ABORT" SELECT-LAB : Indice hors limite"
  2* LABY + @ EXECUTE
;

\ Affichage du labyrinthe courant.
: .LAB  ( --- )
  DIMY 0
  DO
    DIMX 0
    DO
      I J 2DUP LAB C@ .LUTINS-CGA16X16
    LOOP
  LOOP
;

\ Renvoi VRAI si on peut aller en bas.
: BAS?  ( X Y --- flag )
  DUP 11 <         \ V‚rification de d‚passement.
  IF
    1+ LAB C@ DUP 5 =
    SWAP 6 = OR
  ELSE
    2DROP
    FALSE
  THEN
;

\ Renvoi VRAI si on peut aller … droite.
: DROITE?  ( X Y --- flag )
  OVER 19 <        \ V‚rification de d‚passement.
  IF
    SWAP 1+ SWAP LAB C@ DUP
    5 = SWAP 6 = OR
  ELSE
    2DROP
    FALSE
  THEN
;

\ Renvoi VRAI si on peut aller en haut.
: HAUT?  ( X Y --- flag )
  DUP 0 >          \ V‚rification de d‚passement.
  IF
    1- LAB C@ DUP 5 = 
    SWAP 6 = OR
  ELSE
    2DROP
    FALSE
  THEN
;

\ Renvoi VRAI si on peut aller … gauche.
: GAUCHE?  ( X Y --- flag )
  OVER 0 >         \ V‚rification de d‚passement.
  IF
    SWAP 1- SWAP LAB C@ DUP
    5 = SWAP 6 = OR
  ELSE
    2DROP
    FALSE
  THEN
;

\ GŠre le mouvement du personnage du joueur.
: SWAP-PERSO  ( --- )
  PERSO DUP @ 3 =
  IF
    4 SWAP !
  ELSE
    3 SWAP !
  THEN
;

\ Renvoi VRAI si le joueur a rencontr‚ boby (gasp !...)
: BOBY?  ( --- flag )
  PERX @ BOX @ =
  PERY @ BOY @ = 
  AND
;

\ Renvoi VRAI si le joueur a rencontr‚ dooly (damned !...)
: DOOLY?  ( --- flag )
  PERX @ DOX @ =
  PERY @ DOY @ = 
  AND
;

\ GŠre la collision du joueur avec boby ou dooly
: COLLISION?  ( --- flag )
  BOBY? DOOLY? OR
;

\ Affichage du score.
: .SCORE  ( --- )
  1 24 AT ." Score : "
  SCORE 2@ UD.
;

\ Affichage du nombre de vies restant.
: .VIE  ( --- )
  30 24 AT ." Vies : "
  VIE @ .
;

\ EnlŠve une vie au joueur.
: -VIE  ( --- )
  VIE 1-!
;

\ Incr‚mente le score.
: +SCORE  ( --- )
  SCORE 2@ VALPT 2@ D+ SCORE 2!
;

\ Affichage du personnage du joueur.
: AFF-PERSO  ( --- )
  PERX @ PERY @ PERSO @ .LUTINS-CGA16X16
  SWAP-PERSO
;

\ Effacement du personnage du joueur.
: EFF-PERSO  ( --- )
  PERX @ PERY @ 2DUP LAB C@ .LUTINS-CGA16X16
;

\ Affichage de boby.
: AFF-BOBY  ( --- )
  BOX @ BOY @ 1 .LUTINS-CGA16X16
;

\ Effacement de boby.
: EFF-BOBY  ( --- )
  BOX @ BOY @ 2DUP LAB C@ .LUTINS-CGA16X16
;

\ Affichage de dooly.
: AFF-DOOLY  ( --- )
  DOX @ DOY @ 2 .LUTINS-CGA16X16
;

\ Effacement de dooly.
: EFF-DOOLY  ( --- )
  DOX @ DOY @ 2DUP LAB C@ .LUTINS-CGA16X16
;

\ Le joueur est-il sur un point … manger ? (miam miam...)
: POINT?  ( --- )
  PERX @ PERY @ 2DUP LAB C@
  5 =
  IF
    +SCORE
    6 -ROT LAB C!
    NPOINT 1-!
  ELSE
    2DROP
  THEN
;

\ Renvoi VRAI si on a atteint la porte du haut.
: PORTE-HAUT?  ( X Y --- flag )
  0= SWAP 10 = AND
;

\ Renvoi VRAI si on a atteint la porte du bas.
: PORTE-BAS?  ( X Y --- flag )
  11 = SWAP 10 = AND
;

\ Renvoi VRAI si on a atteint la porte de droite.
: PORTE-DROITE?  ( X Y --- flag )
  5 = SWAP 19 = AND
;

\ Renvoi VRAI si on a atteint la porte de gauche.
: PORTE-GAUCHE?  ( X Y --- flag )
  5 = SWAP 0= AND
;

\ Gestion des mouvements du personnage.
\    
: MVT-PERSO  ( c --- )
  CASE #FLG OF 
              PERX @ PERY @ GAUCHE? 
              IF 
                PERX 1-!
              ELSE
                PERX @ PERY @ PORTE-GAUCHE?
                IF
                  19 PERX !
                THEN
              THEN
            ENDOF
       #FLD OF 
              PERX @ PERY @ DROITE? 
              IF 
                PERX 1+! 
              ELSE 
                PERX @ PERY @ PORTE-DROITE?
                IF
                  0 PERX !
                THEN
              THEN
            ENDOF
       #FLB OF 
              PERX @ PERY @ BAS?
              IF 
                PERY 1+! 
              ELSE 
                PERX @ PERY @ PORTE-BAS?
                IF
                  0 PERY !
                THEN
              THEN
            ENDOF
       #FLH OF 
              PERX @ PERY @ HAUT?
              IF 
                PERY 1-! 
              ELSE
                PERX @ PERY @ PORTE-HAUT?
                IF
                  11 PERY !
                THEN
              THEN
            ENDOF
  ENDCASE
;

\ Gestion des mouvements de boby.
: MVT-BOBY  ( --- )
  4 RND
  CASE 0 OF 
           BOX @ BOY @ GAUCHE?
           IF 
             BOX 1-! 
           ELSE
             BOX @ BOY @ PORTE-GAUCHE?
             IF
               19 BOX !
             THEN
           THEN
         ENDOF
       1 OF 
           BOX @ BOY @ DROITE?
           IF 
             BOX 1+! 
           ELSE 
             BOX @ BOY @ PORTE-DROITE?
             IF
               0 BOX !
             THEN
           THEN
         ENDOF
       2 OF 
           BOX @ BOY @ BAS?
           IF 
             BOY 1+! 
           ELSE 
             BOX @ BOY @ PORTE-BAS?
             IF
               0 BOY !
             THEN
           THEN
         ENDOF
       3 OF 
           BOX @ BOY @ HAUT?
           IF 
             BOY 1-! 
           ELSE 
             BOX @ BOY @ PORTE-HAUT?
             IF
               11 BOY !
             THEN
           THEN
         ENDOF
  ENDCASE
;
  
\ Gestion des mouvements de boby.
: MVT-DOOLY  ( --- )
  4 RND
  CASE 0 OF 
           DOX @ DOY @ GAUCHE?
           IF 
             DOX 1-! 
           ELSE 
             DOX @ DOY @ PORTE-GAUCHE?
             IF
               19 DOX !
             THEN
           THEN
         ENDOF
       1 OF 
           DOX @ DOY @ DROITE? 
           IF 
             DOX 1+! 
           ELSE 
             DOX @ DOY @ PORTE-DROITE?
             IF
               0 DOX !
             THEN
           THEN
         ENDOF
       2 OF 
           DOX @ DOY @ BAS?
           IF 
             DOY 1+! 
           ELSE 
             DOX @ DOY @ PORTE-BAS?
             IF
               0 DOY !
             THEN
           THEN
         ENDOF
       3 OF 
           DOX @ DOY @ HAUT?
           IF 
             DOY 1-! 
           ELSE 
             DOX @ DOY @ PORTE-HAUT?
             IF
               11 DOY !
             THEN
           THEN
         ENDOF
  ENDCASE
;

\ Temporisation.
: TEMPO  ( --- )
  DELAI MS
;
  
\ Gestion des monstres !.
: MVT-BDOOL  ( --- )
  EFF-BOBY
  EFF-DOOLY
  MVT-BOBY
  MVT-DOOLY
  AFF-BOBY
  AFF-DOOLY
;

\ Initialisations du jeu.
: INIT-JEU  ( --- )
  0. SCORE 2!
  5. VALPT 2!
  5 VIE !
  1 BOX !
  1 BOY !
  18 DOX !
  10 DOY !
  3 PERSO !
  10 PERX !
  4 PERY !
  0 NIVEAU !
;

\ Fin du jeu :
\ en appuyant sur <Esc>, ou
\ en ayant mang‚ tous les points, ou
\ plus de vie, ou
\ dernier niveau.
: FIN-JEU?  ( c --- flag )
  #ESC =
  NPOINT @ 0= OR
  VIE @ 0= OR 
  NIVEAU @ NBLAB @ = OR
;

\ Programme principal.
: BDOOL  ( --- )
  5 MODE
  INIT-JEU
  NIVEAU @ SELECT-LAB
  .LAB
  .SCORE
  .VIE
  AFF-PERSO
  AFF-BOBY
  AFF-DOOLY
  BEGIN
    KEY?
    IF
      KEY DUP
      EFF-PERSO 
      MVT-PERSO
      POINT?
      COLLISION?
      IF
        \ Une "rencontre" avec Boby ou Dooly
        \ fait perdre une vie...
        -VIE
      THEN
      AFF-PERSO 
      .SCORE
      .VIE
      NPOINT @ 0=
      IF
        \ Si on a tout mang‚, on passe au niveau suivant
        NIVEAU 1+!
        NIVEAU @ NBLAB @ <
        IF
          NIVEAU @ SELECT-LAB
          .LAB
          AFF-PERSO
        THEN
      THEN
      FIN-JEU?
    ELSE
      FALSE
    THEN
    \ On s'occupe des monstres
    MVT-BDOOL
    TEMPO
  UNTIL
  3 MODE
;

CR .( Tapez BDOOL pour lancer le programme. )

EOF

