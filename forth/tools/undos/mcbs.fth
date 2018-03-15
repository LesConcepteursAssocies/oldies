\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ Information sur le contenu de la m‚moire programme (T.P.A).
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? CREER-2MATRICE NOT ?\ INCLUDE MATRICES
EXIST? OSVER NOT ?\ INCLUDE MSDOS
EXIST? DIB NOT ?\ INCLUDE DIB

DECIMAL

1 256 CREER-2MATRICE VECTEURS

\ Initialisation du tableau des vecteurs d'interruptions.
: INIT-VECT  ( --- )
  256 0
  DO
    I INT@ 0 I VECTEURS 2!
  LOOP
;

\ Affiche un nombre compl‚t‚ avec des 0 devant.
: 0.R  ( n nb --- )
  >R (U.) R>
  OVER - ASCII 0 REPLICATE
  TYPE
;

\ Empile l'adresse du premier MCB.
: 1ER-MCB  ( --- seg off )
  (DIB) 2- L@ 0
;

\ Empile l'indentificateur du MCB courant.
: MCB-ID  ( seg off --- seg off c )
  2DUP LC@
;

\ Empile l'adresse de segment du propri‚taire du MCB courant.
: MCB-PRO  ( seg off --- seg off seg_pro )
  2DUP 1+ L@
;

\ Empile l'adresse de la zone m‚moire du MCB courant.
: MCB-MEM  ( seg off --- seg off seg_mem off_mem )
  2DUP DROP 1+ 0
;

\ Empile la taille du bloc m‚moire (en paragraphes) du MCB courant.
: MCB-TAILLE  ( seg off --- seg off taille )
  2DUP 3 + L@
;

\ Empile l'adresse du MCB suivant.
: MCB-SUIVANT  ( seg off --- seg' off' )
  MCB-TAILLE 1+ NIP + 0
;

\ Empile VRAI si le MCB courant est un PSP.
: MCB-PSP?  ( seg off --- seg off fl )
  MCB-PRO 2 PICK 1+ =
;

\ Affichage de la ligne de commande du MCB courant.
HEX
: .MCB-LCMD  ( seg off --- seg off )
  MCB-PRO 81 2DUP 1- LC@ LTYPE
;
DECIMAL

\ Empile VRAI si l'adresse (seg:off) donn‚e pointe dans la zone dep + tail.
: POINTE?  ( seg off dep tail --- fl )
  2SWAP 16 / + DUP           \ dep tail AdrAbsolue AdrAbsolue ---
  3 PICK U>= >R
  -ROT + U<= R> AND
;

\ Affichage des vecteurs redirig‚s.
: MCB-.VECT  ( seg off --- seg off )
  CR 2 SPACES ." Vecteurs [ "
  INIT-VECT
  BASE @ >R HEX
  256 0
  DO
    0 I VECTEURS 2@ D0= NOT
    IF
      0 I VECTEURS 2@
      2OVER MCB-TAILLE NIP
      POINTE?
      IF
        I 2 0.R SPACE
        0. 0 I VECTEURS 2!
      THEN
    THEN
  LOOP
  R> BASE ! ." ]" CR
;

\ Affichage des informations sur le MCB.
: MCB-.INFO  ( seg off --- seg off )
  BASE @ >R
  (CURSOR) NIP 0 SWAP AT
  HEX OVER 4 0.R DECIMAL
  (CURSOR) NIP 5 SWAP AT
  MCB-TAILLE DUP 5 U.R 2 SPACES ." (" 16 UM* 6 UD.R ." )"
  (CURSOR) NIP 22 SWAP AT
  HEX MCB-PRO 4 0.R DECIMAL
  MCB-PRO 0=
  IF
    ."  Libre"
  ELSE
    MCB-PRO 8 =
    IF
      ."  Config"
    THEN
  THEN
  MCB-PSP?
  IF
    (CURSOR) NIP 36 SWAP AT
    .MCB-LCMD
  THEN
  MCB-.VECT
  R> BASE !
;

\ Trace la chaine des MCB.
: MCB-TRACE  ( seg off --- )
  CR ." Seg  parag. (Octets)  Propri‚taire  Ligne de commande" CR
  BEGIN
    MCB-ID
    CASE ASCII M OF
                   MCB-.INFO
                   MCB-SUIVANT
                 ENDOF
         ASCII Z OF
                   MCB-.INFO
                   2DROP EXIT
                 ENDOF
         DROP 2DROP EXIT
    ENDCASE
  AGAIN
;

\ Le programme principal.
\ On peut empiler l'adresse d'un segment ou d‚buter le tracage.
: .MEM  ( [seg] --- )
  CR ." Contenu de la m‚moire :"
  DEPTH 0>
  IF
    0
  ELSE
    1ER-MCB
  THEN
  MCB-TRACE
;

EOF

