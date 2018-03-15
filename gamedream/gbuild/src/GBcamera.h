/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commenc� le          : 01/04/1999
** derni�re modification: 15/03/2000
**
** GBcamera.h
**
** Inspir� des travaux de Ferretti Fabio
** http://ferretti.homepage.com
** et Quentin Merigot
** ----------------------------------------------------------------------------
*/
#ifndef _GBCAMERA_H
#define _GBCAMERA_H

#include "GB3Dtypes.h"

/*
** D�finition d'une cam�ra
** type : 0= cam�ra totalement libre en position et direction (d�faut)
** type : 1= la cam�ra est plac�e en un point, et en suit un autre
** type : 2= la cam�ra peut tourner autour d'un point
*/
class   GDcamera
{
  public:

  gint       type;                 // type de la cam�ra
  v3_t       pos;                  // position de la cam�ra
  v3_t       ref;                  // position du point de r�f�rence
  v3_t       ang;                  // orientation de la cam�ra (3 axes)

  GLint      dragging;             // TRUE si trackball actif
  GLfloat    Rot[4][4];
  GLfloat    current[4],
             last[4];
  GLint      ref_x,
             ref_y;

  // Constructeurs.
  GDcamera();
  GDcamera(int,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat,GLfloat);
  ~GDcamera();

  void PlaceCameraFree(void);
  void PlaceCameraFollow(void);
  void PlaceCameraRound(void);
  void PlaceCamera(void);
};

#endif
/*
** ----------------------------------------------------------------------------
*/
