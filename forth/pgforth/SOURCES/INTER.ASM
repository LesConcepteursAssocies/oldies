;
; INTER.INC
; INTERPRETATION DU TEXTE
;

MOT@          REG_SS,101b,6,"SS-RE","G",PAR_CONSTANT
$REG_SS       dw        ?

MOT@          REG_SP,101b,6,"SP-RE","G",PAR_CONSTANT
$REG_SP       dw        ?

MOT@          REG_BP,101b,6,"BP-RE","G",PAR_CONSTANT
$REG_BP       dw        ?

MOT@          ENTRY_POINT,101b,5,"ENTR","Y",PAR_CREATE
ENTRY:        push      ds
              ;
              ; Remplacement du ^C
              ;
              xor       dx,dx
              mov       ds,dx
              mov       bx,4*23h
              mov       [bx],offset CONTROL_C
              call      _MOV_DX_CODE
              mov       [bx+2],dx
              ;
              ; Initialisation de PGFORTH
              ;
              call      _MOV_DX_CODE
              mov       ds,dx
              mov       ax,ss
              mov       [$REG_SS],ax
              mov       ax,sp
              mov       [$REG_SP],ax
              mov       ax,bp
              mov       [$REG_BP],ax
              add       dx,1000h
              mov       ss,dx
              mov       sp,0FFFEh
              mov       bp,07FFEh
              ;
              ; Saut dans COLD
              ;
              lea       bx,_COLD
              jmp       [bx]

MOT@          INT_CONTROL_C,101b,9,"CONTROL-","C",PAR_CREATE
CONTROL_C:    call      _MOV_DX_CODE
              mov       ds,dx
              mov       ax,dx
              add       ax,1000h
              mov       ss,ax
              sti
              pushf
              push      dx
              lea       bx,_ABORT
              push      [bx]
              mov       al,20h
              out       20h,al
              iret

MOT@          NOOP,101b,4,"NOO","P",NOOP
              jmp       _NEXT

MOT@          ABORT,101b,5,"ABOR","T",PAR_DEFER
              dw        _NOOP

MOT@          PAR_INT_ERROR,101b,8,"(?ERROR",")",PAR_2POINT
              dw        _INT_BRANCH
              dw        _ERROR_01
              dw        _PRINTING
              dw        _OFF
              dw        _BOLD
              dw        _TYPE
              dw        _ATTOFF
              dw        _SP0
              dw        _FETCH
              dw        _SPSTORE
              dw        _HANDLE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _ERROR_00
              dw        _PAR_PGUI
STR@          17,<" dans le fichier ">
              dw        _PATHNAME
              dw        _COUNT
              dw        _BOLD
              dw        _TYPE
              dw        _ATTOFF
              dw        _PAR_PGUI
STR@          8,<", ligne ">
              dw        _LINE_SHA
              dw        _FETCH
              dw        _BOLD
              dw        _POINT
              dw        _ATTOFF
              dw        _HANDLE
              dw        _OFF
_ERROR_00:    dw        _ALLCLOSE
              dw        _QUIT
              dw        _BRANCH
              dw        _ERROR_02
_ERROR_01:    dw        _2DROP
_ERROR_02:    dw        _UNNEST

MOT@          INT_ERROR,101b,6,"?ERRO","R",PAR_DEFER
              dw        _PAR_INT_ERROR

MOT@          PAR_ABORTG,101b,8,'(ABORT"',')',PAR_2POINT
              dw        _R_FETCH
              dw        _COUNT
              dw        _ROT
              dw        _INT_ERROR
              dw        _R_SUP
              dw        _COUNT
              dw        _PLUS
              dw        _EVEN
              dw        _SUP_R
              dw        _UNNEST

MOT@          ABORT_GUI,111b,6,'ABORT','"',PAR_2POINT
              dw        _COMPILE
              dw        _PAR_ABORTG
              dw        _VIR_GUI
              dw        _UNNEST

MOT@          WARM,101b,4,"WAR","M",PAR_2POINT
              dw        _TRUE
              dw        _PAR_ABORTG
STR@          20,"DÇmarrage du systäme"
              dw        _UNNEST

MOT@          BOOT,101b,4,"BOO","T",PAR_DEFER
              dw        _NOOP

MOT@          COLD,101b,4,"COL","D",PAR_2POINT
              dw        _DTA
              dw        _SET_DTA
              dw        _BOOT
              dw        _WARM
              dw        _QUIT
              dw        _UNNEST

MOT@          QUIT,101b,4,"QUI","T",PAR_2POINT
              dw        _TIB
              dw        _TICK_TIB
              dw        _STORE
              dw        _RUNTIME
_QUIT0:       dw        _RP0
              dw        _FETCH
              dw        _RPSTORE
              dw        _STATUS
              dw        _QUERY
              dw        _RUN
              dw        _STATE
              dw        _FETCH
              dw        _NOT
              dw        _INT_BRANCH
              dw        _QUIT1
              dw        _BOLD
              dw        _PROMPT
              dw        _1PLUS
              dw        _COUNT
              dw        _TYPE
              dw        _ATTOFF
_QUIT1:       dw        _BRANCH
              dw        _QUIT0
              dw        _UNNEST

MOT@          STATUS,101b,6,"STATU","S",PAR_DEFER
              dw        _NOOP

MOT@          INT_STACK,101b,6,"?STAC","K",PAR_2POINT
              dw        _SPFETCH
              dw        _SP0
              dw        _FETCH
              dw        _SUP
              dw        _PAR_ABORTG
STR@          16,<"La pile est vide">
              dw        _RPFETCH
              dw        _RP0
              dw        _FETCH
              dw        _SUP
              dw        _PAR_ABORTG
STR@          26,<"La pile de retour est vide">
              dw        _SPFETCH
              dw        _RP0
              dw        _FETCH
              dw        _PAR_LIT
              dw        256
              dw        _PLUS
              dw        _INF
              dw        _PAR_ABORTG
STR@          18,<"La pile est pleine">
              dw        _RPFETCH
              dw        _PAR_LIT
              dw        256
              dw        _INF
              dw        _PAR_ABORTG
STR@          28,<"La pile de retour est pleine">
              dw        _UNNEST

MOT@          DONE_INT,101b,5,"DONE","?",PAR_2POINT
              dw        _STATE
              dw        _FETCH
              dw        _DIFF
              dw        _END_INT
              dw        _FETCH
              dw        _OR
              dw        _END_INT
              dw        _OFF
              dw        _UNNEST

MOT@          TRAVERSE,101b,8,"TRAVERS","E",TRAVERSE
              pop       cx
              pop       bx
              add       bx,cx
_TRAVERSE_00: mov       al,[bx]
              and       al,128
              jnz       _TRAVERSE_01
              add       bx,cx
              jmp       _TRAVERSE_00
_TRAVERSE_01: push      bx
              jmp       _NEXT

MOT@          N_TO_LINK,101b,6,"N>LIN","K",PAR_2POINT
              dw        _2MOINS
              dw        _UNNEST

MOT@          NAME_SUP,101b,5,"NAME",">",PAR_2POINT
              dw        _PAR_LIT
              dw        1
              dw        _TRAVERSE
              dw        _1PLUS
              dw        _EVEN
              dw        _UNNEST

MOT@          SUP_BODY,101b,5,">BOD","Y",PAR_2POINT
              dw        _2PLUS
              dw        _UNNEST

MOT@          RUN,101b,3,"RU","N",PAR_2POINT
              dw        _STATE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _RUN_01
              dw        _COMTIME
              dw        _STATE
              dw        _FETCH
              dw        _NOT
              dw        _INT_BRANCH
              dw        _RUN_00
              dw        _INTERPRET
_RUN_00:      dw        _BRANCH
              dw        _RUN_02
_RUN_01:      dw        _INTERPRET
_RUN_02:      dw        _UNNEST

MOT@          INTERPRET,101b,9,"INTERPRE","T",PAR_2POINT
_INTERPRET2:  dw        _INT_STACK
              dw        _DEFINED
              dw        _INT_BRANCH
              dw        _INTERPRET0
              dw        _EXECUTE
              dw        _BRANCH
              dw        _INTERPRET1
_INTERPRET0:  dw        _NUMBER
              dw        _DOUBLE_INT
              dw        _NOT
              dw        _INT_BRANCH
              dw        _INTERPRET1
              dw        _DROP
_INTERPRET1:  dw        _FALSE
              dw        _DONE_INT
              dw        _INT_BRANCH
              dw        _INTERPRET2
              dw        _UNNEST

MOT@          HASH,101b,4,"HAS","H",HASH
              pop       cx
              pop       bx
              mov       al,[bx]
              and       ax,3
              shl       ax,1
              add       ax,cx
              jmp       _1PUSH

MOT@          PAR_FIND,101b,6,"(FIND",")",PAR_FIND
              pop       dx
              or        dx,dx
              jnz       _PFIND_00
              sub       ax,ax
              jmp       _1PUSH
_PFIND_00:    mov       bx,dx
              inc       bx
              inc       bx
              pop       di
              push      di
              mov       al,[bx]
              xor       al,[di]
              and       al,31
              jnz       _PFIND_03
_PFIND_01:    inc       bx
              inc       di
              mov       al,[bx]
              xor       al,[di]
              jz        _PFIND_01
              and       al,127
              jnz       _PFIND_03
              pop       di
              inc       bx
              mov       ax,bx
              and       ax,1
              add       bx,ax
              push      bx
              mov       bx,dx
              inc       bx
              inc       bx
              mov       al,[bx]
              and       al,64
              jnz       _PFIND_02
              mov       ax,1
              jmp       _1PUSH
_PFIND_02:    mov       ax,-1
              jmp       _1PUSH
_PFIND_03:    mov       bx,dx
              mov       dx,[bx]
              or        dx,dx
              jnz       _PFIND_00
              sub       ax,ax
              jmp       _1PUSH

MOT@          FIND,101b,4,"FIN","D",PAR_2POINT
              dw        _DOP
              dw        _CFETCH
              dw        _INT_BRANCH
              dw        _FIND3
              dw        _PRIOR
              dw        _OFF
              dw        _FALSE
              dw        _SHA_VOCS
              dw        _PAR_LIT
              dw        0
              dw        _PAR_DO
              dw        _FIND5
_FIND0:       dw        _DROP
              dw        _CONTEXT
              dw        _I
              dw        _2MUL
              dw        _PLUS
              dw        _FETCH
              dw        _DOP
              dw        _INT_BRANCH
              dw        _FIND2
              dw        _DOP
              dw        _PRIOR
              dw        _FETCH
              dw        _OVER
              dw        _PRIOR
              dw        _STORE
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _FIND1
              dw        _DROP
              dw        _FALSE
              dw        _BRANCH
              dw        _FIND2
_FIND1:       dw        _OVER
              dw        _SWAP
              dw        _HASH
              dw        _FETCH
              dw        _PAR_FIND
              dw        _DOP
              dw        _PAR_INT_LEAVE
_FIND2:       dw        _PAR_LOOP
              dw        _FIND0
_FIND5:       dw        _BRANCH
              dw        _FIND4
_FIND3:       dw        _DROP
              dw        _END_INT
              dw        _ON
              dw        _PAR_LIT
              dw        _NOOP
              dw        _PAR_LIT
              dw        -1
_FIND4:       dw        _UNNEST

MOT@          INT_UPPERCASE,101b,10,"?UPPERCAS","E",PAR_2POINT
              dw        _CAPS
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _UPCASE_00
              dw        _DOP
              dw        _COUNT
              dw        _UPPER
_UPCASE_00:   dw        _UNNEST

MOT@          DEFINED,101b,7,"DEFINE","D",PAR_2POINT
              dw        _BL
              dw        _WORD
              dw        _INT_UPPERCASE
              dw        _FIND
              dw        _UNNEST

MOT@          TICK,101b,1,"","'",PAR_2POINT
              dw        _DEFINED
              dw        _0EGAL
              dw        _INT_MISSING
              dw        _UNNEST

MOT@          PAR_MISSING,101b,10,"(?MISSING",")",PAR_2POINT
              dw        _INT_BRANCH
              dw        _INT_MIS_00
              dw        _BOLD
              dw        _TICK_WORD
              dw        _COUNT
              dw        _TYPE
              dw        _ATTOFF
              dw        _TRUE
              dw        _PAR_ABORTG
STR@          15,<" est inexistant">
_INT_MIS_00:  dw        _UNNEST

MOT@          INT_MISSING,101b,8,"?MISSIN","G",PAR_DEFER
              dw        _PAR_MISSING

;
; FIN DE INTER.INC
; 

