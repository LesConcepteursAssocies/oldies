\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ D‚terminer le type du processeur pr‚sent.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

\ ECHO OFF

HEX

\ Empile un code indiquant le uP pr‚sent
\ 0 = 8088
\ 1 = 8086
\ 2 = V20
\ 3 = V30
\ 4 = 80188
\ 5 = 80186
\ 6 = 80286
\ 7 = 80386
CODE MP?  ( --- n )
  PUSHF
  AX AX XOR
  AX PUSH
  POPF
  PUSHF
  AX POP
  F0 # AX AND
  F0 # AX CMP           \ Les bits 12 … 15 du registre des flags = 1 ?
  0<>
  IF                    \ Oui, il s'agit alors d'un 286 ou d'un 386.
    6 # DL MOV
    7000 # AX MOV
    AX PUSH
    POPF
    PUSHF
    AX POP
    7000 # AX AND       \ Masquage des bits sauf 12 … 14
    0<>
    IF                  \ Si les bits 12 … 14 <> 0, c'est un 80386
      DL INC
    THEN
    POPF
    DH DH XOR
    DX AX MOV
    1PUSH
  THEN
  5 # DL MOV
  FF # AL MOV
  21 # CL MOV
  AL CL SHR             \ D‚cale AL CL fois vers la droite
  0=
  IF                    \ Si AL = 0, ce n'est pas un 80188 ou 80186
    2 # DL MOV
    STI                 \ On autorise les interruptions
    SI PUSH
    0 # SI MOV
    FFFF # CX MOV
    ES:
    REP AL LODS         \ Fonctionne seulement avec un NEC V20 ou V30
    SI POP
    CX CX OR            \ Le segment …-t-il ‚t‚ entiŠrement lu ?
    0<>
    IF                  \ Non, c'est un 8088 ou un 8086
      0 # DL MOV
    THEN
  THEN
  CS PUSH               \ On d‚termine s'il s'agit d'un x88 ou x86 ou V20 ou V30
  ES POP
  STD
  HERE 11 + # DI MOV    \ Pointe sur le "STI" avant le "POPF"
  FB # AL MOV           \ Code de "STI"
  3 # CX MOV
  CLI
  REP AL STOS
  CLD
  NOP                   \ On rempli la file d'attente du processeur
  NOP                   \ avec des instructions fictives.
  NOP
  DX INC
  NOP
  STI
  POPF
  DH DH XOR
  DX AX MOV
  1PUSH
END-CODE

DECIMAL

\ Routine de test
: QUEL-MP  ( --- )
  MP?
  CASE 0 OF ." 8088" ENDOF
       1 OF ." 8086" ENDOF
       2 OF ." NEC V20" ENDOF
       3 OF ." NEC V30" ENDOF
       4 OF ." 80188" ENDOF
       5 OF ." 80186" ENDOF
       6 OF ." 80286" ENDOF
       7 OF ." 80386" ENDOF
  ENDCASE
;

EOF

