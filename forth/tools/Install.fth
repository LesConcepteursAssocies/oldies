\ -----------------------------------------------------------------------------
\                            Programme d'installation.
\
\ Voir doc en fin de fichier.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? COMSPEC! NOT ?\ INCLUDE COMSPEC
EXIST? MALLOC NOT ?\ INCLUDE MEMORY
EXIST? WMENU NOT ?\ INCLUDE FMENU

2   STRING LECTEUR           \ Lecteur de destination.
2   STRING LSOURCE           \ Lecteur source.
64  STRING CHEMIN            \ Chemin de destination.
20  STRING PROG$             \ Nom du programme de d‚compactage.
20  STRING PARAM$            \ ParamŠtres du prog. de d‚compactage.
80  STRING COMMANDE          \ Ligne de commande.
65  STRING SAISIE            \ Chaine de saisie.

2VARIABLE #O/I               \ Octets n‚c‚ssaires pour l'installation.
1051129. #O/I 2!

" C:" LECTEUR $!             \ Unit‚ C: par d‚faut.
" A:" LSOURCE $!             \ Unit‚ A: par d‚faut.
" x /n2 " PARAM$ $!
" TBFTOOLS " COMMANDE $!
" LHA.EXE " PROG$ $!

: MSGPAUSE  ( --- )
  25 17 27 1 3 VERT-CLAIR MAGENTA F/C
  NOIR VERT-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
  ." Appuyez sur une touche ..."
  KEY DROP
  WFERME
;

: LOGO  ( --- )
  5 10 AT
 ." ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿"
  5 11 AT
 ." ³ßÛß Û Û ÛßÛ Ûßİ ÛßÛ     Ûßß ÛßÛ ÛßÛ ßÛß Û Û     ßÛß ÛßÛ ÛßÛ Û   Ûßß³"
  5 12 AT
 ." ³ Û  Û Û Ûßİ ÛßÛ Û Û     Ûß  Û Û Ûßİ  Û  ÛßÛ      Û  Û Û Û Û Û   ßßÛ³"
  5 13 AT
 ." ³ Û  ÛÜÛ Û Û ÛÜÛ ÛÜÛ     Û   ÛÜÛ Û Û  Û  Û Û      Û  ÛÜÛ ÛÜÛ ÛÜÜ ÜÜÛ³"
  5 14 AT
 ." ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"
  25 15 AT
 ." Auteur: Guillaumaud Philippe"
;

\ Dessin du fond d'‚cran.
: .DESKTOP  ( --- )
  0 0 78 23 0 CYAN BLANC F/C
  CYAN BLANC F/C DSEGMENT " " NOIR NOIR F/C FALSE WOUVRE
  LOGO
;

\ Renvoi l'espace disponible en octets du lecteur choisi.
: ESPACE  ( $adr l --- d )
  DROP C@ UPC
  64 - (FREE) * UM*
;

\ V‚rifications sur le lecteur choisi.
: VERIF  ( $adr l --- fl )
  2DUP DROP C@ UPC 64 - (FREE)
  NIP NIP 0<
  IF
    BEEP 10 MS BEEP
    23 5 33 1 2 NOIR BLANC F/C
    BLEU JAUNE F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
    ." D‚signation d'unit‚ non valide !"
    MSGPAUSE
    WFERME
    2DROP
    FALSE
  ELSE
    ESPACE #O/I 2@ DU<
    IF
      BEEP 10 MS BEEP
      19 5 40 3 2 NOIR BLANC F/C
      BLEU JAUNE F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
      ." Espace disque insuffisant."
      CR ." J'ai besoin d'au moins " #O/I 2@ UD. ." octets"
      CR ." pour pouvoir effectuer l'installation."
      MSGPAUSE
      WFERME
      FALSE
    ELSE
      TRUE
    THEN
  THEN
;

\ Choix du lecteur source.
: LSOURCE!  ( --- )
  OPTX @ 2 2 2 1 NOIR BLANC F/C VERT BLANC F/C DSEGMENT " "
  NOIR NOIR F/C TRUE DSEGMENT " A: B:" CYAN BLEU-CLAIR F/C
  MVERTICAL WMENU
  WGESMENU WFERME
  CASE
    1 OF
        " A:" LSOURCE $!
      ENDOF
    2 OF
        " B:" LSOURCE $!
      ENDOF
  ENDCASE
;

\ Choix du lecteur de destination.
: LECTEUR!  ( --- )
  OPTX @ 2 10 1 1 BLEU JAUNE F/C
  VERT MAGENTA-CLAIR F/C DSEGMENT " Unit‚" ROUGE CYAN F/C TRUE WOUVRE
  1 1 AT LECTEUR TYPE 1 1 AT
  SAISIE INPUT$
  SAISIE NIP
  IF
    SAISIE VERIF
    IF
      SAISIE DROP C@ LECTEUR DROP C!
    THEN
  THEN
  WFERME
;

\ Choix du r‚pertoire de destination.
: CHEMIN!  ( --- )
  OPTX @ 5 - 2 64 1 1 BLEU JAUNE F/C
  VERT MAGENTA-CLAIR F/C DSEGMENT " R‚pertoire" ROUGE CYAN F/C TRUE WOUVRE
  1 1 AT CHEMIN TYPE 1 1 AT
  SAISIE INPUT$
  SAISIE NIP
  IF
     SAISIE CHEMIN $!
     CHEMIN NIP 1 >
     IF
       " \" CHEMIN APPEND$
     THEN
  THEN
  WFERME
;

\ Ex‚cution de la commande d'installation.
: EXEC-INSTALL  ( --- )
  4 17 70 4 3 GRIS-CLAIR NOIR F/C
  BLEU JAUNE F/C DSEGMENT " Installer" NOIR BLANC F/C
  TRUE DSEGMENT " Oui Non" ROUGE BLANC F/C MHORIZONTAL WMENU
  CR ." Destination  : " CR
  LECTEUR TYPE CHEMIN TYPE
  WGESMENU 1 =
  IF
    COMMANDE PASS PROG$ $EXECUTE
    BEEP
    29 13 25 1 2 GRIS-CLAIR NOIR F/C
    VERT MAGENTA-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
    ." Installation termin‚e."
    MSGPAUSE
    WFERME
  THEN
  WFERME
;

\ Le programme principal.
: INSTALL  ( --- )
  WINIT
  EKEY-ON
  .DESKTOP
  0 0 78 8 2 GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT " Installation" ROUGE CYAN F/C
  FALSE DSEGMENT " Installe Unit‚ Chemin Source Quitter" NOIR BLANC F/C
  MHORIZONTAL WMENU
  CR CR
  CR ." Installe : Lance l'installation des Utilitaires."
  CR ." Unit‚    : Choix de l'unit‚ de destination."
  CR ." Chemin   : Choix du r‚pertoire de destination."
  CR ." Source   : Choix de l'unit‚ source du d‚compacteur et du fichier archive."
  CR ." Quitter  : Quitte le programme."
  BEGIN
    WGESMENU DUP
    CASE
      1 OF
          EXEC-INSTALL
        ENDOF
      2 OF
          LECTEUR!
        ENDOF
      3 OF
          CHEMIN!
        ENDOF
      4 OF
          LSOURCE!
        ENDOF
    ENDCASE
    5 =
    IF
      OPTX @ 2 3 2 3 GRIS-CLAIR NOIR F/C VERT BLANC F/C DSEGMENT " "
      NOIR NOIR F/C TRUE DSEGMENT " Non Oui" NOIR BLANC F/C
      MVERTICAL WMENU
      WGESMENU WFERME
      2 =
      IF
        TRUE
      ELSE
        FALSE
      THEN
    ELSE
      FALSE
    THEN
  UNTIL
  MSGPAUSE
  WALLCLOSE
  EKEY-OFF
  BYE
;

EOF
--------------------------------------------------------------------------------
Install permet d'installer un ensemble de fichiers rassembl‚s dans un
fichier compact‚.

Marche … suivre :
-----------------

- Totaliser le nombre d'octets occup‚s par les fichiers … installer
  et stocker le r‚sultat dans la variable double #O/I
  (utiliser pour cela XDIR.FTH si n‚c‚ssaire).
  Cette valeur est utilis‚e pour les tests d'espace disponible sur
  l'unit‚ choisie par l'utilisateur au moment de l'installation.

- Compacter l'ensemble des fichiers en utilisant le compacteur de son
  choix (ici LHA.EXE, voir la variable PROG$)
  Syntaxe de LHA.EXE :
  LHA a /rx NameLZH Files
  a       : pour ajouter dans le fichier archive.
  /rx     : pour collecter les sous-r‚pertoires.
  NameLZH : Nom du fichier archive (.LZH est assum‚ par d‚faut)
  Files   : Nom du ou des fichiers … archiver (on peut indiquer une unit‚ et
            un chemin)

- Recopier le compacteur et le fichier archive sur une disquette.

- Modifier le source de INSTALL.FTH, pour prendre en compte les nouveaux
  paramŠtres (Nom du compacteur, paramŠtres, espace occup‚)

- Rendre le programme ‚x‚cutable par la commande habituelle :
  ' INSTALL IS BOOT
  SAVE-SYSTEM INSTALLE.COM

Et voil… !

