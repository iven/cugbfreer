#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <stdio.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "cugbfreer.h"
#include "keyfile.h"

#define SERVER_NAME "sun250.cugb.edu.cn"
#define SERVER_PORT 80
#define MAX_DATA_SIZE 8192

GtkWidget *create_page_connection (void);

#endif
