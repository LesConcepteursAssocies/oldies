\ -----------------------------------------------------------------------------
\                           Remplacement du CONTROL-C.
\
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? INT@ NOT ?\ INCLUDE MSDOS

HEX

2VARIABLE OLDCTRLC           \ ancien vecteur de Control-C.
23 INT@ OLDCTRLC 2!

\ Nouvelle interruption de Control-C.
LABEL NEWCTRLC
  DSEGMENT # DX MOV
  STI
  PUSHF
  DX PUSH
  ' ABORT # BX MOV
  DS POP
  DS PUSH
  0 [BX] PUSH
  20 # AL MOV                \ Accus‚ de r‚ception au 8259.
  20 # DX MOV
  0 AL OUT
  IRET                       \ Effectue un saut dans ABORT.

\ Activation du Control-C.
: CONTROL-C  ( --- )
  23 INT@ OLDCTRLC 2!
  DSEGMENT NEWCTRLC 23 INT!
;

\ D‚sactivation du Control-C.
: NOCONTROL-C  ( --- )
  OLDCTRLC 2@ 23 INT!
;

DECIMAL
EOF
L'interruption 23h est ‚x‚cut‚e … chaque appui sur les touches CTRL + C, o—
CTRL + Break.

