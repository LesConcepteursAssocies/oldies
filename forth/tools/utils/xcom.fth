\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Variation sur le programme DIRTREE.FTH de M. ZUPAN.
\
\ Ex‚cution d'une commande dans le r‚pertoire courant, et dans les
\ sous-r‚pertoire fils de celui-ci.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

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

3 STRING0 *.*
" *.*" *.* $!

2 STRING0 PATH>              \ Pour remonter au niveau pr‚c‚dent.
" .." PATH> $!

16 CONSTANT IS_DIR           \ Attribut de recherche des sous-r‚pertoires.

255 STRING XCOM$             \ Contient la commande … ‚x‚cuter.

\ Cr‚ation de la DTA.
\ La zone doit ˆtre assez longue pour permettre
\ le balayage des diff‚rents sous-r‚pertoires imbriqu‚s.
: DTA  ( --- adr )
  PAD 512 +
;

VARIABLE INDENT              \ indentation espaces

\ Primitive de changement de r‚pertoire.
: (CHDIR)  ( path --- )
  59 BDOS DROP
;

\ Primitive de parcours r‚cursive de l'arbre des sous-r‚pertoires, et
\ ‚x‚cution de la commande contenue dans la chaine XCOM$ dans chaque
\ sous-r‚pertoire trouv‚.
: (XCOM)  ( --- )
  RECURSIVE
  DMA -ROT                   \ Sauvegarde de la DMA actuelle.
  XCOM$ $EXECUTE
  ROT SET-DMA                \ Restitution de la DMA.
  *.* DROP IS_DIR (SEARCH0)
  IF
    BEGIN
      DMA 21 + C@ IS_DIR AND IS_DIR =
      DMA 30 + C@ ASCII . <> AND
      IF
        CR INDENT @ SPACES
        BOLD .NAME ATTOFF
        DMA 30 + (CHDIR)
        DMA 256 + SET-DMA
        3 INDENT +!
        (XCOM)
        PATH> DROP (CHDIR)
        -3 INDENT +!
        DMA 256 - SET-DMA
      THEN
      (SEARCH) NOT
    UNTIL
  THEN
;

\ Le mot principal.
\ Syntaxe : XCOM commande%
: XCOM  ( <commande> --- )
  DTA SET-DMA
  ASCII % WORD COUNT
  XCOM$ $!
  INDENT OFF
  (XCOM)
;

EOF
\ -----------------------------------------------------------------------------
\ Exemples d'utilisation :

Pour afficher tous les fichier sources FORTH :
  XCOM DIR *.FTH%

Pour effacer tous les fichiers .BAK du disque :
(inclure COMSPEC.FTH si COMMAND.COM ne se trouve pas dans le r‚pertoire
 courant)
  CHDIR \
  XCOM " DEL *.BAK" SHELL%

Pour compter le nombre de fichiers sources FORTH :
  INCLUDE XDIR
  VARIABLE NBFTH
  XCOM XDIR *.FTH DIR.FILES @ NBFTH +!%

