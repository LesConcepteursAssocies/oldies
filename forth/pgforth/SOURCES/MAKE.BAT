echo off
echo ----------------
echo  Compilation de %1
echo ----------------
if not "%2"=="" goto link
tasm %1
if errorlevel 1 goto errasm
:link
tlink %1,PGKRNL
if errorlevel 1 goto errlnk
echo -------------
echo  Fin Normale
echo -------------
goto fin
:errasm
echo --------------------------
echo  Erreur(s) de Compilation
echo --------------------------
goto fin
:errlnk
echo ----------------------
echo  Erreur(s) de Lien(s)
echo ----------------------
:fin
