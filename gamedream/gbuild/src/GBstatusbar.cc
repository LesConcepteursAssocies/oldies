/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 02/03/2000
**
** GBstatusbar.c
**
** Création de la barre de status principale
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBstatusbar.h"
#include "GBmisc.h"

/*
** ----------------------------------------------------------------------------
** Variables
*/
static    guint     idStatusBar;

/*
** ----------------------------------------------------------------------------
*/
GtkWidget *CreateMainStatusbar(GtkWidget **pmainw)
{
  GtkWidget *mainwindow=*pmainw;
  GtkWidget *statusbar1;

  statusbar1 = gtk_statusbar_new ();
  gtk_widget_set_name (statusbar1, "statusbar1");
  gtk_widget_ref (statusbar1);
  gtk_object_set_data_full (GTK_OBJECT (mainwindow), "statusbar1", statusbar1,(GtkDestroyNotify) gtk_widget_unref);
  /* Création d'une série */
  idStatusBar = gtk_statusbar_get_context_id(GTK_STATUSBAR(statusbar1),"Mes messages");
  gtk_widget_show (statusbar1);

  return statusbar1;
}
/*
** ----------------------------------------------------------------------------
** Affichage d'un message dans la barre de status
*/
void GBPrintStatusBar(const char *format, ...)
{
  GtkStatusbar *Barre=NULL;
  gchar msg[1024];
  va_list args;

  va_start(args,format);
  vsprintf(msg,format,args);
  va_end(args);

  if(mainwindow)
    Barre = (GtkStatusbar *) lookup_widget(mainwindow,"statusbar1");
  if(Barre)
    gtk_statusbar_push(Barre, idStatusBar, msg);
}
/*
** ----------------------------------------------------------------------------
*/
