/************************************************** 
    >FileName:      skiplist.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * skiplist, a data structure 
***************************************************/

/* Attention value free ! */
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "skiplist.h"

//#define DEMO
#define SKIPLIST_MAX_LEVEL 6

int skiplist_init(SkipList *list, unsigned char *keymax)
{
    int i;

    Node *header = (Node *)malloc(sizeof(Node));    
    if (header == NULL) {
        printf("Error allocate for header.\n");
        return -1;
    }
    
    list->header = header;
    header->key = keymax;

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
void skiplist_lock(SkipList *list)
{
    pthread_mutex_lock(&list->lock);
}

/* unlock the skiplist */
void skiplist_unlock(SkipList *list)
{
    pthread_mutex_unlock(&list->lock);
}

/* get random level 1--1/2 2--1/4 3--1/8 ... */
static int rand_level()
{
    int level = 1;
    while(rand() < RAND_MAX/2 && level < SKIPLIST_MAX_LEVEL)
        level++;
    return level;
}

int skiplist_insert_raw(SkipList *list, unsigned char *key, void *value, int(* cmp)(unsigned char *, unsigned char *))
{
    Node *update[SKIPLIST_MAX_LEVEL + 1];
    Node *x = list->header;
    int i, level;
    for(i = list->level; i >= 1; i--) {
        while(cmp(x->forward[i]->key, key) < 0) {
            x = x->forward[i]; 
        }
        update[i] = x;
    }
    x = x->forward[1];

    if (cmp(key, x->key) == 0) {
        return 1; 
    } 
    /*
    if (key == x->key) {
        free(x->value);
        x->value = value; 
        return 0;
    }
    */
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


void *skiplist_search_raw(SkipList *list, unsigned char *key, int(* cmp)(unsigned char *, unsigned char *))
{
    Node *x = list->header;
    int i;
    for(i = list->level; i >= 1; i--) {
        while(cmp(x->forward[i]->key, key) < 0) {
            x = x->forward[i]; 
        } 
    }
    if (cmp(x->forward[1]->key, key) == 0) {
        return x->forward[1]->value; 
    }
    return NULL;
}



/* Attention!! free value */
static void skiplist_node_free(Node *x, void(* del)(void *))
{
    if (x) {
        free(x->forward);
#ifndef DEMO
        free(x->key);
#endif
        del(x->value);
        free(x);
    }
}

int skiplist_delete_raw(SkipList *list, unsigned char *key, int(* cmp)(unsigned char *, unsigned char *), void(* del)(void *))
{
    int i;
    Node *update[SKIPLIST_MAX_LEVEL + 1];
    Node *x = list->header;
    for(i = list->level; i >= 1; i--) {
        while(cmp(x->forward[i]->key, key) < 0) {
            x = x->forward[i]; 
        }
        update[i] = x;
    }

    x = x->forward[1];
    if (cmp(x->key, key) == 0) {
        for (i = 1; i <= list->level; i++) {
            if (update[i]->forward[i] != x) {
                break;
            }
            update[i]->forward[i] = x->forward[i];
        } 
        skiplist_node_free(x, del);

        while(list->level > 1 && list->header->forward[list->level] == list->header)
            list->level--;
        return 0;
    }
    return 1;
}


void skiplist_dump(SkipList *list)
{
    int i;
    Node *x;
    for(i = list->level; i >= 1; i--) {
        x = list->header;
        while(x && x->forward[i] != list->header) {
            printf("%s->", x->forward[i]->key);
            x = x->forward[i];
        }
        printf("NIL\n");
    }
}
