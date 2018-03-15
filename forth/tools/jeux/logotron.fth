\ ------------------------------------------------------------------------------
\                                  Le Logotron
\                            Unit‚ de Lexico SynthŠse
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

EXIST? FIELD NOT ?\ INCLUDE DFILES
EXIST? RND NOT ?\ INCLUDE RND
EXIST? (EKEY) NOT ?\ INCLUDE EKEY

DECIMAL

\ Fichier contenant les pr‚fixes et leur signification
CREATE PREFIXES$ ," PREFIXES.DTA" 0 C,
\ Fichier contenant les suffixes et leur signification
CREATE SUFFIXES$ ," SUFFIXES.DTA" 0 C,

0  25 FIELD PREF/SUFF
25 50 FIELD SIGNIFIE

75 REC-SIZE !

25 STRING PREF
50 STRING SPREF
25 STRING SUFF
50 STRING SSUFF

\ Empile le nombre de pr‚fixes pr‚sents dans le fichier
: NPREF  ( --- n )
  PREFIXES$ 1+
  READ (OPEN) ?DOS-ERR
  HFILE !
  RECCOUNT
  FCLOSE
;

\ Empile le nombre de suffixes pr‚sents dans le fichier
: NSUFF  ( --- n )
  SUFFIXES$ 1+
  READ (OPEN) ?DOS-ERR
  HFILE !
  RECCOUNT
  FCLOSE
;

\ Lecture du niŠme pr‚fixe
: LECPREF  ( n --- )
  BUF-DIR REC-SIZE @ BLANK
  PREFIXES$ 1+
  READ (OPEN) ?DOS-ERR
  HFILE !
  FREAD
  PREF/SUFF -TRAILING PREF $!
  SIGNIFIE -TRAILING 1+ SPREF $!
  FCLOSE
;

\ Lecture du niŠme suffixe
: LECSUFF  ( n --- )
  BUF-DIR REC-SIZE @ BLANK
  SUFFIXES$ 1+
  READ (OPEN) ?DOS-ERR
  HFILE !
  FREAD
  PREF/SUFF -TRAILING SUFF $!
  SIGNIFIE -TRAILING 1+ SSUFF $!
  FCLOSE
;

\ Renvoi 1 si le caractŠre est une voyelle, 0 sinon
: VOYELLE?  ( c --- 0 | 1 )
  UPC
  DUP  ASCII A =
  OVER ASCII E = OR
  OVER ASCII I = OR
  OVER ASCII O = OR
  OVER ASCII U = OR
  SWAP ASCII Y = OR
  ABS
;

\ G‚nŠre un mot … partir d'un Pr‚fixe et d'un Suffixe.
\ EnlŠve la derniŠre voyelle du pr‚fixe si le premier caractŠre
\ du suffixe est aussi une voyelle.
: GENWORD
  PREF                       \ Adr Long ---
  2DUP + 1- C@               \ Adr Long Dernier_car ---
  VOYELLE?                   \ Adr Long fl ---
  SUFF DROP C@               \ Adr Long fl 1er_car ---
  VOYELLE? AND -             \ Adr Long-flag ---
  TYPE
  SUFF TYPE
;

\ Empile VRAI si la signification du pr‚fixe commence par "LE".
: LE?  ( --- flag )
  SPREF DROP
  DUP      C@ ASCII L =
  OVER 1+  C@ ASCII E = AND
  SWAP 2+  C@ 32 = AND
;

\ Empile VRAI si la signification du pr‚fixe commence par "LA".
: LA?  ( --- flag )
  SPREF DROP
  DUP      C@ ASCII L =
  OVER 1+  C@ ASCII A = AND
  SWAP 2+  C@ 32 = AND
;

\ Empile VRAI si la signification du pr‚fixe commence par "L'".
: L'?  ( --- flag )
  SPREF DROP
  DUP      C@ ASCII L =
  SWAP 1+  C@ ASCII ' = AND
;

\ Empile VRAI si la signification du pr‚fixe commence par "LES".
: LES?  ( --- flag )
  SPREF DROP
  DUP      C@ ASCII L =
  OVER 1+  C@ ASCII E = AND
  OVER 2+  C@ ASCII S = AND
  SWAP 3 + C@ 32 = AND
;

\ Empile VRAI si la signification du suffixe se termine par "DE".
: DE?  ( --- flag )
  SSUFF + 1-
  DUP  1-  C@ ASCII E =
  OVER 2-  C@ ASCII D = AND
  SWAP 3 - C@ 32 = AND
;

\ Empile VRAI si la signification du suffixe se termine par "A".
: A?  ( --- flag )
  SSUFF + 1-
  DUP  1- C@ ASCII A =
  SWAP 2- C@ 32 = AND
;

\ G‚nŠre une signification "lisible" du mot.
: SIGNIFICATION  ( --- )
  DE?
  IF
    LES?
    IF
      SSUFF 2- TYPE                    \ Remplace "DE LES" par "DES"
      SPREF 1- SWAP 1+ SWAP TYPE
    ELSE
      LE?
      IF                               \ Remplace "DE LE" par "DU"
        SSUFF 3 - TYPE ." DU "
        SPREF 3 - SWAP 3 + SWAP TYPE
      ELSE
        LA? L'? OR
        IF
          SSUFF TYPE
          SPREF TYPE
        ELSE
          SSUFF 3 - TYPE
          SPREF TYPE
        THEN
      THEN
    THEN
  ELSE
    A?
    IF
      LES?
      IF                               \ Remplace "A LES" par "AUX"
        SSUFF 2- TYPE ." AUX"
        SPREF 3 - SWAP 3 + SWAP TYPE
      ELSE
        LE?
        IF                             \ Remplace "A LE" par "AU"
          SSUFF 2- TYPE ." AU"
          SPREF 2- SWAP 2+ SWAP TYPE
        ELSE
          SSUFF TYPE
          SPREF TYPE
        THEN
      THEN
    ELSE
      SSUFF TYPE
      SPREF TYPE
    THEN
  THEN
;

\ Le programme de g‚n‚ration.
: LOGO
  RANDOMIZE
  NPREF 1+ RND LECPREF
  NSUFF 1+ RND LECSUFF
  10 15 AT 60 BL REPLICATE
  10 17 AT 60 BL REPLICATE
  10 19 AT 60 BL REPLICATE
  10 15 AT ." Mot g‚n‚r‚    : " GENWORD
  10 17 AT ." Racines       : "
  ." Pr‚fixe " PREF TYPE ."  Suffixe " SUFF TYPE
  10 19 AT ." Signification : " SIGNIFICATION
;

\ Le programme principal.
: LOGOTRON
  DARK
  25 2 AT ." LOGO Software Inc. Present :"
  33 4 AT ." THE_LOGOTRON"
  30 6 AT ." Lexico-synthetizer"
  10 9 AT ." Appuyez sur une touche ou sur Esc pour stopper la g‚n‚ration"
  BEGIN
    LOGO 0
    BEGIN
      DROP KEY DUP
    UNTIL
    #ESC =
  UNTIL
;

\ Ajoute un pr‚fixe et sa signification … la fin du fichier des prefixes
: AJOUTE-PREF
  PREFIXES$ 1+
  READ-WRITE (OPEN) ?DOS-ERR
  DUP HFILE !
  0 0 ROT 2 (SEEK) ?DOS-ERR 2DROP
  BUF-DIR REC-SIZE @ BLANK
  CR ." Pr‚fixe       : " PREF/SUFF EXPECT
  CR ." Signification : " SIGNIFIE EXPECT
  FAPPEND
  FCLOSE
;

\ Ajoute un suffixe et sa signification … la fin du fichier des suffixes
: AJOUTE-SUFF
  SUFFIXES$ 1+
  READ-WRITE (OPEN) ?DOS-ERR
  DUP HFILE !
  0 0 ROT 2 (SEEK) ?DOS-ERR 2DROP
  BUF-DIR REC-SIZE @ BLANK
  CR ." Suffixe       : " PREF/SUFF EXPECT
  CR ." Signification : " SIGNIFIE EXPECT
  FAPPEND
  FCLOSE
;

CR .( Taper LOGOTRON pour lancer le programme )

EOF

