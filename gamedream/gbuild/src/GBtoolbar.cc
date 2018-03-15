/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 02/03/2000
**
** GBtoolbar.c
**
** Création des barres d'icones sous le menu principal
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBtoolbar.h"
#include "GBmisc.h"
#include "GBworld.h"
#include "GBobjets.h"
#include "GBwinmvt.h"
#include "GBwinparam.h"

/*
** ----------------------------------------------------------------------------
*/
GtkWidget *CreateMainToolbar(GtkWidget **pmainw)
{
  GtkWidget *mainwindow=*pmainw;
  GtkWidget *toolbar;

  toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,GTK_TOOLBAR_BOTH);
  gtk_container_set_border_width (GTK_CONTAINER(toolbar), 5);
  gtk_toolbar_set_space_size (GTK_TOOLBAR(toolbar), 16);
  gtk_widget_set_name (toolbar, "toolbar1");
  gtk_widget_ref (toolbar);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "toolbar1", toolbar,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (toolbar);

  /* Ajout des différents bouttons */
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Quitter"),NULL,pixmap_new("exit.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Nouveau projet"),NULL,pixmap_new("new.xpm",mainwindow),GTK_SIGNAL_FUNC(NewWorld),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Ouvrir un projet"),NULL,pixmap_new("file.xpm",mainwindow),GTK_SIGNAL_FUNC(OpenWorld),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Sauver le projet courant"),NULL,pixmap_new("save.xpm",mainwindow),GTK_SIGNAL_FUNC(SaveWorld),NULL);
  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Point de vue"),NULL,pixmap_new("viewpoint.xpm",mainwindow),GTK_SIGNAL_FUNC(OpenMvt),NULL);
  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Options du projet"),NULL,pixmap_new("config.xpm",mainwindow),GTK_SIGNAL_FUNC(OpenProjetOptions),NULL);
  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));

  return toolbar;
}
/*
** ----------------------------------------------------------------------------
** Création de la barre d'icones des primitives 3D
*/
GtkWidget *CreatePrim3DToolbar(GtkWidget **pmainw)
{
  GtkWidget *mainwindow=*pmainw;
  GtkWidget *toolbar;

  toolbar = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL,GTK_TOOLBAR_BOTH);
  gtk_container_set_border_width (GTK_CONTAINER(toolbar), 5);
  gtk_toolbar_set_space_size (GTK_TOOLBAR(toolbar), 16);
  gtk_widget_set_name (toolbar, "toolbar2");
  gtk_widget_ref (toolbar);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "toolbar2", toolbar,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (toolbar);

  /* Ajout des différents bouttons */
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Cube"),NULL,pixmap_new("prm_cube.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Cylindre"),NULL,pixmap_new("prm_cyln.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Cone"),NULL,pixmap_new("prm_cone.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Sphère"),NULL,pixmap_new("prm_sphr.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Plan"),NULL,pixmap_new("prm_plne.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Tore"),NULL,pixmap_new("prm_toru.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Texte"),NULL,pixmap_new("prm_ttxt.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);

  gtk_toolbar_append_space(GTK_TOOLBAR(toolbar));
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Caméra"),NULL,pixmap_new("prm_cam.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);
  gtk_toolbar_append_item(GTK_TOOLBAR(toolbar),NULL,_("Lumière"),NULL,pixmap_new("prm_lght.xpm",mainwindow),GTK_SIGNAL_FUNC(GBExit),NULL);

  return toolbar;
}
/*
** ----------------------------------------------------------------------------
*/
