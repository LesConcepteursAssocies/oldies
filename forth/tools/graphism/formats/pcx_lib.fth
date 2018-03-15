\ ------------------------------------------------------------------------------
\                 Kernel de Manipulation du format graphique PCX.
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

ECHO OFF

\ Chargement des outils.
EXIST? (CGA-ECRAN) NOT ?\ INCLUDE CGA_LIB
EXIST? (EGA-ECRAN) NOT ?\ INCLUDE EGA_LIB
EXIST? (MCGA-ECRAN) NOT ?\ INCLUDE MCGA_LIB

\ Palette EGA (16 couleurs).
CREATE SPALETTE-EGA 16 ALLOT      \ Palette de sauvegarde.

\ Palette MCGA (256 couleurs R, V, B).
CREATE SPALETTE-MCGA 768 ALLOT    \ Palette de sauvegarde.
CREATE IPALETTE-MCGA 768 ALLOT    \ Palette de l'image.

\ Lecture des composantes RVB de la palette courante.
\ Disponible uniquement sur BIOS VGA.
\ seg:adr : adresse du buffer de r‚ception (segment offset).
\ deb     : registre de d‚part.
\ nb      : nombre de registres … lire.
HEX
CODE PALETTE-RVB@  ( seg adr deb nb --- )
  CX POP
  BX POP
  DX POP
  ES POP
  1017 # AX MOV
  10 INT
  NEXT
END-CODE

\ Chargement des composantes RVB de la palette courante.
\ Disponible uniquement sur BIOS VGA.
\ seg:adr : adresse du buffer contenant la palette (segment offset).
\ deb     : registre de d‚part.
\ nb      : nombre de registres … ‚crire.
CODE PALETTE-RVB!  ( seg adr deb nb --- )
  CX POP
  BX POP
  DX POP
  ES POP
  1012 # AX MOV
  10 INT
  NEXT
END-CODE
DECIMAL

\ Sauvegarde de la palette MCGA courante.
: >MCGA-PALETTE  ( --- )
  DSEGMENT SPALETTE-MCGA 0 255 PALETTE-RVB@
;

\ Restauration de la palette MCGA.
: MCGA-PALETTE>  ( --- )
  DSEGMENT SPALETTE-MCGA 0 255 PALETTE-RVB!
;

\ Entˆte des fichiers PCX.
CREATE ENTETE-PCX 128 ALLOT

\ Buffer de r‚ception temporaire.
CREATE PCX-BUFFER 1 ALLOT

\ Lecture de l'entˆte du fichier.
: LIT-ENTETE-PCX  ( hdl --- )
  >R DSEGMENT ENTETE-PCX 128
  R> (GET) ?DOS-ERR DROP
;

\ V‚rifie que le fichier lu est bien au format PCX.
: ?PCX  ( --- )
  ENTETE-PCX C@ 10 = NOT ABORT" Le fichier n'est pas au format PCX"
;

\ Renvoi un code correspondant au format du fichier.
\ 1 = CGA 4 couleurs.
\ 2 = EGA 16 couleurs.
\ 3 = MCGA 256 couleurs.
\ 0 = format non reconnu.
: #PCX-FORMAT  ( --- n )
  ENTETE-PCX 3 + C@
  ENTETE-PCX 65 + C@
  2DUP 1 = SWAP 2 = AND
  IF
    2DROP 1
  ELSE
    2DUP 4 = SWAP 1 = AND
    IF
      2DROP 2
    ELSE
      1 = SWAP 8 = AND
      IF
        3
      ELSE
        0
      THEN
    THEN
  THEN
;

\ Renvoi le nombre d'octets par lignes de l'image.
: #PCX-O/L  ( --- o/l )
  ENTETE-PCX 66 + @
;

\ Renvoi la coordonn‚e du coin sup‚rieur gauche de l'image.
: #PCX-XYMIN  ( --- X Y )
  ENTETE-PCX 4 + @
  ENTETE-PCX 6 + @
;

\ Renvoi la coordonn‚e du coin inf‚rieur droit de l'image.
: #PCX-XYMAX  ( --- X Y )
  ENTETE-PCX 8 + @
  ENTETE-PCX 10 + @
;

\ Renvoi la r‚solution horizontale de l'image.
: #PCX-HRES  ( --- n )
  ENTETE-PCX 12 + @
;

\ Renvoi la r‚solution verticale de l'image.
: #PCX-VRES  ( --- n )
  ENTETE-PCX 14 + @
;

\ Lecture d'un octet du fichier et stockage dans le buffer.
: (PCX-BUFFER!)  ( hdl --- )
  >R DSEGMENT PCX-BUFFER 1 R>
  (GET) ?DOS-ERR DROP
;

\ Lecture de la palette MCGA.
\ Situ‚e … la fin du fichier, 256 composantes R, V, B, pr‚c‚d‚es d'un octet
\ d'identification ‚gal … 12 (0Ch).
: MCGA-LIT-PALETTE  ( hdl --- )
  IPALETTE-MCGA 768 ERASE
  >R -769. R@ 2 (SEEK)
  ?DOS-ERR 2DROP
  R@ (PCX-BUFFER!)            \ Lecture de l'octet d'identification.
  PCX-BUFFER C@
  12 <> ABORT" Palette MCGA non pr‚sente"
  0
  BEGIN
    R@ (PCX-BUFFER!)
    PCX-BUFFER C@ 4 IDIV
    IPALETTE-MCGA 2 PICK + C!
    1+
    DUP 768 =
  UNTIL
  DROP 128. R> 0 (SEEK) ?DOS-ERR 2DROP
;

HEX

\ Empile VRAI si l'octet pr‚sent est un compteur.
: PCX-CPT?  ( o --- fl )
  C0 AND C0 =
;

\ Lecture d'un fichier PCX CGA.
\ Temps d'‚x‚cution : environ 00:00:03,30 sec. sur 80386 … 16 Mhz.
\ hdl     : Handler du fichier.
\ adr_seg : Adresse de segment du buffer de r‚ception.
: (CGA-LIT-PCX)  ( adr_seg hdl --- )
  >R 0 0 0
  BEGIN                           \ adr_seg offecr nlig noct ---
    R@ (PCX-BUFFER!)              \ Lecture d'un octet
    PCX-BUFFER C@                 \ adr_seg offecr nlig noct oct ---
    DUP PCX-CPT?
    IF                            \ Est-ce un compteur ?
      R@ (PCX-BUFFER!)            \ Lecture de l'octet suivant
      C0 - 0                      \ adr_seg offecr nlig noct cpt 0 ---
      DO                          \ adr_seg offecr nlig noct ---
        2 PICK OVER +             \ adr_seg offecr nlig noct offecr+noct ---
        4 PICK SWAP               \ adr_seg offecr nlig noct adr_seg offecr+noct ---
        PCX-BUFFER C@ -ROT LC!
        1+                        \ adr_seg offecr nlig noct+1 ---
      LOOP                        \ adr_seg offecr nlig noct ---
    ELSE
      >R 2 PICK OVER +            \ adr_seg offecr nlig noct offecr+noct ---
      4 PICK SWAP                 \ adr_seg offecr nlig noct adr_seg offecr ---
      R> -ROT LC! 1+              \ adr_seg offecr nlig noct+1 ---
    THEN
    DUP #PCX-O/L >=               \ Fin de la ligne ?
    IF
      DROP 1+ >R                  \ Ligne suivante
      SWAP 0200 XOR SWAP          \ adr_segXOR200 offecr ---
      OVER 0200 AND 0=
      IF
        #PCX-O/L +                \ Pour les lignes paires seulement.
      THEN
      R> 0                        \ adr_seg offecr nlig noct ---
    THEN
    OVER #PCX-VRES =              \ Jusqu'… la fin de l'‚cran.
  UNTIL                           \ adr_seg offecr nlig noct ---
  R> DROP 2DROP 2DROP
;

\ Lecture d'un fichier PCX EGA.
\ Temps d'‚x‚cution : environ 00:00:17,85 sec. sur 80386 … 16 Mhz.
\ hdl     : Handler du fichier.
\ adr_seg : Adresse de segment du buffer de r‚ception.
: (EGA-LIT-PCX)  ( adr_seg hdl --- )
  >R 0 SWAP 0 0 0
  BEGIN                           \ nplan adr_seg offecr nlig noct ---
    R@ (PCX-BUFFER!)              \ Lecture d'un octet
    PCX-BUFFER C@                 \ nplan adr_seg offecr nlig noct oct ---
    DUP PCX-CPT?
    IF                            \ Est-ce un compteur ?
      R@ (PCX-BUFFER!)            \ Lecture de l'octet suivant
      C0 - 0                      \ nplan adr_seg offecr nlig noct cpt 0 ---
      DO                          \ nplan adr_seg offecr nlig noct ---
        2 PICK OVER +             \ nplan adr_seg offecr nlig noct offecr+noct ---
        5 PICK 1F40 * +           \ nplan adr_seg offecr nlig noct offecr+noct+plan ---
        4 PICK SWAP               \ nplan adr_seg offecr nlig noct adr_seg offecr+noct ---
        PCX-BUFFER C@ -ROT LC!
        1+                        \ nplan adr_seg offecr nlig noct+1 ---
        DUP #PCX-O/L >=           \ Fin de la ligne ?
        IF
          DROP >R                 \ Ligne suivante
          ROT 1+                  \ Plan suivant.
          DUP 3 >
          IF
            DROP #PCX-O/L + 0     \ adr_seg offecr+o/l nplan ---
            R> 1+ >R
          THEN
          -ROT R> 0               \ nplan adr_seg offecr nlig noct ---
        THEN
      LOOP                        \ nplan adr_seg offecr nlig noct ---
    ELSE
      >R 2 PICK OVER +            \ nplan adr_seg offecr nlig noct offecr+noct ---
      5 PICK 1F40 * +             \ nplan adr_seg offecr nlig noct offecr+noct+plan ---
      4 PICK SWAP                 \ nplan adr_seg offecr nlig noct adr_seg offecr ---
      R> -ROT LC! 1+              \ nplan adr_seg offecr nlig noct+1 ---
    THEN
    DUP #PCX-O/L >=               \ Fin de la ligne ?
    IF
      DROP >R                     \ Ligne suivante
      ROT 1+                      \ Plan suivant.
      DUP 3 >
      IF
        DROP #PCX-O/L + 0         \ adr_seg offecr+o/l nplan ---
        R> 1+ >R
      THEN
      -ROT R> 0                   \ nplan adr_seg offecr nlig noct ---
    THEN
    OVER #PCX-VRES =              \ Jusqu'… la fin de l'‚cran.
  UNTIL                           \ nplan adr_seg offecr nlig noct ---
  R> 2DROP 2DROP 2DROP
;

\ Lecture d'un fichier PCX MCGA.
\ Temps d'‚x‚cution : environ 00:00:21,53 sec. sur 80386 … 16 Mhz.
\ hdl     : Handler du fichier.
\ adr_seg : Adresse de segment du buffer de r‚ception.
: (MCGA-LIT-PCX)  ( adr_seg hdl --- )
  >R 0 0 0
  BEGIN                           \ adr_seg offecr nlig noct ---
    R@ (PCX-BUFFER!)              \ Lecture d'un octet
    PCX-BUFFER C@                 \ adr_seg offecr nlig noct oct ---
    DUP PCX-CPT?
    IF                            \ Est-ce un compteur ?
      R@ (PCX-BUFFER!)            \ Lecture de l'octet suivant
      C0 - 0                      \ adr_seg offecr nlig noct cpt 0 ---
      DO                          \ adr_seg offecr nlig noct ---
        2 PICK OVER +             \ adr_seg offecr nlig noct offecr+noct ---
        4 PICK SWAP               \ adr_seg offecr nlig noct adr_seg offecr+noct ---
        PCX-BUFFER C@ -ROT LC!
        1+                        \ adr_seg offecr nlig noct+1 ---
      LOOP                        \ adr_seg offecr nlig noct ---
    ELSE
      >R 2 PICK OVER +            \ adr_seg offecr nlig noct offecr+noct ---
      4 PICK SWAP                 \ adr_seg offecr nlig noct adr_seg offecr ---
      R> -ROT LC! 1+              \ adr_seg offecr nlig noct+1 ---
    THEN
    DUP #PCX-O/L >=               \ Fin de la ligne ?
    IF
      DROP 1+ >R                  \ Ligne suivante
      #PCX-O/L +                  \ adr_seg offecr+o/l ---
      R> 0                        \ adr_seg offecr nlig noct ---
    THEN
    OVER #PCX-VRES =              \ Jusqu'… la fin de l'‚cran.
  UNTIL                           \ adr_seg offecr nlig noct ---
  R> DROP 2DROP 2DROP
;

DECIMAL

\ Le programme principal.
: LIT-PCX  ( adr_fic l --- )
  ?OPEN
  DROP 0 (OPEN) ?DOS-ERR >R
  R@ LIT-ENTETE-PCX ?PCX
  #PCX-FORMAT
  CASE
    1 OF
        (*CGA-ECRAN) R@ (CGA-LIT-PCX)
      ENDOF
    2 OF
        (*EGA-ECRAN) R@ (EGA-LIT-PCX)
      ENDOF
    3 OF
        R@ MCGA-LIT-PALETTE
        (*MCGA-ECRAN) R@ (MCGA-LIT-PCX)
      ENDOF
  ENDCASE
  R> (CLOSE)
;

\ A titre de test :
\ Lecture et affichage d'un fichier MCGA.
: .PCX-MCGA  ( adr l --- )
  19 MODE
  (MCGA-*INIT)
  LIT-PCX
  >MCGA-PALETTE
  DSEGMENT IPALETTE-MCGA 0 255 PALETTE-RVB!
  (MCGA-*ECR->ECR)
  BEGIN
    KEY 32 =
  UNTIL
  MCGA-PALETTE>
  (*MCGA-ECRAN) RELEASE
  (*MCGA-SAUVE) RELEASE
  3 MODE
;

CR .( Librairie PCX charg‚e )
EOF

