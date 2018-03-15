\ -----------------------------------------------------------------------------
\                 D‚finisseur de R‚seaux de Neurones Dynamiques.
\
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF
EXIST? CREER-NOEUD NOT ?\ INCLUDE LIST

DECIMAL

12 CONSTANT #NEURONE         \ Taille d'un neurone (en octets).

\ Cr‚ation d'une couche de n neurones.
\ La couche est initialis‚e … NIL.
: CREER-NEURONES  ( n --- )
  CREATE
    DUP ,                    \ On conserve le nombre de neurones.
    HERE SWAP
    #NEURONE * DUP ALLOT     \ here n*#neurone ---
    NIL FILL
;

\ Empile l'adresse de d‚but du niŠme neurone de la couche donn‚e.
\ (correspond au champ EI du neurone).
: NEURONE#  ( niŠme adrcouche --- N.EI )
  2DUP @ 1-
  U> ABORT" NEURONE#: Indice hors limites"
  2+ SWAP #NEURONE * +
;

\ --------------------- Manipulation des champs d'un neurone ------------------
\ N.EI est l'adresse de d‚but du neurone donn‚.
\ Renvoi l'adresse de WIJ.
: N.WIJ  ( N.EI --- N.WIJ )
  2+
;

\ Renvoi l'adresse de H.
: N.H  ( N.EI --- N.H )
  4 +
;

\ Renvoi l'adresse de E.
: N.E  ( N.EI --- N.E )
  6 +
;

\ Renvoi l'adresse de A.
: N.A  ( N.EI --- N.A )
  8 +
;

\ Renvoi l'adresse de S.
: N.S  ( N.EI --- N.S )
  10 +
;

\ Renvoi l'adresse du vecteur EI.
: [N.EI]  ( N.EI --- EI )
  @
;

\ Renvoi l'adresse du vecteur WIJ.
: [N.WIJ]  ( N.EI --- WIJ )
  N.WIJ @
;

\ Renvoi l'adresse de la fonction d'entr‚e H.
: [N.H]  ( N.EI --- H )
  N.H @
;

\ Renvoi l'adresse de la fonction d'activation A.
: [N.A]  ( N.EI --- A )
  N.A @
;

\ ------------------------- Manipulation des connexions -----------------------
\ Ajoute un noeud … la fin de la liste donn‚e.
\ Renvoi l'adresse du champ val du noeud cr‚‚.
: AJOUTE-NOEUD  ( adrliste --- adrNoeud.val )
  BEGIN                      \ On se place sur le dernier noeud.
    DUP @
  WHILE
    @
  REPEAT                     \ adrDernierNoeud ---
  CREER-NOEUD DUP -ROT       \ adrNouveauNoeud adrDernierNoeud adrNouveauNoeud ---
  INSERT-NOEUD 2+            \ adrNouveauNoeud.val ---
;

\ Connection de deux neurones.
\ Connecte le neurone N1 au neurone N2.
\ Affecte … la connection le poids W.
: CONNECTE  ( W N1 N2 --- )
  DUP AJOUTE-NOEUD           \ On ajoute un noeud au vecteur des entr‚es de N2.
  ROT N.S SWAP !             \ On prend N1.S que l'on stocke dans le noeud de N2.
  N.WIJ AJOUTE-NOEUD !       \ On ajoute un noeud au vecteur des poids de N2, dans lequel on stocke W.
;

\ Connecte l'ensemble des neurones de la couche adrcouche au neurone N.
\ Affecte … chaque connection le poids W.
: NCONNECTE  ( W adrcouche N --- )
  OVER @ 0
  DO                         \ W adrcouche N ---
    2 PICK I 3 PICK          \ W adrcouche N W i adrcouche ---
    NEURONE# 2 PICK          \ W adrcouche N W Ni.EI N ---
    CONNECTE
  LOOP                       \ W adrcouche N ---
  2DROP DROP
;

\ Connecte chaque neurone de la couche adrcouche2 … l'ensemble des neurones
\ de la couche adrcouche1.
\ Affecte … chaque connection le poids W.
: CCONNECTE  ( W adrcouche1 adrcouche2 --- )
  DUP @ 0
  DO
    2 PICK 2 PICK I 3 PICK   \ W adrcouche1 adrcouche2 W adrcouche1 i adrcouche2 ---
    NEURONE#                 \ W adrcouche1 adrcouche2 W adrcouche1 Ni.EI ---
    NCONNECTE
  LOOP                       \ W adrcouche1 adrcouche2 ---
  2DROP DROP
;

\ ------------------- Manipulation des entr‚es et des sorties -----------------
\ Affecte … l'ensemble des neurones de la couche donn‚e, la fonction d'entr‚e
\ dont le CFA est sur la pile.
: IS-H  ( cfa adrcouche --- )
  DUP @ 0
  DO
    OVER I 2 PICK NEURONE#   \ cfa adrcouche cfa Ni.EI ---
    N.H !
  LOOP
  2DROP
;

\ Affecte … l'ensemble des neurones de la couche donn‚e, la fonction
\ d'activation dont le CFA est sur la pile.
: IS-A  ( cfa adrcouche --- )
  DUP @ 0
  DO
    OVER I 2 PICK NEURONE#   \ cfa adrcouche cfa Ni.EI ---
    N.A !
  LOOP
  2DROP
;

\ Ex‚cute la fonction d'entr‚e des neurones de la couche donn‚e.
: H-EXECUTE  ( adrcouche --- )
  DUP @ 0
  DO
    I OVER NEURONE#          \ adrcouche Ni.EI ---
    DUP [N.H] EXECUTE        \ On passe comme paramŠtre … la fonction d'entr‚e
  LOOP                       \ l'adresse du neurone courant.
  DROP
;

\ Ex‚cute la fonction d'activation des neurones de la couche donn‚e.
: A-EXECUTE  ( adrcouche --- )
  DUP @ 0
  DO
    I OVER NEURONE#          \ adrcouche Ni.EI ---
    DUP [N.A] EXECUTE        \ On passe comme paramŠtre … la fonction d'activation
  LOOP                       \ l'adresse du neurone courant.
  DROP
;

EOF

