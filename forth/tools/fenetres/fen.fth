\ -----------------------------------------------------------------------------
\                        Gestionnaire de fen�tres dynamiques.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF

\ Chargement des outils.
EXIST? MALLOC NOT ?\ INCLUDE MEMORY

ONLY FORTH ALSO DEFINITIONS

VOCABULARY FEN
FEN DEFINITIONS

HEX

\ -----------------------------------------------------------------------------
\                       Variables et constantes globales
\ -----------------------------------------------------------------------------
B800 CONSTANT SCREEN         \ Mettre B000 pour un �cran monochrome
                             \ Mettre A000 pour un �cran EGA.
0032 CONSTANT #WMAXI         \ Nombre maxi de fen�tres ouvrables

0600 CONSTANT SCROLLUP       \ Utilis�es dans la fonction de scrolling
0700 CONSTANT SCROLLDWN
0007 CONSTANT SHADOWCOL      \ Couleur utilis�e pour l'ombrage (Blanc sur noir)

DECIMAL

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

78 CONSTANT MAXLARG          \ Largeur maxi d'une fen�tre
                             \ (80 car. - 2 de bord)
23 CONSTANT MAXHAUT          \ Hauteur maxi d'une fen�tre
                             \ (25 lig. - 2 de bord)

\ -----------------------------------------------------------------------------
\                         Manipulation des param�tres fen�tres
\ -----------------------------------------------------------------------------
VARIABLE FNUM                \ Num�ro de la fen�tre courante

100 CONSTANT #WPARAM         \ Taille (en octets) de la zone param�tre d'une fen�tre

CREATE WPARAM                \ Zone param�tres de la fen�tre courante.
  #WPARAM ALLOT

\ Mise � z�ro de la zone param�tres.
: RAZ-WPARAM  ( --- )
  WPARAM #WPARAM ERASE
;

\ Acc�s aux param�tres de la fen�tre courante.
\ Adresse de segment de la zone de sauvegarde.
: WSEGMENT  ( --- adr )
  WPARAM
;

\ Position de la fen�tre en x.
: POSX  ( --- adr )
  WPARAM 2+
;

\ Position de la fen�tre en y.
: POSY  ( --- adr )
  WPARAM 4 +
;

\ Largeur de la fen�tre.
: LARG  ( --- adr )
  WPARAM 6 +
;

\ Hauteur de la fen�tre.
: HAUT  ( --- adr )
  WPARAM 8 +
;

\ Forme du bord.
: BORD  ( --- adr )
  WPARAM 10 +
;

\ Couleur de la fen�tre (fond et caract�res).
: FCOL  ( --- adr )
  WPARAM 12 +
;

\ Position du curseur en x.
: CURX  ( --- adr )
  WPARAM 14 +
;

\ Position du curseur en y.
: CURY  ( --- adr )
  WPARAM 16 +
;

\ Num�ro de la fen�tre.
: NUME  ( --- adr )
  WPARAM 18 +
;

\ Couleur du bord (fond et caract�res).
: BCOL  ( --- adr )
  WPARAM 20 +
;

\ Titre de la fen�tre (segment, offset et longueur. de la chaine).
: $FTITRE  ( --- adr )
  WPARAM 22 +
;

\ Couleur du titre (fond et caract�res).
: TCOL  ( --- adr )
  WPARAM 28 +
;

\ Ombrage (contient VRAI si oui).
: SHADOW  ( --- adr )
  WPARAM 30 +
;

\ Cr�ation de la table des param�tres des fen�tres.
: WVECTEUR  ( <nom> --- )
  CREATE
    #WMAXI #WPARAM * MALLOC
    ,
  DOES>        ( n�fen adr --- seg adr )
    OVER #WMAXI U< NOT ABORT" WVECTEUR: Num�ro de fen�tre hors limites"
    @ SWAP #WPARAM *
;

WVECTEUR WTABLE

\ Lecture des param�tres de la fen�tre n� n�fen.
: WTABLE>  ( n�fen --- )
  WTABLE DSEGMENT WPARAM
  #WPARAM LCMOVE
;

\ Ecriture des param�tres de la fen�tre n� n�fen.
: >WTABLE  ( n�fen --- )
  DSEGMENT WPARAM ROT WTABLE
  #WPARAM LCMOVE
;

\ Lecture d'une variable triple.
: 3@  ( adr --- n1 n2 n3 )
  DUP 2+ 2@ ROT @
;

\ Initialisation d'une variable triple.
: 3!  ( n1 n2 n3 adr --- )
  SWAP OVER ! 2+ 2!
;

\ Le double NIP.
: 2NIP  ( n1 n2 n3 --- n3 )
  NIP NIP
;

\ -----------------------------------------------------------------------------
\                       D�finition et s�lection d'un bord
\ -----------------------------------------------------------------------------
\ Cr�ation et s�lection d'un type de bord.
CREATE BD0 ,"       "
CREATE BD1 ," ڿ��ĳ"
CREATE BD2 ," ɻȼͺ"
CREATE BD3 ," ַӽĺ"
CREATE BD4 ," ոԾͳ"

: WSELECT  ( <nom> --- )
  DEPTH 1- OVER < ABORT" WSELECT : Pas assez d'�l�ments"
  CREATE
    0 DO , LOOP
  DOES>
    SWAP 2* + @
;

BD4 1+ BD3 1+ BD2 1+ BD1 1+ BD0 1+ 5 WSELECT CADRE

\ -----------------------------------------------------------------------------
\                              Fonctions g�n�rales
\ -----------------------------------------------------------------------------
\ Empile la largeur totale de la fen�tre courante, en caract�res.
: (WLARG)  ( --- lc )
  LARG @ 2+ SHADOW @
  IF
    2+
  THEN
;

\ Empile la hauteur totale de la fen�tre courante, en caract�res.
: (WHAUT)  ( --- hc)
  HAUT @ 2+ SHADOW @
  IF
    1+
  THEN
;

\ Calcule l'espace �cran occup� par une fen�tre.
\ Empile le r�sultat, en caract�res.
: (WSIZE)  ( --- n )
  (WLARG)
  (WHAUT)
  * 2*
;

\ Converti la position de la fen�tre courante en offset d'�cran.
CODE (XY->OFF)  ( X Y --- off)
  AX POP
  80 # BL MOV
  BL MUL
  AX SHL                     \ AX = Y*160
  BX POP
  BX SHL                     \ BX = X*2
  BX AX ADD                  \ AX = (Y*160)+(X*2)
  1PUSH
END-CODE

: WORG  ( --- off )
  POSX @ POSY @ (XY->OFF)
;

\ Converti les coordonn�es pr�sentes en adresse (seg:off) dans l'�cran.
: WSCR  ( X Y --- seg adr )
  (XY->OFF) WORG +
  SCREEN SWAP
;

\ Calcul de l'adresse (seg:off) de la ligne n de la fen�tre courante
\ dans WSEGMENT.
: WMEM  ( n --- seg adr )
  (WLARG)
  2* *
  WSEGMENT @ SWAP
;

\ Primitive de scrolling vertical.
CODE (WSCROLL)  ( sens attr lcid lcsg --- )
  CX POP                     \ Ligne, Colonne du coin sup�rieur gauche
  DX POP                     \ Ligne, Colonne du coin inf�rieur droit
  BX POP                     \ Attribut des caract�res � afficher
  BL BH MOV
  AX POP                     \ Sens (N� de la fonction)
  1 # AL MOV                 \ Une ligne.
  16 INT
  NEXT
END-CODE

\ Scrolling d'une ligne de la fen�tre courante.
: WSCROLL  ( sens --- )
  FCOL @
  POSY @ HAUT @ + FLIP
  POSX @ LARG @ + +
  POSY @ 1+ FLIP
  POSX @ 1+ +
  (WSCROLL)
;

\ -----------------------------------------------------------------------------
\                          Manipulation du curseur
\ -----------------------------------------------------------------------------
\ Primitive de positionnement du curseur.
CODE (WLOCATE)  ( pos --- )
  DX POP
  BH BH XOR
  2 # AH MOV
  16 INT
  NEXT
END-CODE

\ Positionne le curseur dans la fen�tre courante.
: WLOCATE  ( --- )
  POSY @ CURY @ + FLIP
  POSX @ CURX @ + +
  (WLOCATE)
;

\ Positionne le curseur.
: (WAT)  ( X Y --- X Y )
  2DUP
  HAUT @ MIN CURY !
  LARG @ MIN CURX !
  WLOCATE
;

\ Emet un retour arri�re dans la fen�tre courante.
: WBS  ( --- )
  CURX @ 1 >
  IF
    CURX 1-!
    WLOCATE
  THEN
;

\ Emet un retour chariot dans la fen�tre courante.
: (WCR)  ( --- )
  FNUM @
  IF
    CURY @ HAUT @ =
    IF
      SCROLLUP WSCROLL
    ELSE
      CURY 1+!
    THEN
    1 CURX !
    WLOCATE
  ELSE
    CRLF
  THEN
;

\ Active la gestion normale du retour chariot.
: CR-ON  ( --- )
  ['] (WCR) IS CR
;

\ D�sactive la gestion du retour chariot.
: CR-OFF  ( --- )
  ['] NOOP IS CR
;

\ Fait disparaitre le curseur.
: CURSOR-OFF  ( --- )
  -1 -1 (FRAME)
;

\ Fait apparaitre le curseur.
: CURSOR-ON  ( --- )
  7 8 (FRAME)
;

\ -----------------------------------------------------------------------------
\                            Affichage des caract�res
\ -----------------------------------------------------------------------------
\ Emission d'un caract�re dans la fen�tre courante.
\ On �crit directement dans la R.A.M vid�o.
\
\ Si l'on d�sire une �criture conventionnelle, c'est � dire avec les fonctions
\ classiques du B.I.O.S, utiliser les d�finitions suivantes :
\
\ Primitive d'�mission d'un caract�re.
\ CODE ((WEMIT))  ( char coul --- )
\   BX POP
\   BH BH XOR
\   AX POP
\   9 # AH MOV
\   1 # CX MOV
\   16 INT
\   NEXT
\ END-CODE
\
\ : (WEMIT)  ( char --- )
\   FCOL @ ((WEMIT))
\ ;
: (WEMIT)  ( char --- )
  FCOL @ FLIP +
  CURX @ CURY @ WSCR L!
;

: WEMIT  ( char --- )
  FNUM @
  IF
    DUP 8 =
    IF
      DROP WBS
    ELSE
      DUP 7 =
      IF
        (EMIT)
      ELSE
        (WEMIT)
        CURX @ LARG @ =
        IF
          CR
        ELSE
          CURX 1+!
          WLOCATE
        THEN
      THEN
    THEN
  ELSE
    (EMIT)
  THEN
;

\ -----------------------------------------------------------------------------
\               Gestion de l'espace fen�tres et de la m�moire �cran
\ -----------------------------------------------------------------------------
\ Primitive d'allocation d'espace fen�tre.
: (WALLOC)  ( --- )
  (WSIZE) MALLOC WSEGMENT !
;

\ Alloue une fen�tre.
: WALLOC  ( --- )
  (WALLOC)
  FNUM @ 1+
  DUP NUME !
  DUP FNUM !
  >WTABLE
;

\ Primitive de lib�ration de l'espace occup� par la fen�tre courante.
: (WLIBRE)  ( --- )
  WSEGMENT @ RELEASE
;

\ Lib�re l'espace occup� par la fen�tre courante.
\ La fen�tre pr�c�dente devient la fen�tre courante.
: WLIBRE  ( --- )
  FNUM @ DUP 0= ABORT" WLIBRE: Pas de fen�tre ouverte"
  DUP WTABLE> 1- FNUM !
  (WLIBRE)
;

\ D�placements de mots (16 bits) inter-segments.
CODE LMOVE  ( seg_src off_src seg_dst off_dst long --- )
  CLD
  IP BX MOV
  CX POP
  DI POP
  ES POP
  DS DX MOV
  IP POP
  DS POP
  REP MOVS
  BX IP MOV
  DX DS MOV
  NEXT
END-CODE

\ Sauvegarde de l'�cran sous la fen�tre courante.
: WBACK  ( --- )
  (WHAUT) 0
  DO
    0 I WSCR
    I WMEM
    (WLARG) LMOVE
  LOOP
;

\ Restauration de l'�cran sous la fen�tre courante.
: WREST  ( --- )
  (WHAUT) 0
  DO
    I WMEM
    0 I WSCR
    (WLARG) LMOVE
  LOOP
;

\ -----------------------------------------------------------------------------
\                            Affichage de la fen�tre
\ -----------------------------------------------------------------------------
\ Remplissage d'une zone avec un mot (16 bits)
CODE LWFILL  ( seg off long n --- )
  CLD
  AX POP
  CX POP
  DI POP
  ES POP
  REP AX STOS
  NEXT
END-CODE

\ Dessin de la fen�tre courante.
\ Pour acc�l�rer le processus, on �crit directement
\ dans la R.A.M �cran.
: WBORD  ( --- )
  CURX @ CURY @              \ Dessin du cadre.
  BORD @ CADRE
  DUP C@ BCOL @ FLIP +
  0 0 WSCR L!
  DUP 4 + C@
  1 0 WSCR LARG @ 3 ROLL
  BCOL @ FLIP + LWFILL
  DUP 1+ C@ BCOL @ FLIP +
  LARG @ 1+ 0 WSCR L!
  DUP 5 + C@ BCOL @ FLIP +
  HAUT @ 1+ 1
  DO
    DUP 0 I WSCR L!
    DUP LARG @ 1+ I WSCR L!
  LOOP
  DROP
  DUP 2+ C@ BCOL @ FLIP +
  0 HAUT @ 1+ WSCR L!
  DUP 4 + C@
  1 HAUT @ 1+ WSCR LARG @ 3 ROLL
  BCOL @ FLIP + LWFILL
  3 + C@ BCOL @ FLIP +
  LARG @ 1+ HAUT @ 1+ WSCR L!
  SHADOW @                   \ Dessin de l'ombre �ventuelle.
  IF
    LARG @ 4 + 2
    DO
      SHADOWCOL I HAUT @ 2+ WSCR 1+ LC!
    LOOP
    HAUT @ 3 + 1
    DO
      SHADOWCOL LARG @ 2+ I WSCR
      3DUP 1+ LC! 3 + LC!
    LOOP
  THEN
  CURY ! CURX ! WLOCATE
;

\ Affichage du titre �ventuel de la fen�tre.
: WTITRE  ( --- )
  $FTITRE 3@
  2NIP
  IF
    FCOL @
    CURX @ CURY @
    TCOL @ FCOL !
    $FTITRE 3@
    LARG @ 2+ MIN            \ Longueur limit�e � la largeur de la fen�tre.
    LARG @ 2+ OVER - 
    2/ 0 CURY ! CURX !       \ Affichage centr� sur le bord sup�rieur.
    WLOCATE LTYPE
    CURY ! CURX !
    FCOL ! WLOCATE
  THEN
;

\ Primitive d'effacement.
\ c'est en fait un scrolling de toute la fen�tre vers le haut.
CODE ((WDARK))  ( attr lcid lcsg --- )
  CX POP
  DX POP
  BX POP
  BL BH MOV
  6 # AH MOV
  0 # AL MOV
  16 INT
  NEXT
END-CODE

\ Efface la fen�tre courante.
: WDARK  ( --- )
  FNUM @
  IF
    FCOL @
    POSY @ HAUT @ + FLIP
    POSX @ LARG @ + +
    POSY @ 1+ FLIP
    POSX @ 1+ +
    ((WDARK))
    1 CURX ! 1 CURY ! WLOCATE
  ELSE
    DARK
  THEN
;

\ Primitive d'affichage de la fen�tre courante.
: (W.FEN)  ( --- )
  WBACK
  WBORD
  WTITRE
;

\ -----------------------------------------------------------------------------
\                             Gestion des couleurs
\ -----------------------------------------------------------------------------
\ Fabrique un octet de couleur (fond+caract�re) � partir des deux couleurs
\ (fond, caract�re) donn�es.
: F/C  ( coul_fond coul_caract�re --- octet_coul )
  SWAP 16 * +
;

\ Remplace la couleur de la ligne X,Y par ncoul, sur la longueur long.
: WXCHGCOL  ( ncoul X Y long --- )
  LARG @ MIN 0
  DO
    3DUP WSCR 1+ LC!
    SWAP 1+ SWAP
  LOOP
  2DROP DROP
;

\ Remplace la couleur des n lignes, � partir de la position X,Y, sur la
\ longueur long, par ncoul.
: WXCHGCOLS  ( ncoul X Y long n --- )
  HAUT @ MIN 0
  DO
    4DUP WXCHGCOL
    SWAP 1+ SWAP
  LOOP
  2DROP 2DROP
;

\ -----------------------------------------------------------------------------
\                           D�placements d'une fen�tre
\ -----------------------------------------------------------------------------
\ Primitive de sauvegarde de la zone client de la fen�tre courante.
\ seg:off est l'adresse du buffer de sauvegarde.
: (WBCKCLI)  ( seg off --- seg off )
  POSX @ POSY @
  POSX 1+! POSY 1+!
  3 PICK 3 PICK
  HAUT @ 0
  DO
    2DUP 0 I WSCR 2SWAP
    LARG @ 2* I * +
    LARG @ LMOVE
  LOOP
  2DROP POSY ! POSX !
;

\ Primitive de restauration de la zone client de la fen�tre courante.
\ seg:off est l'adresse du buffer de sauvegarde.
: (WRSTCLI)  ( seg off --- seg off )
  POSX @ POSY @
  POSX 1+! POSY 1+!
  3 PICK 3 PICK
  HAUT @ 0
  DO
    2DUP LARG @ 2* I * +
    0 I WSCR
    LARG @ LMOVE
  LOOP
  2DROP POSY ! POSX !
;

\ Sauvegarde la zone client de la fen�tre courante.
\ Empile l'adresse (seg off) du buffer de sauvegarde.
: WCLIENTBCK  ( --- seg off )
  LARG @ HAUT @ * 2*
  MALLOC 0                   \ seg off ---
  (WBCKCLI)
;

\ Restauration de la zone client de la fen�tre courante.
\ seg:off est l'adresse du buffer de sauvegarde.
: WCLIENTRST  ( seg off --- )
  (WRSTCLI)
  DROP RELEASE
;

\ Primitive de d�placement de la fen�tre courante.
\ D�place la fen�tre courante de nblig vers le haut/la droite (positif),
\ ou vers le bas/la gauche (n�gatif).
\ La direction d�pend de l'adresse de la variable pass�e (POSX ou POSY).
: (WMOVE)  ( nblig adr_var --- )
  WCLIENTBCK              \ nblig adr_var seg off ---
  WREST
  2SWAP +!
  (W.FEN)
  WCLIENTRST
;

\ D�placement de la fen�tre courante d'une ligne vers le haut.
: WMOVUP  ( --- )
  POSY @ 1- 0 >=
  IF
    -1 POSY (WMOVE)
  THEN
;

\ D�placement de la fen�tre courante d'une ligne vers le bas.
: WMOVDN  ( --- )
  POSY @ (WHAUT) +
  24 <=
  IF
    1 POSY (WMOVE)
  THEN
;

\ D�placement de la fen�tre courante d'une colonne vers la gauche.
: WMOVLFT  ( --- )
  POSX @ 1- 0 >=
  IF
    -1 POSX (WMOVE)
  THEN
;

\ D�placement de la fen�tre courante d'une colonne vers la droite.
: WMOVRGT  ( --- )
  POSX @ (WLARG) +
  79 <=
  IF
    1 POSX (WMOVE)
  THEN
;

\ -----------------------------------------------------------------------------
\                        Redimensionnements d'une fen�tre
\ -----------------------------------------------------------------------------
\ R�affichage de la zone client de la fen�tre courante.
\ atail est l'ancienne taille de la zone client.
\ seg:off est l'adresse du buffer de sauvegarde.
: (W.CLIENT)  ( atail seg off --- )
  DARK
  HAUT @ 0
  DO
    3DUP ROT
    1 I 1+ WSCR ROT
    LARG @ MIN LMOVE
    ROT LARG @ - -ROT
    2 PICK 0< ?LEAVE
    LARG @ 2* +
  LOOP
  DROP RELEASE DROP
;

\ Primitive de redimensionnement.
\ Enl�ve (n�gatif) ou ajoute (positif) nc caract�res � la fen�tre courante.
: (WRESIZE)  ( nc adr_var --- )
  HAUT @ LARG @ *            \ nc adr_var atail ---
  WCLIENTBCK                 \ nc adr_var atail seg off ---
  WREST
  (WLIBRE)
  4 ROLL 4 ROLL +!
  (WALLOC)
  (W.FEN)
  (W.CLIENT)
;

\ Enl�ve une ligne � la fen�tre courante.
: WHAUT-  ( --- )
  HAUT @ 1 >
  IF
    -1 HAUT (WRESIZE)
  THEN
;

\ Ajoute une ligne � la fen�tre courante.
: WHAUT+  ( --- )
  (WHAUT) POSY @ + 25 <
  IF
    1 HAUT (WRESIZE)
  THEN
;

\ Enl�ve une colonne � la fen�tre courante.
: WLARG-  ( --- )
  LARG @ 1 >
  IF
    -1 LARG (WRESIZE)
  THEN
;

\ Ajoute une colonne � la fen�tre courante.
: WLARG+  ( --- )
  (WLARG) POSX @ + 80 <
  IF
    1 LARG (WRESIZE)
  THEN
;

\ -----------------------------------------------------------------------------
\                        Fonctions principales de fen�tres
\ -----------------------------------------------------------------------------
\ D�salloue le segment des param�tres fen�tres.
\ R�tabli les vecteurs initiaux.
: WRELEASE  ( --- )
  ['] WTABLE >BODY @ RELEASE
  ['] (AT) IS AT
  ['] CRLF IS CR
  ['] (EMIT) IS EMIT
  ['] (MODE) IS DARK
;
 
\ Ferme la fen�tre courante.
\ Si c'est la derni�re, se comporte comme WALLCLOSE.
: WFERME  ( --- )
  FNUM @ 0= ABORT" WFERME: Pas de fen�tre ouverte"
  WREST
  WLIBRE
  FNUM @ WTABLE>
  WLOCATE
  FNUM @ 0=
  IF
    WRELEASE
  THEN
;

\ Ferme toutes les fen�tres ouvertes.
\ D�salloue le segment des param�tres fen�tres.
\ R�tabli les vecteurs initiaux.
: WALLCLOSE  ( --- )
  1 FNUM @
  DO
    WFERME
    -1
  +LOOP
  WRELEASE
;

\ Initialisation correcte des vecteurs redirig�s,
\ et du segment de la table des param�tres.
: WINIT  ( --- )
  RAZ-WPARAM
  LIB-COM
  #WMAXI #WPARAM *
  MALLOC IS WTABLE
  ['] (WAT) IS AT
  CR-ON
  ['] WEMIT IS EMIT
  ['] WDARK IS DARK
;

\ Primitive d'ouverture d'une fen�tre.
: (WOUVRE)  ( pos_x pos_y larg haut forme_bord coul_fen coul_bord $<titre> coul_titre shadow --- )
  SHADOW ! TCOL ! $FTITRE 3! BCOL ! FCOL ! BORD !
  MAXHAUT
  SHADOW @
  IF
    1-
  THEN
  MIN HAUT !
  MAXLARG
  SHADOW @
  IF
    2-
  THEN
  MIN LARG !
  POSY ! POSX !
  WALLOC
  (W.FEN)
  DARK
;

\ Ouvre une fen�tre.
: WOUVRE  ( pos_x pos_y larg haut forme_bord coul_fen coul_bord $<titre> coul_titre shadow --- )
  FNUM @ >WTABLE
  RAZ-WPARAM
  (WOUVRE)
;

ONLY FORTH FEN ALSO FORTH ALSO DEFINITIONS
CR .( Gestionnaire de FENETRES charg� )
EOF

