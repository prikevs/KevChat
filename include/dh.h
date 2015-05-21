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

void getPublic(unsigned char *, int *, const unsigned char *, int);
void getKey(unsigned char *, int *, const unsigned char *, int, const unsigned char *, int);

#endif
