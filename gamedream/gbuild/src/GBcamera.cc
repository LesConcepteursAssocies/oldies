/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 15/03/2000
**
** GBcamera.cc
**
** Gestion de la caméra principale.
** Inspiré des travaux de Ferretti Fabio
** http://ferretti.homepage.com
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GB3Dtypes.h"
#include "GBmisc.h"
#include "GBmath.h"
#include "GBtrackball.h"
#include "GBcamera.h"

/*
** ----------------------------------------------------------------------------
** Variables
*/

/* Caméra principale */
GDcamera Camera;

/*
** ----------------------------------------------------------------------------
** Création d'une caméra positionnée par défaut.
*/
GDcamera::GDcamera()
{
  type     = GD_DEFAULT_CAM_TYPE;
  pos.x    = GD_DEFAULT_CAM_PX;
  pos.y    = GD_DEFAULT_CAM_PY;
  pos.z    = GD_DEFAULT_CAM_PZ;
  ref.x    = GD_DEFAULT_CAM_RX;
  ref.y    = GD_DEFAULT_CAM_RY;
  ref.z    = GD_DEFAULT_CAM_RZ;
  ang.x    = GD_DEFAULT_CAM_AX;
  ang.y    = GD_DEFAULT_CAM_AY;
  ang.z    = GD_DEFAULT_CAM_AZ;
  dragging = FALSE;
  // Initialise le trackball
  trackball(&current[0],0.0,0.0,0.0,0.0);
}
/*
** ----------------------------------------------------------------------------
** Création d'une caméra.
*/
GDcamera::GDcamera(int typ,GLfloat px,GLfloat py,GLfloat pz,GLfloat rx,GLfloat ry,GLfloat rz,GLfloat ax,GLfloat ay,GLfloat az)
{
  type     = typ;
  pos.x    = px;
  pos.y    = py;
  pos.z    = pz;
  ref.x    = rx;
  ref.y    = ry;
  ref.z    = rz;
  ang.x    = ax;
  ang.y    = ay;
  ang.z    = az;
  dragging = FALSE;
  // Initialise le trackball
  trackball(&current[0],0.0,0.0,0.0,0.0);
}
/*
** ----------------------------------------------------------------------------
** Destructeur
*/
GDcamera::~GDcamera()
{
}
/*
** ----------------------------------------------------------------------------
** Placement d'une caméra de type 0 (libre)
*/
void GDcamera::PlaceCameraFree(void)
{
#if 1
  glTranslatef(-pos.x,-pos.y,-pos.z);
  build_rotmatrix(Rot,current);
  glMultMatrixf(&Rot[0][0]);
#else
  gluLookAt(pos.x,pos.y,pos.z,ref.x,ref.y,ref.z,0.0,1.0,0.0);
#endif
}
/*
** ----------------------------------------------------------------------------
** Placement d'une caméra de type 1 (placée en un point et en suit un autre)
*/
void GDcamera::PlaceCameraFollow(void)
{
  v3_t d;
  GLfloat r,tmp;

  d.x=(pos.x-ref.x);
  d.y=(pos.y-ref.y);
  d.z=(pos.z-ref.z);
  tmp = (d.x*d.x)+(d.z*d.z);
  r   = sqrt(tmp+(d.y*d.y));
  tmp = sqrt(tmp);
	
  if(tmp!=0) 
  {
    // teta
    // camera->ang.y = asin(-d.x/tmp)*RAD_TO_DEG;
    ang.y = RADIAN(asin(-d.x/tmp));
    if(d.z<0)
      ang.y = -ang.y;
  }
  else
    ang.y = 0.0;

  if(r!=0)
  {
    // phi
    // camera->ang.x = asin(d.y/r)*RAD_TO_DEG;
    ang.x = RADIAN(asin(d.y/r));
    if(d.z<0)
      ang.x = 180-ang.x;
  }
  else
    ang.y = 0.0;

  if(d.z<0)
    glRotatef(180.0,0.0,0.0,1.0);
  glRotatef(ang.x,1.0,0.0,0.0);
  glRotatef(ang.y,0.0,1.0,0.0);
  glTranslatef(-pos.x,-pos.y,-pos.z);
  GBPrintf("follow px=%f py=%f pz=%f rx=%f ry=%f rz=%f ax=%f ay=%f az=%f\n",pos.x,pos.y,pos.z,ref.x,ref.y,ref.z,ang.x,ang.y,ang.z);
}
/*
** ----------------------------------------------------------------------------
** Placement d'une caméra de type 2 (placée en un point à une distance z d'un
** point de référence et orientée arbitrairement)
*/
void GDcamera::PlaceCameraRound(void)
{
  if(pos.z>0)
    glTranslatef(0.0,0.0,-pos.z);
  else
    glTranslatef(0.0,0.0,pos.z);
  glRotatef(ang.x,1.0,0.0,0.0);
  glRotatef(ang.y,0.0,1.0,0.0);
  glRotatef(ang.z,0.0,0.0,1.0);
  glTranslatef(-ref.x,-ref.y,-ref.z);
}
/*
** ----------------------------------------------------------------------------
** Place la caméra donnée en fonction de son type
*/
void GDcamera::PlaceCamera(void)
{
  switch(type)
  {
    case 0: { PlaceCameraFree()  ; break; }
    case 1: { PlaceCameraFollow(); break; }
    case 2: { PlaceCameraRound() ; break; }
  }
}
/*
** ----------------------------------------------------------------------------
*/
