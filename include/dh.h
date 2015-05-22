/************************************************** 
    >FileName:      dh.h
    >Author:        Kevince
    >Email:         prikevs@gmail.com          
    >CreateTime:    2015-5-21 Thu
    >Description:
      * implementation of diffie-hellman with GNUMP
***************************************************/

#ifndef DH_H
#define DH_H

void getPublic(unsigned char *, unsigned long *, const unsigned char *, unsigned long);
void getKey(unsigned char *, unsigned long *, const unsigned char *, unsigned long, const unsigned char *, unsigned long);

#endif
