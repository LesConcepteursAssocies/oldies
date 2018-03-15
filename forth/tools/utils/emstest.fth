\ -----------------------------------------------------------------------------
\ Test de la m‚moire EMS.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIT? EMS-INST NOT ?\ INCLUDE EMS_LIB

HEX
B800 CONSTANT VIDEORAM       \ Pour le test.
DECIMAL
VARIABLE HEMS                \ Contiendra le handler sur la m‚moire allou‚e.

: EMSTEST  ( --- )
  EMS-INST                   \ V‚rification de la pr‚sence d'un EMM.
  EMS?
  IF
    DARK
    EMM-GETFRAME             \ On r‚cupŠre l'adresse du cadre de page
    ." Informations sur la m‚moire ‚tendue"
    CR ." Num‚ro de version       : "
    EMM-@VERSION . 1 BACKSPACES ASCII . EMIT .
    CR ." Adresse de segment      : "
    #EMS-FRAME H.
    CR ." Nombre de pages totales : "
    EMM-NUMPAGES DUP . ." (" 16 * U. ." Ko)"
    CR ." Nombre de pages libres  : "
    EMM-FREEPAGES DUP . ." (" 16 * U. ." Ko)"
    EMM-FREEPAGES 0<>
    IF
      CR CR CR ." allocation d'une page de 16 Ko"
      1 EMM-ALLOC HEMS !
      0 0 HEMS @ EMM-SETMAPPING
      CR ." Appuyez sur une touche"
      BEGIN
        KEY
      UNTIL
      VIDEORAM 0 #EMS-FRAME 0 4000 LCMOVE
      DARK
      ." Le contenu pr‚c‚dent de l'‚cran a ‚t‚ copi‚" CR
      ." dans la page allou‚e." CR CR
      ." Appuyez sur une touche pour le restituer" CR
      BEGIN
        KEY
      UNTIL
      #EMS-FRAME 0 VIDEORAM 0 4000 LCMOVE
      HEMS @ EMM-FREE
    ELSE
      CR ." Pas de page disponible !"
    THEN
  ELSE
    CR ." D‚sol‚, vous n'avez pas de m‚moire ‚tendue !"
  THEN
;

EOF

