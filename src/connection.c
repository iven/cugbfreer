#include "connection.h"

enum {
    CF_RANGE_WORLD = 1,
    CF_RANGE_CHINA
};
enum {
    CF_TIMEOUT_20MIN,
    CF_TIMEOUT_120MIN
};
typedef struct {
    gchar *username, *password;
    gboolean range, timeout;
    gboolean savepass, autoconnect;
} connect_data;
typedef struct {
    GtkWidget *user_entry, *pass_entry;
    GtkWidget *range_btn, *timeout_btn;
    GtkWidget *savepass_btn, *autoconnect_btn;
} connect_widgets;

static connect_widgets cwidgets;
static connect_data cdata;
static GtkTextBuffer *connect_status;
static void on_user_entry_activated (GtkWidget *widget, gpointer connect_btn) {
    gtk_button_clicked (GTK_BUTTON (connect_btn));
}
static void on_pass_entry_activated (GtkWidget *widget, gpointer connect_btn) {
    gtk_button_clicked (GTK_BUTTON (connect_btn));
}
static void on_autoconnect_btn_toggled (GtkWidget *widget, gpointer data) {
    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), TRUE);
    }
}
static void on_savepass_btn_toggled (GtkWidget *widget, gpointer data) {
    if (! gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget))) {
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (data), FALSE);
    }
}
static void connect_init (GtkWidget *connect_btn) {
    gchar *temp_value;
    cdata.range = cf_key_file_get_boolean ("Linker", "range");
    cdata.timeout = cf_key_file_get_boolean ("Linker", "timeout");
    cdata.savepass = cf_key_file_get_boolean ("Linker", "savepass");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.range_btn), cdata.range);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.timeout_btn), cdata.timeout);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.savepass_btn), cdata.savepass);
    if (cdata.savepass) {
        cdata.username = cf_key_file_get_value ("Linker", "username");
        cdata.password = cf_key_file_get_value ("Linker", "password");
        cdata.autoconnect = cf_key_file_get_boolean ("Linker", "autoconnect");
        temp_value = cf_decrypt (cdata.password);
        gtk_entry_set_text (GTK_ENTRY (cwidgets.user_entry), cdata.username);
        gtk_entry_set_text (GTK_ENTRY (cwidgets.pass_entry), temp_value);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.autoconnect_btn), cdata.autoconnect);
        if (cdata.autoconnect) {
            gtk_button_clicked (GTK_BUTTON (connect_btn));
        }
        g_free (cdata.username);
        g_free (cdata.password);
        g_free (temp_value);
    }
}
static void connect_status_insert (const gchar *message) {
    GtkTextIter iter_end;
    g_assert (connect_status != NULL);
    gtk_text_buffer_get_end_iter (connect_status, &iter_end);
    gtk_text_buffer_insert (connect_status, &iter_end, message, -1);
}
static gpointer connect_action (gpointer operation) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    gint sockfd, length, conn;
    gchar request [100], message [500], buf [MAX_DATA_SIZE];
    // get ip address
    host = gethostbyname (SERVER_NAME);
    connect_status_insert (_ ("Connecting to the server...\n"));
    if (host == NULL) {
        connect_status_insert (_ ("Failed to get IP address.\n"));
        return NULL;
    }
    // create a socket
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        connect_status_insert (_ ("Failed to create Socket.\n"));
        return NULL;
    }
    // set up server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (SERVER_PORT);
    server_addr.sin_addr = * ((struct in_addr *) host -> h_addr_list [0]);
    bzero (&server_addr.sin_zero, 8);
    // prepear the message
    sprintf (request, "uid=%s&password=%s&range=%d&timeout=%d&operation=%s",
            cdata.username, cdata.password,
            cdata.range ? CF_RANGE_WORLD : CF_RANGE_CHINA,
            cdata.timeout ? CF_TIMEOUT_120MIN : CF_TIMEOUT_20MIN,
            (const gchar *) operation
            );
    length = g_utf8_strlen (request, -1);
    sprintf (message, "POST /ipgw/ipgw.ipgw HTTP/1.1\r\n"
            "Host: "SERVER_NAME"\r\n"
            "Content-Length: %d\r\n"
            "Content-Type: application/x-www-form-urlencoded\r\n"
            "Length: %d\r\n\r\n%s",
            length, length, request);
    // connect
    conn = connect (sockfd, (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
    if (conn == -1) {
        connect_status_insert (_ ("Failed to connect"));
        return NULL;
    }
    send (sockfd, message, g_utf8_strlen (message, -1), 0);
    recv (sockfd, buf, sizeof (buf), 0);
    printf ("%s\n", buf);
    close (sockfd);
    return NULL;
}
static void on_connect_btn_clicked (GtkWidget *widget, gpointer operation) {
    GError *err = NULL;
    gchar *temp_value;
    GThread *connect_thread;
    cdata.username = (gchar *) gtk_entry_get_text (GTK_ENTRY (cwidgets.user_entry));
    cdata.password = (gchar *) gtk_entry_get_text (GTK_ENTRY (cwidgets.pass_entry));
    cdata.range = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (cwidgets.range_btn));
    cdata.timeout = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (cwidgets.timeout_btn));
    cdata.savepass = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (cwidgets.savepass_btn));
    cdata.autoconnect = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (cwidgets.autoconnect_btn));
    if (g_utf8_strlen (operation, -1) != 10) {
        if (cdata.username [0] == '\0') {
            connect_status_insert (_ ("Username cannot be null.\n"));
            return;
        }
        else if (cdata.password [0] == '\0') {
            connect_status_insert (_ ("Password cannot be null.\n"));
            return;
        }
        if (cdata.savepass) {
            temp_value = cf_encrypt (cdata.password);
            cf_key_file_set_value ("Linker", "username", cdata.username);
            cf_key_file_set_value ("Linker", "password", temp_value);
            cf_key_file_set_boolean ("Linker", "autoconnect", cdata.autoconnect);
            g_free (temp_value);
        }
        cf_key_file_set_boolean ("Linker", "range", cdata.range);
        cf_key_file_set_boolean ("Linker", "timeout", cdata.timeout);
        cf_key_file_set_boolean ("Linker", "savepass", cdata.savepass);
    }
    connect_thread = g_thread_create (connect_action, operation, FALSE, &err);
    if (err != NULL) {
        cf_show_error (&err);
    }
}
GtkWidget *create_page_connection (void) {
    GtkWidget *page;
    GtkWidget *frame, *table;
    GtkWidget *label, *button, *image;
    GtkWidget *connect_btn;
    GtkWidget *scrollwindow, *textview;
    page = gtk_alignment_new (.5, .4, .1, .3);
    frame = gtk_frame_new (_ ("IP Gateway"));
    gtk_container_add (GTK_CONTAINER (page), frame);
    table = gtk_table_new (3, 6, FALSE);
    gtk_table_set_row_spacings (GTK_TABLE (table), 5);
    gtk_container_set_border_width (GTK_CONTAINER (table), 15);
    gtk_container_add (GTK_CONTAINER (frame), table);
    // username entry
    label = gtk_label_new (_ ("Username"));
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
    cwidgets.user_entry = gtk_entry_new ();
    g_signal_connect (G_OBJECT (cwidgets.user_entry), "activate", G_CALLBACK (on_user_entry_activated), connect_btn);
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.user_entry, 1, 3, 0, 1);
    // password entry
    label = gtk_label_new (_ ("Password"));
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
    cwidgets.pass_entry = gtk_entry_new ();
    gtk_entry_set_visibility (GTK_ENTRY (cwidgets.pass_entry), FALSE);
    g_signal_connect (G_OBJECT (cwidgets.pass_entry), "activate", G_CALLBACK (on_pass_entry_activated), connect_btn);
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.pass_entry, 1, 3, 1, 2);
    // range
    label = gtk_label_new (_ ("Range:"));
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
    cwidgets.range_btn = gtk_radio_button_new_with_label (NULL, _ ("China"));
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.range_btn, 1, 2, 2, 3);
    cwidgets.range_btn = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (cwidgets.range_btn), _ ("World"));
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.range_btn, 2, 3, 2, 3);
    // timeout
    label = gtk_label_new (_ ("Timeout:"));
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);
    cwidgets.timeout_btn = gtk_radio_button_new_with_label (NULL, _ ("20 Minutes"));
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.timeout_btn, 1, 2, 3, 4);
    cwidgets.timeout_btn = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (cwidgets.timeout_btn), _ ("2 Hours"));
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.timeout_btn, 2, 3, 3, 4);
    // options
    label = gtk_label_new (_ ("Options:"));
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 4, 5);
    cwidgets.savepass_btn = gtk_check_button_new_with_label (_ ("Save password"));
    g_signal_connect (G_OBJECT (cwidgets.savepass_btn), "toggled", G_CALLBACK (on_savepass_btn_toggled), cwidgets.autoconnect_btn);
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.savepass_btn, 1, 2, 4, 5);
    cwidgets.autoconnect_btn = gtk_check_button_new_with_label (_ ("Auto connect"));
    g_signal_connect (G_OBJECT (cwidgets.autoconnect_btn), "toggled", G_CALLBACK (on_autoconnect_btn_toggled), cwidgets.savepass_btn);
    gtk_table_attach_defaults (GTK_TABLE (table), cwidgets.autoconnect_btn, 2, 3, 4, 5);
    // buttons
    connect_btn = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
    g_signal_connect (G_OBJECT (connect_btn), "clicked", G_CALLBACK (on_connect_btn_clicked), "connect");
    gtk_table_attach_defaults (GTK_TABLE (table), connect_btn, 0, 1, 5, 6);
    button = gtk_button_new_from_stock (GTK_STOCK_DISCONNECT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (on_connect_btn_clicked), "disconnect");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 5, 6);
    button = gtk_button_new_with_mnemonic (Q_ ("Mnemonic should be (_D) in Chinese.|_Disconnect All"));
    image = gtk_image_new_from_stock (GTK_STOCK_STOP, GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_image (GTK_BUTTON (button), image);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (on_connect_btn_clicked), "disconnectall");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 3, 5, 6);
    // textview (status)
    scrollwindow = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrollwindow), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scrollwindow), GTK_SHADOW_IN);
    gtk_table_attach_defaults (GTK_TABLE (table), scrollwindow, 0, 3, 6, 7);
    connect_status = gtk_text_buffer_new (NULL);
    textview = gtk_text_view_new_with_buffer (connect_status);
    gtk_text_view_set_editable (GTK_TEXT_VIEW (textview), FALSE);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD_CHAR);
    gtk_container_add (GTK_CONTAINER (scrollwindow), textview);
    // load configuration
    connect_init (connect_btn);
    return page;
}
