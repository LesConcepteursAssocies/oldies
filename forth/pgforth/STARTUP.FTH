\
\ Fichier STARTUP
\
CR .( Lecture du fichier STARTUP.FTH )
CSEGMENT IS SEGMENT>

\ Chargement de l'utilitaire de gestion des touches de fonction

CR .( Assignation des touches de fonction )
IFNDEF FUNCTION INCLUDE PROGS\FUNCTAB.FTH

\ assignation de quelques touches
 1 ASSIGN INCLUDE PROGS\$      \ Touche F1
 2 ASSIGN SAVE-SYSTEM $        \ Touche F2
 3 ASSIGN WORDS%$              \ Touche F3
 4 ASSIGN SHELL" DIR PROGS"%$  \ Touche F4

\ Activation des couleurs
COLOR? ON
CYAN
