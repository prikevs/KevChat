#include <stdio.h>
#include <string.h>
#include <gmp.h>

int main()
{
    int i;
    size_t len;
    unsigned char num[16];
    mpz_t integ, k, n;
    mpz_init(integ);
    mpz_init_set_ui(k, 233);
    mpz_init_set_ui(n, 55537);
    mpz_ui_pow_ui(integ, 2, 128);
    mpz_powm(k, integ, n, n);
//    mpz_out_str(stdout, 10, integ);
//    printf("\n");
//    mpz_out_str(stdout, 10, k);
//    printf("\n");


    mpz_init(integ);
    memset(num, 0, sizeof(num));
    num[0] = 1;
    mpz_import(integ, 13, -1, 1, 1, 0, (void *)num);
    mpz_out_str(stdout, 10, integ);
    printf("\n");
//    len = sizeof(num);
//    mpz_export((void *)num, &len, 1, 1, 1, 0, integ);
//    mpz_submul_ui(integ, integ, 2);
//    mpz_out_str(stdout, 10, integ);
//    printf("\n");
//    for(i = 0; i < len; i++)
//        printf("%02x ", num[i]); 
//    printf("\n");
    mpz_clear(integ);
    return 0;
}
