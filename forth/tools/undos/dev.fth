\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ Information sur les p‚riph‚riques pr‚sents en m‚moire.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? OSVER NOT ?\ INCLUDE MSDOS
EXIST? DIB NOT ?\ INCLUDE DIB
EXIST? L2@ NOT ?\ INCLUDE 2MEMSEG

DECIMAL

\ Empile l'adresse du premier driver (NUL).
\ 0000:0000 si la version du D.O.S ne le permet pas (osmajor < 2 ou > 4).
: 1ER-DEV  ( --- seg off )
  (DIB)
  OSMAJOR
  CASE 2 OF
           23 +
         ENDOF
       3 OF
           OSMINOR
           0>
           IF
             34 +                 \ DOS 3.1 - 3.3
           ELSE
             40 +                 \ DOS 3.0
           THEN
         ENDOF
       4 OF
           34 +                   \ DOS 4.x
         ENDOF
         2DROP 0.                 \ DOS < 2 ou > 4
  ENDCASE
;

\ Affiche le nom du driver (de caractŠres).
: DEV-.NOM  ( seg off --- seg off )
  2DUP 10 + 8 LTYPE
;

\ Empile le nombre d'unit‚s support‚es par le driver (de blocs).
: DEV-NBUNIT  ( seg off --- seg off nbunit )
  2DUP 10 + LC@
;

\ Empile le mot d'attributs du driver courant.
: DEV-ATTR  ( seg off --- seg off attr )
  2DUP 4 + L@
;

\ Empile VRAI si le driver pr‚sent est un driver de caractŠres.
: DEV-CHAR?  ( seg off --- seg off fl )
  DEV-ATTR
  32768 AND 32768 =
;

\ Empile l'adresse (seg:off) de l'entˆte du driver suivant.
: DEV-SUIVANT  ( seg off --- seg' off' )
  L2@
;

\ Empile VRAI si le driver courant est le dernier.
: DEV-FIN?  ( seg off --- seg off fl )
  DEV-SUIVANT DUP -1 =
;

\ Le programme principal.
: .DEV  ( --- )
  CR ." Liste des p‚riph‚riques pr‚sents :" CR
  1ER-DEV 2DUP D0= ABORT" Version du DOS non valide"
  BEGIN
    DEV-CHAR?
    IF
      DEV-.NOM
      10 SPACES
    ELSE
      ." Blocs : " DEV-NBUNIT . ."  Unit‚s"
      SPACE
    THEN
    DEV-ATTR ." (Attribut="
    DUP HEX U. DECIMAL 1 BACKSPACES ." h)"
    16384 AND 16384 =
    IF
      2 SPACES
      ." (Fonctions IOCTL support‚es)"
    THEN
    CR
    DEV-FIN?
  UNTIL
  2DROP
;

EOF

