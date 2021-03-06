/************************************************** 
    >FileName:      session.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * save session infomation
      * use skiplist as data structure 
***************************************************/

#ifndef SESSION_H
#define SESSION_H

#include "datauct.h"
#include "skiplist.h"


typedef struct _Session {
//    pthread_mutex_t lock;
    unsigned char userid[USERLEN];
}Session;

typedef struct _SessionList {
    SkipList skiplist;    
}SessionList;

int sessionlist_init(SessionList *);

int sessionlist_insert(SessionList *, unsigned char *, Session *);

Session *sessionlist_search(SessionList *, unsigned char *);

int sessionlist_delete(SessionList *, unsigned char *);

void sessionlist_dump(SessionList *);

#endif
