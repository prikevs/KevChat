/************************************************** 
    >FileName:      dh.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    2015-5-21 Thu
    >Description:
      * implementation of diffie-hellman with GNUMP
***************************************************/

#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BITS 128
#define K 233
#define BASE 65537

static void getPrime(mpz_t prime, unsigned long bits, unsigned long k)
{
    mpz_t bk;
    mpz_init_set_ui(bk, k);
    mpz_ui_pow_ui(prime, 2, bits);
    mpz_submul_ui(prime, bk, 1);
    mpz_clear(bk);
}

static void getPowm(unsigned char *res, size_t *reslen, const mpz_t base, const unsigned char *chexp, int len)
{
    mpz_t prime;
    mpz_t exp;
    mpz_t ans;

    mpz_init(ans);
    mpz_init(exp);
    mpz_init(prime);

    getPrime(prime, BITS, K);
    mpz_import(exp, len, -1, 1, 1, 0, (void *)chexp); 

    /*
    printf("exp: "); 
    mpz_out_str(stdout, 10, exp);
    printf("\n");

    printf("base: "); 
    mpz_out_str(stdout, 10, base);
    printf("\n");

    printf("prime: "); 
    mpz_out_str(stdout, 10, prime);
    printf("\n");
    */

    mpz_powm_sec(ans, base, exp, prime);

    mpz_export((void *)res, reslen, -1, 1, 1, 0, ans);

    mpz_clear(prime);
    mpz_clear(ans);
    mpz_clear(exp);
}

void getPublic(unsigned char *public, size_t *publen, const unsigned char *private, int prilen)
{
    mpz_t base;
    mpz_init_set_ui(base, BASE);

    getPowm(public, publen, base, private, prilen);

    mpz_clear(base); 
}

void getKey(unsigned char *key, size_t *keylen, const unsigned char *public, int publen, const unsigned char *private, int prilen)
{
    mpz_t base;
    mpz_init(base);

    mpz_import(base, publen, -1, 1, 1, 0, (void *)public);
    getPowm(key, keylen, base, private, prilen);

    mpz_clear(base);
}
