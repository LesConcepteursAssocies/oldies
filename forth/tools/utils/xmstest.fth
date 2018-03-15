\ -----------------------------------------------------------------------------
\ Test de la m‚moire XMS.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
EXIST? XMS? NOT ?\ INCLUDE XMS_LIB

HEX
B800 CONSTANT VIDEORAM       \ Pour le test.
DECIMAL
VARIABLE SEGUMB              \ Contiendra l'adresse de segment de l'U.M.B allou‚.

: XMSTEST  ( --- )
  XMS?
  IF
    DARK
    XMS-ADR!                 \ On r‚cupŠre l'adresse du point d'entr‚e
    ." Informations sur la m‚moire ‚tendue (X.M.S.)"
    CR ." Num‚ro de version       : "
    XMS-@VERSION . 1 BACKSPACES ASCII . EMIT .
    CR ." R‚vision Nø             : "
    . 1 BACKSPACES ASCII . EMIT .
    CR ." H.M.A.                  : "
    0=
    IF
      ." absent"
    ELSE
      ." pr‚sent"
    THEN
    CR ." Ligne A20               : "
    XMS-STATUS-A20
    0=
    IF
      ." invalid‚e"
    ELSE
      ." valid‚e"
    THEN
    CR ." Plus grand E.M.B. dispo : "
    XMS-#EMB U. ."  Ko.  Sur " U. CR
    ." Appuyez sur une touche"
    BEGIN
      KEY
    UNTIL
    CR CR ." allocation d'un U.M.B de 4 Ko"
    4096 16 / 1+ XMS-ALLOC-UMB DROP SEGUMB !
    VIDEORAM 0 SEGUMB @ 0 4000 LCMOVE
    DARK
    ." Le contenu pr‚c‚dent de l'‚cran a ‚t‚ copi‚" CR
    ." dans le bloc allou‚." CR CR
    ." Appuyez sur une touche pour le restituer" CR
    BEGIN
      KEY
    UNTIL
    SEGUMB @ 0 VIDEORAM 0 4000 LCMOVE
    SEGUMB @ XMS-FREE-UMB
  ELSE
    CR ." D‚sol‚, vous n'avez pas de m‚moire ‚tendue !"
  THEN
;

EOF


