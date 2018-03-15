\ -----------------------------------------------------------------------------
\                      Petit systŠme Expert bas‚ sur le principe
\                      des rŠgles de production.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? HFILE NOT ?\ INCLUDE DFILES
EXIST? WMENU NOT ?\ INCLUDE FMENU

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

80 STRING0 BASE-REGLES       \ Base de rŠgles.
80 STRING0 BASE-FAITS        \ Base de faits.

" BREGLES.DAT" BASE-REGLES $!
" BFAITS.DAT" BASE-FAITS $!

\ D‚finition des champs de la base de rŠgles.
  0 200 FIELD RG-COND        \ Texte de la condition (4 lignes de 50 car.).
200 100 FIELD RG-ACT         \ Texte de l'action (2 lignes de 50 car.).

\ D‚finition des champs de la base de faits.
  0 100 FIELD FT-FAIT        \ Texte du fait (2 lignes de 50 car.).

VARIABLE HREGLE              \ Handler du fichier des rŠgles.
VARIABLE RSREGLE             \ Taille d'un enregistrement du fichier des rŠgles.
VARIABLE HFAIT               \ Handler du fichier des faits.
VARIABLE RSFAIT              \ Taille d'un enregistrement du fichier des faits.

CREATE #REGLE 300 ALLOT      \ Texte de la rŠgle courante.
                             \ Utilis‚ pour le chainage avant.

\ Dessin du fond d'‚cran.
: .DESKTOP  ( --- )
  0 0 78 23 0 CYAN BLANC F/C
  CYAN BLANC F/C DSEGMENT " " NOIR NOIR F/C FALSE WOUVRE
;

\ Message et pause.
: MSGPAUSE  ( --- )
  25 5 27 1 4 NOIR BLANC F/C
  NOIR VERT-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE WOUVRE
  ." Appuyez sur une touche ..."
  KEY DROP
  WFERME
;

\ Ouverture ou cr‚ation de la base de faits.
: OPEN-FAITS  ( --- )
  ?OPEN
  BASE-FAITS DROP
  DUP 0 (SEARCH0)
  IF
    READ-WRITE (OPEN)
  ELSE
    0 (CREATE)
  THEN
  ?DOS-ERR
  DUP HFAIT !
  100 RSFAIT  !
  0 0 ROT 2 (SEEK)
  ?DOS-ERR 2DROP
;

\ Ouverture ou cr‚ation de la base de rŠgles.
: OPEN-REGLES  ( --- )
  ?OPEN
  BASE-REGLES DROP
  DUP 0 (SEARCH0)
  IF
    READ-WRITE (OPEN)
  ELSE
    0 (CREATE)
  THEN
  ?DOS-ERR
  DUP HREGLE !
  300 RSREGLE !
  0 0 ROT 2 (SEEK)
  ?DOS-ERR 2DROP
;

\ Initialise la base de rŠgles comme fichier principal.
: >REGLES  ( --- )
  HREGLE @ HFILE !
  RSREGLE @ REC-SIZE !
;

\ Initialise la base de faits comme fichier principal.
: >FAITS  ( --- )
  HFAIT @ HFILE !
  RSFAIT @ REC-SIZE !
;

\ Fermeture des bases.
: CLOSE-BASES  ( --- )
  >REGLES FCLOSE
  >FAITS FCLOSE
;

\ Saisie du texte d'une rŠgle.
: REGLE!  ( --- )
  14 10 50 6 2 GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT " RŠgle" ROUGE CYAN F/C TRUE WOUVRE
  RG-COND EXPECT
  1 CURX ! CURY 1+! WLOCATE
  RG-ACT  EXPECT
  WFERME
;

\ Saisie du texte d'un fait.
: FAIT!  ( --- )
  14 8 50 2 2 GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT " Fait" ROUGE CYAN F/C TRUE WOUVRE
  FT-FAIT EXPECT
  WFERME
;

\ Ajout d'une rŠgle dans la base.
: AJOUTE-REGLE  ( --- )
  >REGLES
  BUF-DIR RSREGLE @ BLANK
  REGLE!
  FAPPEND
;

\ Ajout d'un fait dans la base.
: AJOUTE-FAIT  ( --- )
  >FAITS
  BUF-DIR RSFAIT @ BLANK
  FAIT!
  FAPPEND
;

\ Liste de la base de rŠgles.
: LISTE-REGLES  ( --- )
  14 10 50 10 1 BLEU JAUNE F/C
  VERT GRIS-FONCE F/C DSEGMENT " Liste des RŠgles" ROUGE CYAN-CLAIR F/C
  TRUE WOUVRE
  >REGLES
  RECCOUNT 0
  DO
    DARK
    I FREAD
    RG-COND TYPE
    CR RG-ACT TYPE
    MSGPAUSE
  LOOP
  WFERME
;

\ Liste de la base de faits.
: LISTE-FAITS  ( --- )
  14 10 50 10 1 BLEU JAUNE F/C
  VERT GRIS-FONCE F/C DSEGMENT " Liste des Faits" ROUGE CYAN-CLAIR F/C
  TRUE WOUVRE
  >FAITS
  RECCOUNT 0
  DO
    DARK
    I FREAD
    FT-FAIT TYPE
    MSGPAUSE
  LOOP
  WFERME
;

\ Chainage avant.
\ Principe : Balayage de la base de rŠgles.
\            Pour chaque rŠgle, on applique chacun des faits de la base de faits.
\            Si un fait concorde avec celui de la rŠgle courante, l'action de
\            celle-ci est a jout‚e … la base de fait.
: CHAINAGE-AVANT  ( --- )
  14 10 50 12 1 GRIS-CLAIR NOIR F/C
  VERT GRIS-FONCE F/C DSEGMENT " Chainage avant" ROUGE CYAN-CLAIR F/C
  TRUE WOUVRE
  >REGLES
  RECCOUNT 0
  DO
    I FREAD
    BUF-DIR #REGLE RSREGLE @ CMOVE
    >FAITS
    0
    BEGIN
      DUP RECCOUNT <
    WHILE
      DUP FREAD
      FT-FAIT -TRAILING #REGLE 200 SEARCH
      NIP
      IF
        ." J'ai d‚duit : "
        #REGLE 200 + 100 TYPE CR
        BUF-DIR RSFAIT @ BLANK
        #REGLE 200 + BUF-DIR 100 CMOVE
        RECCOUNT DROP FAPPEND
      THEN
      1+
    REPEAT
    DROP
    >REGLES
  LOOP
  MSGPAUSE
  WFERME
;

\ Gestion des menus.
80 STRING $MENUGEN
" Regles Faits ChainageAvant Fin" $MENUGEN $!

80 STRING $MENREGLES
" Ajoute Liste" $MENREGLES $!

80 STRING $MENFAITS
" Ajoute Liste" $MENFAITS $!

: .MENUGEN  ( --- )
  0 0 78 6 1 MAGENTA NOIR F/C
  CYAN ROUGE-CLAIR F/C DSEGMENT " Menu principal" BLEU CYAN-CLAIR F/C
  FALSE DSEGMENT $MENUGEN NOIR BLANC F/C MHORIZONTAL WMENU
;

: .MENREGLES  ( --- )
  OPTX @ 2 6 2 2 NOIR ROUGE-CLAIR F/C
  BLEU BLEU-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE DSEGMENT $MENREGLES
  BLEU ROUGE F/C MVERTICAL WMENU
;

: .MENFAITS  ( --- )
  OPTX @ 2 6 2 2 NOIR ROUGE-CLAIR F/C
  BLEU BLEU-CLAIR F/C DSEGMENT " " NOIR NOIR F/C TRUE DSEGMENT $MENFAITS
  BLEU ROUGE F/C MVERTICAL WMENU
;

\ Fin du programme.
: FIN  ( --- )
  CLOSE-BASES
  WALLCLOSE
;

\ Le programme principal.
: EXPERT  ( --- )
  WINIT
  .DESKTOP
  OPEN-REGLES
  OPEN-FAITS
  .MENUGEN
  BEGIN
    WGESMENU DUP
    CASE 1 OF
             .MENREGLES
             BEGIN
               WGESMENU DUP
               CASE 1 OF
                        AJOUTE-REGLE
                      ENDOF
                    2 OF
                        LISTE-REGLES
                      ENDOF
               ENDCASE
               0=
             UNTIL
             WFERME
           ENDOF
         2 OF
             .MENFAITS
             BEGIN
               WGESMENU DUP
               CASE 1 OF
                        AJOUTE-FAIT
                      ENDOF
                    2 OF
                        LISTE-FAITS
                      ENDOF
               ENDCASE
               0=
             UNTIL
             WFERME
           ENDOF
         3 OF
             CHAINAGE-AVANT
           ENDOF
    ENDCASE
    4 =
  UNTIL
  FIN
;

EOF

