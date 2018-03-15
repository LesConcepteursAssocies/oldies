/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 15/03/2000
**
** GBcamera.h
**
** Inspiré des travaux de Ferretti Fabio
** http://ferretti.homepage.com
** et Quentin Merigot
** ----------------------------------------------------------------------------
*/
#ifndef _GBCAMERA_H
#define _GBCAMERA_H

#include "GB3Dtypes.h"

/*
** Définition d'une caméra
** type : 0= caméra totalement libre en position et direction (défaut)
** type : 1= la caméra est placée en un point, et en suit un autre
** type : 2= la caméra peut tourner autour d'un point
*/
class   GDcamera
{
  public:

  gint       type;                 // type de la caméra
  v3_t       pos;                  // position de la caméra
  v3_t       ref;                  // position du point de référence
  v3_t       ang;                  // orientation de la caméra (3 axes)

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
