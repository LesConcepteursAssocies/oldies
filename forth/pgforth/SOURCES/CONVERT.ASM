;
; CONVERTIONS NUMERIQUE <--> ALPHA
;

MOT@          FFAIL,101b,4,"FAI","L",PAR_CREATE
_FAIL:        sub       ax,ax
              jmp       _1PUSH

MOT@          DIGIT,101b,5,"DIGI","T",DIGIT
              pop       dx
              pop       ax
              push      ax
              sub       al,"0"
              jb        _FAIL
              cmp       al,9
              jng       _DIGIT0
              cmp       al,17
              jb        _FAIL
              sub       al,7
_DIGIT0:      cmp       al,dl
              jae       _FAIL
              mov       dl,al
              pop       ax
              mov       ax,-1
              jmp       _2PUSH

MOT@          DOUBLE_INT,101b,7,"DOUBLE","?",PAR_2POINT
              dw        _DPL
              dw        _FETCH
              dw        _1PLUS
              dw        _0DIFF
              dw        _UNNEST

MOT@          CONVERT,101b,7,"CONVER","T",PAR_2POINT
_CONVERT2:    dw        _1PLUS
              dw        _DOP
              dw        _SUP_R
              dw        _CFETCH
              dw        _IBASE
              dw        _FETCH
              dw        _DIGIT
              dw        _INT_BRANCH
              dw        _CONVERT1
              dw        _SWAP
              dw        _IBASE
              dw        _FETCH
              dw        _UMMUL
              dw        _DROP
              dw        _ROT
              dw        _IBASE
              dw        _FETCH
              dw        _UMMUL
              dw        _DPLUS
              dw        _DOUBLE_INT
              dw        _INT_BRANCH
              dw        _CONVERT0
              dw        _DPL
              dw        _1PSTORE
_CONVERT0:    dw        _R_SUP
              dw        _BRANCH
              dw        _CONVERT2
_CONVERT1:    dw        _DROP
              dw        _R_SUP
              dw        _UNNEST

MOT@          PAR_NUMBER_INT,101b,9,"(NUMBER?",")",PAR_2POINT
              dw        _PAR_LIT
              dw        0
              dw        _PAR_LIT
              dw        0
              dw        _ROT
              dw        _DOP
              dw        _1PLUS
              dw        _CFETCH
              dw        _PAR_LIT
              dw        "-"
              dw        _EGAL
              dw        _DOP
              dw        _SUP_R
              dw        _MOINS
              dw        _PAR_LIT
              dw        -1
              dw        _DPL
              dw        _STORE
_PAR_N_INT0:  dw        _CONVERT
              dw        _DOP
              dw        _CFETCH
              dw        _PAR_LIT
              dw        ","
              dw        _PAR_LIT
              dw        "/"
              dw        _BETWEEN
              dw        _INT_BRANCH
              dw        _PAR_N_INT1
              dw        _PAR_LIT
              dw        0
              dw        _DPL
              dw        _STORE
              dw        _BRANCH
              dw        _PAR_N_INT0
_PAR_N_INT1:  dw        _MROT
              dw        _R_SUP
              dw        _INT_BRANCH
              dw        _PAR_N_INT2
              dw        _DNEGATE
_PAR_N_INT2:  dw        _ROT
              dw        _CFETCH
              dw        _BL
              dw        _EGAL
              dw        _UNNEST

MOT@          BOUNDS,101b,6,"BOUND","S",BOUNDS
              pop       bx
              pop       ax
              mov       dx,ax
              add       dx,bx
              jmp       _2PUSH

MOT@          NUMBER_INT,101b,7,"NUMBER","?",PAR_2POINT
              dw        _FALSE
              dw        _OVER
              dw        _COUNT
              dw        _BOUNDS
              dw        _PAR_INT_DO
              dw        _NUMBER_INT2
_NUMBER_INT0: dw        _I
              dw        _CFETCH
              dw        _IBASE
              dw        _FETCH
              dw        _DIGIT
              dw        _NIP
              dw        _INT_BRANCH
              dw        _NUMBER_INT1
              dw        _DROP
              dw        _TRUE
              dw        _PAR_LEAVE
_NUMBER_INT1: dw        _PAR_LOOP
              dw        _NUMBER_INT0
_NUMBER_INT2: dw        _INT_BRANCH
              dw        _NUMBER_INT3
              dw        _PAR_NUMBER_INT
              dw        _BRANCH
              dw        _NUMBER_INT4
_NUMBER_INT3: dw        _DROP
              dw        _PAR_LIT
              dw        0
              dw        _PAR_LIT
              dw        0
              dw        _FALSE
_NUMBER_INT4: dw        _UNNEST

MOT@          PAR_NUMBER,101b,8,"(NUMBER",")",PAR_2POINT
              dw        _NUMBER_INT
              dw        _NOT
              dw        _INT_MISSING
              dw        _UNNEST

MOT@          NUMBER,101b,6,"NUMBE","R",PAR_DEFER
              dw        _PAR_NUMBER

;
; Sortie des Nombres
;

MOT@          HOLD,101b,4,"HOL","D",PAR_2POINT
              dw        _HLD
              dw        _1MSTORE
              dw        _HLD
              dw        _FETCH
              dw        _CSTORE
              dw        _UNNEST

MOT@          INF_SHA,101b,2,"<","#",PAR_2POINT
              dw        _PAD
              dw        _HLD
              dw        _STORE
              dw        _UNNEST

MOT@          SHA_SUP,101b,2,"#",">",PAR_2POINT
              dw        _2DROP
              dw        _HLD
              dw        _FETCH
              dw        _PAD
              dw        _OVER
              dw        _MOINS
              dw        _UNNEST

MOT@          SIGN,101b,4,"SIG","N",PAR_2POINT
              dw        _0INF
              dw        _INT_BRANCH
              dw        _SIGN0
              dw        _PAR_LIT
              dw        "-"
              dw        _HOLD
_SIGN0:       dw        _UNNEST

MOT@          SHA,101b,1,"","#",PAR_2POINT
              dw        _OBASE
              dw        _FETCH
              dw        _MUDIVMOD
              dw        _ROT
              dw        _PAR_LIT
              dw        9
              dw        _OVER
              dw        _INF
              dw        _INT_BRANCH
              dw        _SHA0
              dw        _PAR_LIT
              dw        7
              dw        _PLUS
_SHA0:        dw        _PAR_LIT
              dw        "0"
              dw        _PLUS
              dw        _HOLD
              dw        _UNNEST

MOT@          SHA_S,101b,2,"#","S",PAR_2POINT
_SHA_S_00:    dw        _SHA
              dw        _2DOP
              dw        _OR
              dw        _0EGAL
              dw        _INT_BRANCH
              dw        _SHA_S_00
              dw        _UNNEST

MOT@          PAR_POINT,101b,3,"(.",")",PAR_2POINT
              dw        _DOP
              dw        _ABS
              dw        _PAR_LIT
              dw        0
              dw        _INF_SHA
              dw        _SHA_S
              dw        _ROT
              dw        _SIGN
              dw        _SHA_SUP
              dw        _UNNEST

MOT@          POINT,101b,1,"",".",PAR_2POINT
              dw        _PAR_POINT
              dw        _TYPE
              dw        _SPACE
              dw        _UNNEST

MOT@          POINTR,101b,2,".","R",PAR_2POINT
              dw        _SUP_R
              dw        _PAR_POINT
              dw        _R_SUP
              dw        _OVER
              dw        _MOINS
              dw        _SPACES
              dw        _TYPE
              dw        _UNNEST

;
; FIN DE CONVERT.INC
; 

