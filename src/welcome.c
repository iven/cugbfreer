#include "interface.h"

GtkWidget *create_page_welcome (void) {
    GtkWidget *page;
    GtkWidget *label;
    page = gtk_vbox_new (FALSE, 5);
    label = gtk_label_new (APP_NAME" "APP_VERSION"\nCugb Freer目前为内部测试版，欢迎试用并提出宝贵意见，或者加入此项目。\n");
    gtk_box_pack_start (GTK_BOX (page), label, TRUE, FALSE, 5);
    return page;
}
