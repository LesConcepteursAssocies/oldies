;
; FILES.ASM
; Accäs aux fichiers
;

MOT@          PAR_PAR_IN,101b,9,"((STDIN)",")",PAR_CREATE
$OCTET        db        0 
              even

MOT@          PAR_IN,101b,7,"(STDIN",")",PAR_IN
              mov       word ptr [$EOF],0
              cmp       word ptr [$STDIN],0
              jz        _PAR_IN_00
              pop       ax
              mov       cx,1
              mov       byte ptr [$OCTET],0
              lea       dx,$OCTET
              mov       bx,[$STDIN]
              mov       ah,3Fh
              int       21h
              jnc       _PAR_IN_01
              mov       word ptr [$EOF],-1
              mov       word ptr [$STDIN],0
_PAR_IN_01:   mov       al,[$OCTET]
              xor       ah,ah
              jmp       _1PUSH
_PAR_IN_00:   pop       ax
              mov       ah,al
              int       21h
              xor       ah,ah
              jmp       _1PUSH

MOT@          PAR_OUT,101b,8,"(STDOUT",")",PAR_OUT
              cmp       word ptr [$STDOUT],1
              je        _PAR_OUT_00
              pop       ax
              pop       ax
              mov       [$OCTET],al
              lea       dx,$OCTET
              mov       bx,[$STDOUT]
              mov       cx,1
              mov       ah,40h
              int       21h
              jmp       _NEXT
_PAR_OUT_00:  pop       ax
              mov       ah,al
              cmp       ah,09h
              je        _PAR_OUT_01
              pop       dx
              int       21h
              jmp       _NEXT
_PAR_OUT_01:  pop       bx
              xor       bh,bh
              mov       cx,1
              pop       dx
              mov       al,dl
              int       10h
              jmp       _NEXT

MOT@          PAR_PRN,101b,8,"(STDPRN",")",PAR_PRN
              pop       ax
              mov       [$OCTET],al
              lea       dx,$OCTET
              mov       bx,[$STDPRN]
              mov       cx,1
              mov       ah,40h
              int       21h
              jmp       _NEXT

MOT@          AUX_IN,101b,7,"(AUXIN",")",AUX_IN
              mov       word ptr [$EOF],0
              mov       cx,1
              mov       byte ptr [$OCTET],0
              lea       dx,$OCTET
              mov       bx,[$AUXIN]
              mov       ah,3Fh
              int       21h
              jnc       _AUX_IN_00
              mov       word ptr [$EOF],-1
              mov       word ptr [$AUXIN],3
_AUX_IN_00:   mov       al,[$OCTET]
              xor       ah,ah
              jmp       _1PUSH

MOT@          AUX_OUT,101b,8,"(AUXOUT",")",AUX_OUT
              pop       ax
              mov       [$OCTET],al
              lea       dx,$OCTET
              mov       bx,[$AUXOUT]
              mov       cx,1
              mov       ah,40h
              int       21h
              jmp       _NEXT

MOT@          PAR_OPEN,101b,6,"(OPEN",")",PAR_OPEN
              pop       ax
              pop       dx
              mov       ah,3Dh
              int       21h
              push      ax
              jnc       _PAR_OPEN_00
              jmp       _1PUSH
_PAR_OPEN_00: inc       word ptr [$SHA_FILES]
              xor       ax,ax
              jmp       _1PUSH

MOT@          PAR_CLOSE,101b,7,"(CLOSE",")",PAR_CLOSE
              pop       bx
              mov       ah,3Eh
              int       21h
              cmp       word ptr [$SHA_FILES],5
              jg        _PCLOSE_00
              mov       word ptr [$SHA_FILES],6
_PCLOSE_00:   dec       word ptr [$SHA_FILES]
              jmp       _NEXT

MOT@          PAR_GET,101b,5,"(GET",")",PAR_GET
              mov       ax,ds
              mov       es,ax
              pop       bx
              pop       cx
              pop       dx
              pop       ds
              mov       ah,3Fh
              int       21h
              push      ax
              mov       bx,es
              mov       ds,bx
              jc        _PAR_GET_00
              xor       ax,ax
_PAR_GET_00:  jmp       _1PUSH

MOT@          PAR_GETLINE,101b,9,"(GETLINE",")",PAR_GETLINE
              pop       bx
              pop       dx
              mov       di,dx
              mov       byte ptr [di],0
              jmp       _PGETL_01
_PGETL_00:    mov       al,[di]
              cmp       al,0
              jnz       _PGETL_01
              mov       ax,-1
              jmp       _1PUSH
_PGETL_01:    inc       dx
              mov       cx,1
              mov       ah,3Fh
              int       21h
              jnc       _PGETL_02
              mov       ax,-1
              jmp       _1PUSH
_PGETL_02:    cmp       ax,cx
              jz        _PGETL_03
              mov       ax,-1
              jmp       _1PUSH
_PGETL_03:    mov       cx,di
              mov       di,dx
              mov       al,[di]
              sub       ah,ah
              cmp       ax,0Ah
              jz        _PGETL_05
              cmp       ax,20h
              jnl       _PGETL_04
              mov       al,20h
              mov       [di],al
_PGETL_04:    mov       di,cx
              inc       byte ptr [di]
              jmp       _PGETL_00
_PGETL_05:    xor       ax,ax
              jmp       _1PUSH

MOT@          INT_DOSERR,101b,7,"?DOSER","R",PAR_2POINT
              dw        _INT_DOP
              dw        _INT_BRANCH
              dw        _INT_DOS_00
              dw        _TICK_WORD
              dw        _COUNT
              dw        _2DOP
              dw        _UPPER
              dw        _BOLD
              dw        _TYPE
              dw        _SPACE
              dw        _1MOINS
              dw        _PAR_LIT
              dw        35
              dw        _MIN
              dw        _PAR_LIT
              dw        50
              dw        _MUL
              dw        _ERRMSG
              dw        _PLUS
              dw        _COUNT
              dw        _TYPE
              dw        _ATTOFF
              dw        _ABORT
_INT_DOS_00:  dw        _UNNEST

MOT@          EOF,101b,3,"EO","F",PAR_2POINT
              dw        _EOF_INT
              dw        _ON
              dw        _UNNEST

MOT@          BUFFER,101b,6,"BUFFE","R",PAR_2POINT
              dw        _LBUF
              dw        _CPATH
              dw        _PLUS
              dw        _2PLUS
              dw        _HANDLE
              dw        _FETCH
              dw        _MUL
              dw        _BUFFERS
              dw        _PLUS
              dw        _UNNEST

MOT@          PATHNAME,101b,8,"PATHNAM","E",PAR_2POINT
              dw        _BUFFER
              dw        _LBUF
              dw        _PLUS
              dw        _UNNEST

MOT@          LINE_SHA,101b,5,"LINE","#",PAR_2POINT
              dw        _PATHNAME
              dw        _CPATH
              dw        _PLUS
              dw        _UNNEST

MOT@          SET_DTA,101b,7,"SET-DT","A",SET_DTA
              pop       dx
              mov       ah,1Ah
              int       21h
              jmp       _NEXT

MOT@          GET_DTA,101b,7,"GET-DT","A",GET_DTA
              mov       ah,2Fh
              int       21h
              push      bx
              jmp       _NEXT

MOT@          ASCII0,101b,6,"ASCII","0",PAR_2POINT
              dw        _BL
              dw        _WORD
              dw        _COUNT
              dw        _OVER
              dw        _PLUS
              dw        _PAR_LIT
              dw        0
              dw        _SWAP
              dw        _CSTORE
              dw        _UNNEST

MOT@          CLOSE,101b,5,"CLOS","E",PAR_2POINT
              dw        _HANDLE
              dw        _FETCH
              dw        _PAR_CLOSE
              dw        _EOF_INT
              dw        _OFF
              dw        _UNNEST

MOT@          ALLCLOSE,101b,8,"ALLCLOS","E",PAR_2POINT
              dw        _FILES
              dw        _1PLUS
              dw        _PAR_LIT
              dw        5
              dw        _PAR_DO
              dw        _ALLCLO_01
_ALLCLO_00:   dw        _I
              dw        _PAR_CLOSE
              dw        _PAR_LOOP
              dw        _ALLCLO_00
_ALLCLO_01:   dw        _UNNEST

MOT@          PATH_STORE,101b,9,"PATHNAME","!",PAR_2POINT
              dw        _PATHNAME
              dw        _DOP
              dw        _CPATH
              dw        _ERASE
              dw        _OVER
              dw        _CFETCH
              dw        _1PLUS
              dw        _CPATH
              dw        _MIN
              dw        _CMOVE
              dw        _UNNEST

MOT@          INT_OPEN,101b,5,"?OPE","N",PAR_2POINT
              dw        _SHA_FILES
              dw        _1MOINS
              dw        _FILES
              dw        _EGAL
              dw        _PAR_ABORTG
STR@          24,<"Trop de fichiers ouverts">
              dw        _UNNEST

MOT@          OPEN,101b,4,"OPE","N",PAR_2POINT
              dw        _INT_OPEN
              dw        _ASCII0
              dw        _SWAP
              dw        _PAR_OPEN
              dw        _INT_DOSERR
              dw        _HANDLE
              dw        _FETCH
              dw        _SUP_R
              dw        _HANDLE
              dw        _STORE
              dw        _BUFFER
              dw        _OFF
              dw        _TICK_WORD
              dw        _PATH_STORE
              dw        _LINE_SHA
              dw        _OFF
              dw        _HANDLE
              dw        _FETCH
              dw        _R_SUP
              dw        _HANDLE
              dw        _STORE
              dw        _UNNEST

MOT@          GET,101b,3,"GE","T",PAR_2POINT
              dw        _HANDLE
              dw        _FETCH
              dw        _0EGAL
              dw        _INT_BRANCH
              dw        _GET_03
              dw        _PAR_KEY
              dw        _BRANCH
              dw        _GET_03
_GET_00:      dw        _BUFFER
              dw        _PAR_LIT
              dw        1
              dw        _HANDLE
              dw        _FETCH
              dw        _PAR_GET
              dw        _0DIFF
              dw        _INT_BRANCH
              dw        _GET_01
              dw        _DROP
              dw        _EOF
              dw        _PAR_LIT
              dw        13
              dw        _BRANCH
              dw        _GET_03
_GET_01:      dw        _PAR_LIT
              dw        1
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _GET_02
              dw        _BUFFER
              dw        _CFETCH
              dw        _BRANCH
              dw        _GET_03
_GET_02:      dw        _EOF
              dw        _PAR_LIT
              dw        13
_GET_03:      dw        _UNNEST

MOT@          PLINE,101b,5,".LIN","E",PAR_2POINT
              dw        _BUFFER
              dw        _COUNT
              dw        _CR
              dw        _TYPE
              dw        _UNNEST

MOT@          GETLINE,101b,7,"GETLIN","E",PAR_2POINT
              dw        _BUFFER
              dw        _HANDLE
              dw        _FETCH
              dw        _PAR_GETLINE
              dw        _EOF_INT
              dw        _STORE
              dw        _ECHO
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _GETLINE_00
              dw        _PLINE
_GETLINE_00:  dw        _LINE_SHA
              dw        _1PSTORE
              dw        _UNNEST

MOT@          PAR_INCLUDE,101b,9,"(INCLUDE",")",PAR_2POINT
              dw        _HANDLE
              dw        _FETCH
              dw        _SUP_R
              dw        _SUP_IN
              dw        _FETCH
              dw        _SUP_R
              dw        _HANDLE
              dw        _STORE
_INCLUDE_00:  dw        _GETLINE
              dw        _SUP_IN
              dw        _OFF
              dw        _RUN
              dw        _EOF_INT
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _INCLUDE_00
              dw        _CLOSE
              dw        _R_SUP
              dw        _SUP_IN
              dw        _STORE
              dw        _R_SUP
              dw        _HANDLE
              dw        _STORE
              dw        _UNNEST

MOT@          INCLUDE,111b,7,"INCLUD","E",PAR_2POINT
              dw        _READ
              dw        _OPEN
              dw        _PAR_INCLUDE
              dw        _UNNEST

;
; FIN DE FILES.ASM
;

