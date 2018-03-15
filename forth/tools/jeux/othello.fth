\ -----------------------------------------------------------------------------
\                                Othello (ou Reversi).
\
\ Par Guillaumaud Philippe
\
\ Contrainte :
\ ------------
\ Une carte graphique de type C.G.A minimum est n‚c‚ssaire pour ‚x‚cuter
\ ce programme.
\ -----------------------------------------------------------------------------

\ D‚finition de quelques outils n‚c‚ssaires au programme
EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES
EXIST? (EKEY) NOT ?\ INCLUDE EKEY

DECIMAL

\ D‚cr‚mente de n le contenu de adr.
CODE -!  ( n adr --- )
  BX POP
  AX POP
  AX 0 [BX] SUB
  NEXT
END-CODE

HEX
\ Le programme ‚tant con‡u pour fonctionner dans deux modes graphiques,
\ il nous faut les adresses des segment des ‚crans C.G.A et E.G.A
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
0B800 CONSTANT (ECRAN-CGA)
0A000 CONSTANT (ECRAN-EGA)

\
\ --------------------------- Gestion des Lutins ------------------------------
\

DEFER .LUTIN       \ Vecteur sur la routine d'affichage des lutins.
DEFER ECRAN        \ Vecteur contenant l'adr. de segment de l'‚cran courant.
                   \ Ces deux vecteurs sont initialis‚s selon le mode graphique
                   \ choisi.

VARIABLE V         \ Variables utilis‚es par la routine d'affichage
VARIABLE PEC       \ de lutins en E.G.A.

\ Cr‚ation de lutins (sprites, en anglais) de dimensions 16x16 points en 
\ E.G.A.
\ Au moment de la cr‚ation de lutins, ce mot doit trouver
\ sur la pile de donn‚es le nombre n‚c‚ssaire de mots 
\ d‚finissants les lutins, ainsi que le nombre total de
\ ces lutins plus 1 (le premier ayant le num‚ro 0).
\
\ A l'‚x‚cution du mot cr‚‚, empiler le num‚ro du lutin auquel
\ on veut acc‚der.
\
: CREER-LUTIN-EGA16X16  ( [data.1/0..data.64/0]..[data.1/n..data.64/n] n+1 --- )
  CREATE
    DUP 1- ,       \ Stockage du nombre de lutins.
    0              \ Pour n+1 lutins
    DO
      40 0
      DO
        ,
      LOOP
    LOOP
  DOES>  ( n [PFA] --- adr_lutin )
    2DUP @         \ V‚rification de d‚passement de la zone.
    U> ABORT" CREER-LUTIN-EGA16X16: Indice hors limites"
    DUP @ ROT -    \ Calcul de l'adresse de d‚part de la zone contenant
    80 * 2+ +      \ la d‚finition du lutin choisi.
;

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

\ D‚finition des lutins C.G.A du jeu.
\ Num‚ro 0 case vide
0FFFF 0FFFF 055D5 05755 055D5 05755 055D5 05755
055D5 05755 055D5 05755 055D5 05755 055D5 05755
055D5 05755 055D5 05755 055D5 05755 055D5 05755
055D5 05755 055D5 05755 055D5 05755 0FFFF 0FFFF

\ Num‚ro 1 Pion blanc
0FFFF 0FFFF 05FD5 057F5 0FFD5 057FF 0FFD7 0D7FF
0FFDF 0F7FF 057DF 0F7D5 077FF 0FFDD 057FF 0FFD5
0FFFF 0FFFF 0DFFF 0FFF7 0F7DF 0F7DF 0FDDF 0F77F
0FFD7 0D7FF 0FFD5 057FF 05FD5 057F5 0FFFF 0FFFF

\ Num‚ro 2 Pion noir
0FFFF 0FFFF 050D5 05705 000D5 05700 000D4 01700
000D0 00700 054D0 00715 044C0 00311 054C0 00315
000C0 00300 001C0 00340 004D0 00710 010D0 00704
000D4 01700 000D5 05700 050D5 05705 0FFFF 0FFFF

\ Num‚ro 3 Curseur 1 (sur case vide)
0FFFF 0FFFF 055D5 05755 055D5 05755 057D5 057D5
05ED5 057B5 05ED5 057B5 05ED5 057B5 0FED7 0D7BF
0AADE 0B7AA 0FED7 0D7BF 05ED5 057B5 05ED5 057B5
05ED5 057B5 057D5 057D5 055D5 05755 0FFFF 0FFFF

\ Num‚ro 4 Curseur 2 (sur pion blanc)
0FFFF 0FFFF 05FD5 057F5 0FFD5 057FF 0FCD7 0D73F
0F2DF 0F78F 052DF 0F785 072FF 0FF8D 00AFC 03FA0
0AAF2 08FAA 00AFC 03FA0 0F2DF 0F78F 0F2DF 0F78F
0F2D7 0D78F 0FCD5 0573F 05FD5 057F5 0FFFF 0FFFF

\ Num‚ro 5 Curseur 3 (sur pion noir)
0FFFF 0FFFF 050D5 05705 000D5 05700 003D4 017C0
00ED0 007B0 05ED0 007B5 04EC0 003B1 0FAC3 0C3AF
0AACE 0B3AA 0FAC3 0C3AF 00ED0 007B0 01ED0 007B4
00ED4 017B0 003D5 057C0 050D5 05705 0FFFF 0FFFF

6 CREER-LUTIN-CGA16X16 LUTINS-CGA16X16

\ D‚finition des lutins E.G.A du jeu.
\ Num‚ro 0 case vide
0FFFF 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 0FFFF
00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000 00000
0FFFF 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 0FFFF
0FFFF 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 0FFFF

\ Num‚ro 1 Pion blanc
0FFFF 0518D 0B996 0A5AA 07FDF 04DF3 07B9F 0A5AD 089D2 00B90 013F8 02FA0 09595 05989 07182 0FFFF
0C003 0F00F 0F01F 0DC3F 08061 08261 084E1 0FAF6 07EFF 0D4FB 02C6C 0D07F 0683E 0A016 0800D 00000
0FFFF 0F18F 0F19F 0DDBF 0F9FF 0CBF3 0FCFF 0FAF6 04EF3 004F0 00DE8 011FC 069BE 0A196 0818D 03FFC
0FFFF 0F18F 0F99F 0FDBF 087E1 0A7E5 087E1 0FFFF 0FFFF 0DFFB 03FFC 0FFFF 0FDBF 0F99F 0F18F 0FFFF

\ Num‚ro 2 Pion noir
03FFC 0A182 04989 07995 0FFBE 0CF92 0FF7E 05F5B 0C721 00F60 01F90 03FDC 0FDAB 0F99F 0F18F 0FFFF
00000 00000 00000 00000 00000 00000 00000 00000 0300C 0D00B 02004 0C003 00000 00000 00000 00000
03FFC 00180 00980 02180 07F9E 06D96 07B1E 00509 0B10C 0DB0B 03394 0EF83 09581 05989 07182 0FFFF
03FFC 00FF0 007E0 003C0 0799E 0599A 0781E 00000 00000 02004 0C183 00180 003C0 007E0 00FF0 03FFC

\ Num‚ro 3 Curseur 1 (sur case vide)
0FFFF 00180 08181 08181 08181 08181 04183 03DBE 03DBC 04182 08181 08181 08181 08181 00180 0FFFF
00000 08001 04002 04002 04002 04002 03C3C 00240 00240 03C3C 04002 04002 04002 04002 08001 00000
0FFFF 08181 0C182 0C182 0C182 0C182 07DBC 03FC0 003FC 03DBE 04183 04183 04183 04183 08181 0FFFF
0FFFF 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 00180 0FFFF

\ Num‚ro 4 Curseur 2 (sur pion blanc)
0FFFF 0518C 0B995 0A5A9 0BFDD 08DF1 04382 03DBC 03DBC 04382 093F9 0AFA1 09595 09989 07182 0FFFF
0C003 0F00F 0701E 05C3E 04062 04262 03CFC 002C0 002C0 03CFC 06C6E 0507E 0683E 06016 0800D 00000
0FFFF 0F18F 0F19E 0DDBE 0F9FE 0CBF2 07CFC 03EC0 002FC 03CFE 04DEB 051FF 069BF 06197 0818D 03FFC
0FFFF 0718E 0399C 03DBC 007E0 027E4 003C0 00180 00180 003C0 03FFC 03FFC 03DBC 0399C 0718E 0FFFF

\ Num‚ro 5 Curseur 3 (sur pion noir)
03FFC 02182 08989 0B995 0BFBD 08F91 04342 03D3E 03D3C 04342 09F91 0BFDD 0BDA9 0B99D 0718E 0FFFF
00000 08001 04002 04002 04002 04002 03C3C 00240 00240 03C3C 06006 04002 04002 04002 08001 00000
03FFC 08181 0C982 0E182 0FF9E 0ED96 07F3C 03F40 0037C 03F3E 07397 06F83 05583 0598B 0F183 0FFFF
03FFC 00FF0 007E0 003C0 0399C 01998 00000 00000 00000 00000 00180 00180 003C0 007E0 00FF0 03FFC

6 CREER-LUTIN-EGA16X16 LUTINS-EGA16X16

\ Affichage de lutins de dimensions 16x16 points … la position X,Y en C.G.A.
\
\ L'affichage se fait en ‚crivant directement dans la R.A.M ‚cran,
\ pour des raisons de rapidit‚.
\
: (.LUTINS-CGA16X16)  ( X Y Nø_lutin --- )
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
  LOOP
  DROP
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

\ Affichage de lutins de dimensions 16x16 points … la position X,Y en E.G.A.
\
\ L'affichage se fait en ‚crivant directement dans la R.A.M ‚cran,
\ pour des raisons de rapidit‚.
\
: (.LUTINS-EGA16X16)  ( X Y Nø_lutin --- )
  LUTINS-EGA16X16
  ROT 14 OVER      \ V‚rification de d‚passement horizontal.
  U> NOT ABORT" .LUTINS-EGA16X16: Indice X hors limites"
  2 *
  ROT 0C OVER      \ V‚rification de d‚passement vertical.
  U> NOT ABORT" .LUTINS-EGA16X16: Indice Y hors limites"
  280 * +          \ Calcul de l'offset dans l'‚cran.
  PEC !
  40 0
  DO
    DUP @ SWAP 2+  \ R‚cup‚ration des mots d‚finissants le lutin choisi.
  LOOP
  DROP
  08 V !
  08  03CE PC!          \ S‚lection du mode de travail
  0FF 03CF PC!          \ (‚criture).
  4 0
  DO
    PEC @
    02  03C4 PC!
    V @ 03C5 PC!        \ S‚lection du plan de travail.
    10 0
    DO
      >R
      R@ ECRAN SWAP L!
      R> 28 +           \ Ligne suivante.
    LOOP
    DROP
    V @ 2/ V !          \ Plan suivant.
  LOOP
;

DECIMAL

\ ------------------------ D‚finition des variables du jeu --------------------
0 CONSTANT CASEVIDE
1 CONSTANT PIONBLANC
2 CONSTANT PIONNOIR
3 CONSTANT CURSEUR1
4 CONSTANT CURSEUR2
5 CONSTANT CURSEUR3
1 CONSTANT MARGEHAUTE        \ Marge haute et droite utilis‚e
6 CONSTANT MARGEDROITE       \ pour centrer le damier.
8 8 CREER-CMATRICE DAMIER    \ Le damier de jeu
VARIABLE CURCOL              \ Position X et Y du curseur
VARIABLE CURLIG
VARIABLE SAVEDAMIER          \ Lutin pr‚sent … la position du curseur
VARIABLE SCOREBLANC          \ Les scores des deux joueurs.
VARIABLE SCORENOIR
VARIABLE COMPTE              \ Nombre de pions adverses entre deux pions du joueur actif
0 , 0 , 0 , 0 , 0 , 0 , 0 ,  \ dans les huits directions
                             \ dans l'ordre : Haut Bas Droite Gauche Haut-Gauche Haut-Droite Bas-Gauche Bas-droite
VARIABLE SAVECOMPTE          \ Utilis‚ dans la simulation
0 , 0 , 0 , 0 , 0 , 0 , 0 ,
VARIABLE NPIONS              \ Variable interm‚diaire pour le comptage des pions
-1 CONSTANT DIRCOL           \ Utilis‚es pour les balayages
-1 CONSTANT DIRLIG

\ Temporisation
: TEMPO  ( --- )
  1000 0
  DO
    100 0
    DO
    LOOP
  LOOP
;

\ Attend l'appui sur une touche
: ATTEND-TOUCHE  ( --- )
  BEGIN
    KEY
  UNTIL
;

\ R‚initialisation du damier
: RAZ-DAMIER  ( --- )
  8 0
  DO
    8 0
    DO
      CASEVIDE I J DAMIER C!
    LOOP
  LOOP
  PIONBLANC 3 3 DAMIER C!
  PIONBLANC 4 4 DAMIER C!
  PIONNOIR  3 4 DAMIER C!
  PIONNOIR  4 3 DAMIER C!
;

\ Affichage du damier
: .DAMIER  ( --- )
  8 0
  DO
    8 0
    DO
      I J 2DUP DAMIER C@ >R       \ col lig ---
      SWAP MARGEDROITE + SWAP     \ col+md lig ---
      MARGEHAUTE + R> .LUTIN      \ col+md lig+mh nø_lutin ---
    LOOP
  LOOP
;

\ Remise … z‚ro des comptes
: RAZ-COMPTE  ( --- )
  COMPTE 16 ERASE
  SAVECOMPTE 16 ERASE
;

\ Remise … z‚ro des scores
: RAZ-SCORE  ( --- )
  2 DUP SCOREBLANC ! SCORENOIR !
;

\
\ --------------------------- Gestion du curseur ------------------------------
\

\ Effacement du curseur
: EFFACE-CURSEUR  ( --- )
  CURCOL @ MARGEDROITE +
  CURLIG @ MARGEHAUTE +
  SAVEDAMIER @ .LUTIN   \ On r‚affiche le lutin pr‚c‚dent.
;

\ Affichage du curseur en fonction de l'‚l‚ment pr‚sent … sa position.
: .CURSEUR  ( --- )
  CURCOL @ CURLIG @ DAMIER C@ SAVEDAMIER !
  CURCOL @ MARGEDROITE + CURLIG @ MARGEHAUTE +
  SAVEDAMIER @
  CASE CASEVIDE  OF
                   CURSEUR1
                 ENDOF
       PIONBLANC OF
                   CURSEUR2
                 ENDOF
       PIONNOIR  OF
                   CURSEUR3
                 ENDOF
  ENDCASE
  .LUTIN
;

\ D‚placement du curseur vers la droite
: CURSEUR-DROITE  ( --- )
  CURCOL @ 1+ DUP
  7 > ABS - CURCOL !
;

\ D‚placement du curseur vers la gauche
: CURSEUR-GAUCHE  ( --- )
  CURCOL @ 1- DUP
  0 < ABS + CURCOL !
;

\ D‚placement du curseur vers le haut
: CURSEUR-HAUT  ( --- )
  CURLIG @ 1- DUP
  0 < ABS + CURLIG !
;

\ D‚placement du curseur vers le bas
: CURSEUR-BAS  ( --- )
  CURLIG @ 1+ DUP
  7 > ABS - CURLIG !
;

\ Gestion des mouvements du curseur
\ Touches :    
: MVT-CURSEUR  ( c --- )
  EFFACE-CURSEUR
  CASE #FLG OF     \ FlŠche gauche.
              CURSEUR-GAUCHE
            ENDOF
       #FLD OF     \ FlŠche droite.
              CURSEUR-DROITE
            ENDOF
       #FLB OF     \ FlŠche basse.
              CURSEUR-BAS
            ENDOF
       #FLH OF     \ FlŠche haute.
              CURSEUR-HAUT
            ENDOF
  ENDCASE
  .CURSEUR
;

\
\ --------------------------- Gestion des joueurs -----------------------------
\

\ Renvoi le contenu du damier si c'est possible, sinon case vide
: DAMIER@  ( c l --- coul_pion )
  DUP 0 7 BETWEEN -ROT  \ V‚rifications de d‚passement.
  OVER 0 7 BETWEEN
  >R ROT R> AND
  IF
    DAMIER C@
  ELSE
    2DROP CASEVIDE
  THEN
;

\ Renvoi vrai si la case courante est vide … la position col,lig.
: CASE-VIDE?  ( col lig --- col lig fl )
  2DUP DAMIER C@ CASEVIDE =
;

\ Renvoi le contenu de la case situ‚e en haut de la position c,l.
: HAUT@  ( c l --- c l pion )
  2DUP 1- DAMIER@
;

\ Renvoi le contenu de la case situ‚e en bas de la position c,l.
: BAS@  ( c l --- c l pion )
  2DUP 1+ DAMIER@
;

\ Renvoi le contenu de la case situ‚e … droite de la position c,l.
: DROITE@  ( c l --- c l pion )
  2DUP SWAP 1+ SWAP DAMIER@
;

\ Renvoi le contenu de la case situ‚e … gauche de la position c,l.
: GAUCHE@  ( c l --- c l pion )
  2DUP SWAP 1- SWAP DAMIER@
;

\ Renvoi le contenu de la case situ‚e en haut … gauche de la position c,l.
: DIAGHAGA@  ( c l --- c l pion )
  2DUP 1- SWAP 1- SWAP DAMIER@
;

\ Renvoi le contenu de la case situ‚e en haut … droite de la position c,l.
: DIAGHADR@  ( c l --- c l pion )
  2DUP 1- SWAP 1+ SWAP DAMIER@
;

\ Renvoi le contenu de la case situ‚e en bas … gauche de la position c,l.
: DIAGBAGA@  ( c l --- c l pion )
  2DUP 1+ SWAP 1- SWAP DAMIER@
;

\ Renvoi le contenu de la case situ‚e en bas … droite de la position c,l.
: DIAGBADR@  ( c l --- c l pion )
  2DUP 1+ SWAP 1+ SWAP DAMIER@
;

\ Renvoi vrai si la case de position c,l est entour‚e par
\ au moins un pion de couleur ‚gale … celle donn‚e
: ENTOUREE?  ( c l couleur_pion --- c l fl )
  >R
  HAUT@     R@ = -ROT                  \ fl c l ---
  BAS@      R@ = -ROT                  \ fl fl c l ---
  DROITE@   R@ = -ROT                  \ fl fl fl c l ---
  GAUCHE@   R@ = -ROT                  \ fl fl fl fl c l ---
  DIAGHAGA@ R@ = -ROT                  \ fl fl fl fl fl c l ---
  DIAGHADR@ R@ = -ROT                  \ fl fl fl fl fl fl c l ---
  DIAGBAGA@ R@ = -ROT                  \ fl fl fl fl fl fl fl c l ---
  DIAGBADR@ R> = -ROT                  \ fl fl fl fl fl fl fl fl c l ---
  >R >R                                \ fl fl fl fl fl fl fl fl ---
  OR OR OR OR OR OR OR R> R> ROT       \ c l fl ---
;

\ Primitive de recherche dans le damier.
\ Balaie le damier jusqu'… trouver un pion de la couleur du joueur ou une
\ case vide.
\ Compte le nombre de pions adverses rencontr‚s.
\
\ c,l           = position de d‚part.
\ coul_pion_adv = couleur du pion adverse.
\ coul_pion_jou = couleur du pion du joueur.
\
: (BALAIE?)  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  0 NPIONS !
  2OVER SWAP DIRCOL + SWAP DIRLIG +
  CASEVIDE -ROT                        \ c l adv jou pion c+dirc l+dirl ---
  BEGIN
    2DUP DAMIER@                       \ c l adv jou pion c l pion ---
    DUP 5 PICK <>                      \ c l adv jou pion c l pion fl ---
    OVER CASEVIDE 2DUP =               \ c l adv jou pion c l pion fl pion pion fl ---
    IF
      0 NPIONS !                       \ On arrete si on trouve une case vide.
    THEN
    <> AND                             \ On continue tant que les pions sont diff‚rents.
  WHILE
    NPIONS 1+!
    3 ROLL DROP -ROT                   \ c l adv jou pion c l ---
    SWAP DIRCOL + SWAP DIRLIG +
  REPEAT                               \ c l adv jou pion c l pion ---
  >R 2DROP DROP R>                     \ c l adv jou pion ---
  ROT DROP =                           \ c l fl ---
;

\ Recherche vers le haut.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-HAUT?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
   0 IS DIRCOL
  -1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE !
;

\ Recherche vers le bas.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-BAS?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  0 IS DIRCOL
  1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 2+ !
;

\ Recherche vers la droite.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-DROITE?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  1 IS DIRCOL
  0 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 4 + !
;

\ Recherche vers la gauche.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-GAUCHE?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  -1 IS DIRCOL
   0 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 6 + !
;

\ Recherche vers le haut … gauche.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-HAGA?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  -1 IS DIRCOL
  -1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 8 + !
;

\ Recherche vers le haut … droite.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-HADR?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
   1 IS DIRCOL
  -1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 10 + !
;

\ Recherche vers le bas … gauche.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-BAGA?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  -1 IS DIRCOL
   1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 12 + !
;

\ Recherche vers le bas … droite.
\ Renvoi VRAI si un pion du joueur actif est trouv‚ derriŠre
\ le ou les pions de l'adversaire.
\ Compte le nombre de pions adverses rencontr‚s.
: CHERCHE-BADR?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  1 IS DIRCOL
  1 IS DIRLIG
  (BALAIE?)
  NPIONS @ COMPTE 14 + !
;

\ Renvoi VRAI si le coup est jouable
\ coul_pion_adv est la couleur du pion de l'adversaire.
\ coul_pion_jou est la couleur du pion du joueur actif.
\ c l est la position du coup.
: COUP-JOUABLE?  ( c l coul_pion_adv coul_pion_jou --- c l fl )
  >R >R HAUT@ R@ =
  IF                                   \ Le pion adverse est-il en haut ?
    R> R> 2DUP >R >R CHERCHE-HAUT?     \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl c l ---
  BAS@ R@ =
  IF                                   \ Le pion adverse est-il en bas ?
    R> R> 2DUP >R >R CHERCHE-BAS?      \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl c l ---
  DROITE@ R@ =
  IF                                   \ Le pion adverse est-il … droite ?
    R> R> 2DUP >R >R CHERCHE-DROITE?   \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl fl c l ---
  GAUCHE@ R@ =
  IF                                   \ Le pion adverse est-il … gauche ?
    R> R> 2DUP >R >R CHERCHE-GAUCHE?   \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl fl fl c l ---
  DIAGHAGA@ R@ =
  IF                                   \ Le pion adverse est-il en haut … gauche ?
    R> R> 2DUP >R >R CHERCHE-HAGA?     \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl fl fl fl c l ---
  DIAGHADR@ R@ =
  IF                                   \ Le pion adverse est-il en haut … droite ?
    R> R> 2DUP >R >R CHERCHE-HADR?     \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl fl fl fl fl c l ---
  DIAGBAGA@ R@ =
  IF                                   \ Le pion adverse est-il en bas … gauche ?
    R> R> 2DUP >R >R CHERCHE-BAGA?     \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT                                 \ fl fl fl fl fl fl fl c l ---
  DIAGBADR@ R@ =
  IF                                   \ Le pion adverse est-il en bas … droite ?
    R> R> 2DUP >R >R CHERCHE-BADR?     \ On cherche un pion du joueur actif de l'autre cot‚ du ou des pions adverses
  ELSE
    FALSE
  THEN
  -ROT R> R> 2DROP >R >R               \ fl fl fl fl fl fl fl fl ---
  OR OR OR OR OR OR OR R> R> ROT       \ c l fl ---
;

\ Primitive de balayage du damier.
\ Balaie le damier et place un pion de la couleur du joueur … chaque
\ position, jusqu'… trouver un pion de couleur diff‚rente de celle du
\ joueur ou une case vide.
\
: (BALAIE!)  ( c l coul_pion_jou n --- c l )
  NPIONS !
  2 PICK 2 PICK
  SWAP DIRCOL + SWAP DIRLIG +          \ c l jou c+dirc l+dirl ---
  BEGIN
    2DUP DAMIER@                       \ c l jou c l pion ---
    DUP 4 PICK <>                      \ c l jou c l pion fl ---
    SWAP CASEVIDE <> AND
    NPIONS @ 0> AND                    \ c l jou c l fl ---
  WHILE
    2DUP 4 PICK -ROT DAMIER C!
    SWAP DIRCOL + SWAP DIRLIG +
    NPIONS 1-!
  REPEAT                               \ c l jou c l ---
  2DROP DROP                           \ c l ---
;

\ Retourne les pions situ‚s en haut, de couleur diff‚rente de celle du joueur.
: RETOURNE-HAUT  ( c l coul_pion_jou --- c l )
   0 IS DIRCOL
  -1 IS DIRLIG
  COMPTE @ (BALAIE!)
;

\ Retourne les pions situ‚s en bas, de couleur diff‚rente de celle du joueur.
: RETOURNE-BAS  ( c l coul_pion_jou --- c l )
  0 IS DIRCOL
  1 IS DIRLIG
  COMPTE 2+ @ (BALAIE!)
;

\ Retourne les pions situ‚s … droite, de couleur diff‚rente de celle du joueur.
: RETOURNE-DROITE  ( c l coul_pion_jou --- c l )
  1 IS DIRCOL
  0 IS DIRLIG
  COMPTE 4 + @ (BALAIE!)
;

\ Retourne les pions situ‚s … gauche, de couleur diff‚rente de celle du joueur.
: RETOURNE-GAUCHE  ( c l coul_pion_jou --- c l )
  -1 IS DIRCOL
   0 IS DIRLIG
  COMPTE 6 + @ (BALAIE!)
;

\ Retourne les pions situ‚s en haut … gauche, de couleur diff‚rente de celle du joueur.
: RETOURNE-HAGA  ( c l coul_pion_jou --- c l )
  -1 IS DIRCOL
  -1 IS DIRLIG
  COMPTE 8 + @ (BALAIE!)
;

\ Retourne les pions situ‚s en haut … droite, de couleur diff‚rente de celle du joueur.
: RETOURNE-HADR  ( c l coul_pion_jou --- c l )
   1 IS DIRCOL
  -1 IS DIRLIG
  COMPTE 10 + @ (BALAIE!)
;

\ Retourne les pions situ‚s en bas … gauche,
\ de couleur diff‚rente de celle du joueur.
: RETOURNE-BAGA  ( c l coul_pion_jou --- c l )
  -1 IS DIRCOL
   1 IS DIRLIG
  COMPTE 12 + @ (BALAIE!)
;

\ Retourne les pions situ‚s en bas … droite,
\ de couleur diff‚rente de celle du joueur.
: RETOURNE-BADR  ( c l coul_pion_jou --- c l )
  1 IS DIRCOL
  1 IS DIRLIG
  COMPTE 14 + @ (BALAIE!)
;

\ Retourne les pions de l'adversaire et met … jour les scores.
\ coul_pion_jou est la couleur du pion du joueur actif.
\ score_jou est l'adresse de la variable contenant le score du joueur actif.
\ score_adv est l'adresse de la variable contenant le score de l'adversaire.
\ c l est la position du coup jou‚.
: RETOURNE  ( c l coul_pion_jou score_jou score_adv --- )
  >R >R >R                             \ c l ---
  COMPTE @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en haut ?
    R@ RETOURNE-HAUT
    R> R> COMPTE @ OVER +!
    R> COMPTE @ OVER -!
    >R >R >R
    0 COMPTE !
  THEN                                 \ c l ---
  COMPTE 2+ @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en bas ?
    R@ RETOURNE-BAS
    R> R> COMPTE 2+ @ OVER +!
    R> COMPTE 2+ @ OVER -!
    >R >R >R
    0 COMPTE 2+ !
  THEN                                 \ c l ---
  COMPTE 4 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses … droite ?
    R@ RETOURNE-DROITE
    R> R> COMPTE 4 + @ OVER +!
    R> COMPTE 4 + @ OVER -!
    >R >R >R
    0 COMPTE 4 + !
  THEN                                 \ c l ---
  COMPTE 6 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses … gauche ?
    R@ RETOURNE-GAUCHE
    R> R> COMPTE 6 + @ OVER +!
    R> COMPTE 6 + @ OVER -!
    >R >R >R
    0 COMPTE 6 + !
  THEN                                 \ c l ---
  COMPTE 8 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en haut … gauche ?
    R@ RETOURNE-HAGA
    R> R> COMPTE 8 + @ OVER +!
    R> COMPTE 8 + @ OVER -!
    >R >R >R
    0 COMPTE 8 + !
  THEN                                 \ c l ---
  COMPTE 10 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en haut … droite ?
    R@ RETOURNE-HADR
    R> R> COMPTE 10 + @ OVER +!
    R> COMPTE 10 + @ OVER -!
    >R >R >R
    0 COMPTE 10 + !
  THEN                                 \ c l ---
  COMPTE 12 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en bas … gauche ?
    R@ RETOURNE-BAGA
    R> R> COMPTE 12 + @ OVER +!
    R> COMPTE 12 + @ OVER -!
    >R >R >R
    0 COMPTE 12 + !
  THEN                                 \ c l ---
  COMPTE 14 + @ 0>
  IF                                   \ A-t'on trouv‚ des pions adverses en bas … droite ?
    R@ RETOURNE-BADR
    R> R> COMPTE 14 + @ OVER +!
    R> COMPTE 14 + @ OVER -!
    >R >R >R
    0 COMPTE 14 + !
  THEN                                 \ c l ---
  2DROP R> R> R> 2DROP DROP
;

\ Affichage d'un message d'erreur.
: .ERREUR  ( --- )
  10 24 AT ." COUP NON AUTORISE !"
  TEMPO
  10 24 AT 20 BL REPLICATE
;

\ Affichage d'un message quand l'adversaire passe.
: .PASSE  ( --- )
  0 24 AT ." Je Passe.... ! (Appuyez sur une touche)"
  ATTEND-TOUCHE
  0 24 AT 39 BL REPLICATE
;

\ Affichage de la banniŠre.
: .BANNIERE  ( --- )
  17 0 AT ." OTHELLO"
  4 2 AT ." Vous"
  3 5 AT ." Blancs"
  33 2 AT ." Moi"
  32 5 AT ." Noirs"
;

\ Affichage du menu.
: .MENU  ( --- )
  3 19 AT ."     : Deplacements du curseur"
  3 20 AT ." Esc     : Fin du jeu"
  3 21 AT ." Return  : Validation du coup"
  3 22 AT ." Espace  : Passer"
;

\ Affichage des scores.
: .SCORES  ( --- )
  5 7 AT SCOREBLANC @ U.
  34 7 AT SCORENOIR @ U.
;

\ Place un pion noir … la position c,l.
: PLACE-PION  ( c l --- )
  2DUP PIONNOIR
  -ROT DAMIER C!
  PIONNOIR SCORENOIR
  SCOREBLANC RETOURNE
  SCORENOIR 1+!
;

\ Les noirs essaient de jouer le coup c,l.
\ Renvoi VRAI si c'est possible.
: (ESSAIE-N?)  ( c l --- c l fl )
  RAZ-COMPTE
  CASE-VIDE?
  IF
    PIONBLANC ENTOUREE?
    IF
      PIONBLANC PIONNOIR COUP-JOUABLE?
      IF
        TRUE EXIT
      THEN
    THEN
  THEN
  FALSE
;

\ Les blancs essaient de jouer le coup c,l.
\ Renvoi VRAI si c'est possible.
: (ESSAIE-B?)  ( c l --- c l fl )
  CASE-VIDE?
  IF
    PIONNOIR ENTOUREE?
    IF
      PIONNOIR PIONBLANC COUP-JOUABLE?
      IF
        TRUE EXIT
      THEN
    THEN
  THEN
  FALSE
;

\ Essayage des coins.
\ Renvoi VRAI et la position du coin jouable.
: ESSAIE-COINS?  ( --- c l fl)
  0 0 (ESSAIE-N?)  \ Coin sup‚rieur gauche.
  IF
    TRUE EXIT
  THEN
  2DROP
  0 7 (ESSAIE-N?)  \ Coin inf‚rieur gauche.
  IF
    TRUE EXIT
  THEN
  2DROP
  7 0 (ESSAIE-N?)  \ Coin sup‚rieur droit.
  IF
    TRUE EXIT
  THEN
  2DROP
  7 7 (ESSAIE-N?)  \ Coin inf‚rieur droit.
;

\ Primitive de balayage de ligne.
\ Le joueur noir balaie la ligne de deb … fin.
: (LIG)  ( li c l fl fin deb --- c l fl )
  DO
    2DROP DROP
    I OVER (ESSAIE-N?)
    DUP ?LEAVE
  LOOP                                 \ li c l fl ---
  3 ROLL DROP
;

\ Primitive de balayage de colonne.
\ Le joueur noir balaie la colonne de deb … fin.
: (COL)  ( co c l fl fin deb --- c l fl )
  DO
    2DROP DROP
    DUP I (ESSAIE-N?)
    DUP ?LEAVE
  LOOP                                 \ co c l fl ---
  3 ROLL DROP
;

\ Simule le jeu du pion noir.
: (>SIMUL)  ( c l --- c l )
  COMPTE SAVECOMPTE 16 CMOVE           \ On sauvegarde les comptes
  2DUP PLACE-PION
;

\ Remet le jeu en l'‚tat.
: (SIMUL>)  ( c l c l --- c l )
  SAVECOMPTE COMPTE 16 CMOVE
  2DROP 2DUP
  CASEVIDE -ROT DAMIER C!
  2DUP PIONBLANC SCOREBLANC            \ On replace les pions blancs
  SCORENOIR RETOURNE
  SAVECOMPTE COMPTE 16 CMOVE
  SCORENOIR 1-!
  SAVECOMPTE @ 0>                      \ Replace le pion noir … sa position initiale.
  IF
    2DUP 1- SAVECOMPTE @ -             \ c l c l-1-n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 2+ @ 0>
  IF
    2DUP 1+ SAVECOMPTE 2+ @ +          \ c l c l+1+n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 4 + @ 0>
  IF
    2DUP SWAP 1+
    SAVECOMPTE 4 + @ + SWAP            \ c l c+1+n l ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 6 + @ 0>
  IF
    2DUP SWAP 1-
    SAVECOMPTE 6 + @ - SWAP            \ c l c-1-n l ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 8 + @ 0>
  IF
    2DUP 1- SWAP 1- SWAP
    SAVECOMPTE 8 + @ DUP ROT SWAP -
    SWAP ROT SWAP - SWAP               \ c l c-1-n l-1-n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 10 + @ 0>
  IF
    2DUP 1- SWAP 1+ SWAP
    SAVECOMPTE 10 + @ DUP ROT SWAP -
    SWAP ROT + SWAP                    \ c l c+1+n l-1-n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 12 + @ 0>
  IF
    2DUP 1+ SWAP 1- SWAP
    SAVECOMPTE 12 + @ DUP ROT +
    SWAP ROT SWAP - SWAP               \ c l c-1-n l+1+n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 14 + @ 0>
  IF
    2DUP 1+ SWAP 1+ SWAP
    SAVECOMPTE 14 + @ DUP ROT +        \ c l c+1 n l+1+n ---
    SWAP ROT + SWAP                    \ c l c+1+n l+1+n ---
    PIONNOIR -ROT DAMIER C!
  THEN
  SAVECOMPTE 16 ERASE
;

\ V‚rifie que le joueur noir ne donne pas un coin du haut … l'adversaire.
\ Renvoi VRAI si oui.
: DONNE-COINHA?  ( c l --- c l fl )
  (>SIMUL)                             \ On simule le jeu du pion noir
  0 0 (ESSAIE-B?)                      \ On essaie les coins avec le pion blanc
  IF
    (SIMUL>)                           \ On remet la case … vide
    TRUE EXIT
  THEN
  2DROP 7 0 (ESSAIE-B?)
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  (SIMUL>) FALSE
;

\ V‚rifie que le joueur noir ne donne pas un coin du bas … l'adversaire.
\ Renvoi VRAI si oui.
: DONNE-COINBA?  ( c l --- c l fl )
  (>SIMUL)                             \ On simule le jeu du pion noir
  0 7 (ESSAIE-B?)                      \ On essaie les coins avec le pion blanc
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  2DROP 7 7 (ESSAIE-B?)
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  (SIMUL>) FALSE
;

\ V‚rifie que le joueur noir ne donne pas un coin de droite … l'adversaire.
\ Renvoi VRAI si oui.
: DONNE-COINDR?  ( c l --- c l fl )
  (>SIMUL)                             \ On simule le jeu du pion noir
  7 0 (ESSAIE-B?)                      \ On essaie les coins avec le pion blanc
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  2DROP 7 7 (ESSAIE-B?)
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  (SIMUL>) FALSE
;

\ V‚rifie que le joueur noir ne donne pas un coin de gauche … l'adversaire.
\ Renvoi VRAI si oui.
: DONNE-COINGA?  ( c l --- c l fl )
  (>SIMUL)                             \ On simule le jeu du pion noir
  0 0 (ESSAIE-B?)                      \ On essaie les coins avec le pion blanc
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  2DROP 0 7 (ESSAIE-B?)
  IF
    (SIMUL>)
    TRUE EXIT
  THEN
  (SIMUL>) FALSE
;

\ Le joueur noir essaie les bords.
\ Renvoi VRAI et la position du coup.
: ESSAIE-BORDS?  ( --- c l fl )
  0 0 0 FALSE 7 1 (LIG)                \ c l fl ---
  IF
    DONNE-COINHA? NOT                  \ donne-t'on un coin … l'adversaire ?
    IF
      TRUE EXIT                        \ Non, on peut jouer ce coup
    THEN
  THEN
  7 -ROT FALSE 7 1 (LIG)               \ c l fl ---
  IF
    DONNE-COINBA? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  0 -ROT FALSE 7 1 (COL)               \ c l fl ---
  IF
    DONNE-COINGA? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  7 -ROT FALSE 7 1 (COL)               \ c l fl ---
  IF
    DONNE-COINDR? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  FALSE
;

\ Le joueur noir essaie la premiŠre ligne int‚rieure.
\ Renvoi VRAI et la position du coup.
: ESSAIE-INT1?  ( --- c l fl )
  1 0 0 FALSE 7 1 (LIG)                \ c l fl ---
  IF
    DONNE-COINHA? NOT                  \ donne-t'on un coin … l'adversaire ?
    IF
      TRUE EXIT                        \ Non, on peut jouer ce coup
    THEN
  THEN
  6 -ROT FALSE 7 1 (LIG)               \ c l fl ---
  IF
    DONNE-COINBA? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  1 -ROT FALSE 7 1 (COL)               \ c l fl ---
  IF
    DONNE-COINGA? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  6 -ROT FALSE 7 1 (COL)               \ c l fl ---
  IF
    DONNE-COINDR? NOT
    IF
      TRUE EXIT
    THEN
  THEN
  FALSE
;

\ Le joueur noie essaie la deuxiŠme ligne int‚rieure.
\ Renvoi VRAI et la position du coup.
: ESSAIE-INT2?  ( --- c l fl )
  2 0 0 FALSE 6 2 (LIG)                \ c l fl ---
  IF
    TRUE EXIT
  THEN
  5 -ROT FALSE 6 2 (LIG)               \ c l fl ---
  IF
    TRUE EXIT
  THEN
  2 -ROT FALSE 6 2 (COL)               \ c l fl ---
  IF
    TRUE EXIT
  THEN
  5 -ROT FALSE 6 2 (COL)               \ c l fl ---
  IF
    TRUE EXIT
  THEN
  FALSE
;

\ C'est au programme de jouer...
\
\ Strat‚gie utilis‚e :
\ - Dans un premier temps, il essaie de prendre un coin
\   (position forte dans ce jeu).
\ - En cas d'‚chec, il essaie de prendre un bord.
\ - Si un bord est prenable, il prend ce bord si cela ne donne pas un coin
\   … l'adversaire.
\ - Si aucun bord n'est prenable, il essaie une position interne, si cela ne
\   donne pas un coin … l'adversaire.
\ - Si aucune position interne n'est prenable, il passe !.
\
: JE-JOUE
  14 24 AT ." Je Joue ...."
  ESSAIE-COINS?
  IF
    PLACE-PION
  ELSE
    2DROP ESSAIE-BORDS?
    IF
      PLACE-PION
    ELSE
      2DROP ESSAIE-INT1?
      IF
        PLACE-PION
      ELSE
        2DROP ESSAIE-INT2?
        IF
          PLACE-PION
        ELSE
          2DROP .PASSE
        THEN
      THEN
    THEN
  THEN
  14 24 AT 20 BL REPLICATE
  RAZ-COMPTE
  .DAMIER
  .SCORES
  .CURSEUR
;

\ Renvoi VRAI si c'est la fin du jeu.
\
\ Quand un des joueurs a son score … 0, ou que le damier est plein.
: FIN?  ( --- fl )
  SCOREBLANC @ 0=
  SCORENOIR  @ 0= OR
  SCOREBLANC @
  SCORENOIR @ + 64 = OR
;

\ Gestion des coups des joueurs.
: JOUEURS  ( c --- )
  #RET =
  IF
    CURCOL @ CURLIG @ CASE-VIDE?
    IF                                 \ La case est-elle vide ?
      PIONNOIR ENTOUREE?
      IF                               \ La case est-elle entour‚e par au moins un pion noir ?
        PIONNOIR PIONBLANC COUP-JOUABLE?
        IF                             \ Le coup est-il jouable ?
          2DUP PIONBLANC
          -ROT DAMIER C!               \ Oui, on le joue !
          PIONBLANC SCOREBLANC
          SCORENOIR RETOURNE           \ On retourne les pions et on met le score … jour
          SCOREBLANC 1+!               \ Plus le pion que l'on vient de placer !
          .DAMIER
          .SCORES
          .CURSEUR
          FIN? NOT
          IF
            JE-JOUE                    \ Au programme de jouer.
          THEN
        ELSE
          2DROP .ERREUR
        THEN
      ELSE
        2DROP .ERREUR
      THEN
    ELSE
      2DROP .ERREUR
    THEN
  ELSE
    DUP #SPC =                         \ Le joueur passe...
    IF
      FIN? NOT
      IF
        JE-JOUE
      THEN
    THEN
  THEN
  RAZ-COMPTE
;

\ Fin du jeu.
\ Affichage du message de fin.
: FIN-JEU
  0 24 AT
  SCOREBLANC @ SCORENOIR @ =
  IF
    ." Egalite"
  ELSE
    ." J'ai "
    SCOREBLANC @ SCORENOIR @ >
    IF
      ." perdu"
    ELSE
      ." gagne"
    THEN
  THEN
  ."  ... (appuyez sur une touche)"
  ATTEND-TOUCHE
  0 24 AT 39 BL REPLICATE
;

\ Le programme principal
: (JEU)
  RAZ-DAMIER
  RAZ-SCORE
  RAZ-COMPTE
  0 DUP CURCOL ! CURLIG !
  .BANNIERE
  .MENU
  BEGIN
    .DAMIER
    .SCORES
    .CURSEUR
    KEY
    DUP MVT-CURSEUR
    DUP JOUEURS
    #ESC = FIN? OR
  UNTIL
  FIN-JEU
;

: OTHELLO-CGA
  ['] (ECRAN-CGA) IS ECRAN
  ['] (.LUTINS-CGA16X16) IS .LUTIN
  5 MODE
  (JEU)
  3 MODE
;

: OTHELLO-EGA
  ['] (ECRAN-EGA) IS ECRAN
  ['] (.LUTINS-EGA16X16) IS .LUTIN
  13 MODE
  (JEU)
  3 MODE
;

CR .( Tapez OTHELLO-CGA pour lancer le programme avec un ‚cran CGA)
CR .( Tapez OTHELLO-EGA pour lancer le programme avec un ‚cran EGA)

EOF

