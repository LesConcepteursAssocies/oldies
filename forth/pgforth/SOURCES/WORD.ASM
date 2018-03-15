;
; WORD.INC
; TRAITEMENT ENTREES/SORTIES ALPHA
;

MOT@          PAR_KEY_INT,101b,6,"(KEY?",")",PAR_2POINT
              dw        _PAR_LIT
              dw        0Bh
              dw        _PAR_IN
              dw        _0DIFF
              dw        _UNNEST

MOT@          PAR_KEY,101b,5,"(KEY",")",PAR_2POINT
_PAR_KEY_00:  dw        _PAR_KEY_INT
              dw        _INT_BRANCH
              dw        _PAR_KEY_00
              dw        _PAR_LIT
              dw        07h
              dw        _PAR_IN
              dw        _UNNEST

MOT@          KEY,101b,3,"KE","Y",PAR_DEFER
              dw        _PAR_KEY

MOT@          KEY_INT,101b,4,"KEY","?",PAR_DEFER
              dw        _PAR_KEY_INT

MOT@          PAR_CONS,101b,6,"(CONS",")",PAR_2POINT
              dw        _PAR_LIT
              dw        06h
              dw        _PAR_OUT
              dw        _UNNEST

MOT@          PAR_CONSOLE,101b,9,"(CONSOLE",")",PAR_2POINT
              dw        _PAR_CONS
              dw        _SHA_OUT
              dw        _1PSTORE
              dw        _UNNEST

MOT@          PAR_CCONSOLE,101b,10,"(CCONSOLE",")",PAR_DEFER
              dw        _NOOP

MOT@          PAR_PRINT,101b,7,"(PRINT",")",PAR_2POINT
              dw        _PAR_PRN
              dw        _SHA_OUT
              dw        _1PSTORE
              dw        _UNNEST

MOT@          PAR_EMIT,101b,6,"(EMIT",")",PAR_2POINT
              dw        _PRINTING
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PAR_EMIT_00
              dw        _DOP
              dw        _PAR_PRINT
              dw        _SHA_OUT
              dw        _1MSTORE
_PAR_EMIT_00: dw        _COLOR_INT
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PAR_EMIT_01
              dw        _PAR_CCONSOLE
              dw        _BRANCH
              dw        _PAR_EMIT_02
_PAR_EMIT_01: dw        _PAR_CONSOLE
_PAR_EMIT_02: dw        _UNNEST

MOT@          EMIT,101b,4,"EMI","T",PAR_DEFER
              dw        _PAR_EMIT

MOT@          PAR_ATTRIB,101b,8,"(ATTRIB",")",PAR_2POINT
              dw        _ATTRIBUTS
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PAR_ATT_00
              dw        _PAR_LIT
              dw        27
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "["
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "0"
              dw        _PLUS
              dw        _PAR_CONS
              dw        _PAR_LIT
              dw        "m"
              dw        _PAR_CONS
              dw        _BRANCH
              dw        _PAR_ATT_01
_PAR_ATT_00:  dw        _DROP
_PAR_ATT_01:  dw        _UNNEST

MOT@          ATTOFF,101b,6,"ATTOF","F",PAR_2POINT
              dw        _PAR_LIT
              dw        0
              dw        _PAR_ATTRIB
              dw        _UNNEST

MOT@          BOLD,101b,4,"BOL","D",PAR_2POINT
              dw        _PAR_LIT
              dw        1
              dw        _PAR_ATTRIB
              dw        _UNNEST

MOT@          PAR_CCR,101b,5,"(CCR",")",PAR_DEFER
              dw        _NOOP

MOT@          CRLF,101b,4,"(CR",")",PAR_2POINT
              dw        _COLOR_INT
              dw        _FETCH
              dw        _INT_BRANCH
              dw        _PAR_CR_00
              dw        _PAR_CCR
              dw        _BRANCH
              dw        _PAR_CR_01
_PAR_CR_00:   dw        _PAR_LIT
              dw        13
              dw        _EMIT
              dw        _PAR_LIT
              dw        10
              dw        _EMIT
              dw        _SHA_OUT
              dw        _OFF
              dw        _SHA_LINE
              dw        _1PSTORE
_PAR_CR_01:   dw        _UNNEST

MOT@          CR,101b,2,"C","R",PAR_DEFER
              dw        _CRLF

MOT@          TYPE,101b,4,"TYP","E",PAR_2POINT
              dw        _PAR_LIT
              dw        0
              dw        _PAR_INT_DO
              dw        _TYPE1
_TYPE0:       dw        _COUNT
              dw        _EMIT
              dw        _PAR_LOOP
              dw        _TYPE0
_TYPE1:       dw        _DROP
              dw        _UNNEST

MOT@          REPLICATE,101b,9,"REPLICAT","E",PAR_2POINT
              dw        _SWAP
              dw        _PAR_LIT
              dw        0
              dw        _MAX
              dw        _PAR_LIT
              dw        0
              dw        _PAR_INT_DO
              dw        _REPLIC_01
_REPLIC_00:   dw        _DOP
              dw        _EMIT
              dw        _PAR_LOOP
              dw        _REPLIC_00
_REPLIC_01:   dw        _DROP
              dw        _UNNEST

MOT@          SPACE,101b,5,"SPAC","E",PAR_2POINT
              dw        _BL
              dw        _EMIT
              dw        _UNNEST

MOT@          SPACES,101b,6,"SPACE","S",PAR_2POINT
              dw        _BL
              dw        _REPLICATE
              dw        _UNNEST

MOT@          BACKSPACES,101b,10,"BACKSPACE","S",PAR_2POINT
              dw        _BS
              dw        _REPLICATE
              dw        _UNNEST

MOT@          BEEP,101b,4,"BEE","P",PAR_2POINT
              dw        _COLOR_INT
              dw        _FETCH
              dw        _0EGAL
              dw        _INT_BRANCH
              dw        _BEEP_00
              dw        _BELL
              dw        _EMIT
_BEEP_00:     dw        _UNNEST

MOT@          DEL_IN,101b,6,"DEL-I","N",PAR_2POINT
              dw        _DOP
              dw        _INT_BRANCH
              dw        _DEL_IN0
              dw        _1MOINS
              dw        _BS
              dw        _EMIT
              dw        _SPACE
              dw        _BS
              dw        _BRANCH
              dw        _DEL_IN1
_DEL_IN0:     dw        _BELL
_DEL_IN1:     dw        _EMIT
              dw        _UNNEST

MOT@          CR_IN,101b,5,"CR-I","N",PAR_2POINT
              dw        _SPAN
              dw        _STORE
              dw        _OVER
              dw        _BL
              dw        _EMIT
              dw        _UNNEST

MOT@          BS_IN,101b,5,"BS-I","N",PAR_2POINT
              dw        _DROP
              dw        _DOP
              dw        _INT_BRANCH
              dw        _BS_IN0
              dw        _1MOINS
              dw        _BS
              dw        _BRANCH
              dw        _BS_IN1
_BS_IN0:      dw        _BELL
_BS_IN1:      dw        _EMIT
              dw        _UNNEST

MOT@          BACK_UP,101b,7,"BACK-U","P",PAR_2POINT
              dw        _DOP
              dw        _BACKSPACES
              dw        _DOP
              dw        _SPACES
              dw        _BACKSPACES
              dw        _PAR_LIT
              dw        0
              dw        _UNNEST

MOT@          PAR_CHAR,101b,6,"(CHAR",")",PAR_2POINT
              dw        _3DOP
              dw        _EMIT
              dw        _PLUS
              dw        _CSTORE
              dw        _1PLUS
              dw        _UNNEST

MOT@          CHAR,101b,4,"CHA","R",PAR_DEFER
              dw        _PAR_CHAR

MOT@          EXPECT,101b,6,"EXPEC","T",PAR_2POINT
              dw        _DOP
              dw        _SPAN
              dw        _STORE
              dw        _SWAP
              dw        _PAR_LIT
              dw        0
_EXPECT_00:   dw        _PAR_LIT
              dw        2
              dw        _PICK
              dw        _OVER
              dw        _MOINS
              dw        _INT_BRANCH
              dw        _EXPECT_04
              dw        _KEY
              dw        _DOP
              dw        _BL
              dw        _INF
              dw        _INT_BRANCH
              dw        _EXPECT_01
              dw        _2MUL
              dw        _C_C
              dw        _FETCH
              dw        _PLUS
              dw        _PERFORM
              dw        _BRANCH
              dw        _EXPECT_03
_EXPECT_01:   dw        _DOP
              dw        _PAR_LIT
              dw        127
              dw        _EGAL
              dw        _INT_BRANCH
              dw        _EXPECT_02
              dw        _DROP
              dw        _DEL_IN
              dw        _BRANCH
              dw        _EXPECT_03
_EXPECT_02:   dw        _CHAR
_EXPECT_03:   dw        _BRANCH
              dw        _EXPECT_00
_EXPECT_04:   dw        _2DROP
              dw        _DROP
              dw        _UNNEST

MOT@          QUERY,101b,5,"QUER","Y",PAR_2POINT
              dw        _TICK_TIB
              dw        _FETCH
              dw        _CBYL
              dw        _EXPECT
              dw        _SPAN
              dw        _FETCH
              dw        _SHA_TIB
              dw        _CSTORE
              dw        _SUP_IN
              dw        _OFF
              dw        _UNNEST

;
; Op‚rations sur M‚moire Extra-Segment
;


MOT@          SOURCE,101b,6,"SOURC","E",PAR_DEFER
              dw        _PAR_SOURCE

MOT@          PAR_SOURCE,101b,8,"(SOURCE",")",PAR_2POINT
              dw        _BUFFER
              dw        _COUNT
              dw        _UNNEST

MOT@          SKIP,101b,4,"SKI","P",SKIP
              pop       ax
              pop       cx
              jcxz      _DONE
              pop       di
              call      _MOV_DX_CODE
              mov       es,dx
              repz      scasb
              jz        _SKIP0
              inc       cx
              dec       di
_SKIP0:       push      di
_DONE:        push      cx
              jmp       _NEXT


MOT@          SCAN,101b,4,"SCA","N",SCAN
              pop       ax
              pop       cx
              jcxz      _DONE
              pop       di
              call      _MOV_DX_CODE
              mov       es,dx
              mov       bx,cx
              repnz     scasb
              jnz       _SCAN0
              inc       cx
              dec       di
_SCAN0:       push      di
              push      cx
              jmp       _NEXT

MOT@          DIVSTRING,101b,7,"/STRIN","G",DIVSTRING
              pop       bx
              pop       ax
              pop       dx
              cmp       bx,ax
              jle       _DIVSTR_00
              mov       bx,ax
_DIVSTR_00:   add       dx,bx
              sub       ax,bx
              jmp       _2PUSH

MOT@          PARSE_WORD,101b,10,"PARSE-WOR","D",PAR_2POINT
              dw        _SUP_R
              dw        _SOURCE
              dw        _TUCK
              dw        _SUP_IN
              dw        _FETCH
              dw        _DIVSTRING
              dw        _R_FETCH
              dw        _SKIP
              dw        _OVER
              dw        _SWAP
              dw        _R_SUP
              dw        _SCAN
              dw        _SUP_R
              dw        _OVER
              dw        _MOINS
              dw        _ROT
              dw        _R_SUP
              dw        _DOP
              dw        _0DIFF
              dw        _PLUS
              dw        _MOINS
              dw        _SUP_IN
              dw        _STORE
              dw        _UNNEST

MOT@          PARSE,101b,5,"PARS","E",PAR_2POINT
              dw        _SUP_R
              dw        _SOURCE
              dw        _SUP_IN
              dw        _FETCH
              dw        _DIVSTRING
              dw        _OVER
              dw        _SWAP
              dw        _R_SUP
              dw        _SCAN
              dw        _SUP_R
              dw        _OVER
              dw        _MOINS
              dw        _DOP
              dw        _R_SUP
              dw        _0DIFF
              dw        _MOINS
              dw        _SUP_IN
              dw        _PSTORE
              dw        _UNNEST

MOT@          TICK_WORD,101b,5,"'WOR","D",PAR_2POINT
              dw        _HERE
              dw        _PAR_LIT
              dw        256
              dw        _PLUS
              dw        _UNNEST

MOT@          WORD,101b,4,"WOR","D",PAR_2POINT
              dw        _PARSE_WORD
              dw        _TICK_WORD
              dw        _PLACE
              dw        _TICK_WORD
              dw        _DOP
              dw        _COUNT
              dw        _PLUS
              dw        _BL
              dw        _SWAP
              dw        _CSTORE
              dw        _UNNEST

MOT@          PARENTHESE,111b,1,"","(",PAR_2POINT
              dw        _PAR_LIT
              dw        ")"
              dw        _PARSE
              dw        _2DROP
              dw        _UNNEST

MOT@          PPARENTHESE,101b,2,".","(",PAR_2POINT
              dw        _PAR_LIT
              dw        ")"
              dw        _PARSE
              dw        _TYPE
              dw        _UNNEST

MOT@          COMMLIG,111b,1,"","\",PAR_2POINT
              dw        _SUP_IN
              dw        _FETCH
              dw        _NEGATE
              dw        _CBYL
              dw        _MOD
              dw        _SUP_IN
              dw        _PSTORE
              dw        _UNNEST
              
;
; FIN DE WORD.INC
; 

