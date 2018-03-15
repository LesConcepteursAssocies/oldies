\ ------------------------------------------------------------------------------
\     Kernel de Manipulation du modes graphique 13 (EGA 320x200 16 Couleurs)
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

WARNING OFF

\ Chargement des outils.
EXIST? << NOT ?\ INCLUDE KERNGRPH

DECIMAL

\ D‚finition des 4 plans de bits des 16 couleurs EGA
CREATE EGA-COULEURS
0 C, 0 C, 0 C, 0 C,     \ Couleur 0
1 C, 0 C, 0 C, 0 C,     \ Couleur 1
0 C, 1 C, 0 C, 0 C,     \ Couleur 2
1 C, 1 C, 0 C, 0 C,     \ Couleur 3
0 C, 0 C, 1 C, 0 C,     \ Couleur 4
1 C, 0 C, 1 C, 0 C,     \ Couleur 5
0 C, 1 C, 1 C, 0 C,     \ Couleur 6
1 C, 1 C, 1 C, 0 C,     \ Couleur 7
0 C, 0 C, 0 C, 1 C,     \ Couleur 8
1 C, 0 C, 0 C, 1 C,     \ Couleur 9
0 C, 1 C, 0 C, 1 C,     \ Couleur 10
1 C, 1 C, 0 C, 1 C,     \ Couleur 11
0 C, 0 C, 1 C, 1 C,     \ Couleur 12
1 C, 0 C, 1 C, 1 C,     \ Couleur 13
0 C, 1 C, 1 C, 1 C,     \ Couleur 14
1 C, 1 C, 1 C, 1 C,     \ Couleur 15

HEX
0A000 CONSTANT (EGA-ECRAN)
DECIMAL

\ Pointeurs sur ‚crans virtuels
\ A initialiser avant toutes manipulations
\ ****************************************
0 CONSTANT (*EGA-ECRAN)                \ 4 plans de 8000 octets.
0 CONSTANT (*EGA-SAUVE)                \ Ecran de sauvegarde.

\ Initialisation des pointeurs
: (EGA-*INIT)  ( --- )
  LIB-COM
  32768 MALLOC IS (*EGA-ECRAN)
  32768 MALLOC IS (*EGA-SAUVE)
;

\ Sauvegarde de l'‚cran dans l'‚cran virtuel
HEX
: (EGA-ECR->*ECR)  ( --- )
  4 0
  DO
    4 03CE PC!
    I 03CF PC!
    (EGA-ECRAN) 0 (*EGA-ECRAN) I 1F40 * 1F40 LCMOVE
  LOOP
  4 03CE PC!
  0 03CF PC!
;

\ Restitution de l'‚cran
: (EGA-*ECR->ECR)  ( --- )
  4 0
  DO
    2 03C4 PC!
    1 I << 03C5 PC!
    (*EGA-ECRAN) I 1F40 * (EGA-ECRAN) 0 1F40 LCMOVE
  LOOP
  2 03C4 PC!
  F 03C5 PC!
;
DECIMAL

\ Ecriture d'un point de couleur coul aux coordonn‚es x,y
\ dans l'‚cran virtuel
\ coul = [0..15]
: (EGA-PSET)  ( x y coul --- )
  7 3 PICK - 8 MOD           \ x y coul mask ---
  4 0
  DO                         \ x y coul mask ---
    EGA-COULEURS 2 PICK
    4 * + I + C@ DUP         \ x y coul mask bit ---
    0>
    IF                       \ x y coul mask bit ---
      (*EGA-ECRAN) I 8000 *
      5 PICK 40 * 7 PICK
      8 IDIV + +             \ x y coul mask bit seg off+(y*40+xDIV8) ---
      2DUP LC@
      1 5 PICK << NOT AND    \ x y coul mask bit seg off+(y*40+xDIV8) (coul AND NOT(1<-mask)) ---
      3 ROLL 4 PICK << OR    \ x y coul mask seg off+(y*40+xDIV8) ((coul AND NOT(1<-mask)) OR (bit<-mask)) ---
      -ROT LC!
    ELSE
      DROP
    THEN                     \ x y coul mask ---
  LOOP
  2DROP 2DROP
;

\ Ecriture d'un point.
LABEL <EGA-PLOT>
  DX PUSH
  BL CL MOV
  80 # DX MOV
  DX MUL                     \ AX = (y * 80)
  BX SHR
  BX SHR
  BX SHR                     \ BX = (x / 8)
  AX BX ADD                  \ BX = (x / 8) + (y * 80)
  (EGA-ECRAN) # AX MOV
  AX ES MOV
  7 # CL AND                 \ Masque les bits 4 … 7 de x.
  7 # CL XOR                 \ Revient … faire (7 - x).
  1 # AH MOV
  DX POP
  RET

HEX
\ Performances : 224000 points ‚crits en 00:00:10,93 sec. sur 80386 … 16 MHz.
CODE (EGA-PLOT)  ( x y coul --- )
  CX POP
  CH CL XCHG                 \ CH = coul.
  AX POP
  BX POP
  AX PUSH
  BX PUSH
  <EGA-PLOT> #) CALL
  03CE # DX MOV              \ Adresse du controleur graphique EGA.
  AH CL SHL                  \ Masquage du bit … modifier.
  8 # AL MOV                 \ Registre de masque bits.
  0 AX OUT                   \ Ecriture.
  205 # AX MOV               \ Nø de registre et code de mode.
  0 AX OUT
  3 # AX MOV                 \ Nø de registre et combinaison.
  0 AX OUT
  ES: 0 [BX] AL MOV          \ Lecture des latches.
  ES: CH 0 [BX] MOV          \ Ecrire couleur.
  \ R‚tablissement des valeurs initiales des registres.
  FF08 # AX MOV              \ Ancien masque de bits.
  0 AX OUT
  5 # AX MOV                 \ Ancienne valeur pour le registre de mode.
  0 AX OUT
  3 # AX MOV                 \ Ancienne valeur pour le registre function select.
  0 AX OUT
  BX POP
  AX POP
  NEXT
END-CODE
DECIMAL

\ Lecture d'un fichier graphique et stockage dans l'‚cran virtuel
: (EGA-DSQ->*ECR)  ( adr_fic l --- )
  ?OPEN
  DROP 0 (OPEN) ?DOS-ERR >R
  (*EGA-ECRAN) 0 32000 R@ (GET) ?DOS-ERR DROP
  R> (CLOSE)
;

\ Sauvegarde de l'‚cran sur disque
HEX
: (EGA-ECR->DSQ)  ( adr_fic l --- )
  DROP 0 (CREATE) ?DOS-ERR
  4 0
  DO
    4 03CE PC!
    I 03CF PC!
    (EGA-ECRAN) 0 1F40 3 PICK (PUT) ?DOS-ERR DROP
  LOOP
  4 03CE PC!
  0 03CF PC!
  (CLOSE)
;

\ Sauvegarde d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x1,y1   = coordonn‚es du coin sup‚rieur gauche de l'image.
\ x2,y2   = coordonn‚es du coin inf‚rieur droit de l'image.
: (EGA-@IMAGE)  ( seg_src seg_dst x1 y1 x2 y2 --- )
  SWAP-COORD
  DUP 3 PICK - 1+ 0          \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  4 0
  DO                         \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
    4 03CE PC!
    I 03CF PC!
    2DUP
    DO                       \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
      4 PICK I + 28 *        \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 (y1+I)*40 ---
      6 PICK +               \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 (y1+I)*40+x1 ---
      8 PICK SWAP            \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 seg_src (y1+I)*40+x1 ---
      8 PICK I 28 *
      J 1F40 * +             \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 seg_src (y1+I)*40+x1 seg_dst I*40+plan ---
      7 PICK 0A PICK - 1+    \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 seg_src (y1+I)*40+x1 seg_dst I*40+plan x2-x1+1 ---
      LCMOVE
    LOOP                     \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  LOOP                       \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  4 03CE PC!
  0 03CF PC!
  2DROP 2DROP 2DROP 2DROP
;

\ Restitution d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x,y     = coordonn‚e ou placer l'image (coin sup‚rieur gauche).
\ largeur = largeur de l'image en points.
\ hauteur = hauteur de l'image en points.
: (EGA-!IMAGE)  ( seg_src seg_dst x y largeur hauteur --- )
  SWAP 8 IDIV SWAP 4 0
  DO                          \ seg_src seg_dst x y largeur hauteur ---
    2 03C4 PC!
    1 I << 03C5 PC!
    DUP 0
    DO                        \ seg_src seg_dst x y largeur hauteur ---
      5 PICK I 28 *
      J 1F40 * +              \ seg_src seg_dst x y largeur hauteur seg_src I*40+plan ---
      6 PICK 5 PICK I +       \ seg_src seg_dst x y largeur hauteur seg_src I*40+plan seg_dst (y+I) ---
      28 * 7 PICK + 5 PICK    \ seg_src seg_dst x y largeur hauteur seg_src I*40+plan seg_dst ((y+I)*40)+x largeur ---
      LCMOVE
    LOOP                      \ seg_src seg_dst x y largeur hauteur ---
  LOOP                        \ seg_src seg_dst x y largeur hauteur ---
  2 03C4 PC!
  F 03C5 PC!
  2DROP 2DROP 2DROP
;

\ Scrolling horizontal de l'‚cran.
\ flag est VRAI pour aller … gauche, FAUX pour aller … droite.
: (EGA-HSCROLL)  ( flag --- )
  0D 03D4 PC!      \ indique … la carte que l'on veut modifier l'adresse
                   \ de poids faible du buffer d'‚cran.
  IF
    28 1
    DO
      I 03D5 PC!   \ on indique ensuite de combien de 1/80Šme d'‚cran
                   \ on veut d‚caler l'adresse du buffer.
      12 MS        \ temporisation (trop rapide sans cela...)
    LOOP
  ELSE
    28 1
    DO
      28 I - 03D5 PC!
      12 MS
    LOOP
  THEN
  0 03D5 PC!
  12 MS
;

DECIMAL
\ Compactage d'images EGA 320 x 200.
\ Compacte l'image pr‚sente dans l'‚cran virtuel dans (*EGA-SAUVE).
\ Renvoie le nombre d'octets de l'image compact‚e ou 0 si
\ le compactage ne rapporte rien.
\ Principe : on balaye l'image, et pour chaque point, on compte
\            le nombre de points identiques, que l'on stocke
\            dans la zone de compactage, avec la couleur du point
\            correspondante.
\
\ Le gain sera bien sur fonction de la complexit‚ du dessin.
\
: ((EGA-CPT))  ( --- l )
  0 0 0 0
  BEGIN                      \ offC oct nbo offE ---
    DUP 0 U>
    IF
      1+
    THEN
    ROT DROP
    (*EGA-ECRAN) OVER LC@    \ offC nbo offE oct ---
    ROT DROP 1 -ROT          \ offC 1 offE oct ---
    OVER 31998 U<
    IF
      BEGIN                  \ offC nbo offE oct ---
        OVER 1+ (*EGA-ECRAN)
        SWAP LC@
        2 PICK (*EGA-ECRAN)
        SWAP LC@ =
        2 PICK 31998 U< AND
        IF
          ROT 1+ -ROT        \ offC nbo+1 offE oct ---
          SWAP 1+ SWAP       \ offC nbo offE+1 oct ---
        THEN
        OVER 31998 =
        2 PICK DUP 1+
        (*EGA-ECRAN) SWAP
        LC@ (*EGA-ECRAN) ROT
        LC@ <> OR 3 PICK 255
        = OR
      UNTIL                  \ offC nbo offE oct ---
    THEN
    3 PICK 31998 U>
    IF
      2DROP 2DROP
      0
      EXIT
    THEN
    3 PICK (*EGA-SAUVE)
    SWAP 4 PICK -ROT LC!
    2SWAP SWAP 1+ SWAP 2SWAP \ offC+1 nbo offE oct ---
    DUP 4 PICK (*EGA-SAUVE)
    SWAP LC!
    2SWAP SWAP 1+ SWAP 2SWAP \ offC+1 nbo offE oct ---
    -ROT DUP 31998 =
  UNTIL                      \ offC oct nbo offE
  DROP 2DROP (*EGA-SAUVE)
  OVER (*EGA-ECRAN) 31999 LC@
  -ROT LC!                   \ offC ---
;

\ le programme principal.
\ adr_fic est le nom du fichier compact‚ … cr‚er.
: (EGA-COMPACTE)  ( adr_fic l --- )
  DROP 0 (CREATE) ?DOS-ERR >R
  ((EGA-CPT))
  (*EGA-SAUVE) 0 ROT R@ (PUT)
  ?DOS-ERR DROP R> (CLOSE)
;

\ D‚compactage d'un fichier image.
\ Met l'image reconstitu‚e dans (*EGA-ECRAN).
: (EGA-DCOMPACTE)  ( adr_fic l --- )
  ?OPEN
  (F-SIZE@)                       \ dtaille adr_fic l ---
  DROP 0 (OPEN) ?DOS-ERR >R
  (*EGA-SAUVE) 0 3 PICK           \ dtaille seg offC l ---
  R@ (GET) ?DOS-ERR DROP          \ dtaille ---
  R> (CLOSE) DROP
  0 0
  BEGIN                           \ taille offE offC ---
    (*EGA-SAUVE) OVER LC@         \ taille offE offC nbo ---
    0
    DO
      (*EGA-SAUVE) OVER 1+ LC@    \ taille offE offC oct ---
      (*EGA-ECRAN) 3 PICK LC!     \ taille offE offC ---
      SWAP 1+ SWAP                \ taille offE+1 offC ---
    LOOP
    2+
    2 PICK 2- OVER =
  UNTIL
  2DROP DROP
;

CR .( Librairie EGA charg‚e )
EOF

