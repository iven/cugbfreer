#include "interface.h"

GtkWidget *create_main_window (void) {
    GtkWidget *window;
    
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);
    return window;
}
