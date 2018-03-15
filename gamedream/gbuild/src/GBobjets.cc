/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 25/02/2000
**
** GBobjets.c
**
** Gestion des objets 3D
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GB3Dtypes.h"
#include "GBmisc.h"
#include "GBmath.h"
#include "GBworld.h"
#include "GBobjets.h"

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static void Openfile_ok_cb(GtkButton *,gpointer);
static void Savefile_ok_cb(GtkButton *,gpointer);
static void file_cancel_cb(GtkButton *,gpointer);

/*
** ----------------------------------------------------------------------------
** Création d'un objet
** Renvoie un pointeur sur la structure allouée, NULL sinon
*/
GDobject_t *CreateObject(gint type)
{
  GDobject_t *objet;

  objet = (GDobject_t *)g_malloc0(sizeof(GDobject_t));
  if(objet)
  {
    objet->type = type;
    // Echelle 1 par défaut
    GBv3set(&objet->zoom,1.0,1.0,1.0);
  }
  return (objet);
}
/*
** ----------------------------------------------------------------------------
** Destruction d'un objet
*/
void DeleteObject(GDobject_t *obj)
{
  g_free(obj->name);
  g_free(obj->structure);
  g_free(obj);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné le nom donné
*/
void SetObjectName(GDobject_t *obj,gchar *nom)
{
  if(obj)
    obj->name = g_strdup(nom);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné le nom de la structure donné
*/
void SetObjectStructure(GDobject_t *obj,gchar *struc)
{
  if(obj)
    obj->structure = g_strdup(struc);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné le Pickname donné
*/
void SetObjectSelectID(GDobject_t *obj,gint pname)
{
  if(obj)
    obj->Pname = pname;
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné l'echelle donnée
*/
void SetObjectScale(GDobject_t *obj,GLfloat zx,GLfloat zy,GLfloat zz)
{
  GBv3set(&obj->zoom,zx,zy,zz);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné l'echelle donnée (version vectorielle)
*/
void SetObjectScalev(GDobject_t *obj,v3_t *v)
{
  SetObjectScale(obj,v->x,v->y,v->z);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la rotation donnée
*/
void SetObjectRotate(GDobject_t *obj,GLfloat rx,GLfloat ry,GLfloat rz)
{
  GBv3set(&obj->rot,rx,ry,rz);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la rotation donnée (version vectorielle)
*/
void SetObjectRotatev(GDobject_t *obj,v3_t *v)
{
  SetObjectRotate(obj,v->x,v->y,v->z);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la position donnée
*/
void SetObjectPos(GDobject_t *obj,GLfloat px,GLfloat py,GLfloat pz)
{
  if(obj)
    GBv3set(&obj->cpos,px,py,pz);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la position donnée (version vectorielle)
*/
void SetObjectPosv(GDobject_t *obj,v3_t *v)
{
  SetObjectPos(obj,v->x,v->y,v->z);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la couleur donnée
*/
void SetObjectRGBColor(GDobject_t *obj,GLfloat r,GLfloat g,GLfloat b)
{
  if(obj)
  {
    obj->color.r = r;
    obj->color.g = g;
    obj->color.b = b;
  }
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la couleur donnée (version vectorielle)
*/
void SetObjectColorv(GDobject_t *obj,color_t *col)
{
  SetObjectRGBColor(obj,col->r,col->g,col->b);
}
/*
** ----------------------------------------------------------------------------
** Donne à l'objet donné la taille donnée
*/
void SetObjectSize(GDobject_t *obj,GLfloat sz)
{
  if(obj)
    obj->size = sz;
}
/*
** ----------------------------------------------------------------------------
** Cherche l'objet ayant le Pname donné
** Renvoi un pointeur sur l'objet trouvé, NULL sinon.
*/
GDobject_t *SearchObjectID(GDworld_t *world,gint pname)
{
  register GList *obj;

  obj = world->objects;
  while(obj)
  {
    if(((GDobject_t *)obj->data)->Pname == pname)
      return (GDobject_t *)obj->data;
    obj = obj->next;
  }
  return NULL;
}
/*
** ----------------------------------------------------------------------------
** Ouverture d'un objet
*/
static void Openfile_ok_cb(GtkButton *button,gpointer user_data)
{
  gchar *filename;

  filename = g_strdup (gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data)));
  GBPrintf("Objet à importer: %s\n", filename);
  gtk_widget_destroy(GTK_WIDGET(user_data));

  // Ouverture...
}
/*
** ----------------------------------------------------------------------------
** Sauvegarde d'un objet
*/
static void Savefile_ok_cb(GtkButton *button,gpointer user_data)
{
  gchar *filename;

  filename = g_strdup (gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data)));
  GBPrintf("Objet à sauver: %s\n", filename);
  gtk_widget_destroy(GTK_WIDGET(user_data));

  // Sauvegarde...
}
/*
** ----------------------------------------------------------------------------
** Annulation sur ouverture/sauvegarde d'un objet
*/
static void file_cancel_cb(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}
/*
** ----------------------------------------------------------------------------
** Import d'un objet
*/
void OpenObjet(GtkWidget *widget,gpointer data)
{
  GtkWidget *file;

  GBPrintf("Importer un objet\n");
  file = create_fileselection(_("Importer un Objet"),(gpointer)Openfile_ok_cb,(gpointer)file_cancel_cb);
  gtk_widget_show(file);
}
/*
** ----------------------------------------------------------------------------
** Sauvegarde d'un objet
*/
void SaveObjet(GtkWidget *widget,gpointer data)
{
  GtkWidget *file;

  GBPrintf("Sauvegarde d'un objet\n");
  file = create_fileselection(_("Sauver un Objet"),(gpointer)Savefile_ok_cb,(gpointer)file_cancel_cb);
  gtk_widget_show(file);
}
/*
** ----------------------------------------------------------------------------
** Menu Création d'un objet
*/
void ObjetNew(GtkWidget *widget,gpointer data)
{
  GBPrintf("Création d'un objet\n");
}
/*
** ----------------------------------------------------------------------------
** Affiche l'objet donné.
*/
void DrawObjet(GDobject_t *obj)
{
  //  GBPrintf("DrawObjet\n");
  glLoadName(obj->Pname);
  glPushMatrix();
  switch(obj->type)
  {
    case GD_WIRECUBE:
    {
      glTranslatef(obj->cpos.x,obj->cpos.y,obj->cpos.z);
      glRotatef(obj->rot.x,1.0,0.0,0.0);
      glRotatef(obj->rot.y,0.0,1.0,0.0);
      glRotatef(obj->rot.z,0.0,0.0,1.0);
      glScalef(obj->zoom.x,obj->zoom.y,obj->zoom.z);
      glColor3f(obj->color.r,obj->color.g,obj->color.b);
      glutWireCube(obj->size);
      break;
    }
    case GD_SOLIDCUBE:
    {
      glTranslatef(obj->cpos.x,obj->cpos.y,obj->cpos.z);
      glRotatef(obj->rot.x,1.0,0.0,0.0);
      glRotatef(obj->rot.y,0.0,1.0,0.0);
      glRotatef(obj->rot.z,0.0,0.0,1.0);
      glScalef(obj->zoom.x,obj->zoom.y,obj->zoom.z);
      glColor3f(obj->color.r,obj->color.g,obj->color.b);
      glutSolidCube(obj->size);
      break;
    }
  }
  glPopMatrix();
}
/*
** ----------------------------------------------------------------------------
*/
