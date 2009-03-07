#include <stdio.h>
#include "misc.h"
#include "keyfile.h"

extern GtkWidget *main_window;
void cf_show_error (GError **err) {
    GtkWidget *dialog;
    g_assert (*err != NULL);
    dialog = gtk_message_dialog_new (
            //GTK_WINDOW (main_window), GTK_DIALOG_MODAL,
            NULL, GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,
            (*err) -> message
            );
    gtk_dialog_run (GTK_DIALOG (dialog));
    gtk_widget_destroy (dialog);
    g_error_free (*err);
    *err = NULL;
}
void cf_open_link (const gchar *link_) {
    gchar *cmd = g_strjoin (" ", CF_LINK_OPEN_CMD, link_, NULL);
    g_spawn_command_line_async (cmd, NULL);
    g_free (cmd);
}
gboolean cf_write (const gchar *file, const gchar *data) {
    FILE *fp;
    fp = fopen (file, "w");
    if (fp == NULL) {
        perror (_ ("Open file failed."));
        return TRUE;
    }
    else {
        fprintf (fp, data);
        fclose (fp);
        return FALSE;
    }
}
gchar *cf_encrypt (const gchar *text) {
    gint i;
    gchar *encrypted_text;
    // encrypted_text should be freed when no longer needed
    encrypted_text = g_strdup (text);
    for (i = 0; encrypted_text [i] != '\0'; i++) {
        encrypted_text [i] = encrypted_text [i] ^ CF_ENCRYPT_KEY;
    }
    return encrypted_text;
}
gchar *cf_decrypt (const gchar *text) {
    gint i;
    gchar *decrypted_text;
    // decrypted_text should be freed when no longer needed
    decrypted_text = g_strdup (text);
    for (i = 0; decrypted_text [i] != '\0'; i++) {
        decrypted_text [i] = decrypted_text [i] ^ CF_ENCRYPT_KEY;
    }
    return decrypted_text;
}
gboolean cf_main_quit (GtkWidget *widget, GdkEvent *event, gpointer data) {
    cf_key_file_save ();
    gtk_main_quit ();
    return FALSE;
}
