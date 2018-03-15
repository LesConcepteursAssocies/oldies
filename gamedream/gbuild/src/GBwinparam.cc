/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 02/03/2000
**
** GBwinparam.c
**
** Gestion des paramètres du projet courant.
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBmisc.h"
#include "GBglarea.h"
#include "GBglmisc.h"
#include "GBwinparam.h"


/*
** ----------------------------------------------------------------------------
** Variables
*/
static GtkWidget *Parametres=NULL;

static gboolean   oldbDrawAxeX;
static gfloat     oldAxeXcol[3];
static gboolean   oldbDrawAxeY;
static gfloat     oldAxeYcol[3];
static gboolean   oldbDrawAxeZ;
static gfloat     oldAxeZcol[3];
static gboolean   oldbDrawGridX;
static gfloat     oldGridXcol[3];
static gint       oldGridSzX;
static gint       oldGridSpX;
static gboolean   oldbDrawGridY;
static gfloat     oldGridYcol[3];
static gint       oldGridSzY;
static gint       oldGridSpY;
static gboolean   oldbDrawGridZ;
static gfloat     oldGridZcol[3];
static gint       oldGridSzZ;
static gint       oldGridSpZ;

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static void       color_ok_cb(GtkButton *,gpointer);
static void       color_cancel_cb(GtkButton *,gpointer);
static void       DrawColorPixmap(GtkWidget *,gfloat *);
static gboolean   on_draw_color_expose_event(GtkWidget *,GdkEventExpose *,gpointer);
static void       on_check_axe_z_toggled(GtkToggleButton *,gpointer);
static void       on_check_axe_y_toggled(GtkToggleButton *,gpointer);
static void       on_check_axe_x_toggled(GtkToggleButton *,gpointer);
static gboolean   on_draw_color_axe_x_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean   on_draw_color_axe_y_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean   on_draw_color_axe_z_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static void       on_check_grid_x_toggled(GtkToggleButton *,gpointer);
static void       on_check_grid_y_toggled(GtkToggleButton *,gpointer);
static void       on_check_grid_z_toggled(GtkToggleButton *,gpointer);
static void       on_spin_grid_sz_x_changed(GtkSpinButton *,gpointer);
static void       on_spin_grid_sz_y_changed(GtkSpinButton *,gpointer);
static void       on_spin_grid_sz_z_changed(GtkSpinButton *,gpointer);
static void       on_spin_grid_sp_x_changed(GtkSpinButton *,gpointer);
static void       on_spin_grid_sp_y_changed(GtkSpinButton *,gpointer);
static void       on_spin_grid_sp_z_changed(GtkSpinButton *,gpointer);
static gboolean   on_draw_color_grid_x_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean   on_draw_color_grid_y_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean   on_draw_color_grid_z_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static void       on_ok_opt_clicked(GtkButton *,gpointer);
static void       on_cancel_opt_clicked(GtkButton *,gpointer);
static void       on_default_opt_clicked(GtkButton *,gpointer);

static GtkWidget *CreateMainParam(void);
static void       RedrawColorPixmap(void);
static void       UpdateParam(void);

/*
** ----------------------------------------------------------------------------
** Choix d'une couleur
*/
static void color_ok_cb(GtkButton *button,gpointer user_data)
{
  GtkWidget *widget;
  GtkColorSelectionDialog *csel;
  gdouble rgb[4];
  gfloat *vcol = (gfloat *) user_data;

  widget = lookup_widget((GtkWidget*) button, "colorselectiondialog");
  csel = GTK_COLOR_SELECTION_DIALOG (widget);
  gtk_color_selection_get_color (GTK_COLOR_SELECTION (csel->colorsel),rgb);
  vcol[0] = (gfloat) rgb[0];
  vcol[1] = (gfloat) rgb[1];
  vcol[2] = (gfloat) rgb[2];
  gtk_widget_destroy(widget);

  // On redessine le pixmap
  RedrawColorPixmap();
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
** Annulation sur le choix d'une couleur
*/
static void color_cancel_cb(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}
/*
** ----------------------------------------------------------------------------
** Affiche un pixmap composé de la couleur donnée, dans le widget donné.
*/
static void DrawColorPixmap(GtkWidget *widget,gfloat *vcol)
{
  GdkPixmap *pixmap;
  GdkGC *gc;
  GdkColor color;
  GdkColormap *colormap;

  gc          = gdk_gc_new(widget->window);
  colormap    = gdk_window_get_colormap(widget->window);
  color.red   = (gint) (vcol[0] * 65535.0);
  color.green = (gint) (vcol[1] * 65535.0);
  color.blue  = (gint) (vcol[2] * 65535.0);
  gdk_color_alloc(colormap,&color);
  gdk_gc_set_foreground(gc,&color);

  pixmap = gdk_pixmap_new(widget->window,widget->allocation.width,widget->allocation.height,-1);
  gdk_draw_rectangle (pixmap,gc,TRUE,0, 0,widget->allocation.width,widget->allocation.height);
  gdk_draw_pixmap(widget->window,widget->style->fg_gc[GTK_WIDGET_STATE (widget)],pixmap,0,0,0,0,widget->allocation.width, widget->allocation.height);

  gdk_pixmap_unref(pixmap);
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_expose_event(GtkWidget *widget,GdkEventExpose *event,gpointer user_data)
{
  DrawColorPixmap(widget,(gfloat *)user_data);
  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_axe_z_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawAxeZ = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_axe_y_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawAxeY = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_axe_x_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawAxeX = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_axe_x_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de l'axe X"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) AxeXcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_axe_y_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de l'axe Y"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) AxeYcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_axe_z_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de l'axe Z"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) AxeZcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_grid_x_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawGridX = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_grid_y_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawGridY = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_check_grid_z_toggled(GtkToggleButton *togglebutton,gpointer user_data)
{
  bDrawGridZ = togglebutton->active;
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sz_x_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSzX = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sz_y_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSzY = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sz_z_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSzZ = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sp_x_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSpX = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sp_y_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSpY = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_grid_sp_z_changed(GtkSpinButton *spin,gpointer user_data)
{
  GridSpZ = gtk_spin_button_get_value_as_int(spin);
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_grid_x_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de la grille X"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) GridXcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_grid_y_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de la grille Y"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) GridYcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_draw_color_grid_z_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer user_data)
{
  GtkWidget *coldlg;

  coldlg = create_colorselection(_("Couleur de la grille Z"),(gpointer)color_ok_cb,(gpointer)color_cancel_cb,(gpointer) GridZcol);
  gtk_widget_show(coldlg);

  return FALSE;
}
/*
** ----------------------------------------------------------------------------
*/
static void on_ok_opt_clicked(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(Parametres);
  Parametres=NULL;
}
/*
** ----------------------------------------------------------------------------
** Annulation des choix
*/
static void on_cancel_opt_clicked(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(Parametres);
  Parametres=NULL;

  // Restaure les paramètres
  bDrawAxeX   = oldbDrawAxeX;
  AxeXcol[0]  = oldAxeXcol[0];
  AxeXcol[1]  = oldAxeXcol[1];
  AxeXcol[2]  = oldAxeXcol[2];
  bDrawAxeY   = oldbDrawAxeY;
  AxeYcol[0]  = oldAxeYcol[0];
  AxeYcol[1]  = oldAxeYcol[1];
  AxeYcol[2]  = oldAxeYcol[2];
  bDrawAxeZ   = oldbDrawAxeZ;
  AxeZcol[0]  = oldAxeZcol[0];
  AxeZcol[1]  = oldAxeZcol[1];
  AxeZcol[2]  = oldAxeZcol[2];
  bDrawGridX  = oldbDrawGridX;
  GridXcol[0] = oldGridXcol[0];
  GridXcol[1] = oldGridXcol[1];
  GridXcol[2] = oldGridXcol[2];
  GridSzX     = oldGridSzX;
  GridSpX     = oldGridSpX;
  bDrawGridY  = oldbDrawGridY;
  GridYcol[0] = oldGridYcol[0];
  GridYcol[1] = oldGridYcol[1];
  GridYcol[2] = oldGridYcol[2];
  GridSzY     = oldGridSzY;
  GridSpY     = oldGridSpY;
  bDrawGridZ  = oldbDrawGridZ;
  GridZcol[0] = oldGridZcol[0];
  GridZcol[1] = oldGridZcol[1];
  GridZcol[2] = oldGridZcol[2];
  GridSzZ     = oldGridSzZ;
  GridSpZ     = oldGridSpZ;

  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
** Restaure les paramètres par défaut des options.
*/
static void on_default_opt_clicked(GtkButton *button,gpointer user_data)
{
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
  // On redessine le pixmap
  RedrawColorPixmap();
  UpdateParam();
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
** Création de la fenêtre principale de gestion des paramètres
** Renvoi un pointeur sur la fenêtre créée.
*/
static GtkWidget *CreateMainParam(void)
{
  GtkWidget *optprojet;
  GtkWidget *vbox1;
  GtkWidget *table2;
  GtkWidget *frame_axes;
  GtkWidget *table_axes;
  GtkWidget *check_axe_z;
  GtkWidget *check_axe_y;
  GtkWidget *check_axe_x;
  GtkWidget *frame_color_axe_x;
  GtkWidget *draw_color_axe_x;
  GtkWidget *label_color_axe_x;
  GtkWidget *label_color_axe_y;
  GtkWidget *labelcolor_axe_z;
  GtkWidget *frame_color_axe_y;
  GtkWidget *draw_color_axe_y;
  GtkWidget *frame_color_axe_z;
  GtkWidget *draw_color_axe_z;
  GtkWidget *frame_grid;
  GtkWidget *table_grid;
  GtkWidget *check_grid_x;
  GtkWidget *check_grid_y;
  GtkWidget *check_grid_z;
  GtkWidget *label_grid_sz_x;
  GtkWidget *label_grid_sz_y;
  GtkWidget *label_grid_sz_z;
  GtkWidget *label_grid_sp_x;
  GtkWidget *label_grid_sp_y;
  GtkWidget *label_grid_sp_z;
  GtkObject *spin_grid_sz_x_adj;
  GtkWidget *spin_grid_sz_x;
  GtkObject *spin_grid_sz_y_adj;
  GtkWidget *spin_grid_sz_y;
  GtkObject *spin_grid_sz_z_adj;
  GtkWidget *spin_grid_sz_z;
  GtkObject *spin_grid_sp_x_adj;
  GtkWidget *spin_grid_sp_x;
  GtkObject *spin_grid_sp_y_adj;
  GtkWidget *spin_grid_sp_y;
  GtkObject *spin_grid_sp_z_adj;
  GtkWidget *spin_grid_sp_z;
  GtkWidget *label_color_grid_x;
  GtkWidget *label_color_grid_y;
  GtkWidget *label_color_grid_z;
  GtkWidget *frame_color_grid_x;
  GtkWidget *draw_color_grid_x;
  GtkWidget *frame_color_grid_y;
  GtkWidget *draw_color_grid_y;
  GtkWidget *frame_color_grid_z;
  GtkWidget *draw_color_grid_z;
  GtkWidget *table1;
  GtkWidget *ok_opt;
  GtkWidget *cancel_opt;
  GtkWidget *default_opt;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  optprojet = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (optprojet, "optprojet");
  gtk_object_set_data (GTK_OBJECT (optprojet), "optprojet", optprojet);
  gtk_window_set_title (GTK_WINDOW (optprojet), _("Options générales"));

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_set_name (vbox1, "vbox1");
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "vbox1", vbox1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (optprojet), vbox1);

  table2 = gtk_table_new (4, 1, FALSE);
  gtk_widget_set_name (table2, "table2");
  gtk_widget_ref (table2);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "table2", table2,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table2);
  gtk_box_pack_start (GTK_BOX (vbox1), table2, TRUE, TRUE, 0);

  frame_axes = gtk_frame_new (_("Visualisation des axes"));
  gtk_widget_set_name (frame_axes, "frame_axes");
  gtk_widget_ref (frame_axes);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_axes", frame_axes,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_axes);
  gtk_table_attach (GTK_TABLE (table2), frame_axes, 0, 1, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame_axes), 5);

  table_axes = gtk_table_new (3, 3, TRUE);
  gtk_widget_set_name (table_axes, "table_axes");
  gtk_widget_ref (table_axes);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "table_axes", table_axes,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table_axes);
  gtk_container_add (GTK_CONTAINER (frame_axes), table_axes);
  gtk_table_set_row_spacings (GTK_TABLE (table_axes), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table_axes), 5);

  check_axe_z = gtk_check_button_new_with_label (_("Z"));
  gtk_widget_set_name (check_axe_z, "check_axe_z");
  gtk_widget_ref (check_axe_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_axe_z", check_axe_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_axe_z);
  gtk_table_attach (GTK_TABLE (table_axes), check_axe_z, 0, 1, 2, 3,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);

  check_axe_y = gtk_check_button_new_with_label (_("Y"));
  gtk_widget_set_name (check_axe_y, "check_axe_y");
  gtk_widget_ref (check_axe_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_axe_y", check_axe_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_axe_y);
  gtk_table_attach (GTK_TABLE (table_axes), check_axe_y, 0, 1, 1, 2,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);

  check_axe_x = gtk_check_button_new_with_label (_("X"));
  gtk_widget_set_name (check_axe_x, "check_axe_x");
  gtk_widget_ref (check_axe_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_axe_x", check_axe_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_axe_x);
  gtk_table_attach (GTK_TABLE (table_axes), check_axe_x, 0, 1, 0, 1,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);

  frame_color_axe_x = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_axe_x, "frame_color_axe_x");
  gtk_widget_ref (frame_color_axe_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_axe_x", frame_color_axe_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_axe_x);
  gtk_table_attach (GTK_TABLE (table_axes), frame_color_axe_x, 2, 3, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_axe_x), GTK_SHADOW_OUT);

  draw_color_axe_x = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_axe_x, "draw_color_axe_x");
  gtk_widget_ref (draw_color_axe_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_axe_x", draw_color_axe_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_axe_x);
  gtk_container_add (GTK_CONTAINER (frame_color_axe_x), draw_color_axe_x);
  gtk_tooltips_set_tip (tooltips, draw_color_axe_x, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_axe_x, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  label_color_axe_x = gtk_label_new (_("couleur"));
  gtk_widget_set_name (label_color_axe_x, "label_color_axe_x");
  gtk_widget_ref (label_color_axe_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_color_axe_x", label_color_axe_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_color_axe_x);
  gtk_table_attach (GTK_TABLE (table_axes), label_color_axe_x, 1, 2, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_color_axe_x), GTK_JUSTIFY_RIGHT);

  label_color_axe_y = gtk_label_new (_("couleur"));
  gtk_widget_set_name (label_color_axe_y, "label_color_axe_y");
  gtk_widget_ref (label_color_axe_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_color_axe_y", label_color_axe_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_color_axe_y);
  gtk_table_attach (GTK_TABLE (table_axes), label_color_axe_y, 1, 2, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_color_axe_y), GTK_JUSTIFY_RIGHT);

  labelcolor_axe_z = gtk_label_new (_("couleur"));
  gtk_widget_set_name (labelcolor_axe_z, "labelcolor_axe_z");
  gtk_widget_ref (labelcolor_axe_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "labelcolor_axe_z", labelcolor_axe_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (labelcolor_axe_z);
  gtk_table_attach (GTK_TABLE (table_axes), labelcolor_axe_z, 1, 2, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (labelcolor_axe_z), GTK_JUSTIFY_RIGHT);

  frame_color_axe_y = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_axe_y, "frame_color_axe_y");
  gtk_widget_ref (frame_color_axe_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_axe_y", frame_color_axe_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_axe_y);
  gtk_table_attach (GTK_TABLE (table_axes), frame_color_axe_y, 2, 3, 1, 2,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_axe_y), GTK_SHADOW_OUT);

  draw_color_axe_y = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_axe_y, "draw_color_axe_y");
  gtk_widget_ref (draw_color_axe_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_axe_y", draw_color_axe_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_axe_y);
  gtk_container_add (GTK_CONTAINER (frame_color_axe_y), draw_color_axe_y);
  gtk_tooltips_set_tip (tooltips, draw_color_axe_y, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_axe_y, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  frame_color_axe_z = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_axe_z, "frame_color_axe_z");
  gtk_widget_ref (frame_color_axe_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_axe_z", frame_color_axe_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_axe_z);
  gtk_table_attach (GTK_TABLE (table_axes), frame_color_axe_z, 2, 3, 2, 3,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_axe_z), GTK_SHADOW_OUT);

  draw_color_axe_z = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_axe_z, "draw_color_axe_z");
  gtk_widget_ref (draw_color_axe_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_axe_z", draw_color_axe_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_axe_z);
  gtk_container_add (GTK_CONTAINER (frame_color_axe_z), draw_color_axe_z);
  gtk_tooltips_set_tip (tooltips, draw_color_axe_z, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_axe_z, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  frame_grid = gtk_frame_new (_("Visualisation des grilles"));
  gtk_widget_set_name (frame_grid, "frame_grid");
  gtk_widget_ref (frame_grid);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_grid", frame_grid,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_grid);
  gtk_table_attach (GTK_TABLE (table2), frame_grid, 0, 1, 1, 2,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (frame_grid), 5);

  table_grid = gtk_table_new (3, 7, TRUE);
  gtk_widget_set_name (table_grid, "table_grid");
  gtk_widget_ref (table_grid);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "table_grid", table_grid,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table_grid);
  gtk_container_add (GTK_CONTAINER (frame_grid), table_grid);
  gtk_table_set_row_spacings (GTK_TABLE (table_grid), 5);
  gtk_table_set_col_spacings (GTK_TABLE (table_grid), 5);

  check_grid_x = gtk_check_button_new_with_label (_("X"));
  gtk_widget_set_name (check_grid_x, "check_grid_x");
  gtk_widget_ref (check_grid_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_grid_x", check_grid_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_grid_x);
  gtk_table_attach (GTK_TABLE (table_grid), check_grid_x, 0, 1, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);

  check_grid_y = gtk_check_button_new_with_label (_("Y"));
  gtk_widget_set_name (check_grid_y, "check_grid_y");
  gtk_widget_ref (check_grid_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_grid_y", check_grid_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_grid_y);
  gtk_table_attach (GTK_TABLE (table_grid), check_grid_y, 0, 1, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);

  check_grid_z = gtk_check_button_new_with_label (_("Z"));
  gtk_widget_set_name (check_grid_z, "check_grid_z");
  gtk_widget_ref (check_grid_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "check_grid_z", check_grid_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (check_grid_z);
  gtk_table_attach (GTK_TABLE (table_grid), check_grid_z, 0, 1, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);

  label_grid_sz_x = gtk_label_new (_("taille"));
  gtk_widget_set_name (label_grid_sz_x, "label_grid_sz_x");
  gtk_widget_ref (label_grid_sz_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sz_x", label_grid_sz_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sz_x);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sz_x, 1, 2, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sz_x), GTK_JUSTIFY_RIGHT);

  label_grid_sz_y = gtk_label_new (_("taille"));
  gtk_widget_set_name (label_grid_sz_y, "label_grid_sz_y");
  gtk_widget_ref (label_grid_sz_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sz_y", label_grid_sz_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sz_y);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sz_y, 1, 2, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sz_y), GTK_JUSTIFY_RIGHT);

  label_grid_sz_z = gtk_label_new (_("taille"));
  gtk_widget_set_name (label_grid_sz_z, "label_grid_sz_z");
  gtk_widget_ref (label_grid_sz_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sz_z", label_grid_sz_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sz_z);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sz_z, 1, 2, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sz_z), GTK_JUSTIFY_RIGHT);

  label_grid_sp_x = gtk_label_new (_("pas"));
  gtk_widget_set_name (label_grid_sp_x, "label_grid_sp_x");
  gtk_widget_ref (label_grid_sp_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sp_x", label_grid_sp_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sp_x);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sp_x, 3, 4, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sp_x), GTK_JUSTIFY_RIGHT);

  label_grid_sp_y = gtk_label_new (_("pas"));
  gtk_widget_set_name (label_grid_sp_y, "label_grid_sp_y");
  gtk_widget_ref (label_grid_sp_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sp_y", label_grid_sp_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sp_y);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sp_y, 3, 4, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sp_y), GTK_JUSTIFY_RIGHT);

  label_grid_sp_z = gtk_label_new (_("pas"));
  gtk_widget_set_name (label_grid_sp_z, "label_grid_sp_z");
  gtk_widget_ref (label_grid_sp_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_grid_sp_z", label_grid_sp_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_grid_sp_z);
  gtk_table_attach (GTK_TABLE (table_grid), label_grid_sp_z, 3, 4, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_grid_sp_z), GTK_JUSTIFY_RIGHT);

  spin_grid_sz_x_adj = gtk_adjustment_new (1, 1, 1000, 1, 10, 10);
  spin_grid_sz_x = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sz_x_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sz_x, "spin_grid_sz_x");
  gtk_widget_ref (spin_grid_sz_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sz_x", spin_grid_sz_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sz_x);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sz_x, 2, 3, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sz_x), TRUE);

  spin_grid_sz_y_adj = gtk_adjustment_new (1, 1, 1000, 1, 10, 10);
  spin_grid_sz_y = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sz_y_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sz_y, "spin_grid_sz_y");
  gtk_widget_ref (spin_grid_sz_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sz_y", spin_grid_sz_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sz_y);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sz_y, 2, 3, 1, 2,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sz_y), TRUE);

  spin_grid_sz_z_adj = gtk_adjustment_new (1, 1, 1000, 1, 10, 10);
  spin_grid_sz_z = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sz_z_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sz_z, "spin_grid_sz_z");
  gtk_widget_ref (spin_grid_sz_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sz_z", spin_grid_sz_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sz_z);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sz_z, 2, 3, 2, 3,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sz_z), TRUE);

  spin_grid_sp_x_adj = gtk_adjustment_new (1, 1, 100, 1, 10, 10);
  spin_grid_sp_x = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sp_x_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sp_x, "spin_grid_sp_x");
  gtk_widget_ref (spin_grid_sp_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sp_x", spin_grid_sp_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sp_x);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sp_x, 4, 5, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sp_x), TRUE);

  spin_grid_sp_y_adj = gtk_adjustment_new (1, 1, 100, 1, 10, 10);
  spin_grid_sp_y = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sp_y_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sp_y, "spin_grid_sp_y");
  gtk_widget_ref (spin_grid_sp_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sp_y", spin_grid_sp_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sp_y);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sp_y, 4, 5, 1, 2,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sp_y), TRUE);

  spin_grid_sp_z_adj = gtk_adjustment_new (1, 1, 100, 1, 10, 10);
  spin_grid_sp_z = gtk_spin_button_new (GTK_ADJUSTMENT (spin_grid_sp_z_adj), 1, 0);
  gtk_widget_set_name (spin_grid_sp_z, "spin_grid_sp_z");
  gtk_widget_ref (spin_grid_sp_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "spin_grid_sp_z", spin_grid_sp_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_grid_sp_z);
  gtk_table_attach (GTK_TABLE (table_grid), spin_grid_sp_z, 4, 5, 2, 3,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_grid_sp_z), TRUE);

  label_color_grid_x = gtk_label_new (_("couleur"));
  gtk_widget_set_name (label_color_grid_x, "label_color_grid_x");
  gtk_widget_ref (label_color_grid_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_color_grid_x", label_color_grid_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_color_grid_x);
  gtk_table_attach (GTK_TABLE (table_grid), label_color_grid_x, 5, 6, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_color_grid_x), GTK_JUSTIFY_RIGHT);

  label_color_grid_y = gtk_label_new (_("couleur"));
  gtk_widget_set_name (label_color_grid_y, "label_color_grid_y");
  gtk_widget_ref (label_color_grid_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_color_grid_y", label_color_grid_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_color_grid_y);
  gtk_table_attach (GTK_TABLE (table_grid), label_color_grid_y, 5, 6, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_color_grid_y), GTK_JUSTIFY_RIGHT);

  label_color_grid_z = gtk_label_new (_("couleur"));
  gtk_widget_set_name (label_color_grid_z, "label_color_grid_z");
  gtk_widget_ref (label_color_grid_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "label_color_grid_z", label_color_grid_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label_color_grid_z);
  gtk_table_attach (GTK_TABLE (table_grid), label_color_grid_z, 5, 6, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (label_color_grid_z), GTK_JUSTIFY_RIGHT);

  frame_color_grid_x = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_grid_x, "frame_color_grid_x");
  gtk_widget_ref (frame_color_grid_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_grid_x", frame_color_grid_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_grid_x);
  gtk_table_attach (GTK_TABLE (table_grid), frame_color_grid_x, 6, 7, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_grid_x), GTK_SHADOW_OUT);

  draw_color_grid_x = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_grid_x, "draw_color_grid_x");
  gtk_widget_ref (draw_color_grid_x);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_grid_x", draw_color_grid_x,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_grid_x);
  gtk_container_add (GTK_CONTAINER (frame_color_grid_x), draw_color_grid_x);
  gtk_tooltips_set_tip (tooltips, draw_color_grid_x, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_grid_x, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  frame_color_grid_y = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_grid_y, "frame_color_grid_y");
  gtk_widget_ref (frame_color_grid_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_grid_y", frame_color_grid_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_grid_y);
  gtk_table_attach (GTK_TABLE (table_grid), frame_color_grid_y, 6, 7, 1, 2,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_grid_y), GTK_SHADOW_OUT);

  draw_color_grid_y = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_grid_y, "draw_color_grid_y");
  gtk_widget_ref (draw_color_grid_y);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_grid_y", draw_color_grid_y,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_grid_y);
  gtk_container_add (GTK_CONTAINER (frame_color_grid_y), draw_color_grid_y);
  gtk_tooltips_set_tip (tooltips, draw_color_grid_y, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_grid_y, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  frame_color_grid_z = gtk_frame_new (NULL);
  gtk_widget_set_name (frame_color_grid_z, "frame_color_grid_z");
  gtk_widget_ref (frame_color_grid_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "frame_color_grid_z", frame_color_grid_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame_color_grid_z);
  gtk_table_attach (GTK_TABLE (table_grid), frame_color_grid_z, 6, 7, 2, 3,(GtkAttachOptions) (GTK_FILL),(GtkAttachOptions) (GTK_FILL), 0, 0);
  gtk_frame_set_shadow_type (GTK_FRAME (frame_color_grid_z), GTK_SHADOW_OUT);

  draw_color_grid_z = gtk_drawing_area_new ();
  gtk_widget_set_name (draw_color_grid_z, "draw_color_grid_z");
  gtk_widget_ref (draw_color_grid_z);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "draw_color_grid_z", draw_color_grid_z,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (draw_color_grid_z);
  gtk_container_add (GTK_CONTAINER (frame_color_grid_z), draw_color_grid_z);
  gtk_tooltips_set_tip (tooltips, draw_color_grid_z, _("Choisissez une couleur..."), NULL);
  gtk_widget_set_events (draw_color_grid_z, GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK);

  table1 = gtk_table_new (1, 3, TRUE);
  gtk_widget_set_name (table1, "table1");
  gtk_widget_ref (table1);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "table1", table1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (table1);
  gtk_box_pack_start (GTK_BOX (vbox1), table1, TRUE, TRUE, 0);

  ok_opt = gtk_button_new_with_label (_("Valider"));
  gtk_widget_set_name (ok_opt, "ok_opt");
  gtk_widget_ref (ok_opt);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "ok_opt", ok_opt,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (ok_opt);
  gtk_table_attach (GTK_TABLE (table1), ok_opt, 0, 1, 0, 1,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (ok_opt), 5);
  gtk_tooltips_set_tip (tooltips, ok_opt, _("Valide les options choisies"), NULL);

  cancel_opt = gtk_button_new_with_label (_("Annuler"));
  gtk_widget_set_name (cancel_opt, "cancel_opt");
  gtk_widget_ref (cancel_opt);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "cancel_opt", cancel_opt,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cancel_opt);
  gtk_table_attach (GTK_TABLE (table1), cancel_opt, 1, 2, 0, 1,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (cancel_opt), 5);
  gtk_tooltips_set_tip (tooltips, cancel_opt, _("Annule les choix"), NULL);

  default_opt = gtk_button_new_with_label (_("défaut"));
  gtk_widget_set_name (default_opt, "default_opt");
  gtk_widget_ref (default_opt);
  gtk_object_set_data_full (GTK_OBJECT (optprojet), "default_opt", default_opt,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (default_opt);
  gtk_table_attach (GTK_TABLE (table1), default_opt, 2, 3, 0, 1,(GtkAttachOptions) (GTK_EXPAND),(GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (default_opt), 5);
  gtk_tooltips_set_tip (tooltips, default_opt, _("Réinitialise avec les valeurs par défaut"), NULL);

  // Connexion des signaux
  gtk_signal_connect (GTK_OBJECT (optprojet),"delete_event",GTK_SIGNAL_FUNC(on_cancel_opt_clicked), NULL);
  gtk_signal_connect (GTK_OBJECT (optprojet), "destroy_event",GTK_SIGNAL_FUNC (on_cancel_opt_clicked),NULL);
  gtk_signal_connect (GTK_OBJECT (check_axe_z), "toggled",GTK_SIGNAL_FUNC (on_check_axe_z_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (check_axe_y), "toggled",GTK_SIGNAL_FUNC (on_check_axe_y_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (check_axe_x), "toggled",GTK_SIGNAL_FUNC (on_check_axe_x_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_x), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_axe_x_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_x), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) AxeXcol);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_y), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_axe_y_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_y), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) AxeYcol);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_z), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_axe_z_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_axe_z), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) AxeZcol);
  gtk_signal_connect (GTK_OBJECT (check_grid_x), "toggled",GTK_SIGNAL_FUNC (on_check_grid_x_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (check_grid_y), "toggled",GTK_SIGNAL_FUNC (on_check_grid_y_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (check_grid_z), "toggled",GTK_SIGNAL_FUNC (on_check_grid_z_toggled),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sz_x), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sz_x_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sz_y), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sz_y_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sz_z), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sz_z_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sp_x), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sp_x_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sp_y), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sp_y_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_grid_sp_z), "changed",GTK_SIGNAL_FUNC (on_spin_grid_sp_z_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_x), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_grid_x_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_x), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) GridXcol);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_y), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_grid_y_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_y), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) GridYcol);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_z), "button_press_event",GTK_SIGNAL_FUNC (on_draw_color_grid_z_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (draw_color_grid_z), "expose_event",GTK_SIGNAL_FUNC (on_draw_color_expose_event),(gpointer) GridZcol);
  gtk_signal_connect (GTK_OBJECT (ok_opt), "clicked",GTK_SIGNAL_FUNC (on_ok_opt_clicked),NULL);
  gtk_signal_connect (GTK_OBJECT (cancel_opt), "clicked",GTK_SIGNAL_FUNC (on_cancel_opt_clicked),NULL);
  gtk_signal_connect (GTK_OBJECT (default_opt), "clicked",GTK_SIGNAL_FUNC (on_default_opt_clicked),NULL);

  gtk_object_set_data (GTK_OBJECT (optprojet), "tooltips", tooltips);

  return optprojet;
}
/*
** ----------------------------------------------------------------------------
** Redessine les pixmaps de couleurs.
*/
static void RedrawColorPixmap(void)
{
  GtkWidget *widget;
  GdkRectangle area;

  widget = lookup_widget(Parametres, "draw_color_axe_x");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
  widget = lookup_widget(Parametres, "draw_color_axe_y");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
  widget = lookup_widget(Parametres, "draw_color_axe_z");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
  widget = lookup_widget(Parametres, "draw_color_grid_x");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
  widget = lookup_widget(Parametres, "draw_color_grid_y");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
  widget = lookup_widget(Parametres, "draw_color_grid_z");
  area.x = 0;
  area.y = 0;
  area.width  = widget->allocation.width;
  area.height = widget->allocation.height;
  gtk_widget_draw(widget,&area);
}
/*
** ----------------------------------------------------------------------------
** Mise à jour des infos de la fenêtre parametres avec les paramètres des
** diverses structures manipulées.
*/
static void UpdateParam(void)
{
  GtkWidget *widget;

  if(Parametres)
  {
    // Les axes
    widget = lookup_widget(Parametres, "check_axe_x");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawAxeX);
    widget = lookup_widget(Parametres, "draw_color_axe_x");
    gtk_widget_set_sensitive (widget, bDrawAxeX);

    widget = lookup_widget(Parametres, "check_axe_y");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawAxeY);
    widget = lookup_widget(Parametres, "draw_color_axe_y");
    gtk_widget_set_sensitive (widget, bDrawAxeY);

    widget = lookup_widget(Parametres, "check_axe_z");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawAxeZ);
    widget = lookup_widget(Parametres, "draw_color_axe_z");
    gtk_widget_set_sensitive (widget, bDrawAxeZ);

    // Les grilles
    widget = lookup_widget(Parametres, "check_grid_x");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawGridX);
    widget = lookup_widget(Parametres, "spin_grid_sz_x");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSzX);
    gtk_widget_set_sensitive (widget, bDrawGridX);
    widget = lookup_widget(Parametres, "spin_grid_sp_x");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSpX);
    gtk_widget_set_sensitive (widget, bDrawGridX);
    widget = lookup_widget(Parametres, "draw_color_grid_x");
    gtk_widget_set_sensitive (widget, bDrawGridX);

    widget = lookup_widget(Parametres, "check_grid_y");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawGridY);
    widget = lookup_widget(Parametres, "spin_grid_sz_y");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSzY);
    gtk_widget_set_sensitive (widget, bDrawGridY);
    widget = lookup_widget(Parametres, "spin_grid_sp_y");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSpY);
    gtk_widget_set_sensitive (widget, bDrawGridY);
    widget = lookup_widget(Parametres, "draw_color_grid_y");
    gtk_widget_set_sensitive (widget, bDrawGridY);

    widget = lookup_widget(Parametres, "check_grid_z");
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(widget),bDrawGridZ);
    widget = lookup_widget(Parametres, "spin_grid_sz_z");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSzZ);
    gtk_widget_set_sensitive (widget, bDrawGridZ);
    widget = lookup_widget(Parametres, "spin_grid_sp_z");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), GridSpZ);
    gtk_widget_set_sensitive (widget, bDrawGridZ);
    widget = lookup_widget(Parametres, "draw_color_grid_z");
    gtk_widget_set_sensitive (widget, bDrawGridZ);
  }
}
/*
** ----------------------------------------------------------------------------
** Gestion des options du projet courant
*/
void OpenProjetOptions(GtkWidget *widget,gpointer data)
{
  if(Parametres == NULL)
  {
    // Sauvegarde les paramètres actuels
    oldbDrawAxeX   = bDrawAxeX;
    oldAxeXcol[0]  = AxeXcol[0];
    oldAxeXcol[1]  = AxeXcol[1];
    oldAxeXcol[2]  = AxeXcol[2];
    oldbDrawAxeY   = bDrawAxeY;
    oldAxeYcol[0]  = AxeYcol[0];
    oldAxeYcol[1]  = AxeYcol[1];
    oldAxeYcol[2]  = AxeYcol[2];
    oldbDrawAxeZ   = bDrawAxeZ;
    oldAxeZcol[0]  = AxeZcol[0];
    oldAxeZcol[1]  = AxeZcol[1];
    oldAxeZcol[2]  = AxeZcol[2];
    oldbDrawGridX  = bDrawGridX;
    oldGridXcol[0] = GridXcol[0];
    oldGridXcol[1] = GridXcol[1];
    oldGridXcol[2] = GridXcol[2];
    oldGridSzX     = GridSzX;
    oldGridSpX     = GridSpX;
    oldbDrawGridY  = bDrawGridY;
    oldGridYcol[0] = GridYcol[0];
    oldGridYcol[1] = GridYcol[1];
    oldGridYcol[2] = GridYcol[2];
    oldGridSzY     = GridSzY;
    oldGridSpY     = GridSpY;
    oldbDrawGridZ  = bDrawGridZ;
    oldGridZcol[0] = GridZcol[0];
    oldGridZcol[1] = GridZcol[1];
    oldGridZcol[2] = GridZcol[2];
    oldGridSzZ     = GridSzZ;
    oldGridSpZ     = GridSpZ;

    Parametres = CreateMainParam();
    UpdateParam();
    gtk_widget_show(Parametres);
  }
}
/*
** ----------------------------------------------------------------------------
*/
