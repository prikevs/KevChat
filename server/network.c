/************************************************** 
    >FileName:      network.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * functions attached to socket
***************************************************/

#include "network.h"
#include "database.h"
#include "client.h"
#include "session.h"
#include "logger.h"
#include "msgdispose.h"

#include "datauct.h"

#include <pthread.h>

void *DisposeNewClient(void *ptr)
{
    ThreadData *threaddata = NULL;
    int client_fd = 0;
    unsigned char buf[BUFSIZE];
    SessionList *sessionlist = NULL;
    ClientList *clientlist = NULL;

    threadata = (ThreadData *)ptr;
    client_fd = threadata->client_fd;
    sessionlist = threaddata->sessionlist;
    clientlist = threadata->clientlist;


    threaddata = NULL;
    pthread_exit(NULL);
    
}

int accept_client(int sockfd, SessionList *sessionlist, ClientList *clientlist)
{
    int client_fd = 0;
    struct sockaddr_in client_addr;
    int addrlen = sizeof(struct sockaddr);

    ThreadData threadata;

    pthread_t thread;

    while(1){
        cliend_fd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

        memset(&s, 0, sizeof(threaddata));
        memcpy(&threadata.client_fd, &client_fd, sizeof(client_fd));
        memcpy(&threaddata.client_addr, &client_addr, sizeof(client_addr));
        memcpy(&threaddata.sessionlist, &sessionlist, sizeof(sessionlist));
        memcpy(&threaddata.clientlist, &clientlist, sizeof(clientlist));

        pthread_create(&thread, NULL, DisposeNewClient, (void *)threaddata);
        pthread_detach(thread);
        usleep(100000);
    }

}

int tcp_start(const char *ip_addr, const unsigned short port, SessionList *sessionlist, ClientList *clientlist)
{
    int sockfd, ret;
    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        FM_LOG_FATAL("init sockfd error");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    ret = inet_pton(AF_INET, ip_addr, &server.sin_addr);
    if (ret < 0) {
        FM_LOG_FATAL("inet_pton error");
        return -1;
    }

    ret = bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr));
    if (ret == -1) {
        FM_LOG_FATAL("bind socket error"); 
        return -1;
    }

    ret = listen(sockfd, MAXLISTENl);
    if (ret == -1) {
        FM_LOG_FATAL("listen error"); 
        return -1;
    }

    return accept_client(sockfd, sessionlist, clientlist);
}
