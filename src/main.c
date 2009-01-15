#include "interface.h"

int main (int argc, char *argv []) {
    GtkWidget *main_window;

    gtk_init (&argc, &argv);

    main_window = create_main_window ();
    gtk_widget_show_all (main_window);

    gtk_main ();
    return 0;
}
