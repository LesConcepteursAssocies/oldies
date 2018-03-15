\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ Cr‚ation de d‚finitions anonymes.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ D‚marre une d‚finition anonyme.
: LAMBDA[  ( --- adr )
  COMPILE BRANCH
  HERE 0 ,
;
IMMEDIATE

\ Termine une d‚finition anonyme.
: ]LAMBDA  ( adr --- adr adrexe )
  COMPILE BRANCH
  HERE 0 ,
  SWAP HERE OVER ! 2+
;
IMMEDIATE

\ Compile l'‚x‚cution de la d‚finition anonyme cr‚‚e.
: >LAMBDA  ( adr adrexe --- )
  COMPILE BRANCH
  ,
  HERE SWAP !
;
IMMEDIATE

EOF
--------------------------------------------------------------------------------
Pouvoir ins‚rer une d‚finition anonyme, pendant la compilation d'une autre
d‚finition, peut s'av‚rer trŠs pratique, et surtout trŠs ‚conomique en espace
m‚moire, car une d‚finition anonyme est compil‚e sans entˆte. D'autre part,
une d‚finition anonyme ‚vite d'emcombrer le dictionnaire avec des d‚finitions
qui ne sont utilis‚es qu'une seule fois.

Exemple type d'utilisation :

: MOT
  ...
  ...                        \ Corps de la d‚finition principale.
  LAMBDA[                    \ Cr‚ation d'une d‚finition anonyme.
    ...
    ...                      \ Corps de la d‚finition anonyme.
  ]LAMBDA                    \ Fin de la d‚finition anonyme.
  ...
  ...                        \ Reste de la d‚finition principale.
  >LAMBDA                    \ Appel de la d‚finition anonyme.
  ...
  ...
;

