#include "client.h"
#include "skiplist.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>

int cmp(unsigned char *a, unsigend char *b)
{
    return memcmp(a, b, USERLEN);
}

void del()
{}

int clientlist_init(ClientList *list)
{
    unsigned char *keymax = NULL;
    int i;
    keymax = (unsigned char *)malloc(sizeof(unsigned char) * USERLEN);    
    for(i = 0; i < USERLEN; i++) {
        keymax = 0xff; 
    }
    if (skiplist_init(&list->skiplist, keymax) < 0)
        return -1;
    return 0;
}

int clientlist_insert(ClientList *list, unsigned char *key, Client *client)
{}

Client *clientlist_search(ClientList *list, unsigned char *key)
{}

int clientlist_isfriend(ClientList *list, unsigned char *key1, unsigned char *key2)
{}

void clientlist_dump(ClientList *list)
{}
