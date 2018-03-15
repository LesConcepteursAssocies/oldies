\ -----------------------------------------------------------------------------
\                            Utilitaires divers.
\
\ Op‚rations binaires.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

\ Outils n‚c‚ssaires pour LBMOVE.
EXIST? >FRAME NOT ?\ INCLUDE ASM-PILE
EXIST? ASM-LABELS NOT ?\ INCLUDE ASMLABEL
DECIMAL

\ Empile la valeur du 'b'iŠme bit de n.
\ b = [0..15]
CODE B@  ( n b --- n' )
  CX POP
  AX POP
  AX CL SHR
  1 # AX AND
  1PUSH
END-CODE

\ Initialise la valeur du 'b'iŠme bit de n avec vb.
\ b  = [0..15]
\ vb = [0..1]
CODE B!  ( n vb b --- n' )
  CX POP
  BX POP
  AX POP
  BX CL SHL
  BX AX OR
  1PUSH
END-CODE

\ Empile la valeur des bits de n, … partir de b1, sur une longueur b2.
\ b1 = [0..15]
\ b2 = [0..15]
CODE BITS@  ( n b1 b2 --- n' )
  CX POP
  DX POP
  AX POP
  BX BX XOR
  HERE                       \ Fabrication du masque.
    BX SHL
    1 # BX OR
  LOOP
  DX CX MOV
  AX CL SHR
  BX AX AND
  1PUSH
END-CODE

\ Empile un nombre dont les bits … partir de b1 sur une longueur b2, ont la
\ valeur de ceux de n.
\ b1 = [0..15]
\ b2 = [0..15]
CODE XBITS@  ( n b1 b2 --- n' )
  CX POP
  DX POP
  AX POP
  BX BX XOR
  HERE                       \ Fabrication du masque.
    BX SHL
    1 # BX OR
  LOOP
  DX CX MOV
  BX CL SHL
  BX AX AND
  1PUSH
END-CODE

\ D‚calage … droite double 16 bits.
\ La primitive.
\ D‚cale … droite DX de CX bits, en remplacant ses bits de poids fort par
\ les bits de poids faible de AX.
\
\ En sortie, AX et CX sont inchang‚s, DX contient la valeur modifi‚e.
LABEL (SHRD)  ( --- )
  BX PUSH
  CX PUSH
  AX PUSH
  15 # CX AND                \ Autorise seulement des d‚calages de 0 … 15.
  DX CL SHR
  16 # BX MOV
  CX BX SUB
  BX CX MOV                  \ CX = 16 - CX
  AX CL SHL
  AX DX OR
  AX POP
  CX POP
  BX POP
  RET

\ D‚cale … droite n1 de cnt bits, en remplacant ses bits de poids fort par
\ les bits de poids faible de n2.
\ cnt = [0..15]
CODE SHRD  ( n1 n2 cnt --- n1' )
  CX POP
  AX POP
  DX POP
  (SHRD) #) CALL
  DX AX XCHG
  1PUSH
END-CODE

\ D‚calage … gauche double 16 bits.
\ La primitive.
\ D‚cale … gauche DX de CX bits, en remplacant ses bits de poids faible par
\ les bits de poids fort de AX.
\
\ En sortie, AX et CX sont inchang‚s, DX contient la valeur modifi‚e.
LABEL (SHLD)  ( --- )
  BX PUSH
  CX PUSH
  AX PUSH
  15 # CX AND                \ Autorise seulement des d‚calages de 0 … 15.
  DX CL SHL
  16 # BX MOV
  CX BX SUB
  BX CX MOV                  \ CX = 16 - CX
  AX CL SHR
  AX DX OR
  AX POP
  CX POP
  BX POP
  RET

\ D‚cale … gauche n1 de cnt bits, en remplacant ses bits de poids faible par
\ les bits de poids fort de n2.
\ cnt = [0..15]
CODE SHLD  ( n1 n2 cnt --- n1' )
  CX POP
  AX POP
  DX POP
  (SHLD) #) CALL
  DX AX XCHG
  1PUSH
END-CODE

\ Copie de chaines binaires extra-segment.
\ Copie la chaine segs offs … partir du bit bs, dans la chaine segd offd
\ … partir du bit bd, sur une longueur lb.
\ segs,offs : adresse de la chaine source.
\ bs        : nø du bit … partir duquel commence la copie ([0..15]).
\ segd,offd : adresse de la chaine destination.
\ bd        : nø du bit … partir duquel commence le stockage ([0..15]).
\ lb        : longueur, en bits, de la copie.
CODE LBMOVE  ( segs offs bs segd offd bd lb --- )
     RAZ-LABELS
     1 >LOCALS               \ On a besoin d'une variable locale (un flag).
     CLD
     SI PUSH
     DS PUSH
     ES PUSH
     AX AX XOR
     AX 1 LOCAL@ MOV         \ Mise … z‚ro du flag.
     7 PARAM@ AX MOV
     AX DS MOV               \ DS = segs.
     6 PARAM@ SI MOV         \ DS:SI = source.
     4 PARAM@ AX MOV
     AX ES MOV               \ ES = segd.
     3 PARAM@ DI MOV         \ ES:DI = destination.
     5 PARAM@ CX MOV         \ CX = bs.
\
\ Pr‚lude. Aligne la destination sur une frontiŠre de mot.
\
     0 [SI] DX MOV
     2 [SI] AX MOV
     (SHRD) #) CALL          \ DX contient les 16 premiers bits de l'origine.
     ES: 0 [DI] AX MOV       \ Prend les bits restants
     2 PARAM@ CX MOV         \ et les transfŠre dans le mot
     AX CL ROR               \ de la destination.
     (SHLD) #) CALL
     16 # BX MOV             \ Teste le cas o— la chaine entiŠre
     2 PARAM@ BX SUB         \ est contenue dans un mot unique.
     1 PARAM@ BX SUB         \ Si elle est contenue dans le premier mot
     1 $ JBE
     16 # CX MOV             \ de la destination, on saute … la fin
     BX CX SUB               \ de la routine qui inclura tous les
     CX BX MOV               \ bits restants
     7 $ #) JMP              \ de poids fort.
1 $: DX ES: 0 [DI] MOV       \ Range le premier r‚sultat.
     8 $ JE                  \ Rentre exactement dans le premier mot de la destination.
     2 # DI ADD
     5 PARAM@ CX SUB         \ Calcule le nombre correct de d‚calages.
     2 $ JL                  \ Si bs > bd alors CL = bs-bd,
     16 # BL MOV             \ sinon, CL = bd-bs
     CL BL SUB
     BL CL MOV
     3 $ #) JMP
2 $: 5 PARAM@ CX MOV
     2 PARAM@ CX SUB
     2 # SI ADD              \ Puisque bd <= bs la routine a trait‚e tous
     AX PUSH                 \ les premiers mots de l'origine.
     1 # AX MOV
     AX 1 LOCAL@ MOV         \ Flag indiquant que bs > bd.
     AX POP
\
\ Corps de la routine LBMOVE.
\ A ce stade, la destination est align‚e sur une frontiŠre de 16 bits.
\ CL contient le nombre de d‚calages, SI et DI pointent … l'endroit correct
\ pour commencer le transfert.
\
3 $: 1 PARAM@ BX MOV
     2 PARAM@ BX ADD
     16 # BX SUB             \ BX contient le nombre de bits … transf‚rer.
     0 [SI] DX MOV
     2 # SI ADD
\
\ La boucle de LBMOVE.
\ TransfŠre 16 bits depuis l'origine vers la destination … chaque
\ it‚ration.
\ La boucle s'arrˆte quand il reste moins de 16 bits … transf‚rer.
\
4 $: 16 # BX SUB
     5 $ JLE
     AX LODS
     (SHRD) #) CALL          \ AX = partie align‚e de destination.
     DX AX XCHG              \ DX = partie restante de l'origine pour la
     AX STOS                 \ prochaine it‚ration.
     4 $ #) JMP
\
\ Postlude. Partie finale de LBMOVE.
\ Transfert des ‚ventuels bits restants.
\
5 $: 8 $ JE                  \ Si pas de bits … transf‚rer, alors fin !.
     AX PUSH
     1 LOCAL@ AX MOV
     1 # AX CMP
     AX POP
     6 $ JE
     AX LODS
     (SHRD) #) CALL
     7 $ #) JMP
6 $: DX CL SHR
\
\ A ce stade, DX contient les bits restants de la chaine d'origine align‚s
\ sur la destination.
\
7 $: ES: 0 [DI] AX MOV
     BL CL MOV
     DX CL ROR
     AX CL ROR
     16 # BL MOV
     CL BL SUB
     BL CL MOV
     (SHRD) #) CALL          \ Effectue le d‚calage final en placant la
     DX AX XCHG              \ destination aprŠs la fin de la chaine binaire.
     AX STOS
\
\ Fin de LBMOVE. Elimination des paramŠtres.
\
8 $: ES POP
     DS POP
     SI POP
     LOCALS>
     7 -PARAM
     NEXT
END-CODE

\ Copie de chaines binaires inter-segment.
\ Copie la chaine adrs … partir du bit bs, dans la chaine adrd … partir
\ du bit bd, sur une longueur lb.
\ adrs : adresse de la chaine source.
\ bs   : nø du bit … partir duquel commence la copie ([0..15]).
\ adrd : adresse de la chaine destination.
\ bd   : nø du bit … partir duquel commence le stockage ([0..15]).
\ lb   : longueur, en bits, de la copie.
: BMOVE  ( adrs bs adrd bd lb --- )
  >R >R >R DSEGMENT -ROT     \ segs adrs bs ---
  DSEGMENT R> R> R>          \ segs adrs bs segd adrd bd lb ---
  LBMOVE
;

ONLY FORTH ALSO DEFINITIONS
EOF
--------------------------------------------------------------------------------
Exemple d'utilisation de BMOVE:

CREATE TST1 10 ALLOT TST1 10 ERASE
CREATE TST2 10 ALLOT TST2 10 ERASE

56334 TST1 !
45    TST1 2+ C!

TST1 @    B. 1101110000001110
TST1 1+ @ B.   10110111011100

TST1 1+ 2 TST2 0 12 BMOVE

TST2 @    B.     101101110111

