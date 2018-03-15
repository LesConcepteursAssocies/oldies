\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ Etude de la strat‚gie d'allocation m‚moire du D.O.S.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? MALLOC NOT ?\ INCLUDE MEMORY

HEX

\ Initialise la strat‚gie d'allocation m‚moire.
\ n = [0..2]
CODE STRAT!  ( n --- )
  BX POP
  5801 # AX MOV
  21 INT
  NEXT
END-CODE

\ Empile le code de strat‚gie d'allocation courant.
CODE STRAT@  ( --- n )
  5800 # AX MOV
  21 INT
  1PUSH
END-CODE

DECIMAL

\ Quelques essais.
VARIABLE BLOCK1
VARIABLE BLOCK2

: STRAT?  ( n --- str )
  CR ." Strat‚gie d'allocation courante : "
  STRAT@ DUP . SPACE
  CASE 0 OF
           ." (First-Fit)"
         ENDOF
       1 OF
           ." (Best-Fit) "
         ENDOF
       2 OF
           ." (Last-Fit) "
         ENDOF
  ENDCASE
;

: STRATST  ( --- )
  CR ." Tests d'allocation m‚moire"
  CR 26 ASCII - REPLICATE
  CR ." Allocation de 4096 Octets"
  4096 MALLOC DUP BLOCK1 !
  ." , Segment : " HEX U. DECIMAL
  CR ." Allocation de 2048 Octets"
  2048 MALLOC                     \ Un bloc entre les deux
  ." , Segment : " HEX U. DECIMAL
  CR ." Allocation de 2048 Octets"
  2048 MALLOC DUP BLOCK2 !
  ." , Segment : " HEX U. DECIMAL
  CR ." Allocation de 2048 Octets"
  2048 MALLOC                     \ Un autre bloc
  ." , Segment : " HEX U. DECIMAL
  BLOCK1 @
  CR ." Lib‚ration du bloc : "
  DUP HEX U. DECIMAL RELEASE
  BLOCK2 @
  CR ." Lib‚ration du bloc : "
  DUP HEX U. DECIMAL RELEASE      \ Lib‚ration des deux blocs
  CR 26 ASCII - REPLICATE
  0 STRAT!
  STRAT?
  CR ." Allocation de 2048 Octets"
  2048 MALLOC
  ." , Segment : " DUP HEX U. DECIMAL
  CR ." Lib‚ration du bloc : "
  DUP HEX U. DECIMAL RELEASE
  CR 26 ASCII - REPLICATE
  1 STRAT!
  STRAT?
  CR ." Allocation de 2048 Octets"
  2048 MALLOC
  ." , Segment : " DUP HEX U. DECIMAL
  CR ." Lib‚ration du bloc : "
  DUP HEX U. DECIMAL RELEASE
  CR 26 ASCII - REPLICATE
  2 STRAT!
  STRAT?
  CR ." Allocation de 2048 Octets"
  2048 MALLOC
  ." , Segment : " DUP HEX U. DECIMAL
  CR ." Lib‚ration du bloc : "
  DUP HEX U. DECIMAL RELEASE
  0 STRAT!
;

EOF

