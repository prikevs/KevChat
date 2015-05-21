/************************************************** 
    >FileName:      symmetric.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * Package APIs for symmetric functions(AES)
***************************************************/

#include <tomcrypt.h>
#include "libcrypt.h"

/* use aes and ctr mode */

int generateRandom(unsigned char *array, int len)
{
    int err, x;
    prng_state prng;
    if (register_prng(&yarrow_desc) == -1) {
        return ERROR_PRNG;
    }
    if ((err = rng_make_prng(128, find_prng("yarrow"), &prng, NULL)) != CRYPT_OK) {
        return err;
    } 
    x = yarrow_read(array, len, &prng);
    if (x != len) {
        return ERROR_READ_IV;
    }
    sprng_done(&prng);
    return CRYPT_OK;
}

int symmetricEncrypt(unsigned char *key, int keylen, unsigned char *in, unsigned long len, unsigned char *IV, int ivlen)
{
    symmetric_CTR ctr;
    int err;

    /* register aes first */
    
    if ((err = register_cipher(&rijndael_desc)) == -1) {
        return ERROR_REG_AES;
    }
    
    /* start up CTR mode */
    if ((err = ctr_start(
        find_cipher("rijndael"),    /* index of desired cipher */
                             IV,    /* the initial vecoter */ 
                            key,    /* the secret key */
                         keylen,    /* length of secret key */
                              0,
      CTR_COUNTER_LITTLE_ENDIAN,
                           &ctr)
        ) != CRYPT_OK) {
        //printf("%s\n", error_to_string(err));
        return err;
    }
    /*
    printf("from libcrypt: \n");
    for(i = 0; i < 30; i++)
        printf("%02x ", in[i]);
    printf("\n");
    fflush(stdout);
    */
    if ((err = ctr_encrypt(     in, /* plaintext */
                                in, /* ciphertext */
                                   len, /* length of plaintext */
                                  &ctr) /* CTR state */
        ) != CRYPT_OK) {
        return err;
    }

    if ((err = ctr_done(&ctr)) != CRYPT_OK) {
        return err;
    }

    return CRYPT_OK;
}

int symmetricDecrypt(unsigned char *key, int keylen, unsigned char *in, unsigned long len, unsigned char *IV, int ivlen)
{
    symmetric_CTR ctr;
    int err;

    /* register aes first */
    if (register_cipher(&rijndael_desc) == -1) {
        return ERROR_REG_AES;
    }
    
    /* start up CTR mode */
    if ((err = ctr_start(
        find_cipher("rijndael"),    /* index of desired cipher */
                             IV,    /* the initial vecoter */ 
                            key,    /* the secret key */
                         keylen,    /* length of secret key */
                              0,
      CTR_COUNTER_LITTLE_ENDIAN,
                           &ctr)
        ) != CRYPT_OK) {
        return err;
    }

//    if ((err = ctr_setiv( IV, /* the initial IV we gave to ctr_start */
//                    16, /* the IV is 16 bytes long */
//                    &ctr) /* the ctr state we wish to modify */
//        ) != CRYPT_OK) {
//        printf("ctr_setiv error: %s\n", error_to_string(err));
//        return -1;
//    }

    if ((err = ctr_decrypt(     in, /* plaintext */
                                in, /* ciphertext */
                               len, /* length of plaintext */
                              &ctr) /* CTR state */
        ) != CRYPT_OK) {
        return err;
    }
    if ((err = ctr_done(&ctr)) != CRYPT_OK) {
        return err;
    }

    return CRYPT_OK;
}

