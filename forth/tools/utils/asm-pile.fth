\ -----------------------------------------------------------------------------
\                            Utilitaire pour l'ASSEMBLEUR.
\
\ Cadre de pile et variables locales.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

WARNING OFF
ECHO OFF

ASSEMBLER ALSO

\ GÇnäre un cadre de pile.
: >FRAME  ( --- )
  BP PUSH
  SP BP MOV
;

\ Libäre le cadre de pile.
: FRAME>  ( --- )
  BP POP
;

\ Allocation de variables locales.
\ Alloue n valeurs (16 bits) sur la pile pour des variables locales.
: >LOCALS  ( n --- )
  >FRAME
  2* [ ASSEMBLER ] # [ FORTH ] SP SUB
;

\ DÇsallocation des variables locales.
: LOCALS>  ( --- )
  BP SP MOV
  FRAME>
;

\ Accäs aux variables locales.
\ Pointe la niäme variable locale.
: LOCAL@  ( n --- )
  2* NEGATE [BP]
;

\ RÇcupÇration non destructive d'un paramätre sur la pile.
\ Pointe le niäme paramätre (n=1 pour le sommet de la pile).
: PARAM@  ( n --- )
  2* [BP]
;

\ Retire n valeurs (16 bits) de la pile.
: -PARAM  ( n --- )
  2* [ ASSEMBLER ] # [ FORTH ] SP ADD
;

EOF
\ -----------------------------------------------------------------------------
\ Exemples d'utilisation :

CODE NEW-DUP  ( n --- n n )
  >FRAME
  1 PARAM@ AX MOV            \ AX = Sommet de la pile
  FRAME>
  1PUSH
END-CODE

CODE NEW-OVER  ( n1 n2 --- n1 n2 n1 )
  >FRAME
  2 PARAM@ AX MOV            \ AX = n1
  FRAME>
  1PUSH
END-CODE

CODE NEW-DROP  ( n --- )
  1 -PARAM
  NEXT
END-CODE

CODE NEW-NIP  ( n1 n2 --- n2 )
  AX POP
  1 -PARAM
  1PUSH
END-CODE

CODE NEW-SWAP  ( n1 n2 --- n2 n1 )
  1 >LOCALS
  1 PARAM@ AX MOV
  AX 1 LOCAL@ MOV
  2 PARAM@ AX MOV
  AX 1 PARAM@ MOV
  1 LOCAL@ AX MOV
  AX 2 PARAM@ MOV
  LOCALS>
  NEXT
END-CODE

\ Addition de trois nombres.
CODE 3ADD  ( n1 n2 n3 --- n1+n2+n3 )
  1 >LOCALS
  1 PARAM@ AX MOV            \ AX = n3
  AX 1 LOCAL@ MOV
  2 PARAM@ AX MOV
  1 LOCAL@ AX ADD            \ AX = n3+n2
  AX 1 LOCAL@ MOV
  3 PARAM@ AX MOV
  1 LOCAL@ AX ADD            \ AX = n3+n2+n1
  LOCALS>
  3 -PARAM                   \ On retire les paramätres de la pile.
  1PUSH
END-CODE

