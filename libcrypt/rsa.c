#define LTM_DESC
#include <tomcrypt.h>
#include "libcrypt.h"


int makeRSAKey(rsa_key *key, int bits, int padding)
{
    int hash_idx;
    int prng_idx;
    int err;
        
    /*register prng*/
    if (register_prng(&sprng_desc) == -1) {
        return ERROR_PRNG;
    }

    /* register a math library (in this case TomsFastMath) */
    ltc_mp = ltm_desc;
    if (register_hash(&sha1_desc) == -1) {
        return ERROR_SHA1;
    }
    hash_idx = find_hash("sha1");
    prng_idx = find_prng("sprng");

    /* make an RSA-bits key */ 
    err = rsa_make_key(NULL,    /* PRNG status */
            prng_idx,           /* PRNG idx */
            bits / 8,           /* bits-bit key */
            65537,              /* we like e=65537 */
            key);               /* where to store the key */
    if (err != CRYPT_OK) {
        return res;
    }

    return CRYPT_OK;
}

int importRSAKey(rsa_key *key, unsigned char *in, unsigned long len)
{
    int err;
    if ((err = rsa_import(in, len, key)) != CRYPT_OK) {
        return err;
    }
    return CRYPT_OK;
}

int exportRSAKey(rsa_key *key, unsigned char *out, unsigned long  *len, int ispublic)
{
    int err;
    int type;
    if (ispublic) {
        type = PK_PUBLIC; 
    }
    else {
        type = PK_PRIVATE; 
    }
    if ((err = rsa_export(out, len, type, key)) != CRYPT_OK) {
        return err;
    }
    return CRYPT_OK;
}

int RSAEncryptKey(rsa_key *key, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen, char *lparam)
{
    int err;
    int hash_idx, prng_idx;
    /* register prng */
    if (register_prng(&sprng_desc) == -1) {
        return ERROR_PRNG;
    }

    /* register a math library (in this case TomsFastMath) */
    ltc_mp = ltm_desc;
    if (register_hash(&sha1_desc) == -1) {
        return ERROR_SHA1;
    }
    hash_idx = find_hash("sha1");
    prng_idx = find_prng("sprng");
    if ((err = rsa_encrypt_key(in,  /* data we wish to encrypt */
                    inlen,          /* data len */
                    out,            /* where to store ciphertext */
                    outlen,         /* length of ciphertext */
                    lparam,         /* lparam for this program */
                    strlen(lparam), /* length of lparam */
                    NULL,           /* PRNG state */
                    prng_idx,       /* prng idx */
                    hash_idx,       /* hash idx */
                    key)           /* RSA key */
        ) != CRYPT_OK) {
        return err;
    }
    return CRYPT_OK;
}

int RSADecryptKey(rsa_key *key, unsigned char *in, unsigned long inlen, unsigned char *out, unsigned long *outlen, char *lparam)
{
    int err, res;
    int hash_idx;
    ltc_mp = ltm_desc;
    if (register_hash(&sha1_desc) == -1) {
        return ERROR_SHA1;
    }
    hash_idx = find_hash("sha1");
    if ((err = rsa_decrypt_key(in,  /* encrypted data */
                    inlen,          /* length of ciphertext */
                    out,            /* where to put plaintext */
                    outlen,         /* plaintext length */
                    lparam,         /* lparams for this program */
                    strlen(lparam), /* lparam's bytes */
                    hash_idx,       /* hash idx */
                    &res,           /* validity of data */
                    key)            /* our RSA key */
        ) != CRYPT_OK) {
        return err;
    }
    if (res != 1) {
        return ERROR_DECRYPT; 
    }
    return CRYPT_OK;
}

