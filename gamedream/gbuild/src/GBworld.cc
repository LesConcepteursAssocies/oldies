/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 25/02/2000
**
** GBworld.c
**
** Gestion d'un monde 3D
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
** Variables
*/

/* Mode 3D principal */
GDworld_t *CurrentWorld;

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static void Openfile_ok_cb(GtkButton *,gpointer);
static void Savefile_ok_cb(GtkButton *,gpointer);
static void file_cancel_cb(GtkButton *,gpointer);

/*
** ----------------------------------------------------------------------------
** Création d'un monde
** Renvoie un pointeur sur la structure allouée, NULL sinon
*/
GDworld_t *CreateWorld(void)
{
  GDworld_t *world;

  world = (GDworld_t *)g_malloc0(sizeof(GDworld_t));

  return (world);
}
/*
** ----------------------------------------------------------------------------
** Destruction du monde donné
*/
void DeleteWorld(GDworld_t *world)
{
  register GList *obj;

  obj = world->objects;
  while(obj)
  {
    DeleteObject((GDobject_t *)obj->data);
    obj = obj->next;
  }
}
/*
** ----------------------------------------------------------------------------
** Ouverture d'un projet
*/
static void Openfile_ok_cb(GtkButton *button,gpointer user_data)
{
  gchar *filename;

  filename = g_strdup (gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data)));
  GBPrintf("Fichier à ouvrir: %s\n", filename);
  gtk_widget_destroy(GTK_WIDGET(user_data));

  // Ouverture...
}
/*
** ----------------------------------------------------------------------------
** Sauvegarde d'un projet
*/
static void Savefile_ok_cb(GtkButton *button,gpointer user_data)
{
  gchar *filename;

  filename = g_strdup (gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data)));
  GBPrintf("Fichier à sauver: %s\n", filename);
  gtk_widget_destroy(GTK_WIDGET(user_data));

  // Sauvegarde...
}
/*
** ----------------------------------------------------------------------------
** Annulation sur ouverture/sauvegarde d'un projet
*/
static void file_cancel_cb(GtkButton *button,gpointer user_data)
{
  gtk_widget_destroy(GTK_WIDGET(user_data));
}
/*
** ----------------------------------------------------------------------------
** Ouverture d'un projet
*/
void OpenWorld(GtkWidget *widget,gpointer data)
{
  GtkWidget *file;

  GBPrintf("Ouverture d'un projet\n");
  file = create_fileselection(_("Ouvrir un Projet"),(gpointer)Openfile_ok_cb,(gpointer)file_cancel_cb);
  gtk_widget_show(file);
}
/*
** ----------------------------------------------------------------------------
** Sauvegarde d'un projet
*/
void SaveWorld(GtkWidget *widget,gpointer data)
{
  GtkWidget *file;

  GBPrintf("Sauvegarde d'un projet\n");
  file = create_fileselection(_("Sauver un Projet"),(gpointer)Savefile_ok_cb,(gpointer)file_cancel_cb);
  gtk_widget_show(file);
}
/*
** ----------------------------------------------------------------------------
** Création d'un projet
*/
void NewWorld(GtkWidget *widget,gpointer data)
{
  GBPrintf("Création d'un projet\n");
}
/*
** ----------------------------------------------------------------------------
** Affichage du monde 3D donné
*/
void DrawWorld(GDworld_t *world)
{
  register GList *obj;

  //  GBPrintf("DrawWorld\n");

#if 0
  /* Test font... */
  glColor3f(1,1,1);
  glRasterPos2f(0.0f,0.0f);
  glPrint("hello");

  glRasterPos2f(0.0f,0.5f);
  glPrint("coucou");

  glRasterPos2f(0.0f,-1);
  glPrint("bonjour");

  glRasterPos2f(0.0f,-2);
  glPrint("y=-2");

  glRasterPos2f(0.0f,-3);
  glPrint("y=-3");

  glRasterPos2f(0.0f,-4);
  glPrint("y=-4");

  glRasterPos2f(0.0f,-5);
  glPrint("y=-5");

  glRasterPos2f(0.0f,-6);
  glPrint("y=-6");

  glRasterPos2f(0.0f,-7);
  glPrint("y=-7");
#endif

  obj = world->objects;
  while(obj)
  {
    DrawObjet((GDobject_t *)obj->data);
    obj = obj->next;
  }
}
/*
** ----------------------------------------------------------------------------
** Pour tester
*/
void MakeWorldTest(void)
{
  GDobject_t *obj;

  CurrentWorld = CreateWorld();
  if(CurrentWorld)
  {
    /* Un cube blanc en 0,0,0 de taille 1 */
    obj = CreateObject(GD_WIRECUBE);
    if(obj)
    {
      SetObjectName(obj,"Cube01");
      SetObjectStructure(obj,"CUBE");
      SetObjectSelectID(obj,pickname);
      pickname++;
      SetObjectPos(obj,0.0,0.0,0.0);
      SetObjectRGBColor(obj,1.0,1.0,1.0);
      SetObjectSize(obj,1.0);
      CurrentWorld->objects = g_list_append(CurrentWorld->objects,obj);
    }

    /* Un cube bleu en 5,0,0 de taille 1, tourné de 45° en Y */
    obj = CreateObject(GD_WIRECUBE);
    if(obj)
    {
      SetObjectName(obj,"Cube02");
      SetObjectStructure(obj,"CUBE");
      SetObjectSelectID(obj,pickname);
      pickname++;
      SetObjectPos(obj,5.0,0.0,0.0);
      SetObjectRGBColor(obj,0.0,0.0,1.0);
      SetObjectSize(obj,1.0);
      SetObjectRotate(obj,0.0,45.0,0.0);
      CurrentWorld->objects = g_list_append(CurrentWorld->objects,obj);
    }

    /* Un cube vert plein en 5,5,0 de taille 1, échelle 2 */
    obj = CreateObject(GD_SOLIDCUBE);
    if(obj)
    {
      SetObjectName(obj,"Cube03");
      SetObjectStructure(obj,"CUBE");
      SetObjectSelectID(obj,pickname);
      pickname++;
      SetObjectPos(obj,5.0,5.0,0.0);
      SetObjectRGBColor(obj,0.0,1.0,0.0);
      SetObjectSize(obj,1.0);
      SetObjectScale(obj,2.0,2.0,2.0);
      CurrentWorld->objects = g_list_append(CurrentWorld->objects,obj);
    }
  }
}
/*
** ----------------------------------------------------------------------------
*/
