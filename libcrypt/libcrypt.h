#ifndef KEVCRYPT_H
#define KEVCRYPT_H
#include "rsa.h"
#include "symmetric.h"

enum {
    ERROR_DECRYPT = -1,
    ERROR_PRNG = -2,
    ERROR_SHA1 = -3,
    ERROR_YARROW = -4,
    ERROR_READ_IV = -5,
    ERROR_REG_AES = -6
};

const char *crypt_error_to_string(int errno);

#endif
