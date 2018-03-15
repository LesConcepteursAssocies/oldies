/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commenc� le          : 01/04/1999
** derni�re modification: 18/02/2000
**
** GB3Dtypes.h
** ----------------------------------------------------------------------------
*/
#ifndef _GB3DTYPES_H
#define _GB3DTYPES_H

/*
** D�finition d'un vecteur 3D
*/
typedef struct
{
  GLfloat x,y,z;
} v3_t;

/*
** Le type couleur
*/
typedef struct
{
  GLfloat r,g,b,a;
} color_t;

/*
** Le type Matrice 4x4
*/
typedef struct
{
  GLfloat e[16];
} matrix44_t;

#endif
/*
** ----------------------------------------------------------------------------
*/
