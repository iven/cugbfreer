#ifndef _KEYFILE_H
#define _KEYFILE_H

#include "cugbfreer.h"
#include "misc.h"

#define CONFIG_FILE "cugbfreer.conf"

void cf_key_file_load (void);
void cf_key_file_save (void);
gint cf_key_file_get_integer (const gchar *group, const gchar *key);
gboolean cf_key_file_get_boolean (const gchar *group, const gchar *key);
gchar *cf_key_file_get_value (const gchar *group, const gchar *key);
void cf_key_file_set_integer (const gchar *group, const gchar *key, gint value);
void cf_key_file_set_boolean (const gchar *group, const gchar *key, gboolean value);
void cf_key_file_set_value (const gchar *group, const gchar *key, const gchar *value);

#endif
