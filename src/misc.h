#ifndef _MISC_H
#define _MISC_H

#include <stdio.h>
#include "cugbfreer.h"
#include "keyfile.h"

#define LINK_OPEN_CMD "xdg-open"

void cf_show_error (void);
void cf_open_link (const gchar *link_);
void cf_error_dialog (const gchar *message);
gboolean cf_write (const gchar *file, const gchar *data);
gboolean cf_main_quit (GtkWidget *widget, GdkEvent *event, gpointer data);

#endif
