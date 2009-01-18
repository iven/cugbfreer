#include "interface.h"

gint main (gint argc, gchar *argv []) {
    GtkWidget *main_window;

    gtk_init (&argc, &argv);

    main_window = create_main_window ();
    gtk_widget_show_all (main_window);

    gtk_main ();
    return 0;
}
