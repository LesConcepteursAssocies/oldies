#
# MAKE de NEOSERV, pour le compilateur Symantec C++ 6.10
#
# Vous ˆtes autoris‚ … diffuser et … utiliser ce logiciel sans restrictions.
# Ce logiciel est fourni tel quel, sans aucune garantie.
# L'auteur ne saurait voir sa responsabilit‚ engag‚e en cas de
# dommages de quelque nature que ce soit subis par l'utilisateur
# ou des tiers et r‚sultant directement ou
# indirectement de son utilisation, notamment la perte de donn‚es,
# ou toute perte financiŠre r‚sultant de son
# utilisation ou de l'impossibilit‚ de l'utiliser, et ceci mˆme si
# l'auteur a ‚t‚ pr‚venu de la possibilit‚ de tels dommages.
#
# Options de compilation
# ----------------------
# (Mode se d‚finit sur la ligne de commande:
#  make -s MODE=-DDEMO par exemple)
# par d‚faut, pas de restrictions
#
MODE=
CFLAGS = -mx -4 -o -Jm -c $(MODE)

#
# RŠgle de compilation
#.c.obj:
#       sc $(CFLAGS) $*

NEOSERV.EXE : GESFIC.OBJ MEM.OBJ FILESPEC.OBJ CLAVIER.obj COMPILER.obj CORINTE.obj CVDATA.obj \
              EXCEPTIO.obj HORLOGE.obj KERNEL.obj LIBCOM.obj LOCALSER.obj MEMOIRE.obj SCRNSAVE.obj \
              STATS.obj SWASYNC.obj SWVIDEO.obj INITIAL.obj TACHEDEF.obj AFFICHAG.obj SYMCCI.obj \
              MITEC.obj MODEM.obj GESTION.obj DIVERS.OBJ CRCTAB.OBJ CONVBUFF.OBJ PDATA.obj \
              REGEXP.obj NEOSERV.obj
              sc -mx NEOSERV.obj CLAVIER.obj COMPILER.obj CORINTE.obj CVDATA.obj EXCEPTIO.obj \
                 HORLOGE.obj KERNEL.obj LIBCOM.obj LOCALSER.obj MEMOIRE.obj SCRNSAVE.obj \
                 STATS.obj SWASYNC.obj SWVIDEO.obj INITIAL.obj TACHEDEF.obj AFFICHAG.obj \
                 MODEM.obj SYMCCI.OBJ MITEC.OBJ MEM.OBJ FILESPEC.OBJ GESTION.OBJ GESFIC.OBJ \
                 DIVERS.OBJ CRCTAB.OBJ CONVBUFF.OBJ PDATA.OBJ REGEXP.OBJ C4BASE.LIB

REGEXP.obj   : generic.h compiler.h cvdata.h REGEXP.C
       sc $(CFLAGS) regexp
PDATA.obj    : generic.h pdata.h PDATA.C
       sc $(CFLAGS) pdata
CONVBUFF.obj : generic.h CONVBUFF.C
       sc $(CFLAGS) convbuff
CRCTAB.obj   : CRCTAB.C
       sc $(CFLAGS) crctab
DIVERS.obj   : generic.h DIVERS.C
       sc $(CFLAGS) divers
GESFIC.obj   : generic.h GESFIC.C
       sc $(CFLAGS) gesfic
MEM.OBJ      : MEM.C
       sc $(CFLAGS) mem
FILESPEC.OBJ : FILESPEC.C
       sc $(CFLAGS) filespec
CLAVIER.obj : generic.h stats.h compiler.h cvdata.h keyboard.h minitel.h ports.h CLAVIER.C
       sc $(CFLAGS) clavier
COMPILER.obj : minitel.h generic.h stats.h compiler.h cvdata.h declkrnl.h COMPILER.C
       sc $(CFLAGS) compiler
MODEM.obj    : generic.h stats.h compiler.h cvdata.h minitel.h ports.h MODEM.C
       sc $(CFLAGS) modem
SYMCCI.obj   : generic.h symcci.h SYMCCI.C
       sc $(CFLAGS) symcci
MITEC.obj    : generic.h stats.h compiler.h cvdata.h minitel.h ports.h MITEC.C
       sc $(CFLAGS) mitec
CORINTE.obj : generic.h stats.h compiler.h cvdata.h minitel.h ports.h CORINTE.C
       sc $(CFLAGS) corinte
CVDATA.obj : generic.h stats.h compiler.h cvdata.h CVDATA.C
       sc $(CFLAGS) cvdata
EXCEPTIO.obj : generic.h EXCEPTIO.C
       sc $(CFLAGS) exceptio
HORLOGE.obj : generic.h HORLOGE.C
       sc $(CFLAGS) horloge
KERNEL.obj : minitel.h generic.h stats.h compiler.h cvdata.h KERNEL.C
       sc $(CFLAGS) kernel
LIBCOM.obj : minitel.h generic.h ports.h stats.h compiler.h cvdata.h LIBCOM.C
       sc $(CFLAGS) libcom
LOCALSER.obj : generic.h stats.h compiler.h cvdata.h minitel.h ports.h LOCALSER.C
       sc $(CFLAGS) localser
MEMOIRE.obj : generic.h MEMOIRE.C
       sc $(CFLAGS) memoire
SCRNSAVE.obj : SCRNSAVE.C
       sc $(CFLAGS) scrnsave
STATS.obj : generic.h stats.h compiler.h cvdata.h STATS.C
       sc $(CFLAGS) stats
SWASYNC.obj : generic.h ports.h SWASYNC.C
       sc $(CFLAGS) swasync
SWVIDEO.obj : generic.h SWVIDEO.C
       sc $(CFLAGS) swvideo
INITIAL.obj : generic.h ports.h stats.h INITIAL.C
       sc $(CFLAGS) initial
TACHEDEF.obj : minitel.h generic.h stats.h compiler.h cvdata.h TACHEDEF.C
       sc $(CFLAGS) tachedef
AFFICHAG.obj : generic.h AFFICHAG.C
       sc $(CFLAGS) affichag
GESTION.obj : generic.h stats.h compiler.h cvdata.h minitel.h ports.h GESTION.C
       sc $(CFLAGS) gestion
NEOSERV.obj : generic.h stats.h compiler.h cvdata.h minitel.h ports.h pdata.h NEOSERV.C
       sc $(CFLAGS) neoserv

