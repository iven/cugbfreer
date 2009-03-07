#include "keyfile.h"
#include "misc.h"


static GKeyFile *cf_key_file;
void cf_key_file_load (void) {
    GError *err = NULL;
    const gchar *search_path [2] = {g_get_user_config_dir (), CF_DATA_DIR};
    cf_key_file = g_key_file_new ();
    g_key_file_load_from_dirs (cf_key_file, CF_CONFIG_FILE_NAME,
            search_path, NULL,
            G_KEY_FILE_KEEP_COMMENTS, &err);
    if (err != NULL) {
        cf_show_error (&err);
        gtk_main_quit ();
    }
}
void cf_key_file_save (void) {
    gchar *config_file_path;
    gchar *key_file_data;
    config_file_path = g_build_filename (g_get_user_config_dir (), CF_CONFIG_FILE_NAME, NULL);
    key_file_data = g_key_file_to_data (cf_key_file, NULL, NULL);
    cf_write (config_file_path, key_file_data);
    g_free (key_file_data);
    g_key_file_free (cf_key_file);
}
gint cf_key_file_get_integer (const gchar *group, const gchar *key) {
    GError *err = NULL;
    gint value = g_key_file_get_integer (cf_key_file, group, key, &err);
    if (err != NULL) {
        cf_show_error (&err);
        cf_key_file_set_integer (group, key, 0);
        value = 0;
    }
    return value;
}
gboolean cf_key_file_get_boolean (const gchar *group, const gchar *key) {
    GError *err = NULL;
    gboolean value = g_key_file_get_boolean (cf_key_file, group, key, &err);
    if (err != NULL) {
        cf_show_error (&err);
        cf_key_file_set_boolean (group, key, FALSE);
        value = FALSE;
    }
    return value;
}
gchar *cf_key_file_get_value (const gchar *group, const gchar *key) {
    GError *err = NULL;
    gchar *value = g_key_file_get_value (cf_key_file, group, key, &err);
    if (err != NULL) {
        cf_show_error (&err);
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
