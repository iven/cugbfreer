#ifndef _CONFIG_H
#define _CONFIG_H

#include <gtk/gtk.h>
#include <glib/gi18n.h>

#define APP_NAME "Cugb Freer"
#define APP_VERSION "0.1 Alpha2"
#define APP_URL "http://www.tofree.org/"

#define CF_DATA_DIR "${CF_DATA_DIR}"
#define CF_PIXMAPS_DIR "${CF_PIXMAPS_DIR}"
#define CF_DOC_DIR "${CF_DOC_DIR}"
#define CF_LOCALE_DIR "${CF_LOCALE_DIR}"

#define CF_GETTEXT_PACKAGE "cugbfreer"
#define CF_CONFIG_FILE_NAME "cugbfreer.conf"
#define CF_LINK_OPEN_CMD "xdg-open"
#define CF_ENCRYPT_KEY 124

#endif
