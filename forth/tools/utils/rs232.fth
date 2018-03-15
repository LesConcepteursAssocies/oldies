\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Gestion des ports s‚rie par interruption.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

EXIST? INT@ NOT ?\ INCLUDE MSDOS

HEX

\ AccŠs au contr“leur de communications 8250.
\ Remplacer 03Fx par 02Fx pour COM2.
03F8      CONSTANT #COM1          \ Adresse de base du port COM1.

#COM1     CONSTANT COM1.TX        \ Registre de transmission du 8250.
#COM1     CONSTANT COM1.RX        \ Registre de r‚ception du 8250.
#COM1     CONSTANT COM1.LATCHL    \ Octet faible du diviseur du 8250.
#COM1 1+  CONSTANT COM1.LATCHH    \ Octet fort du diviseur du 8250.
#COM1 1+  CONSTANT COM1.RVI       \ Registre de validation des interruptions du 8250.
#COM1 2+  CONSTANT COM1.RII       \ Registre d'identification des interruptions du 8250.
#COM1 3 + CONSTANT COM1.RCL       \ Registre de contr“le de la ligne du 8250.
#COM1 4 + CONSTANT COM1.RCM       \ Registre de contr“le de modem du 8250.
#COM1 5 + CONSTANT COM1.REL       \ Registre d'‚tat de la ligne du 8250.
#COM1 6 + CONSTANT COM1.REM       \ Registre d'‚tat du modem du 8250.

2VARIABLE OLDIRQ4                 \ ancien vecteur de COM1 (0Ch)
0C INT@ OLDIRQ4 2!                \ Remplacer 0C par 0B pour COM2 (IRQ3).

VARIABLE #BAUD                    \ Vitesse de la ligne (12 = 115200/9600)
0C #BAUD !                        \ Correspond … 9600 Bauds.

VARIABLE CONFRS232                \ Octet de configuration de la ligne.
07 CONFRS232 !                    \ 8 Bits, sans parit‚, 2 bits de stop.

DECIMAL
4000 CONSTANT MAXBUFF             \ Longueur du buffer de r‚ception.
CREATE RSBUFF MAXBUFF ALLOT       \ Buffer circulaire de r‚ception.
VARIABLE 'LC>BUFF                 \ Pointeur utilis‚ par >BUFF.
RSBUFF 'LC>BUFF !
VARIABLE 'RS>BUFF                 \ Pointeur utilis‚ par RSINPUT.
RSBUFF 'RS>BUFF !
HEX

\ Initialise et valide l'interruption IRQ4.
CODE STIIRQ4  ( --- )
  21 # DX MOV
  0 AL IN                         \ Lecture du masque d'interruption du 8259.
  EF # AL AND                     \ Valide IRQ4.
  0 AL OUT                        \ Remplacer EF par F7 pour IRQ3.
  COM1.RVI # DX MOV
  1 # AL MOV                      \ Valide l'interruption "donn‚e prˆte" du 8250.
  0 AL OUT
  NEXT
END-CODE

\ D‚sactive l'interruption IRQ4.
CODE CLIIRQ4  ( --- )
  COM1.RVI # DX MOV
  AL AL XOR                       \ D‚sactive toutes les interruptions du 8250.
  0 AL OUT
  21 # DX MOV
  0 AL IN                         \ AccŠs au masque d'interruptions du 8259.
  10 # AL OR                      \ D‚sactivation de IRQ4.
  0 AL OUT                        \ Remplacer 10 par 8 pour IRQ3.
  NEXT
END-CODE

\ Initialisation de la ligne RS232.
\ oc = octet de configuration de la ligne.
DECIMAL
CODE INITRS232  ( oc --- )
  128 # AL MOV
  COM1.RCL # DX MOV
  0 AL OUT                        \ AccŠs au diviseur du 8250.
  #BAUD #) AX MOV
  COM1.LATCHL # DX MOV
  0 AL OUT
  COM1.LATCHH # DX MOV
  AL AH XCHG
  0 AL OUT                        \ Vitesse initialis‚e.
  AX POP
  COM1.RCL # DX MOV
  0 AL OUT                        \ Configuration de la ligne effectu‚e.
  COM1.RCM # DX MOV
  15 # AL MOV
  0 AL OUT                        \ Valide RTS, DSR, OUT1, OUT2
  AL AL XOR
  COM1.RVI # DX MOV
  0 AL OUT                        \ Masque les interruptions du 8250.
  NEXT
END-CODE

\ Envoi d'un caractŠre sur la ligne.
CODE RSEMIT  ( c --- )
  COM1.REM # DX MOV
  BEGIN
    0 AL IN
    16 # AL TEST                  \ Est-on prˆt … emmettre ?
    0=
  UNTIL
  AX POP                          \ Oui.
  COM1.TX # DX MOV
  0 AL OUT                        \ Envoi du caractŠre.
  NEXT
END-CODE

\ Nouvelle Interruption 0Ch (IRQ4).
HEX
\ Sortie de l'interruption.
LABEL RSINPUT>
  20 # DX MOV
  20 # AL MOV
  0 AL OUT                        \ Envoi de l'accus‚ de r‚ception d'interruption au 8259.
  ES POP
  DS POP
  DI POP
  SI POP
  DX POP
  CX POP
  BX POP
  AX POP                          \ Restitution du contexte.
  IRET

LABEL RSINPUT
  STI
  AX PUSH
  BX PUSH
  CX PUSH
  DX PUSH
  SI PUSH
  DI PUSH
  DS PUSH
  ES PUSH                         \ Sauvegarde du contexte.
  DSEGMENT # AX MOV
  AX DS MOV                       \ AccŠs aux donn‚es.
  BEGIN
    COM1.RII # DX MOV             \ On regarde qui nous a interrompu.
    0 AL IN
    1 # AL CMP                    \ Y-a t'il une interruption en attente ?
    0<>
    IF
      4 # AL CMP                  \ Un caractŠre est'il re‡u ?
      0=
      IF
        COM1.RX # DX MOV
        0 AL IN                   \ Lecture du caractŠre.
        'RS>BUFF #) BX MOV
        AL 0 [BX] MOV             \ Stockage de celui-ci dans le buffer.
        BX INC
        RSBUFF MAXBUFF +
        # BX CMP                  \ Est-on … la fin du buffer ?
        0=
        IF
          RSBUFF # BX MOV         \ Oui, retour au d‚but.
        THEN
        BX 'RS>BUFF #) MOV        \ Mise … jour du pointeur.
      THEN
    ELSE
      RSINPUT> #) JMP             \ Pas d'interruption, on sort.
    THEN
  AGAIN

\ Les mots de haut niveau.
\ Met en place et initialise la nouvelle interruption S‚rie.
: COM1-ON  ( --- )
  0C INT@ OLDIRQ4 2!
  DSEGMENT RSINPUT 0C INT!
  CONFRS232 @ INITRS232           \ Initialisation de la ligne.
  STIIRQ4                         \ Valide la nouvelle interruption RS232.
;

\ D‚sactive l'interruption S‚rie.
: COM1-OFF  ( --- )
  CLIIRQ4                         \ D‚sactivation de l'interruption.
  OLDIRQ4 2@ 0C INT!
;

\ Empile VRAI si un caractŠre est re‡u dans le buffer.
: RSKEY?  ( --- fl )
  'LC>BUFF @
  'RS>BUFF @
  <>
;

\ Lecture du buffer de r‚ception.
\ Empile le caractŠre re‡u.
: RSKEY  ( --- c )
  'LC>BUFF @ C@                 \ Lecture du caractŠre.
  'LC>BUFF 1+!
  'LC>BUFF @
  RSBUFF MAXBUFF +
  =
  IF                            \ Est-on … la fin du buffer ?
    RSBUFF 'LC>BUFF !           \ Oui, retour au d‚but.
  THEN
;

DECIMAL

EOF

