#include "libcrypt.h"

int main()
{
    int i;
    unsigned char key[16];
    int err;
    err = generateRandom(key, 16);
    if (err != CRYPT_OK) {
        printf("%s\n", crypt_error_to_string(err));    
    }
    for(i = 0; i < 16; i++)
        printf("%d ", key[i]);
    printf("\n");
    return 0;
}
