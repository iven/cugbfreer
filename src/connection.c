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

static void connect_init (GtkWidget *connect_btn) {
    gchar *temp_value;
    connect_data temp_data;
    temp_data.range = cf_key_file_get_boolean ("Linker", "range");
    temp_data.timeout = cf_key_file_get_boolean ("Linker", "timeout");
    temp_data.savepass = cf_key_file_get_boolean ("Linker", "savepass");
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.range_btn), temp_data.range);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.timeout_btn), temp_data.timeout);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.savepass_btn), temp_data.savepass);
    if (temp_data.savepass) {
        temp_data.username = cf_key_file_get_value ("Linker", "username");
        temp_data.password = cf_key_file_get_value ("Linker", "password");
        temp_data.autoconnect = cf_key_file_get_boolean ("Linker", "autoconnect");
        temp_value = cf_decrypt (temp_data.password);
        gtk_entry_set_text (GTK_ENTRY (cwidgets.user_entry), temp_data.username);
        gtk_entry_set_text (GTK_ENTRY (cwidgets.pass_entry), temp_value);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (cwidgets.autoconnect_btn), temp_data.autoconnect);
        if (temp_data.autoconnect) {
            gtk_button_clicked (GTK_BUTTON (connect_btn));
        }
        g_free (temp_data.username);
        g_free (temp_data.password);
        g_free (temp_value);
    }
}
static void connect_set_status (const gchar *message) {
    gtk_text_buffer_set_text (connect_status, message, -1);
}
static void connect_show_message (const gchar *buf) {
    GError *err = NULL;
    GRegex *regex;
    GMatchInfo *match_info;
    gboolean matched;
    gchar *message, *message_table, *message_with_blank;
    // convert message to UTF-8
    message = g_convert (buf, -1, "UTF-8", "GB2312", NULL, NULL, &err);
    if (err != NULL) {
        cf_show_error (&err);
        return;
    }
    // get table part in message
    regex = g_regex_new ("<table.*</table>", G_REGEX_CASELESS | G_REGEX_DOTALL, 0, &err);
    if (err != NULL) {
        cf_show_error (&err);
        g_free (message);
        return;
    }
    matched = g_regex_match (regex, message, 0, &match_info);
    g_regex_unref (regex);
    g_free (message);
    if (!matched) {
        connect_set_status (_ ("Unusual connection message recieved.\n"));
        return;
    }
    message_table = g_match_info_fetch (match_info, 0);
    g_match_info_free (match_info);
    // remove html tags
    regex = g_regex_new ("&nbsp;|<[^>]*>", G_REGEX_CASELESS, 0, &err);
    if (err != NULL) {
        cf_show_error (&err);
        g_free (message_table);
        return;
    }
    message_with_blank = g_regex_replace (regex, message_table, -1, 0, "", 0, &err);
    g_regex_unref (regex);
    g_free (message_table);
    if (err != NULL) {
        cf_show_error (&err);
        return;
    }
    // remove new lines
    // TODO: not perfect
    regex = g_regex_new ("\n\n|^\n", G_REGEX_MULTILINE, 0, &err);
    if (err != NULL) {
        cf_show_error (&err);
        g_free (message_with_blank);
        return;
    }
    message = g_regex_replace (regex, message_with_blank, -1, 0, "\n", 0, &err);
    g_regex_unref (regex);
    g_free (message_with_blank);
    if (err != NULL) {
        cf_show_error (&err);
        return;
    }
    // show message
    connect_set_status (message);
    g_free (message);
}
static gpointer connect_action (gpointer operation) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    gint sockfd, length, conn;
    gchar request [100], message [500], buf [MAX_DATA_SIZE];
    // get ip address
    host = gethostbyname (SERVER_NAME);
    if (host == NULL) {
        connect_set_status (_ ("Failed to get IP address.\n"));
        return NULL;
    }
    // create a socket
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        connect_set_status (_ ("Failed to create Socket.\n"));
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
    // TODO:non-block
    connect_set_status (_ ("Connecting to the server...\n"));
    conn = connect (sockfd, (struct sockaddr *) &server_addr, sizeof (struct sockaddr));
    if (conn == -1) {
        connect_set_status (_ ("Failed to connect"));
        close (sockfd);
        return NULL;
    }
    send (sockfd, message, g_utf8_strlen (message, -1), 0);
    // TODO:non-block
    recv (sockfd, buf, sizeof (buf), 0);
    close (sockfd);
    connect_show_message (buf);
    return NULL;
}
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
            connect_set_status (_ ("Username cannot be null.\n"));
            return;
        }
        else if (cdata.password [0] == '\0') {
            connect_set_status (_ ("Password cannot be null.\n"));
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
