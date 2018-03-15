\ -----------------------------------------------------------------------------
\                                Utilitaire DOS.
\
\ Outils de gestion de la m‚moire.
\
\ Par Guillaumaud Philippe
\ -----------------------------------------------------------------------------

ECHO OFF

HEX

\ Conversion d'un nombre d'octets en nombre de paragraphes.
CODE NO->NP  ( no --- np )
  AX POP
  AX SHR
  AX SHR
  AX SHR
  AX SHR
  1PUSH
END-CODE

\ Primitive d'allocation m‚moire.
\ np : taille du bloc en paragraphes.
\ si fl est FAUX :
\      n : adresse de segment du bloc allou‚.
\ si fl est VRAI :
\      n : code d'erreur.
CODE (MALLOC)  ( np --- n fl )
  BX POP
  48 # AH MOV
  21 INT
  AX PUSH
  U<
  IF
    TRUE # AX MOV
  ELSE
    FALSE # AX MOV
  THEN
  1PUSH
END-CODE

\ Allocation d'un bloc de m‚moire.
\ no : taille du bloc, en octets.
: MALLOC  ( no --- seg )
  NO->NP 1+                  \ Conversion en paragraphes.
  (MALLOC) ?DOS-ERR
;

\ Primitive de lib‚ration d'un bloc pr‚c‚dement allou‚.
\ adr : adresse de segment du bloc.
\ si fl est VRAI, err est le code d'erreur.
CODE (RELEASE)  ( adr --- err fl )
  ES POP
  49 # AH MOV
  21 INT
  AX PUSH
  U<
  IF
    TRUE # AX MOV
  ELSE
    FALSE # AX MOV
  THEN
  1PUSH
END-CODE

\ Lib‚ration d'un bloc de m‚moire.
: RELEASE  ( adr --- )
  (RELEASE) ?DOS-ERR DROP
;

\ Primitive de redimensionnement d'un bloc m‚moire.
\  np : nouvelle taille du bloc, en paragraphes.
\ adr : adresse de segment du bloc.
\ si fl est VRAI, err est un code d'erreur.
CODE (RESIZE)  ( adr np --- err fl )
  BX POP
  ES POP
  4A # AH MOV
  21 INT
  AX PUSH
  U<
  IF
    TRUE # AX MOV
  ELSE
    FALSE # AX MOV
  THEN
  1PUSH
END-CODE

\ Redimensionnement d'un bloc de m‚moire.
\  no : nouvelle taille du bloc, en octets.
\ adr : adresse de segment du bloc allou‚.
: RESIZE  ( adr no --- )
  NO->NP 1+                  \ Conversion en paragraphes.
  (RESIZE) ?DOS-ERR DROP
;

\ Lib‚ration de la m‚moire allou‚e … Turbo-FORTH.
CODE LIB-COM  ( --- )
  CS AX MOV
  AX ES MOV
  1000 # BX MOV
  4A # AH MOV
  21 INT
  NEXT
END-CODE

DECIMAL

LIB-COM

EOF

