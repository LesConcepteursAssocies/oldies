/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 02/03/2000
**
** GBmainmenu.c
**
** Création du menu principal
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBmainmenu.h"
#include "GBmisc.h"
#include "GBhelp.h"
#include "GBworld.h"
#include "GBobjets.h"
#include "GBwinparam.h"

/*
** ----------------------------------------------------------------------------
** Création du menu principal du programme
*/
GtkWidget *CreateMainMenu(GtkWidget **pmainw,GtkTooltips **ptool,GtkAccelGroup **paccel)
{
  GtkWidget *mainwindow=*pmainw;
  GtkAccelGroup *accel_group=*paccel;
  GtkTooltips *tooltips=*ptool;
  GtkWidget *mainmenu;
  guint tmp_key;

  GtkWidget *projet1;
  GtkWidget *projet1_menu;
  GtkAccelGroup *projet1_menu_accels;
  GtkWidget *new1;
  GtkWidget *open1;
  GtkWidget *save1;
  GtkWidget *exit1;

  GtkWidget *objets1;
  GtkWidget *objet1_menu;
  GtkAccelGroup *objet1_menu_accels;
  GtkWidget *obnew1;
  GtkWidget *obopen1;

  GtkWidget *textures1;

  GtkWidget *outils1;
  GtkWidget *outils1_menu;
  GtkAccelGroup *outils1_menu_accels;
  GtkWidget *otopt1;
  GtkWidget *otpar1;

  GtkWidget *aide1;
  GtkWidget *aide1_menu;
  GtkAccelGroup *aide1_menu_accels;
  GtkWidget *about1;

  mainmenu = gtk_menu_bar_new ();
  gtk_widget_set_name (mainmenu, "mainmenu");
  gtk_widget_ref (mainmenu);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "mainmenu", mainmenu,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (mainmenu);

  /*
  ** Menu PROJET
  ** -----------
  */
  projet1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (projet1)->child),_("_Projet"));
  gtk_widget_add_accelerator (projet1, "activate_item", accel_group,tmp_key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
  gtk_widget_set_name (projet1, "projet1");
  gtk_widget_ref (projet1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "projet1", projet1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (projet1);
  gtk_container_add (GTK_CONTAINER (mainmenu), projet1);
  gtk_tooltips_set_tip (tooltips, projet1, _("Gestion d'un projet"), NULL);

  projet1_menu = gtk_menu_new ();
  gtk_widget_set_name (projet1_menu, "projet1_menu");
  gtk_widget_ref (projet1_menu);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "projet1_menu", projet1_menu,(GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (projet1), projet1_menu);
  projet1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (projet1_menu));

  new1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (new1)->child),_("_Nouveau..."));
  gtk_widget_add_accelerator (new1, "activate_item", projet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (new1, "new1");
  gtk_widget_ref (new1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "new1", new1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (new1);
  gtk_container_add (GTK_CONTAINER (projet1_menu), new1);
  gtk_tooltips_set_tip (tooltips, new1, _("Creation d'un nouveau projet"), NULL);
  gtk_widget_add_accelerator (new1, "activate", accel_group,GDK_N, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  open1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (open1)->child),_("_Ouvrir..."));
  gtk_widget_add_accelerator (open1, "activate_item", projet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (open1, "open1");
  gtk_widget_ref (open1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "open1", open1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (open1);
  gtk_container_add (GTK_CONTAINER (projet1_menu), open1);
  gtk_tooltips_set_tip (tooltips, open1, _("Ouvre un projet existant"), NULL);
  gtk_widget_add_accelerator (open1, "activate", accel_group,GDK_O, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  save1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (save1)->child),_("_Sauver"));
  gtk_widget_add_accelerator (save1, "activate_item", projet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (save1, "save1");
  gtk_widget_ref (save1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "save1", save1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (save1);
  gtk_container_add (GTK_CONTAINER (projet1_menu), save1);
  gtk_tooltips_set_tip (tooltips, save1, _("Sauve le projet courant"), NULL);
  gtk_widget_add_accelerator (save1, "activate", accel_group,GDK_S, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  exit1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (exit1)->child),_("_Quitter"));
  gtk_widget_add_accelerator (exit1, "activate_item", projet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (exit1, "exit1");
  gtk_widget_ref (exit1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "exit1", exit1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (exit1);
  gtk_container_add (GTK_CONTAINER (projet1_menu), exit1);
  gtk_tooltips_set_tip (tooltips, exit1, _("Fin du programme"), NULL);
  gtk_widget_add_accelerator (exit1, "activate", accel_group,GDK_X, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  /*
  ** Menu OBJETS
  ** -----------
  */
  objets1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (objets1)->child),_("_Objets"));
  gtk_widget_add_accelerator (objets1, "activate_item", accel_group,tmp_key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
  gtk_widget_set_name (objets1, "objets1");
  gtk_widget_ref (objets1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "objets1", objets1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (objets1);
  gtk_container_add (GTK_CONTAINER (mainmenu), objets1);
  gtk_tooltips_set_tip (tooltips, objets1, _("Gestion des objets"), NULL);

  objet1_menu = gtk_menu_new ();
  gtk_widget_set_name (objet1_menu, "objet1_menu");
  gtk_widget_ref (objet1_menu);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "objet1_menu", objet1_menu,(GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (objets1), objet1_menu);
  objet1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (objet1_menu));

  obnew1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (obnew1)->child),_("no_Uveau..."));
  gtk_widget_add_accelerator (obnew1, "activate_item", objet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (obnew1, "obnew1");
  gtk_widget_ref (obnew1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "obnew1", obnew1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (obnew1);
  gtk_container_add (GTK_CONTAINER (objet1_menu), obnew1);
  gtk_tooltips_set_tip (tooltips, obnew1, _("Insertion d'un nouvel objet"), NULL);
  gtk_widget_add_accelerator (obnew1, "activate", accel_group,GDK_U, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  obopen1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (obopen1)->child),_("_Importer..."));
  gtk_widget_add_accelerator (obopen1, "activate_item", objet1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (obopen1, "obopen1");
  gtk_widget_ref (obopen1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "obopen1", obopen1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (obopen1);
  gtk_container_add (GTK_CONTAINER (objet1_menu), obopen1);
  gtk_tooltips_set_tip (tooltips, obopen1, _("Importe un objet..."), NULL);
  gtk_widget_add_accelerator (obopen1, "activate", accel_group,GDK_I, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  /*
  ** Menu TEXTURES
  ** -------------
  */
  textures1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (textures1)->child),_("_Textures"));
  gtk_widget_add_accelerator (textures1, "activate_item", accel_group,tmp_key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
  gtk_widget_set_name (textures1, "textures1");
  gtk_widget_ref (textures1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "textures1", textures1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (textures1);
  gtk_container_add (GTK_CONTAINER (mainmenu), textures1);
  gtk_tooltips_set_tip (tooltips, textures1, _("Gestion des textures"), NULL);

  /*
  ** Menu OUTILS
  ** -----------
  */
  outils1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (outils1)->child),_("o_Utils"));
  gtk_widget_add_accelerator (outils1, "activate_item", accel_group,tmp_key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
  gtk_widget_set_name (outils1, "outils1");
  gtk_widget_ref (outils1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "outils1", outils1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (outils1);
  gtk_container_add (GTK_CONTAINER (mainmenu), outils1);
  gtk_tooltips_set_tip (tooltips, outils1, _("Outils..."), NULL);

  outils1_menu = gtk_menu_new ();
  gtk_widget_set_name (outils1_menu, "outils1_menu");
  gtk_widget_ref (outils1_menu);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "outils1_menu", outils1_menu,(GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (outils1), outils1_menu);
  outils1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (outils1_menu));

  otopt1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (otopt1)->child),_("o_Ptions..."));
  gtk_widget_add_accelerator (otopt1, "activate_item", outils1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (otopt1, "otopt1");
  gtk_widget_ref (otopt1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "otopt1", otopt1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (otopt1);
  gtk_container_add (GTK_CONTAINER (outils1_menu), otopt1);
  gtk_tooltips_set_tip (tooltips, otopt1, _("Options du projet courant"), NULL);
  gtk_widget_add_accelerator (otopt1, "activate", accel_group,GDK_P, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  otpar1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (otpar1)->child),_("p_Aramètres..."));
  gtk_widget_add_accelerator (otpar1, "activate_item", outils1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (otpar1, "otpar1");
  gtk_widget_ref (otpar1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "otpar1", otpar1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (otpar1);
  gtk_container_add (GTK_CONTAINER (outils1_menu), otpar1);
  gtk_tooltips_set_tip (tooltips, otpar1, _("Paramètres du projet courant"), NULL);
  gtk_widget_add_accelerator (otpar1, "activate", accel_group,GDK_A, GDK_CONTROL_MASK,GTK_ACCEL_VISIBLE);

  /*
  ** Menu AIDE
  ** ---------
  */
  aide1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (aide1)->child),_("_Aide"));
  gtk_widget_add_accelerator (aide1, "activate_item", accel_group,tmp_key, GDK_MOD1_MASK, (GtkAccelFlags) 0);
  gtk_widget_set_name (aide1, "aide1");
  gtk_widget_ref (aide1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "aide1", aide1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (aide1);
  gtk_container_add (GTK_CONTAINER (mainmenu), aide1);
  gtk_tooltips_set_tip (tooltips, aide1, _("Aide"), NULL);
  gtk_menu_item_right_justify (GTK_MENU_ITEM (aide1));

  aide1_menu = gtk_menu_new ();
  gtk_widget_set_name (aide1_menu, "aide1_menu");
  gtk_widget_ref (aide1_menu);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "aide1_menu", aide1_menu,(GtkDestroyNotify) gtk_widget_unref);
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (aide1), aide1_menu);
  aide1_menu_accels = gtk_menu_ensure_uline_accel_group (GTK_MENU (aide1_menu));

  about1 = gtk_menu_item_new_with_label ("");
  tmp_key = gtk_label_parse_uline (GTK_LABEL (GTK_BIN (about1)->child),_("_A propos..."));
  gtk_widget_add_accelerator (about1, "activate_item", aide1_menu_accels,tmp_key, 0, (GtkAccelFlags) 0);
  gtk_widget_set_name (about1, "about1");
  gtk_widget_ref (about1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "about1", about1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (about1);
  gtk_container_add (GTK_CONTAINER (aide1_menu), about1);
  gtk_tooltips_set_tip (tooltips, about1, _("A propos de gbuild..."), NULL);

  /* Connexion des options */
  /* Menu Projets */
  /*      Projet->Nouveau */
  gtk_signal_connect(GTK_OBJECT(new1),"activate",GTK_SIGNAL_FUNC(NewWorld),NULL);
  /*      Projet->Ouvrir  */
  gtk_signal_connect(GTK_OBJECT(open1),"activate",GTK_SIGNAL_FUNC(OpenWorld),NULL);
  /*      Projet->Sauver  */
  gtk_signal_connect(GTK_OBJECT(save1),"activate",GTK_SIGNAL_FUNC(SaveWorld),NULL);
  /*      Projet->Quitter */
  gtk_signal_connect(GTK_OBJECT(exit1),"activate",GTK_SIGNAL_FUNC(GBExit),NULL);

  /* Menu Objets */
  /*      Objet->Nouveau */
  gtk_signal_connect(GTK_OBJECT(obnew1),"activate",GTK_SIGNAL_FUNC(ObjetNew),NULL);
  /*      Objet->Ouvrir  */
  gtk_signal_connect(GTK_OBJECT(obopen1),"activate",GTK_SIGNAL_FUNC(OpenObjet),NULL);

  /* Menu Outils */
  /*      Outils->options */
  gtk_signal_connect(GTK_OBJECT(otopt1),"activate",GTK_SIGNAL_FUNC(OpenProjetOptions),NULL);

  /* Menu Aide */
  /*      Aide->A propos */
  gtk_signal_connect(GTK_OBJECT(about1),"activate",GTK_SIGNAL_FUNC(APropos_cb),NULL);

  return mainmenu;
}
/*
** ----------------------------------------------------------------------------
*/
