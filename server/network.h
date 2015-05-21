/************************************************** 
    >FileName:      network.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
        * functions attached to socket
***************************************************/

#ifndef NETWORK_H
#define NETWORK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netdb.h>
#include <time.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <limits.h>

#define BUFSIZE 4096

typedef _ThreadData {
    int client_fd;
    struct sockaddr_in client_addr;    
    SessionList *sessionlist;
    ClientList *clientlist;
}ThreadData;

#endif
