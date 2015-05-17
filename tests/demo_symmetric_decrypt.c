#include "symmetric.h"

int main()
{
    unsigned char key[16], buffer[512], IV[16] = {0};    
    int i, len;
    FILE *fp;
    memset(buffer, 0, sizeof(buffer));
    fp = fopen("test.txt", "rb");
    scanf("%s", key);
    scanf("%s", IV);
    len = 5;
    fread(buffer, 1, len, fp);
    for(i = 0; i < len; i++) {
        printf("%02x ", buffer[i]);     
    }
    printf("\n");
    fclose(fp);
    symmetricDecrypt(key, 16, buffer, len, IV, 16);
    printf("%s", buffer);
    return 0;
}
