/* use skip list to maintain the list of online clients */
/* to be done: mutex for value */

#ifndef SKIPLIST_H
#define SKIPLIST

#define DEMO

#include <pthread.h>
//#include "clientinfo.h"
#ifdef DEMO
typedef struct _Client {
    int value;    
}Client;
#endif

typedef struct _Node {
    int key;
    void *value;      /* Online Client info */
    struct _Node **forward;
}Node;

typedef struct _Skiplist {
    int level;
    int size;
    pthread_mutex_t lock;
    struct _Node *header;
}Skiplist;


int skiplist_init(Skiplist *);

int skiplist_insert(Skiplist *, int, void *);

void *skiplist_search(Skiplist *, int);

int skiplist_delete(Skiplist *, int);

void skiplist_dump(Skiplist *);


#endif
