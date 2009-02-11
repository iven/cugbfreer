#ifndef _LINKER_H
#define _LINKER_H

#include <stdio.h>
#include <strings.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include "main.h"

#define SERVER_NAME "sun250.cugb.edu.cn"
#define SERVER_PORT 80
#define MAX_DATA_SIZE 8192

GtkWidget *create_page_linker (void);

#endif
