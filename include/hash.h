#ifndef HASH_H
#define HASH_H
int hashSHA1(unsigned char *in, int inlen, unsigned char *out, int *outlen);

int hashHMAC(unsigned char *key, int keylen, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);
#endif
