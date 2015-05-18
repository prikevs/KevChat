#include "../server/skiplist.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int arr[] = {3, 6, 8, 2, 5}, i;
    Skiplist list;
    Client *newclient = NULL;
    skiplist_init(&list);
    printf("Insert:----------------\n");  
    for(i = 0; i < sizeof(arr)/sizeof(arr[0]); i++) {
        newclient = (Client *)malloc(sizeof(Client));
        newclient->value = arr[i];
        skiplist_insert(&list, arr[i], (void *)newclient); 
        newclient = NULL;
    }
    skiplist_dump(&list);

    printf("Search:------------------\n");
    int keys[] = {3, 2, 5, 9, 8};
    for(i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        void *value = skiplist_search(&list, keys[i]);
        if (value) {
            printf("key = %d, value = %d\n", keys[i], ((Client *)value)->value); 
        }
        else {
            printf("key = %d, not found\n", keys[i]);
        }
    }
    printf("Delete:---------------------\n");
    skiplist_delete(&list, 3);
    skiplist_delete(&list, 2);
    skiplist_dump(&list);

    return 0;
}
