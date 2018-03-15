\ -----------------------------------------------------------------------------
\                              Utilitaire disques.
\
\ Recherche d'une chaine dans un fichier.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ Balaye le fichier courant, et affiche toutes les lignes
\ contenant la chaine adr l.
: (TS)  ( adr l --- adr l )
  BEGIN                      \ adr l ---
    GETLINE 2DUP             \ adr l adr l ---
    BUFFER COUNT SEARCH      \ adr l pos fl ---
    IF
      DROP
      CR ." Ligne " LINE# ? SPACE
      BUFFER COUNT TYPE
    ELSE
      DROP
    THEN
    EOF? @
  UNTIL
;

\ Le programme principal.
: TXSEARCH  ( adr l <fichier> --- )
  OPEN
  HANDLE @ >R HANDLE !
  CR ." Fichier : " PATHNAME C/PATH TYPE
  (TS) 2DROP
  CLOSE
  R> HANDLE !
;

EOF

