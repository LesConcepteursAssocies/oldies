\ -----------------------------------------------------------------------------
\                      Simulation simplifi‚e du Perceptron.
\
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? HFILE NOT ?\ INCLUDE DFILES
EXIST? WOUVRE NOT ?\ INCLUDE FEN
EXIST? CREER-CMATRICE NOT ?\ INCLUDE MATRICES

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

10 CONSTANT DIMX
10 CONSTANT DIMY
DEFER BOOL-FUNC              \ Fonction bool‚enne … appliquer aux cellules
' OR IS BOOL-FUNC            \ de la r‚tine.

VARIABLE SEUIL               \ Seuil d'activation de la cellule de d‚cision.
VARIABLE DECISION            \ Cellule de d‚cision.
VARIABLE K                   \ Utilis‚e dans la fonction d'apprentissage.
1 K !

\ Cr‚ation de la r‚tine.
DIMX DIMY CREER-CMATRICE RETINE

\ Cr‚ation de la couche association.
DIMX DIMY CREER-CMATRICE ASSOCIATION

\ Cr‚ation de la matrice des connexions entre la r‚tine
\ et la couche association.
DIMX DIMY CREER-CMATRICE CONNEXIONS

\ Cr‚ation de la matrice des poids entre la couche association
\ et la couche de d‚cision.
DIMX DIMY CREER-MATRICE POIDS

80 STRING0 PCONFIG           \ contient les diff‚rentes configurations du perceptron.
" PCONFIG.DAT" PCONFIG $!

\ D‚finition des champs du fichier configuration.
  0 100 FIELD FRETINE        \ Etat de la r‚tine.
100 100 FIELD FASSO          \ Etat de la couche association.
200 100 FIELD FCONNECT       \ Etat de la matrice des connexions.
300 200 FIELD FPOIDS         \ Etat de la matrice des poids.
500 2   FIELD FSEUIL         \ Etat de la variable seuil.
502 2   FIELD FDECISION      \ Etat de la cellule de d‚cision.
504 2   FIELD FFONC          \ Etat de la fonction bool‚enne.

VARIABLE RSPCONFIG           \ Taille de l'enregistrement.
VARIABLE HPCONFIG            \ Handler du fichier.

\ Ouverture ou cr‚ation du fichier de configuration.
: OPEN-CONFIG  ( --- )
  ?OPEN
  PCONFIG DROP
  DUP 0 (SEARCH0)
  IF
    READ-WRITE (OPEN)
  ELSE
    0 (CREATE)
  THEN
  ?DOS-ERR
  DUP HPCONFIG !
  506 RSPCONFIG !
  0 0 ROT 2 (SEEK)
  ?DOS-ERR 2DROP
;

\ Initialisation du fichier de configuration comme fichier principal.
: >PCONFIG  ( --- )
  RSPCONFIG @ REC-SIZE !
  HPCONFIG @ HFILE !
;

\ Fermeture du fichier de configuration.
: CLOSE-CONFIG  ( --- )
  >PCONFIG FCLOSE
;

\ Lecture de la niŠme configuration.
: LIT-CONFIG  ( n --- )
  >PCONFIG
  BUF-DIR RSPCONFIG @ BLANK
  FREAD
  FRETINE 0 0 RETINE SWAP CMOVE
  FASSO 0 0 ASSOCIATION SWAP CMOVE
  FCONNECT 0 0 CONNEXIONS SWAP CMOVE
  FPOIDS 0 0 POIDS SWAP CMOVE
  FSEUIL DROP @ SEUIL !
  FDECISION DROP @ DECISION !
  FFONC DROP @ IS BOOL-FUNC
;

\ Ecriture de la niŠme configuration.
: ECRIT-CONFIG  ( n --- )
  >PCONFIG
  BUF-DIR RSPCONFIG @ BLANK
  0 0 RETINE FRETINE CMOVE
  0 0 ASSOCIATION FASSO CMOVE
  0 0 CONNEXIONS FCONNECT CMOVE
  0 0 POIDS FPOIDS CMOVE
  SEUIL @ FSEUIL DROP !
  DECISION @ FDECISION DROP !
  ['] BOOL-FUNC >BODY @ FFONC DROP !
  FWRITE
;

\ Mise en place des connexions.
\ Chaque bit correspond … une connexion.
\ Valeurs :   1 : cellule connect‚e … celle du haut
\             2 :   "        "      "   "   du haut … droite
\             4 :   "        "      "   "   de droite
\             8 :   "        "      "   "   du bas … droite
\            16 :   "        "      "   "   bas
\            32 :   "        "      "   "   bas … gauche
\            64 :   "        "      "   "   de gauche
\           128 :   "        "      "   "   du haut … gauche
\ Par d‚faut, chaque cellule est connect‚e … toutes celles
\ qui l'entoure.
: CONNECTER  ( --- )
  28  0 0 CONNEXIONS C!      \ coin sup‚rieur gauche.
  112 0 9 CONNEXIONS C!      \ coin sup‚rieur droit.
  7   9 0 CONNEXIONS C!      \ coin inf‚rieur gauche.
  193 9 9 CONNEXIONS C!      \ coin inf‚rieur droit.
  9 1
  DO
    124 0 I CONNEXIONS C!    \ Bord sup‚rieur.
    199 9 I CONNEXIONS C!    \ Bord inf‚rieur.
    31  I 0 CONNEXIONS C!    \ Bord gauche.
    241 I 9 CONNEXIONS C!    \ Bord droit.
  LOOP
  9 1
  DO
    9 1
    DO
      255 J I CONNEXIONS C!  \ Les cellules int‚rieurs.
    LOOP
  LOOP
;

\ Remise … z‚ro de la r‚tine.
: RAZ-RETINE  ( --- )
  DIMX 0
  DO
    DIMY 0
    DO
      0 J I RETINE C!
    LOOP
  LOOP
;

\ Mise … 1 des poids.
\ … utiliser quand on veut apprendre une nouvelle forme au perceptron.
: SET-POIDS  ( --- )
  DIMX 0
  DO
    DIMY 0
    DO
      1 J I POIDS !
    LOOP
  LOOP
;

\ Dessin du fond d'‚cran.
: .DESKTOP  ( --- )
  0 0 78 23 0 CYAN BLANC F/C
  CYAN BLANC F/C DSEGMENT " " NOIR NOIR F/C FALSE WOUVRE
;

\ Message et pause.
: MSGPAUSE  ( --- )
  25 19 27 1 4 NOIR BLANC F/C
  NOIR VERT-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
  ." Appuyez sur une touche ..."
  KEY DROP
  WFERME
;

\ Affichage des rŠgles de graduation dans la fenˆtre courante.
: .REGLE  ( --- )
  2 1 AT
  DIMX 0
  DO
    I 48 + EMIT
  LOOP
  DIMY 0
  DO
    1 I 2+ AT I .
  LOOP
;

\ Affichage de la r‚tine.
: (.RETINE)  ( --- )
  CR-OFF
  0 6 11 11 1 NOIR BLANC F/C
  VERT BLEU F/C DSEGMENT " R‚tine" NOIR BLANC F/C TRUE WOUVRE
  .REGLE
  DIMX 0
  DO
    DIMY 0
    DO
      J 2+ I 2+ AT
      J I RETINE C@
      IF
        178 EMIT
      THEN
    LOOP
  LOOP
  CR-ON
;

: .RETINE  ( --- )
  (.RETINE)
  MSGPAUSE
  WFERME
;

\ Affichage de la couche association.
: (.ASSOCIATION)  ( --- )
  CR-OFF
  16 6 12 11 1 NOIR BLANC F/C
  VERT BLEU F/C DSEGMENT " Association" NOIR BLANC F/C TRUE WOUVRE
  .REGLE
  FCOL @
  3 FCOL !
  DIMX 0
  DO
    DIMY 0
    DO
      J 2+ I 2+ AT
      J I ASSOCIATION C@ .
    LOOP
  LOOP
  FCOL !
  CR-ON
;

\ Entr‚e d'informations dans la r‚tine.
: >RETINE  ( --- )
  15 6 60 3 4 BLEU ROUGE F/C
  BLEU JAUNE F/C DSEGMENT " Saisie" NOIR BLANC F/C TRUE WOUVRE
  ." Entrez 0 ou 1 pour allumer ou ‚teindre le point courant" CR
  ." <Ù  : Passe au point suivant sans modification"CR
  ." Esc : Sortie"
  (.RETINE)
  DIMX 0
  DO
    0
    DIMY 0
    DO
      DROP KEY DUP
      27 = ?LEAVE
      DUP 13 <>
      IF
        DUP 48 >=
        IF
          DUP 48 - I J RETINE C!
          I 2+ J 2+ AT
          I J RETINE C@
          IF
            178 EMIT
          ELSE
            BL EMIT
          THEN
        THEN
      THEN
    LOOP
    27 = ?LEAVE
  LOOP
  WFERME
  WFERME
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du haut.
: CONNECT-H?  ( l c --- fl )
  CONNEXIONS C@
  1 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du bas.
: CONNECT-B?  ( l c --- fl )
  CONNEXIONS C@
  16 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle de droite.
: CONNECT-D?  ( l c --- fl )
  CONNEXIONS C@
  4 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle de gauche.
: CONNECT-G?  ( l c --- fl )
  CONNEXIONS C@
  64 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du haut … droite.
: CONNECT-HD?  ( l c --- fl )
  CONNEXIONS C@
  2 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du haut … gauche.
: CONNECT-HG?  ( l c --- fl )
  CONNEXIONS C@
  128 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du bas … droite.
: CONNECT-BD?  ( l c --- fl )
  CONNEXIONS C@
  8 AND
;

\ Renvoie VRAI si la cellule L,C est connect‚e … celle du bas … gauche.
: CONNECT-BG?  ( l c --- fl )
  CONNEXIONS C@
  32 AND
;

\ Lecture de la r‚tine et calcul de la couche association.
\ Chaque cellule de la couche association est connect‚e
\ … son ‚quivalent de la r‚tine, ainsi qu'aux cellules voisines, en fonction
\ du contenu de la matrice CONNEXIONS.
\ Une fonction bool‚enne (contenue dans BOOL-FUNC) est appliqu‚e.
\ Le r‚sultat est stock‚ dans la cellule de la couche association
\ correspondante.
: LIT-RETINE  ( --- )
  DIMX 0
  DO
    DIMY 0
    DO
      J I RETINE C@          \ n ---
      J I CONNECT-H?
      IF
        J 1- I RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-B?
      IF
        J 1+ I RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-D?
      IF
        J I 1+ RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-G?
      IF
        J I 1- RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-HD?
      IF
        J 1- I 1+ RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-HG?
      IF
        J 1- I 1- RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-BD?
      IF
        J 1+ I 1+ RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I CONNECT-BG?
      IF
        J 1+ I 1- RETINE C@
        BOOL-FUNC            \ n &op n' --- n
      THEN
      J I ASSOCIATION C!
    LOOP
  LOOP
;

\ Seuillage de la cellule de d‚cision.
\ Formule : Oh = 1 si äj WjEjh >= W0
\ Oh  = R‚ponse fournie par la cellule de d‚cision pour l'exemple h.
\ Ejh = Valeur du j-iŠme ‚l‚ment de l'exemple h.
\ Wj  = Poids de la connexion reliant la j-iŠme cellule d'association … la
\       cellule de d‚cision.
\ W0  = Seuil de la cellule de d‚cision.
: SEUILLAGE  ( --- Oh )
  LIT-RETINE                 \ Lecture de la r‚tine et calcul de la couche association.
  0                          \ Oh ---
  DIMX 0
  DO
    DIMY 0
    DO
      J I POIDS @            \ Oh Wj ---
      J I ASSOCIATION C@     \ Oh Wj Ejh ---
      * +                    \ Oh
    LOOP
  LOOP
  DUP SEUIL @ >=
  IF
    1 DECISION !
  ELSE
    0 DECISION !
  THEN                       \ Oh ---
  (.RETINE)
  (.ASSOCIATION)
  35 6 30 10 2 BLEU ROUGE F/C
  BLEU JAUNE F/C DSEGMENT " Seuillage" CYAN BLANC F/C TRUE WOUVRE
  ." Seuil               : " SEUIL ? CR
  ." R‚ponse fournie     : " DUP . CR
  ." Cellule de d‚cision : " DECISION ?
  MSGPAUSE
  WFERME
  WFERME
  WFERME
;

\ RŠgle d'apprentissage du perceptron.
\ La formule appliqu‚e est la rŠgle de Widrow-Hoff :
\  - On pr‚sente un exemple Eh au r‚seau.
\  - On calcule h = Th - Oh
\    Th = r‚ponse souhait‚e pour l'exemple h.
\    Oh = r‚ponse fournie pour l'exemple h.
\ - On modifie chaque poids Wj de Wj = k.h.Ejh avec k > 0.
: APPREND  ( Th --- )
  SEUILLAGE -                \ h ---
  DIMX 0
  DO
    DIMY 0
    DO
      K @ OVER *             \ h k*h ---
      J I ASSOCIATION C@ *   \ h k*h*Ejh ---
      J I POIDS @ +          \ h Wj+Wj ---
      J I POIDS !            \ h ---
    LOOP
  LOOP
  DROP
;

\ Fin du programme.
: FIN  ( --- )
  CLOSE-CONFIG
  WALLCLOSE
;

\ Affichage du menu.
: .MENU  ( --- )
  0 0 78 11 3 MAGENTA NOIR F/C
  CYAN ROUGE-CLAIR F/C DSEGMENT " Perceptron, Commandes Disponibles"
  BLEU CYAN-CLAIR F/C FALSE WOUVRE
  ." CONNECTER      : Mise en place des connexions par d‚faut" CR
  ." RAZ-RETINE     : Remise … z‚ro du contenu de la r‚tine" CR
  ." SET-POIDS      : Remise … un de la matrice des poids" CR
  ." .RETINE        : Affichage de la r‚tine" CR
  ." n LIT-CONFIG   : Lecture du n-iŠme ‚tat sauvegard‚" CR
  ." n ECRIT-CONFIG : Ecriture de l'‚tat courant dans le n-iŠme enregistrement." CR
  ." LIT-RETINE     : Lecture du contenu de la r‚tine" CR
  ." SEUILLAGE      : Seuillage de la cellule de d‚cision" CR
  ." n APPREND      : Apprentissage du perceptron (n est la r‚ponse souhait‚e)" CR
  ." >RETINE        : Saisie d'informations dans la r‚tine" CR
  ." FIN            : Fin du programme"
;

\ Le programme principal.
: PERCEPTRON  ( --- )
  WINIT
  .DESKTOP
  OPEN-CONFIG
  .MENU
  0 20 78 3 1 BRUN JAUNE F/C
  BLEU JAUNE F/C DSEGMENT " Tapez votre commande"
  VERT MAGENTA-CLAIR F/C FALSE WOUVRE
;

EOF

