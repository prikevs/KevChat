#include "symmetric.h"

/* use aes and ctr mode */

int main()
{
    unsigned char key[16], buffer[512];
    int i, len;
    scanf("%s", key);
    scanf("%s", buffer);
    len = strlen(buffer);
    symmetricEncrypt(key, 16, buffer, strlen(buffer));
    for(i = 0; i < len; i++) {
        printf("%02x ", buffer[i]); 
    }
    printf("\n");
    symmetricDecrypt(key, 16, buffer, strlen(buffer));
    printf("de: %s\n", buffer);
    return 0;
}
