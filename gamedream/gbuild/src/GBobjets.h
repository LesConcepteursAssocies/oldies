/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 24/02/2000
**
** GBobjets.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBOBJETS_H
#define _GBOBJETS_H

#include "GB3Dtypes.h"

// Les types d'objets
#define   GD_WIRECUBE              1
#define   GD_SOLIDCUBE             2

/*
** Définition du type objet
*/
typedef struct
{
  gint         type;               // type de l'objet (caméra, lumière, complex, ...)
  gchar        *name;              // nom de l'objet
  gchar        *structure;         // nom de la structure de l'objet ("cube", "sphere", "unknow", ...)
  gint         Pname;              // nom interne utilisé pour la sélection (picking)
  v3_t         zoom;               // facteur d'echelle
  v3_t         rot;                // orientation
  v3_t         cpos;               // coord. du centre de la "Bounding Box" de l'objet
  color_t      color;
  GLfloat      size;
} GDobject_t;

GDobject_t *CreateObject(gint);
void       DeleteObject(GDobject_t *);
void       SetObjectName(GDobject_t *,gchar *);
void       SetObjectStructure(GDobject_t *,gchar *);
void       SetObjectSelectID(GDobject_t *,gint);
void       SetObjectScale(GDobject_t *,GLfloat,GLfloat,GLfloat);
void       SetObjectScalev(GDobject_t *,v3_t *);
void       SetObjectRotate(GDobject_t *,GLfloat,GLfloat,GLfloat);
void       SetObjectRotatev(GDobject_t *,v3_t *);
void       SetObjectPos(GDobject_t *,GLfloat,GLfloat,GLfloat);
void       SetObjectPosv(GDobject_t *,v3_t *);
void       SetObjectRGBColor(GDobject_t *,GLfloat,GLfloat,GLfloat);
void       SetObjectColorv(GDobject_t *,color_t *);
void       SetObjectSize(GDobject_t *,GLfloat);
GDobject_t *SearchObjectID(GDworld_t *,gint);
void       OpenObjet(GtkWidget *,gpointer);
void       SaveObjet(GtkWidget *,gpointer);
void       ObjetNew(GtkWidget *,gpointer);
void       DrawObjet(GDobject_t *);

#endif
/*
** ----------------------------------------------------------------------------
*/
