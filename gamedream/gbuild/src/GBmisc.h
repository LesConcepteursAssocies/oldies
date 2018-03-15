/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 16/02/2000
**
** GBmisc.h
** ----------------------------------------------------------------------------
*/
#ifndef _GBMISC_H
#define _GBMISC_H

GtkWidget *lookup_widget(GtkWidget *,const gchar *);
void       GBPrintf(const char *, ...);
void       GBAddListDir(GList **,const gchar *);
gchar     *GBCheckFileExist(const gchar *,const gchar *);
GtkWidget *pixmap_new(char *,GtkWidget *);
GtkWidget *create_fileselection(char *,gpointer,gpointer);
GtkWidget *create_colorselection(char *,gpointer,gpointer,gfloat *);
void       GBInit(void);
void       GBExit(GtkWidget *,gpointer);

#endif
/*
** ----------------------------------------------------------------------------
*/
