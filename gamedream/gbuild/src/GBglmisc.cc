/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 08/03/2000
**
** GBglmisc.c
**
** Routines diverses openGL
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GB3Dtypes.h"
#include "GBmisc.h"
#include "GBmath.h"
#include "GBglmisc.h"

/*
** ----------------------------------------------------------------------------
** Active ou désactive la paramètre openGL donné, en fonction du flag donné.
*/
void GBSetGLParam(gboolean bflag, GLenum param)
{
  if(bflag)
    glEnable(param);
  else
    glDisable(param);
}
/*
** ----------------------------------------------------------------------------
** Trace les axes X Y Z
*/
void DrawAxes(void)
{
  if(bDrawAxeX)
  {
    glBegin(GL_LINES);
    {
      // X rouge
      glColor3fv(AxeXcol);
      glVertex3f(-5,0,0);
      glVertex3f(5,0,0);
    }
    glEnd();
  }
  if(bDrawAxeY)
  {
    glBegin(GL_LINES);
    {
      // Y vert
      glColor3fv(AxeYcol);
      glVertex3f(0,-5,0);
      glVertex3f(0,5,0);
    }
    glEnd();
  }
  if(bDrawAxeZ)
  {
    glBegin(GL_LINES);
    {
      // Z bleu
      glColor3fv(AxeZcol);
      glVertex3f(0,0,-5);
      glVertex3f(0,0,5);
    }
    glEnd();
  }
}
/*
** ----------------------------------------------------------------------------
** Trace les grilles X Y Z
** GridSz est la taille d'un quadrant de la grille concernée
*/
void DrawGrids(void)
{
  int x,y,z;

  if(bDrawGridX)
  {
    glColor3fv(GridXcol);
    glBegin(GL_LINES);
    {
      for (x=0; x <= 2*GridSzX; x+=GridSpX)
      {
	glVertex3f(-GridSzX, 0.0, (GridSzX - x));
	glVertex3f(GridSzX, 0.0, (GridSzX - x));
      }
      for (x=0; x <= 2*GridSzX; x+=GridSpX)
      {
	glVertex3f((GridSzX - x), 0.0, -GridSzX);
	glVertex3f((GridSzX - x), 0.0, GridSzX);
      }
    }
    glEnd();
  }
  if(bDrawGridY)
  {
    glColor3fv(GridYcol);
    glBegin(GL_LINES);
    {
      for (y=0; y <= 2*GridSzY; y+=GridSpY)
      {
	glVertex3f(-GridSzY, (GridSzY - y), 0.0);
	glVertex3f(GridSzY, (GridSzY - y), 0.0);
      }
      for (y=0; y <= 2*GridSzY; y+=GridSpY)
      {
	glVertex3f((GridSzY - y), -GridSzY, 0.0);
	glVertex3f((GridSzY - y), GridSzY, 0.0);
      }
    }
    glEnd();
  }
  if(bDrawGridZ)
  {
    glColor3fv(GridZcol);
    glBegin(GL_LINES);
    {
      for (z=0; z <= 2*GridSzZ; z+=GridSpZ)
      {
	glVertex3f(0.0, -GridSzZ, (GridSzZ - z));
	glVertex3f(0.0, GridSzZ, (GridSzZ - z));
      }
      for (z=0; z <= 2*GridSzZ; z+=GridSpZ)
      {
	glVertex3f(0.0, (GridSzZ - z), -GridSzZ);
	glVertex3f(0.0, (GridSzZ - z), GridSzZ);
      }
    }
    glEnd();
  }
}
/*
** ----------------------------------------------------------------------------
** définition de la perspective.
** Directement inspiré des sources de Quake (merci john...)
*/
void MYgluPerspective(GLdouble fovy, GLdouble aspect,GLdouble zNear, GLdouble zFar)
{
  GLdouble xmin, xmax, ymin, ymax;

  ymax = zNear * tan( fovy * M_PI / 360.0 );
  ymin = -ymax;

  xmin = ymin * aspect;
  xmax = ymax * aspect;

  glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}
/*
** ----------------------------------------------------------------------------
*/
