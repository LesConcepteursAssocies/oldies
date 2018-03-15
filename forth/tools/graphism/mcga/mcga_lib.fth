\ ------------------------------------------------------------------------------
\     Kernel de Manipulation du mode graphique 19 (MCGA 320x200 256 Couleurs)
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

WARNING OFF

\ Chargement des outils.
EXIST? << NOT ?\ INCLUDE KERNGRPH

HEX
0A000 CONSTANT (MCGA-ECRAN)
DECIMAL

\ Pointeurs sur ‚crans virtuels
\ A initialiser avant toutes manipulations
\ ****************************************
0 CONSTANT (*MCGA-ECRAN)
0 CONSTANT (*MCGA-SAUVE)               \ Ecran de sauvegarde.

\ Initialisation des pointeurs
: (MCGA-*INIT)  ( --- )
  LIB-COM
  65535 MALLOC IS (*MCGA-ECRAN)
  65535 MALLOC IS (*MCGA-SAUVE)
;

\ Sauvegarde de l'‚cran dans l'‚cran virtuel
: (MCGA-ECR->*ECR)  ( --- )
  (MCGA-ECRAN) 0 (*MCGA-ECRAN) 0 64000 LCMOVE
;

\ Restitution de l'‚cran
: (MCGA-*ECR->ECR)  ( --- )
  (*MCGA-ECRAN) 0 (MCGA-ECRAN) 0 64000 LCMOVE
;

\ Ecriture d'un point de couleur coul aux coordonn‚es x,y
\ dans l'‚cran virtuel
\ coul = [0..255]
: (MCGA-PSET)  ( x y coul --- )
  -ROT 320 * +               \ coul y*320+x ---
  (*MCGA-ECRAN) SWAP LC!
;

\ Lecture d'un fichier graphique et stockage dans l'‚cran virtuel
: (MCGA-DSQ->*ECR)  ( adr_fic l --- )
  ?OPEN
  DROP 0 (OPEN) ?DOS-ERR >R
  (*MCGA-ECRAN) 0 64000 R@ (GET) ?DOS-ERR DROP
  R> (CLOSE)
;

\ Sauvegarde de l'‚cran sur disque
: (MCGA-ECR->DSQ)  ( adr_fic l --- )
  DROP 0 (CREATE) ?DOS-ERR >R
  (MCGA-ECRAN) 0 64000 R@ (PUT) ?DOS-ERR DROP
  R> (CLOSE)
;

\ Sauvegarde d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x1,y1   = coordonn‚es du coin sup‚rieur gauche de l'image.
\ x2,y2   = coordonn‚es du coin inf‚rieur droit de l'image.
: (MCGA-@IMAGE)  ( seg_src seg_dst x1 y1 x2 y2 --- )
  SWAP-COORD
  DUP 3 PICK - 1+ 0          \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  DO                         \ seg_src seg_dst x1 y1 x2 y2 ---
    2 PICK I + 320 *         \ seg_src seg_dst x1 y1 x2 y2 y1+I*320 ---
    4 PICK +                 \ seg_src seg_dst x1 y1 x2 y2 y1+I*320+x1 ---
    6 PICK SWAP              \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*320+x1 ---
    6 PICK I 320 *           \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*320+x1 seg_dst I*320 ---
    5 PICK 8 PICK - 1+       \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*320+x1 seg_dst I*320 x2-x1+1 ---
    LCMOVE
  LOOP                       \ seg_src seg_dst x1 y1 x2 y2 ---
  2DROP 2DROP 2DROP
;

\ Restitution d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x,y     = coordonn‚e ou placer l'image (coin sup‚rieur gauche).
\ largeur = largeur de l'image en points.
\ hauteur = hauteur de l'image en points.
: (MCGA-!IMAGE)  ( seg_src seg_dst x y largeur hauteur --- )
  0
  DO                         \ seg_src seg_dst x y largeur ---
    4 PICK I 320 *           \ seg_src seg_dst x y largeur seg_src I*320 ---
    5 PICK 4 PICK I +        \ seg_src seg_dst x y largeur seg_src I*320 seg_dst y+I ---
    320 * 6 PICK +           \ seg_src seg_dst x y largeur seg_src I*320 seg_dst y+I*320+x ---
    4 PICK LCMOVE
  LOOP                       \ seg_src seg_dst x y largeur ---
  2DROP DROP 2DROP
;

\ Scrolling horizontal de l'‚cran.
\ flag est VRAI pour aller … gauche, FAUX pour aller … droite.
HEX
: (MCGA-HSCROLL)  ( flag --- )
  0D 03D4 PC!      \ indique … la carte que l'on veut modifier l'adresse
                   \ de poids faible du buffer d'‚cran.
  IF
    4F 1
    DO
      I 03D5 PC!   \ on indique ensuite de combien de 1/80Šme d'‚cran
                   \ on veut d‚caler l'adresse du buffer.
      14 MS        \ temporisation (trop rapide sans cela...)
    LOOP
  ELSE
    4F 1
    DO
      4F I - 03D5 PC!
      14 MS
    LOOP
  THEN
  0 03D5 PC!
;

\ Modifie la composante R V B d'une couleur.
CODE RVB!  ( r v b coul --- )
  BX POP
  CX POP
  AX POP
  AL CH MOV
  DX POP
  DH DL XCHG
  1010 # AX MOV
  10 INT
  NEXT
END-CODE

\ Lit la composante R V B d'une couleur.
CODE RVB@  ( coul --- r v b )
  BX POP
  1015 # AX MOV
  10 INT
  DH DL XCHG
  DH DH XOR
  DX PUSH
  CX AX XCHG
  AH DL MOV
  AH AH XOR
  2PUSH
END-CODE

DECIMAL

\ Compactage d'images MCGA 320 x 200.
\ Compacte l'image pr‚sente … l'‚cran dans (*MCGA-SAUVE).
\ Renvoie le nombre d'octets de l'image compact‚e ou 0 si
\ le compactage ne rapporte rien.
\ Principe : on balaye l'image, et pour chaque point, on compte
\            le nombre de points identiques, que l'on stocke
\            dans la zone de compactage, avec la couleur du point
\            correspondante.
\
\ Le gain sera bien sur fonction de la complexit‚ du dessin.
\
: ((MCGA-CPT))  ( --- l )
  0 0 0 0
  BEGIN                      \ offC oct nbo offE ---
    DUP 0 U>
    IF
      1+
    THEN
    ROT DROP
    (MCGA-ECRAN) OVER LC@    \ offC nbo offE oct ---
    ROT DROP 1 -ROT          \ offC 1 offE oct ---
    OVER 63998 U<
    IF
      BEGIN                  \ offC nbo offE oct ---
        OVER 1+ (MCGA-ECRAN)
        SWAP LC@
        2 PICK (MCGA-ECRAN)
        SWAP LC@ =
        2 PICK 63998 U< AND
        IF
          ROT 1+ -ROT        \ offC nbo+1 offE oct ---
          SWAP 1+ SWAP       \ offC nbo offE+1 oct ---
        THEN
        OVER 63998 =
        2 PICK DUP 1+
        (MCGA-ECRAN) SWAP
        LC@ (MCGA-ECRAN) ROT
        LC@ <> OR 3 PICK 255
        = OR
      UNTIL                  \ offC nbo offE oct ---
    THEN
    3 PICK 63999 U>
    IF
      2DROP 2DROP
      0
      EXIT
    THEN
    3 PICK (*MCGA-SAUVE)
    SWAP 4 PICK -ROT LC!
    2SWAP SWAP 1+ SWAP 2SWAP \ offC+1 nbo offE oct ---
    DUP 4 PICK (*MCGA-SAUVE)
    SWAP LC!
    2SWAP SWAP 1+ SWAP 2SWAP \ offC+1 nbo offE oct ---
    -ROT DUP 63998 =
  UNTIL                      \ offC oct nbo offE
  DROP 2DROP (*MCGA-SAUVE)
  OVER (MCGA-ECRAN) 63999 LC@
  -ROT LC!                   \ offC ---
;

\ le programme principal.
\ adr_fic est le nom du fichier compact‚ … cr‚er.
: (MCGA-COMPACTE)  ( adr_fic l --- )
  DROP 0 (CREATE) ?DOS-ERR >R
  ((MCGA-CPT))
  (*MCGA-SAUVE) 0 ROT R@ (PUT)
  ?DOS-ERR DROP R> (CLOSE)
;

: (MCGA-DCOMPACTE)  ( adr_fic l --- )
  ?OPEN
  (F-SIZE@)                       \ dtaille adr_fic l ---
  DROP 0 (OPEN) ?DOS-ERR >R
  (*MCGA-SAUVE) 0 3 PICK          \ dtaille seg offC l ---
  R@ (GET) ?DOS-ERR DROP          \ dtaille ---
  R> (CLOSE) DROP
  0 0
  BEGIN                           \ taille offE offC ---
    (*MCGA-SAUVE) OVER LC@        \ taille offE offC nbo ---
    0
    DO
      (*MCGA-SAUVE) OVER 1+ LC@   \ taille offE offC oct ---
      (*MCGA-ECRAN) 3 PICK LC!    \ taille offE offC ---
      SWAP 1+ SWAP                \ taille offE+1 offC ---
    LOOP
    2+
    2 PICK 2- OVER =
  UNTIL
  2DROP DROP
;

CR .( Librairie MCGA charg‚e )
EOF

