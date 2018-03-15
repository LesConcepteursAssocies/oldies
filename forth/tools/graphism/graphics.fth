\ ------------------------------------------------------------------------------
\         Sur-couche, d‚finitions des vecteurs des fonctions graphiques.
\ 
\ Par Guillaumaud Philippe
\ ------------------------------------------------------------------------------

DECIMAL

\ Chargement des primitives.
EXIST? (CGA-ECRAN)  NOT ?\ INCLUDE CGA_LIB
EXIST? (EGA-ECRAN)  NOT ?\ INCLUDE EGA_LIB
EXIST? (MCGA-ECRAN) NOT ?\ INCLUDE MCGA_LIB

\ Sauvegarde de l'‚cran dans l'‚cran virtuel.
DEFER ECRAN->*ECRAN

\ Restitution de l'‚cran virtuel dans l'‚cran.
DEFER *ECRAN->ECRAN

\ Ecriture d'un point dans l'‚cran virtuel.
DEFER PSET-V

\ Sauvegarde du contenu de l'‚cran sur disque.
DEFER ECRAN->DISQUE

\ Copie le contenu d'un fichier image dans l'‚cran virtuel
DEFER DISQUE->*ECRAN

\ Capture d'une portion de l'‚cran.
DEFER @IMAGE

\ Restitution d'une portion de l'‚cran.
DEFER !IMAGE

\ Scrolling horizontal.
DEFER HSCROLL

\ Compactage d'images.
DEFER COMPACTE

\ D‚compactage d'images.
DEFER DCOMPACTE

\ Nombre maximum de couleurs disponibles dans le mode courant.
0 CONSTANT COULEURS-MAX

\ Initialisation des modes graphiques.
: CGA  ( --- )
  5 GRMODE !
  ['] (CGA-ECR->*ECR) IS ECRAN->*ECRAN
  ['] (CGA-*ECR->ECR) IS *ECRAN->ECRAN
  ['] (CGA-PSET) IS PSET-V
  ['] (CGA-ECR->DSQ) IS ECRAN->DISQUE
  ['] (CGA-DSQ->*ECR) IS DISQUE->*ECRAN
  ['] (@IMAGE) IS @IMAGE
  ['] (!IMAGE) IS !IMAGE
  ['] NOOP IS HSCROLL
  ['] NOOP IS COMPACTE
  ['] NOOP IS DCOMPACTE
  2 IS COULEURS-MAX
  (CGA-*INIT)
;

: EGA  ( --- )
  13 GRMODE !
  ['] (EGA-ECR->*ECR) IS ECRAN->*ECRAN
  ['] (EGA-*ECR->ECR) IS *ECRAN->ECRAN
  ['] (EGA-PSET) IS PSET-V
  ['] (EGA-ECR->DSQ) IS ECRAN->DISQUE
  ['] (EGA-DSQ->*ECR) IS DISQUE->*ECRAN
  ['] (EGA-@IMAGE) IS @IMAGE
  ['] (EGA-!IMAGE) IS !IMAGE
  ['] (EGA-HSCROLL) IS HSCROLL
  ['] (EGA-COMPACTE) IS COMPACTE
  ['] (EGA-DCOMPACTE) IS DCOMPACTE
  16 IS COULEURS-MAX
  (EGA-*INIT)
;

: MCGA  ( --- )
  19 GRMODE !
  ['] (MCGA-ECR->*ECR) IS ECRAN->*ECRAN
  ['] (MCGA-*ECR->ECR) IS *ECRAN->ECRAN
  ['] (MCGA-PSET) IS PSET-V
  ['] (MCGA-ECR->DSQ) IS ECRAN->DISQUE
  ['] (MCGA-DSQ->*ECR) IS DISQUE->*ECRAN
  ['] (MCGA-@IMAGE) IS @IMAGE
  ['] (MCGA-!IMAGE) IS !IMAGE
  ['] (MCGA-HSCROLL) IS HSCROLL
  ['] (MCGA-COMPACTE) IS COMPACTE
  ['] (MCGA-DCOMPACTE) IS DCOMPACTE
  255 IS COULEURS-MAX
  (MCGA-*INIT)
;

EOF

