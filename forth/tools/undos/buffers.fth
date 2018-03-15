\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ R‚cupŠre le nombre de buffers allou‚s.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? OSVER NOT ?\ INCLUDE MSDOS
EXIST? DIB NOT ?\ INCLUDE DIB
EXIST? L2@ NOT ?\ INCLUDE 2MEMSEG

DECIMAL

\ Empile l'adresse du premier buffer.
\ 0000:0000 si la version du D.O.S ne le permet pas (osmajor < 2 ou > 4).
: (BUFFERS)  ( --- seg off )
  (DIB)
  OSMAJOR
  CASE 2 OF
           19 +
         ENDOF
       3 OF
           OSMINOR
           0>
           IF
             18 +                 \ DOS 3.1 - 3.3
           ELSE
             19 +                 \ DOS 3.0
           THEN
         ENDOF
       4 OF
           63 +                   \ DOS 4.x
         ENDOF
         2DROP 0.                 \ DOS < 2 ou > 4
  ENDCASE
;

\ Le programme principal.
: BUFFERS  ( --- n )
  (BUFFERS) 2DUP D0= ABORT" Version du DOS non valide"
  OSMAJOR 4 <
  IF
    1 -ROT
    BEGIN
      L2@ DUP -1 U<
    WHILE
      ROT 1+ -ROT
    REPEAT
    2DROP
  ELSE
    L@                            \ BUFFERS= directement dans le DIB … partir
  THEN                            \ de D.O.S 4.x
;

EOF
Structure d'un buffer D.O.S :

Seg:Off+00 : Pointeur sur le prochain buffer (Off=FFFFh si c'est le dernier).
Seg:Off+04 : Code du drive (0=A,1=B,..., FF si non utilis‚)
Seg:Off+05 : Octet de Flags
