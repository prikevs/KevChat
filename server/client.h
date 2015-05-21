/************************************************** 
    >FileName:      client.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
        save clients' online status and friendlist
        use skiplist as base data structure 
***************************************************/

#ifndef CLIENT_H
#define CLIENT_H
#include "datauct.h"
#include "skiplist.h"
#include <pthread.h>
#include <time.h>

/* nodes for storing one's friends */
typedef struct _Friend {
    unsigned char userid[IDLEN];
    unsigned char username[USERLEN];
    int status;
    struct _Friend *next; 
}Friend;

/* head of the friend list */
typedef struct FriendList {
    pthread_mutex_t lock;
    Friend *head; 
}FriendList;

/* client info */
typedef struct _Client {
    unsigned char userid[IDLEN];
    unsigned char username[USERLEN];      /* user's id */
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

int clientlist_insert_client(ClientList *, unsigned int, Client *);

//Client *clientlist_search(ClienList *, unsigned char *);

int clientlist_isfriend(ClientList *, unsigned int, unsigned int);

int clientlist_friend_op(ClientList *, unsigned int, unsigned int, int);

void clientlist_dump(ClientList *);

#endif
