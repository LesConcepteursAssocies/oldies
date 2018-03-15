\ -----------------------------------------------------------------------------
\                                Utilitaire divers.
\
\ Inclusion ‚tendue de fichiers sources.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF

EXIST? []STRING0 NOT ?\ INCLUDE ASTRING

DECIMAL

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

20 CONSTANT #MAX-PATH             \ Nombre maxi de chemins de recherche.
51 CONSTANT #LPATH                \ Longueur maxi. d'un chemin.
64 STRING0  #PATHNAME             \ Nom complet du fichier … inclure.

\ Cr‚ation du tableau de recherche.
#MAX-PATH #LPATH 1+ []STRING0 PATH$

VARIABLE PATH#                    \ Indice du premier chemin libre.

\ Ajoute un chemin au tableau.
\ Ajoute le caractŠre "\" … la fin de la chaine, si n‚c‚ssaire.
: >PATH  ( str l --- )
  2DUP 1- + C@
  ASCII \ <>
  IF
    " \" 2OVER APPEND$
    1+
  THEN
  PATH# @ PATH$ $!
  PATH# 1+!
;

\ Affiche la liste des chemins de recherche.
: .PATH  ( --- )
  CR ." Liste des chemins de recherche" CR
  PATH# @ 0
  ?DO
    I PATH$ TYPE CR
  LOOP
;

\ Primitive d'ouverture.
: ((OPEN))  ( str0 --- hdl )
  DUP 0 (OPEN) ?DOS-ERR
  HANDLE @ >R HANDLE !
  1- COUNT PATHNAME!
  LINE# OFF
  HANDLE @ R> HANDLE !
;

\ Primitive d'inclusion.
\ Peut s'utiliser dans une d‚finition.
: (INCLUDE)  ( hdl --- )
  HANDLE @ >R >IN @ >R
  HANDLE !
  BEGIN
    GETLINE
    >IN OFF RUN
    EOF? @
  UNTIL
  CLOSE
  R> >IN !
  R> HANDLE !
;

\ Le programme principal.
\ Principe : Recherche le fichier :
\            - d'abord dans le r‚pertoire courant.
\            - si non trouv‚, dans chacun des r‚pertoires stock‚s dans PATH$.
\
\ Avantage     : Permet une recherche automatique d'un fichier … inclure.
\ Inconv‚nient : Ralenti la compilation.
: INCLUDE  ( <fichier> --- )
  FILENAME
  DUP 1- COUNT #PATHNAME $!
  DUP 0 (SEARCH0)                 \ Recherche dans le r‚pertoire courant.
  IF
    ((OPEN))
    (INCLUDE)
  ELSE
    TRUE SWAP
    PATH# @ 0                     \ Si non trouv‚, recherche dans les
    ?DO                           \ diff‚rents chemins stock‚s.
      I PATH$ #PATHNAME $!        \ On fabrique le nom complet du fichier
      DUP 1- COUNT                \ avec le r‚pertoire courant.
      #PATHNAME APPEND$
      #PATHNAME DROP
      0 (SEARCH0)
      IF
        #PATHNAME DROP
        ((OPEN))
        (INCLUDE)
        SWAP DROP
        FALSE SWAP LEAVE
      THEN
    LOOP
    DROP
    ABORT" Fichier non trouv‚"
  THEN
;
IMMEDIATE

\ Initialisation de mes r‚pertoires de recherche.
" C:\L\TF\PG" >PATH
" C:\L\TF\PG\TOOLS\ASTRO" >PATH
" C:\L\TF\PG\TOOLS\FENETRES" >PATH
" C:\L\TF\PG\TOOLS\FOOD" >PATH
" C:\L\TF\PG\TOOLS\GRAPHISM" >PATH
" C:\L\TF\PG\TOOLS\GRAPHISM\CGA" >PATH
" C:\L\TF\PG\TOOLS\GRAPHISM\EGA" >PATH
" C:\L\TF\PG\TOOLS\GRAPHISM\MCGA" >PATH
" C:\L\TF\PG\TOOLS\GRAPHISM\FORMATS" >PATH
" C:\L\TF\PG\TOOLS\IA" >PATH
" C:\L\TF\PG\TOOLS\JEUX" >PATH
" C:\L\TF\PG\TOOLS\UNDOS" >PATH
" C:\L\TF\PG\TOOLS\UTILS" >PATH
" C:\L\TF\PRGTBF" >PATH
" C:\L\TF" >PATH

EOF

