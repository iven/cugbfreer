#ifndef _MISC_H
#define _MISC_H

#include <stdio.h>
#include "cugbfreer.h"
#include "keyfile.h"

#define LINK_OPEN_CMD "xdg-open"
#define CF_ENCRYPT_KEY 17

void cf_show_error (GError **err);
gchar *cf_encrypt (const gchar *text);
gchar *cf_decrypt (const gchar *text);
void cf_open_link (const gchar *link_);
gboolean cf_write (const gchar *file, const gchar *data);
gboolean cf_main_quit (GtkWidget *widget, GdkEvent *event, gpointer data);

#endif
