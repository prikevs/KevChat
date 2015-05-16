#ifndef RSA_H
#define RSA_H
#include <tomcrypt.h>
int makeRSAKey(rsa_key *key, int bits, int padding);

int importRSAKey(rsa_key *key, unsigned char *in, unsigned long len);

int exportRSAKey(rsa_key *key, unsigned char *out, unsigned long *len, int ispublic);

int RSAEncryptKey(rsa_key *key, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen, char *lparam);

int RSADecryptKey(rsa_key *key, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen, char *lparam);
#endif
