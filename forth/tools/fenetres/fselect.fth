\ -----------------------------------------------------------------------------
\                             Extension … FMENU.FTH
\
\ Gestion de fenˆtres de type s‚lection.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? WMENU NOT ?\ INCLUDE FMENU

FEN DEFINITIONS
DECIMAL

\ -----------------------------------------------------------------------------
\                        Manipulation des paramŠtres de menu
\ -----------------------------------------------------------------------------
\ Indice … partir duquel l'affichage d‚bute.
: #F1ER  ( --- adr )
  WPARAM 50 +
;

\ Sauvegarde du caractŠre remplac‚ par le symbole de continuation
\ vers le haut.
: #CARH  ( --- adr )
  WPARAM 52 +
;

\ Sauvegarde du caractŠre remplac‚ par le symbole de continuation
\ vers le bas.
: #CARB  ( --- adr )
  WPARAM 54 +
;

\ -----------------------------------------------------------------------------
\                             Fonctions de s‚lection
\ -----------------------------------------------------------------------------
\ Affichage du niŠme nom … la ligne l.
: (.FNOM)  ( l n --- )
  OPTX @ ROT AT >R
  $OPTIONS 3@ R>
  LITEM LTYPE
;

\ Primitive d'affichage du menu de s‚lection.
: (W.FMENU)  ( --- )
  CR-OFF
  OPTX @ OPTY @ (AT) 2DROP
  HAUT @ 1+ 1
  DO
    I DUP
    #F1ER @ 1 >
    IF
      #F1ER @ + 1-
    THEN
    (.FNOM)
  LOOP
  CURX @ CURY @ AT
  CR-ON
;

\ Sauvegarde le caractŠre remplac‚ par le symbole de continuation vers
\ le haut.
: #CARH!  ( --- )
  LARG @ 1+
  0 WSCR LC@
  #CARH !
;

\ Restitue le caractŠre remplac‚ par le symbole de continuation vers
\ le haut.
: #CARH@  ( --- )
  #CARH @
  LARG @ 1+
  0 WSCR LC!
;

\ Affiche un ‚ventuel symbole de continuation vers le haut.
: (.FHAUT)  ( --- )
  LARG @ 1+
  0 WSCR
  #F1ER @ 1 >
  IF
    24
  ELSE
    #CARH @
  THEN
  BCOL @ FLIP + -ROT L!
;

\ Sauvegarde le caractŠre remplac‚ par le symbole de continuation vers
\ le bas.
: #CARB!  ( --- )
  LARG @ 1+
  HAUT @ 1+ WSCR LC@
  #CARB !
;

\ Restitue le caractŠre remplac‚ par le symbole de continuation vers
\ le bas.
: #CARB@  ( --- )
  #CARB @
  LARG @ 1+
  HAUT @ 1+ WSCR LC!
;

\ Affiche un ‚ventuel symbole de continuation vers le bas.
: (.FBAS)  ( --- )
  LARG @ 1+
  HAUT @ 1+ WSCR
  HAUT @ #OPTIONS @ #F1ER @ - 1+
  <
  IF
    25
  ELSE
    #CARB @
  THEN
  BCOL @ FLIP + -ROT L!
;

\ Gestion de la fenˆtre de s‚lection courante.
\ Touches g‚r‚es :
\                  - Les flŠches haute et basse pour les d‚placements.
\                  - La touche Home pour s‚lectionner le premier nom.
\                  - La touche Fin pour s‚lectionner le dernier nom.
\                  - La touche PgUp pour passer … la page pr‚c‚dente.
\                  - La touche PgDn pour passer … la page suivante.
\                  - La touche Return pour s‚lectionner le nom courant.
\                  - La touche Escape pour abandonner.
\ Empile le nø de l'option choisie, ou 0 si abandon.
: WGSELECT  ( --- n )
  CURSOR-OFF
  CR-OFF
  OPTX @ OPTY @ (AT) 2DROP
  #CARH!
  #CARB!
  BEGIN
    (.FHAUT)
    (.FBAS)
    OPTION-ON
    FALSE
    KEY?
    IF
      KEY
      CASE #FLH OF           \ flŠche Haute
                  OPTION-OFF
                  OPTY @ DUP 1 >
                  IF
                    DROP OPTY 1-!
                    OPTION# 1-!
                  ELSE
                    1 =
                    IF
                      #F1ER @ 1 >
                      IF
                        #F1ER 1-!
                        SCROLLDWN WSCROLL
                        1 DUP OPTY !
                        #F1ER @ DUP OPTION# !
                        (.FNOM)
                      THEN
                    THEN
                  THEN
                ENDOF
           #FLB OF           \ flŠche Basse
                  OPTION-OFF
                  OPTY @ DUP HAUT @ <>
                  IF
                    #OPTIONS @ #F1ER @ - 1+ <
                    IF
                      OPTY 1+!
                      OPTION# @ 1+
                      #OPTIONS @ MIN
                      OPTION# !
                    THEN
                  ELSE
                    DROP #F1ER @
                    #OPTIONS @ HAUT @ -
                    <=
                    IF
                      #F1ER 1+!
                      SCROLLUP WSCROLL
                      HAUT @ DUP OPTY !
                      #F1ER @ HAUT @ + 1-
                      #OPTIONS @ MIN
                      DUP OPTION# !
                      (.FNOM)
                    THEN
                  THEN
                ENDOF
           #PGU OF           \ Page pr‚c‚dente.
                  #F1ER @ 1 <>
                  IF
                    DARK
                    #F1ER @ HAUT @ -
                    1 MAX
                    DUP #F1ER !
                    OPTION# !
                    1 OPTY !
                    (W.FMENU)
                  THEN
                ENDOF
           #PGD OF           \ Page suivante.
                  #F1ER @ #OPTIONS @ <>
                  IF
                    DARK
                    #F1ER @ HAUT @ +
                    #OPTIONS @ MIN
                    DUP #F1ER !
                    OPTION# !
                    1 OPTY !
                    (W.FMENU)
                  THEN
                ENDOF
           #HOM OF           \ Premier nom.
                  DARK
                  1 DUP #F1ER !
                  DUP OPTY !
                  OPTION# !
                  (W.FMENU)
                ENDOF
           #END OF           \ Dernier nom.
                  DARK
                  #OPTIONS @ HAUT @ - 1+
                  1 MAX
                  DUP #F1ER !
                  HAUT @ 1- +
                  #OPTIONS @ MIN OPTION# !
                  HAUT @ #OPTIONS @ #F1ER @ -
                  1+ MIN OPTY !
                  (W.FMENU)
                ENDOF
           #RET OF           \ Validation
                  DROP OPTION# @ TRUE
                ENDOF
           #ESC OF           \ Abandon
                  DROP 0 TRUE
                ENDOF
      ENDCASE
    THEN
  UNTIL
  #CARH@
  #CARB@
  CURX @ CURY @ AT
  CR-ON
  CURSOR-ON
;

\ Ouvre une fenˆtre de type s‚lection.
: WSELECT  ( pos_x pos_y larg haut forme_bord coul_fen coul_bord $<titre> coul_titre shadow $<options> ocol #1er --- )
  FNUM @ >WTABLE
  RAZ-WPARAM
  MVERTICAL TMENU !
  DUP #F1ER !
  OPTION# !
  OCOL !
  3DUP LITEMS #OPTIONS !
  $OPTIONS 3!
  1 OPTX !
  1 OPTY !
  (WOUVRE)
  (W.FMENU)
;

ONLY FORTH FEN ALSO FORTH ALSO DEFINITIONS
CR .( Gestionnaire de FENETRES DE SELECTION charg‚ )
EOF

