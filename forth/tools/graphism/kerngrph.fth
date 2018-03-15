\ ------------------------------------------------------------------------------
\              Quelques outils n‚c‚ssaires aux fonctions graphiques.
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

EXIST? MALLOC NOT ?\ INCLUDE MEMORY
EXIST? RND NOT ?\ INCLUDE RND

DECIMAL

\ D‚cale n1 de n2 bits vers la gauche.
CODE <<  ( n1 n2 --- n3 )
  CX POP
  AX POP
  AX CL SHL
  1PUSH
END-CODE

\ Division entiŠre.
CODE IDIV  ( n1 n2 --- n1/n2 )
  BX POP
  AX POP
  CWD
  BX IDIV
  1PUSH
END-CODE

\ Renvoi n modulo 2.
CODE 2MOD  ( n --- n MODulo 2 )
  AX POP
  2 # BX MOV
  BL IDIV
  AH AL XCHG
  AH AH XOR
  1PUSH
END-CODE

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

\ Echange les deux couples de coordonn‚es si x1 > x2.
: SWAP-COORD  ( x1 y1 x2 y2 --- x1' y1' x2' y2' )
  3 PICK 2 PICK              \ x1 y1 x2 y2 x1 x2 ---
  >
  IF
    2SWAP
  THEN
;

\ Tableau des puissances de 2
CREATE 2^
128 , 64 , 32 , 16 , 8 , 4 , 2 , 1 ,

\ Calcule la taille (en points) d'une portion d'‚cran.
\ Suppose x1 < x2.
: IMAGE-SIZE  ( x1 y1 x2 y2 --- x1 y1 x2 y2 taille )
  4DUP SWAP 3 ROLL           \ x1 y1 x2 y2 y1 y2 x2 x1 ---
  -  1+ -ROT SWAP - 1+ *     \ x1 y1 x2 y2 (x2-x1+1)*(y2-y1+1) ---
;

\ Renvoie la taille d'un fichier.
: (F-SIZE@)  ( adr_fic l --- d adr_fic l )
  2DUP DROP 0 (OPEN) ?DOS-ERR >R
  0 0 R@ 2 (SEEK) ?DOS-ERR
  R> (CLOSE) 2SWAP
;

\ Attend l'appui sur une touche
: ATTEND-TOUCHE  ( --- )
  BEGIN
    KEY
  UNTIL
;

EOF

