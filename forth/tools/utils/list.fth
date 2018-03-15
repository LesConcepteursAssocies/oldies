\ -----------------------------------------------------------------------------
\                  Cr‚ation et manipulation de listes chain‚es.
\
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF

0 CONSTANT NIL

\ Cr‚ation d'une tˆte de liste.
: CREER-LISTE  ( --- )
  CREATE
    NIL ,
;

\ Test liste vide.
: LISTE-VIDE?  ( adrTˆteliste --- adrTˆteliste )
  DUP @ NIL = ABORT" Liste Vide"
;

\ Cr‚ation d'un nouvel ‚l‚ment (noeud) de liste.
\ Empile l'adresse du nouvel ‚l‚ment cr‚‚.
: CREER-NOEUD  ( --- adrNoeud )
  HERE 4 ALLOT
  NIL OVER !
;

\ Insertion d'un noeud dans la liste.
: INSERT-NOEUD  ( adr adrNoeud --- )
  OVER @ OVER ! SWAP !
;

\ EnlŠve le premier noeud de la liste.
: REMOVE-NOEUD  ( adrTˆteliste --- )
  DUP @ DUP
  IF
    @ SWAP !
  ELSE
    2DROP
  THEN
;

\ Ajoute val … la liste donn‚e.
: AJOUTE-LISTE  ( val adrTˆteliste --- )
  CREER-NOEUD
  ROT OVER 2+ !
  INSERT-NOEUD
;

\ Empile la valeur du noeud courant.
: NOEUD@  ( adrNoeud --- val )
  2+ @
;

\ Se place sur le dernier noeud de la liste.
: FIN-LISTE  ( adrTˆteliste --- adrNoeud )
  LISTE-VIDE?
  @                          \ Adresse du premier noeud.
  BEGIN
    DUP @
  WHILE
    @
  REPEAT
;

\ Empile le nombre de noeuds de la liste donn‚e.
: LEN-LISTE  ( adrTˆteliste --- lenliste )
  LISTE-VIDE?
  1 SWAP @                   \ Adresse du premier noeud.
  BEGIN
    DUP @
  WHILE
    @ SWAP 1+ SWAP
  REPEAT
  DROP
;

\ Affichage des valeurs de la liste donn‚e.
: .LISTE  ( adrTˆteliste --- )
  BEGIN
    @ ?DUP
  WHILE
    DUP 2+ @ .
  REPEAT
;

EOF

