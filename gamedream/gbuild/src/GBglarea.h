/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commenc� le          : 01/04/1999
** derni�re modification: 27/01/2000
**
** GBglarea.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBGLAREA_H
#define _GBGLAREA_H

gint init(GtkWidget *);
gint draw(GtkWidget *,GdkEventExpose *);
gint reshape(GtkWidget *,GdkEventConfigure *);

GtkWidget *CreateGLarea(GtkWidget **);

#endif
/*
** ----------------------------------------------------------------------------
*/
