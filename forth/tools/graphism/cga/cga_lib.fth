\ ------------------------------------------------------------------------------
\  Kernel de Manipulation des modes graphiques 4, 5 (CGA 320x200 4 Couleurs)
\                                              et 6 (CGA 640x200 2 Couleurs)
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

WARNING OFF

\ Chargement des outils.
EXIST? << NOT ?\ INCLUDE KERNGRPH
EXIST? LBMOVE NOT ?\ INCLUDE BITS

HEX
\ Pointeur sur R.A.M ‚cran
0B800 CONSTANT (CGA-ECRAN)
DECIMAL
 4 CONSTANT (CGA-#P/O)                 \ Nombre de points par octet.
80 CONSTANT (CGA-#O/L)                 \ Nombre d'octets par ligne.

\ Pointeurs sur ‚crans virtuels
\ A initialiser avant toutes manipulations
\ ****************************************
0 CONSTANT (*CGA-ECRAN)
0 CONSTANT (*CGA-SAUVE)                \ Ecran de sauvegarde.

\ Initialisation des pointeurs
: (CGA-*INIT)  ( --- )
  LIB-COM
  16384 MALLOC IS (*CGA-ECRAN)
  16384 MALLOC IS (*CGA-SAUVE)
;

\ Sauvegarde de l'‚cran dans l'‚cran virtuel
: (CGA-ECR->*ECR)  ( --- )
  (CGA-ECRAN) 0 (*CGA-ECRAN) 0 16384 LCMOVE
;

\ Restitution de l'‚cran
: (CGA-*ECR->ECR)  ( --- )
  (*CGA-ECRAN) 0 (CGA-ECRAN) 0 16384 LCMOVE
;

\ Ecriture d'un point de couleur coul aux coordonn‚es x,y
\ dans l'‚cran virtuel
\ coul = [0..3]
\
\ Performances : 64000 points ‚crits en 00:00:27,36 sec. sur 80386 … 16 MHz.
: (CGA-PSET)  ( x y coul --- )
  3 3 PICK - (CGA-#P/O)
  MOD 2*                     \ x y coul dep ---
  DUP -ROT <<                \ x y dep coul<-dep ---
  SWAP 3 SWAP <<             \ x y coul<-dep mask ---
  3 ROLL 3 PICK              \ y coul<-dep mask x y ---
  2 IDIV (CGA-#O/L) *        \ y coul<-dep mask x y/2*80 ---
  SWAP (CGA-#P/O) IDIV +     \ y coul<-dep mask y/2*80+x/4 ---
  3 ROLL 2MOD 1 =
  IF                         \ coul<-dep mask y/2*80+x/4 ---
    8192 +                   \ si ligne impaire.
  THEN
  (*CGA-ECRAN) OVER LC@      \ coul<-dep mask y/2*80+x/4 o ---
  ROT NOT AND                \ coul<-dep y/2*80+x/4 (o AND (NOT mask)) ---
  ROT OR                     \ y/2*80+x/4 ((o AND (NOT mask)) OR coul<-dep) ---
  (*CGA-ECRAN) ROT LC!
;

\ Ecriture d'un point.
LABEL <CGA-PLOT>
  AX PUSH
  DI DI XOR
  (CGA-ECRAN) # CX MOV
  CX ES MOV
  DX AX MOV
  AX SHR                     \ AX = y / 2
  80 # CL MOV
  CL MUL                     \ AX = (y / 2) * 80
  1 # DX AND                 \ DX = y mod 2
  3 # CL MOV
  DX CL ROR                  \ DX = (y mod 2) * 2000h
  AX DI MOV
  DX DI ADD
  BX DI ADD                  \ DI = (y / 2) * 80 + 2000h * (y mod 2) + (x / 4)
  AX POP
  ES: 0 [DI] BL MOV
  AH BL AND
  AL BL OR
  ES: BL 0 [DI] MOV
  NEXT

\ Ecrit un point en basse r‚solution (320x200).
\ Performances : 64000 points ‚crits en 00:00:01,87 sec. sur 80386 … 16 MHz.
CODE (CGA-PLOTL)  ( x y coul --- )
  AX POP                     \ AL = coul
  DX POP                     \ DX = y
  BX POP                     \ BX = x
  6 # CL MOV
  BL AH MOV
  3 # AH AND                 \ AH = x mod 4
  AH SHL                     \ AH = 2 * (x mod 4)
  AH CL SUB                  \ CL = 6 - 2 * (x mod 4)
  3 # AH MOV
  AX CL SHL
  AH NOT                     \ AH = masque de bits
  BX SHR
  BX SHR                     \ BX = x / 4
  <CGA-PLOT> #) JMP
END-CODE

\ Ecrit un point en haute r‚solution (640x200).
\ Performances : 128000 points ‚crits en 00:00:03,79 sec. sur 80386 … 16 MHz.
CODE (CGA-PLOTH)  ( x y coul --- )
  AX POP                     \ AL = coul
  DX POP                     \ DX = y
  BX POP                     \ BX = x
  7 # CL MOV
  BL AH MOV
  7 # AH AND                 \ AH = x mod 8
  AH CL SUB                  \ CL = 7 - (x mod 8)
  1 # AH MOV
  AX CL SHL
  AH NOT                     \ AH = masque de bits
  3 # CL MOV
  BX CL SHR                  \ BX = x / 8
  <CGA-PLOT> #) JMP
END-CODE

\ Lecture d'un fichier graphique et stockage dans l'‚cran virtuel
: (CGA-DSQ->*ECR)  ( adr_fic l --- )
  ?OPEN
  DROP 0 (OPEN) ?DOS-ERR >R
  (*CGA-ECRAN) 0 16384 R@ (GET) ?DOS-ERR DROP
  R> (CLOSE)
;

\ Sauvegarde de l'‚cran sur disque
: (CGA-ECR->DSQ)  ( adr_fic l --- )
  DROP 0 (CREATE) ?DOS-ERR >R
  (CGA-ECRAN) 0 16384 R@ (PUT) ?DOS-ERR DROP
  R> (CLOSE)
;

\ Sauvegarde d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x1,y1   = coordonn‚es du coin sup‚rieur gauche de l'image.
\ x2,y2   = coordonn‚es du coin inf‚rieur droit de l'image.
: (CGA-@IMAGE)  ( seg_src seg_dst x1 y1 x2 y2 --- )
  SWAP-COORD
  DUP 3 PICK - 1+ 0          \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  DO                         \ seg_src seg_dst x1 y1 x2 y2 ---
    2 PICK I + (CGA-#O/L) *  \ seg_src seg_dst x1 y1 x2 y2 y1+I*80 ---
    4 PICK +                 \ seg_src seg_dst x1 y1 x2 y2 y1+I*80+x1 ---
    6 PICK SWAP              \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*80+x1 ---
    6 PICK I (CGA-#O/L) *    \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*80+x1 seg_dst I*80 ---
    5 PICK 8 PICK - 1+       \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*80+x1 seg_dst I*80 x2-x1+1 ---
    (CGA-#P/O) / 1+          \ seg_src seg_dst x1 y1 x2 y2 seg_src y1+I*80+x1 seg_dst I*80 ((x2-x1+1)/4)+1 ---
    >R 4DUP R@ LCMOVE        \ lignes paires
    8192 + ROT 8192 +
    -ROT R> LCMOVE           \ lignes impaires
  LOOP                       \ seg_src seg_dst x1 y1 x2 y2 ---
  2DROP 2DROP 2DROP
;

\ Calcule le nø du bit du point X.
\ Si cette formule renvoie 4, le point est cod‚ dans les bits 4 et 5.
: X->BIT#  ( X --- bit# )
  6 SWAP 4 MOD 2* -          \ 6-2*(xMOD4) ---
;

\ Calcule l'offset dans l'‚cran du point X,Y.
: XY->OFFS  ( X Y -- offs )
  SWAP 4 IDIV                \ y (x/4) ---
  OVER 2MOD 8192 *           \ y (x/4) (yMOD2)*2000h ---
  ROT 2 IDIV 80 *            \ (x/4) (yMOD2)*2000h (y/2)*80 ---
  + +                        \ (x/4)+(yMOD2)*2000h+(y/2)*80 ---
;

\ Sauvegarde d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x1,y1   = coordonn‚es du coin sup‚rieur gauche de l'image.
\ x2,y2   = coordonn‚es du coin inf‚rieur droit de l'image.
: (@IMAGE)  ( seg_src seg_dst x1 y1 x2 y2 --- )
  SWAP-COORD
  DUP 3 PICK - 1+ 0          \ seg_src seg_dst x1 y1 x2 y2 y2-y1+1 0 ---
  DO                         \ seg_src seg_dst x1 y1 x2 y2 ---
    3 PICK 3 PICK I + XY->OFFS   \ seg_src seg_dst x1 y1 x2 y2 offs ---
    6 PICK SWAP              \ seg_src seg_dst x1 y1 x2 y2 seg_src offs ---
    5 PICK X->BIT# 9 +        \ seg_src seg_dst x1 y1 x2 y2 seg_src offs bs ---
    7 PICK 5 PICK 8 PICK - 1+ \ seg_src seg_dst x1 y1 x2 y2 seg_src offs bs seg_dst x2-x1+1 ---
    4 IDIV I * 0              \ seg_src seg_dst x1 y1 x2 y2 seg_src offs bs seg_dst ((x2-x1+1)/4)*I bd ---
    7 PICK 10 PICK - 1+ 2*
    LBMOVE
  LOOP                       \ seg_src seg_dst x1 y1 x2 y2 ---
  2DROP 2DROP 2DROP
;

\ Restitution d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x,y     = coordonn‚e ou placer l'image (coin sup‚rieur gauche).
\ largeur = largeur de l'image en points.
\ hauteur = hauteur de l'image en points.
: (!IMAGE)  ( seg_src seg_dst x y largeur hauteur --- )
  1+ 0
  DO                         \ seg_src seg_dst x y largeur ---
    4 PICK OVER 1+ 4 IDIV
    I * 0                   \ seg_src seg_dst x y largeur seg_src offs bs ---
    6 PICK                   \ seg_src seg_dst x y largeur seg_src offs bs seg_dst ---
    6 PICK 6 PICK I + XY->OFFS \ seg_src seg_dst x y largeur seg_src offs bs seg_dst offd ---
    7 PICK X->BIT# 1+        \ seg_src seg_dst x y largeur seg_src offs bs seg_dst offd bs ---
    6 PICK 1+ 2* LBMOVE
  LOOP                       \ seg_src seg_dst x y largeur ---
  DROP 2DROP 2DROP
;

\ Restitution d'une portion d'image.
\ seg_src = Adresse de segment de l'‚cran source.
\ seg_dst = Adresse de segment de l'‚cran destination.
\ x,y     = coordonn‚e ou placer l'image (coin sup‚rieur gauche).
\ largeur = largeur de l'image en points.
\ hauteur = hauteur de l'image en points.
: (CGA-!IMAGE)  ( seg_src seg_dst x y largeur hauteur --- )
  2/ 0
  DO                         \ seg_src seg_dst x y largeur ---
    4 PICK I (CGA-#O/L) *    \ seg_src seg_dst x y largeur seg_src I*80 ---
    5 PICK 4 PICK I +        \ seg_src seg_dst x y largeur seg_src I*80 seg_dst y+I ---
    (CGA-#O/L) * 6 PICK
    (CGA-#P/O) / 1+ +        \ seg_src seg_dst x y largeur seg_src I*80 seg_dst y+I*80+x ---
    4 PICK (CGA-#P/O) / 1+   \ seg_src seg_dst x y largeur seg_src I*80 seg_dst y+I*80+x (largeur/4)+1 ---
    >R 4DUP
    R@ LCMOVE                \ lignes paires
    8192 + ROT 8192 +
    -ROT R> LCMOVE           \ lignes impaires
  LOOP                       \ seg_src seg_dst x y largeur ---
  DROP 2DROP 2DROP
;

CR .( Librairie CGA charg‚e )
EOF

