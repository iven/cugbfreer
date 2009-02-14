#include "misc.h"

static void on_url_activated (GtkAboutDialog *about, const gchar *link_, gpointer data) {
    // this will convert urls to link button
}
static void on_email_activated (GtkAboutDialog *about, const gchar *link_, gpointer data) {
    cf_open_link (link_);
}
void show_about (GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    const gchar *authors [] = {
        "Iven <mailto:ivenvd@gmail.com>",
        NULL
    };
    dialog = gtk_about_dialog_new ();
    gtk_about_dialog_set_name (GTK_ABOUT_DIALOG (dialog), APP_NAME);
    gtk_about_dialog_set_version (GTK_ABOUT_DIALOG (dialog), APP_VERSION);
    gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (dialog), "http://www.tofree.org/");
    gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (dialog), _ ("ToFree Open Forum"));
    gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (dialog), authors);
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (dialog), _ ("Cugb Freer is a toolkit for students in CUGB."));
    gtk_about_dialog_set_url_hook (on_url_activated, NULL, NULL);
    gtk_about_dialog_set_email_hook (on_email_activated, NULL, NULL);
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}
