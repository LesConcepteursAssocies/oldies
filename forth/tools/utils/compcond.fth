\ -----------------------------------------------------------------------------
\                      Compilation conditionnelle ‚volu‚e.
\ 
\ 
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF

\ Compile tout ce qui ce trouve entre le #IF et le #ELSE ou le #THEN,
\ si le flag pr‚sent sur la pile est VRAI.
: #IF  ( fl --- fl )
  DUP FALSE =
  IF
    BEGIN
      GETLINE
      " #ELSE"               \ adr l ---
      BUFFER COUNT SEARCH    \ pos fl ---
      IF
        >IN ! TRUE
      ELSE
        DROP " #THEN"        \ adr l ---
        BUFFER COUNT SEARCH  \ pos fl ---
        IF
          >IN ! TRUE
        ELSE
          DROP FALSE
        THEN
      THEN
      EOF? @ OR
    UNTIL
  THEN
;
IMMEDIATE

: #ELSE  ( fl --- fl )
  DUP FALSE = NOT
  IF
    BEGIN
      GETLINE
      " #THEN"
      BUFFER COUNT SEARCH
      IF
        >IN ! TRUE
      ELSE
        DROP FALSE
      THEN
      EOF? @ OR
    UNTIL
  THEN
;
IMMEDIATE

: #THEN  ( fl --- )
  DROP
;
IMMEDIATE


\ Tests imm‚diats !
EXIST? DUP NOT
#IF
  : NO-DUP  ( --- )
    CR ." DUP n'existe pas !" CR
  ;
#THEN

\ Autre exemple
2 2 =
#IF
  : 2=2  ( --- )
    ." 2 est egal … 2 !"
  ;
#ELSE
  ceci ne sera pas compil‚ (!)
  : 2<>2  ( --- )
    ." 2 est diff‚rent de 2 !"
  ;
#THEN

EOF

