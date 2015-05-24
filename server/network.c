//gcc -o network network.c skiplist.c session.c logger.c msgdispose.c ../libcrypt/libcrypt.c ../libcrypt/dh.c ../libcrypt/symmetric.c -I../include -lgmp -lpthread -ltomcrypt -Wall

/************************************************** 
    >FileName:      network.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * functions attached to socket
***************************************************/
/* 
   This is just a demonstrate implementation with multi-thread handling TCP connections, and will be optimized with epoll and get Auth and Msg separated
*/

/*
    Authentication process:
      * Use Diffie-Hellman to generate AES key first:
          server and client both generate a 128-bit random number, and get their public keys with the base 65537 and a 128-bit prime number 2^128-233
          server send the number to client
          client recv server's public key and cacualte the number to get the final key, then send its public key
          server recv client's public key to get the same final key too, then use the key encrypt a random salt number
          client recv the data and decrypt it, then use the salt to hash the hashed key, and send it with the userid
          server verify userid and passwd, if success, respond with the UDP address, and refresh this client's online status, or return wrong passwd.
          client use UDP to send other messages.
 
*/

#include "network.h"
#include "database.h"
#include "client.h"
#include "session.h"
#include "logger.h"
#include "msgdispose.h"

#include "datauct.h"

#include <pthread.h>

#define MAXLISTEN 6
#define MAX_WRONG_PASSWD 3
void dump(unsigned char *arr, int len)
{
    int i;
    for(i = 0; i < len; i++)
        printf("%02x ", arr[i]);
    printf("\n");
}

void log_success(SessionList *sessionlist, ClientList *clientlist, unsigned char *session, unsigned long *slen)
{}


void *DisposeNewClient(void *ptr)
{
    ThreadData *threaddata = (ThreadData *)ptr;

    unsigned char pri[AESKEYLEN];
    unsigned char key[AESKEYLEN];
    unsigned char salt[AESKEYLEN];
    unsigned char session[AESKEYLEN];
    unsigned char buf[BUFSIZE];

    struct sockaddr_in client_addr;

    SessionList *sessionlist = NULL;
    ClientList *clientlist = NULL;

    unsigned long len = sizeof(buf);
    unsigned long keylen = sizeof(key);
    unsigned long slen = sizeof(session);
    unsigned long saltlen = sizeof(salt);

    ssize_t res = 0;
    int client_fd = 0;
    int t = 0;

    memset(buf, 0, sizeof(buf));
    memset(&client_addr, 0, sizeof(client_addr));

    memcpy(&(client_addr), &(threaddata->client_addr), sizeof(client_addr));
    client_fd = threaddata->client_fd;
    sessionlist = threaddata->sessionlist;
    clientlist = threaddata->clientlist;
    /* free the memory of threaddata malloc in the main thread */
    free(threaddata);


    /* make diffie-hellman message */
    Auth_MakeDHMsg(buf, &len, pri, AESKEYLEN);
    res = send(client_fd, buf, len, 0);
    //dump(buf, len);
    if (res < 0) {
        close(threaddata->client_fd);
        threaddata = NULL; 
        pthread_exit(NULL);
    } 
    //    printf(":x\n");
    memset(buf, 0, sizeof(buf));
    res = recv(client_fd, buf, BUFSIZE, 0); 
    //dump(buf, 16);
    if (res < 0) {
        close(threaddata->client_fd);
        threaddata = NULL; 
        pthread_exit(NULL);
    }
    Auth_ParseDHMsg(buf, res, pri, 16, key, &keylen);
    //dump(key, 16);

    /* send salt number */
    Auth_MakeSaltMsg(buf, &len, salt, &saltlen, key, keylen);
    printf("salt: ");
    dump(salt, saltlen);
    res = send(client_fd, buf, len, 0);
    //dump(buf, len);
    if (res < 0) {
        close(threaddata->client_fd);
        threaddata = NULL; 
        pthread_exit(NULL);
    } 

    t = 0;
    while(t < MAX_WRONG_PASSWD) {
        res = recv(client_fd, buf, BUFSIZE, 0); 
        if (res < 0) {
            t++; 
            continue;
        }
        if (Auth_Verify(buf, res, salt, saltlen, key, keylen) == 0) {
            t++;
            continue;
        } 
        log_success(sessionlist, clientlist, session, &slen);
        Auth_MakeSuccessMsg(buf, &len, session, slen, key, keylen);
        break; 
    } 

    close(threaddata->client_fd);
    pthread_exit(NULL);
    
}

int accept_client(int sockfd, SessionList *sessionlist, ClientList *clientlist)
{
    int client_fd = 0;
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(struct sockaddr);

    ThreadData *threaddata = NULL;

    pthread_t thread;

    while(1){
        client_fd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        threaddata = (ThreadData *)malloc(sizeof(ThreadData));
//        send(client_fd, "1234", 4, 0);

        memset(threaddata, 0, sizeof(ThreadData));

        memcpy(&(threaddata->client_fd), &client_fd, sizeof(client_fd));

        memcpy(&(threaddata->client_addr), &client_addr, sizeof(client_addr));
        memcpy(&(threaddata->sessionlist), &sessionlist, sizeof(sessionlist));
        memcpy(&(threaddata->clientlist), &clientlist, sizeof(clientlist));

        pthread_create(&thread, NULL, DisposeNewClient, (void *)threaddata);
        threaddata = NULL;
        pthread_detach(thread);
        usleep(100000);
    }

}

int tcp_start(const char *ip_addr, const unsigned short port, SessionList *sessionlist, ClientList *clientlist)
{
    int sockfd, ret;
    int on = 1;
    struct sockaddr_in server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(sockfd ,SOL_SOCKET ,SO_REUSEADDR,(const char*)&on,sizeof(on));
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

    ret = listen(sockfd, MAXLISTEN);
    if (ret == -1) {
        FM_LOG_FATAL("listen error"); 
        return -1;
    }

    return accept_client(sockfd, sessionlist, clientlist);
}

int main()
{
    SessionList sessionlist;
    ClientList clientlist;
    log_open("log.txt");
    tcp_start("127.0.0.1", 9999, &sessionlist, &clientlist); 
    return 0;
}
