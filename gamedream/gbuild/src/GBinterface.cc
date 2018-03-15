/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 25/02/2000
**
** GBinterface.c
**
** Création de l'interface principale
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBinterface.h"
#include "GBmainmenu.h"
#include "GBtoolbar.h"
#include "GBglarea.h"
#include "GBstatusbar.h"

#include "GBmisc.h"


/*
** ----------------------------------------------------------------------------
** Création de l'interface principale du programme
*/
GtkWidget *CreateMainWindow(void)
{
  GtkWidget *mainwindow;
  GtkWidget *vbox1;
  GtkWidget *mainmenu;
  GtkWidget *toolbar1;
  GtkWidget *toolbar2;
  GtkWidget *statusbar1;
  GtkWidget *scrolledwindow1;
  GtkWidget *text1;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  /* Bulles d'aide */
  tooltips = gtk_tooltips_new ();
  /* Accelerateurs */
  accel_group = gtk_accel_group_new ();

  /* La fenêtre englobante */
  mainwindow = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (mainwindow, "mainwindow");
  gtk_object_set_data (GTK_OBJECT (mainwindow), "mainwindow", mainwindow);
  gtk_window_set_title (GTK_WINDOW (mainwindow),gd_titre);
  /* Quit form main if got delete event */
  gtk_signal_connect(GTK_OBJECT(mainwindow),"delete_event",GTK_SIGNAL_FUNC(GBExit), NULL);
  /* You should always delete gtk_gl_area widgets before exit or else
     GLX contexts are left undeleted, this may cause problems (=core dump)
     in some systems.
     Destroy method of objects is not automatically called on exit.
     You need to manually enable this feature. Do gtk_quit_add_destroy()
     for all your top level windows unless you are certain that they get
     destroy signal by other means.
  */
  gtk_quit_add_destroy(1, GTK_OBJECT(mainwindow));

  /* nécessaire pour la toolbar qui contient des icones */
  gtk_widget_realize(mainwindow);

  /* La boite qui va contenir les divers widgets de l'interface */
  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox1, "vbox1");
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "vbox1", vbox1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (mainwindow), vbox1);

  /* Le menu principal */
  mainmenu = CreateMainMenu(&mainwindow,&tooltips,&accel_group);
  gtk_box_pack_start (GTK_BOX (vbox1), mainmenu, FALSE, FALSE, 0);

  /* La barre d'icones */
  toolbar1 = CreateMainToolbar(&mainwindow);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar1, FALSE, FALSE, 0);

  /* La barre d'icones des primitives 3D */
  toolbar2 = CreatePrim3DToolbar(&mainwindow);
  gtk_box_pack_start (GTK_BOX (vbox1), toolbar2, FALSE, FALSE, 0);

  /* La fenêtre openGL */
  glarea = CreateGLarea(&mainwindow);
  gtk_box_pack_start (GTK_BOX (vbox1), glarea, TRUE, TRUE, 0);

  /* La bar de status */
  statusbar1 = CreateMainStatusbar(&mainwindow);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar1, FALSE, FALSE, 0);

  /* La zone de texte scrollable pour informations */
  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "scrolledwindow1", scrolledwindow1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
  gtk_box_pack_start (GTK_BOX (vbox1), scrolledwindow1, FALSE, FALSE, 0);

  text1 = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "text1", text1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text1);
  gtk_text_set_editable((GtkText *)text1,FALSE);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), text1);

  /* Connexion des bulles d'aides */
  gtk_object_set_data (GTK_OBJECT (mainwindow), "tooltips", tooltips);
  gtk_window_add_accel_group (GTK_WINDOW (mainwindow), accel_group);

  return mainwindow;
}
/*
** ----------------------------------------------------------------------------
*/
