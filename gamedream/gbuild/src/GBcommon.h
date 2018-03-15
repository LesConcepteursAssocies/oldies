/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 10/03/2000
**
** GBcommon.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBCOMMON_H
#define _GBCOMMON_H

/*
** includes généraux
** -----------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/param.h>
/*
** Réseau
** ------
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
/*
** openGL/Mesa/glut/gtk/gtkgl/imlib
** --------------------------------
*/
#include <gdk/gdk.h>
#include <gdk_imlib.h>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/glx.h>
#include <gtkgl/gtkglarea.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

/*
 * Standard gettext macros.
*/
#ifdef ENABLE_NLS
#  include <libintl.h>
#  undef _
#  define _(String) dgettext (PACKAGE, String)
#  ifdef gettext_noop
#    define N_(String) gettext_noop (String)
#  else
#    define N_(String) (String)
#  endif
#else
#  define textdomain(String) (String)
#  define gettext(String) (String)
#  define dgettext(Domain,Message) (Message)
#  define dcgettext(Domain,Message,Type) (Message)
#  define bindtextdomain(Domain,Directory) (Domain)
#  define _(String) (String)
#  define N_(String) (String)
#endif

/*
** Variables et structures globales
** --------------------------------
*/
#ifdef GBMAINPROC
#define GBGLOBAL
#else
#define GBGLOBAL extern
#endif

#define        GD_LEN_STRING       80
// Taille par défaut de la fenêtre openGL
#define        GLWIDTH             640
#define        GLHEIGHT            480
// Paramètres du cone de vision par défaut
#define        GD_DEFAULT_FOV      90.0
#define        GD_DEFAULT_ZNEAR    1.0
#define        GD_DEFAULT_ZFAR     400.0
// Paramètres de la caméra par defaut
#define        GD_DEFAULT_CAM_TYPE 0
#define        GD_DEFAULT_CAM_PX   0.0
#define        GD_DEFAULT_CAM_PY   0.0
#define        GD_DEFAULT_CAM_PZ   10.0
#define        GD_DEFAULT_CAM_RX   0.0
#define        GD_DEFAULT_CAM_RY   0.0
#define        GD_DEFAULT_CAM_RZ   0.0
#define        GD_DEFAULT_CAM_AX   0.0
#define        GD_DEFAULT_CAM_AY   0.0
#define        GD_DEFAULT_CAM_AZ   0.0

// La fenêtre principale
GBGLOBAL GtkWidget *mainwindow;
// La fenêtre openGL
GBGLOBAL GtkWidget  *glarea;
GBGLOBAL gint       SelectMode;

// Les répertoires ou peuvent êtres placés les icones
GBGLOBAL GList      *PixmapsDirs;

GBGLOBAL const gchar   *gd_auteur;

GBGLOBAL const GLubyte *gl_vendor;
GBGLOBAL const GLubyte *gl_renderer;
GBGLOBAL const GLubyte *gl_version;
GBGLOBAL const GLubyte *gl_extensions;

GBGLOBAL const gchar *gd_titre;
GBGLOBAL const gchar *gd_apropos;

// Paramètres généraux du projet
GBGLOBAL gboolean   bDrawAxeX;
GBGLOBAL gfloat     AxeXcol[3];
GBGLOBAL gboolean   bDrawAxeY;
GBGLOBAL gfloat     AxeYcol[3];
GBGLOBAL gboolean   bDrawAxeZ;
GBGLOBAL gfloat     AxeZcol[3];
GBGLOBAL gboolean   bDrawGridX;
GBGLOBAL gfloat     GridXcol[3];
GBGLOBAL gint       GridSzX;
GBGLOBAL gint       GridSpX;
GBGLOBAL gboolean   bDrawGridY;
GBGLOBAL gfloat     GridYcol[3];
GBGLOBAL gint       GridSzY;
GBGLOBAL gint       GridSpY;
GBGLOBAL gboolean   bDrawGridZ;
GBGLOBAL gfloat     GridZcol[3];
GBGLOBAL gint       GridSzZ;
GBGLOBAL gint       GridSpZ;

GBGLOBAL gint       pickname;
#endif
/*
** ----------------------------------------------------------------------------
*/
