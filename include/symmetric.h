/************************************************** 
    >FileName:      symmetric.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * Package APIs for symmetric functions(AES)
***************************************************/

#ifndef SYM_H
#define SYM_H
#include <tomcrypt.h>
int generateRandom(unsigned char *array, int len);

int symmetricEncrypt(unsigned char *key, int keylen, unsigned char *in, unsigned long len, unsigned char *IV, int ivlen);

int symmetricDecrypt(unsigned char *key, int keylen, unsigned char *buffer, unsigned long len, unsigned char *IV, int ivlen);

#endif
