/*
** ----------------------------------------------------------------------------
** Projet gameDream
**
** Auteur               : Guillaumaud Philippe
**
** commencé le          : 01/04/1999
** dernière modification: 20/01/2000
**
** GBhelp.c
**
** Menu Aide
** ----------------------------------------------------------------------------
*/
#include "GBcommon.h"

#include "GBmisc.h"

static GtkWidget *APropos_dialog1=NULL;

/*
** ----------------------------------------------------------------------------
** Prototypes
*/
static void AProposOK_cb(GtkWidget *,gpointer);

/*
** ----------------------------------------------------------------------------
*/
static void AProposOK_cb(GtkWidget *widget,gpointer data)
{
  gtk_widget_destroy(APropos_dialog1);
  APropos_dialog1=NULL;
}
/*
** ----------------------------------------------------------------------------
** A Propos...
*/
void APropos_cb(GtkWidget *widget,gpointer data)
{
  GtkWidget *dialog_vbox1;
  GtkWidget *label1;
  GtkWidget *dialog_action_area1;
  GtkWidget *button1;

  if(APropos_dialog1 == NULL)
  {
    APropos_dialog1 = gtk_dialog_new ();
    gtk_object_set_data (GTK_OBJECT (APropos_dialog1), "APropos_dialog1", APropos_dialog1);
    gtk_window_set_title (GTK_WINDOW (APropos_dialog1), "A propos...");
    gtk_window_set_policy (GTK_WINDOW (APropos_dialog1), TRUE, TRUE, FALSE);
    gtk_widget_set_usize (APropos_dialog1,200,150);

    dialog_vbox1 = GTK_DIALOG (APropos_dialog1)->vbox;
    gtk_object_set_data (GTK_OBJECT (APropos_dialog1), "dialog_vbox1", dialog_vbox1);
    gtk_widget_show (dialog_vbox1);
    gtk_container_set_border_width (GTK_CONTAINER (dialog_vbox1), 5);

    label1 = gtk_label_new(gd_apropos);
    gtk_widget_ref (label1);
    gtk_object_set_data_full (GTK_OBJECT (APropos_dialog1), "label1", label1,(GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (label1);
    gtk_box_pack_start (GTK_BOX (dialog_vbox1), label1, FALSE, FALSE, 0);

    dialog_action_area1 = GTK_DIALOG (APropos_dialog1)->action_area;
    gtk_object_set_data (GTK_OBJECT (APropos_dialog1), "dialog_action_area1", dialog_action_area1);
    gtk_widget_show (dialog_action_area1);
    gtk_container_set_border_width (GTK_CONTAINER (dialog_action_area1), 10);

    button1 = gtk_button_new_with_label ("OK");
    gtk_widget_ref (button1);
    gtk_object_set_data_full (GTK_OBJECT (APropos_dialog1), "button1", button1,(GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (button1);
    gtk_box_pack_start (GTK_BOX (dialog_action_area1), button1, FALSE, FALSE, 0);

    gtk_signal_connect(GTK_OBJECT(APropos_dialog1),"delete_event",GTK_SIGNAL_FUNC(AProposOK_cb), NULL);
    gtk_signal_connect_object (GTK_OBJECT (button1), "clicked",GTK_SIGNAL_FUNC(AProposOK_cb),NULL);
    gtk_widget_show (APropos_dialog1);
  }
}
/*
** ----------------------------------------------------------------------------
*/
