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
    getKey(key, &keylen, msg, msglen, private, prilen);
}
