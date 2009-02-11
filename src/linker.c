#include "linker.h"

enum {
    CF_RANGE_WORLD = 1,
    CF_RANGE_CHINA
};
enum {
    CF_TIMEOUT_20MIN,
    CF_TIMEOUT_120MIN
};
typedef struct {
    const gchar *user;
    const gchar *pass;
    gint range;
    gint timeout;
} cf_linker_data;
static cf_linker_data linker_data;
static GtkWidget *user_entry, *pass_entry;
static GtkWidget *range, *timeout;
static void get_linker_data (void) {
    gboolean bool;
    linker_data.user = gtk_entry_get_text (GTK_ENTRY (user_entry));
    linker_data.pass = gtk_entry_get_text (GTK_ENTRY (pass_entry));
    bool = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (range));
    linker_data.range = bool ? CF_RANGE_WORLD : CF_RANGE_CHINA;
    bool = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (timeout));
    linker_data.timeout = bool ? CF_TIMEOUT_120MIN : CF_TIMEOUT_20MIN;
}
static void linker_action (GtkWidget *widget, gpointer operation) {
    struct hostent *host;
    struct sockaddr_in server_addr;
    gint sockfd, length, conn;
    gchar request [100], message [500], buf [MAX_DATA_SIZE];
    // get data, break if there is any error
    get_linker_data ();
    if (g_utf8_strlen (operation, -1) != 10) {
        if (linker_data.user [0] == '\0') {
            perror ("用户名不能为空。");
            return;
        }
        else if (linker_data.pass [0] == '\0') {
            perror ("密码不能为空。");
            return;
        }
    }
    // get ip address
    host = gethostbyname (SERVER_NAME);
    if (host == NULL) {
        herror ("获取IP地址失败。\n");
        return;
    }
    // create a socket
    sockfd = socket (AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror ("创建Socket失败。\n");
        return;
    }
    // set up server_addr
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (SERVER_PORT);
    server_addr.sin_addr = * ((struct in_addr *) host -> h_addr);
    bzero (&server_addr.sin_zero, 8);
    // prepear the message
    sprintf (request, "uid=%s&password=%s&range=%d&timeout=%d&operation=%s",
            linker_data.user, linker_data.pass, linker_data.range,
            linker_data.timeout, (gchar *) operation);
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
        perror ("创建连接失败。\n");
        return;
    }
    send (sockfd, message, g_utf8_strlen (message, -1), 0);
    recv (sockfd, buf, sizeof (buf), 0);
    printf ("%s\n", buf);
    close (sockfd);
}
GtkWidget *create_page_linker (void) {
    GtkWidget *page;
    GtkWidget *frame, *table;
    GtkWidget *label, *button, *image;
    page = gtk_alignment_new (.5, .4, .1, .3);
    frame = gtk_frame_new ("IP控制网关");
    gtk_container_add (GTK_CONTAINER (page), frame);
    table = gtk_table_new (3, 5, FALSE);
    gtk_container_set_border_width (GTK_CONTAINER (table), 15);
    gtk_container_add (GTK_CONTAINER (frame), table);
    // username entry
    label = gtk_label_new ("用户名：");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);
    user_entry = gtk_entry_new ();
    gtk_table_attach_defaults (GTK_TABLE (table), user_entry, 1, 3, 0, 1);
    // password entry
    label = gtk_label_new ("密　码：");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);
    pass_entry = gtk_entry_new ();
    gtk_entry_set_visibility (GTK_ENTRY (pass_entry), FALSE);
    gtk_table_attach_defaults (GTK_TABLE (table), pass_entry, 1, 3, 1, 2);
    // range
    label = gtk_label_new ("访问范围：");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);
    range = gtk_radio_button_new_with_label (NULL, "国内");
    gtk_table_attach_defaults (GTK_TABLE (table), range, 1, 2, 2, 3);
    range = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (range), "国外");
    gtk_table_attach_defaults (GTK_TABLE (table), range, 2, 3, 2, 3);
    // timeout
    label = gtk_label_new ("超时检查：");
    gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);
    timeout = gtk_radio_button_new_with_label (NULL, "20分钟");
    gtk_table_attach_defaults (GTK_TABLE (table), timeout, 1, 2, 3, 4);
    timeout = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (timeout), "2小时");
    gtk_table_attach_defaults (GTK_TABLE (table), timeout, 2, 3, 3, 4);
    // buttons
    button = gtk_button_new_from_stock (GTK_STOCK_CONNECT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (linker_action), "connect");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 0, 1, 4, 5);
    button = gtk_button_new_from_stock (GTK_STOCK_DISCONNECT);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (linker_action), "disconnect");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 1, 2, 4, 5);
    button = gtk_button_new_with_mnemonic ("断开全部(_A)");
    image = gtk_image_new_from_stock (GTK_STOCK_STOP, GTK_ICON_SIZE_SMALL_TOOLBAR);
    gtk_button_set_image (GTK_BUTTON (button), image);
    g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (linker_action), "disconnectall");
    gtk_table_attach_defaults (GTK_TABLE (table), button, 2, 3, 4, 5);
    return page;
}
