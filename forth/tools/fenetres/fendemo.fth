\ -----------------------------------------------------------------------------
\                    D‚monstration du fenˆtrage dynamique.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? TMENU NOT ?\ INCLUDE FMENU

40 STRING $TITRE1
" Fenˆtrage dynamique avec Turbo Forth !" $TITRE1 $!

10 STRING $NOTITRE

: MSGPAUSE  ( --- )
  35 13 27 1 3
  VERT-CLAIR MAGENTA F/C
  NOIR VERT-CLAIR F/C
  DSEGMENT $NOTITRE NOIR TRUE WOUVRE
  ." Appuyez sur une touche ..."
  KEY DROP
  WFERME
;

: FENDEMO  ( --- )
  WINIT
  EKEY-ON
  0 0 78 23 0 BRUN NOIR F/C BRUN NOIR F/C DSEGMENT " " NOIR FALSE WOUVRE
  0 0 78 10 2
  GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C
  DSEGMENT $TITRE1 ROUGE CYAN F/C FALSE WOUVRE
  ." Des fenˆtres dynamiques de toutes les tailles, de toutes les couleurs !"
  BLEU ROUGE F/C 62 1 8 WXCHGCOL
  5 0
  DO
    I 3 * 2+ I 2* 3 + 10 5 I I 5 * 1+ I 10 * 2+
    DSEGMENT $NOTITRE 0 FALSE WOUVRE
    ." Bord nø " I .
  LOOP
  0 20 50 3 1
  CYAN GRIS-FONCE F/C
  VERT VERT-CLAIR F/C DSEGMENT " Titre" NOIR BLANC F/C FALSE WOUVRE
  ." Voici les cinq types de cadres support‚s..."
  MSGPAUSE
  27 5 50 15 2 CYAN BLANC F/C BLEU BLANC F/C
  DSEGMENT $NOTITRE NOIR FALSE WOUVRE
  ." La derniŠre fenˆtre ouverte devient l'‚cran " CR
  ." principal, dans laquelle s'effectuent les" CR
  ." traitements"
  MSGPAUSE
  WORDS
  MSGPAUSE
  2 2 40 10 1
  NOIR ROUGE-CLAIR F/C
  BLEU BLEU-CLAIR F/C DSEGMENT $NOTITRE NOIR TRUE WOUVRE
  ." La gestion de la fin de ligne et de scrolling se fait automatiquement"
  ."  quelle que soit la longueur de la chaine affich‚e"
  ROUGE JAUNE F/C 4 1 26 WXCHGCOL
  MSGPAUSE
  7 0
  DO
    SCROLLDWN WSCROLL
  LOOP
  MSGPAUSE
  5 5 60 15 0
  BLEU VERT F/C
  MAGENTA MAGENTA F/C DSEGMENT " Propri‚t‚s" VERT MAGENTA-CLAIR F/C TRUE WOUVRE
  ." Propri‚t‚s d'une fenˆtre :" CR CR
  ."   - Position dans l'‚cran physique" CR
  ."   - Taille (largeur, hauteur)" CR
  ."   - Type de bord" CR
  ."   - Couleur de la fenˆtre" CR
  ."   - Position du curseur dans la fenˆtre" CR
  ."   - Num‚ro de la fenˆtre" CR
  ."   - Couleur du bord" CR
  ."   - Titre ‚ventuel" CR
  ."   - Couleur du titre" CR
  ."   - Ombrage ‚ventuel"
  MSGPAUSE
  10 10 20 5 4 VERT BRUN F/C CYAN ROUGE-CLAIR F/C DSEGMENT " " NOIR TRUE WOUVRE
  ." Jusqu'… " #WMAXI . ." Fenˆtres … l'‚cran !"
  MSGPAUSE
  29 3 30 9 3
  BRUN JAUNE F/C
  NOIR GRIS-CLAIR F/C DSEGMENT " Couleurs" NOIR VERT-CLAIR F/C TRUE WOUVRE
  ." Vous pouvez modifier la couleur d'un bloc sur une ligne ..."
  MSGPAUSE
  ROUGE-CLAIR NOIR F/C 13 1 8 WXCHGCOL
  MSGPAUSE
  ." ... ou d'un ensemble de lignes d'un seul coup !"
  MSGPAUSE
  CYAN NOIR F/C 5 2 LARG @ 7 - 2 WXCHGCOLS
  MSGPAUSE
  15 12 32 5 2
  NOIR VERT-CLAIR F/C
  GRIS-CLAIR MAGENTA-CLAIR F/C DSEGMENT " D‚placements"
  BLEU JAUNE F/C TRUE WOUVRE
  ." Une fenˆtre, ca peut se d‚placer avec les flŠches"
  ."  (Appuyez sur Escape pour arreter)"
  BEGIN
    KEY?
    IF
      KEY DUP
      CASE #FLH OF           \ flŠche Haute
                  WMOVUP
                ENDOF
           #FLB OF           \ flŠche Basse
                  WMOVDN
                ENDOF
           #FLG OF           \ flŠche Gauche
                  WMOVLFT
                ENDOF
           #FLD OF           \ flŠche Droite
                  WMOVRGT
                ENDOF
      ENDCASE
      #ESC =
    ELSE
      FALSE
    THEN
  UNTIL
  3 3 35 5 1
  GRIS-CLAIR ROUGE F/C
  NOIR VERT-CLAIR F/C DSEGMENT " Redimensionnements"
  BLEU MAGENTA F/C TRUE WOUVRE
  ." Vous pouvez aussi redimensionner la fenˆtre courante"
  ."  (avec les flŠches, puis Escape pour arreter)"
  BEGIN
    KEY?
    IF
      KEY DUP
      CASE #FLH OF           \ flŠche Haute
                  WHAUT-
                ENDOF
           #FLB OF           \ flŠche Basse
                  WHAUT+
                ENDOF
           #FLG OF           \ flŠche Gauche
                  WLARG-
                ENDOF
           #FLD OF           \ flŠche Droite
                  WLARG+
                ENDOF
      ENDCASE
      #ESC =
    ELSE
      FALSE
    THEN
  UNTIL
  MSGPAUSE
  WFERME
  MSGPAUSE
  WFERME
  MSGPAUSE
  WFERME
  MSGPAUSE
  WFERME
  MSGPAUSE
  WFERME
  7 0
  DO
    SCROLLUP WSCROLL
  LOOP
  MSGPAUSE
  WFERME
  DARK
  ." Retour dans la fenˆtre pr‚c‚dente"
  MSGPAUSE
  WFERME
  DARK
  ." la fenˆtre pr‚c‚dente redevient" CR
  ." la fenˆtre courante"
  MSGPAUSE
  WALLCLOSE
  EKEY-OFF
;

: MENUDEMO  ( --- )
  WINIT
  EKEY-ON
  0 0 78 10 2
  GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT " Menu horizontal" ROUGE CYAN F/C FALSE
  DSEGMENT " Option1 Option2 Option3 Option4 CeciEstl'Option5 Quitter"
  NOIR BLANC F/C MHORIZONTAL WMENU
  CR CR ." Concevoir une gestion arborescente de menus d‚roulants est trŠs simple"
  CR ." avec Turbo-Forth et FEN !"
  CR ." d‚placez-vous avec les flŠches, validez avec <Ù ou abandonnez avec Escape"
  CR ." S‚lectionnez Quitter pour finir."
  BEGIN
    WGESMENU
    1 9 AT ." Option choisie : " DUP .
    DUP 1 5 BETWEEN
    IF
      OPTX @ 2 14 5 1
      NOIR ROUGE-CLAIR F/C BLEU BLEU-CLAIR F/C DSEGMENT " Vertical"
      ROUGE CYAN F/C TRUE
      DSEGMENT " Option1 Option2 Voicil'Option3 Option4 Option5" BLEU VERT-CLAIR F/C
      MVERTICAL WMENU
      WGESMENU WFERME
      25 9 AT ." Sous-Option choisie : " .
    THEN
    6 =
  UNTIL
  MSGPAUSE
  WALLCLOSE
  EKEY-OFF
;

CR .( Tapez FENDEMO pour une d‚monstration de FENETRAGE )
CR .( Tapez MENUDEMO pour une d‚monstration de MENUS DEROULANTS )
EOF

