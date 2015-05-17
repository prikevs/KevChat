#include "symmetric.h"

int main()
{
    int i;
    unsigned char key[16];
    generateRandom(key, 16);
    for(i = 0; i < 16; i++)
        printf("%d ", key[i]);
    printf("\n");
    return 0;
}
