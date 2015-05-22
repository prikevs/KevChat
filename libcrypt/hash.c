/************************************************** 
    >FileName:      hash.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * Package APIs for hash functions
***************************************************/

#include "libcrypt.h"
#include <tomcrypt.h>

/* sha1 hash function */
int hashSHA1(unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen)
{
    hash_state md;
    if (*outlen < 20) {
        return ERROR_HASH_TOO_SHORT; 
    }
    *outlen = 20;
    sha1_init(&md);
    sha1_process(&md, in, inlen);
    sha1_done(&md, out);

    return CRYPT_OK;
}

/* hmac hash function */
int hashHMAC(unsigned char *key, unsigned long keylen, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen)
{
    int idx, err;
    hmac_state hmac;

    /* register SHA-1 */
    if (register_hash(&sha1_desc) == -1) {
        return ERROR_REG_SHA1; 
    }

    /* get index of SHA1 in hash descriptor table */
    idx = find_hash("sha1");

    /* start the HMAC */
    if ((err = hmac_init(&hmac, idx, key, keylen)) != CRYPT_OK) {
        return err; 
    }

    /* process */
    if ((err = hmac_process(&hmac, in, inlen)) != CRYPT_OK) { 
        return err;
    }

    /* get result */
    if ((err = hmac_done(&hmac, out, outlen)) != CRYPT_OK) {
        return err; 
    }

    return CRYPT_OK;
}
