              title     "PGFORTH"
              subttl    "P. Guillaumaud"

              INCLUDE   PGFORTH.INC
code          segment
              assume    cs:code, ds:code
DEFNIL@
VOC@          FORTH,101b,5,"FORT","H"
              INCLUDE   VARIABLE.ASM
              INCLUDE   NOYAU.ASM
              INCLUDE   CONVERT.ASM
              INCLUDE   WORD.ASM
              INCLUDE   INTER.ASM
              INCLUDE   COMPILER.ASM
              INCLUDE   FILES.ASM
FINVOC@
FIN@
code          ends
              end       ENTRY

