ECHO ON
\ ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
\ ³                                                                           ³
\ ³                               F G D R I V E R                             ³
\ ³                                                                           ³
\ ³              Vocabulaire d'utilisation des fonctions graphiques           ³
\ ³                             Copyright MP7 1993                            ³
\ ³                       M. PETREMANN & Ph. GUILLAUMAUD                      ³
\ ³                                                                           ³
\ ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
ECHO OFF

ONLY FORTH ALSO ASSEMBLER ALSO ROOT DEFINITIONS
VOCABULARY FASTGRAF
WARNING ON
FASTGRAF ALSO DEFINITIONS

DECIMAL
CREATE FGVPARAM 12 ALLOT               \ Table de ParamŠtres vid‚o internes … FG.
HEX
CREATE FGZPARAM                        \ Utilis‚ par quelques fonctions de FG.
90 C, 90 C,

\
\ Renvoie Vrai si le driver est install‚.
\
CODE FASTGRAPH? ( --- fl )
   35 # AH MOV
   62 # AL MOV
   21 INT
   ES: 0A [BX] AX MOV
   AH AL XCHG
   4647 # AX CMP
   0=
   IF
      TRUE # AX MOV
      1PUSH
   THEN
   FALSE # AX MOV
   1PUSH
END-CODE

: >FGDRIVER ( nfunc --- )
   # AX MOV
   62 INT
;

CODE ALLOCATE ( pagenumber --- status )
   BX POP
   00 >FGDRIVER
   1PUSH
END-CODE

CODE ALLOCCMS ( pagenumber --- status )
   BX POP
   01 >FGDRIVER
   1PUSH
END-CODE

CODE ALLOCEMS ( pagenumber --- status )
   BX POP
   02 >FGDRIVER
   1PUSH
END-CODE

CODE ALLOCXMS ( pagenumber --- status )
   BX POP
   03 >FGDRIVER
   1PUSH
END-CODE

CODE AUTOMODE ( --- videomode )
   04 >FGDRIVER
   1PUSH
END-CODE

CODE BESTMODE ( horizontal vertical pages --- videomode | -1 )
   DX POP
   CX POP
   BX POP
   05 >FGDRIVER
   1PUSH
END-CODE

CODE BOX ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   06 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE BOXDEPTH ( xdepth ydepth --- )
   CX POP
   BX POP
   07 >FGDRIVER
   NEXT
END-CODE

CODE BOXX ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   08 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE BUTTON ( n --- status )
   BX POP
   09 >FGDRIVER
   1PUSH
END-CODE

CODE CAPSLOCK ( --- status )
   0A >FGDRIVER
   1PUSH
END-CODE

CODE CHGATTR ( nbchar --- )
   BX POP
   0B >FGDRIVER
   NEXT
END-CODE

CODE CHGTEXT ( str len --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   0C >FGDRIVER
   NEXT
END-CODE

CODE CIRCLE ( radius --- )
   BX POP
   0D >FGDRIVER
   NEXT
END-CODE

CODE CIRCLEF ( radius --- )
   BX POP
   0E >FGDRIVER
   NEXT
END-CODE

CODE CLIPMASK ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   0F >FGDRIVER
   NEXT
END-CODE

CODE CLPIMAGE ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   10 >FGDRIVER
   NEXT
END-CODE

CODE CLPRECT ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   11 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE COPYPAGE ( pagesource pagedest --- )
   CX POP
   BX POP
   12 >FGDRIVER
   NEXT
END-CODE

CODE CURSOR ( status --- )
   BX POP
   13 >FGDRIVER
   NEXT
END-CODE

CODE DASH ( ix iy pattern --- )
   DX POP
   CX POP
   BX POP
   14 >FGDRIVER
   NEXT
END-CODE

CODE DASHREL ( ix iy pattern --- )
   DX POP
   CX POP
   BX POP
   15 >FGDRIVER
   NEXT
END-CODE

CODE DEFCOLOR ( index value --- )
   CX POP
   BX POP
   16 >FGDRIVER
   NEXT
END-CODE

CODE DEFPAGES ( source_page dest_page --- )
   CX POP
   BX POP
   17 >FGDRIVER
   NEXT
END-CODE

CODE DISPFILE ( str width format --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   18 >FGDRIVER
   NEXT
END-CODE

CODE DISPLAY ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   19 >FGDRIVER
   NEXT
END-CODE

CODE DISPLAYP ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   1A >FGDRIVER
   NEXT
END-CODE

CODE DRAW ( ix iy --- )
   CX POP
   BX POP
   1B >FGDRIVER
   NEXT
END-CODE

CODE DRAWMAP ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   1C >FGDRIVER
   NEXT
END-CODE

CODE DRAWMASK ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   1D >FGDRIVER
   NEXT
END-CODE

CODE DRAWREL ( ix iy --- )
   CX POP
   BX POP
   1E >FGDRIVER
   NEXT
END-CODE

CODE DRAWRELX ( ix iy --- )
   CX POP
   BX POP
   1F >FGDRIVER
   NEXT
END-CODE

CODE DRAWX ( ix iy --- )
   CX POP
   BX POP
   20 >FGDRIVER
   NEXT
END-CODE

CODE DRECT ( minx maxx miny maxy str --- )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   DS PUSH
   ES POP
   21 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE DRWIMAGE ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   22 >FGDRIVER
   NEXT
END-CODE

CODE EGACHECK ( --- flag )
   23 >FGDRIVER
   1PUSH
END-CODE

CODE ELLIPSE ( horiz vert --- )
   CX POP
   BX POP
   24 >FGDRIVER
   NEXT
END-CODE

CODE ELLIPSEF ( horiz vert --- )
   CX POP
   BX POP
   25 >FGDRIVER
   NEXT
END-CODE

CODE FGERASE ( --- )
   26 >FGDRIVER
   NEXT
END-CODE

CODE FADEIN ( delay --- )
   BX POP
   27 >FGDRIVER
   NEXT
END-CODE

CODE FADEOUT ( delay --- )
   BX POP
   28 >FGDRIVER
   NEXT
END-CODE

CODE FILLPAGE ( --- )
   29 >FGDRIVER
   NEXT
END-CODE

CODE FLIPMASK ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   2A >FGDRIVER
   NEXT
END-CODE

CODE FLOOD ( ix iy --- )
   CX POP
   BX POP
   2B >FGDRIVER
   NEXT
END-CODE

CODE FLPIMAGE ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   2C >FGDRIVER
   NEXT
END-CODE

CODE FONTSIZE ( char_height --- )
   BX POP
   2D >FGDRIVER
   1PUSH
END-CODE

CODE FREEPAGE ( pagenumber --- status )
   BX POP
   2E >FGDRIVER
   1PUSH
END-CODE

CODE GETADDR ( --- segment )
   2F >FGDRIVER
   1PUSH
END-CODE

CODE GETATTR ( row column --- c )
   CX POP
   BX POP
   30 >FGDRIVER
   1PUSH
END-CODE

CODE GETBLOCK ( segbuff offbuff minx maxx miny maxy --- )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   ES POP
   SI PUSH
   AX SI MOV
   31 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE GETCHAR ( row column --- c )
   CX POP
   BX POP
   32 >FGDRIVER
   1PUSH
END-CODE

CODE GETCLOCK ( --- dclockticks )
   33 >FGDRIVER
   2PUSH
END-CODE

CODE GETCOLOR ( --- color )
   34 >FGDRIVER
   1PUSH
END-CODE

CODE GETDACS ( start count str --- )
   BX POP
   CX POP
   DX POP
   DS PUSH
   ES POP
   35 >FGDRIVER
   NEXT
END-CODE

CODE GETENTRY ( page_number adrpage_adr adrpage_type --- )
   FGVPARAM #) BX MOV
   DS PUSH
   ES POP
   DX POP
   ES 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   ES 00 [BX] MOV
   DX 02 [BX] MOV
   CX POP
   36 >FGDRIVER
   NEXT
END-CODE

CODE GETHPAGE ( --- hpagenumber )
   37 >FGDRIVER
   1PUSH
END-CODE

CODE GETIMAGE ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   38 >FGDRIVER
   NEXT
END-CODE

CODE GETINDEX ( index --- color )
   BX POP
   39 >FGDRIVER
   1PUSH
END-CODE

CODE GETKEY ( strkey straux --- )
   FGVPARAM #) BX MOV
   DS PUSH
   ES POP
   DI POP
   ES 04 [BX] MOV
   DI 06 [BX] MOV
   DI POP
   ES 00 [BX] MOV
   DI 02 [BX] MOV
   3A >FGDRIVER
   NEXT
END-CODE

CODE GETLINES ( --- nlines )
   3B >FGDRIVER
   1PUSH
END-CODE

CODE GETMAP ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   3C >FGDRIVER
   NEXT
END-CODE

CODE GETMAXX ( --- maxx )
   3D >FGDRIVER
   1PUSH
END-CODE

CODE GETMAXY ( --- maxy )
   3E >FGDRIVER
   1PUSH
END-CODE

CODE GETMODE ( --- videomode )
   DS PUSH
   ES POP
   FGZPARAM #) BX MOV                  \ BX pointe sur la table de paramŠtres
   3F >FGDRIVER
   1PUSH
END-CODE

CODE GETPAGE ( --- pagenumber )
   40 >FGDRIVER
   1PUSH
END-CODE

CODE GETPIXEL ( ix iy --- color )
   CX POP
   BX POP
   41 >FGDRIVER
   1PUSH
END-CODE

CODE GETRGB ( number adrred adrgreen adrblue --- )
   FGVPARAM #) BX MOV
   DX POP
   DS 08 [BX] MOV
   DX 0A [BX] MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   CX POP
   DS PUSH
   ES POP
   42 >FGDRIVER
   NEXT
END-CODE

CODE GETVPAGE ( --- vpagenumber )
   43 >FGDRIVER
   1PUSH
END-CODE

CODE GETXJOY ( n --- xjoypos )
   BX POP
   44 >FGDRIVER
   1PUSH
END-CODE

CODE GETXPOS ( --- xpos )
   45 >FGDRIVER
   1PUSH
END-CODE

CODE GETYJOY ( n --- yjoypos )
   BX POP
   46 >FGDRIVER
   1PUSH
END-CODE

CODE GETYPOS ( --- ypos )
   47 >FGDRIVER
   1PUSH
END-CODE

CODE HUSH ( --- )
   48 >FGDRIVER
   NEXT
END-CODE

CODE HUSHNEXT ( --- )
   49 >FGDRIVER
   NEXT
END-CODE

CODE IMAGEBUF ( segbuff offbuff size --- )
   CX POP
   BX POP
   ES POP
   4A >FGDRIVER
   NEXT
END-CODE

CODE IMAGESIZ ( width height --- dimagesize )
   CX POP
   BX POP
   4B >FGDRIVER
   2PUSH
END-CODE

CODE INITEMS ( --- flag )
   4C >FGDRIVER
   1PUSH
END-CODE

CODE INITJOY ( n --- flag )
   BX POP
   4D >FGDRIVER
   1PUSH
END-CODE

CODE INITXMS ( --- flag )
   4E >FGDRIVER
   1PUSH
END-CODE

CODE INSIDE ( adrvertex_array n ix iy --- flag )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   DS PUSH
   ES POP
   4F >FGDRIVER
   SI POP
   1PUSH
END-CODE

CODE INTJOY ( n strkey straux --- )
   FGVPARAM #) BX MOV
   DI POP
   DS 04 [BX] MOV
   DI 06 [BX] MOV
   DI POP
   DS 00 [BX] MOV
   DI 02 [BX] MOV
   CX POP
   DS PUSH
   ES POP
   50 >FGDRIVER
   NEXT
END-CODE

CODE INTKEY ( strkey straux --- )
   FGVPARAM #) BX MOV
   DI POP
   DS 04 [BX] MOV
   DI 06 [BX] MOV
   DI POP
   DS 00 [BX] MOV
   DI 02 [BX] MOV
   DS PUSH
   ES POP
   51 >FGDRIVER
   NEXT
END-CODE

CODE JUSTIFY ( xjust yjust --- )
   CX POP
   BX POP
   52 >FGDRIVER
   NEXT
END-CODE

CODE KBINIT ( state --- )
   BX POP
   53 >FGDRIVER
   NEXT
END-CODE

CODE KBTEST ( scan_code --- status )
   BX POP
   54 >FGDRIVER
   1PUSH
END-CODE

CODE LOCATE ( row column --- )
   CX POP
   BX POP
   55 >FGDRIVER
   NEXT
END-CODE

CODE MAKEPCX ( minx maxx miny maxy str --- status )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   DS PUSH
   ES POP
   57 >FGDRIVER
   SI POP
   1PUSH
END-CODE

CODE MAKEPPR ( minx maxx miny maxy str --- status )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   DS PUSH
   ES POP
   58 >FGDRIVER
   SI POP
   1PUSH
END-CODE

CODE MAKESPR ( minx maxx miny maxy str --- status )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   DS PUSH
   ES POP
   59 >FGDRIVER
   SI POP
   1PUSH
END-CODE

CODE MAPRGB ( red green blue --- palette )
   DX POP
   CX POP
   BX POP
   5A >FGDRIVER
   1PUSH
END-CODE

CODE MEASURE ( --- delay )
   5B >FGDRIVER
   1PUSH
END-CODE

CODE MEMAVAIL ( --- dmemavail )
   5C >FGDRIVER
   2PUSH
END-CODE

CODE MEMORY ( --- kmemavail )
   5D >FGDRIVER
   1PUSH
END-CODE

CODE MOUSEBUT ( number adrcount adrlastx adrlasty --- )
   FGVPARAM #) BX MOV
   DX POP
   DS 08 [BX] MOV
   DX 0A [BX] MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   CX POP
   DS PUSH
   ES POP
   5E >FGDRIVER
   NEXT
END-CODE

CODE MOUSECUR ( screen_mask cursor_mask --- )
   CX POP
   BX POP
   5F >FGDRIVER
   NEXT
END-CODE

CODE MOUSEFIN ( --- )
   60 >FGDRIVER
   NEXT
END-CODE

CODE MOUSEINI ( --- status )
   61 >FGDRIVER
   1PUSH
END-CODE

CODE MOUSELIM ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   62 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE MOUSEMOV ( ix iy --- )
   CX POP
   BX POP
   63 >FGDRIVER
   NEXT
END-CODE

CODE MOUSEPOS ( adrix adriy adrbuttons --- )
   FGVPARAM #) BX MOV
   DX POP
   DS 08 [BX] MOV
   DX 0A [BX] MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   64 >FGDRIVER
   NEXT
END-CODE

CODE MOUSEPTR ( adrmask xoffset yoffset --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   65 >FGDRIVER
   NEXT
END-CODE

CODE MOUSESPD ( xmickeys ymickeys --- )
   CX POP
   BX POP
   66 >FGDRIVER
   NEXT
END-CODE

CODE MOUSEVIS ( state --- )
   BX POP
   67 >FGDRIVER
   NEXT
END-CODE

CODE FGMOVE ( ix iy --- )
   CX POP
   BX POP
   68 >FGDRIVER
   NEXT
END-CODE

CODE MOVEREL ( ix iy --- )
   CX POP
   BX POP
   69 >FGDRIVER
   NEXT
END-CODE

CODE MUSIC ( strmusic --- )
   BX POP
   DS PUSH
   ES POP
   6A >FGDRIVER
   NEXT
END-CODE

CODE MUSICB ( strmusic ntimes --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   6B >FGDRIVER
   NEXT
END-CODE

CODE NUMLOCK ( --- status )
   6C >FGDRIVER
   1PUSH
END-CODE

CODE PAGESIZE ( --- dpagesize )
   6D >FGDRIVER
   2PUSH
END-CODE

CODE PAINT ( ix iy --- )
   CX POP
   BX POP
   6E >FGDRIVER
   NEXT
END-CODE

CODE PALETTE ( number color --- )
   CX POP
   BX POP
   6F >FGDRIVER
   NEXT
END-CODE

CODE PALETTES ( adrcolor --- )
   BX POP
   DS PUSH
   ES POP
   70 >FGDRIVER
   NEXT
END-CODE

CODE PAN ( ix iy --- )
   CX POP
   BX POP
   71 >FGDRIVER
   NEXT
END-CODE

CODE PATTERN ( index display_pattern --- )
   CX POP
   BX POP
   72 >FGDRIVER
   NEXT
END-CODE

CODE PCXHEAD ( adrpcx_file adrpcx_header --- status )
   FGVPARAM #) BX MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   73 >FGDRIVER
   1PUSH
END-CODE

CODE PCXMODE ( adrpcx_header --- status )
   BX POP
   DS PUSH
   ES POP
   74 >FGDRIVER
   1PUSH
END-CODE

CODE PLAYING ( --- flag )
   75 >FGDRIVER
   1PUSH
END-CODE

CODE POINT ( ix iy --- )
   CX POP
   BX POP
   76 >FGDRIVER
   NEXT
END-CODE

CODE POINTX ( ix iy --- )
   CX POP
   BX POP
   77 >FGDRIVER
   NEXT
END-CODE

CODE POLYFILL ( adrvertex_array adrwork_array n --- )
   FGVPARAM #) BX MOV
   CX POP
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   78 >FGDRIVER
   NEXT
END-CODE

CODE POLYGON ( adrix adriy n --- )
   FGVPARAM #) BX MOV
   CX POP
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   79 >FGDRIVER
   NEXT
END-CODE

CODE POLYLINE ( adrvertex_array n --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   7A >FGDRIVER
   NEXT
END-CODE

CODE POLYOFF ( ix iy --- )
   CX POP
   BX POP
   7B >FGDRIVER
   NEXT
END-CODE

CODE PRINT ( str len --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   7C >FGDRIVER
   NEXT
END-CODE

CODE PUTBLOCK ( segbuff offbuff minx maxx miny maxy --- )
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   ES POP
   SI PUSH
   AX SI MOV
   7D >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE PUTIMAGE ( adrmap_array width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   7E >FGDRIVER
   NEXT
END-CODE

CODE QUIET ( --- )
   7F >FGDRIVER
   NEXT
END-CODE

CODE RECT ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   80 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE RESET ( --- )
   81 >FGDRIVER
   NEXT
END-CODE

CODE RESIZE ( width height --- )
   CX POP
   BX POP
   82 >FGDRIVER
   NEXT
END-CODE

CODE RESTORE ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   83 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE FGRESUME ( --- )
   84 >FGDRIVER
   NEXT
END-CODE

CODE REVIMAGE ( str width height --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   85 >FGDRIVER
   NEXT
END-CODE

CODE REVMASK ( str runs width --- )
   DX POP
   CX POP
   BX POP
   DS PUSH
   ES POP
   86 >FGDRIVER
   NEXT
END-CODE

CODE FGSAVE ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   87 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE SCRLOCK ( --- status )
   88 >FGDRIVER
   1PUSH
END-CODE

CODE SCROLL ( minx maxx miny maxy jump type --- )
   ES POP
   DI POP
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   89 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE SETATTR ( foreground background blink --- )
   DX POP
   CX POP
   BX POP
   8A >FGDRIVER
   NEXT
END-CODE

CODE SETCAPS ( status --- )
   BX POP
   8B >FGDRIVER
   NEXT
END-CODE

CODE SETCLIP ( minx maxx miny maxy --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   8C >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE SETCOLOR ( color --- )
   BX POP
   8D >FGDRIVER
   NEXT
END-CODE

CODE SETDACS ( start count strvalues --- )
   BX POP
   CX POP
   DX POP
   DS PUSH
   ES POP
   8E >FGDRIVER
   NEXT
END-CODE

CODE SETENTRY ( page_number page_addr page_type --- )
   DX POP
   CX POP
   BX POP
   8F >FGDRIVER
   NEXT
END-CODE

CODE SETFUNC ( mode --- )
   BX POP
   90 >FGDRIVER
   NEXT
END-CODE

CODE SETHPAGE ( pagenumber --- )
   BX POP
   91 >FGDRIVER
   NEXT
END-CODE

CODE SETLINES ( lines --- )
   BX POP
   92 >FGDRIVER
   NEXT
END-CODE

CODE SETMODE ( videomode --- )
   BX POP
   FGZPARAM #) DX MOV                  \ DX pointe sur la table de paramŠtres
   DS PUSH
   ES POP
   93 >FGDRIVER
   NEXT
END-CODE

CODE SETNUM ( status --- )
   BX POP
   94 >FGDRIVER
   NEXT
END-CODE

CODE SETPAGE ( pagenumber --- )
   BX POP
   95 >FGDRIVER
   NEXT
END-CODE

CODE SETRGB ( number red green blue --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   96 >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE SETVPAGE ( pagenumber --- )
   BX POP
   98 >FGDRIVER
   NEXT
END-CODE

CODE SHOWPCX ( str flags --- status )
   CX POP
   BX POP
   DS PUSH
   ES POP
   9A >FGDRIVER
   1PUSH
END-CODE

CODE SHOWPPR ( str width --- status )
   CX POP
   BX POP
   DS PUSH
   ES POP
   9B >FGDRIVER
   1PUSH
END-CODE

CODE SHOWSPR ( str width --- status )
   CX POP
   BX POP
   DS PUSH
   ES POP
   9C >FGDRIVER
   1PUSH
END-CODE

CODE SOUND ( frequency duration --- )
   CX POP
   BX POP
   9D >FGDRIVER
   NEXT
END-CODE

CODE SOUNDS ( adrsound ntimes --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   9E >FGDRIVER
   NEXT
END-CODE

CODE STALL ( delay --- )
   BX POP
   9F >FGDRIVER
   NEXT
END-CODE

CODE SUSPEND ( --- )
   0A0 >FGDRIVER
   NEXT
END-CODE

CODE SVGAINIT ( method --- status )
   BX POP
   0A1 >FGDRIVER
   1PUSH
END-CODE

CODE SVGASTAT ( --- status )
   0A2 >FGDRIVER
   1PUSH
END-CODE

CODE SVGAVER ( adrmajor adrminor --- )
   FGVPARAM #) BX MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   0A3 >FGDRIVER
   NEXT
END-CODE

CODE TCDEFINE ( index attribute --- )
   CX POP
   BX POP
   0A4 >FGDRIVER
   NEXT
END-CODE

CODE TCMASK ( mask --- )
   BX POP
   0A5 >FGDRIVER
   NEXT
END-CODE

CODE TCXFER ( minx maxx miny maxy newx newy sourcepage destpage --- )
   FGVPARAM #) BX MOV
   DX POP
   CX POP
   AX POP
   AX 0A [BX] MOV
   AX POP
   AX 08 [BX] MOV
   AX POP
   AX 06 [BX] MOV
   AX POP
   AX 04 [BX] MOV
   AX POP
   AX 02 [BX] MOV
   AX POP
   AX 00 [BX] MOV
   DS PUSH
   ES POP
   0A6 >FGDRIVER
   NEXT
END-CODE

CODE TESTMODE ( mode pages --- flag )
   CX POP
   BX POP
   0A7 >FGDRIVER
   1PUSH
END-CODE

CODE TEXT ( str len --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   0A8 >FGDRIVER
   NEXT
END-CODE

CODE TRANSFER ( minx maxx miny maxy newx newy sourcepage destpage --- )
   FGVPARAM #) BX MOV
   DX POP
   CX POP
   AX POP
   AX 0A [BX] MOV
   AX POP
   AX 08 [BX] MOV
   AX POP
   AX 06 [BX] MOV
   AX POP
   AX 04 [BX] MOV
   AX POP
   AX 02 [BX] MOV
   AX POP
   AX 00 [BX] MOV
   DS PUSH
   ES POP
   0A9 >FGDRIVER
   NEXT
END-CODE

CODE VERSION ( adrmajor adrminor --- )
   BX POP
   03 # AX MOV
   AX 00 [BX] MOV
   BX POP
   03 # AX MOV
   AX 00 [BX] MOV
   NEXT
END-CODE

CODE VOICE ( channel frequency volume duration --- )
   AX POP
   DX POP
   CX POP
   BX POP
   SI PUSH
   AX SI MOV
   0AA >FGDRIVER
   SI POP
   NEXT
END-CODE

CODE VOICES ( adrsound ntimes --- )
   CX POP
   BX POP
   DS PUSH
   ES POP
   0AB >FGDRIVER
   NEXT
END-CODE

CODE WAITFOR ( ticks --- )
   BX POP
   0AC >FGDRIVER
   NEXT
END-CODE

CODE WAITKEY ( --- )
   0AD >FGDRIVER
   NEXT
END-CODE

CODE WAITVR ( state --- )
   BX POP
   0AE >FGDRIVER
   NEXT
END-CODE

CODE FGWHERE ( adrrow adrcolumn --- )
   FGVPARAM #) BX MOV
   DX POP
   DS 04 [BX] MOV
   DX 06 [BX] MOV
   DX POP
   DS 00 [BX] MOV
   DX 02 [BX] MOV
   DS PUSH
   ES POP
   0AF >FGDRIVER
   NEXT
END-CODE

CODE XALPHA ( ix --- column )
   BX POP
   0B0 >FGDRIVER
   1PUSH
END-CODE

CODE XCONVERT ( column --- ix )
   BX POP
   0B1 >FGDRIVER
   1PUSH
END-CODE

CODE YALPHA ( iy --- row )
   BX POP
   0B2 >FGDRIVER
   1PUSH
END-CODE

CODE YCONVERT ( row --- iy )
   BX POP
   0B3 >FGDRIVER
   1PUSH
END-CODE

DECIMAL
ONLY FORTH ALSO FASTGRAF ALSO
FORTH DEFINITIONS
EOF


