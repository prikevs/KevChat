#include "../libcrypt/libcrypt.h"

int main()
{
    unsigned char str[10], out[20];
    unsigned char key[16];
    int i, err, len;
    scanf("%s", str);
    len = 20;
    err = hashSHA1(str, strlen(str), out, &len);
    if (err != CRYPT_OK) {
        printf("%s\n", crypt_error_to_string(err));     
        return 0;
    }
    for(i = 0; i < len; i++)
        printf("%02x", out[i]);
    printf("\n");

    generateRandom(key, 16);
    hashHMAC(key, 16, str, strlen(str), out, &len);
    for(i = 0; i < len; i++)
        printf("%02x", out[i]);
    printf("\n");
    return 0;
}
