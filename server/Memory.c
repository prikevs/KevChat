#include <memory.h>
#include <sys/types.h>

/* Allocate memory for an array of chars */
char *
alloclate_strmem(int len)
{
    void *tmp;
    int i;
    if (len < 0) {
        fprintf(stderr, "ERROR: Cannot allocate memory because len = %i in allocate_strmem().\n", len);
        exit(EXIT_FAILURE);
    }    

    tmp = (char *)malloc(len * sizeof(char));
    if (tmp == NULL) {
        fprintf(stderr, "ERROR: Cannot allocate memory for array allocate_strmem().\n");
        return NULL; 
    }
    for(i = 0; i < len; i++) {
        tmp[i] = 0;
    }
    return tmp;
}
