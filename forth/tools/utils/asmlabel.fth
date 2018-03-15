\ -----------------------------------------------------------------------------
\                            Utilitaire pour l'ASSEMBLEUR.
\
\ Cr‚ation et utilisation de labels.
\ (adapt‚ d'une id‚e de R. L. Smith)
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

WARNING OFF
ECHO OFF

DECIMAL

\ Nombre maxi de labels.
30 CONSTANT #LABELS

\ Cr‚ation de la table des labels.
CREATE ASM-LABELS
  #LABELS 4 * ALLOT

\ Etend le signe d'un octet.
HEX
: EXBYTE  ( n --- n' )
  DUP 80 AND
  IF
    FF00 OR
  THEN
;
DECIMAL

\ Remise … z‚ro de la table des labels.
: RAZ-LABELS  ( --- )
  ASM-LABELS #LABELS 4 * 0 FILL
;

\ Renvoi l'adresse du niŠme label.
: LABEL@  ( n --- adr )
  DUP #LABELS 1- U> ABORT" LABEL@: Label hors limite"
  2* 2* ASM-LABELS +
;

\ Fait r‚f‚rence au niŠme label.
\ La label peut ˆtre r‚f‚renc‚ avant et/ou aprŠs sa d‚finition.
: $  ( n --- adr )
  LABEL@ DUP @
  IF
    @
  ELSE
    2+ DUP @
    SWAP HERE 2+ SWAP !
    DUP 0=
    IF
      HERE 2+ +
    THEN
  THEN
;

\ R‚solution d'un label.
: $RESOLVE  ( adr --- )
  @ DUP 0=
  IF
    DROP EXIT
  THEN
  0
  BEGIN
    + DUP 1- C@
    OVER HERE OVER -
    SWAP 1- C!
    EXBYTE DUP 0=
  UNTIL
  2DROP
;

\ D‚finition d'un label.
: $:  ( n --- )
  LABEL@ DUP 2+ $RESOLVE
  0 OVER 2+ !
  HERE SWAP !
;

EOF
--------------------------------------------------------------------------------
Exemple type d'utilisation :
CODE TRUC
RAZ-LABELS
       ...
       n1 $ JB                    \ R‚f‚rence avant
       ...
       n2 $ JA                    \ R‚f‚rence avant
       ...
       ...
n1 $:  AX BX ADD                  \ D‚finition du label n1
       ...
       ...
       n1 $ #) JMP                \ R‚f‚rence arriŠre
n2 $:  ...                        \ D‚finition du label n2
END-CODE

Exemple concret :

CODE 0=  ( n --- fl )
     RAZ-LABELS
     AX POP
     AX AX OR
     1 $ JNE
     TRUE # AX MOV
     2 $ #) JMP
1 $: FALSE # AX MOV
2 $: 1PUSH
END-CODE

