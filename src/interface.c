#include "interface.h"

GtkWidget *create_main_window (void) {
    GtkWidget *window;
    
    //Setup main window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Cugb Freer");
    gtk_widget_set_size_request (window, 600, 500);
    gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);
    return window;
}
