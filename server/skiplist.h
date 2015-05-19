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

typedef struct _SkipList {
    int level;
    int size;
    pthread_mutex_t lock;
    struct _Node *header;
}SkipList;


int skiplist_init(SkipList *, unsigned char *);

int skiplist_insert_raw(SkipList *, unsigned char *, void *, int(* cmp)(unsigned char *, unsigned char *));

void *skiplist_search_raw(SkipList *, unsigned char *, int(* cmp)(unsigned char *, unsigned char *));

int skiplist_delete_raw(SkipList *, unsigned char *, int(* cmp)(unsigned char *, unsigned char *),void(* del)(void *));

void skiplist_dump(SkipList *);

void skiplist_lock(SkipList *);
void skiplist_unlock(SkipList *);


#endif
