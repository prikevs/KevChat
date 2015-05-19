#ifndef CLIENT_H
#define CLIENT_H
#include "datauct.h"
#include "skiplist.h"
#include <pthread.h>
#include <time.h>

/* nodes for storing one's friends */
typedef struct _Friend {
    unsigned char friendid[USERLEN];
    struct _Friend *next; 
}Friend;

/* head of the friend list */
typedef struct FriendList {
    pthread_mutex_t lock;
    Friend *head; 
}FriendList;

/* client info */
typedef struct _Client {
    unsigned char userid[USERLEN];      /* user's id */
    unsigned char aeskey[AESKEYLEN];    /* aes key for this user */
    time_t request_time;                /* last request time */
    time_t reply_time;                  /* last reply_time */
    int isauthed;                       /* is online */

    FriendList friends;                 /* friend list */
}Client;

typedef struct _ClientList {
    SkipList skiplist;
}ClientList;

int clientlist_init(ClientList *);

int clientlist_insert(ClientList *, unsigned char *, Client *);

Client *clientlist_search(ClienList *, unsigned char *);

int clientlist_isfriend(ClientList *, unsigned char *, unsigned char *);

void clientlist_dump(ClientList *);

#endif
