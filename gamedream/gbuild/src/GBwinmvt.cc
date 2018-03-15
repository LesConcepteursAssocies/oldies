/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 17/03/2000
**
** GBwinmvt.c
**
** Création de la fenetre de gestion des mouvements (caméra, oeil, scène)
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBwinmvt.h"
#include "GBglarea.h"
#include "GBglmisc.h"
#include "GBmisc.h"
#include "GBcamera.h"

/*
** ----------------------------------------------------------------------------
** Variables
*/

/* Cone de vision principal */
GDvision            Vision;
static GtkWidget    *Mouvements=NULL;

GBGLOBAL GDcamera   Camera;

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static void      on_spin_fov_changed(GtkSpinButton *,gpointer);
static void      on_spin_near_changed(GtkSpinButton *,gpointer);
static void      on_spin_far_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_px_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_py_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_pz_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_rx_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_ry_changed(GtkSpinButton *,gpointer);
static void      on_spin_cam_rz_changed(GtkSpinButton *,gpointer);
static void      on_cam_button_close_clicked(GtkButton *,gpointer);
static void      on_cam_button_raz_clicked(GtkButton *,gpointer);

static GtkWidget *CreateMainMvt(void);
static void       UpdateMvt(void);

/*
** ----------------------------------------------------------------------------
*/
static void on_spin_fov_changed(GtkSpinButton *spin,gpointer user_data)
{
  Vision.fov = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_near_changed(GtkSpinButton *spin,gpointer user_data)
{
  Vision.zNear = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_far_changed(GtkSpinButton *spin,gpointer user_data)
{
  Vision.zFar = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_px_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.pos.x = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_py_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.pos.y = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_pz_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.pos.z = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_rx_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.ref.x = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_ry_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.ref.y = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_spin_cam_rz_changed(GtkSpinButton *spin,gpointer user_data)
{
  Camera.ref.z = gtk_spin_button_get_value_as_float(spin);
  reshape(glarea, NULL);
  draw(glarea, NULL);
}
/*
** ----------------------------------------------------------------------------
*/
static void on_cam_button_close_clicked(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(Mouvements);
  Mouvements=NULL;
}
/*
** ----------------------------------------------------------------------------
*/
static void on_cam_button_raz_clicked(GtkButton *button,gpointer user_data)
{
  Vision.fov   = GD_DEFAULT_FOV;
  Vision.zNear = GD_DEFAULT_ZNEAR;
  Vision.zFar  = GD_DEFAULT_ZFAR;

  Camera.pos.x = GD_DEFAULT_CAM_PX;
  Camera.pos.y = GD_DEFAULT_CAM_PY;
  Camera.pos.z = GD_DEFAULT_CAM_PZ;
  Camera.ref.x = GD_DEFAULT_CAM_RX;
  Camera.ref.y = GD_DEFAULT_CAM_RY;
  Camera.ref.z = GD_DEFAULT_CAM_RZ;

  reshape(glarea, NULL);
  draw(glarea, NULL);
  UpdateMvt();
}
/*
** ----------------------------------------------------------------------------
** Création d'un point de vue par défaut.
*/
GDvision::GDvision()
{
  fov    = GD_DEFAULT_FOV;
  zNear  = GD_DEFAULT_ZNEAR;
  zFar   = GD_DEFAULT_ZFAR;
  width  = GLWIDTH;
  height = GLHEIGHT;
}
/*
** ----------------------------------------------------------------------------
** Création d'un point de vue.
** Renvoi un pointeur sur la structure créée, NULL sinon.
*/
GDvision::GDvision(GLdouble fov,GLdouble near,GLdouble far,GLuint w,GLuint h)
{
  fov    = fov;
  zNear  = near;
  zFar   = far;
  width  = w;
  height = h;
}
/*
** ----------------------------------------------------------------------------
** Destructeur
*/
GDvision::~GDvision()
{
}
/*
** ----------------------------------------------------------------------------
** Création de la fenêtre principale de gestion des mouvements
** Renvoi un pointeur sur la fenêtre créée.
*/
static GtkWidget *CreateMainMvt(void)
{
  GtkWidget *mouvement;
  GtkWidget *notebook1;
  GtkWidget *cam_tab;
  GtkWidget *cam_fov;
  GtkObject *spin_fov_adj;
  GtkWidget *spin_fov;
  GtkWidget *cam_near;
  GtkObject *spin_near_adj;
  GtkWidget *spin_near;
  GtkWidget *cam_far;
  GtkObject *spin_far_adj;
  GtkWidget *spin_far;
  GtkWidget *cam_pos;
  GtkObject *spin_cam_px_adj;
  GtkWidget *spin_cam_px;
  GtkObject *spin_cam_py_adj;
  GtkWidget *spin_cam_py;
  GtkObject *spin_cam_pz_adj;
  GtkWidget *spin_cam_pz;
  GtkWidget *cam_ref;
  GtkObject *spin_cam_rx_adj;
  GtkWidget *spin_cam_rx;
  GtkObject *spin_cam_ry_adj;
  GtkWidget *spin_cam_ry;
  GtkObject *spin_cam_rz_adj;
  GtkWidget *spin_cam_rz;
  GtkWidget *cam_button_close;
  GtkWidget *cam_button_raz;
  GtkWidget *cam_page;
  GtkWidget *empty_notebook_page;
  GtkWidget *oeil;
  GtkWidget *scene;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  mouvement = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (mouvement, "mouvement");
  gtk_object_set_data (GTK_OBJECT (mouvement), "mouvement", mouvement);
  gtk_window_set_title (GTK_WINDOW (mouvement), _("Mouvements"));

  notebook1 = gtk_notebook_new ();
  gtk_widget_set_name (notebook1, "notebook1");
  gtk_widget_ref (notebook1);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "notebook1", notebook1,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (notebook1);
  gtk_container_add (GTK_CONTAINER (mouvement), notebook1);

  cam_tab = gtk_table_new (6, 4, TRUE);
  gtk_widget_set_name (cam_tab, "cam_tab");
  gtk_widget_ref (cam_tab);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_tab", cam_tab,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_tab);
  gtk_container_add (GTK_CONTAINER (notebook1), cam_tab);

  cam_fov = gtk_label_new (_("champ de vision"));
  gtk_widget_set_name (cam_fov, "cam_fov");
  gtk_widget_ref (cam_fov);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_fov", cam_fov,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_fov);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_fov, 0, 1, 0, 1,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (cam_fov),GTK_JUSTIFY_LEFT);

  spin_fov_adj = gtk_adjustment_new (1, 0, 180, 1, 10, 10);
  spin_fov = gtk_spin_button_new (GTK_ADJUSTMENT (spin_fov_adj), 1, 0);
  gtk_widget_set_name (spin_fov, "spin_fov");
  gtk_widget_ref (spin_fov);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_fov", spin_fov,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_fov);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_fov, 2, 3, 0, 1,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_fov, _("angle du champ de vision"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_fov), TRUE);

  cam_near = gtk_label_new (_("near"));
  gtk_widget_set_name (cam_near, "cam_near");
  gtk_widget_ref (cam_near);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_near", cam_near,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_near);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_near, 0, 1, 1, 2,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (cam_near), GTK_JUSTIFY_LEFT);

  spin_near_adj = gtk_adjustment_new (1, 0, 1000, 1, 10, 10);
  spin_near = gtk_spin_button_new (GTK_ADJUSTMENT (spin_near_adj), 1, 0);
  gtk_widget_set_name (spin_near, "spin_near");
  gtk_widget_ref (spin_near);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_near", spin_near,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_near);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_near, 2, 3, 1, 2,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_near), TRUE);

  cam_far = gtk_label_new (_("far"));
  gtk_widget_set_name (cam_far, "cam_far");
  gtk_widget_ref (cam_far);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_far", cam_far,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_far);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_far, 0, 1, 2, 3,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (cam_far), GTK_JUSTIFY_LEFT);

  spin_far_adj = gtk_adjustment_new (1, 0, 1000, 1, 10, 10);
  spin_far = gtk_spin_button_new (GTK_ADJUSTMENT (spin_far_adj), 1, 0);
  gtk_widget_set_name (spin_far, "spin_far");
  gtk_widget_ref (spin_far);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_far", spin_far,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_far);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_far, 2, 3, 2, 3,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_far), TRUE);

  cam_pos = gtk_label_new (_("position"));
  gtk_widget_set_name (cam_pos, "cam_pos");
  gtk_widget_ref (cam_pos);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_pos", cam_pos,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_pos);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_pos, 0, 1, 3, 4,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);

  spin_cam_px_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_px = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_px_adj), 1, 0);
  gtk_widget_set_name (spin_cam_px, "spin_cam_px");
  gtk_widget_ref (spin_cam_px);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_px", spin_cam_px,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_px);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_px, 1, 2, 3, 4,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_px, _("position en x de la caméra"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_px), TRUE);

  spin_cam_py_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_py = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_py_adj), 1, 0);
  gtk_widget_set_name (spin_cam_py, "spin_cam_py");
  gtk_widget_ref (spin_cam_py);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_py", spin_cam_py,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_py);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_py, 2, 3, 3, 4,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_py, _("position en y de la caméra"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_py), TRUE);

  spin_cam_pz_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_pz = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_pz_adj), 1, 0);
  gtk_widget_set_name (spin_cam_pz, "spin_cam_pz");
  gtk_widget_ref (spin_cam_pz);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_pz", spin_cam_pz,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_pz);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_pz, 3, 4, 3, 4,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_pz, _("position en z de la caméra"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_pz), TRUE);

  cam_ref = gtk_label_new (_("référence"));
  gtk_widget_set_name (cam_ref, "cam_ref");
  gtk_widget_ref (cam_ref);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_ref", cam_ref,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_ref);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_ref, 0, 1, 4, 5,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (cam_ref), GTK_JUSTIFY_LEFT);

  spin_cam_rx_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_rx = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_rx_adj), 1, 0);
  gtk_widget_set_name (spin_cam_rx, "spin_cam_rx");
  gtk_widget_ref (spin_cam_rx);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_rx", spin_cam_rx,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_rx);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_rx, 1, 2, 4, 5,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_rx, _("position en x du point de visée"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_rx), TRUE);

  spin_cam_ry_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_ry = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_ry_adj), 1, 0);
  gtk_widget_set_name (spin_cam_ry, "spin_cam_ry");
  gtk_widget_ref (spin_cam_ry);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_ry", spin_cam_ry,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_ry);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_ry, 2, 3, 4, 5,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_ry, _("position en y du point de visée"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_ry), TRUE);

  spin_cam_rz_adj = gtk_adjustment_new (0, -1000, 1000, 1, 10, 10);
  spin_cam_rz = gtk_spin_button_new (GTK_ADJUSTMENT (spin_cam_rz_adj), 1, 0);
  gtk_widget_set_name (spin_cam_rz, "spin_cam_rz");
  gtk_widget_ref (spin_cam_rz);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "spin_cam_rz", spin_cam_rz,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (spin_cam_rz);
  gtk_table_attach (GTK_TABLE (cam_tab), spin_cam_rz, 3, 4, 4, 5,(GtkAttachOptions) (GTK_EXPAND | GTK_FILL),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, spin_cam_rz, _("position en z du point de visée"), NULL);
  gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spin_cam_rz), TRUE);

  cam_button_close = gtk_button_new_with_label (_("fermer"));
  gtk_widget_set_name (cam_button_close, "cam_button_close");
  gtk_widget_ref (cam_button_close);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_button_close", cam_button_close,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_button_close);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_button_close, 1, 2, 5, 6,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (cam_button_close), 2);
  gtk_tooltips_set_tip (tooltips, cam_button_close, _("ferme la fenêtre"), NULL);

  cam_button_raz = gtk_button_new_with_label (_("défaut"));
  gtk_widget_set_name (cam_button_raz, "cam_button_raz");
  gtk_widget_ref (cam_button_raz);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_button_raz", cam_button_raz,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_button_raz);
  gtk_table_attach (GTK_TABLE (cam_tab), cam_button_raz, 2, 3, 5, 6,(GtkAttachOptions) (0),(GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, cam_button_raz, _("réinitialise avec les valeurs par défaut"), NULL);

  cam_page = gtk_label_new (_("camera"));
  gtk_widget_set_name (cam_page, "cam_page");
  gtk_widget_ref (cam_page);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "cam_page", cam_page,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (cam_page);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), cam_page);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook1), empty_notebook_page);

  oeil = gtk_label_new (_("oeil"));
  gtk_widget_set_name (oeil, "oeil");
  gtk_widget_ref (oeil);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "oeil", oeil,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (oeil);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), oeil);

  empty_notebook_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (empty_notebook_page);
  gtk_container_add (GTK_CONTAINER (notebook1), empty_notebook_page);

  scene = gtk_label_new (_("scene"));
  gtk_widget_set_name (scene, "scene");
  gtk_widget_ref (scene);
  gtk_object_set_data_full (GTK_OBJECT (mouvement), "scene", scene,(GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scene);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 2), scene);

  // Connexion des signaux
  gtk_signal_connect (GTK_OBJECT (mouvement),"delete_event",GTK_SIGNAL_FUNC(on_cam_button_close_clicked), NULL);
  gtk_signal_connect (GTK_OBJECT (mouvement), "destroy_event",GTK_SIGNAL_FUNC (on_cam_button_close_clicked),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_fov), "changed",GTK_SIGNAL_FUNC (on_spin_fov_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_near), "changed",GTK_SIGNAL_FUNC (on_spin_near_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_far), "changed",GTK_SIGNAL_FUNC (on_spin_far_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_px), "changed",GTK_SIGNAL_FUNC (on_spin_cam_px_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_py), "changed",GTK_SIGNAL_FUNC (on_spin_cam_py_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_pz), "changed",GTK_SIGNAL_FUNC (on_spin_cam_pz_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_rx), "changed",GTK_SIGNAL_FUNC (on_spin_cam_rx_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_ry), "changed",GTK_SIGNAL_FUNC (on_spin_cam_ry_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (spin_cam_rz), "changed",GTK_SIGNAL_FUNC (on_spin_cam_rz_changed),NULL);
  gtk_signal_connect (GTK_OBJECT (cam_button_close), "clicked",GTK_SIGNAL_FUNC (on_cam_button_close_clicked),NULL);
  gtk_signal_connect (GTK_OBJECT (cam_button_raz), "clicked",GTK_SIGNAL_FUNC (on_cam_button_raz_clicked),NULL);

  gtk_object_set_data (GTK_OBJECT (mouvement), "tooltips", tooltips);

  return mouvement;
}
/*
** ----------------------------------------------------------------------------
** Mise à jour des infos de la fenêtre mouvement avec les paramètres des
** diverses structures manipulées.
*/
static void UpdateMvt(void)
{
  GtkWidget *widget;

  if(Mouvements)
  {
    // Le cone de vision
    widget = lookup_widget(Mouvements, "spin_fov");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Vision.fov);
    widget = lookup_widget(Mouvements, "spin_near");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Vision.zNear);
    widget = lookup_widget(Mouvements, "spin_far");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Vision.zFar);
    // La caméra
    widget = lookup_widget(Mouvements, "spin_cam_px");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.pos.x);
    widget = lookup_widget(Mouvements, "spin_cam_py");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.pos.y);
    widget = lookup_widget(Mouvements, "spin_cam_pz");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.pos.z);
    widget = lookup_widget(Mouvements, "spin_cam_rx");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.ref.x);
    widget = lookup_widget(Mouvements, "spin_cam_ry");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.ref.y);
    widget = lookup_widget(Mouvements, "spin_cam_rz");
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(widget), Camera.ref.z);
  }
}
/*
** ----------------------------------------------------------------------------
** Gestion des mouvements
*/
void OpenMvt(GtkWidget *widget,gpointer data)
{
  if(Mouvements == NULL)
  {
    Mouvements = CreateMainMvt();
    UpdateMvt();
    gtk_widget_show(Mouvements);
  }
}
/*
** ----------------------------------------------------------------------------
*/
