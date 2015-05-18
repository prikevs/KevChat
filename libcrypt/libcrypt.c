#include "libcrypt.h"
#include <tomcrypt.h>


const char *crypt_error_to_string(int errno)
{
    switch(errno) {
        case ERROR_PRNG: return "Error registering sprng"; 
        case ERROR_SHA1: return "Error registering sha1";
        case ERROR_DECRYPT: return "Failed to decrypt"; 
        case ERROR_YARROW: return "Error registering yarrow PRNG";
        case ERROR_READ_IV: return "Error reading PRNG for IV required";
        case ERROR_REG_AES: return "Error registering cipher.";
        default: return error_to_string(errno);
    }    
    return "";
}
