## Utilisé pour générer Makefile.in

INCLUDES= -I$(top_srcdir)/intl @GTK_CFLAGS@ @GTKGL_CFLAGS@ @GDK_IMLIB_CFLAGS@ @XML_CFLAGS@

bin_PROGRAMS = gbuild

gbuild_SOURCES= GBmain.cc			\
				GBcommon.h	\
				GB3Dtypes.h	\
		GBinterface.cc	GBinterface.h	\
		GBmainmenu.cc	GBmainmenu.h	\
		GBtrackball.cc	GBtrackball.h	\
		GBhelp.cc	GBhelp.h	\
		GBglarea.cc	GBglarea.h	\
		GBtoolbar.cc	GBtoolbar.h	\
		GBstatusbar.cc	GBstatusbar.h	\
		GBmath.cc	GBmath.h	\
		GBglmisc.cc	GBglmisc.h	\
		GBobjets.cc	GBobjets.h	\
		GBworld.cc	GBworld.h	\
		GBglfont.cc	GBglfont.h	\
		GBcamera.cc	GBcamera.h	\
		GBwinmvt.cc	GBwinmvt.h	\
		GBwinparam.cc	GBwinparam.h	\
		GBmisc.cc	GBmisc.h

gbuild_LDADD= @GTK_LIBS@ @GTKGL_LIBS@ @GDK_IMLIB_LIBS@ @XML_LIBS@ $(INTLLIBS)
