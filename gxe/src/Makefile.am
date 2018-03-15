#
# GXE
#

INCLUDES= @DEBUG_FLAGS@ @GXE_THREAD_CFLAGS@ @GLIB_CFLAGS@ @XML_CFLAGS@

bin_PROGRAMS = gxe

gxe_SOURCES=	gxe.c				\
				common.h	\
				s_globals.h	\
		s_api.c		s_api.h		\
		s_cmd.c		s_cmd.h		\
		s_incomming.c	s_incomming.h	\
		s_misc.c	s_misc.h	\
		s_modules.c	s_modules.h	\
		s_out.c		s_out.h		\
		s_process.c	s_process.h	\
		s_runcmd.c	s_runcmd.h	\
		s_session.c	s_session.h	\
		s_signaux.c	s_signaux.h	\
		s_sockets.c	s_sockets.h	\
		s_syslog.c	s_syslog.h	\
		s_xmlattr.c	s_xmlattr.h	\
		s_xmlparse.c	s_xmlparse.h

gxe_LDADD= @GLIB_LIBS@ @XML_LIBS@
