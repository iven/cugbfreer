#include "keyfile.h"

extern GError *cf_error;
static GKeyFile *cf_key_file;
void cf_key_file_load (void) {
    const gchar *search_path [2] = {g_get_user_config_dir (), PKGDATADIR};
    cf_key_file = g_key_file_new ();
    g_key_file_load_from_dirs (cf_key_file, CONFIG_FILE,
            search_path, NULL,
            G_KEY_FILE_KEEP_COMMENTS, &cf_error);
    if (cf_error != NULL) {
        cf_show_error ();
        gtk_main_quit ();
    }
}
void cf_key_file_save (void) {
    gchar *config_file_path;
    gchar *key_file_data;
    config_file_path = g_build_filename (g_get_user_config_dir (), CONFIG_FILE, NULL);
    key_file_data = g_key_file_to_data (cf_key_file, NULL, NULL);
    cf_write (config_file_path, key_file_data);
    g_free (key_file_data);
    g_key_file_free (cf_key_file);
}
gint cf_key_file_get_integer (const gchar *group, const gchar *key) {
    gint value = g_key_file_get_integer (cf_key_file, group, key, &cf_error);
    if (cf_error != NULL) {
        cf_show_error ();
        cf_key_file_set_integer (group, key, 0);
        value = 0;
    }
    return value;
}
gboolean cf_key_file_get_boolean (const gchar *group, const gchar *key) {
    gboolean value = g_key_file_get_boolean (cf_key_file, group, key, &cf_error);
    if (cf_error != NULL) {
        cf_show_error ();
        cf_key_file_set_boolean (group, key, FALSE);
        value = FALSE;
    }
    return value;
}
gchar *cf_key_file_get_value (const gchar *group, const gchar *key) {
    gchar *value = g_key_file_get_value (cf_key_file, group, key, &cf_error);
    if (cf_error != NULL) {
        cf_show_error ();
        cf_key_file_set_value (group, key, "");
        value = "";
    }
    return value;
}
void cf_key_file_set_integer (const gchar *group, const gchar *key, gint value) {
    g_key_file_set_integer (cf_key_file, group, key, value);
}
void cf_key_file_set_boolean (const gchar *group, const gchar *key, gboolean value) {
    g_key_file_set_boolean (cf_key_file, group, key, value);
}
void cf_key_file_set_value (const gchar *group, const gchar *key, const gchar *value) {
    g_key_file_set_value (cf_key_file, group, key, value);
}
