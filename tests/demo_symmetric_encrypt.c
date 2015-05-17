#include "symmetric.h"

/* use aes and ctr mode */

int main()
{
    unsigned char key[16], buffer[512], IV[16] = {0};
    int i, len;
    FILE *fp;
    fp = fopen("test.txt", "wb");
    scanf("%s", key);
    scanf("%s", buffer);
    scanf("%s", IV);
    len = strlen(buffer);
    symmetricEncrypt(key, 16, buffer, len, IV, 16);
    for(i = 0; i < len; i++) {
        printf("%02x ", buffer[i]); 
    }
    printf("\n");
//    symmetricDecrypt(key, 16, buffer, len);
///    printf("%s", buffer);
    fwrite(buffer, 1, len, fp);
    fclose(fp);
    return 0;
}
