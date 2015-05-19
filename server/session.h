#ifndef SESSION_H
#define SESSION_H

#include "datauct.h"
#include "skiplist.h"

#define SESSIONLEN 16

typedef struct _Session {
    unsigned char userid[USERLEN];
}Session;

typedef struct _Sessionlist {
    Skiplist skiplist;    
}Sessionlist;

int sessionlist_init(Sessionlist *, unsigned char *);
int sessionlist_insert(Sessionlist *, unsigned char *, Session *);
Session *sessionlist_search(Sessionlist *, unsigned char *);
int sessionlist_delete(Sessionlist *, unsigned char *);
void sessionlist_dump(Sessionlist *);

#endif
