#include "misc.h"

extern GError *cf_error;
extern GtkWidget *main_window;
void cf_show_error (void) {
    if (cf_error != NULL) {
        cf_error_dialog (cf_error -> message);
        g_error_free (cf_error);
    }
}
void cf_error_dialog (const gchar *message) {
    GtkWidget *dialog;
    dialog = gtk_message_dialog_new (
            GTK_WINDOW (main_window), GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, message
            );
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
}
void cf_open_link (const gchar *link_) {
    gchar *cmd = g_strjoin (" ", LINK_OPEN_CMD, link_, NULL);
    g_spawn_command_line_async (cmd, NULL);
    g_free (cmd);
}
gboolean cf_write (const gchar *file, const gchar *data) {
    FILE *fp;
    fp = fopen (file, "w");
    if (fp == NULL) {
        cf_error_dialog (_ ("Open file failed."));
        return TRUE;
    }
    else {
        fprintf (fp, data);
        fclose (fp);
        return FALSE;
    }
}
gboolean cf_main_quit (GtkWidget *widget, GdkEvent *event, gpointer data) {
    cf_key_file_save ();
    gtk_main_quit ();
    return FALSE;
}
