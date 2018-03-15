; PGFORTH.INC
;
; D‚finition des macros de PGFORTH
;--------------------------------------------------------------------

EVEN@         macro     LON
              if        (LON and 01h) eq 0
              db        " "
              endif
              endm

;--------------------------------------------------------------------

DEFNIL@       macro
;
@NIL          dw        0
@LNK_0        =         @NIL
@LNK_1        =         @NIL
@LNK_2        =         @NIL
@LNK_3        =         @NIL
@VOCLNK       =         @NIL
              endm

;--------------------------------------------------------------------

MKLFA@        macro     LON
;
; Cr‚ation d'un LFA avec gestion des THREADS.
;
              even
@THREAD       =         (LON and 3)
              if        @THREAD eq 0
              dw        @LNK_0
@LNK_0        =         $-2
              else
              if        @THREAD eq 1
              dw        @LNK_1
@LNK_1        =         $-2
              else
              if        @THREAD eq 2
              dw        @LNK_2
@LNK_2        =         $-2
              else
              if        @THREAD eq 3
              dw        @LNK_3
@LNK_3        =         $-2
              endif
              endif
              endif
              endif
@LSTLFA       =         $-2
              endm

;--------------------------------------------------------------------

MOT@          macro     CFA, DEB, LON, NOM, FIN, RUN
;
; G‚n‚ration d'un MOT.
;
; CFA  est le nom du label assembleur.
; DEB  contient les trois premiers bits du NFA.
; LON  est la longueur du mot.
; NOM  est le nom du mot.
; FIN  est la derniŠre lettre du nom du mot.
; RUN  est le pointeur vers le code d'‚x‚cution.
;
MKLFA@        LON
              db        (&DEB shl 5)+LON
              db        &NOM,&FIN+80h
EVEN@         LON
_&CFA:
              dw        _&RUN+2
              endm

;--------------------------------------------------------------------

VOC@          macro     CFA, DEB, LON, NOM, FIN
;
; G‚n‚ration d'un VOCABULAIRE.
;
; CFA  est le nom du label assembleur.
; DEB  contient les trois premiers bits du NFA.
; LON  est la longueur du mot.
; NOM  est le nom du mot.
; FIN  est la derniŠre lettre du nom du mot.
;
MKLFA@        LON
              db        (&DEB shl 5)+LON
              db        &NOM,&FIN+80h
EVEN@         LON
_&CFA:
              dw        _PAR_VOC+2
              dw        @FINLNK_0, @FINLNK_1, @FINLNK_2, @FINLNK_3
              dw        @VOCLNK
@VOCLNK       =         $-2
              endm

;--------------------------------------------------------------------

STR@          macro     LON,STR
;
; G‚n‚ration d'une chaine FORTH.
;
; LON  est la longueur de la chaine.
; STR  est la chaine.
;
              db        &LON,&STR
              even
              endm

;--------------------------------------------------------------------

FIN@          macro
;
; Ces symboles sont d‚finis … la fin de la compilation
; pour initialiser des variables FORTH.
;
              even
@HERE         =         $
@FINVOC       =         @VOCLNK
@FINLFA       =         @LSTLFA
              endm

;--------------------------------------------------------------------

FINVOC@       macro
;
; Ces symboles sont d‚finis … la fin de la compilation
; d'un vocabulaire pour initialiser les THREADS.
;
@FINLNK_0     =         @LNK_0
@FINLNK_1     =         @LNK_1
@FINLNK_2     =         @LNK_2
@FINLNK_3     =         @LNK_3
              endm

;--------------------------------------------------------------------
; FIN DE PGFORTH.INC
; 
