
/* Attention value free ! */
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "skiplist.h"

#define SKIPLIST_MAX_LEVEL 6

int skiplist_init(Skiplist *list)
{
    int i;

    Node *header = (Node *)malloc(sizeof(Node));    
    if (header == NULL) {
        printf("Error allocate for header.\n");
        return -1;
    }
    
    list->header = header;
    header->key =INT_MAX;

    /* init mutex lock */
    if (pthread_mutex_init(&list->lock, NULL) != 0) {
        printf("Error mutex init failed.\n");     
        return -1;
    }

    header->forward = (Node **)malloc(sizeof(Node) * (SKIPLIST_MAX_LEVEL + 1));
    if (header->forward == NULL) {
        printf("Error alloate for header->forward");    
        return -1;
    }

    for(i = 0; i <= SKIPLIST_MAX_LEVEL; i++) {
        header->forward[i] = list->header;     
    }
    list->level = 1;
    list->size = 0;

    return 0;
}

/* lock the skiplist */
static int skiplist_lock(Skiplist *list)
{
    if (pthread_mutex_lock(&list->lock) != 0) {
        return -1; 
    }
    return 0;
}

/* unlock the skiplist */
static int skiplist_unlock(Skiplist *list)
{
    if (pthread_mutex_unlock(&list->lock) != 0) {
        return -1; 
    }
    return 0;
}

/* get random level 1--1/2 2--1/4 3--1/8 ... */
static int rand_level()
{
    int level = 1;
    while(rand() < RAND_MAX/2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}

int skiplist_insert_raw(Skiplist *list, int key, void *value)
{
    Node *update[SKIPLIST_MAX_LEVEL + 1];
    Node *x = list->header;
    int i, level;
    for(i = list->level; i >= 1; i--) {
        while(x->forward[i]->key < key) {
            x = x->forward[i]; 
        }
        update[i] = x;
    }
    x = x->forward[1];

    if (key == x->key) {
        /* Attention */
        free(x->value);
        x->value = value; 
        return 0;
    }
    else {
        level = rand_level();
        if (level > list->level) {
            for(i = list->level + 1; i <= level; i++) {
                update[i] = list->header; 
            } 
            list->level = level;
        } 

        x = (Node *)malloc(sizeof(Node));
        if (x == NULL) {
            printf("Error skiplist_insert_raw, allocate Node failed.\n");
            return -1;
        }
        x->key = key;
        x->value = value;
        x->forward = (Node **)malloc(sizeof(Node *) * (level + 1));
        for(i = 1; i <= level; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
    return 0;
}

int skiplist_insert(Skiplist *list, int key, void *value)
{
    int res;
    if(skiplist_lock(list) != 0) {
        printf("Error skiplist_insert lock failed.\n");
        return -1; 
    }

    res = skiplist_insert_raw(list, key, value);

    if(skiplist_unlock(list) != 0) {
        printf("Error skiplist_insert unlock failed.\n");
        return -1; 
    }

    return res;
}

void *skiplist_search_raw(Skiplist *list, int key)
{
    Node *x = list->header;
    int i;
    for(i = list->level; i >= 1; i--) {
        while(x->forward[i]->key < key) {
            x = x->forward[i]; 
        } 
    }
    if (x->forward[1]->key == key) {
        return x->forward[1]->value; 
    }
    return NULL;
}

void *skiplist_search(Skiplist *list, int key)
{
    void *res = NULL;
    if(skiplist_lock(list) != 0) {
        printf("Error skiplist_search lock failed.\n");
        return NULL;
    }

    res = skiplist_search_raw(list, key);

    if(skiplist_unlock(list) != 0) {
        printf("Error skiplist_search unlock failed.\n");
        return NULL; 
    }
    return res; 
}


/* Attention!! free value */
static void skiplist_node_free(Node *x)
{
    if (x) {
        free(x->forward);
        free(x->value);
        free(x);
    }
}

int skiplist_delete_raw(Skiplist *list, int key)
{
    int i;
    Node *update[SKIPLIST_MAX_LEVEL + 1];
    Node *x = list->header;
    for(i = list->level; i >= 1; i--) {
        while(x->forward[i]->key < key) {
            x = x->forward[i]; 
        }
        update[i] = x;
    }

    x = x->forward[1];
    if (x->key == key) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->forward[i] != x) {
                break;
            }
            update[i]->forward[i] = x->forward[i];
        } 
        skiplist_node_free(x);

        while(list->level > 1 && list->header->forward[list->level] == list->header)
            list->level--;
        return 0;
    }
    return 1;
}

int skiplist_delete(Skiplist *list, int key)
{
    int res;
    if(skiplist_lock(list) != 0) {
        printf("Error skiplist_delete lock failed.\n");
        return -1; 
    }

    res = skiplist_delete_raw(list, key);

    if(skiplist_unlock(list) != 0) {
        printf("Error skiplist_delete unlock failed.\n");
        return -1; 
    }
    return res; 
}

void skiplist_dump(Skiplist *list)
{
    int i;
    Node *x;
    for(i = list->level; i >= 1; i--) {
        x = list->header;
        while(x && x->forward[i] != list->header) {
            printf("%d->", x->forward[i]->key);
            x = x->forward[i];
        }
        printf("NIL\n");
    }
}
