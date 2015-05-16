#ifndef SYM_H
#define SYM_H
#include <tomcrypt.h>
int generateKey(unsigned char *key, int keylen);

int symmetricEncrypt(unsigned char *key, int keylen, unsigned char *buffer, unsigned long len);

int symmetricDecrypt(unsigned char *key, int keylen, unsigned char *buffer, unsigned long len);

#endif
