/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 23/02/2000
**
** GBmain.cc
**
** Programme principal
** ----------------------------------------------------------------------------
*/
#define GBMAINPROC
#include "GBcommon.h"

#include "GBinterface.h"
#include "GBmisc.h"

/*
** ----------------------------------------------------------------------------
** PROGRAMME PRINCIPAL
*/
int main(int argc, char **argv)
{
#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);
#endif

  gtk_set_locale ();
  /* initialize gtk et Imlib */
  gtk_init( &argc, &argv );
  gdk_imlib_init();

  /* Get gdk to use imlib's visual and colormap */
  gtk_widget_push_visual(gdk_imlib_get_visual());
  gtk_widget_push_colormap(gdk_imlib_get_colormap());

  GBAddListDir(&PixmapsDirs, PACKAGE_DATA_DIR "/pixmaps");
  GBAddListDir(&PixmapsDirs, PACKAGE_SOURCE_DIR "/pixmaps");

  /* Test présence d'openGL */
  if(gdk_gl_query() == FALSE)
  {
    GBPrintf(_("openGL n'est pas supporté par votre système\n"));
    return 0;
  }

  glarea     = NULL;
  mainwindow = NULL;
  GBInit();

  mainwindow = CreateMainWindow();
  gtk_widget_show (mainwindow);

  gtk_main();
  return 0;
}
/*
** ----------------------------------------------------------------------------
*/
