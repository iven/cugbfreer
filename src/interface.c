#include "interface.h"

enum {
    NOTE_LINKER,
    NOTE_IPMSG,
    NOTES_COUNT,
    NOTE_WELCOME
};
enum {
    PAGE_WELCOME,
    PAGE_LINKER,
    PAGES_COUNT
};
typedef struct {
    gchar *label;
    gchar *icon;
} cf_note;
typedef struct {
    gint notenum;
    gchar *label;
    GtkWidget * (*func) (void);
} cf_page;
static cf_note notes [] = {
    {"网关连接", LINKER_ICON},
    {"飞鸽传书", IPMSG_ICON},
    {NULL, NULL}
};
static cf_page pages [] = {
    {NOTE_WELCOME, "欢迎界面", create_page_welcome},
    {NOTE_LINKER, "连接网关", create_page_linker},
    {NOTES_COUNT, NULL, NULL}
};
static GtkWidget *notebook;
static void show_pages (GtkWidget *widget, gpointer notenum) {
    GtkWidget *page;
    gint pagenum;
    // only show pages whose note number is notenum
    for (pagenum = 0; pagenum < PAGES_COUNT; pagenum ++) {
        page = gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook), pagenum);
        if (pages [pagenum].notenum == GPOINTER_TO_INT (notenum)) {
            gtk_widget_show (page);
        }
        else {
            gtk_widget_hide (page);
        }
    }
}
static void create_notebook (void) {
    GtkWidget *label, *page;
    cf_page *p_page;
    notebook = gtk_notebook_new ();
    // create and append pages
    for (p_page = pages; p_page -> label != NULL; p_page ++) {
        label = gtk_label_new (p_page -> label);
        page = (*p_page -> func) ();
        gtk_widget_show_all (page);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebook), page, label);
    }
}
static GtkWidget *create_toolbar (void) {
    GtkWidget *toolbar;
    GtkWidget *image;
    GtkToolItem *item_button;
    cf_note *p_note;
    toolbar = gtk_toolbar_new ();
    gtk_toolbar_set_orientation (GTK_TOOLBAR (toolbar), GTK_ORIENTATION_VERTICAL);
    // create and insert buttons
    for (p_note = notes; p_note -> label != NULL; p_note ++) {
        image = gtk_image_new_from_file (p_note -> icon);
        item_button = gtk_tool_button_new (image, p_note -> label);
        gtk_toolbar_insert (GTK_TOOLBAR (toolbar), item_button, -1);
        g_signal_connect (G_OBJECT (item_button), "clicked", G_CALLBACK (show_pages), GINT_TO_POINTER (p_note - notes));
    }
    return toolbar;
}
GtkWidget *create_main_window (void) {
    GtkWidget *window;
    GtkWidget *vbox, *hbox;
    GtkWidget *banner;
    GtkWidget *toolbar, *button;
    
    // setup default icon
    gtk_window_set_default_icon_from_file (APP_LOGO, NULL);
    // set link buttons uri hook
    gtk_link_button_set_uri_hook (cf_link_button_clicked, NULL, NULL);
    // setup main window
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), APP_NAME);
    gtk_widget_set_size_request (window, 650, 650);
    gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
    gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);
    g_signal_connect (G_OBJECT (window), "delete-event", G_CALLBACK (gtk_main_quit), NULL);
    // setup main container
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    // add banner
    banner = gtk_image_new_from_file (APP_BANNER);
    gtk_box_pack_start (GTK_BOX (vbox), banner, FALSE, FALSE, 0);
    // add mainbody hbox in the middle
    hbox = gtk_hbox_new (FALSE, 5);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
    // add tool buttons in the left
    toolbar = create_toolbar ();
    gtk_box_pack_start (GTK_BOX (hbox), toolbar, FALSE, TRUE, 0);
    // add a notebook in the right
    create_notebook ();
    gtk_box_pack_start (GTK_BOX (hbox), notebook, TRUE, TRUE, 0);
    gtk_widget_set_no_show_all (notebook, TRUE);
    gtk_widget_show (notebook);
    show_pages (notebook, GINT_TO_POINTER (NOTE_WELCOME));
    // add a button box in the bottom
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_set_border_width (GTK_CONTAINER (hbox), 5);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
    // add buttons in the button box
    button = gtk_button_new_from_stock (GTK_STOCK_ABOUT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (show_about), NULL);
    gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
    button = gtk_link_button_new_with_label (APP_URL, "欢迎光临 ToFree 开源社区");
    gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 5);
    button = gtk_button_new_from_stock (GTK_STOCK_QUIT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (gtk_main_quit), NULL);
    gtk_box_pack_end (GTK_BOX (hbox), button, FALSE, FALSE, 0);

    return window;
}
