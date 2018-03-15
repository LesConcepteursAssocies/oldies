\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ AccŠs aux variables systŠmes du B.I.O.S
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

HEX

\ Adresse de segment des variables du B.I.O.S
0040 CONSTANT #BIOS-SEG

\ Empile l'adresse de base du 1er port s‚rie, ou 0 si non pr‚sent.
: #COM1  ( --- adr )
  #BIOS-SEG 0 L@
;

\ Empile l'adresse de base du 2Šme port s‚rie, ou 0 si non pr‚sent.
: #COM2  ( --- adr )
  #BIOS-SEG 2 L@
;

\ Empile l'adresse de base du 3Šme port s‚rie, ou 0 si non pr‚sent.
: #COM3  ( --- adr )
  #BIOS-SEG 4 L@
;

\ Empile l'adresse de base du 4Šme port s‚rie, ou 0 si non pr‚sent.
: #COM4  ( --- adr )
  #BIOS-SEG 6 L@
;

\ Empile l'adresse de base du 1er port parallŠle, ou 0 si non pr‚sent.
: #LPT1  ( --- adr )
  #BIOS-SEG 8 L@
;

\ Empile l'adresse de base du 2Šme port parallŠle, ou 0 si non pr‚sent.
: #LPT2  ( --- adr )
  #BIOS-SEG 0A L@
;

\ Empile l'adresse de base du 3Šme port parallŠle, ou 0 si non pr‚sent.
: #LPT3  ( --- adr )
  #BIOS-SEG 0C L@
;

\ Empile l'adresse de base du 4Šme port parallŠle, 0 si non pr‚sent.
\ Si l'on est sur un micro de type PS, empile l'adresse de segment
\ du segment de donn‚es ‚tendue du BIOS.
: #LPT4  ( --- adr )
  #BIOS-SEG 0E L@
;

\ Empile un mot correspondant … la configuration mat‚rielle install‚e.
\ Format : Bits 14 - 15 = Nombre de ports parallŠles pr‚sents
\          Bit       13 = Modem interne pr‚sent (sur CONVERTIBLE)
\          Bit       12 = R‚serv‚
\          Bits 11 -  9 = Nombre de p‚riph‚riques s‚ries pr‚sents.
\          Bit        8 = R‚serv‚
\          Bits 7  -  6 = Nombre de lecteurs de disquettes - 1.
\          Bits 5  -  4 = Mode vid‚o initial
\                         00 = EGA,VGA,PGA
\                         01 = 40 x 25, couleur
\                         10 = 80 x 25, couleur
\                         11 = 80 x 25, mono
\          Bits 3  -  2 = R‚serv‚s
\          Bit        1 = Coprocesseur math‚matique (1 pr‚sent,0 sinon)
\          Bit        0 = ‚gal … 1 si un lecteur de disquette est disponible
\                         pour le boot.
: #CONFIG  ( --- n )
  #BIOS-SEG 10 L@
;

\ Empile la m‚moire de base pr‚sente, en Ko (0 - 640).
: #MEM  ( --- n )
  #BIOS-SEG 13 L@
;

\ Empile le 1er octet de status du clavier.
\ Format : Bit 7 = 1 si mode INSert actif
\          Bit 6 = 1 si caps lock actif
\          Bit 5 = 1 si num lock actif
\          Bit 4 = 1 si scroll lock actif
\          Bit 3 = 1 si touche Alt press‚e
\          Bit 2 = 1 si touche Ctrl press‚e
\          Bit 1 = 1 si touche shift gauche press‚e
\          Bit 0 = 1 si touche shift droit press‚e
: #KBD-STATUS1  ( --- o )
  #BIOS-SEG 17 LC@
;

\ Empile le 2Šme octet de status du clavier.
\ Format : Bit 7 = 1 si touche INSert press‚e
\          Bit 6 = 1 si touche caps lock press‚e
\          Bit 5 = 1 si touche num lock press‚e
\          Bit 4 = 1 si touche scroll lock press‚e
\          Bit 3 = 1 si pause active
\          Bit 2 = 1 si touche sys req press‚e (AT)
\          Bit 1 = 1 si touche Alt gauche press‚e
\          Bit 0 = 1 si touche Ctrl gauche press‚e
: #KBD-STATUS2  ( --- o )
  #BIOS-SEG 18 LC@
;

\ Empile l'adresse du prochain car. dans le buffer clavier.
: NEXT-CAR  ( --- adr )
  #BIOS-SEG 1A L@
;

\ Empile l'adresse du premier emplacement libre dans le buffer clavier.
: >KBD-BUFF  ( --- adr )
  #BIOS-SEG 1C L@
;

\ adresse (segment offset) du buffer circulaire du clavier.
\ (longueur = 16 mots).
: #KBD-BUFF  ( --- seg off )
  #BIOS-SEG 1E
;

\ Empile l'octet de status de recalibrage des lecteurs de disquette.
\ Format : Bit      7 = 1 si interruption mat‚rielle disquette survenue.
\          Bits 6 - 4 = R‚serv‚s
\          Bit      3 = 1 si recalibrage lecteur 3
\          Bit      2 = 1 si recalibrage lecteur 2
\          Bit      1 = 1 si recalibrage lecteur 1
\          Bit      0 = 1 si recalibrage lecteur 0
: #RSTAT-FDD  ( --- o )
  #BIOS-SEG 3E LC@
;

\ Empile l'octet de status des moteurs des lecteurs de disquettes.
\ Format : Bit      7 = 1 si l'op‚ration courante est une ‚criture ou un formatage
\                       0 si l'op‚ration courante est une lecture ou une v‚rification
\          Bit      6 = r‚serv‚
\          Bits 5 - 4 = Num‚ro de lecteur s‚lectionn‚ (0 … 3)
\          Bit      3 = 1 si le moteur du lecteur 3 est on
\          Bit      2 = 1 si le moteur du lecteur 2 est on
\          Bit      1 = 1 si le moteur du lecteur 1 est on
\          Bit      0 = 1 si le moteur du lecteur 0 est on
: #MSTAT-FDD  ( --- o )
  #BIOS-SEG 3F LC@
;

\ Empile l'octet de status de la derniŠre op‚ration survenue sur un lecteur
\ de disquette.
\ Format : Octet = 0, Ok, sinon
\          Bit      7 = 1 si lecteur non pret
\          Bit      6 = 1 si erreur de positionnement (seek)
\          Bit      5 = 1 si erreur g‚n‚rale du contr“leur
\          Bits 4 - 0 = 00, pas d'erreur
\                       01, requŠte invalide
\                       02, marqueur d'adresse non trouv‚
\                       03, protection en ‚criture
\                       04, secteur non trouv‚
\                       06, changement de disquette
\                       08, "DMA overrun"
\                       09, "DMA across 64k boundary"
\                       0C, type du support inconnu
\                       10, erreur de CRC … la lecture
: #OSTAT-FDD  ( --- o )
  #BIOS-SEG 41 LC@
;

\ Empile le mode vid‚o courant.
: #VMODE  ( --- o )
  #BIOS-SEG 49 LC@
;

\ Empile le nombre de colonnes de l'‚cran.
: #VCOL  ( --- n )
  #BIOS-SEG 4A L@
;

\ Empile la taille d'une page vid‚o, en octets.
: #PAGE-SIZE  ( --- n )
  #BIOS-SEG 4C L@
;

\ Empile l'adresse de d‚part de la page vid‚o courante.
: #PAGE  ( --- adr )
  #BIOS-SEG 4E L@
;

\ Empile l'adresse (segment offset) de la zone contenant la position du curseur
\ (col, lig) pour huit pages.
: #CURPOS  ( --- seg off )
  #BIOS-SEG 50
;

\ Empile les lignes de d‚but et de fin du curseur.
\ octet haut : d‚but, octet bas : fin.
: #CUR-FRAME  ( --- n )
  #BIOS-SEG 60 L@
;

\ Empile le num‚ro de la page courante.
: PAGE#  ( --- n )
  #BIOS-SEG 62 LC@
;

\ Empile l'adresse de base du contr“leur vid‚o.
\ en g‚n‚ral, 03D4 = couleur, 03B4 = mono
: #CRT  ( --- adr )
  #BIOS-SEG 63 L@
;

\ Empile l'octet de status de la derniŠre op‚ration survenue sur un disque
\ dur.
\ Format : Octet = 0, Ok, sinon
\                 01, requŠte invalide
\                 02, marqueur d'adresse non valide
\                 03, protection en ‚criture
\                 04, secteur non trouv‚
\                 05, erreur de reset
\                 07, erreur des paramŠtres d'activit‚ du lecteur
\                 08, "DMA overrun"
\                 09, "DMA data boundary error"
\                 0A, flag "bad sector" d‚tect‚
\                 0B, flag "bad track" d‚tect‚
\                 0D, nombre de secteurs invalides pour le formatage
\                 0E, marqeur d'adresse de contr“le de donn‚es d‚tect‚
\                 0F, niveau d'arbitrage DMA hors limite
\                 10, erreur ECC, ou CRC non corrigible
\                 11, erreur ECC de correction de donn‚es
\                 20, erreur g‚n‚rale du contr“leur
\                 40, erreur de positionnement (seek)
\                 80, erreur de time out
\                 AA, lecteur non pret
\                 BB, erreur non d‚finie
\                 CC, erreur d'‚criture sur le lecteur s‚lectionn‚
\                 E0, le status du registre error/error est 0
\                 FF, "sense failed"
: #OSTAT-HDD  ( --- o )
  #BIOS-SEG 74 LC@
;

\ Empile le nombre de disques durs pr‚sents.
: #HDD  ( --- n )
  #BIOS-SEG 75 LC@
;

\ Empile l'octet de configuration des cartes EGA/VGA.
\ Format : Bit      7 = 1 si pas d'effacement de la RAM vid‚o
\                       (voir INT 10h, fonction 0)
\          Bits 6 - 5 = RAM disponible sur la carte
\                       (quantit‚ = ce champ +1 * 64K)
\          Bit      4 = R‚serv‚
\          Bit      3 = 0 si systŠme EGA/VGA actif
\          Bit      2 = R‚serv‚
\          Bit      1 = 0 si moniteur couleur ou ECD, 1 si moniteur mono
\          Bit      0 = 0 si l'‚mulation du curseur en mode texte est active
: #EGA/VGA-CONF  ( --- o )
  #BIOS-SEG 87 LC@
;

DECIMAL

EOF

