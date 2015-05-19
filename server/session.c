#include "session.h"
#include "skiplist.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>
int cmp(unsigned char * a, unsigned char *b)
{
    return memcmp(a, b, SESSIONLEN);
}

void del(void *value)
{
    free(value);
}

int sessionlist_init(SessionList *list)
{
    unsigned char *keymax = NULL;
    int i;
    keymax = (unsigned char *)malloc(sizeof(unsigned char) * SESSIONLEN);
    for(i = 0; i < SESSIONLEN; i++) {
        keymax = 0xff;     
    }
    if (skiplist_init(&list->skiplist, keymax) < 0)
        return -1;
    return 0;
}

int sessionlist_insert(SessionList *list, unsigned char *key, Session *value)
{
    int res;
    skiplist_lock(&list->skiplist);
    res = skiplist_insert_raw(&list->skiplist, key, (void *)value, cmp);
    skiplist_unlock(&list->skiplist);
    return res;
}

/* retrict optimization to be done */
Session *sessionlist_search(SessionList *list, unsigned char *key)
{
    void *res = NULL;
    skiplist_lock(&list->skiplist);
    res = skiplist_search_raw(&list->skiplist, key, cmp); 
    skiplist_unlock(&list->skiplist);
    if (res == NULL)
        return NULL;
    return (Session *)res;
}

int sessionlist_delete(SessionList *list, unsigned char *key)
{
    int res;
    skiplist_lock(&list->skiplist);
    res = skiplist_delete_raw(&list->skiplist, key, cmp, del);
    skiplist_unlock(&list->skiplist);
    return res;
}

void sessionlist_dump(SessionList *list)
{
    skiplist_dump(&list->skiplist);    
}
