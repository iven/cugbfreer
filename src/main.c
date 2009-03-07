#include "main.h"
#include "keyfile.h"
#include "interface.h"

GtkWidget *main_window;

gint main (gint argc, gchar *argv []) {
    gtk_init (&argc, &argv);
    g_thread_init (NULL);

    bindtextdomain (CF_GETTEXT_PACKAGE, CF_LOCALE_DIR);
    bind_textdomain_codeset (CF_GETTEXT_PACKAGE, "UTF-8");
    textdomain (CF_GETTEXT_PACKAGE);

    cf_key_file_load ();

    main_window = create_main_window ();
    gtk_widget_show_all (main_window);

    gtk_main ();
    return 0;
}
