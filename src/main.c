#include "main.h"

GtkWidget *main_window;
GError *cf_error;

gint main (gint argc, gchar *argv []) {
    gtk_init (&argc, &argv);

    bindtextdomain (GETTEXT_PACKAGE, PKGLOCALEDIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    cf_key_file_load ();

    main_window = create_main_window ();
    gtk_widget_show_all (main_window);

    gtk_main ();
    return 0;
}
