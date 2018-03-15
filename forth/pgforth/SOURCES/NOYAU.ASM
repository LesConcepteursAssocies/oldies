;
; NOYAU.ASM
; D‚finitions CODE du noyau PGFORTH
;
;
; Changements de Base
;
MOT@          DECIMAL,101b,7,"DECIMA","L",PAR_2POINT
              dw        _PAR_LIT
              dw        10
              dw        _SWAP
              dw        _STORE
              dw        _UNNEST

;
; M‚moire 8-16 bits Intra-Segment
;

MOT@          CFETCH,101b,2,"C","@",CFETCH
              pop       bx
              xor       ah,ah
              mov       al,[bx]
              jmp       _1PUSH

MOT@          FETCH,101b,1,"","@",FETCH
              pop       bx
              push      [bx]
              jmp       _NEXT

MOT@          CSTORE,101b,2,"C","!",CSTORE
              pop       bx
              pop       ax
              mov       [bx],al
              jmp       _NEXT

MOT@          STORE,101b,1,"","!",STORE
              pop       bx
              pop       [bx]
              jmp       _NEXT

;
; D‚placement M‚moire Intra-Segment
;

MOT@          LCMOVE,101b,6,"LCMOV","E",LCMOVE
              cld
              mov       bx,si
              mov       dx,ds
              pop       cx
              pop       di
              pop       es
              pop       si
              pop       ds
              jcxz      _CMOVE_00
              rep       movsb
_CMOVE_00:    mov       si,bx
              mov       ds,dx
              jmp       _NEXT

MOT@          LCMOVE_SUP,101b,7,"LCMOVE",">",LCMOVE_SUP
              std
              mov       bx,si
              mov       dx,ds
              pop       cx
              pop       di
              pop       es
              pop       si
              pop       ds
              jcxz      _CMOVE_S00
              dec       cx
              add       di,cx
              add       si,cx
              inc       cx
              rep       movsb
_CMOVE_S00:   mov       si,bx
              mov       ds,dx
              cld
              jmp       _NEXT

MOT@          LMOVE,101b,5,"LMOV","E",PAR_2POINT
              dw        _PAR_LIT
              dw        3
              dw        _PICK
              dw        _PAR_LIT
              dw        2
              dw        _PICK
              dw        _UINF
              dw        _INT_BRANCH
              dw        _MOVE_00
              dw        _LCMOVE_SUP
              dw        _BRANCH
              dw        _MOVE_01
_MOVE_00:     dw        _LCMOVE
_MOVE_01:     dw        _UNNEST

MOT@          CMOVE,101b,5,"CMOV","E",CMOVE
              pop       ax
              pop       bx
              pop       cx
              push      ds
              push      cx
              push      ds
              push      bx
              push      ax
              lea       bx,_LCMOVE
              jmp       [bx]

MOT@          CMOVE_SUP,101b,6,"CMOVE",">",CMOVE_SUP
              pop       ax
              pop       bx
              pop       cx
              push      ds
              push      cx
              push      ds
              push      bx
              push      ax
              lea       bx,_LCMOVE_SUP
              jmp       [bx]

MOT@          MOVE,101b,4,"MOV","E",MOVE
              pop       ax
              pop       bx
              pop       cx
              push      ds
              push      cx
              push      ds
              push      bx
              push      ax
              lea       bx,_LMOVE
              jmp       [bx]

MOT@          PLACE,101b,5,"PLAC","E",PAR_2POINT
              dw        _3DOP
              dw        _1PLUS
              dw        _SWAP
              dw        _MOVE
              dw        _CSTORE
              dw        _DROP
              dw        _UNNEST

;
; Adressage des piles
;

MOT@          SPFETCH,101b,3,"SP","@",SPFETCH
              mov       ax,sp
              jmp       _1PUSH

MOT@          SPSTORE,101b,3,"SP","!",SPSTORE
              pop       sp
              jmp       _NEXT

MOT@          RPFETCH,101b,3,"RP","@",RPFETCH
              mov       ax,bp
              jmp       _1PUSH

MOT@          RPSTORE,101b,3,"RP","!",RPSTORE
              pop       bp
              jmp       _NEXT

MOT@          DEPTH,101b,5,"DEPT","H",PAR_2POINT
              dw        _SP0
              dw        _FETCH
              dw        _SPFETCH
              dw        _MOINS
              dw        _2DIV
              dw        _1MOINS
              dw        _UNNEST

MOT@          RDEPTH,101b,6,"RDEPT","H",PAR_2POINT
              dw        _RP0
              dw        _FETCH
              dw        _RPFETCH
              dw        _MOINS
              dw        _2DIV
              dw        _1MOINS
              dw        _UNNEST

MOT@          DROP,101b,4,"DRO","P",DROP
              pop       ax
              jmp       _NEXT

MOT@          DOP,101b,3,"DU","P",DOP
              pop       ax
              push      ax
              jmp       _1PUSH

MOT@          INT_DOP,101b,4,"?DU","P",INT_DOP
              pop       ax
              or        ax,ax
              jz        _INTDU0
              push      ax
_INTDU0:      jmp       _1PUSH

MOT@          SWAP,101b,4,"SWA","P",SWAP
              pop       dx
              pop       ax
              jmp       _2PUSH

MOT@          OVER,101b,4,"OVE","R",OVER
              pop       dx
              pop       ax
              push      ax
              jmp       _2PUSH

MOT@          ROT,101b,3,"RO","T",ROT
              pop       dx
              pop       bx
              pop       ax
              push      bx
              jmp       _2PUSH

MOT@          MROT,101b,4,"-RO","T",MROT
              pop       bx
              pop       ax
              pop       dx
              push      bx
              jmp       _2PUSH

MOT@          PICK,101b,4,"PIC","K",PICK
              pop       bx
              shl       bx,1
              add       bx,sp
              mov       ax,ss:[bx]
              jmp       _1PUSH

MOT@          ROLL,101b,4,"ROL","L",PAR_2POINT
              dw        _SUP_R
              dw        _R_FETCH
              dw        _PICK
              dw        _SPFETCH
              dw        _SSEGMENT
              dw        _SWAP
              dw        _2DOP
              dw        _2PLUS
              dw        _R_SUP
              dw        _1PLUS
              dw        _2MUL
              dw        _LCMOVE_SUP
              dw        _DROP
              dw        _UNNEST

MOT@          TUCK,101b,4,"TUC","K",TUCK
              pop       ax
              pop       dx
              push      ax
              jmp       _2PUSH

MOT@          NIP,101b,3,"NI","P",NIP
              pop       ax
              pop       dx
              jmp       _1PUSH

MOT@          FLIP,101b,4,"FLI","P",FLIP
              pop       ax
              xchg      ah,al
              jmp       _1PUSH

MOT@          R_SUP,101b,2,"R",">",R_SUP
              mov       ax,[bp]
              inc       bp
              inc       bp
              jmp       _1PUSH

MOT@          SUP_R,101b,2,">","R",SUP_R
              pop       ax
              dec       bp
              dec       bp
              mov       [bp],ax
              jmp       _NEXT

MOT@          R_FETCH,101b,2,"R","@",R_FETCH
              mov       ax,[bp]
              jmp       _1PUSH

;
; Commandes logiques
;

MOT@          AND,101b,3,"AN","D",AND
              pop       ax
              pop       dx
              and       ax,dx
              jmp       _1PUSH

MOT@          OR,101b,2,"O","R",OR
              pop       ax
              pop       dx
              or        ax,dx
              jmp       _1PUSH

MOT@          XOR,101b,3,"XO","R",XOR
              pop       ax
              pop       dx
              xor       ax,dx
              jmp       _1PUSH

MOT@          NOT,101b,3,"NO","T",NOT
              pop       ax
              not       ax
              jmp       _1PUSH

MOT@          TRUE,101b,4,"TRU","E",TRUE
              mov       ax,-1
              jmp       _1PUSH

MOT@          FALSE,101b,5,"FALS","E",FALSE
              mov       ax,0
              jmp       _1PUSH

MOT@          CTOGGLE,101b,7,"CTOGGL","E",CTOGGLE
              pop       bx
              pop       ax
              xor       [bx],al
              jmp       _NEXT

MOT@          ON,101b,2,"O","N",ON
              pop       bx
              mov       word ptr [bx],-1
              jmp       _NEXT

MOT@          OFF,101b,3,"OF","F",OFF
              pop       bx
              mov       word ptr [bx],0
              jmp       _NEXT

;
; Arithm‚tiques 16 bits
;

MOT@          PLUS,101b,1,"","+",PLUS
              pop       bx
              pop       ax
              add       ax,bx
              jmp       _1PUSH

MOT@          MOINS,101b,1,"","-",MOINS
              pop       bx
              pop       ax
              sub       ax,bx
              jmp       _1PUSH

MOT@          NEGATE,101b,6,"NEGAT","E",NEGATE
              pop       ax
              neg       ax
              jmp       _1PUSH

MOT@          INT_NEGATE,101b,7,"?NEGAT","E",INT_NEGATE
              pop       ax
              or        ax,ax
              js        _NEGATE+2
              jmp       _NEXT

MOT@          ABS,101b,3,"AB","S",ABS
              pop       ax
              or        ax,ax
              jg        _ABS0
              neg       ax
_ABS0:        jmp       _1PUSH

MOT@          PSTORE,101b,2,"+","!",PSTORE
              pop       bx
              pop       ax
              add       [bx],ax
              jmp       _NEXT

MOT@          MSTORE,101b,2,"-","!",MSTORE
              pop       bx
              pop       ax
              sub       [bx],ax
              jmp       _NEXT

;
; Constantes machine
;

MOT@          ZERO,101b,1,"","0",ZERO
              mov       ax,0
              jmp       _1PUSH

MOT@          UN,101b,1,"","1",UN
              mov       ax,1
              jmp       _1PUSH

MOT@          DEUX,101b,1,"","2",DEUX
              mov       ax,2
              jmp       _1PUSH

MOT@          TROIS,101b,1,"","3",TROIS
              mov       ax,3
              jmp       _1PUSH

MOT@          2MUL,101b,2,"2","*",2MUL
              pop       ax
              shl       ax,1
              jmp       _1PUSH

MOT@          2DIV,101b,2,"2","/",2DIV
              pop       ax
              sar       ax,1
              jmp       _1PUSH

MOT@          1PLUS,101b,2,"1","+",1PLUS
              pop       ax
              inc       ax
              jmp       _1PUSH

MOT@          2PLUS,101b,2,"2","+",2PLUS
              pop       ax
              inc       ax
              inc       ax
              jmp       _1PUSH

MOT@          1MOINS,101b,2,"1","-",1MOINS
              pop       ax
              dec       ax
              jmp       _1PUSH

MOT@          2MOINS,101b,2,"2","-",2MOINS
              pop       ax
              dec       ax
              dec       ax
              jmp       _1PUSH

MOT@          1PSTORE,101b,3,"1+","!",1PSTORE
              pop       bx
              inc       word ptr [bx]
              jmp       _NEXT

MOT@          1MSTORE,101b,3,"1-","!",1MSTORE
              pop       bx
              dec       word ptr [bx]
              jmp       _NEXT

;
; Commandes de comparaison
;

MOT@          EGAL,101b,1,"","=",EGAL
              pop       ax
              pop       bx
              cmp       bx,ax
              je        $OUI
              jmp       $NON

MOT@          0EGAL,101b,2,"0","=",0EGAL
              pop       ax
              or        ax,ax
              je        $OUI
              jmp       $NON

MOT@          0INF,101b,2,"0","<",0INF
              pop       ax
              or        ax,ax
              js        $OUI
              jmp       $NON

MOT@          0SUP,101b,2,"0",">",0SUP
              pop       ax
              or        ax,ax
              jg        $OUI
              jmp       $NON

MOT@          0DIFF,101b,3,"0<",">",0DIFF
              pop       ax
              or        ax,ax
              jne       $OUI
              jmp       $NON

MOT@          DIFF,101b,2,"<",">",DIFF
              pop       ax
              pop       bx
              cmp       bx,ax
              je        $NON
              jmp       $OUI

MOT@          YYES,101b,3,"YE","S",YYES
$OUI:         mov       ax,-1
              jmp       _1PUSH

MOT@          NNO,101b,2,"N","O",NNO
$NON:         xor       ax,ax
              jmp       _1PUSH

MOT@          UINF,101b,2,"U","<",UINF
              pop       ax
              pop       bx
              cmp       bx,ax
              jb        $OUI
              jmp       $NON

MOT@          USUP,101b,2,"U",">",USUP
              pop       ax
              pop       bx
              cmp       bx,ax
              jb        $NON
              jmp       $OUI

MOT@          INF,101b,1,"","<",INF
              pop       ax
              pop       bx
              cmp       bx,ax
              jl        $OUI
              jmp       $NON

MOT@          SUP,101b,1,"",">",SUP
              pop       ax
              pop       bx
              cmp       bx,ax
              jg        $OUI
              jmp       $NON

MOT@          MIN,101b,3,"MI","N",MIN
              pop       ax
              pop       bx
              cmp       ax,bx
              jng       _MIN_00
              mov       ax,bx
_MIN_00:      jmp       _1PUSH

MOT@          MAX,101b,3,"MA","X",MAX
              pop       bx
              pop       ax
              cmp       bx,ax
              jng       _MAX_00
              mov       ax,bx
_MAX_00:      jmp       _1PUSH

MOT@          CSET,101b,4,"CSE","T",CSET
              pop       bx
              pop       ax
              or        [bx],al
              jmp       _NEXT

MOT@          BETWEEN,101b,7,"BETWEE","N",PAR_2POINT
              dw        _SUP_R
              dw        _OVER
              dw        _SUP
              dw        _SWAP
              dw        _R_SUP
              dw        _SUP
              dw        _OR
              dw        _NOT
              dw        _UNNEST

;
; Commandes d'entiers doubles
;

MOT@          2DROP,101b,5,"2DRO","P",2DROP
              pop       ax
              pop       ax
              jmp       _NEXT

MOT@          2DOP,101b,4,"2DU","P",2DOP
              pop       ax
              pop       dx
              push      dx
              push      ax
              jmp       _2PUSH

MOT@          2SWAP,101b,5,"2SWA","P",2SWAP
              pop       cx
              pop       bx
              pop       ax
              pop       dx
              push      bx
              push      cx
              jmp       _2PUSH

MOT@          2OVER,101b,5,"2OVE","R",2OVER
              pop       cx
              pop       bx
              pop       ax
              pop       dx
              push      dx
              push      ax
              push      bx
              push      cx
              jmp       _2PUSH

MOT@          3DOP,101b,4,"3DU","P",3DOP
              pop       ax
              pop       dx
              pop       bx
              push      bx
              push      dx
              push      ax
              push      bx
              jmp       _2PUSH

MOT@          4DOP,101b,4,"4DU","P",PAR_2POINT
              dw        _2OVER
              dw        _2OVER
              dw        _UNNEST

MOT@          2ROT,101b,4,"2RO","T",PAR_2POINT
              dw        _PAR_LIT
              dw        5
              dw        _ROLL
              dw        _PAR_LIT
              dw        5
              dw        _ROLL
              dw        _UNNEST

;
; Arithm‚tique 32 bits
;

MOT@          DPLUS,101b,2,"D","+",DPLUS
              pop       ax
              pop       dx
              pop       bx
              pop       cx
              add       dx,cx
              adc       ax,bx
              jmp       _2PUSH

MOT@          DNEGATE,101b,7,"DNEGAT","E",DNEGATE
              pop       bx
              pop       cx
              sub       ax,ax
              mov       dx,ax
              sub       dx,cx
              sbb       ax,bx
              jmp       _2PUSH

MOT@          INT_DNEGATE,101b,8,"?DNEGAT","E",PAR_2POINT
              dw        _0INF
              dw        _INT_BRANCH
              dw        _INT_DNEG_00
              dw        _DNEGATE
_INT_DNEG_00: dw        _UNNEST

MOT@          DABS,101b,4,"DAB","S",DABS
              pop       dx
              push      dx
              or        dx,dx
              js        _DNEGATE+2
              jmp       _NEXT

;
; Arithm‚tique mixte
;

MOT@          MULD,101b,2,"*","D",PAR_2POINT
              dw        _2DOP
              dw        _XOR
              dw        _SUP_R
              dw        _ABS
              dw        _SWAP
              dw        _ABS
              dw        _UMMUL
              dw        _R_SUP
              dw        _INT_DNEGATE
              dw        _UNNEST

MOT@          MDIVMOD,101b,5,"M/MO","D",PAR_2POINT
              dw        _INT_DOP
              dw        _INT_BRANCH
              dw        _MDIVMOD_01
              dw        _DOP
              dw        _SUP_R
              dw        _2DOP
              dw        _XOR
              dw        _SUP_R
              dw        _SUP_R
              dw        _DABS
              dw        _R_FETCH
              dw        _ABS
              dw        _UMDIVMOD
              dw        _SWAP
              dw        _R_SUP
              dw        _INT_NEGATE
              dw        _SWAP
              dw        _R_SUP
              dw        _0INF
              dw        _INT_BRANCH
              dw        _MDIVMOD_00
              dw        _NEGATE
              dw        _OVER
              dw        _INT_BRANCH
              dw        _MDIVMOD_00
              dw        _1MOINS
              dw        _R_FETCH
              dw        _ROT
              dw        _MOINS
              dw        _SWAP
_MDIVMOD_00:  dw        _R_SUP
              dw        _DROP
_MDIVMOD_01:  dw        _UNNEST

MOT@          MUDIVMOD,101b,6,"MU/MO","D",PAR_2POINT
              dw        _SUP_R
              dw        _PAR_LIT
              dw        0
              dw        _R_FETCH
              dw        _UMDIVMOD
              dw        _R_SUP
              dw        _SWAP
              dw        _SUP_R
              dw        _UMDIVMOD
              dw        _R_SUP
              dw        _UNNEST

;
; Multiplications, Division et Restes 16 bits
;

MOT@          MUL,101b,1,"","*",MUL
              pop       cx
              pop       ax
              mul       cx
              jmp       _1PUSH

MOT@          DIV,101b,1,"","/",DIV
              pop       cx
              pop       ax
              xor       dx,dx
              div       cx
              jmp       _1PUSH

MOT@          MOD,101b,3,"MO","D",MOD
              pop       cx
              pop       ax
              xor       dx,dx
              div       cx
              mov       ax,dx
              jmp       _1PUSH

MOT@          DIVMOD,101b,4,"/MO","D",DIVMOD
              pop       cx
              pop       ax
              xor       dx,dx
              div       cx
              jmp       _2PUSH

MOT@          MULDIV,101b,2,"*","/",MULDIV
              pop       cx
              pop       bx
              pop       ax
              mul       bx
              div       cx
              jmp       _1PUSH

MOT@          MULDIVMOD,101b,5,"*/MO","D",PAR_2POINT
              dw        _SUP_R
              dw        _MULD
              dw        _R_SUP
              dw        _MDIVMOD
              dw        _UNNEST

;
; Multiplications, Divisions et Restes 16-32 bits
;

MOT@          UMMUL,101b,3,"UM","*",UMMUL
              pop       ax
              pop       bx
              mul       bx
              xchg      ax,dx
              jmp       _2PUSH

MOT@          UMULD,101b,3,"U*","D",UMULD
              jmp       _UMMUL+2

MOT@          UMDIVMOD,101b,6,"UM/MO","D",UMDIVMOD
              pop       bx
              pop       dx
              pop       ax
              cmp       dx,bx
              jl        _UMDIVMOD_00
              mov       ax,-1
              mov       dx,ax
              jmp       _2PUSH
_UMDIVMOD_00: div       bx
              jmp       _2PUSH

;
; Commandes de chaines
;

MOT@          FILL,101b,4,"FIL","L",FILL
              cld
              mov       ax,ds
              mov       es,ax
              pop       ax
              pop       cx
              pop       di
              rep       stosb
              jmp       _NEXT

MOT@          ERASE,101b,5,"ERAS","E",PAR_2POINT
              dw        _PAR_LIT
              dw        0
              dw        _FILL
              dw        _UNNEST

MOT@          COUNT,101b,5,"COUN","T",COUNT
              pop       bx
              mov       al,[bx]
              xor       ah,ah
              inc       bx
              push      bx
              jmp       _1PUSH

MOT@          S_UPPER,101b,6,">UPPE","R",PAR_CREATE
_SUP_UPPER:   cmp       al,"a"
              jl        _SUP_UP_00
              cmp       al,"z"
              jg        _SUP_UP_00
              sub       al," "
_SUP_UP_00:   ret

MOT@          UPPER,101b,5,"UPPE","R",UPPER
              pop       cx
              pop       bx
              jcxz      _UPPER_01
_UPPER_00:    mov       al,[bx]
              call      _SUP_UPPER
              mov       [bx],al
              inc       bx
              loop      _UPPER_00
_UPPER_01:    jmp       _NEXT

MOT@          UPC,101b,3,"UP","C",UPC
              pop       ax
              call      _SUP_UPPER
              jmp       _1PUSH

MOT@          INT_CHAR,101b,5,"?CHA","R",PAR_2POINT
              dw        _CAPS
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _INT_CHAR0
              dw        _UPC
_INT_CHAR0:   dw        _UNNEST

MOT@          COMP,101b,4,"COM","P",COMP
              mov       dx,si
              pop       cx
              pop       di
              pop       si
              jcxz      _NOMORE
              mov       ax,ds
              mov       es,ax
              repz      cmpsb
              je        _NOMORE

_MISMATCH:    jns       _COMP_00
              mov       cx,-1
              jmp       _NOMORE
_COMP_00:     mov       cx,1
_NOMORE:      mov       si,dx
              push      cx
              jmp       _NEXT

MOT@          CAPS_COMP,101b,9,"CAPS-COM","P",CAPS_COMP
              mov       dx,si
              pop       cx
              pop       di
              pop       si
              jcxz      _NOMORE
_CAPS_C_00:   mov       al,[si]
              call      _SUP_UPPER
              inc       si
              mov       ah,al
              mov       al,[di]
              call      _SUP_UPPER
              inc       di
              cmp       ah,al
              jne       _MISMATCH
              loop      _CAPS_C_00
              jmp       _NOMORE

MOT@          COMPARE,101b,7,"COMPAR","E",PAR_2POINT
              dw        _CAPS
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _COMPARE_00
              dw        _CAPS_COMP
              dw        _BRANCH
              dw        _COMPARE_01
_COMPARE_00:  dw        _COMP
_COMPARE_01:  dw        _UNNEST

;
; FIN DE MATH.INC
; 

