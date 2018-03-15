\ -----------------------------------------------------------------------------
\                   D‚tection de la carte graphique pr‚sente.
\
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
HEX

\ Teste la pr‚sence d'une carte VGA.
\ Principe : on ‚x‚cute la fonction 1A, sp‚cifique du B.I.O.S V.G.A.
\            Si au retour, le registre AL contient 1A, alors on est
\            en pr‚sence d'une carte V.G.A.
CODE VGA?  ( --- flag )
  1A00 # AX MOV
  10 INT
  1A # AL CMP
  0=
  IF
    -1 # AX MOV
    1PUSH
  THEN
  AX AX XOR
  1PUSH
END-CODE

\ Teste la pr‚sence d'une carte EGA.
\ Principe : on ‚x‚cute la fonction 12, sous-fonction 10, sp‚cifique du B.I.O.S
\            E.G.A. Si au retour, le registre BL ne contient plus 10, alors on
\            est en pr‚sence d'une carte E.G.A.
CODE EGA?  ( --- flag )
  12 # AH MOV
  10 # BL MOV
  10 INT
  10 # BL CMP
  0=
  IF
    AX AX XOR
    1PUSH
  THEN
  -1 # AX MOV
  1PUSH
END-CODE

\ Teste la pr‚sence d'un contr“leur vid‚o 6845.
\ utilis‚ pour les tests CGA et monochrome (MDA,HGC)
CODE 6845?  ( adr_port --- flag )
  DX POP                     \ Adresse de port
  0A # AL MOV                \ Appeler le registre nø 10
  0 AL OUT
  DX INC                     \ Pointer sur le registre de donn‚es
  0 AL IN                    \ Lire le contenu du registre nø 10
  AL AH MOV                  \ On conserve l'ancienne valeur
  4F # AL MOV                \ On ‚crit une valeur quelconque
  0 AL OUT                   \ dans le registre nø 10
  100
  DO                         \ Boucle de temporisation
  LOOP                       \ pour laisser le temps au contr“leur
  0 AL IN                    \ On relit le contenu du registre nø 10
  AH AL XCHG
  0 AL OUT                   \ On restitue l'ancienne valeur
  4F # AH CMP                \ a t'on lu la mˆme chose ?
  0=
  IF
    -1 # AX MOV
    1PUSH
  THEN
  AX AX XOR
  1PUSH
END-CODE

\ Teste la pr‚sence d'une carte CGA.
: CGA?  ( --- flag )
  03D4 6845?
;

\ Teste la pr‚sence d'une carte monochrome (MDA, ou HGC).
: MDA-HGC?  ( --- flag )
  03B4 6845?
;

DECIMAL
EOF
--------------------------------------------------------------------------------
Rappel des diff‚rents modes graphiques CGA, EGA et VGA :

Mode    Carte    R‚solution    Nb. Couleurs      Taille d'une image
 4       CGA      320x200           4                16 Ko.
 5       CGA      320x200           4                16 Ko.
 6       CGA      640x200           2                16 Ko.
13       EGA      320x200          16                32 Ko.
14       EGA      640x200          16                64 Ko.
15       EGA      640x350          mono              56 Ko.
16       EGA      640x350          16               112 Ko.
17      MCGA      640x480           2                38 Ko.
18       VGA      640x480          16               156 Ko.
19      MCGA      320x200         256                64 Ko.

