/* use skip list to maintain the list of online clients */
/* to be done: mutex for value */

#ifndef SKIPLIST_H
#define SKIPLIST_H


#include <pthread.h>
//#include "clientinfo.h"

typedef struct _Node {
    unsigned char *key;
    void *value;      /* Online Client info */
    struct _Node **forward;
}Node;

typedef struct _Skiplist {
    int level;
    int size;
    pthread_mutex_t lock;
    struct _Node *header;
}Skiplist;


int skiplist_init(Skiplist *, unsigned char *);

int skiplist_insert_raw(Skiplist *, unsigned char *, void *, int(* cmp)(unsigned char *, unsigned char *));

void *skiplist_search_raw(Skiplist *, unsigned char *, int(* cmp)(unsigned char *, unsigned char *));

int skiplist_delete_raw(Skiplist *, unsigned char *, int(* cmp)(unsigned char *, unsigned char *),void(* del)(void *));

void skiplist_dump(Skiplist *);

void skiplist_lock(Skiplist *);
void skiplist_unlock(Skiplist *);


#endif
