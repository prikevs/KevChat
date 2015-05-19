#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "datauct.h"

int main()
{
    unsigned char arr[][16] = {"3", "6", "8", "2", "5"};
    ClientList list;
    Client *newclient = NULL;
    clientlist_init(&list);
    int i;
    int res;
    for(i = 0; i < 5; i++) {
        memset(arr[i], 0, sizeof(arr[i]));    
        arr[i][0] = '0' + i;
    }
    printf("Insert:----------------\n");  
    for(i = 0; i < 5; i++) {
        newclient = (Client *)malloc(sizeof(Client));
        memcpy(newclient->userid, arr[i], USERLEN);
        clientlist_insert_client(&list, arr[i], newclient); 
        newclient = NULL;
    }
    clientlist_dump(&list);

    res = clientlist_friend_op(&list, arr[0], arr[1], 1);
    clientlist_dump(&list);

    if (clientlist_isfriend(&list, arr[0], arr[1])) {
        printf("%s, %s is friend\n", arr[0], arr[1]);    
    }
    else {
        printf("%s, %s is not friend\n", arr[0], arr[1]);    
    }

    if (clientlist_isfriend(&list, arr[0], arr[2])) {
        printf("%s, %s is friend\n", arr[0], arr[2]);    
    }
    else {
        printf("%s, %s is not friend\n", arr[0], arr[2]);    
    }
    res = clientlist_friend_op(&list, arr[0], arr[1], 0);
    printf("res=%d\n", res);
    clientlist_dump(&list);
    /*
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
    */

    return 0;
}
