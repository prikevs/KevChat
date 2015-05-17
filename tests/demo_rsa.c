#include "../libcrypt/rsa.h"
#include <stdio.h>

int runRSA(int padding)
{
    int res, i;
    rsa_key key;
    unsigned char buffer[512];
    unsigned long len;
    unsigned long l1, l2;
    unsigned char pt[16], pt2[16], out[1024];
    if ((res = makeRSAKey(&key, 1024, padding)) < 0) {
        printf("Failed to make Key.\n");
        return -1;
    }
    len = sizeof(buffer);
    if ((res = exportRSAKey(&key, buffer, &len, 1)) < 0) {
        printf("Failed to export key.\n");
        return -1;
    }
    for(i = 0; i < len; i++) {
        printf("%02x", buffer[i]);    
    }
    printf("\n");
    l1 = sizeof(out);
    scanf("%s", pt);
    if ((res = RSAEncryptKey(&key, pt, 16, out, &l1, "Test")) < 0) {
        printf("Failed to encrypt.\n");
        return -1;
    }
    l2 = sizeof(pt2);
    if ((res = RSADecryptKey(&key, out, l1, pt2, &l2, "Test")) < 0) {
        printf("Failed to decrypt.\n");
        return -1;
    }
    printf("after: %s\n", pt2);
    return 0; 
}

int main()
{
    runRSA(3);
    return 0;
}
