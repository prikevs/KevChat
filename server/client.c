/************************************************** 
    >FileName:      client.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * save clients' online status and friendlist
      * use skiplist as base data structure 
***************************************************/

#include "client.h"
#include "skiplist.h"
#include "logger.h"
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

/* convert char* to int */
void keytoid(unsigned int *id, unsigned char *key)
{
    *id = *((unsigned int *)key);
    *id = ntohl(*id);
}

/* convert int to char* */
void idtokey(unsigned int id, unsigned char *key)
{
    id = htonl(id);
    *((int *)key) = id;
}

/* callback function, used to compare key */
int cmp(unsigned char *a, unsigned char *b)
{
    unsigned int na, nb;
    keytoid(&na, a);
    keytoid(&nb, b);
    return na-nb;
    /*
    printf("na-nb=%d\n", na-nb);
    printf("na=%d, nb=%d\n", na, nb);
    */
}

void del(void *value)
{}

int clientlist_init(ClientList *list)
{
    unsigned char *keymax = NULL;
    int i;
    keymax = (unsigned char *)malloc(sizeof(unsigned char) * IDLEN);    
    /* set the max of the key */
    keymax[0] = 0x7f;
    for(i = 1; i < IDLEN; i++) {
        keymax[i] = 0xff; 
    }
    if (skiplist_init(&list->skiplist, keymax) < 0)
        return -1;
    return 0;
}

int clientlist_insert_client(ClientList *list, unsigned int userid, Client *value)
{
    int res, i;
    /*  */
    Friend *head = NULL;
    unsigned char *key = (unsigned char *)malloc(IDLEN * sizeof(unsigned char));
    idtokey(userid, key);

    pthread_mutex_init(&value->friends.lock, NULL);
    head = (Friend *)malloc(sizeof(Friend));
    head->next = NULL;
    for(i = 0; i < IDLEN; i++) {
        head->userid[i] = 0xff; 
    }
    value->friends.head = head;
    skiplist_lock(&list->skiplist);
    res = skiplist_insert_raw(&list->skiplist, key, (void *)value, cmp);
    skiplist_unlock(&list->skiplist);
    return res;
}

/* search for the value with the key */
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

/* judge if id2 in id1's friend list */
int clientlist_isfriend(ClientList *list, unsigned int id1, unsigned int id2)
{
    Client *client = NULL;
    Friend *fri = NULL;
    int res = 1;
    unsigned char key1[IDLEN];
    unsigned char key2[IDLEN];
    idtokey(id1, key1);
    idtokey(id2, key2);

    skiplist_lock(&list->skiplist);
    client = clientlist_search(list, key1);
    if (client == NULL)
        res = 0;

    pthread_mutex_lock(&client->friends.lock);

    skiplist_unlock(&list->skiplist);

    fri = client->friends.head->next;
    while(fri != NULL && cmp(key2, fri->userid) != 0) {
        fri = fri->next; 
    }
    if (fri == NULL)
        res = 0;

    pthread_mutex_unlock(&client->friends.lock);

    return res;
}

/* add friendid in one's friend list */
static int clientlist_addfriend(Friend *head, unsigned char *friendid)
{
    Friend *fri = head; 
    Friend *newfri;
    while(fri->next != NULL && cmp(friendid, fri->userid) != 0) {
        fri = fri->next;
    }
    if (fri->next == NULL && cmp(friendid, fri->userid) != 0) {
        newfri = (Friend *)malloc(sizeof(Friend));
        memcpy(newfri->userid, friendid, IDLEN);
        newfri->next = NULL;
        fri->next = newfri;
    }
    return 0;
}

/* delete friendid in one's friend list
   success return 0
   failed  return 1
*/
static int clientlist_delfriend(Friend *head, unsigned char *friendid)
{
    Friend *fri = head->next; 
    Friend *before = head;
    while(fri != NULL && cmp(friendid, fri->userid) != 0) {
        before = fri;
        fri = fri->next;
    }
    if (fri != NULL && cmp(friendid, fri->userid) == 0) {
        before->next = fri->next;
        free(fri); 
        return 0;
    }
    return 1;
}

/* operation of friendlist 
   id1 is the target list
   id2 is the friendid to be added or deleted
   op: 1 add friend
       2 del friend
*/
int clientlist_friend_op(ClientList *list, unsigned int id1, unsigned int id2, int op)
{
    Client *client = NULL;
    int res = 1;
    unsigned char key[IDLEN];
    unsigned char friendid[IDLEN];
    idtokey(id1, key);
    idtokey(id2, friendid);
    
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


/* output the friendlist */
void clientlist_dump(ClientList *list)
{
    Node *x;
    Friend *y;
    printf("\n");
    skiplist_dump(&list->skiplist);
    x = (&(list->skiplist))->header;
    while(x && x->forward[1] != (&(list->skiplist))->header) {
        printf("%d:", ntohl(*((int *)(x->forward[1]->key))));
        y = ((Client *)(x->forward[1]->value))->friends.head->next;
        while(y) {
            printf("%d->", ntohl(*((int *)(y->userid))));
            y = y->next;
        }
        x = x->forward[1];
        printf("NIL\n");
    }
}
