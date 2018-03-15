/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 17/03/2000
**
** GBmisc.cc
**
** Fonctions diverses
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBmisc.h"
#include "GBcamera.h"
#include "GBwinmvt.h"
#include "GBworld.h"

/*
** ----------------------------------------------------------------------------
** Variables
*/
GBGLOBAL GDvision   Vision;
GBGLOBAL GDcamera   Camera;
GBGLOBAL GDworld_t  *CurrentWorld;

/*
** ----------------------------------------------------------------------------
** Recherche de widget_name dans widget.
** Renvoi un pointeur sur le widget trouvé, NULL sinon.
*/
GtkWidget *lookup_widget(GtkWidget *widget,const gchar *widget_name)
{
  GtkWidget *parent,*found_widget;

  for (;;)
  {
    if (GTK_IS_MENU (widget))
      parent = gtk_menu_get_attach_widget(GTK_MENU(widget));
    else
      parent = widget->parent;
    if (parent == NULL)
      break;
    widget = parent;
  }

  found_widget = (GtkWidget*) gtk_object_get_data (GTK_OBJECT (widget),widget_name);
  if (!found_widget)
    g_warning (_("Widget non trouvé: %s"), widget_name);
  return found_widget;
}
/*
** ----------------------------------------------------------------------------
** Affichage d'un message dans la zone de texte principale
*/
void GBPrintf(const char *format, ...)
{
  GtkWidget *textarea=NULL;
  gchar msg[1024];
  va_list args;

  va_start(args,format);
  vsprintf(msg,format,args);
  va_end(args);

  if(mainwindow)
    textarea = lookup_widget(mainwindow,"text1");
  if(textarea)
    gtk_text_insert(GTK_TEXT(textarea), NULL, NULL, NULL, msg, strlen(msg));
  else
    g_print(msg);
}
/*
** ----------------------------------------------------------------------------
** Ajout du répertoire donné à la liste donnée
*/
void GBAddListDir(GList **list,const gchar *dir)
{
  *list = g_list_prepend(*list,g_strdup(dir));
  //  g_print("ajout rep [%s]\n",dir);
}
/*
** ----------------------------------------------------------------------------
** Teste l'existence du fichier donné dans le répertoire donné
** Renvoie le nom complet si trouvé, NULL sinon.
*/
gchar *GBCheckFileExist(const gchar *directory,const gchar *filename)
{
  gchar *full_filename;
  struct stat s;
  gint status;

  full_filename = (gchar*) g_malloc(strlen(directory) + 1 + strlen(filename) + 1);
  strcpy(full_filename, directory);
  strcat(full_filename, G_DIR_SEPARATOR_S);
  strcat(full_filename, filename);

  status = stat (full_filename, &s);
  if (status == 0 && S_ISREG (s.st_mode))
    return full_filename;
  g_free (full_filename);
  return NULL;
}
/*
** ----------------------------------------------------------------------------
** Création d'un pixmap à partir du fichier donné
*/
GtkWidget *pixmap_new(char *filename,GtkWidget *window)
{
  GdkPixmap *gdk_pixmap;
  GdkBitmap *mask;
  GtkWidget *pixmap=NULL;
  gchar *real_filename=NULL;
  GList *elem;

  // On cherche le fichier
  elem = PixmapsDirs;
  while(elem)
  {
    real_filename = GBCheckFileExist((gchar *)elem->data, filename);
    if(real_filename)
      break;
    elem = elem->next;
  }

  if(!real_filename)
    real_filename = GBCheckFileExist("./pixmaps", filename);

  if(!real_filename)
    real_filename = GBCheckFileExist("../pixmaps", filename);

  if(real_filename)
  {
    gdk_pixmap = gdk_pixmap_create_from_xpm(window->window, &mask,&window->style->bg[GTK_STATE_NORMAL],real_filename);
    pixmap = gtk_pixmap_new(gdk_pixmap, mask);
    gtk_widget_show(pixmap);
  }
  else
    GBPrintf("pixmap %s non trouvé!!!\n",filename);

  g_free(real_filename);
  return pixmap;
}
/*
** ----------------------------------------------------------------------------
** Création d'une fenêtre de sélection de fichier
*/
GtkWidget *create_fileselection(char *titre,gpointer ok_cb,gpointer cancel_cb)
{
  GtkWidget *fileselection;
  GtkWidget *file_ok_button;
  GtkWidget *file_cancel_button;

  fileselection = gtk_file_selection_new(titre);
  gtk_object_set_data (GTK_OBJECT (fileselection), "fileselection", fileselection);
  // gtk_container_set_border_width (GTK_CONTAINER (fileselection), 10);
  gtk_file_selection_hide_fileop_buttons (GTK_FILE_SELECTION (fileselection));

  file_ok_button = GTK_FILE_SELECTION (fileselection)->ok_button;
  gtk_object_set_data (GTK_OBJECT (fileselection), "file_ok_button", file_ok_button);
  gtk_widget_show (file_ok_button);
  GTK_WIDGET_SET_FLAGS (file_ok_button, GTK_CAN_DEFAULT);

  file_cancel_button = GTK_FILE_SELECTION (fileselection)->cancel_button;
  gtk_object_set_data (GTK_OBJECT (fileselection), "file_cancel_button", file_cancel_button);
  gtk_widget_show (file_cancel_button);
  GTK_WIDGET_SET_FLAGS (file_cancel_button, GTK_CAN_DEFAULT);

  // Connexion des boutons
  gtk_signal_connect (GTK_OBJECT (fileselection), "delete_event",GTK_SIGNAL_FUNC(cancel_cb),fileselection);
  gtk_signal_connect (GTK_OBJECT (file_ok_button), "clicked",GTK_SIGNAL_FUNC(ok_cb),fileselection);
  gtk_signal_connect (GTK_OBJECT (file_cancel_button), "clicked",GTK_SIGNAL_FUNC(cancel_cb),fileselection);

  return fileselection;
}
/*
** ----------------------------------------------------------------------------
** Création d'une fenêtre de sélection de couleur, et l'initialise avec la
** couleur donnée.
*/
GtkWidget *create_colorselection(char *titre,gpointer ok_cb,gpointer cancel_cb,gfloat *vcol)
{
  GtkWidget *colorselectiondialog;
  GtkWidget *color_ok_button;
  GtkWidget *color_cancel_button;
  GtkColorSelectionDialog *csel;
  gdouble rgb[4];

  colorselectiondialog = gtk_color_selection_dialog_new (titre);
  gtk_widget_set_name (colorselectiondialog, "colorselectiondialog");
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "colorselectiondialog", colorselectiondialog);
  //  gtk_container_set_border_width (GTK_CONTAINER (colorselectiondialog), 10);
  csel = GTK_COLOR_SELECTION_DIALOG (colorselectiondialog);
  gtk_widget_hide (GTK_WIDGET (csel->help_button));

  color_ok_button = csel->ok_button;
  gtk_widget_set_name (color_ok_button, "color_ok_button");
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "color_ok_button", color_ok_button);
  gtk_widget_show (color_ok_button);
  GTK_WIDGET_SET_FLAGS (color_ok_button, GTK_CAN_DEFAULT);

  color_cancel_button = csel->cancel_button;
  gtk_widget_set_name (color_cancel_button, "color_cancel_button");
  gtk_object_set_data (GTK_OBJECT (colorselectiondialog), "color_cancel_button", color_cancel_button);
  gtk_widget_show (color_cancel_button);
  GTK_WIDGET_SET_FLAGS (color_cancel_button, GTK_CAN_DEFAULT);

  // Connexion des boutons
  gtk_signal_connect (GTK_OBJECT (colorselectiondialog), "delete_event",GTK_SIGNAL_FUNC(cancel_cb),colorselectiondialog);
  gtk_signal_connect (GTK_OBJECT (color_ok_button), "clicked",GTK_SIGNAL_FUNC(ok_cb),(gpointer) vcol);
  gtk_signal_connect (GTK_OBJECT (color_cancel_button), "clicked",GTK_SIGNAL_FUNC(cancel_cb),colorselectiondialog);

  // Initialise la couleur
  if(vcol)
  {
    rgb[0] = (gdouble) vcol[0];
    rgb[1] = (gdouble) vcol[1];
    rgb[2] = (gdouble) vcol[2];
    gtk_color_selection_set_color (GTK_COLOR_SELECTION (csel->colorsel),rgb);
  }

  return colorselectiondialog;
}
/*
** ----------------------------------------------------------------------------
** Initialisations générales de gbuild
*/
void GBInit(void)
{
  GBPrintf("GBInit\n");
  gd_auteur  = g_strdup_printf("%s","Guillaumaud Philippe");
  gd_titre   = g_strdup_printf("%s %s",PACKAGE,VERSION);
  gd_apropos = g_strdup_printf("%s\n3D Game's Builder\n%s\n1999 - 2000",gd_titre,gd_auteur);

  /*
  ** Paramètres par défaut
  */
  SelectMode  = FALSE;

  bDrawAxeX   = TRUE;
  AxeXcol[0]  = 1.0;
  AxeXcol[1]  = 0.0;
  AxeXcol[2]  = 0.0;
  bDrawAxeY   = TRUE;
  AxeYcol[0]  = 0.0;
  AxeYcol[1]  = 1.0;
  AxeYcol[2]  = 0.0;
  bDrawAxeZ   = TRUE;
  AxeZcol[0]  = 0.0;
  AxeZcol[1]  = 0.0;
  AxeZcol[2]  = 1.0;
  bDrawGridX  = FALSE;
  GridXcol[0] = 1.0;
  GridXcol[1] = 0.0;
  GridXcol[2] = 0.0;
  GridSzX     = 100;
  GridSpX     = 1;
  bDrawGridY  = FALSE;
  GridYcol[0] = 0.0;
  GridYcol[1] = 1.0;
  GridYcol[2] = 0.0;
  GridSzY     = 100;
  GridSpY     = 1;
  bDrawGridZ  = FALSE;
  GridZcol[0] = 0.0;
  GridZcol[1] = 0.0;
  GridZcol[2] = 1.0;
  GridSzZ     = 100;
  GridSpZ     = 1;

  pickname    = 1;
  /*
  ** Création du cone de vision principal
  */
  // Vision = CreateVision(GD_DEFAULT_FOV,GD_DEFAULT_ZNEAR,GD_DEFAULT_ZFAR,GLWIDTH,GLHEIGHT);
  /*
  ** Création de la caméra principale
  */
  //  Camera = CreateCamera(GD_DEFAULT_CAM_TYPE,GD_DEFAULT_CAM_PX,GD_DEFAULT_CAM_PY,GD_DEFAULT_CAM_PZ,GD_DEFAULT_CAM_RX,GD_DEFAULT_CAM_RY,GD_DEFAULT_CAM_RZ,GD_DEFAULT_CAM_AX,GD_DEFAULT_CAM_AY,GD_DEFAULT_CAM_AZ);

  /*
  ** Création du monde de test
  */
  MakeWorldTest();
}
/*
** ----------------------------------------------------------------------------
** Sortie du programme
*/
void GBExit(GtkWidget *widget,gpointer data)
{
  g_print("SORTIE\n");
  DeleteWorld(CurrentWorld);
  gtk_main_quit();
}
/*
** ----------------------------------------------------------------------------
*/
