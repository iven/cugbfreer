#ifndef _INTERFACE_H
#define _INTERFACE_H

#include <gtk/gtk.h>
#include "misc.h"
#include "about.h"
#include "linker.h"
#include "welcome.h"

#define APP_NAME "Cugb Freer"
#define APP_VERSION "0.1 Alpha1"
#define APP_URL "http://www.tofree.org/"
#define APP_LOGO PKGDATADIR"/pixmaps/logo.png"
#define APP_BANNER PKGDATADIR"/pixmaps/banner.png"
#define LINKER_ICON PKGDATADIR"/pixmaps/linker.png"
#define IPMSG_ICON PKGDATADIR"/pixmaps/ipmsg.png"

GtkWidget *create_main_window (void);

#endif
