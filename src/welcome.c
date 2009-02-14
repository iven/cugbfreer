#include "welcome.h"

GtkWidget *create_page_welcome (void) {
    GtkWidget *page;
    GtkWidget *label;
    page = gtk_vbox_new (FALSE, 5);
    label = gtk_label_new (_ ("\nCugb Freer is just for testing now. Welcome to send your feedback to me, or to join the project.\n"));
    gtk_box_pack_start (GTK_BOX (page), label, TRUE, FALSE, 5);
    return page;
}
