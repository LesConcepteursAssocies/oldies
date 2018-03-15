\ -----------------------------------------------------------------------------
\                    Gestion des codes de touche Çtendus.
\
\ Par Guillaumaud Philippe.
\ -----------------------------------------------------------------------------

ECHO OFF
WARNING OFF
DECIMAL

\ Constante ajoutÇe aux codes de touches Çtendues
128 CONSTANT #EKEY

\ Primitive de lecture.
\ Principe : Elimine le 0 prÇcÇdent les codes Çtendus, et ajoute #EKEY
\            au code de la touche lue.
: (EKEY)  ( --- c )
  (KEY) DUP 0=
  IF
    DROP (KEY) #EKEY +
  THEN
;

\ Activation de la gestion des codes Çtendus.
: EKEY-ON  ( --- )
  ['] (EKEY) IS KEY
;

\ DÇsactivation de la gestion des codes Çtendus.
: EKEY-OFF  ( --- )
  ['] (KEY) IS KEY
;

\ DÇfinition des constantes de touches.
072 #EKEY + CONSTANT #FLH         \ Fläche haute.
080 #EKEY + CONSTANT #FLB         \ Fläche basse.
075 #EKEY + CONSTANT #FLG         \ Fläche gauche.
077 #EKEY + CONSTANT #FLD         \ Fläche droite.
115 #EKEY + CONSTANT #CFLG        \ Ctrl+Fläche gauche.
116 #EKEY + CONSTANT #CFLD        \ Ctrl+Fläche droite.

082 #EKEY + CONSTANT #INS         \ Touche Inser.
083 #EKEY + CONSTANT #SUP         \ Touche Suppr.
071 #EKEY + CONSTANT #HOM         \ Touche Home.
079 #EKEY + CONSTANT #END         \ Touche Fin.
073 #EKEY + CONSTANT #PGU         \ Touche Page Up.
081 #EKEY + CONSTANT #PGD         \ Touche Page Down.
117 #EKEY + CONSTANT #CEND        \ Ctrl+Touche Fin.
118 #EKEY + CONSTANT #CPGD        \ Ctrl+Touche Page Down.
119 #EKEY + CONSTANT #CHOM        \ Ctrl+Touche Home.
132 #EKEY + CONSTANT #CPGU        \ Ctrl+Touche Page Up.

027 CONSTANT #ESC                 \ Touche Escape.
013 CONSTANT #RET                 \ Touche Return.
009 CONSTANT #TAB                 \ Touche Tab.
015 #EKEY + CONSTANT #STAB        \ Shift+Touche Tab.
032 CONSTANT #SPC                 \ Barre d'espace.

059 #EKEY + CONSTANT #TF1         \ Touche F1.
060 #EKEY + CONSTANT #TF2         \ Touche F2.
061 #EKEY + CONSTANT #TF3         \ Touche F3.
062 #EKEY + CONSTANT #TF4         \ Touche F4.
063 #EKEY + CONSTANT #TF5         \ Touche F5.
064 #EKEY + CONSTANT #TF6         \ Touche F6.
065 #EKEY + CONSTANT #TF7         \ Touche F7.
066 #EKEY + CONSTANT #TF8         \ Touche F8.
067 #EKEY + CONSTANT #TF9         \ Touche F9.
068 #EKEY + CONSTANT #TF10        \ Touche F10.

084 #EKEY + CONSTANT #STF1        \ Shift+Touche F1.
085 #EKEY + CONSTANT #STF2        \ Shift+Touche F2.
086 #EKEY + CONSTANT #STF3        \ Shift+Touche F3.
087 #EKEY + CONSTANT #STF4        \ Shift+Touche F4.
088 #EKEY + CONSTANT #STF5        \ Shift+Touche F5.
089 #EKEY + CONSTANT #STF6        \ Shift+Touche F6.
090 #EKEY + CONSTANT #STF7        \ Shift+Touche F7.
091 #EKEY + CONSTANT #STF8        \ Shift+Touche F8.
092 #EKEY + CONSTANT #STF9        \ Shift+Touche F9.
093 #EKEY + CONSTANT #STF10       \ Shift+Touche F10.

094 #EKEY + CONSTANT #CTF1        \ Ctrl+Touche F1.
095 #EKEY + CONSTANT #CTF2        \ Ctrl+Touche F2.
096 #EKEY + CONSTANT #CTF3        \ Ctrl+Touche F3.
097 #EKEY + CONSTANT #CTF4        \ Ctrl+Touche F4.
098 #EKEY + CONSTANT #CTF5        \ Ctrl+Touche F5.
099 #EKEY + CONSTANT #CTF6        \ Ctrl+Touche F6.
100 #EKEY + CONSTANT #CTF7        \ Ctrl+Touche F7.
101 #EKEY + CONSTANT #CTF8        \ Ctrl+Touche F8.
102 #EKEY + CONSTANT #CTF9        \ Ctrl+Touche F9.
103 #EKEY + CONSTANT #CTF10       \ Ctrl+Touche F10.

104 #EKEY + CONSTANT #ATF1        \ Alt+Touche F1.
105 #EKEY + CONSTANT #ATF2        \ Alt+Touche F2.
106 #EKEY + CONSTANT #ATF3        \ Alt+Touche F3.
107 #EKEY + CONSTANT #ATF4        \ Alt+Touche F4.
108 #EKEY + CONSTANT #ATF5        \ Alt+Touche F5.
109 #EKEY + CONSTANT #ATF6        \ Alt+Touche F6.
110 #EKEY + CONSTANT #ATF7        \ Alt+Touche F7.
111 #EKEY + CONSTANT #ATF8        \ Alt+Touche F8.
112 #EKEY + CONSTANT #ATF9        \ Alt+Touche F9.
113 #EKEY + CONSTANT #ATF10       \ Alt+Touche F10.

EKEY-ON
EOF

