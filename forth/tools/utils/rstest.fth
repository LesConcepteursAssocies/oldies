\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Test du port RS232.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? (EKEY) NOT ?\ INCLUDE EKEY
EXIST? WOUVRE NOT ?\ INCLUDE FEN
EXIST? RSINPUT NOT ?\ INCLUDE RS232

DECIMAL
2VARIABLE RSCPT              \ Compte le nombre de caractŠres re‡us.

: .TITRE  ( --- )
  0 0 78 1 1
  NOIR GRIS-CLAIR F/C
  NOIR BLANC F/C DSEGMENT " Test de la RS-232" NOIR BLANC F/C FALSE WOUVRE
  26 1 AT ." Appuyez sur Esc pour finir"
;

: RSTEST  ( --- )
  WINIT
  EKEY-ON
  COM1-ON
  .TITRE
  0 3 78 17 2 GRIS-CLAIR NOIR F/C
  CYAN BLEU-CLAIR F/C DSEGMENT " Buffer" ROUGE CYAN F/C FALSE WOUVRE
  0 22 78 1 3 NOIR GRIS-CLAIR F/C
  NOIR BLANC F/C DSEGMENT " " NOIR NOIR F/C FALSE WOUVRE
  ." Nombre de caractŠres re‡us : "
  BEGIN
    0
    RSKEY?
    IF
      2 WTABLE>              \ Fenˆtre du buffer.
      WLOCATE                \ Positionnement du curseur.
      RSKEY H.
      2 >WTABLE              \ Pour sauvegarder la pos. du curseur.
      RSCPT 2@ 1. D+
      RSCPT 2!               \ Un caractŠre de plus.
      3 WTABLE>              \ Fenˆtre du compteur.
      30 1 AT
      RSCPT 2@ UD.
    THEN
    KEY?
    IF
      DROP KEY
    THEN
    #ESC =
  UNTIL
  COM1-OFF
  WALLCLOSE
  EKEY-OFF
;

EOF

