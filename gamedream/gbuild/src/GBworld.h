/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 25/02/2000
**
** GBworld.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBWORLD_H
#define _GBWORLD_H

/*
** Bloc de paramètres d'un monde 3D
*/
typedef struct
{
  GLenum       GLflags;
} wparam_t;

/*
** Définition d'un monde 3D
*/
typedef struct
{
  gchar        *name;              // nom du monde.
  GLboolean    modified;
  wparam_t     param;              // les paramètres du monde 3D.
  GList        *objects;           // Liste des objets présents
} GDworld_t;

GDworld_t *CreateWorld(void);
void       DeleteWorld(GDworld_t *);
void       OpenWorld(GtkWidget *,gpointer);
void       SaveWorld(GtkWidget *,gpointer);
void       NewWorld(GtkWidget *,gpointer);
void       DrawWorld(GDworld_t *);
void       MakeWorldTest(void);

#endif
/*
** ----------------------------------------------------------------------------
*/
