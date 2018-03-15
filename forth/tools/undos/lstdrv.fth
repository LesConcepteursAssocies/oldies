\ -----------------------------------------------------------------------------
\                            La face cach‚e du D.O.S.
\
\ R‚cupŠre le code du dernier drive logique (LASTDRIVE).
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? OSMAJOR NOT ?\ INCLUDE MSDOS
EXIST? (DIB) NOT ?\ INCLUDE DIB

DECIMAL

\ Empile le code du dernier lecteur logique.
\ (0=A, 1=B, ...)
\ -1 si la version du D.O.S ne le permet pas (osmajor < 2 ou > 4).
: LASTDRIVE  ( --- n )
  (DIB)
  OSMAJOR
  CASE 2 OF
           16 + LC@               \ DOS 2.x
         ENDOF
       3 OF
           OSMINOR
           0>
           IF
             33 + LC@             \ DOS 3.1 - 3.3
           ELSE
             27 + LC@             \ DOS 3.0
           THEN
         ENDOF
       4 OF
           33 + LC@               \ DOS 4.x
         ENDOF
         2DROP -1                 \ DOS < 2 ou > 4
  ENDCASE
;

EOF
Voici la fa‡on "document‚e" d'obtenir le code du dernier drive logique :

HEX
CODE LASTDRIVE  ( --- n )
  19 # AH MOV                     \ R‚cupŠre le code du lecteur courant.
  21 INT
  AL DL MOV
  0E # AH MOV                     \ S‚lectionne le lecteur sp‚cifi‚ dans DL
  21 INT                          \ comme lecteur par d‚faut.
  AH AH XOR                       \ Au retour, AL = LASTDRIVE.
  1PUSH
END-CODE
DECIMAL

Le problŠme, c'est que le mot pr‚c‚dent ne fonctionne pas si la machine est
une station Novell, dans ce cas, 32 est toujours renvoy‚ (ce nombre correspond
au nombre de drive logiques "mappables" par une station (A … Z, plus [, ],
\, ^, _, et `).

