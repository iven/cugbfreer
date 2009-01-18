#include "interface.h"

GtkWidget *create_main_window (void) {
    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    GtkWidget *banner, *hbuttonbox;
    GtkWidget *button;
    
    //setup main window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "Cugb Freer");
    gtk_widget_set_size_request (window, 700, 600);
    gtk_window_set_resizable (GTK_WINDOW (window), TRUE);
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);
    //setup main container
    vbox = gtk_vbox_new (FALSE, 5);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    //add banner
    banner = gtk_image_new ();
    gtk_widget_set_size_request (banner, -1, 100);
    gtk_box_pack_start (GTK_BOX (vbox), banner, TRUE, FALSE, 5);
    //add mainbody hbox in the middle
    hbox = gtk_hbox_new (FALSE, 5);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 5);
    //add a button box in the bottom
    hbuttonbox = gtk_hbutton_box_new ();
    gtk_box_pack_start (GTK_BOX (vbox), hbuttonbox, FALSE, FALSE, 0);
    //add buttons in the button box
    button = gtk_button_new_from_stock (GTK_STOCK_ABOUT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (show_about), NULL);
    gtk_box_pack_start (GTK_BOX (hbuttonbox), button, FALSE, FALSE, 5);
    button = gtk_button_new_from_stock (GTK_STOCK_QUIT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (gtk_main_quit), NULL);
    gtk_box_pack_end (GTK_BOX (hbuttonbox), button, FALSE, FALSE, 5);
    return window;
}
