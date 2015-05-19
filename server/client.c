#include "client.h"
#include "skiplist.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>

int cmp(unsigned char *a, unsigned char *b)
{
    return memcmp(a, b, USERLEN);
}

void del(void *value)
{}

int clientlist_init(ClientList *list)
{
    unsigned char *keymax = NULL;
    int i;
    keymax = (unsigned char *)malloc(sizeof(unsigned char) * USERLEN);    
    for(i = 0; i < USERLEN; i++) {
        keymax[i] = 0xff; 
    }
    if (skiplist_init(&list->skiplist, keymax) < 0)
        return -1;
    return 0;
}

int clientlist_insert_client(ClientList *list, unsigned char *key, Client *value)
{
    int res, i;
    /*  */
    Friend *head = NULL;
    pthread_mutex_init(&value->friends.lock, NULL);
    head = (Friend *)malloc(sizeof(Friend));
    head->next = NULL;
    for(i = 0; i < USERLEN; i++) {
        head->friendid[i] = 0xff; 
    }
    value->friends.head = head;
    skiplist_lock(&list->skiplist);
    res = skiplist_insert_raw(&list->skiplist, key, (void *)value, cmp);
    skiplist_unlock(&list->skiplist);
    return res;
}

static Client *clientlist_search(ClientList *list, unsigned char *key)
{
    void *res = NULL;
   // skiplist_lock(&list->skiplist);
    res = skiplist_search_raw(&list->skiplist, key, cmp);
   // skiplist_unlock(&list->skiplist);
    if (res == NULL)
        return NULL;
    return (Client *)res;
}

int clientlist_isfriend(ClientList *list, unsigned char *key1, unsigned char *key2)
{
    Client *client = NULL;
    Friend *fri = NULL;
    int res = 1;

    skiplist_lock(&list->skiplist);
    client = clientlist_search(list, key1);
    if (client == NULL)
        res = 0;

    pthread_mutex_lock(&client->friends.lock);

    skiplist_unlock(&list->skiplist);

    fri = client->friends.head->next;
    while(fri != NULL && cmp(key2, fri->friendid) != 0) {
        fri = fri->next; 
    }
    if (fri == NULL)
        res = 0;

    pthread_mutex_unlock(&client->friends.lock);

    return res;
}

int clientlist_addfriend(Friend *head, unsigned char *friendid)
{
    Friend *fri = head; 
    Friend *newfri;
    while(fri->next != NULL && cmp(friendid, fri->friendid) != 0) {
        fri = fri->next;
    }
    if (fri->next == NULL && cmp(friendid, fri->friendid) != 0) {
        newfri = (Friend *)malloc(sizeof(Friend));
        memcpy(newfri->friendid, friendid, USERLEN);
        newfri->next = NULL;
        fri->next = newfri;
    }
    /*
    printf("   ");
    while(head != NULL) {
        printf("%s->", head->friendid);    
        head = head->next;
    }
    printf("NNN\n");
    */
    return 0;
}

int clientlist_delfriend(Friend *head, unsigned char *friendid)
{
    Friend *fri = head->next; 
    Friend *before = head;
    while(fri != NULL && cmp(friendid, fri->friendid) != 0) {
        before = fri;
        fri = fri->next;
    }
    if (fri != NULL && cmp(friendid, fri->friendid) == 0) {
        before->next = fri->next;
        free(fri); 
        return 0;
    }
    return 1;
}

int clientlist_friend_op(ClientList *list, unsigned char *key, unsigned char *friendid, int op)
{
    Client *client = NULL;
    int res = 1;
    skiplist_lock(&list->skiplist);
    client = clientlist_search(list, key);
    if (client == NULL) {
        res = 1;
    }
    pthread_mutex_lock(&client->friends.lock);

    skiplist_unlock(&list->skiplist);

    if (op == 1) {
        res = clientlist_addfriend(client->friends.head, friendid);
    }
    else {
        res = clientlist_delfriend(client->friends.head, friendid);
    }

    pthread_mutex_unlock(&client->friends.lock);
    return res;
}


void clientlist_dump(ClientList *list)
{
    Node *x;
    Friend *y;
    printf("\n");
    skiplist_dump(&list->skiplist);
    x = (&(list->skiplist))->header;
    while(x && x->forward[1] != (&(list->skiplist))->header) {
        printf("%s:", x->forward[1]->key);
        y = ((Client *)(x->forward[1]->value))->friends.head->next;
        while(y) {
            printf("%s->", y->friendid);
            y = y->next;
        }
        x = x->forward[1];
        printf("NIL\n");
    }
}
