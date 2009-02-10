#include "interface.h"

#define LINK_OPEN_CMD "xdg-open"

void cf_link_button_clicked (GtkLinkButton *button, const gchar *link_, gpointer data) {
    cf_open_link (link_);
}
void cf_open_link (const gchar *link_) {
    gchar *cmd = g_strjoin (" ", LINK_OPEN_CMD, link_, NULL);
    g_spawn_command_line_async (cmd, NULL);
    g_free (cmd);
}
