#ifndef CLIENT_H
#define CLIENT_H
#include "datauct.h"
#include "skiplist.h"
#include <pthread.h>

typedef struct _Friend {
    unsigned char friendid[USERLEN];
    struct _Friend *next; 
}Friend;

typedef struct FriendList {
    pthread_mutex_t lock;
    Friend *head; 
}FriendList;

typedef struct _Client {
    unsigned char userid[USERLEN];
    FriendList friends; 
}Client;

typedef struct _ClientList {
    SkipList skiplist;
}ClientList;

int clientlist_init(ClientList *, unsigned char *);

int clientlist_insert(ClientList *, unsigned char *, Client *);

Client *clientlist_search(ClienLlist *, unsigned char *);

void clientlist_dump(ClientList *);

#endif
