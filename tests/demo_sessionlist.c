#include "session.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    unsigned char arr[][16] = {"3", "6", "8", "2", "5"};
    SessionList list;
    Session *newclient = NULL;
    sessionlist_init(&list, "fffffff");
    int i;
    for(i = 0; i < 5; i++) {
        memset(arr[i], 0, sizeof(arr[i]));    
        arr[i][0] = '0' + i;
    }
    printf("Insert:----------------\n");  
    for(i = 0; i < 5; i++) {
        newclient = (Session *)malloc(sizeof(Session));
        memcpy(newclient->userid, arr[i], 16);
        sessionlist_insert(&list, arr[i], newclient); 
        newclient = NULL;
    }
    sessionlist_dump(&list);

    printf("Search:------------------\n");
    unsigned char keys[][16] = {"3", "2", "5", "9", "8"};
    for(i = 0; i < 5; i++) {
        memset(keys[i], 0, sizeof(keys[i]));    
        keys[i][0] = '0' + i + 1;
    }
    for(i = 0; i < 5; i++) {
        Session *value = sessionlist_search(&list, keys[i]);
        if (value) {
            printf("key = %s, value = %s\n", keys[i], value->userid); 
        }
        else {
            printf("key = %s, not found\n", keys[i]);
        }
    }
    printf("Delete:---------------------\n");
    sessionlist_delete(&list, keys[0]);
    sessionlist_delete(&list, keys[1]);
    sessionlist_dump(&list);

    return 0;
}
