/************************************************** 
    >FileName:      msgdispose.c
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    
    >Description:
      * functions attached to analyzing or construct
        messages
***************************************************/

#include <stdio.h>
#include <string.h>

#include "datauct.h"
#include "libcrypt.h"

void Auth_MakeDHMsg(unsigned char *msg, unsigned long *msglen, unsigned char *private, unsigned long prilen)
{
    generateRandom(private, prilen);
    getPublic(msg, msglen, private, prilen);
}

void Auth_ParseDHMsg(unsigned char *msg, unsigned long msglen, unsigned char *private, unsigned long prilen, unsigned char *key, unsigned long *keylen)
{
    getKey(key, keylen, msg, msglen, private, prilen);
}

void Auth_MakeSaltMsg(unsigned char *msg, unsigned long *msglen, unsigned char *salt, unsigned long *saltlen, unsigned char *key, unsigned long keylen)
{
    unsigned char iv[IVLEN];
    generateRandom(iv, IVLEN);
    generateRandom(salt, SALTLEN);

    memcpy(msg, iv, IVLEN);
    memcpy(msg + IVLEN, salt, SALTLEN);

    symmetricEncrypt(key, keylen, msg + IVLEN, SALTLEN, iv, IVLEN);

    *msglen = IVLEN + SALTLEN;
}

int Auth_Verify(unsigned char *msg, unsigned long msglen, unsigned char *salt, unsigned long saltlen, unsigned char *key, unsigned long keylen)
{
    return 0;    
}

void Auth_MakeSuccessMsg(unsigned char *msg, unsigned long *msglen, unsigned char *session, unsigned long slen, unsigned char *key, unsigned long keylen)
{}
