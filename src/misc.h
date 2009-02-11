#ifndef _MISC_H
#define _MISC_H

#include "main.h"

#define LINK_OPEN_CMD "xdg-open"

void cf_link_button_clicked (GtkLinkButton *button, const gchar *link_, gpointer data);
void cf_open_link (const gchar *link_);

#endif
