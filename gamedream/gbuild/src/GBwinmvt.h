/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commenc� le          : 01/04/1999
** derni�re modification: 17/03/2000
**
** GBwinmvt.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBWINMVT_H
#define _GBWINMVT_H

/*
** D�finition du type cone de vision
*/
class   GDvision
{
 public:

  GLdouble     fov;
  GLdouble     zNear;
  GLdouble     zFar;
  GLint        width;
  GLint        height;

  // Constructeurs.
  GDvision();
  GDvision(GLdouble,GLdouble,GLdouble,GLuint,GLuint);
  ~GDvision();
};

void        OpenMvt(GtkWidget *,gpointer);

#endif
/*
** ----------------------------------------------------------------------------
*/
