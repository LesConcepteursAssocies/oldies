\ -----------------------------------------------------------------------------
\                         Extensions math‚matiques.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

\ ECHO OFF

DECIMAL

\ Multiplication 32 bits non sign‚e.
\ Test de vitesse : 1000 multiplications 32 bits en 0,05 sec. sur 80386 16 Mhz
CODE UD*  ( ud1 ud2 --- ud3 )
  DX POP
  AX POP
  BX POP
  CX POP
  DI PUSH
  SI PUSH
  BX DI MOV
  CX SI MOV
  AX CX MOV
  DX AX MOV
  SI MUL
  AX BX MOV
  CX AX MOV
  DI MUL
  AX BX ADD
  CX AX MOV
  SI MUL
  BX DX ADD
  SI POP
  DI POP
  DX AX XCHG
  2PUSH
END-CODE

\ Division 32 bits non sign‚e.
\ Test de vitesse : 1000 divisions 32 bits en 0,11 sec. sur 80386 16 Mhz
CODE UD/  ( ud1 ud2 --- ud3 )
  BX POP
  CX POP
  DX POP
  AX POP
  DI PUSH
  SI PUSH
  BP PUSH
  BX DI MOV
  CX SI MOV
  BX BX XOR
  BP BP XOR
  AX RCL
  DX RCL
  32 
  DO
    BX RCL
    BP RCL
    SI BX SUB
    DI BP SBB
    U<
    IF
      SI BX ADD
      DI BP ADC
    THEN
    AX RCL
    DX RCL
  LOOP
  AX NOT
  DX NOT
  DX AX XCHG
  BP POP
  SI POP
  DI POP
  2PUSH
END-CODE

\ Empile (sous la forme d'un double non sign‚), la factorielle du double
\ non sign‚ donn‚.
: UDN!  ( ud --- udn! )
  RECURSIVE
  2DUP OR
  IF
    2DUP 1. D-
    UDN! UD*
  ELSE
    2DROP 1.
  THEN
;

EOF

