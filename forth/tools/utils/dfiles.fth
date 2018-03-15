\ -----------------------------------------------------------------------------
\                        Outils de gestion de fichiers.
\
\ Adaptation de UFILES.FTH Par Guillaumaud Philippe
\
\ -----------------------------------------------------------------------------

ECHO OFF

\ Primitives de bas niveau :
\  - Gestion des attributs d'un fichier.
\  - Gestion de la date et de l'heure.

\ Constantes d'ouvertures de fichier
0 CONSTANT READ
1 CONSTANT WRITE
2 CONSTANT READ-WRITE

HEX

\ Empile l'attribut du fichier.
: GET-FATTR  ( adr_fic --- attr )
  PATHWAY
  ASM[
       DX POP
       4300 # AX MOV
       21 INT
       CX PUSH
       U>=
       IF
         0 # AX MOV
       THEN
       1PUSH
  ]FORTH
  ?DOS-ERR
;  

\ Modifie l'attribut d'un fichier.
: SET-FATTR  ( attr adr_fic --- )
  PATHWAY
  ASM[
       DX POP
       CX POP
       4301 # AX MOV
       21 INT
       U>=
       IF
         0 # AX MOV
       THEN
       1PUSH
  ]FORTH
  ?DOS-ERR
;

\ Conversion d'un double en jour mois ann‚e, heure minute.
: D->DTHE  ( d --- jour mois ann‚e heure minute )
  >R DUP 1F AND SWAP         \ jour n ---
  DUP 20 / 0F AND SWAP       \ jour mois n ---
  200 / 7BC +                \ jour mois ann‚e ---
  R> DUP 800 / SWAP          \ jour mois ann‚e heure n ---
  20 / 3F AND                \ jour mois ann‚e heure minute ---
;

\ Conversion d'une date et d'une heure en un double.
: DTHE->D  ( jour mois ann‚e heure minute --- d )
  20 * SWAP 800 * + >R
  7BC - 0 MAX 200 * 
  SWAP 20 * + + R>
;

\ Empile la date et l'heure de la derniŠre
\ modification d'un fichier.
CODE (GET-FDATE)  ( handle --- d fl )
  BX POP
  5700 # AX MOV
  21 INT
  DX PUSH
  CX PUSH
  U>=
  IF
    0 # AX MOV
  THEN
  1PUSH
END-CODE
  
: GET-FDATE  ( adr_fic -- jour mois ann‚e heure minute )
  PATHWAY
  READ (OPEN) ?DOS-ERR
  DUP >R
  (GET-FDATE) ?DOS-ERR
  D->DTHE
  R> (CLOSE)
;

\ Initialise la date et l'heure de la derniŠre
\ modification d'un fichier.
CODE (SET-FDATE)  ( d handle --- fl )
  BX POP
  CX POP
  DX POP
  5701 # AX MOV
  21 INT
  U>=
  IF
    0 # AX MOV
  THEN
  1PUSH
END-CODE
  
: SET-FDATE  ( jour mois ann‚e heure minute adr_fic --- )
  PATHWAY
  READ (OPEN) ?DOS-ERR
  >R
  DTHE->D
  R@
  (SET-FDATE) ?DOS-ERR
  R> (CLOSE)
;  

\ Empile la taille d'un fichier en octets.
\ Le nombre empil‚ est un double.
: (GET-FSIZE)  ( handle --- d )
  0 0 ROT 2 (SEEK) ?DOS-ERR
;

: GET-FSIZE  ( adr_fic -- d )
  PATHWAY
  READ (OPEN) ?DOS-ERR
  DUP >R
  (GET-FSIZE)
  R> (CLOSE)
;

\ Empile la place disponible sur un disque en octets.
\ n = 0 pour le lecteur par d‚faut, 1 pour A,...
\ le nombre empil‚ est un double.
: DISK-SPACE  ( n --- d )
  (FREE) * UM*
;

\ -----------------------------------------------------------------------------
\                    Gestion de fichiers … accŠs direct.
\ -----------------------------------------------------------------------------

DECIMAL
4096 CONSTANT MAX-SIZE            \ Taille maxi d'un enregistrement
CREATE BUF-DIR MAX-SIZE ALLOT     \ Buffer du fichier
VARIABLE HFILE                    \ Handle du fichier courant
VARIABLE REC-SIZE                 \ Taille d'un enregistrement en octets

\ Empile le nombre d'enregistrements dans le fichier courant.
: RECCOUNT  ( --- n )
  HFILE @ 0= ABORT" RECCOUNT : Pas de fichier ouvert"
  HFILE @ (GET-FSIZE)
  REC-SIZE @ UM/MOD
  SWAP DROP
;

\ Positionnement sur le niŠme enregistrement.
: LOCATE  ( n --- )
  HFILE @ 0= ABORT" LOCATE : Pas de fichier ouvert"
  REC-SIZE @ UM*
  HFILE @ 0 (SEEK)
  ?DOS-ERR
  2DROP
;

\ Ouverture d'un fichier en lecture.
\ On pr‚cise la taille d'un enregistrement.
: OPENI  ( recsize adr_fic --- )
  ?OPEN
  DUP MAX-SIZE U> ABORT" OPENI : Enregistrement trop grand"
  PATHWAY READ (OPEN) ?DOS-ERR
  HFILE !
  REC-SIZE !
;

\ Ouverture ou cr‚ation d'un fichier en ‚criture.
\ On pr‚cise la taille d'un enregistrement.
: OPENO  ( recsize adr_fic --- )
  ?OPEN
  DUP MAX-SIZE U> ABORT" OPENO : Enregistrement trop grand"
  PATHWAY
  DUP 0 (SEARCH0)
  IF
    WRITE (OPEN) 
  ELSE
    0 (CREATE)
  THEN
  ?DOS-ERR
  HFILE !
  REC-SIZE !
;

\ Ouverture ou cr‚ation d'un fichier en ajout.
\ On pr‚cise la taille d'un enregistrement.
\ Le pointeur est plac‚ … la fin du fichier.
: OPENA  ( recsize adr_fic --- )
  ?OPEN
  DUP MAX-SIZE U> ABORT" OPENA : Enregistrement trop grand"
  PATHWAY
  DUP 0 (SEARCH0)
  IF
    READ-WRITE (OPEN) 
  ELSE
    0 (CREATE)
  THEN
  ?DOS-ERR
  DUP HFILE !
  SWAP REC-SIZE !
  0 0 ROT 2 (SEEK)
  ?DOS-ERR 2DROP
;

\ Lecture du niŠme enregistrement.
: FREAD  ( n --- )
  DUP RECCOUNT U> ABORT" FREAD : Enregistrement hors limite"
  LOCATE
  DSEGMENT BUF-DIR REC-SIZE @ HFILE @ (GET)
  ?DOS-ERR DROP
;

\ Ecriture du contenu du buffer dans le niŠme enregistrement.
: FWRITE  ( n --- )
  LOCATE
  DSEGMENT BUF-DIR REC-SIZE @ HFILE @ (PUT)
  ?DOS-ERR DROP
;

\ Ajout du contenu du buffer … l'emplacement du pointeur.
: FAPPEND  ( --- )
  DSEGMENT BUF-DIR REC-SIZE @ HFILE @ (PUT)
  ?DOS-ERR DROP
;

\ Fermeture du fichier.
: FCLOSE
  HFILE @ 0= ABORT" FCLOSE : Pas de fichier ouvert"
  HFILE DUP @ (CLOSE)
  0 SWAP !
;

\ D‚coupage d'un enregistrement en champs.
: FIELD  ( pos lon nom_champ --- )
  CREATE
    , ,
  DOES>  ( --- adr lon )
    BUF-DIR OVER 2+ @ +
    SWAP @
;

EOF 
-------------------------------------------------------------------------------
Attributs d'un fichier DOS :
         76543210
         XXXXXXXX      - Nombre binaire (6 bits significatifs)
           ::::::
           :::::+->  1 - Fichier ne pouvant ˆtre que lu donc
           :::::         non modifiable et non effa‡able.
           :::::
           ::::+-->  2 - Fichier invisible avec la commande DIR.
           ::::
           :::+--->  4 - Fichier systeme, invisible avec la commande DIR.
           :::
           ::+---->  8 - Fichier contenant le nom du volume dans
           ::            ses onze premier octets.
           ::
           :+-----> 16 - Entr‚e d'un r‚pertoire.
           :
           +------> 32 - Fichier modifi‚ et ferm‚ (archivage).

      L'attributs 0 d‚signe un fichier normal.

