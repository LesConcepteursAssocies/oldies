\ ------------------------------------------------------------------------------
\                        D‚monstrations des routines graphiques
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

DECIMAL

EXIST? PSET-V  NOT ?\ INCLUDE GRAPHICS

\ D‚monstration des diff‚rents modes.
\ Chaines ascii0 des fichiers de d‚mo.
80 STRING0 $FILE-IMG    \ fichier image.
80 STRING0 $FILE-ICP    \ fichier image compact‚e.

: DEMO1  ( --- )
  DARK
  ECRAN->*ECRAN
  ." Generation de points aleatoire"
  CR ." Appuyez sur une touche"
  CR ." pour afficher l'ecran"
  BEGIN
    1000 RND DROP
    FALSE
    KEY? NOT
    IF
      320 RND 200 RND COULEURS-MAX 1+ RND PSET-V
    ELSE
      DROP KEY
    THEN
  UNTIL
  *ECRAN->ECRAN
  CR ." L'ecran va etre copie sur disque"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG ECRAN->DISQUE
;

: DEMO2  ( --- )
  DARK
  ECRAN->*ECRAN
  ." Lecture d'un fichier image"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG DISQUE->*ECRAN
  *ECRAN->ECRAN
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
;

: DEMO3  ( --- )
  DARK
  ." Demonstration de @IMAGE et !IMAGE"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG DISQUE->*ECRAN
  *ECRAN->ECRAN
\   CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  (*CGA-SAUVE) 0 16384 0 LFILL
  GRMODE @
  CASE  5 OF
            (CGA-ECRAN) (*CGA-SAUVE) 0 0 40 100
          ENDOF
       13 OF
            (EGA-ECRAN) (*EGA-SAUVE) 12 72 22 172
          ENDOF
       19 OF
            (MCGA-ECRAN) (*MCGA-SAUVE) 92 72 172 172
          ENDOF
  ENDCASE
  @IMAGE
\   DARK
  (*CGA-SAUVE) (CGA-ECRAN) 100 0 40 100 !IMAGE
  ATTEND-TOUCHE
  DARK
  (*CGA-SAUVE) (CGA-ECRAN) 100 0 40 100 !IMAGE
\   (*CGA-SAUVE) (CGA-ECRAN) 0 100 40 100 !IMAGE
\   2 0
\   DO
\     4 0
\     DO
\       GRMODE @
\       CASE  5 OF
\                 (*CGA-SAUVE) (CGA-ECRAN) I 40 * J 100 * 40 100
\               ENDOF
\            13 OF
\                 (*EGA-SAUVE) (EGA-ECRAN) I 10 * J 100 * 10 100
\               ENDOF
\            19 OF
\                 (*MCGA-SAUVE) (MCGA-ECRAN) I 80 * J 100 * 80 100
\               ENDOF
\       ENDCASE
\       !IMAGE
\     LOOP
\   LOOP
\   CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
;

: DEMO4  ( --- )
  DARK
  ." Demonstration de SCROLLING"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG DISQUE->*ECRAN
  *ECRAN->ECRAN
  BEGIN
    FALSE
    KEY? NOT
    IF
      TRUE  HSCROLL
      FALSE HSCROLL
    ELSE
      DROP KEY
    THEN
  UNTIL
  CR ." Marrant n'est-ce pas ?"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
;

: DEMO5  ( --- )
  DARK
  ." Demonstration de COMPACTAGE"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG DISQUE->*ECRAN
  *ECRAN->ECRAN
  $FILE-ICP COMPACTE
;

: DEMO6  ( --- )
  DARK
  ." Decompactage en cours..."
  $FILE-ICP DCOMPACTE
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  *ECRAN->ECRAN
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
;

: DEMO7  ( --- )
  DARK
  ." Cyclage des composantes RVB"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  $FILE-IMG DISQUE->*ECRAN
  *ECRAN->ECRAN
  CR ." Appuyez sur une touche pour arreter"
  BEGIN
    1000 RND DROP
    FALSE
    KEY? NOT
    IF
      COULEURS-MAX 1+ RND
      COULEURS-MAX 1+ RND
      COULEURS-MAX 1+ RND
      COULEURS-MAX 1+ RND
      RVB!
    ELSE
      DROP KEY
    THEN
  UNTIL
;

: DEMO-CGA  ( --- )
  CGA DARK
  ." Demonstration du mode CGA 320 x 200"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
\   " \L\TF\PG\TOOLS\GRAPHISM\CGA\ESSAI.CGA" $FILE-IMG $!
\   DEMO1
\   DEMO2
  " \L\TF\PG\TOOLS\GRAPHISM\CGA\IMAGE.CGA" $FILE-IMG $!
  DEMO3
;

: DEMO-EGA  ( --- )
  EGA DARK
  ." Demonstration du mode EGA 320 x 200"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  " \L\TF\PG\TOOLS\GRAPHISM\EGA\ESSAI.EGA" $FILE-IMG $!
  " \L\TF\PG\TOOLS\GRAPHISM\EGA\IMAGE.EGC" $FILE-ICP $!
  DEMO1
  DEMO2
  " \L\TF\PG\TOOLS\GRAPHISM\EGA\IMAGE.EGA" $FILE-IMG $!
  DEMO3
  DEMO4
  DEMO5
  DEMO6
;

: DEMO-MCGA  ( --- )
  MCGA DARK
  ." Demonstration du mode MCGA 320 x 200"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  " \L\TF\PG\TOOLS\GRAPHISM\MCGA\ESSAI.MCG" $FILE-IMG $!
  " \L\TF\PG\TOOLS\GRAPHISM\MCGA\IMAGE.MCC" $FILE-ICP $!
  DEMO1
  DEMO2
  " \L\TF\PG\TOOLS\GRAPHISM\MCGA\IMAGE.MCG" $FILE-IMG $!
  DEMO3
  DEMO4
  DEMO5
  DEMO6
  DEMO7
;

: DEMO  ( --- )
  DARK
  ." D‚monstration des diff‚rents modes graphiques"
  CR ." Appuyez sur une touche pour commencer"
  ATTEND-TOUCHE
  DEMO-CGA
  DEMO-EGA
  DEMO-MCGA
  DARK ." C'est fini !"
  CR ." Appuyez sur une touche"
  ATTEND-TOUCHE
  3 MODE
;

CR .( Tapez DEMO pour une d‚monstration )

EOF

