/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commenc� le          : 01/04/1999
** derni�re modification: 18/02/2000
**
** GBmath.cc
**
** Routines math�matiques diverses
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GB3Dtypes.h"
#include "GBmisc.h"
#include "GBmath.h"

/*
** ----------------------------------------------------------------------------
** Met le vecteur donn� � z�ro
*/
void GBv3zero(v3_t *v)
{
  v->x = 0.0;
  v->y = 0.0;
  v->z = 0.0;
}
/*
** ----------------------------------------------------------------------------
** Initialise le vecteur donn� avec les valeurs donn�es.
*/
void GBv3set(v3_t *v, GLfloat x, GLfloat y, GLfloat z)
{
  v->x = x;
  v->y = y;
  v->z = z;
}
/*
** ----------------------------------------------------------------------------
** Renvoie la longueur du vecteur donn�.
*/
GLfloat GBv3length(v3_t v)
{
  return (GLfloat) sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
/*
** ----------------------------------------------------------------------------
** Additionne v1 et v2 et met le r�sultat dans v
*/
void GBv3add(v3_t *v, v3_t v1, v3_t v2)
{
  GBv3set(v,v1.x + v2.x,v1.y + v2.y,v1.z + v2.z);
}
/*
** ----------------------------------------------------------------------------
** Soustrait v1 et v2 et met le r�sultat dans v
*/
void GBv3sub(v3_t *v, v3_t v1, v3_t v2)
{
  GBv3set(v,v1.x - v2.x,v1.y - v2.y,v1.z - v2.z);
}
/*
** ----------------------------------------------------------------------------
*/
void GBv3cross(v3_t *v, v3_t v1, v3_t v2)
{
  GBv3set(v,v1.y * v2.z - v1.z * v2.y,v1.z * v2.x - v1.x * v2.z,v1.x * v2.y - v1.y * v2.x);
}
/*
** ----------------------------------------------------------------------------
*/
void GBv3dot(v3_t *v, v3_t v1, v3_t v2)
{
  GBv3set(v,v1.x * v2.x,v1.y * v2.y,v1.z * v2.z);
}
/*
** ----------------------------------------------------------------------------
** Multiplie le vecteur donn� par le scalaire donn�, et met le r�sultat dans v.
*/
void GBv3scale(v3_t *v, v3_t v1, GLfloat s)
{
  GBv3set(v,v1.x * s,v1.y * s,v1.z * s);
}
/*
** ----------------------------------------------------------------------------
** Normalise le vecteur donn�, et met le r�sultat dans v
*/
void GBv3normalize(v3_t *v, v3_t v1)
{
  GLfloat len=GBv3length(v1);

  if(len != 0.0)
    GBv3scale(v,v1,(1.0/len));
  else
    GBv3set(v,v1.x,v1.y,v1.z);
}
/*
** ----------------------------------------------------------------------------
*/
