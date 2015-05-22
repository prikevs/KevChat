/************************************************** 
    >FileName:      hash.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * Package APIs for hash functions
***************************************************/
#ifndef HASH_H
#define HASH_H
int hashSHA1(unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);

int hashHMAC(unsigned char *key, unsigned long keylen, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen);
#endif
