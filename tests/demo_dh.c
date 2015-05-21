//gcc -o demo_dh demo_dh.c ../libcrypt/libcrypt.c ../libcrypt/dh.c ../libcrypt/symmetric.c -I../include -ltomcrypt -lgmp -Wall

#include "libcrypt.h"
#include <stdio.h>
#include <string.h>

void dump(const unsigned char *p)
{
    int i;
    for(i = 0; i < 16; i++)
        printf("%02x ", p[i]);
    printf("\n");
}

int main()
{
    unsigned char rand_a[16], rand_b[16];
    unsigned char pub_a[16], pub_b[16];
    unsigned char key_a[16], key_b[16];
    int lenpub_a, lenpub_b, lenkey_a, lenkey_b;
    lenpub_a = lenpub_b = lenkey_a = lenkey_b = 16;

    memset(pub_a, 0, sizeof(pub_a));
    memset(pub_b, 0, sizeof(pub_b));

    generateRandom(rand_a, 16);
    generateRandom(rand_b, 16);
    printf("rand_a: ");
    dump(rand_a);
    printf("rand_b: ");
    dump(rand_b);
    
    getPublic(pub_a, &lenpub_a, rand_a, 16);
    getPublic(pub_b, &lenpub_b, rand_b, 16);
    printf("lenpub_a=%d\n", lenpub_a);
    printf("lenpub_b=%d\n", lenpub_b);
    printf("pub_a: ");
    dump(pub_a);
    printf("pub_b: ");
    dump(pub_b);

    getKey(key_a, &lenkey_a, pub_b, lenpub_b, rand_a, 16);
    getKey(key_b, &lenkey_b, pub_a, lenpub_a, rand_b, 16);
    
    dump(key_a);
    dump(key_b);

    return 0;
}
