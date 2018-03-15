;
; COMPILER.INC
; COMPILATEUR PGFORTH
;

MOT@          EXECUTE,101b,7,"EXECUT","E",EXECUTE
              pop       bx
              jmp       [bx]

MOT@          DPUSH,101b,5,"DPUS","H",PAR_CREATE
_2PUSH:       push      dx
_1PUSH:       push      ax
_NEXT:        mov       bx,cs:[si]
              inc       si
              inc       si
              jmp       [bx]

MOT@          APUSH,101b,5,"APUS","H",PAR_CREATE
              jmp       _1PUSH

MOT@          SUP_NEXT,101b,5,">NEX","T",PAR_CREATE
              jmp       _NEXT

MOT@          CODE_REF,101b,8,"CODE-RE","F",PAR_CREATE
_MOV_DX_CODE: mov       dx,code
              ret

MOT@          PERFORM,101b,7,"PERFOR","M",PERFORM
              pop       bx
              mov       bx,[bx]
              jmp       [bx]

MOT@          UNNEST,101b,6,"UNNES","T",UNNEST
              mov       si,[bp]
              inc       bp
              inc       bp
              jmp       _NEXT

MOT@          EXIT,101b,4,"EXI","T",EXIT
              jmp       _UNNEST

MOT@          STORECSP,101b,4,"!CS","P",PAR_2POINT
              dw        _SPFETCH
              dw        _CSP
              dw        _STORE
              dw        _UNNEST

MOT@          INT_CSP,101b,4,"?CS","P",PAR_2POINT
              dw        _SPFETCH
              dw        _CSP
              dw        _FETCH
              dw        _DIFF
              dw        _PAR_ABORTG
STR@          25,<"Structures mal imbriqu‚es">
              dw        _UNNEST

MOT@          ALIGN,111b,5,"ALIG","N",PAR_2POINT
              dw        _HERE
              dw        _PAR_LIT
              dw        1
              dw        _AND
              dw        _INT_BRANCH
              dw        _ALIGN_00
              dw        _BL
              dw        _CVIRGULE
_ALIGN_00:    dw        _UNNEST

MOT@          HIDE,111b,4,"HID","E",PAR_2POINT
              dw        _LAST
              dw        _FETCH
              dw        _DOP
              dw        _N_TO_LINK
              dw        _FETCH
              dw        _SWAP
              dw        _CURRENT
              dw        _FETCH
              dw        _HASH
              dw        _STORE
              dw        _UNNEST

MOT@          REVEAL,111b,6,"REVEA","L",PAR_2POINT
              dw        _LAST
              dw        _FETCH
              dw        _DOP
              dw        _N_TO_LINK
              dw        _SWAP
              dw        _CURRENT
              dw        _FETCH
              dw        _HASH
              dw        _STORE
              dw        _UNNEST

MOT@          PAR_2POINT,101b,3,"(:",")",PAR_2POINT
              inc       bx
              inc       bx
              dec       bp
              dec       bp
              mov       [bp],si
              mov       si,bx
              jmp       _NEXT

MOT@          2POINT,101b,1,"",":",PAR_2POINT
              dw        _STORECSP
              dw        _CURRENT
              dw        _FETCH
              dw        _CONTEXT
              dw        _STORE
              dw        _CREATE
              dw        _HIDE
              dw        _COMTIME
              dw        _UNNEST

MOT@          COMTIME,101b,1,"","]",PAR_2POINT
              dw        _STATE
              dw        _ON
_COMTIME0:    dw        _INT_STACK
              dw        _DEFINED
              dw        _DOP
              dw        _INT_BRANCH
              dw        _COMTIME3
              dw        _0INF
              dw        _INT_BRANCH
              dw        _COMTIME1
              dw        _EXECUTE
              dw        _BRANCH
              dw        _COMTIME2
_COMTIME1:    dw        _VIRGULE
_COMTIME2:    dw        _BRANCH
              dw        _COMTIME5
              ;
              ; Numeriques
              ;
_COMTIME3:    dw        _DROP
              dw        _NUMBER
              dw        _DOUBLE_INT
              dw        _INT_BRANCH
              dw        _COMTIME4
              dw        _DLITERAL
              dw        _BRANCH
              dw        _COMTIME5
_COMTIME4:    dw        _DROP
              dw        _LITERAL
_COMTIME5:    dw        _TRUE
              dw        _DONE_INT
              dw        _INT_BRANCH
              dw        _COMTIME0
              dw        _UNNEST

MOT@          RUNTIME,111b,1,"","[",PAR_2POINT
              dw        _STATE
              dw        _OFF
              dw        _UNNEST

MOT@          ALLOT,101b,5,"ALLO","T",PAR_2POINT
              dw        _DP
              dw        _PSTORE
              dw        _UNNEST

MOT@          VIRGULE,101b,1,"",",",PAR_2POINT
              dw        _HERE
              dw        _STORE
              dw        _PAR_LIT
              dw        2
              dw        _ALLOT
              dw        _UNNEST

MOT@          CVIRGULE,101b,2,"C",",",PAR_2POINT
              dw        _HERE
              dw        _CSTORE
              dw        _PAR_LIT
              dw        1
              dw        _ALLOT
              dw        _UNNEST

MOT@          COMPILE,101b,7,"COMPIL","E",PAR_2POINT
              dw        _R_SUP
              dw        _DOP
              dw        _2PLUS
              dw        _SUP_R
              dw        _FETCH
              dw        _VIRGULE
              dw        _UNNEST

MOT@          IMMEDIATE,101b,9,"IMMEDIAT","E",PAR_2POINT
              dw        _PAR_LIT
              dw        64
              dw        _LAST
              dw        _FETCH
              dw        _CTOGGLE
              dw        _UNNEST

MOT@          EVEN,101b,4,"EVE","N",PAR_2POINT
              dw        _DOP
              dw        _PAR_LIT
              dw        1
              dw        _AND
              dw        _PLUS
              dw        _UNNEST

MOT@          GUIL_CREATE,101b,7,'"CREAT','E',PAR_2POINT
              ;
              ; Transfert du nom
              ;
              dw        _COUNT
              dw        _ALIGN
              dw        _HERE
              dw        _PAR_LIT
              dw        2
              dw        _PLUS
              dw        _PLACE
              ;
              ; Pr‚paration du LFA
              ;
              dw        _PAR_LIT
              dw        0
              dw        _VIRGULE
              ;
              ; LAST pointe sur le nouveau NFA
              ;
              dw        _HERE
              dw        _LAST
              dw        _STORE
              ;
              ; D‚tection doublon
              ;
              dw        _WARNING
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _GUI_C1
              dw        _HERE
              dw        _FIND
              dw        _INT_BRANCH
              dw        _GUI_C0
              dw        _BOLD
              dw        _HERE
              dw        _COUNT
              dw        _TYPE
              dw        _PAR_PGUI
STR@          17,<" n'est pas unique">
              dw        _ATTOFF
              dw        _CR
_GUI_C0:      dw        _DROP
              ;
              ; Modification des liaisons
              ;
_GUI_C1:      dw        _HERE
              dw        _CURRENT
              dw        _FETCH
              dw        _HASH
              dw        _DOP
              dw        _FETCH
              dw        _HERE
              dw        _2MOINS
              dw        _STORE              ; LFA cr‚‚
              dw        _HERE
              dw        _2MOINS
              dw        _SWAP
              dw        _STORE              ; THREAD modifi‚
              ;
              ; Compilation du NFA
              ;
              dw        _HERE
              dw        _DOP
              dw        _CFETCH
              dw        _WIDTH
              dw        _FETCH
              dw        _MIN
              dw        _1PLUS
              dw        _ALLOT
              ;
              ; Formattage premier et dernier octet NFA
              ;
              dw        _PAR_LIT
              dw        160
              dw        _SWAP
              dw        _CSET
              dw        _PAR_LIT
              dw        128
              dw        _HERE
              dw        _1MOINS
              dw        _CSET
              ;
              ; CFA est (CREATE)
              ;
              dw        _ALIGN
              dw        _PAR_LIT
              dw        0
              dw        _VIRGULE
              dw        _PAR_PVIRUSES
              dw        _PAR_CREATE

MOT@          PAR_CREATE,101b,8,"(CREATE",")",PAR_CREATE
              inc       bx
              inc       bx
              push      bx
              jmp       _NEXT

MOT@          CREATE,101b,6,"CREAT","E",PAR_2POINT
              dw        _BL
              dw        _WORD
              dw        _INT_UPPERCASE
              dw        _GUIL_CREATE
              dw        _UNNEST

MOT@          PAR_CONSTANT,101b,10,"(CONSTANT",")",PAR_CONSTANT
              inc       bx
              inc       bx
              mov       ax,[bx]
              jmp       _1PUSH

MOT@          CONSTANT,101b,8,"CONSTAN","T",PAR_2POINT
              dw        _CREATE
              dw        _VIRGULE
              dw        _PAR_PVIRUSES
              dw        _PAR_CONSTANT

MOT@          PAR_2CONSTANT,101b,11,"(2CONSTANT",")",PAR_2CONSTANT
              mov       ax,[bx+2]
              mov       dx,[bx+4]
              jmp       _2PUSH

MOT@          PAR_VARIABLE,101b,10,"(VARIABLE",")",PAR_VARIABLE
              inc       bx
              inc       bx
              push      bx
              jmp       _NEXT

MOT@          VARIABLE,101b,8,"VARIABL","E",PAR_2POINT
              dw        _CREATE
              dw        _PAR_LIT
              dw        0
              dw        _VIRGULE
              dw        _PAR_PVIRUSES
              dw        _PAR_VARIABLE

MOT@          PAR_2VARIABLE,101b,11,"(2VARIABLE",")",PAR_2VARIABLE
              inc       bx
              inc       bx
              push      bx
              jmp       _NEXT

MOT@          PAR_DEFER,101b,7,"(DEFER",")",PAR_DEFER
              inc       bx
              inc       bx
              mov       bx,[bx]
              jmp       [bx]

MOT@          PAR_IS,101b,4,"(IS",")",PAR_2POINT
              dw        _R_FETCH
              dw        _FETCH
              dw        _SUP_BODY
              dw        _STORE
              dw        _R_SUP
              dw        _2PLUS
              dw        _SUP_R
              dw        _UNNEST

MOT@          PAR_LIT,101b,5,"(LIT",")",PAR_LIT
              mov       ax,cs:[si]
              inc       si
              inc       si
              jmp       _1PUSH

MOT@          PAR_DLIT,101b,6,"(DLIT",")",PAR_DLIT
              mov       ax,cs:[si]
              inc       si
              inc       si
              push      ax
              mov       ax,cs:[si]
              inc       si
              inc       si
              jmp       _1PUSH

MOT@          LITERAL,111b,7,"LITERA","L",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_LIT
              dw        _VIRGULE
              dw        _UNNEST

MOT@          DLITERAL,111b,8,"DLITERA","L",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_DLIT
              dw        _SWAP
              dw        _VIRGULE
              dw        _VIRGULE
              dw        _UNNEST

MOT@          PAR_PGUI,101b,4,'(."',')',PAR_2POINT
              dw        _R_SUP
              dw        _COUNT
              dw        _2DOP
              dw        _PLUS
              dw        _EVEN
              dw        _SUP_R
              dw        _TYPE
              dw        _UNNEST

MOT@          VIR_GUI,101b,2,',','"',PAR_2POINT
              dw        _PAR_LIT
              dw        '"'
              dw        _PARSE
              dw        _TUCK
              dw        _HERE
              dw        _PLACE
              dw        _1PLUS
              dw        _ALLOT
              dw        _ALIGN
              dw        _UNNEST

MOT@          HERE,101b,4,"HER","E",PAR_2POINT
              dw        _DP
              dw        _FETCH
              dw        _UNNEST

MOT@          PAR_PVIRUSES,101b,7,"(;USES",")",PAR_2POINT
              dw        _R_SUP
              dw        _FETCH
              dw        _FETCH
              dw        _LAST
              dw        _FETCH
              dw        _NAME_SUP
              dw        _STORE
              dw        _UNNEST

MOT@          PVIRUSES,111b,5,";USE","S",PAR_2POINT
              dw        _INT_CSP
              dw        _PAR_LIT
              dw        _PAR_2POINT
              dw        _FETCH
              dw        _LAST
              dw        _FETCH
              dw        _NAME_SUP
              dw        _STORE
              dw        _COMPILE
              dw        _PAR_PVIRUSES
              dw        _TICK
              dw        _RUNTIME
              dw        _REVEAL
              dw        _UNNEST

MOT@          PAR_DOES,101b,7,"(DOES>",")",PAR_CREATE
              dec       bp
              dec       bp
              mov       [bp],si
              pop       si
              mov       ax,si
              and       ax,1
              add       si,ax
              inc       bx
              inc       bx
              push      bx
              jmp       _NEXT

MOT@          PAR_VOC,101b,5,"(VOC",")",PAR_VOC
              inc       bx
              inc       bx
              lea       di,_CONTEXT+2
              mov       [di],bx
              jmp       _NEXT

;
; Directives de compilation des structures de controle
;

MOT@          BRANCH,101b,6,"BRANC","H",BRANCH
_BRANCH_00:   mov       si,[si]
              jmp       _NEXT

MOT@          INT_BRANCH,101b,7,"?BRANC","H",INT_BRANCH
              pop       ax
              or        ax,ax
              je        _BRANCH_00
              inc       si
              inc       si
              jmp       _NEXT

MOT@          PAR_DO,101b,4,"(DO",")",PAR_DO
              pop       ax
              pop       bx
_PD0:         dec       bp
              dec       bp
              mov       dx,[si]
              mov       [bp],dx
              inc       si
              inc       si
              add       bx,8000h
              dec       bp
              dec       bp
              mov       [bp],bx
              sub       ax,bx
              dec       bp
              dec       bp
              mov       [bp],ax
              jmp       _NEXT

MOT@          PAR_INT_DO,101b,5,"(?DO",")",PAR_INT_DO
              pop       ax
              pop       bx
              cmp       bx,ax
              jne       _PD0
              mov       si,[si]
              jmp       _NEXT

MOT@          PAR_LOOP,101b,6,"(LOOP",")",PAR_LOOP
              mov       ax,1
_PLOOP_00:    add       [bp],ax
              jo        _PLOOP_01
              jmp       _BRANCH_00
_PLOOP_01:    add       bp,6
              inc       si
              inc       si
              jmp       _NEXT

MOT@          PAR_PLOOP,101b,7,"(+LOOP",")",PAR_PLOOP
              pop       ax
              jmp       _PLOOP_00

MOT@          PAR_LEAVE,101b,7,"(LEAVE",")",PAR_LEAVE
_PAR_LEAV00:  add       bp,4
              mov       si,[bp]
              inc       bp
              inc       bp
              jmp       _NEXT

MOT@          PAR_INT_LEAVE,101b,8,"(?LEAVE",")",PAR_INT_LEAVE
              pop       ax
              or        ax,ax
              jnz       _PAR_LEAV00
              jmp       _NEXT
      
MOT@          INT_LEAVE,111b,6,"?LEAV","E",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_INT_LEAVE
              dw        _UNNEST

MOT@          I,101b,1,"","I",I
              mov       ax,[bp]
              add       ax,[bp+2]
              jmp       _1PUSH

MOT@          CRO_COMPILE,111b,9,"[COMPILE","]",PAR_2POINT
              dw        _TICK
              dw        _VIRGULE
              dw        _UNNEST

;
; Acces aux segments
;
MOT@          CSEGMENT,101b,8,"CSEGMEN","T",CSEGMENT
              call      _MOV_DX_CODE
              push      dx
              jmp       _NEXT

MOT@          DSEGMENT,101b,8,"DSEGMEN","T",DSEGMENT
              call      _MOV_DX_CODE
              push      dx
              jmp       _NEXT

MOT@          ESEGMENT,101b,8,"ESEGMEN","T",ESEGMENT
              call      _MOV_DX_CODE
              add       dx,2000h
              push      dx
              jmp       _NEXT

MOT@          SSEGMENT,101b,8,"SSEGMEN","T",SSEGMENT
              call      _MOV_DX_CODE
              add       dx,1000h
              push      dx
              jmp       _NEXT

;
; FIN DE COMPILER.INC
;

