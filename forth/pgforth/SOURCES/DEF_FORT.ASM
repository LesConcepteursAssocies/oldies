Mots Red‚finis en PGFORTH

MOT@          PVIRGULE,111b,1,"",";",PAR_2POINT
              dw        _INT_CSP
              dw        _COMPILE
              dw        _EXIT
              dw        _REVEAL
              dw        _RUNTIME
              dw        _PAR_PVIRUSES
              dw        _PAR_2POINT

MOT@          SUP_MARK,101b,5,">MAR","K",PAR_2POINT
              dw        _HERE
              dw        _PAR_LIT
              dw        0
              dw        _VIRGULE
              dw        _EXIT

MOT@          SUP_RESOLVE,101b,8,">RESOLV","E",PAR_2POINT
              dw        _HERE
              dw        _SWAP
              dw        _STORE
              dw        _EXIT

MOT@          INF_MARK,101b,5,"<MAR","K",PAR_2POINT
              dw        _HERE
              dw        _EXIT

MOT@          INF_RESOLVE,101b,8,"<RESOLV","E",PAR_2POINT
              dw        _VIRGULE
              dw        _EXIT

MOT@          INT_SUP_MARK,101b,6,"?>MAR","K",PAR_2POINT
              dw        _TRUE
              dw        _SUP_MARK
              dw        _EXIT

MOT@          INT_SUP_RESOLVE,101b,9,"?>RESOLV","E",PAR_2POINT
              dw        _SWAP
              dw        _INT_CONDITION
              dw        _SUP_RESOLVE
              dw        _EXIT

MOT@          INT_INF_MARK,101b,6,"?<MAR","K",PAR_2POINT
              dw        _TRUE
              dw        _INF_MARK
              dw        _EXIT

MOT@          INT_INF_RESOLVE,101b,9,"?<RESOLV","E",PAR_2POINT
              dw        _SWAP
              dw        _INT_CONDITION
              dw        _INF_RESOLVE
              dw        _EXIT

MOT@          IF,111b,2,"I","F",PAR_2POINT
              dw        _COMPILE
              dw        _INT_BRANCH
              dw        _INT_SUP_MARK
              dw        _EXIT

MOT@          ELSE,111b,4,"ELS","E",PAR_2POINT
              dw        _COMPILE
              dw        _BRANCH
              dw        _INT_SUP_MARK
              dw        _2SWAP
              dw        _INT_SUP_RESOLVE
              dw        _EXIT

MOT@          THEN,111b,4,"THE","N",PAR_2POINT
              dw        _INT_SUP_RESOLVE
              dw        _EXIT

MOT@          BEGIN,111b,5,"BEGI","N",PAR_2POINT
              dw        _INT_INF_MARK
              dw        _EXIT

MOT@          UNTIL,111b,5,"UNTI","L",PAR_2POINT
              dw        _COMPILE
              dw        _INT_BRANCH
              dw        _INT_INF_RESOLVE
              dw        _EXIT

MOT@          AGAIN,111b,5,"AGAI","N",PAR_2POINT
              dw        _COMPILE
              dw        _BRANCH
              dw        _INT_INF_RESOLVE
              dw        _EXIT

MOT@          WHILE,111b,5,"WHIL","E",PAR_2POINT
              dw        _IF
              dw        _EXIT

MOT@          REPEAT,111b,6,"REPEA","T",PAR_2POINT
              dw        _2SWAP
              dw        _AGAIN
              dw        _THEN
              dw        _EXIT

MOT@          DO,111b,2,"D","O",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_DO
              dw        _INT_SUP_MARK
              dw        _EXIT

MOT@          INT_DO,111b,3,"?D","O",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_INT_DO
              dw        _INT_SUP_MARK
              dw        _EXIT

MOT@          LOOP,111b,4,"LOO","P",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_LOOP
              dw        _2DOP
              dw        _2PLUS
              dw        _INT_INF_RESOLVE
              dw        _INT_SUP_RESOLVE
              dw        _EXIT

MOT@          PLOOP,111b,5,"+LOO","P",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_PLOOP
              dw        _2DOP
              dw        _2PLUS
              dw        _INT_INF_RESOLVE
              dw        _INT_SUP_RESOLVE
              dw        _EXIT

MOT@          LEAVE,111b,5,"LEAV","E",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_LEAVE
              dw        _EXIT

MOT@          CASE,111b,4,"CAS","E",PAR_2POINT
              dw        _CSP
              dw        _FETCH
              dw        _STORECSP
              dw        _EXIT

MOT@          OF,111b,2,"O","F",PAR_2POINT
              dw        _COMPILE
              dw        _OVER
              dw        _COMPILE
              dw        _EGAL
              dw        _COMPILE
              dw        _INT_BRANCH
              dw        _SUP_MARK
              dw        _COMPILE
              dw        _DROP
              dw        _EXIT

MOT@          ENDOF,111b,5,"ENDO","F",PAR_2POINT
 	      dw        _COMPILE
 	      dw	_BRANCH
 	      dw	_SUP_MARK
 	      dw	_SWAP
 	      dw	_SUP_RESOLVE
 	      dw	_EXIT

MOT@          ENDCASE,111b,7,"ENDCAS","E",PAR_2POINT
	      dw	_COMPILE
	      dw	_DROP
_ENDCASE_00:  dw	_SPFETCH
	      dw	_CSP
	      dw	_FETCH
	      dw	_DIFF
              dw	_INT_BRANCH
              dw        _ENDCASE_01
              dw	_SUP_RESOLVE
              dw	_BRANCH
              dw	_ENDCASE_00
_ENDCASE_01:  dw        _CSP
	      dw	_STORE
	      dw        _EXIT

MOT@          PAR_PUT,101b,5,"(PUT",")",PAR_PUT
              mov       ax,ds
              mov       es,ax
              pop       bx
              pop       cx
              pop       dx
              pop       ds
              mov       ah,40h
              int       21h
              push      ax
              mov       bx,es
              mov       ds,bx
              jc        _PAR_PUT_00
              xor       ax,ax
_PAR_PUT_00:  jmp       _1PUSH

MOT@          PAR_MAKE,101b,6,"(MAKE",")",PAR_MAKE
              pop       cx
              pop       dx
              mov       ah,3Ch
              int       21h
              push      ax
              jnc       _PAR_SEK_00
              jmp       _1PUSH
_PAR_SEK_00:  inc       word ptr [$SHA_FILES]
              xor       ax,ax
              jmp       _1PUSH

MOT@          PAR_REN,101b,5,"(REN",")",PAR_REN
              pop       di
              pop       dx
              mov       ax,ds
              mov       es,ax
              mov       ah,56h
              int       21h
              jc        _PAR_REN_00
              xor       ax,ax
_PAR_REN_00:  jmp       _1PUSH

MOT@          PAR_SEARCH0,101b,9,"(SEARCH0",")",PAR_SEARCH0
              pop       cx
              pop       dx
              mov       ah,4Eh
              int       21h
              mov       ax,-1
              jnc       _PAR_SE0_00
              xor       ax,ax
_PAR_SE0_00:  jmp       _1PUSH

MOT@          PAR_SEARCH,101b,8,"(SEARCH",")",PAR_SEARCH
              mov       ah,4Fh
              int       21h
              mov       ax,-1
              jnc       _PAR_SEA_00
              xor       ax,ax
_PAR_SEA_00:  jmp       _1PUSH

MOT@          PAR_FREE,101b,6,"(FREE",")",PAR_FREE
              pop       dx
              mov       ah,36h
              int       21h
              jnc       _PAR_FREE_00
              xor       ax,ax
              jmp       _1PUSH
_PAR_FREE_00: push      bx
              push      ax
              push      cx
              jmp       _NEXT

MOT@          PAR_SEEK,101b,6,"(SEEK",")",PAR_SEEK
              pop       ax
              pop       bx
              pop       cx
              pop       dx
              mov       ah,42h
              int       21h
              push      ax
              push      dx
              jc        _PAR_SEEK_00
              xor       ax,ax
_PAR_SEEK_00: jmp       _1PUSH

MOT@          DOS,101b,3,"DO","S",DOS
              pop       ax
              mov       ah,al
              pop       dx
              int       21h
              jc        _DOS_00
              xor       ax,ax
_DOS_00:      jmp       _1PUSH

MOT@          SELECT,101b,6,"SELEC","T",PAR_2POINT
              dw        _1MOINS
              dw        _PAR_LIT
              dw        0Eh
              dw        _DOS
              dw        _INT_DOSERR
              dw        _EXIT

MOT@          A_2POINT,101b,2,"A",":",PAR_2POINT
              dw        _PAR_LIT
              dw        1
              dw        _SELECT
              dw        _EXIT

MOT@          B_2POINT,101b,2,"B",":",PAR_2POINT
              dw        _PAR_LIT
              dw        2
              dw        _SELECT
              dw        _EXIT

MOT@          C_2POINT,101b,2,"C",":",PAR_2POINT
              dw        _PAR_LIT
              dw        3
              dw        _SELECT
              dw        _EXIT

MOT@          D_2POINT,101b,2,"D",":",PAR_2POINT
              dw        _PAR_LIT
              dw        4
              dw        _SELECT
              dw        _EXIT

MOT@          E_2POINT,101b,2,"E",":",PAR_2POINT
              dw        _PAR_LIT
              dw        5
              dw        _SELECT
              dw        _EXIT

MOT@          DRIVE_INT,101b,6,"DRIVE","?",DRIVE_INT
              mov       ah,19h
              int       21h
              sub       ah,ah
              jmp       _1PUSH

MOT@          MKDIR,101b,5,"MKDI","R",PAR_2POINT
              dw        _ASCII0
              dw        _PAR_LIT
              dw        39h
              dw        _DOS
              dw        _INT_DOSERR
              dw        _EXIT

MOT@          RMDIR,101b,5,"RMDI","R",PAR_2POINT
              dw        _ASCII0
              dw        _PAR_LIT
              dw        3Ah
              dw        _DOS
              dw        _INT_DOSERR
              dw        _EXIT

MOT@          CHDIR,101b,5,"CHDI","R",PAR_2POINT
              dw        _ASCII0
              dw        _PAR_LIT
              dw        3Bh
              dw        _DOS
              dw        _INT_DOSERR
              dw        _EXIT

MOT@          DEL,101b,3,"DE","L",PAR_2POINT
              dw        _ASCII0
              dw        _PAR_LIT
              dw        41h
              dw        _DOS
              dw        _INT_DOSERR
              dw        _EXIT

;
; M‚moire 8-16 bits Inter-Segment
;

MOT@          LSTORE,101b,2,"L","!",LSTORE
              mov       ax,ds
              pop       bx
              pop       ds
              pop       [bx]
              mov       ds,ax
              jmp       _NEXT

MOT@          LCSTORE,101b,3,"LC","!",LCSTORE
              mov       cx,ds
              pop       bx
              pop       ds
              pop       ax
              mov       [bx],al
              mov       ds,cx
              jmp       _NEXT

MOT@          LFETCH,101b,2,"L","@",LFETCH
              mov       cx,ds
              pop       bx
              pop       ds
              push      [bx]
              mov       ds,cx
              jmp       _NEXT

MOT@          LCFETCH,101b,3,"LC","@",LCFETCH
              mov       cx,ds
              pop       bx
              pop       ds
              xor       ax,ax
              mov       al,[bx]
              mov       ds,cx
              jmp       _1PUSH

MOT@          D0DIFF,101b,4,"D0<",">",PAR_2POINT
              dw        _D0EGAL
              dw        _NOT
              dw        _EXIT

MOT@          D0SUP,101b,3,"D0",">",PAR_2POINT
              dw        _OR
              dw        _0SUP
              dw        _EXIT

MOT@          D0INF,101b,3,"D0","<",PAR_2POINT
              dw        _OR
              dw        _0INF
              dw        _EXIT

MOT@          D0INFEGAL,101b,4,"D0<","=",PAR_2POINT
              dw        _D0SUP
              dw        _NOT
              dw        _EXIT

MOT@          D0SUPEGAL,101b,4,"D0>","=",PAR_2POINT
              dw        _D0INF
              dw        _NOT
              dw        _EXIT

MOT@          DSUP_EGAL,101b,3,"D>","=",PAR_2POINT
              dw        _DINF
              dw        _NOT
              dw        _EXIT

MOT@          DINF_EGAL,101b,3,"D<","=",PAR_2POINT
              dw        _DSUP
              dw        _NOT
              dw        _EXIT

MOT@          DDIFF,101b,3,"D<",">",PAR_2POINT
              dw        _DEGAL
              dw        _NOT
              dw        _EXIT

MOT@          OT,101b,2,"A","T",OT
	      pop	ax
	      pop	dx
	      mov	dh,al
	      xor	bh,bh
	      mov	ah,02h
	      int	10h
	      jmp	_NEXT

MOT@          ASCII,111b,5,"ASCI","I",PAR_2POINT
              dw        _BL
              dw        _WORD
              dw        _1PLUS
              dw        _CFETCH
              dw        _STATE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _ASCII_00
              dw        _LITERAL
_ASCII_00:    dw        _EXIT

MOT@          CONTROL,111b,7,"CONTRO","L",PAR_2POINT
              dw        _BL
              dw        _WORD
              dw        _1PLUS
              dw        _CFETCH
              dw        _PAR_LIT
              dw        "@"
              dw        _MOINS
              dw        _STATE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _CONTROL_00
              dw        _LITERAL
_CONTROL_00:  dw        _EXIT

MOT@          2CONSTANT,101b,9,"2CONSTAN","T",PAR_2POINT
              dw        _CREATE
              dw        _VIRGULE
              dw        _VIRGULE
              dw        _PAR_PVIRUSES
              dw        _PAR_2CONSTANT

MOT@          2VARIABLE,101b,9,"2VARIABL","E",PAR_2POINT
              dw        _CREATE
              dw        _PAR_LIT
              dw        0
              dw        _DOP
              dw        _VIRGULE
              dw        _VIRGULE
              dw        _PAR_PVIRUSES
              dw        _PAR_2VARIABLE

MOT@          DEFER,101b,5,"DEFE","R",PAR_2POINT
              dw        _PAR_LIT
              dw        _NOOP
              dw        _CONSTANT
              dw        _PAR_PVIRUSES
              dw        _PAR_DEFER

MOT@          IS,111b,2,"I","S",PAR_2POINT
              dw        _STATE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _IS_00
              dw        _COMPILE
              dw        _PAR_IS
              dw        _BRANCH
              dw        _IS_01
_IS_00:       dw        _TICK
              dw        _SUP_BODY
              dw        _STORE
_IS_01:       dw        _EXIT

MOT@          DOES,111b,5,"DOES",">",PAR_2POINT
              dw        _COMPILE
              dw        _PAR_PVIRCODE
              dw        _PAR_LIT
              dw        0E8h
              dw        _CVIRGULE
              dw        _PAR_DOES
              dw        _HERE
              dw        _2PLUS
              dw        _MOINS
              dw        _VIRGULE
              dw        _ALIGN
              dw        _EXIT

MOT@          VOCABULARY,101b,10,"VOCABULAR","Y",PAR_2POINT
              dw        _CREATE
              dw        _SHA_THREADS
              dw        _PAR_LIT
              dw        0
              dw        _PAR_DO
              dw        _VOCA1
_VOCA0:       dw        _PAR_LIT
              dw        0
              dw        _VIRGULE
              dw        _PAR_LOOP
              dw        _VOCA0
_VOCA1:       dw        _HERE
              dw        _VOC_LINK
              dw        _FETCH
              dw        _VIRGULE
              dw        _VOC_LINK
              dw        _STORE
              dw        _PAR_PVIRUSES
              dw        _PAR_VOC

MOT@          DEFINITIONS,101b,11,"DEFINITION","S",PAR_2POINT
              dw        _CONTEXT
              dw        _FETCH
              dw        _CURRENT
              dw        _STORE
              dw        _EXIT

MOT@          PAR_PVIRCODE,101b,7,"(;CODE",")",PAR_2POINT
              dw        _R_SUP
              dw        _LAST
              dw        _FETCH
              dw        _NAME_SUP
              dw        _STORE
              dw        _EXIT

MOT@          J,101b,1,"","J",J
              mov       ax,[bp+6]
              add       ax,[bp+8]
              jmp       _1PUSH

MOT@          K,101b,1,"","K",K
              mov       ax,[bp+12]
              add       ax,[bp+14]
              jmp       _1PUSH

MOT@          CRO_TICK,111b,3,"['","]",PAR_2POINT
              dw        _TICK
              dw        _LITERAL
              dw        _EXIT

MOT@          PGUI,111b,2,'.','"',PAR_2POINT
              dw        _STATE
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PGUI_00
              dw        _COMPILE
              dw        _PAR_PGUI
              dw        _VIR_GUI
              dw        _BRANCH
              dw        _PGUI_01
_PGUI_00:     dw        _PAR_LIT
              dw        '"'
              dw        _WORD
              dw        _COUNT
              dw        _TYPE
_PGUI_01:     dw        _EXIT

MOT@          INT_CONDITION,101b,10,"?CONDITIO","N",PAR_2POINT
              dw        _NOT
              dw        _PAR_ABORTG
STR@          23,<"Structure mal imbriqu‚e">
              dw        _EXIT

MOT@          PAR_UPOINT,101b,4,"(U.",")",PAR_2POINT
              dw        _PAR_LIT
              dw        0
              dw        _INF_SHA
              dw        _SHA_S
              dw        _SHA_SUP
              dw        _EXIT

MOT@          UPOINT,101b,2,"U",".",PAR_2POINT
              dw        _PAR_UPOINT
              dw        _TYPE
              dw        _SPACE
              dw        _EXIT

MOT@          UPOINTR,101b,3,"U.","R",PAR_2POINT
              dw        _SUP_R
              dw        _PAR_UPOINT
              dw        _R_SUP
              dw        _OVER
              dw        _MOINS
              dw        _SPACES
              dw        _TYPE
              dw        _EXIT

MOT@          PAR_UDPOINT,101b,5,"(UD.",")",PAR_2POINT
              dw        _INF_SHA
              dw        _SHA_S
              dw        _SHA_SUP
              dw        _EXIT

MOT@          UDPOINT,101b,3,"UD",".",PAR_2POINT
              dw        _PAR_UDPOINT
              dw        _TYPE
              dw        _SPACE
              dw        _EXIT

MOT@          UDPOINTR,101b,4,"UD.","R",PAR_2POINT
              dw        _SUP_R
              dw        _PAR_UDPOINT
              dw        _R_SUP
              dw        _OVER
              dw        _MOINS
              dw        _SPACES
              dw        _TYPE
              dw        _EXIT

MOT@          PAR_DPOINT,101b,4,"(D.",")",PAR_2POINT
              dw        _TUCK
              dw        _DABS
              dw        _INF_SHA
              dw        _SHA_S
              dw        _ROT
              dw        _SIGN
              dw        _SHA_SUP
              dw        _EXIT

MOT@          DPOINT,101b,2,"D",".",PAR_2POINT
              dw        _PAR_DPOINT
              dw        _TYPE
              dw        _SPACE
              dw        _EXIT

MOT@          DPOINTR,101b,3,"D.","R",PAR_2POINT
              dw        _SUP_R
              dw        _PAR_DPOINT
              dw        _R_SUP
              dw        _OVER
              dw        _MOINS
              dw        _SPACES
              dw        _TYPE
              dw        _EXIT

MOT@          POINT_ID,101b,3,".I","D",PAR_2POINT
              dw        _DOP
              dw        _1PLUS
              dw        _DOP
              dw        _CFETCH
              dw        _ROT
              dw        _CFETCH
              dw        _PAR_LIT
              dw        31
              dw        _AND
              dw        _PAR_LIT
              dw        0
              dw        _PAR_INT_DO
              dw        _POINT_ID_03
_POINT_ID_00: dw        _DOP
              dw        _PAR_LIT
              dw        127
              dw        _AND
              dw        _EMIT
              dw        _PAR_LIT
              dw        128
              dw        _AND
              dw        _INT_BRANCH
              dw        _POINT_ID_01
              dw        _PAR_LIT
              dw        "_"
              dw        _PAR_LIT
              dw        128
              dw        _OR
              dw        _BRANCH
              dw        _POINT_ID_02
_POINT_ID_01: dw        _1PLUS
              dw        _DOP
              dw        _CFETCH
_POINT_ID_02: dw        _PAR_LOOP
              dw        _POINT_ID_00
_POINT_ID_03: dw        _2DROP
              dw        _SPACE
              dw        _EXIT

MOT@          BYE,101b,3,"BY","E",BYE
              mov       ax,[$REG_SS]
              mov       ss,ax
              mov       ax,[$REG_SP]
              mov       sp,ax
              mov       ax,[$REG_BP]
              mov       bp,ax
              pop       ds
              mov       ax,4C00h
              int       21h

MOT@          L_TO_NAME,101b,6,"L>NAM","E",PAR_2POINT
              dw        _2PLUS
              dw        _EXIT

MOT@          BODY_SUP,101b,5,"BODY",">",PAR_2POINT
              dw        _2MOINS
              dw        _EXIT

MOT@          LINK_SUP,101b,5,"LINK",">",PAR_2POINT
              dw        _L_TO_NAME
              dw        _NAME_SUP
              dw        _EXIT

MOT@          SUP_NAME,101b,5,">NAM","E",PAR_2POINT
              dw        _PAR_LIT
              dw        -1
              dw        _TRAVERSE
              dw        _PAR_LIT
              dw        -1
              dw        _TRAVERSE
              dw        _EXIT

MOT@          SUP_LINK,101b,5,">LIN","K",PAR_2POINT
              dw        _SUP_NAME
              dw        _N_TO_LINK
              dw        _EXIT

;
; Changements de Base
;

MOT@          BINARY,101b,6,"BINAR","Y",PAR_2POINT
              dw        _PAR_LIT
              dw        2
              dw        _SWAP
              dw        _STORE
              dw        _EXIT

MOT@          OCTAL,101b,5,"OCTA","L",PAR_2POINT
              dw        _PAR_LIT
              dw        8
              dw        _SWAP
              dw        _STORE
              dw        _EXIT

MOT@          HEX,101b,3,"HE","X",PAR_2POINT
              dw        _PAR_LIT
              dw        16
              dw        _SWAP
              dw        _STORE
              dw        _EXIT

;
; Comparaison 32 bits
;

MOT@          D0EGAL,101b,3,"D0","=",PAR_2POINT
              dw        _OR
              dw        _0EGAL
              dw        _EXIT

MOT@          DEGAL,101b,2,"D","=",PAR_2POINT
              dw        _DMOINS
              dw        _D0EGAL
              dw        _EXIT

MOT@          DUINF,101b,3,"DU","<",PAR_2POINT
              dw        _ROT
              dw        _SWAP
              dw        _2DOP
              dw        _UINF
              dw        _INT_BRANCH
              dw        _DUINF_00
              dw        _2DROP
              dw        _2DROP
              dw        _TRUE
              dw        _BRANCH
              dw        _DUINF_02
_DUINF_00:    dw        _DIFF
              dw        _INT_BRANCH
              dw        _DUINF_01
              dw        _2DROP
              dw        _FALSE
              dw        _BRANCH
              dw        _DUINF_02
_DUINF_01:    dw        _UINF
_DUINF_02:    dw        _EXIT

MOT@          DINF,101b,2,"D","<",PAR_2POINT
              dw        _PAR_LIT
              dw        2
              dw        _PICK
              dw        _OVER
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _DINF_00
              dw        _DUINF
              dw        _BRANCH
              dw        _DINF_01
_DINF_00:     dw        _NIP
              dw        _ROT
              dw        _DROP
              dw        _INF
_DINF_01:     dw        _EXIT

MOT@          DSUP,101b,2,"D",">",PAR_2POINT
              dw        _2SWAP
              dw        _DINF
              dw        _EXIT

MOT@          DMIN,101b,4,"DMI","N",PAR_2POINT
              dw        _4DOP
              dw        _DSUP
              dw        _INT_BRANCH
              dw        _DMIN_00
              dw        _2SWAP
_DMIN_00:     dw        _2DROP
              dw        _EXIT

MOT@          DMAX,101b,4,"DMA","X",PAR_2POINT
              dw        _4DOP
              dw        _DINF
              dw        _INT_BRANCH
              dw        _DMAX_00
              dw        _2SWAP
_DMAX_00:     dw        _2DROP
              dw        _EXIT

MOT@          S_TO_D,101b,3,"S>","D",S_TO_D
              pop       ax
              cwd
              xchg      ax,dx
              jmp       _2PUSH

MOT@          D2MUL,101b,3,"D2","*",D2MUL
              pop       ax
              pop       dx
              shl       dx,1
              rcl       ax,1
              jmp       _2PUSH

MOT@          D2DIV,101b,3,"D2","/",D2DIV
              pop       ax
              pop       dx
              sar       ax,1
              rcr       dx,1
              jmp       _2PUSH

MOT@          DMOINS,101b,2,"D","-",DMOINS
              pop       bx
              pop       cx
              pop       ax
              pop       dx
              sub       dx,cx
              sbb       ax,bx
              jmp       _2PUSH

MOT@          BLANK,101b,5,"BLAN","K",PAR_2POINT
              dw        _BL
              dw        _FILL
              dw        _EXIT

MOT@          LENGTH,101b,6,"LENGT","H",LENGTH
              pop       bx
              mov       ax,[bx]
              inc       bx
              inc       bx
              push      bx
              jmp       _1PUSH

MOT@          UNDERL,101b,6,"UNDER","L",PAR_2POINT
              dw        _PAR_LIT
              dw        4
              dw        _PAR_ATTRIB
              dw        _EXIT

MOT@          BLINK,101b,5,"BLIN","K",PAR_2POINT
              dw        _PAR_LIT
              dw        5
              dw        _PAR_ATTRIB
              dw        _EXIT

MOT@          INVERS,101b,6,"INVER","S",PAR_2POINT
              dw        _PAR_LIT
              dw        7
              dw        _PAR_ATTRIB
              dw        _EXIT

MOT@          DARK,101b,4,"DAR","K",PAR_2POINT
              dw        _PAR_LIT
              dw        27
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "["
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "2"
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "J"
              dw        _PAR_CONS
              dw        _SHA_OUT
              dw        _OFF
              dw        _SHA_LINE
              dw        _OFF
              dw        _EXIT

MOT@          HOME,101b,4,"HOM","E",PAR_2POINT
              dw        _PAR_LIT
              dw        27
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "["
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "f"
              dw        _PAR_CONS
              dw        _EXIT

MOT@          SUP_TYPE,101b,5,">TYP","E",PAR_2POINT
              dw        _TUCK
              dw        _$PAD
              dw        _SWAP
              dw        _CMOVE
              dw        _$PAD
              dw        _SWAP
              dw        _TYPE
              dw        _EXIT

MOT@          PAR_ABORT,101b,7,"(ABORT",")",PAR_2POINT
              dw        _SP0
              dw        _FETCH
              dw        _SPSTORE
              dw        _ALLCLOSE
              dw        _HANDLE
              dw        _OFF
              dw        _QUIT
              dw        _EXIT

MOT@          PAR_STATUS,101b,8,"(STATUS",")",PAR_2POINT
              dw        _CR
              dw        _STATUS_INT
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PAR_STA_04
              dw        _OBASE
              dw        _FETCH
              dw        _IBASE
              dw        _FETCH
              dw        _OBASE
              dw        _DECIMAL
              dw        _PAR_PGUI
STR@          5,<"SP = ">
              dw        _DEPTH
              dw        _2MOINS
              dw        _POINT
              dw        _CR
              dw        _PAR_PGUI
STR@          5,<"RP = ">
              dw        _RDEPTH
              dw        _1MOINS
              dw        _POINT
              dw        _CR
              dw        _2DOP
              dw        _PAR_LIT
              dw        2
              dw        _PAR_LIT
              dw        0
              dw        _PAR_DO
              dw        _PAR_STA_08
_PAR_STA_05:  dw        _I
              dw        _INT_BRANCH
              dw        _PAR_STA_06
              dw        _PAR_PGUI
STR@          15,<"Base de sortie ">
              dw        _BRANCH
              dw        _PAR_STA_07
_PAR_STA_06:  dw        _PAR_PGUI
STR@          15,<"Base d' entr‚e ">
_PAR_STA_07:  dw        _DOP
              dw        _PAR_LIT
              dw        2
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _PAR_STA_09
              dw        _PAR_PGUI
STR@          7,<"binaire">
              dw        _BRANCH
              dw        _PAR_STA_03
_PAR_STA_09:  dw        _DOP
              dw        _PAR_LIT
              dw        8
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _PAR_STA_00
              dw        _PAR_PGUI
STR@          6,<"octale">
              dw        _BRANCH
              dw        _PAR_STA_03
_PAR_STA_00:  dw        _DOP
              dw        _PAR_LIT
              dw        10
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _PAR_STA_01
              dw        _PAR_PGUI
STR@          8,<"d‚cimale">
              dw        _BRANCH
              dw        _PAR_STA_03
_PAR_STA_01:  dw        _DOP
              dw        _PAR_LIT
              dw        16
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _PAR_STA_02
              dw        _PAR_PGUI
STR@          12,<"hexad‚cimale">
              dw        _BRANCH
              dw        _PAR_STA_03
_PAR_STA_02:  dw        _DOP
              dw        _POINT
_PAR_STA_03:  dw        _CR
              dw        _DROP
              dw        _PAR_LOOP
              dw        _PAR_STA_05
_PAR_STA_08:  dw        _IBASE
              dw        _STORE
              dw        _OBASE
              dw        _STORE
_PAR_STA_04:  dw        _EXIT

MOT@          2FETCH,101b,2,"2","@",2FETCH
              pop       bx
              push      [bx]
              push      [bx+2]
              jmp       _NEXT

MOT@          2STORE,101b,2,"2","!",2STORE
              pop       bx
              pop       [bx+2]
              pop       [bx]
              jmp       _NEXT

MOT@          CRESET,101b,6,"CRESE","T",CRESET
              pop       bx
              pop       ax
              not       ax
              and       [bx],al
              jmp       _NEXT

MOT@          U2DIV,101b,3,"U2","/",U2DIV
              pop       ax
              shr       ax,1
              jmp       _1PUSH

MOT@          8MUL,101b,2,"8","*",8MUL
              pop       ax
              mov       cl,3
              shl       ax,cl
              jmp       _1PUSH

MOT@          SUP_EGAL,101b,2,">","=",PAR_2POINT
              dw        _INF
              dw        _NOT
              dw        _EXIT

MOT@          INF_EGAL,101b,2,"<","=",PAR_2POINT
              dw        _SUP
              dw        _NOT
              dw        _EXIT

MOT@          USUP_EGAL,101b,3,"U>","=",PAR_2POINT
              dw        _UINF
              dw        _NOT
              dw        _EXIT

MOT@          UINF_EGAL,101b,3,"U<","=",PAR_2POINT
              dw        _USUP
              dw        _NOT
              dw        _EXIT

MOT@          0SUP_EGAL,101b,3,"0>","=",PAR_2POINT
              dw        _0INF
              dw        _NOT
              dw        _EXIT

MOT@          0INF_EGAL,101b,3,"0<","=",PAR_2POINT
              dw        _0SUP
              dw        _NOT
              dw        _EXIT

MOT@          WITHIN,101b,6,"WITHI","N",PAR_2POINT
              dw        _1MOINS
              dw        _BETWEEN
              dw        _EXIT

MOT@          MTRAILING,101b,9,"-TRAILIN","G",PAR_2POINT
              dw        _DOP
              dw        _PAR_LIT
              dw        0
              dw        _PAR_INT_DO
              dw        _MTRAIL_01
_MTRAIL_00:   dw        _2DOP
              dw        _PLUS
              dw        _1MOINS
              dw        _CFETCH
              dw        _BL
              dw        _DIFF
              dw        _INT_LEAVE
              dw        _1MOINS
              dw        _PAR_LOOP
              dw        _MTRAIL_00
_MTRAIL_01:   dw        _EXIT

MOT@          RES_IN,101b,6,"RES-I","N",PAR_2POINT
              dw        _FORTH
              dw        _TRUE
              dw        _PAR_ABORTG
STR@          14,<"Initialisation">
              dw        _EXIT

MOT@          P_IN,101b,4,"P-I","N",PAR_2POINT
              dw        _PRINTING
              dw        _FETCH
              dw        _NOT
              dw        _PRINTING
              dw        _STORE
              dw        _EXIT


