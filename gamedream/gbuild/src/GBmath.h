/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 18/02/2000
**
** GBmath.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBMATH_H
#define _GBMATH_H

#ifndef M_PI
#define        M_PI                3.14159265358979323846
#endif

#define RADIAN(x)                  ((x) * M_PI/180.0f)
#define DEGRE(x)                   ((x) * 180.0f/M_PI)


void    GBv3zero(v3_t *);
void    GBv3set(v3_t *, GLfloat, GLfloat, GLfloat);
GLfloat GBv3length(v3_t);
void    GBv3add(v3_t *, v3_t, v3_t);
void    GBv3sub(v3_t *, v3_t, v3_t);
void    GBv3cross(v3_t *, v3_t, v3_t);
void    GBv3dot(v3_t *, v3_t, v3_t);
void    GBv3scale(v3_t *, v3_t, GLfloat);
void    GBv3normalize(v3_t *, v3_t);

#endif
/*
** ----------------------------------------------------------------------------
*/
