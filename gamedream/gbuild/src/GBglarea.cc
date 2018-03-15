/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 17/03/2000
**
** GBglarea.c
**
** Création de la fenetre openGL
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBglarea.h"
#include "GB3Dtypes.h"
#include "GBglmisc.h"
#include "GBglfont.h"
#include "GBmisc.h"
#include "GBcamera.h"
#include "GBwinmvt.h"
#include "GBworld.h"
#include "GBobjets.h"
#include "GBtrackball.h"
#include "GBstatusbar.h"

/*
** ----------------------------------------------------------------------------
** Variables
*/

/*
  Attribute list for gtkglarea widget. Specifies a
  list of Boolean attributes and enum/integer
  attribute/value pairs. The last attribute must be
  GDK_GL_NONE. See glXChooseVisual manpage for further
  explanation.
*/
int attrlist[] =
{
  GDK_GL_RGBA,
  GDK_GL_RED_SIZE, 1,
  GDK_GL_GREEN_SIZE, 1,
  GDK_GL_BLUE_SIZE, 1,
  GDK_GL_DOUBLEBUFFER,
  GDK_GL_DEPTH_SIZE, 1,
  GDK_GL_NONE
};

GBGLOBAL GDvision   Vision;
GBGLOBAL GDcamera   Camera;
GBGLOBAL GDworld_t  *CurrentWorld;

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static gboolean on_glarea_button_press_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean on_glarea_button_release_event(GtkWidget *,GdkEventButton *,gpointer);
static gboolean on_glarea_motion_notify_event(GtkWidget *,GdkEventMotion *,gpointer);
static void     GetGLInfos(GtkWidget *);

/*
** ----------------------------------------------------------------------------
*/
static gboolean on_glarea_button_press_event(GtkWidget *widget,GdkEventButton *event,gpointer data)
{
  gint i,j,maxselect = 100,nhits = 0;
  GLuint selectlist[100], names,*ptr;
  GDobject_t *obj;

  // Bouton 1 (GAUCHE): sélection d'un objet 3D
  if(event->button == 1)
  {
    SelectMode = TRUE;
    glSelectBuffer(maxselect,selectlist);
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName((GLuint)-1);

    glPushMatrix();
    draw(glarea,(GdkEventExpose *)event);
    glPopMatrix();
    SelectMode = FALSE;
    nhits = glRenderMode(GL_RENDER);

    if (nhits == -1)
      g_print("Select buffer overflow\n");

    if (nhits > 0)
    {
      ptr = selectlist;
      for (i = 0; i < nhits; i++)
      {
	/* pour chaque hit */
	names = *ptr;
	// g_print(" number of names for hit = %d\n", names);
	// on passe le nombre de hits
	ptr++;
	// g_print("  z1 is %u;", *ptr);
	// on passe z1
	ptr++;
	// g_print(" z2 is %u\n", *ptr);
	// on passe z2
	ptr++;
	// g_print("   the name is ");
	for (j = 0; j < (gint) names; j++)
	{
	  /* pour chaque nom */
	  if((int) *ptr >= 0)
	  {
	    obj = SearchObjectID(CurrentWorld,(gint) *ptr);
	    if(obj)
	    {
	      GBPrintf("objet %d sélectionné:%s\n", *ptr,obj->name);
	      GBPrintStatusBar("%s X=%f Y=%f Z=%f",obj->name,obj->cpos.x,obj->cpos.y,obj->cpos.z);
	    }
	  }
	  ptr++;
	}
      }
    }
  }
  // Bouton 2/3 (DROIT): changement de position de la caméra (trackball)
  if(event->button>1)
  {
    Camera.dragging = TRUE;
    Camera.ref_x    = (int) event->x;
    Camera.ref_y    = (int) event->y;
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_glarea_button_release_event(GtkWidget *widget,GdkEventButton *event,gpointer data)
{
  if(event->button>1)
  {
    Camera.dragging = FALSE;
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
*/
static gboolean on_glarea_motion_notify_event(GtkWidget *widget,GdkEventMotion *event,gpointer data)
{
  GLint x, y;
  GdkModifierType state;

  if (event->is_hint)
  {
    gdk_window_get_pointer (event->window, &x, &y, &state);
  }
  else
  {
    x = (GLint) event->x;
    y = (GLint) event->y;
    state = (GdkModifierType) event->state;
  }

  // Bouton 2/3 (DROIT): changement de position de la caméra (trackball)
  if((state & GDK_BUTTON2_MASK) || (state & GDK_BUTTON3_MASK))
  {
    if(Camera.dragging)
    {
      trackball(&Camera.last[0],(2.0*Camera.ref_x-Vision.width)/Vision.width,(Vision.height-2.0*Camera.ref_y)/Vision.height,(2.0*x-Vision.width)/Vision.width,(Vision.height-2.0*y)/Vision.height);
      Camera.ref_x=x;
      Camera.ref_y=y;
      add_quats(&Camera.last[0],&Camera.current[0],&Camera.current[0]);
      draw(glarea, NULL);
    }
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
*/
static void GetGLInfos(GtkWidget *widget)
{
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
  {
    gl_vendor = glGetString(GL_VENDOR);
    GBPrintf("GL_VENDOR: %s\n",gl_vendor);
    gl_renderer = glGetString(GL_RENDERER);
    GBPrintf("GL_RENDERER: %s\n",gl_renderer);
    gl_version = glGetString(GL_VERSION);
    GBPrintf("GL_VERSION: %s\n",gl_version);
    gl_extensions = glGetString(GL_EXTENSIONS);
    GBPrintf("GL_EXTENSIONS: %s\n",gl_extensions);
  }
}
/*
** ----------------------------------------------------------------------------
** Initialisation des paramètres d'openGL
*/
gint init(GtkWidget *widget)
{
  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
  {
    GBPrintf("init openGL\n");

    GetGLInfos(widget);
    glEnable(GL_DEPTH_TEST);
    /* dégradé de couleur */
    glShadeModel(GL_FLAT);

    BuildFont();
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
** Redessine la fenêtre openGL après son exposition
*/
gint draw(GtkWidget *widget, GdkEventExpose *event)
{
  float screenaspect;
  GLint viewport[4];
  GdkEventButton *ev;

  /* Draw only last expose. */
  if(! SelectMode) 
    if(event && event->count > 0)
      return TRUE;

  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
  {
    //    GBPrintf("draw openGL\n");

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    if(SelectMode)
    {
      /* Sélection d'un objet sur clic souris */
      ev = (GdkEventButton *) event;
      /* On récupère le viewport courant pour créer une mini-matrice de sélection */
      glGetIntegerv(GL_VIEWPORT,viewport);
      gluPickMatrix(ev->x,viewport[3]-ev->y,3.0,3.0,viewport);
    }
    screenaspect = ((float)Vision.width/(float)Vision.height);
    gluPerspective(Vision.fov,screenaspect,Vision.zNear,Vision.zFar);

    glMatrixMode (GL_MODELVIEW);

    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Définition du point de vue */
    glLoadIdentity ();
    Camera.PlaceCamera();

    /* Trace les axes et les grilles */
    DrawAxes();
    DrawGrids();

    /* Trace le monde courant */
    DrawWorld(CurrentWorld);

    /* Swap backbuffer to front */
    gtk_gl_area_swapbuffers(GTK_GL_AREA(widget));
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
** Redimensionne la fenêtre openGL
*/
gint reshape(GtkWidget *widget, GdkEventConfigure *event)
{
  /* OpenGL functions can be called only if make_current returns true */
  if (gtk_gl_area_make_current(GTK_GL_AREA(widget)))
  {
    //    GBPrintf("reshape openGL\n");

    Vision.width  = widget->allocation.width;
    Vision.height = widget->allocation.height;
    glViewport(0,0, Vision.width, Vision.height);
  }
  return TRUE;
}
/*
** ----------------------------------------------------------------------------
** Création de la fenêtre openGL
*/
GtkWidget *CreateGLarea(GtkWidget **pmainw)
{
  GtkWidget *mainwindow=*pmainw;  
  GtkWidget *glarea;

  glarea = GTK_WIDGET(gtk_gl_area_new(attrlist));
  /* Events for widget must be set before X Window is created */
  gtk_widget_set_events(GTK_WIDGET(glarea),GDK_EXPOSURE_MASK|GDK_BUTTON_PRESS_MASK|GDK_BUTTON_RELEASE_MASK|GDK_POINTER_MOTION_MASK|GDK_POINTER_MOTION_HINT_MASK);

  gtk_widget_set_name (glarea, "glarea");
  gtk_widget_ref (glarea);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "glarea", glarea,(GtkDestroyNotify) gtk_widget_unref);

  gtk_widget_show (glarea);
  gtk_widget_set_usize (glarea,GLWIDTH,GLHEIGHT);
  GTK_WIDGET_UNSET_FLAGS (glarea, GTK_CAN_FOCUS);
  GTK_WIDGET_UNSET_FLAGS (glarea, GTK_CAN_DEFAULT);

  /* Connexion des signaux */
  /* Initialisation */
  gtk_signal_connect(GTK_OBJECT(glarea),"realize",GTK_SIGNAL_FUNC(init),NULL);
  /* Exposition/Affichage */
  gtk_signal_connect(GTK_OBJECT(glarea),"expose_event",GTK_SIGNAL_FUNC(draw),NULL);
  /* Redimensionnement */
  gtk_signal_connect(GTK_OBJECT(glarea),"configure_event",GTK_SIGNAL_FUNC(reshape),NULL);
  /* Les clics et mouvements de la souris */
  gtk_signal_connect (GTK_OBJECT (glarea),"button_press_event",GTK_SIGNAL_FUNC(on_glarea_button_press_event),NULL);
  gtk_signal_connect (GTK_OBJECT (glarea),"button_release_event",GTK_SIGNAL_FUNC(on_glarea_button_release_event),NULL);
  gtk_signal_connect( GTK_OBJECT(glarea),"motion_notify_event",GTK_SIGNAL_FUNC(on_glarea_motion_notify_event),NULL);

  return glarea;
}
/*
** ----------------------------------------------------------------------------
*/
